/*
   SSStreamEngine.cpp: shared Tobii Stream Engine session (used by the app and
   by the 64-bit gaze helper, SimonSaysGaze64.exe).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSStreamEngine.h"
#include <tlhelp32.h>
#include <set>
#include <cwctype>

namespace
{
  constexpr DWORD kRetryMs = 2000;   // re-attempt DLL/engine/device setup on this cadence
  constexpr DWORD kPumpMs = 10;     // callback pump pace while connected (~100 Hz)
  constexpr DWORD kErrorPumpMs = 100; // pump pace while the engine reports errors
  constexpr int   kErrorsBeforeTeardown = 20; // ~2 s of consecutive errors => rebuild device

  // ---------------------------------------------------------------------------
  // Minimal Stream Engine API surface: the smallest set of function signatures,
  // struct layouts and constants needed to call a library the user has already
  // installed, declared from Tobii's publicly documented Stream Engine API.
  // Nothing is redistributed, linked, or required at build time (REQ-N02).
  //
  // SimonSays does not disassemble, decompile or probe the DLL to learn how it
  // is built (Tobii EULA v3.0 §1.4). Testing exercises SimonSays' own code paths
  // — "does the reader receive gaze and do buttons dwell" — and is not used to
  // derive or confirm design information about the Tobii software.
  //
  // Licence-relevant invariant (REQ-N08): gaze obtained through this API is
  // consumed as live user input only. It is never written to disk and never
  // sent over a network or to another computing device. Do NOT add gaze
  // logging or diagnostic dumps to this file.
  //
  // tobii_error_t: only NO_ERROR (0) is interpreted; every other value is
  // handled generically (retry / reconnect / rebuild), so the exact enum
  // numbering across engine versions never matters.
  // ---------------------------------------------------------------------------
  typedef struct tobii_api_t tobii_api_t;
  typedef struct tobii_device_t tobii_device_t;
  typedef int tobii_error_t;
  constexpr tobii_error_t kTobiiNoError = 0;

  struct tobii_version_t { int major, minor, revision, build; };

  // Field of use passed to the engine >= 4 tobii_device_create. UNVERIFIED: no
  // official Tobii documentation of the enum's numeric values has been found,
  // and the names attested elsewhere for this parameter do not agree with each
  // other. The parameter is how an application declares whether it will store
  // or transfer eye tracking data, so a wrong value would declare the opposite
  // of what SimonSays does — silently, as the device opens either way. What the
  // code does is fixed regardless (REQ-N08): gaze is live input only, never
  // stored, never transmitted. Resolve from Tobii's own documentation before
  // relying on this value; do not resolve it by probing the DLL (§1.4).
  constexpr int kFieldOfUseInteractive = 1;

  constexpr int kTobiiValidityValid = 1;

  struct tobii_gaze_point_t
  {
    long long timestamp_us;
    int       validity;       // kTobiiValidityValid when the point is usable
    float     position_xy[2]; // normalized on the tracker's display (may fall outside 0..1)
  };

  typedef void( __cdecl * tobii_gaze_point_callback_t )( tobii_gaze_point_t const *, void * );
  typedef void( __cdecl * tobii_device_url_receiver_t )( char const *, void * );

  typedef tobii_error_t( __cdecl * PFN_tobii_get_api_version )( tobii_version_t * );
  typedef tobii_error_t( __cdecl * PFN_tobii_api_create )( tobii_api_t **, void *, void * );
  typedef tobii_error_t( __cdecl * PFN_tobii_api_destroy )( tobii_api_t * );
  typedef tobii_error_t( __cdecl * PFN_tobii_enumerate_local_device_urls )( tobii_api_t *, tobii_device_url_receiver_t, void * );
  // tobii_device_create gained a field_of_use parameter in engine 4.0; the
  // export name is unchanged, so the same FARPROC is cast per the DLL version.
  typedef tobii_error_t( __cdecl * PFN_tobii_device_create_v3 )( tobii_api_t *, char const *, tobii_device_t ** );
  typedef tobii_error_t( __cdecl * PFN_tobii_device_create_v4 )( tobii_api_t *, char const *, int, tobii_device_t ** );
  typedef tobii_error_t( __cdecl * PFN_tobii_device_destroy )( tobii_device_t * );
  typedef tobii_error_t( __cdecl * PFN_tobii_device_process_callbacks )( tobii_device_t * );
  typedef tobii_error_t( __cdecl * PFN_tobii_device_reconnect )( tobii_device_t * );
  typedef tobii_error_t( __cdecl * PFN_tobii_gaze_point_subscribe )( tobii_device_t *, tobii_gaze_point_callback_t, void * );
  typedef tobii_error_t( __cdecl * PFN_tobii_gaze_point_unsubscribe )( tobii_device_t * );

  struct StreamEngine
  {
    HMODULE                                dll = nullptr;
    PFN_tobii_get_api_version              get_api_version = nullptr;  // optional
    PFN_tobii_api_create                   api_create = nullptr;
    PFN_tobii_api_destroy                  api_destroy = nullptr;
    PFN_tobii_enumerate_local_device_urls  enumerate_local_device_urls = nullptr;
    FARPROC                                device_create = nullptr;    // cast per version
    PFN_tobii_device_destroy               device_destroy = nullptr;
    PFN_tobii_device_process_callbacks     device_process_callbacks = nullptr;
    PFN_tobii_device_reconnect             device_reconnect = nullptr; // optional
    PFN_tobii_gaze_point_subscribe         gaze_point_subscribe = nullptr;
    PFN_tobii_gaze_point_unsubscribe       gaze_point_unsubscribe = nullptr;

    bool Resolve()
    {
      get_api_version = (PFN_tobii_get_api_version) GetProcAddress( dll, "tobii_get_api_version" );
      api_create = (PFN_tobii_api_create) GetProcAddress( dll, "tobii_api_create" );
      api_destroy = (PFN_tobii_api_destroy) GetProcAddress( dll, "tobii_api_destroy" );
      enumerate_local_device_urls = (PFN_tobii_enumerate_local_device_urls) GetProcAddress( dll, "tobii_enumerate_local_device_urls" );
      device_create = GetProcAddress( dll, "tobii_device_create" );
      device_destroy = (PFN_tobii_device_destroy) GetProcAddress( dll, "tobii_device_destroy" );
      device_process_callbacks = (PFN_tobii_device_process_callbacks) GetProcAddress( dll, "tobii_device_process_callbacks" );
      device_reconnect = (PFN_tobii_device_reconnect) GetProcAddress( dll, "tobii_device_reconnect" );
      gaze_point_subscribe = (PFN_tobii_gaze_point_subscribe) GetProcAddress( dll, "tobii_gaze_point_subscribe" );
      gaze_point_unsubscribe = (PFN_tobii_gaze_point_unsubscribe) GetProcAddress( dll, "tobii_gaze_point_unsubscribe" );
      return api_create && api_destroy && enumerate_local_device_urls && device_create &&
        device_destroy && device_process_callbacks && gaze_point_subscribe && gaze_point_unsubscribe;
    }

    void Unload()
    {
      if( dll ) FreeLibrary( dll );
      *this = StreamEngine{};
    }
  };

  std::wstring ToLowerCopy( const std::wstring & s )
  {
    std::wstring r = s;
    for( auto & c : r ) c = (wchar_t) towlower( c );
    return r;
  }

  // Candidate full paths for tobii_stream_engine.dll. The engine ships next to
  // the vendor's executables (4C / PCEye5: 32-bit copies; Eye Tracker 5 with
  // current Tobii Experience: a 64-bit copy in "Tobii EyeX"), so the
  // directories of running tobii* / tdx.* / platform_runtime_is* processes are
  // the best source — and gaze streaming needs that stack running anyway.
  void CollectDllCandidates( std::vector<std::wstring> & out )
  {
    out.push_back( L"tobii_stream_engine.dll" ); // PATH / app folder, cheap first try

    HANDLE snap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( snap == INVALID_HANDLE_VALUE ) return;

    std::set<std::wstring> seen;
    PROCESSENTRY32W pe = {};
    pe.dwSize = sizeof( pe );
    if( Process32FirstW( snap, &pe ) )
    {
      do
      {
        std::wstring exe = ToLowerCopy( pe.szExeFile );
        if( exe.rfind( L"tobii", 0 ) != 0 &&
          exe.rfind( L"tdx.", 0 ) != 0 &&
          exe.rfind( L"platform_runtime_is", 0 ) != 0 )
          continue;

        HANDLE h = OpenProcess( PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID );
        if( !h ) continue;
        wchar_t path[MAX_PATH] = {};
        DWORD len = ARRAYSIZE( path );
        if( QueryFullProcessImageNameW( h, 0, path, &len ) )
        {
          std::wstring dir = path;
          size_t slash = dir.find_last_of( L'\\' );
          if( slash != std::wstring::npos )
          {
            std::wstring cand = dir.substr( 0, slash + 1 ) + L"tobii_stream_engine.dll";
            if( seen.insert( ToLowerCopy( cand ) ).second )
              out.push_back( cand );
          }
        }
        CloseHandle( h );
      } while( Process32NextW( snap, &pe ) );
    }
    CloseHandle( snap );
  }

  // Tries each candidate until one loads AND exposes the required exports. A
  // copy built for the other architecture fails with ERROR_BAD_EXE_FORMAT and
  // is recorded as such (REQ-F96). When keepLoaded is false the usable copy is
  // unloaded again after its version is read (diagnostic scan).
  SSStreamEngineLoadReport LoadEngine( StreamEngine & se, bool keepLoaded )
  {
    SSStreamEngineLoadReport report;
    std::vector<std::wstring> paths;
    CollectDllCandidates( paths );
    for( const auto & path : paths )
    {
      SSStreamEngineCandidate c;
      c.path = path;
      // Altered search path so the engine's own dependencies resolve from its
      // folder. No error dialogs for missing dependencies.
      UINT oldMode = SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX );
      HMODULE dll = ( path.find( L'\\' ) != std::wstring::npos )
        ? LoadLibraryExW( path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH )
        : LoadLibraryW( path.c_str() );
      c.loadError = dll ? 0 : GetLastError();
      SetErrorMode( oldMode );
      c.loaded = ( dll != nullptr );
      if( c.loadError == ERROR_BAD_EXE_FORMAT ) report.otherArchitecture = true;
      if( dll )
      {
        wchar_t actual[MAX_PATH] = {};
        if( GetModuleFileNameW( dll, actual, ARRAYSIZE( actual ) ) ) c.loadedFrom = actual;
      }

      if( dll && !report.usable )
      {
        se.dll = dll;
        c.exportsOk = se.Resolve();
        if( c.exportsOk )
        {
          report.usable = true;
          tobii_version_t ver = {};
          if( se.get_api_version && se.get_api_version( &ver ) == kTobiiNoError )
          {
            report.versionMajor = ver.major; report.versionMinor = ver.minor;
            report.versionRevision = ver.revision; report.versionBuild = ver.build;
          }
          report.candidates.push_back( c );
          if( keepLoaded ) return report;
          se.Unload();
          continue;
        }
        se.Unload();
      }
      else if( dll )
      {
        // Diagnostic scan past the usable copy: still check the exports so
        // the report is accurate, then release it.
        StreamEngine probe;
        probe.dll = dll;
        c.exportsOk = probe.Resolve();
        FreeLibrary( dll );
      }
      report.candidates.push_back( c );
    }
    return report;
  }

  void __cdecl GazePointCallback( tobii_gaze_point_t const * gp, void * userData )
  {
    if( !gp || !userData ) return;
    ISSStreamEngineSink * sink = reinterpret_cast<ISSStreamEngineSink *>( userData );
    sink->OnEngineCallback(); // any validity: the tracker is on (REQ-F94)
    if( gp->validity != kTobiiValidityValid ) return;
    sink->OnValidGazePoint( gp->position_xy[0], gp->position_xy[1] );
  }

  void __cdecl FirstUrlReceiver( char const * url, void * userData )
  {
    auto * s = reinterpret_cast<std::string *>( userData );
    if( s->empty() && url && url[0] ) *s = url;
  }
}

SSStreamEngineLoadReport SSStreamEngineScanCandidates()
{
  StreamEngine se;
  return LoadEngine( se, false );
}

SSStreamEngineRunResult SSStreamEngineRun( ISSStreamEngineSink & sink,
  const HANDLE * stopHandles, DWORD stopCount, HANDLE wakeEvent,
  bool returnOnOtherArchitecture )
{
  StreamEngine se;
  tobii_api_t * api = nullptr;
  tobii_device_t * device = nullptr;
  int consecutiveErrors = 0;
  SSStreamEngineRunResult result = SSStreamEngineRunResult::Stopped;

  std::vector<HANDLE> waitSet( stopHandles, stopHandles + stopCount );
  if( wakeEvent ) waitSet.push_back( wakeEvent );

  // Waits on stop/wake; returns true when the loop must end.
  auto waitOrStop = [&]( DWORD ms ) -> bool
    {
      DWORD r = WaitForMultipleObjects( (DWORD) waitSet.size(), waitSet.data(), FALSE, ms );
      return r < WAIT_OBJECT_0 + stopCount || r == WAIT_FAILED;
    };

  auto teardownDevice = [&]()
    {
      if( device )
      {
        se.gaze_point_unsubscribe( device );
        se.device_destroy( device );
        device = nullptr;
        sink.OnDeviceOpen( false );
      }
      consecutiveErrors = 0;
    };

  for( ;; )
  {
    sink.OnPump();

    // 1) Engine DLL: locate + load the user-installed copy.
    if( !se.dll )
    {
      SSStreamEngineLoadReport report = LoadEngine( se, true );
      sink.OnLoadReport( report );
      if( !report.usable )
      {
        if( returnOnOtherArchitecture && report.otherArchitecture )
        {
          result = SSStreamEngineRunResult::OnlyOtherArchitecture;
          break;
        }
        if( waitOrStop( kRetryMs ) ) break;
        continue;
      }
    }

    // 2) API context.
    if( !api )
    {
      if( se.api_create( &api, nullptr, nullptr ) != kTobiiNoError )
      {
        api = nullptr;
        if( waitOrStop( kRetryMs ) ) break;
        continue;
      }
    }

    // 3) Device: first enumerated tracker, opened for Interactive Use.
    if( !device )
    {
      std::string url;
      se.enumerate_local_device_urls( api, &FirstUrlReceiver, &url );
      if( url.empty() )
      {
        if( waitOrStop( kRetryMs ) ) break;
        continue;
      }

      tobii_version_t ver = {};
      if( se.get_api_version ) se.get_api_version( &ver );
      tobii_error_t err = ( ver.major >= 4 )
        ? ( (PFN_tobii_device_create_v4) se.device_create )( api, url.c_str(), kFieldOfUseInteractive, &device )
        : ( (PFN_tobii_device_create_v3) se.device_create )( api, url.c_str(), &device );
      sink.OnDeviceCreateResult( err );
      if( err != kTobiiNoError || !device )
      {
        // Busy / license / transient: never fatal, never blocks the vendor
        // stack — just retry later (REQ-F93).
        device = nullptr;
        if( waitOrStop( kRetryMs ) ) break;
        continue;
      }

      if( se.gaze_point_subscribe( device, &GazePointCallback, &sink ) != kTobiiNoError )
      {
        se.device_destroy( device );
        device = nullptr;
        if( waitOrStop( kRetryMs ) ) break;
        continue;
      }
      sink.OnDeviceOpen( true );
    }

    // 4) Pump: callbacks fire on this thread inside process_callbacks.
    tobii_error_t err = se.device_process_callbacks( device );
    if( err != kTobiiNoError )
    {
      ++consecutiveErrors;
      if( consecutiveErrors == 1 && se.device_reconnect )
        se.device_reconnect( device ); // typical for a briefly dropped connection
      if( consecutiveErrors > kErrorsBeforeTeardown )
        teardownDevice();              // rebuild from enumeration
      if( waitOrStop( kErrorPumpMs ) ) break;
    }
    else
    {
      consecutiveErrors = 0;
      if( waitOrStop( kPumpMs ) ) break;
    }
  }

  teardownDevice();
  if( api ) { se.api_destroy( api ); api = nullptr; }
  se.Unload();
  return result;
}
