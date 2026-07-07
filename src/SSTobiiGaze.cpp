/*
   SSTobiiGaze.cpp: Tobii Stream Engine gaze reader (process-wide singleton).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSTobiiGaze.h"
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <set>
#include <cwctype>

namespace
{
  constexpr DWORD kMaxAgeMs = 200;    // sample older than this => not "live" (matches SSGazeReader)
  constexpr DWORD kRetryMs = 2000;   // re-attempt DLL/engine/device setup on this cadence
  constexpr DWORD kPumpMs = 10;     // callback pump pace while connected (~100 Hz)
  constexpr DWORD kErrorPumpMs = 100; // pump pace while the engine reports errors
  constexpr int   kErrorsBeforeTeardown = 20; // ~2 s of consecutive errors => rebuild device

  // ---------------------------------------------------------------------------
  // Minimal Stream Engine API surface, self-declared from Tobii's public Stream
  // Engine documentation (the SDK headers are not redistributed here, and the
  // DLL is loaded from the user's own Tobii installation at runtime).
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

  // TOBII_FIELD_OF_USE_INTERACTIVE: gaze as live user input, never stored or
  // transmitted — the licensed use dwell-click falls under (engine >= 4.0).
  constexpr int kFieldOfUseInteractive = 1;

  constexpr int kTobiiValidityValid = 1; // tobii_validity_t TOBII_VALIDITY_VALID

  struct tobii_gaze_point_t
  {
    long long timestamp_us;
    int       validity;       // kTobiiValidityValid when the point is usable
    float     position_xy[2]; // normalized 0..1 on the tracker's display
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
  // the vendor's executables (confirmed on the PCEye5 and 4C test machines,
  // including inside TD Computer Control), so the directories of running
  // tobii* / tdx.* / platform_runtime_is* processes are the best source — and
  // gaze streaming needs that stack running anyway.
  void CollectDllCandidates( std::vector<std::wstring> & out )
  {
    out.push_back( L"tobii_stream_engine.dll" ); // PATH / app folder, cheap first try

    HANDLE snap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( snap == INVALID_HANDLE_VALUE ) return;

    std::set<std::wstring> seen;
    PROCESSENTRY32 pe = {};
    pe.dwSize = sizeof( pe );
    if( Process32First( snap, &pe ) )
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
      } while( Process32Next( snap, &pe ) );
    }
    CloseHandle( snap );
  }

  // Tries each candidate until one loads AND exposes the required exports.
  bool LoadEngine( StreamEngine & se )
  {
    std::vector<std::wstring> candidates;
    CollectDllCandidates( candidates );
    for( const auto & path : candidates )
    {
      // Altered search path so the engine's own dependencies resolve from its
      // folder; an x86 DLL in an x64 process fails here cleanly and is skipped.
      HMODULE dll = ( path.find( L'\\' ) != std::wstring::npos )
        ? LoadLibraryExW( path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH )
        : LoadLibraryW( path.c_str() );
      if( !dll ) continue;
      se.dll = dll;
      if( se.Resolve() ) return true;
      se.Unload();
    }
    return false;
  }

  void __cdecl GazePointCallback( tobii_gaze_point_t const * gp, void * userData )
  {
    if( !gp || gp->validity != kTobiiValidityValid || !userData ) return;
    reinterpret_cast<SSTobiiGaze *>( userData )->OnEngineGazePoint( gp->position_xy[0], gp->position_xy[1] );
  }

  void __cdecl FirstUrlReceiver( char const * url, void * userData )
  {
    auto * s = reinterpret_cast<std::string *>( userData );
    if( s->empty() && url && url[0] ) *s = url;
  }
}

SSTobiiGaze & SSTobiiGaze::Instance()
{
  static SSTobiiGaze s_gaze;
  return s_gaze;
}

SSTobiiGaze::~SSTobiiGaze()
{
  Stop();
}

bool SSTobiiGaze::Start()
{
  if( m_running.exchange( true ) ) return true; // already running

  m_stopEvent = CreateEvent( nullptr, TRUE, FALSE, nullptr );  // manual reset
  m_wakeEvent = CreateEvent( nullptr, FALSE, FALSE, nullptr ); // auto reset
  m_thread = std::thread( &SSTobiiGaze::ReadLoop, this );
  return true;
}

void SSTobiiGaze::Stop()
{
  if( !m_running.exchange( false ) )
  {
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

void SSTobiiGaze::Wake()
{
  if( m_wakeEvent ) SetEvent( m_wakeEvent );
}

void SSTobiiGaze::StoreSample( POINT screenPt )
{
  std::lock_guard<std::mutex> lock( m_mutex );
  m_latest.screenPoint = screenPt;
  m_latest.timestamp = GetTickCount();
  m_latest.valid = true;
}

void SSTobiiGaze::OnEngineGazePoint( float normX, float normY )
{
  // Normalized 0..1 on the tracker's display -> primary-monitor pixels, the
  // same mapping (and limitation) as the HID reader's µm path (REQ-F72).
  if( normX < 0.0f ) normX = 0.0f; else if( normX > 1.0f ) normX = 1.0f;
  if( normY < 0.0f ) normY = 0.0f; else if( normY > 1.0f ) normY = 1.0f;
  POINT pt = {
    (LONG) ( normX * GetSystemMetrics( SM_CXSCREEN ) ),
    (LONG) ( normY * GetSystemMetrics( SM_CYSCREEN ) )
  };
  StoreSample( pt );
}

bool SSTobiiGaze::HasLiveGaze() const
{
  std::lock_guard<std::mutex> lock( m_mutex );
  return m_latest.valid && ( GetTickCount() - m_latest.timestamp < kMaxAgeMs );
}

bool SSTobiiGaze::GetLatest( GazeSample * out ) const
{
  if( !out ) return false;
  std::lock_guard<std::mutex> lock( m_mutex );
  if( !m_latest.valid ) return false;
  *out = m_latest;
  return true;
}

void SSTobiiGaze::ReadLoop()
{
  StreamEngine se;
  tobii_api_t * api = nullptr;
  tobii_device_t * device = nullptr;
  int consecutiveErrors = 0;

  // Waits on stop/wake; returns true when the loop must end.
  auto waitOrStop = [&]( DWORD ms ) -> bool
    {
      HANDLE h[2] = { m_stopEvent, m_wakeEvent };
      return WaitForMultipleObjects( 2, h, FALSE, ms ) == WAIT_OBJECT_0;
    };

  auto teardownDevice = [&]()
    {
      if( device )
      {
        se.gaze_point_unsubscribe( device );
        se.device_destroy( device );
        device = nullptr;
      }
      consecutiveErrors = 0;
      std::lock_guard<std::mutex> lock( m_mutex );
      m_latest.valid = false;
    };

  while( m_running.load() )
  {
    // 1) Engine DLL: locate + load the user-installed copy.
    if( !se.dll )
    {
      if( !LoadEngine( se ) )
      {
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
      if( err != kTobiiNoError || !device )
      {
        // Busy / license / transient: never fatal, never blocks the vendor
        // stack — just retry later (REQ-F93).
        device = nullptr;
        if( waitOrStop( kRetryMs ) ) break;
        continue;
      }

      if( se.gaze_point_subscribe( device, &GazePointCallback, this ) != kTobiiNoError )
      {
        teardownDevice();
        if( waitOrStop( kRetryMs ) ) break;
        continue;
      }
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
}
