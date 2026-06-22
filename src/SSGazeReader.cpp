/*
   SSGazeReader.cpp: direct HID eye-tracker reader (process-wide singleton).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSGazeReader.h"
#include "utils.h"        // GetAppDataCustomFolder, GetProductVersionString, GetISODateString
#include "SSGazeDetect.h" // DetectEyeTracking() for the diagnostic dump
#include <setupapi.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <vector>
#include <fstream>
#include <cstdio>
#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")

// Set to 1 in a test build to feed the cursor as synthetic HID gaze so the
// HidDwell path can be exercised without a HID-capable tracker. MUST stay 0 in
// shipping builds (it would make the detector believe HID is always live).
#define SSGAZE_DEBUG_FEED_CURSOR 0

#define HID_USAGE_PAGE_EYE_HEAD_TRACKER 0x0012
#define HID_USAGE_EYE_TRACKER           0x0001

namespace
{
  constexpr DWORD kMaxAgeMs = 200;   // sample older than this => not "live"
  constexpr DWORD kRetryMs  = 2000;  // re-attempt device open on this cadence

  // Gaze-position usages on the Eye/Head Tracker page. Confirmed from real
  // Irisbond Hiru HID dumps (2026-06): the gaze point is a pair of 32-bit signed
  // values in MICROMETERS on the screen plane, origin at the display's top-left.
  // A clean corner sweep showed usage 0x21 spanning 0..260000 µm (= the 260 mm
  // screen WIDTH) and 0x22 staying within the 173 mm height, so 0x21 = X and
  // 0x22 = Y. Usage 0x23 is Z (always 0 here). If neither X/Y usage is present
  // we fall back to the first two value usages on the page; failing that we idle.
  constexpr USAGE kGazeUsageX = 0x0021;
  constexpr USAGE kGazeUsageY = 0x0022;

  struct DeviceCtx
  {
    HANDLE               h = INVALID_HANDLE_VALUE;
    PHIDP_PREPARSED_DATA pp = nullptr;
    USHORT               reportLen = 0;
    USAGE                usageX = 0, usageY = 0;
    int                  scrWum = 0, scrHum = 0; // display physical size, micrometers
    int                  scrWpx = 0, scrHpx = 0; // display size, pixels (primary)
    bool                 haveFields = false;
  };

  void CloseDevice( DeviceCtx & d )
  {
    if( d.pp ) { HidD_FreePreparsedData( d.pp ); d.pp = nullptr; }
    if( d.h != INVALID_HANDLE_VALUE ) { CloseHandle( d.h ); d.h = INVALID_HANDLE_VALUE; }
    d.haveFields = false;
  }

  // Caches the display's physical size (micrometers) and pixel size, used to
  // turn micrometer gaze coordinates into screen pixels.
  void CacheScreenGeometry( DeviceCtx & d )
  {
    HDC hdc = GetDC( nullptr );
    if( hdc )
    {
      d.scrWum = GetDeviceCaps( hdc, HORZSIZE ) * 1000; // mm -> µm
      d.scrHum = GetDeviceCaps( hdc, VERTSIZE ) * 1000;
      ReleaseDC( nullptr, hdc );
    }
    d.scrWpx = GetSystemMetrics( SM_CXSCREEN );
    d.scrHpx = GetSystemMetrics( SM_CYSCREEN );
  }

  // Picks the gaze X/Y value caps from the input report. Prefers the confirmed
  // usages; otherwise falls back to the first two value usages on the page.
  void SelectGazeFields( DeviceCtx & d )
  {
    HIDP_CAPS caps = {};
    if( HidP_GetCaps( d.pp, &caps ) != HIDP_STATUS_SUCCESS ) return;
    d.reportLen = caps.InputReportByteLength;
    CacheScreenGeometry( d );
    if( caps.NumberInputValueCaps == 0 ) return;

    USHORT n = caps.NumberInputValueCaps;
    std::vector<HIDP_VALUE_CAPS> vc( n );
    if( HidP_GetValueCaps( HidP_Input, vc.data(), &n, d.pp ) != HIDP_STATUS_SUCCESS ) return;

    USAGE firstPage[2] = { 0, 0 };
    int firstCount = 0;
    bool haveX = false, haveY = false;

    for( USHORT i = 0; i < n; ++i )
    {
      const HIDP_VALUE_CAPS & c = vc[i];
      if( c.UsagePage != HID_USAGE_PAGE_EYE_HEAD_TRACKER || c.IsRange ) continue;
      USAGE u = c.NotRange.Usage;
      if( u == kGazeUsageX ) { d.usageX = u; haveX = true; }
      else if( u == kGazeUsageY ) { d.usageY = u; haveY = true; }
      if( firstCount < 2 ) firstPage[firstCount++] = u;
    }

    if( !haveX || !haveY ) // fall back to the first two value usages on the page
    {
      if( firstCount >= 2 ) { d.usageX = firstPage[0]; d.usageY = firstPage[1]; haveX = haveY = true; }
    }

    d.haveFields = haveX && haveY;
  }

  bool OpenDevice( DeviceCtx & d )
  {
    GUID hidGuid;
    HidD_GetHidGuid( &hidGuid );
    HDEVINFO devInfo = SetupDiGetClassDevs( &hidGuid, nullptr, nullptr,
      DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );
    if( devInfo == INVALID_HANDLE_VALUE ) return false;

    SP_DEVICE_INTERFACE_DATA ifData = {};
    ifData.cbSize = sizeof( ifData );

    for( DWORD i = 0; SetupDiEnumDeviceInterfaces( devInfo, nullptr, &hidGuid, i, &ifData ); ++i )
    {
      DWORD reqSize = 0;
      SetupDiGetDeviceInterfaceDetail( devInfo, &ifData, nullptr, 0, &reqSize, nullptr );
      if( reqSize == 0 ) continue;
      std::vector<BYTE> buffer( reqSize );
      auto detail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA *>( buffer.data() );
      detail->cbSize = sizeof( SP_DEVICE_INTERFACE_DETAIL_DATA );
      if( !SetupDiGetDeviceInterfaceDetail( devInfo, &ifData, detail, reqSize, nullptr, nullptr ) )
        continue;

      // Non-exclusive read access + overlapped so the read can be cancelled.
      HANDLE h = CreateFile( detail->DevicePath, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED, nullptr );
      if( h == INVALID_HANDLE_VALUE ) continue;

      PHIDP_PREPARSED_DATA pp = nullptr;
      bool isTracker = false;
      if( HidD_GetPreparsedData( h, &pp ) )
      {
        HIDP_CAPS caps = {};
        if( HidP_GetCaps( pp, &caps ) == HIDP_STATUS_SUCCESS &&
          caps.UsagePage == HID_USAGE_PAGE_EYE_HEAD_TRACKER &&
          caps.Usage == HID_USAGE_EYE_TRACKER )
          isTracker = true;
      }

      if( isTracker )
      {
        d.h = h;
        d.pp = pp;
        SelectGazeFields( d );
        SetupDiDestroyDeviceInfoList( devInfo );
        return true;
      }

      if( pp ) HidD_FreePreparsedData( pp );
      CloseHandle( h );
    }

    SetupDiDestroyDeviceInfoList( devInfo );
    return false;
  }

  // Maps a gaze coordinate in micrometers (from the display's top-left) onto a
  // pixel on that axis, using the display's physical size so the result is
  // resolution-independent. Clamped to the screen.
  LONG MapGazeUmToPx( LONG um, int screenUm, int screenPx )
  {
    if( screenUm <= 0 || screenPx <= 0 ) return 0;
    double t = double( um ) / double( screenUm );
    if( t < 0.0 ) t = 0.0; else if( t > 1.0 ) t = 1.0;
    return (LONG) ( t * screenPx );
  }

  // --- Diagnostic-dump helpers ---------------------------------------------

  // UTF-8 narrowing so wide device/OS strings can go into the narrow text file.
  std::string Narrow( const std::wstring & w )
  {
    if( w.empty() ) return std::string();
    int n = WideCharToMultiByte( CP_UTF8, 0, w.c_str(), (int) w.size(), nullptr, 0, nullptr, nullptr );
    std::string s( n > 0 ? n : 0, '\0' );
    if( n > 0 ) WideCharToMultiByte( CP_UTF8, 0, w.c_str(), (int) w.size(), &s[0], n, nullptr, nullptr );
    return s;
  }

  std::wstring ReadRegStr( HKEY root, const wchar_t * sub, const wchar_t * value )
  {
    wchar_t buf[256] = {};
    DWORD sz = sizeof( buf );
    if( RegGetValueW( root, sub, value, RRF_RT_REG_SZ, nullptr, buf, &sz ) == ERROR_SUCCESS )
      return buf;
    return L"";
  }

  const char * DpiAwarenessName()
  {
    switch( GetAwarenessFromDpiAwarenessContext( GetThreadDpiAwarenessContext() ) )
    {
      case DPI_AWARENESS_UNAWARE:           return "unaware";
      case DPI_AWARENESS_SYSTEM_AWARE:      return "system";
      case DPI_AWARENESS_PER_MONITOR_AWARE: return "per-monitor";
      default:                              return "unknown";
    }
  }

  // OS, display(s), DPI — so a report from another machine is self-describing.
  void DumpSystemInfo( std::ofstream & f )
  {
    f << "=== SimonSays eye-tracking diagnostic ===\n";
    f << "App version: " << Narrow( GetProductVersionString() ) << "\n";
    f << "Date: " << Narrow( GetISODateString() ) << "\n";

    const wchar_t * cv = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
    std::wstring osName = ReadRegStr( HKEY_LOCAL_MACHINE, cv, L"ProductName" );
    std::wstring osDisp = ReadRegStr( HKEY_LOCAL_MACHINE, cv, L"DisplayVersion" );
    std::wstring osBuild = ReadRegStr( HKEY_LOCAL_MACHINE, cv, L"CurrentBuild" );
    f << "OS: " << Narrow( osName );
    if( !osDisp.empty() ) f << " " << Narrow( osDisp );
    if( !osBuild.empty() ) f << " (build " << Narrow( osBuild ) << ")";
    f << "\n";

    HDC hdc = GetDC( nullptr );
    int dpiX = hdc ? GetDeviceCaps( hdc, LOGPIXELSX ) : 0;
    int dpiY = hdc ? GetDeviceCaps( hdc, LOGPIXELSY ) : 0;
    int wmm = hdc ? GetDeviceCaps( hdc, HORZSIZE ) : 0;
    int hmm = hdc ? GetDeviceCaps( hdc, VERTSIZE ) : 0;
    if( hdc ) ReleaseDC( nullptr, hdc );

    f << "Monitors: " << GetSystemMetrics( SM_CMONITORS ) << "\n";
    f << "Primary screen: " << GetSystemMetrics( SM_CXSCREEN ) << "x" << GetSystemMetrics( SM_CYSCREEN )
      << " px, physical " << wmm << "x" << hmm << " mm\n";
    f << "Virtual desktop: " << GetSystemMetrics( SM_CXVIRTUALSCREEN ) << "x" << GetSystemMetrics( SM_CYVIRTUALSCREEN )
      << " px at (" << GetSystemMetrics( SM_XVIRTUALSCREEN ) << "," << GetSystemMetrics( SM_YVIRTUALSCREEN ) << ")\n";
    f << "Reported DPI: " << dpiX << "x" << dpiY << " (process DPI awareness: " << DpiAwarenessName() << ")\n";

    std::wstring monitors;
    EnumDisplayMonitors( nullptr, nullptr,
      []( HMONITOR hm, HDC, LPRECT, LPARAM lp ) -> BOOL
      {
        auto * out = reinterpret_cast<std::wstring *>( lp );
        MONITORINFO mi = { sizeof( mi ) };
        if( GetMonitorInfo( hm, &mi ) )
        {
          wchar_t line[160];
          swprintf_s( line, L"  %dx%d at (%d,%d)%s\n",
            (int) ( mi.rcMonitor.right - mi.rcMonitor.left ), (int) ( mi.rcMonitor.bottom - mi.rcMonitor.top ),
            (int) mi.rcMonitor.left, (int) mi.rcMonitor.top,
            ( mi.dwFlags & MONITORINFOF_PRIMARY ) ? L" [primary]" : L"" );
          *out += line;
        }
        return TRUE;
      }, reinterpret_cast<LPARAM>( &monitors ) );
    if( !monitors.empty() ) f << Narrow( monitors );
    f << "\n";
  }

  // Passive detection: HID present, eye-control software running, Windows Eye Control.
  void DumpDetectedTracking( std::ofstream & f )
  {
    EyeTrackingStatus st = DetectEyeTracking();
    f << "--- Passive detection (SSGazeDetect) ---\n";
    f << "HID eye tracker present: " << ( st.hidDevicePresent ? "yes" : "no" ) << "\n";
    f << "Eye-control software running: "
      << ( st.externalToolRunning ? Narrow( st.activeToolName ) : std::string( "none" ) )
      << ( st.externalToolRunning && st.toolKnownToClick ? " (issues its own clicks)" : "" ) << "\n";
    f << "Windows Eye Control enabled: " << ( st.windowsEyeControl ? "yes" : "no" ) << "\n\n";
  }

  // Lists every HID eye-tracker device (usage 0x12/0x01) with VID/PID + strings.
  void DumpEyeTrackerDevices( std::ofstream & f )
  {
    f << "--- HID eye-tracker devices (usage 0x12/0x01) ---\n";
    GUID hidGuid;
    HidD_GetHidGuid( &hidGuid );
    HDEVINFO devInfo = SetupDiGetClassDevs( &hidGuid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );
    if( devInfo == INVALID_HANDLE_VALUE ) { f << "  (enumeration failed)\n\n"; return; }

    SP_DEVICE_INTERFACE_DATA ifData = {};
    ifData.cbSize = sizeof( ifData );
    int count = 0;
    for( DWORD i = 0; SetupDiEnumDeviceInterfaces( devInfo, nullptr, &hidGuid, i, &ifData ); ++i )
    {
      DWORD reqSize = 0;
      SetupDiGetDeviceInterfaceDetail( devInfo, &ifData, nullptr, 0, &reqSize, nullptr );
      if( reqSize == 0 ) continue;
      std::vector<BYTE> buffer( reqSize );
      auto detail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA *>( buffer.data() );
      detail->cbSize = sizeof( SP_DEVICE_INTERFACE_DETAIL_DATA );
      if( !SetupDiGetDeviceInterfaceDetail( devInfo, &ifData, detail, reqSize, nullptr, nullptr ) ) continue;

      HANDLE h = CreateFile( detail->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr );
      if( h == INVALID_HANDLE_VALUE ) continue;

      PHIDP_PREPARSED_DATA pp = nullptr;
      bool isTracker = false;
      HIDP_CAPS caps = {};
      if( HidD_GetPreparsedData( h, &pp ) )
      {
        if( HidP_GetCaps( pp, &caps ) == HIDP_STATUS_SUCCESS &&
          caps.UsagePage == HID_USAGE_PAGE_EYE_HEAD_TRACKER && caps.Usage == HID_USAGE_EYE_TRACKER )
          isTracker = true;
        HidD_FreePreparsedData( pp );
      }

      if( isTracker )
      {
        ++count;
        HIDD_ATTRIBUTES attr = { sizeof( attr ) };
        HidD_GetAttributes( h, &attr );
        wchar_t mfg[128] = {}, prod[128] = {};
        HidD_GetManufacturerString( h, mfg, sizeof( mfg ) );
        HidD_GetProductString( h, prod, sizeof( prod ) );
        char ids[96];
        sprintf_s( ids, "  [%d] VID=0x%04X PID=0x%04X ver=0x%04X\n", count,
          attr.VendorID, attr.ProductID, attr.VersionNumber );
        f << ids;
        f << "      Manufacturer: " << Narrow( mfg ) << "\n";
        f << "      Product: " << Narrow( prod ) << "\n";
        f << "      Reports (bytes): in=" << caps.InputReportByteLength
          << " out=" << caps.OutputReportByteLength << " feature=" << caps.FeatureReportByteLength << "\n";
      }
      CloseHandle( h );
    }
    SetupDiDestroyDeviceInfoList( devInfo );
    if( count == 0 ) f << "  (none found)\n";
    f << "\n";
  }

  // Diagnostic: dumps the environment (OS/display/DPI, detected trackers and
  // software) plus the eye-tracker's HID capabilities, value caps, a burst of
  // decoded input reports, and the observed gaze range, to
  // %LocalAppData%\SimonSays\hid_dump.txt. Gated by the SIMONSAYS_HID_DUMP env
  // var so it never runs in normal use. Lets a tester capture everything needed
  // to bring up a new eye-tracking device on someone else's computer.
  void DumpHidDiagnostics()
  {
    std::wstring folder = GetAppDataCustomFolder( APP_NAME );
    if( !folder.empty() ) CreateDirectoryW( folder.c_str(), nullptr ); // ensure it exists
    std::wstring path = folder.empty() ? L"hid_dump.txt" : ( folder + L"\\hid_dump.txt" );
    std::ofstream f( path.c_str() );
    if( !f.is_open() ) f.open( "hid_dump.txt" );
    if( !f.is_open() ) return;

    // Environment report first, so it's captured even if no device opens.
    DumpSystemInfo( f );
    DumpDetectedTracking( f );
    DumpEyeTrackerDevices( f );

    DeviceCtx d;
    if( !OpenDevice( d ) )
    {
      f << "No HID eye-tracker (usage page 0x12 / usage 0x01) could be opened for capture.\n";
      return;
    }

    f << "--- Capture device ---\n";

    HIDP_CAPS caps = {};
    HidP_GetCaps( d.pp, &caps );
    f << "HID eye-tracker opened.\n"
      << "UsagePage=0x" << std::hex << caps.UsagePage << " Usage=0x" << caps.Usage << std::dec << "\n"
      << "InputReportByteLength=" << caps.InputReportByteLength
      << " OutputReportByteLength=" << caps.OutputReportByteLength
      << " FeatureReportByteLength=" << caps.FeatureReportByteLength << "\n"
      << "NumberInputValueCaps=" << caps.NumberInputValueCaps
      << " NumberFeatureValueCaps=" << caps.NumberFeatureValueCaps << "\n\n";

    auto dumpValueCaps = [&]( HIDP_REPORT_TYPE rt, const char * label, USHORT count )
      {
        if( count == 0 ) return;
        std::vector<HIDP_VALUE_CAPS> vc( count );
        USHORT n = count;
        if( HidP_GetValueCaps( rt, vc.data(), &n, d.pp ) != HIDP_STATUS_SUCCESS ) return;
        f << label << " value caps (" << n << "):\n";
        for( USHORT i = 0; i < n; ++i )
        {
          const HIDP_VALUE_CAPS & c = vc[i];
          f << "  reportId=" << (int) c.ReportID << " page=0x" << std::hex << c.UsagePage << std::dec;
          if( c.IsRange )
            f << " usageMin=0x" << std::hex << c.Range.UsageMin << " usageMax=0x" << c.Range.UsageMax << std::dec;
          else
            f << " usage=0x" << std::hex << c.NotRange.Usage << std::dec;
          f << " logMin=" << c.LogicalMin << " logMax=" << c.LogicalMax
            << " bits=" << c.BitSize << " count=" << c.ReportCount << "\n";
        }
        f << "\n";
      };
    dumpValueCaps( HidP_Input, "INPUT", caps.NumberInputValueCaps );
    dumpValueCaps( HidP_Feature, "FEATURE", caps.NumberFeatureValueCaps );

    f << "Display: physical " << ( d.scrWum / 1000 ) << "x" << ( d.scrHum / 1000 ) << " mm, "
      << d.scrWpx << "x" << d.scrHpx << " px (primary).\n"
      << "Mapping assumes X=usage 0x" << std::hex << kGazeUsageX
      << " Y=usage 0x" << kGazeUsageY << std::dec << ", origin top-left.\n\n";

    // Page-0x12 input value usages, decoded per report below.
    std::vector<USAGE> pageUsages;
    if( caps.NumberInputValueCaps )
    {
      std::vector<HIDP_VALUE_CAPS> vc( caps.NumberInputValueCaps );
      USHORT n = caps.NumberInputValueCaps;
      if( HidP_GetValueCaps( HidP_Input, vc.data(), &n, d.pp ) == HIDP_STATUS_SUCCESS )
        for( USHORT i = 0; i < n; ++i )
          if( vc[i].UsagePage == HID_USAGE_PAGE_EYE_HEAD_TRACKER && !vc[i].IsRange )
            pageUsages.push_back( vc[i].NotRange.Usage );
    }

    f << "\n--- Gaze report capture (look slowly around the four screen corners) ---\n";
    std::vector<BYTE> report( caps.InputReportByteLength ? caps.InputReportByteLength : 64 );
    OVERLAPPED ov = {};
    ov.hEvent = CreateEvent( nullptr, FALSE, FALSE, nullptr );
    int  captured = 0, timeouts = 0, validGaze = 0;
    LONG rxMin = 0, rxMax = 0, ryMin = 0, ryMax = 0;     // raw micrometers, valid samples
    LONG pxMinX = 0, pxMaxX = 0, pxMinY = 0, pxMaxY = 0; // mapped pixels, valid samples
    for( int i = 0; i < 240 && captured < 120 && timeouts < 10; ++i )
    {
      ResetEvent( ov.hEvent );
      DWORD bytesRead = 0;
      BOOL ok = ReadFile( d.h, report.data(), (DWORD) report.size(), &bytesRead, &ov );
      if( !ok && GetLastError() == ERROR_IO_PENDING )
      {
        if( WaitForSingleObject( ov.hEvent, 400 ) != WAIT_OBJECT_0 ) { CancelIo( d.h ); ++timeouts; continue; }
        ok = GetOverlappedResult( d.h, &ov, &bytesRead, FALSE );
      }
      if( !ok ) { ++timeouts; continue; }

      ++captured;
      f << "#" << captured << " (" << bytesRead << "B):";
      for( DWORD b = 0; b < bytesRead && b < 64; ++b )
      {
        char hex[8];
        sprintf_s( hex, " %02X", report[b] );
        f << hex;
      }
      for( USAGE u : pageUsages )
      {
        LONG sv = 0; ULONG uv = 0;
        if( HidP_GetScaledUsageValue( HidP_Input, HID_USAGE_PAGE_EYE_HEAD_TRACKER, 0, u, &sv, d.pp, (PCHAR) report.data(), (ULONG) report.size() ) == HIDP_STATUS_SUCCESS )
          f << "  0x" << std::hex << u << std::dec << "=" << sv;
        else if( HidP_GetUsageValue( HidP_Input, HID_USAGE_PAGE_EYE_HEAD_TRACKER, 0, u, &uv, d.pp, (PCHAR) report.data(), (ULONG) report.size() ) == HIDP_STATUS_SUCCESS )
          f << "  0x" << std::hex << u << std::dec << "=" << uv << "(u)";
      }

      // Mapped screen pixel under the current assumption + running gaze range.
      LONG gx = 0, gy = 0;
      if( HidP_GetScaledUsageValue( HidP_Input, HID_USAGE_PAGE_EYE_HEAD_TRACKER, 0, kGazeUsageX, &gx, d.pp, (PCHAR) report.data(), (ULONG) report.size() ) == HIDP_STATUS_SUCCESS &&
        HidP_GetScaledUsageValue( HidP_Input, HID_USAGE_PAGE_EYE_HEAD_TRACKER, 0, kGazeUsageY, &gy, d.pp, (PCHAR) report.data(), (ULONG) report.size() ) == HIDP_STATUS_SUCCESS )
      {
        LONG mx = MapGazeUmToPx( gx, d.scrWum, d.scrWpx );
        LONG my = MapGazeUmToPx( gy, d.scrHum, d.scrHpx );
        f << "  -> px(" << mx << "," << my << ")";
        if( gx != 0 || gy != 0 ) // skip the (0,0) "no gaze" sentinel
        {
          if( validGaze == 0 ) { rxMin = rxMax = gx; ryMin = ryMax = gy; pxMinX = pxMaxX = mx; pxMinY = pxMaxY = my; }
          else
          {
            if( gx < rxMin ) rxMin = gx;  if( gx > rxMax ) rxMax = gx;
            if( gy < ryMin ) ryMin = gy;  if( gy > ryMax ) ryMax = gy;
            if( mx < pxMinX ) pxMinX = mx; if( mx > pxMaxX ) pxMaxX = mx;
            if( my < pxMinY ) pxMinY = my; if( my > pxMaxY ) pxMaxY = my;
          }
          ++validGaze;
        }
      }
      f << "\n";
    }
    if( ov.hEvent ) CloseHandle( ov.hEvent );

    f << "\n--- Capture summary ---\n";
    f << "Reports captured: " << captured << " (valid gaze, non-zero: " << validGaze << ")\n";
    if( validGaze > 0 )
    {
      f << "Raw gaze range (micrometers): X[" << rxMin << ".." << rxMax << "] Y[" << ryMin << ".." << ryMax << "]\n";
      f << "Mapped pixel range: X[" << pxMinX << ".." << pxMaxX << "] Y[" << pxMinY << ".." << pxMaxY << "]\n";
      f << "(A full corner sweep should give X about 0.." << d.scrWpx << " and Y about 0.." << d.scrHpx << ".)\n";
    }
    else if( captured > 0 )
      f << "Reports arrived but all gaze values were zero — tracking lost, eyes off-screen, or HID gaze not actually streaming.\n";
    else
      f << "No input reports received — HID gaze streaming is likely OFF (enable HID mode in the tracker tool).\n";

    CloseDevice( d );
  }
}

SSGazeReader & SSGazeReader::Instance()
{
  static SSGazeReader s_reader;
  return s_reader;
}

SSGazeReader::~SSGazeReader()
{
  Stop();
}

bool SSGazeReader::Start()
{
  if( m_running.exchange( true ) ) return true; // already running

  m_stopEvent = CreateEvent( nullptr, TRUE, FALSE, nullptr );  // manual reset
  m_wakeEvent = CreateEvent( nullptr, FALSE, FALSE, nullptr ); // auto reset
  m_thread = std::thread( &SSGazeReader::ReadLoop, this );
  return true;
}

void SSGazeReader::Stop()
{
  if( !m_running.exchange( false ) )
  {
    // Not running, but make sure any half-created handles are gone.
    if( m_stopEvent ) { CloseHandle( m_stopEvent ); m_stopEvent = nullptr; }
    if( m_wakeEvent ) { CloseHandle( m_wakeEvent ); m_wakeEvent = nullptr; }
    return;
  }

  if( m_stopEvent ) SetEvent( m_stopEvent );
  if( m_thread.joinable() ) m_thread.join();

  if( m_stopEvent ) { CloseHandle( m_stopEvent ); m_stopEvent = nullptr; }
  if( m_wakeEvent ) { CloseHandle( m_wakeEvent ); m_wakeEvent = nullptr; }

  std::lock_guard<std::mutex> lock( m_mutex );
  m_latest.valid = false;
}

void SSGazeReader::Wake()
{
  if( m_wakeEvent ) SetEvent( m_wakeEvent );
}

void SSGazeReader::StoreSample( POINT screenPt )
{
  std::lock_guard<std::mutex> lock( m_mutex );
  m_latest.screenPoint = screenPt;
  m_latest.timestamp = GetTickCount();
  m_latest.valid = true;
}

void SSGazeReader::InjectFakeSample( POINT screenPt )
{
  StoreSample( screenPt );
}

bool SSGazeReader::HasLiveGaze() const
{
  std::lock_guard<std::mutex> lock( m_mutex );
  return m_latest.valid && ( GetTickCount() - m_latest.timestamp < kMaxAgeMs );
}

bool SSGazeReader::GetLatest( GazeSample * out ) const
{
  if( !out ) return false;
  std::lock_guard<std::mutex> lock( m_mutex );
  if( !m_latest.valid ) return false;
  *out = m_latest;
  return true;
}

void SSGazeReader::ReadLoop()
{
  // One-shot diagnostic dump (opt-in via env var) before normal reading.
  if( GetEnvironmentVariableW( L"SIMONSAYS_HID_DUMP", nullptr, 0 ) > 0 )
    DumpHidDiagnostics();

  DeviceCtx dev;
  std::vector<BYTE> report;
  OVERLAPPED ov = {};
  ov.hEvent = CreateEvent( nullptr, FALSE, FALSE, nullptr ); // auto-reset

  while( m_running.load() )
  {
    if( dev.h == INVALID_HANDLE_VALUE )
    {
#if SSGAZE_DEBUG_FEED_CURSOR
      // Test mode: pretend the cursor is a live HID gaze stream.
      POINT cp; if( GetCursorPos( &cp ) ) StoreSample( cp );
      HANDLE waitDbg[2] = { m_stopEvent, m_wakeEvent };
      if( WaitForMultipleObjects( 2, waitDbg, FALSE, 16 ) == WAIT_OBJECT_0 ) break;
      continue;
#else
      if( !OpenDevice( dev ) )
      {
        HANDLE waitRetry[2] = { m_stopEvent, m_wakeEvent };
        DWORD w = WaitForMultipleObjects( 2, waitRetry, FALSE, kRetryMs );
        if( w == WAIT_OBJECT_0 ) break; // stop
        continue;                       // wake or timeout -> retry open
      }
      report.assign( dev.reportLen ? dev.reportLen : 64, 0 );
#endif
    }

    // Issue an overlapped read.
    ResetEvent( ov.hEvent );
    DWORD bytesRead = 0;
    BOOL ok = ReadFile( dev.h, report.data(), (DWORD) report.size(), &bytesRead, &ov );
    if( !ok && GetLastError() == ERROR_IO_PENDING )
    {
      HANDLE waitIo[2] = { m_stopEvent, ov.hEvent };
      DWORD w = WaitForMultipleObjects( 2, waitIo, FALSE, INFINITE );
      if( w == WAIT_OBJECT_0 ) { CancelIo( dev.h ); break; } // stop
      ok = GetOverlappedResult( dev.h, &ov, &bytesRead, FALSE );
    }

    if( !ok )
    {
      CloseDevice( dev ); // device error / removed -> retry open
      continue;
    }

    // Parse the gaze position (micrometers) from the report and map to pixels.
    if( dev.haveFields && bytesRead > 0 )
    {
      LONG vx = 0, vy = 0; // micrometers
      NTSTATUS sx = HidP_GetScaledUsageValue( HidP_Input, HID_USAGE_PAGE_EYE_HEAD_TRACKER, 0,
        dev.usageX, &vx, dev.pp, (PCHAR) report.data(), (ULONG) report.size() );
      NTSTATUS sy = HidP_GetScaledUsageValue( HidP_Input, HID_USAGE_PAGE_EYE_HEAD_TRACKER, 0,
        dev.usageY, &vy, dev.pp, (PCHAR) report.data(), (ULONG) report.size() );

      if( sx != HIDP_STATUS_SUCCESS || sy != HIDP_STATUS_SUCCESS )
      {
        ULONG ux = 0, uy = 0; // fall back to unscaled
        sx = HidP_GetUsageValue( HidP_Input, HID_USAGE_PAGE_EYE_HEAD_TRACKER, 0,
          dev.usageX, &ux, dev.pp, (PCHAR) report.data(), (ULONG) report.size() );
        sy = HidP_GetUsageValue( HidP_Input, HID_USAGE_PAGE_EYE_HEAD_TRACKER, 0,
          dev.usageY, &uy, dev.pp, (PCHAR) report.data(), (ULONG) report.size() );
        vx = (LONG) ux; vy = (LONG) uy;
      }

      // Only store a sample when the gaze point is non-zero on at least one axis;
      // (0,0) is the device's "no valid gaze" placeholder.
      if( sx == HIDP_STATUS_SUCCESS && sy == HIDP_STATUS_SUCCESS && ( vx != 0 || vy != 0 ) )
      {
        POINT pt = {
          MapGazeUmToPx( vx, dev.scrWum, dev.scrWpx ),
          MapGazeUmToPx( vy, dev.scrHum, dev.scrHpx )
        };
        StoreSample( pt );
      }
    }
  }

  if( ov.hEvent ) CloseHandle( ov.hEvent );
  CloseDevice( dev );
}
