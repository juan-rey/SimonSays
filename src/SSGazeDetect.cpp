/*
   SSGazeDetect.cpp: passive detection of eye-tracking hardware / software.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSGazeDetect.h"
#include <setupapi.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <tlhelp32.h>
#include <cwctype>
#include <cstring>
#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")

// HID usage page / usage for eye trackers (HID Usage Tables: "Eye and Head
// Trackers" page 0x12, "Eye Tracker" usage 0x01).
#define HID_USAGE_PAGE_EYE_HEAD_TRACKER 0x0012
#define HID_USAGE_EYE_TRACKER           0x0001

namespace
{
  std::wstring ToLowerCopy( const wchar_t * s )
  {
    std::wstring r = s ? s : L"";
    for( auto & c : r ) c = (wchar_t) towlower( c );
    return r;
  }

  // Known eye-control tools. `prefix == true` matches by image-name prefix
  // (e.g. any "tobii*.exe"); otherwise an exact case-insensitive match.
  struct KnownTool
  {
    const wchar_t * exe;
    const wchar_t * name;
    bool            clicks; // issues its own clicks in at least one common mode
    bool            prefix;
  };

  const KnownTool kKnownTools[] = {
    { L"easyclick.exe",   L"Irisbond EasyClick",        false, false },
    { L"hirusystray.exe", L"Irisbond Hiru",             false, false },
    { L"hiru.exe",        L"Irisbond Hiru",             false, false },
    { L"tdcontrol.exe",   L"Tobii Dynavox TD Control",  true,  false }, // dwell mode clicks
    { L"optikey.exe",     L"OptiKey",                   false, false },
    { L"optikeypro.exe",  L"OptiKey",                   false, false },
    { L"tobii",           L"Tobii Eye Tracking",        false, true  }, // tobii*.exe (Experience, EyeX, Service, TobiiDynavox.EyeAssist, ...)
    // Tobii Dynavox modern stack (PCEye5 dump 2026-07-04): tdx.switcher.exe,
    // tdx.computercontrol.updater.exe, ... Click behavior of Computer Control
    // is unconfirmed — treated as non-clicking until observed otherwise.
    { L"tdx.",            L"Tobii Dynavox",             false, true  },
    // Tobii IS-series platform runtime device service, runs while the tracker
    // is attached even if no user-facing tool does (PCEye5:
    // platform_runtime_is5largepceye5_service.exe; Eye Tracker 5 is IS5-based).
    { L"platform_runtime_is", L"Tobii eye tracker service", false, true },
  };

  // True when a process with the given image name is running (case-insensitive).
  bool IsProcessRunning( const wchar_t * exeName )
  {
    HANDLE snap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( snap == INVALID_HANDLE_VALUE ) return false;
    bool found = false;
    PROCESSENTRY32 pe = {};
    pe.dwSize = sizeof( pe );
    if( Process32First( snap, &pe ) )
    {
      do
      {
        if( _wcsicmp( pe.szExeFile, exeName ) == 0 ) { found = true; break; }
      } while( Process32Next( snap, &pe ) );
    }
    CloseHandle( snap );
    return found;
  }
}

bool IsHidEyeTrackerPresent()
{
  GUID hidGuid;
  HidD_GetHidGuid( &hidGuid );

  HDEVINFO devInfo = SetupDiGetClassDevs( &hidGuid, nullptr, nullptr,
    DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );
  if( devInfo == INVALID_HANDLE_VALUE ) return false;

  bool found = false;
  SP_DEVICE_INTERFACE_DATA ifData = {};
  ifData.cbSize = sizeof( ifData );

  for( DWORD i = 0; !found && SetupDiEnumDeviceInterfaces( devInfo, nullptr, &hidGuid, i, &ifData ); ++i )
  {
    DWORD reqSize = 0;
    SetupDiGetDeviceInterfaceDetail( devInfo, &ifData, nullptr, 0, &reqSize, nullptr );
    if( reqSize == 0 ) continue;

    std::vector<BYTE> buffer( reqSize );
    auto detail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA *>( buffer.data() );
    detail->cbSize = sizeof( SP_DEVICE_INTERFACE_DETAIL_DATA );
    if( !SetupDiGetDeviceInterfaceDetail( devInfo, &ifData, detail, reqSize, nullptr, nullptr ) )
      continue;

    // Access 0 = query only: never blocks the external tool from reading reports.
    HANDLE h = CreateFile( detail->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
      nullptr, OPEN_EXISTING, 0, nullptr );
    if( h == INVALID_HANDLE_VALUE ) continue;

    PHIDP_PREPARSED_DATA pp = nullptr;
    if( HidD_GetPreparsedData( h, &pp ) )
    {
      HIDP_CAPS caps = {};
      if( HidP_GetCaps( pp, &caps ) == HIDP_STATUS_SUCCESS )
      {
        if( caps.UsagePage == HID_USAGE_PAGE_EYE_HEAD_TRACKER &&
          caps.Usage == HID_USAGE_EYE_TRACKER )
          found = true;
      }
      HidD_FreePreparsedData( pp );
    }
    CloseHandle( h );
  }

  SetupDiDestroyDeviceInfoList( devInfo );
  return found;
}

bool DetectActiveEyeTrackingTool( EyeTrackingTool * out )
{
  HANDLE snap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
  if( snap == INVALID_HANDLE_VALUE ) return false;

  bool matched = false;
  PROCESSENTRY32 pe = {};
  pe.dwSize = sizeof( pe );
  if( Process32First( snap, &pe ) )
  {
    do
    {
      std::wstring exe = ToLowerCopy( pe.szExeFile );
      for( const auto & tool : kKnownTools )
      {
        bool hit = tool.prefix
          ? ( exe.rfind( tool.exe, 0 ) == 0 )            // starts with prefix
          : ( _wcsicmp( exe.c_str(), tool.exe ) == 0 );  // exact image name
        if( hit )
        {
          if( out )
          {
            out->processName = exe;
            out->displayName = tool.name;
            out->knownToClick = tool.clicks;
          }
          matched = true;
          break;
        }
      }
    } while( !matched && Process32Next( snap, &pe ) );
  }

  CloseHandle( snap );
  return matched;
}

bool IsWindowsEyeControlEnabled()
{
  // Strongest signal first: the Eye Control app itself. Dumps from three
  // machines (2026-07-04/05: original + PCEye5 + 4C) show microsoft.ecapp.exe
  // running exactly while Eye Control is ON.
  if( IsProcessRunning( L"microsoft.ecapp.exe" ) ) return true;

  // Fallback: the persisted toggle. Windows Eye Control stores it in the
  // per-user CloudStore as a binary "CB" blob under
  //   ...\CloudStore\Store\DefaultAccount\Current\<id>$windows.data.accessibility.eyecontrol.syncedsettings
  //       \windows.data.accessibility.eyecontrol.syncedsettings  (value "Data")
  // The live value may sit under the GUID-suffixed sibling OR under
  // "default$..." depending on the machine — both are scanned.
  //
  // Verified by capturing the blob ON vs OFF on three machines: when Eye
  // Control is ON the inner "CB" sub-blob gains an "enabled" field, so the
  // inner header (43 42 01 00) is immediately followed by 02 01; when OFF it is
  // followed by other bytes (0x22, 0x00, or 0xC2 09 were observed). We
  // therefore treat the byte sequence {43 42 01 00 02} as "enabled".
  const wchar_t * kCurrentPath =
    L"Software\\Microsoft\\Windows\\CurrentVersion\\CloudStore\\Store\\DefaultAccount\\Current";

  HKEY hCurrent = nullptr;
  if( RegOpenKeyEx( HKEY_CURRENT_USER, kCurrentPath, 0, KEY_READ, &hCurrent ) != ERROR_SUCCESS )
    return false;

  static const BYTE kEnabledMarker[] = { 0x43, 0x42, 0x01, 0x00, 0x02 };
  bool enabled = false;

  wchar_t subName[512];
  for( DWORD idx = 0; !enabled; ++idx )
  {
    DWORD nameLen = ARRAYSIZE( subName );
    LONG r = RegEnumKeyEx( hCurrent, idx, subName, &nameLen, nullptr, nullptr, nullptr, nullptr );
    if( r == ERROR_NO_MORE_ITEMS ) break;
    if( r != ERROR_SUCCESS ) continue;

    std::wstring name = subName;
    if( ToLowerCopy( name.c_str() ).find( L"eyecontrol.syncedsettings" ) == std::wstring::npos )
      continue;

    // The value sits under a leaf subkey named by the part after '$'.
    std::wstring leaf = name;
    size_t dollar = name.find( L'$' );
    if( dollar != std::wstring::npos ) leaf = name.substr( dollar + 1 );

    HKEY hVal = nullptr;
    std::wstring rel = name + L"\\" + leaf;
    if( RegOpenKeyEx( hCurrent, rel.c_str(), 0, KEY_READ, &hVal ) != ERROR_SUCCESS )
      continue;

    BYTE data[256];
    DWORD dataSize = sizeof( data );
    DWORD type = 0;
    if( RegQueryValueEx( hVal, L"Data", nullptr, &type, data, &dataSize ) == ERROR_SUCCESS &&
      type == REG_BINARY && dataSize >= sizeof( kEnabledMarker ) )
    {
      for( DWORD i = 0; i + sizeof( kEnabledMarker ) <= dataSize; ++i )
      {
        if( memcmp( data + i, kEnabledMarker, sizeof( kEnabledMarker ) ) == 0 )
        {
          enabled = true;
          break;
        }
      }
    }
    RegCloseKey( hVal );
  }

  RegCloseKey( hCurrent );
  return enabled;
}

bool IsClickingTool( const EyeTrackingTool & tool )
{
  return tool.knownToClick;
}

EyeTrackingStatus DetectEyeTracking()
{
  EyeTrackingStatus st;
  st.hidDevicePresent = IsHidEyeTrackerPresent();

  EyeTrackingTool tool;
  if( DetectActiveEyeTrackingTool( &tool ) )
  {
    st.externalToolRunning = true;
    st.activeToolName = tool.displayName;
    st.toolKnownToClick = IsClickingTool( tool );
  }

  st.windowsEyeControl = IsWindowsEyeControlEnabled();
  return st;
}
