/*
   SSTobiiGaze.cpp: Tobii Stream Engine gaze reader (process-wide singleton).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSTobiiGaze.h"
#include "SSStreamEngine.h"
#include "SSGazeHelperProtocol.h"
#include <vector>

// Licence-relevant invariant (REQ-N08): gaze obtained through the Stream Engine
// — in-process or through the 64-bit helper — is consumed as live user input
// only. It is never written to disk and never sent over a network or to
// another computing device. Do NOT add gaze logging or diagnostic dumps to this
// file. The helper's record lives in an unnamed section whose handle only the
// helper inherits (Tobii EULA v3.0 §1.3: no other process can open it).

namespace
{
  constexpr DWORD kMaxAgeMs = 200;          // sample older than this => not "live" (matches SSGazeReader)
  constexpr DWORD kTrackingWindowMs = 1000; // no callback of any validity for this long => not tracking (REQ-F94)
  constexpr DWORD kRetryMs = 2000;          // helper relaunch cadence after a failed launch / exit
  constexpr DWORD kBackoffMs = 30000;       // relaunch cadence after kCrashesBeforeBackoff quick exits
  constexpr int   kCrashesBeforeBackoff = 3;
  constexpr DWORD kStableRunMs = 60000;     // a helper that ran this long resets the crash count
  constexpr DWORD kPollMs = 10;             // helper record poll pace (~100 Hz)
  constexpr DWORD kHeartbeatMaxAgeMs = 1000; // older heartbeat => helper stalled: device not open
  constexpr DWORD kHelperStopWaitMs = 2000; // graceful stop before the job kills the helper

  // In-process route: engine events straight into the reader.
  class InProcessSink : public ISSStreamEngineSink
  {
  public:
    explicit InProcessSink( SSTobiiGaze * g ) : m_g( g ) {}
    void OnDeviceOpen( bool open ) override { m_g->OnEngineDeviceOpen( open ); }
    void OnEngineCallback() override { m_g->OnEngineCallback(); }
    void OnValidGazePoint( float x, float y ) override { m_g->OnEngineGazePoint( x, y ); }
    void OnLoadReport( const SSStreamEngineLoadReport & r ) override
    {
      m_g->OnEngineStatus( r.usable, r.versionMajor, r.versionMinor, r.versionRevision, r.versionBuild );
    }
    void OnDeviceCreateResult( int err ) override { m_g->OnEngineDeviceCreateResult( err ); }
  private:
    SSTobiiGaze * m_g;
  };

  // Normalized 0..1 on the tracker's display -> primary-monitor pixels, the
  // same mapping (and limitation) as the HID reader's µm path (REQ-F72).
  POINT MapNormalized( float normX, float normY )
  {
    if( !( normX >= 0.0f ) ) normX = 0.0f; else if( normX > 1.0f ) normX = 1.0f; // also catches NaN
    if( !( normY >= 0.0f ) ) normY = 0.0f; else if( normY > 1.0f ) normY = 1.0f;
    POINT pt = {
      (LONG) ( normX * GetSystemMetrics( SM_CXSCREEN ) ),
      (LONG) ( normY * GetSystemMetrics( SM_CYSCREEN ) )
    };
    return pt;
  }

  bool IsWow64()
  {
#if defined(_WIN64)
    return false;
#else
    BOOL wow = FALSE;
    return IsWow64Process( GetCurrentProcess(), &wow ) && wow;
#endif
  }

  // Launches the helper with exactly four inherited handles (section, stop,
  // wake, a SYNCHRONIZE handle to this process) and assigns it to the job.
  // Returns the helper's process handle, or nullptr.
  HANDLE LaunchHelper( const std::wstring & exe, HANDLE section, HANDLE stopEv,
    HANDLE wakeEv, HANDLE job, DWORD * pidOut )
  {
    HANDLE parent = nullptr;
    if( !DuplicateHandle( GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(),
      &parent, SYNCHRONIZE, TRUE, 0 ) )
      return nullptr;

    HANDLE inherit[4] = { section, stopEv, wakeEv, parent };
    // Inheritable only for the duration of this launch; the explicit handle
    // list below keeps them out of any other child process.
    for( int i = 0; i < 3; ++i ) SetHandleInformation( inherit[i], HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT );

    HANDLE proc = nullptr;
    SIZE_T attrSize = 0;
    InitializeProcThreadAttributeList( nullptr, 1, 0, &attrSize );
    std::vector<BYTE> attrBuf( attrSize );
    LPPROC_THREAD_ATTRIBUTE_LIST attrs = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>( attrBuf.data() );
    if( attrSize && InitializeProcThreadAttributeList( attrs, 1, 0, &attrSize ) )
    {
      if( UpdateProcThreadAttribute( attrs, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
        inherit, sizeof( inherit ), nullptr, nullptr ) )
      {
        wchar_t args[256];
        swprintf_s( args, L" --section %Iu --stop %Iu --wake %Iu --parent %Iu",
          (ULONG_PTR) section, (ULONG_PTR) stopEv, (ULONG_PTR) wakeEv, (ULONG_PTR) parent );
        std::wstring cmd = L"\"" + exe + L"\"" + args;
        std::vector<wchar_t> cmdBuf( cmd.begin(), cmd.end() );
        cmdBuf.push_back( 0 );

        std::wstring dir = exe.substr( 0, exe.find_last_of( L'\\' ) );

        STARTUPINFOEXW si = {};
        si.StartupInfo.cb = sizeof( si );
        si.lpAttributeList = attrs;
        PROCESS_INFORMATION pi = {};
        if( CreateProcessW( exe.c_str(), cmdBuf.data(), nullptr, nullptr, TRUE,
          CREATE_NO_WINDOW | EXTENDED_STARTUPINFO_PRESENT | CREATE_SUSPENDED,
          nullptr, dir.c_str(), &si.StartupInfo, &pi ) )
        {
          // Kill-on-close job: the helper can never outlive SimonSays (REQ-F99).
          // If assignment fails the helper still exits when this process ends,
          // because it waits on the parent handle.
          if( job ) AssignProcessToJobObject( job, pi.hProcess );
          ResumeThread( pi.hThread );
          CloseHandle( pi.hThread );
          proc = pi.hProcess;
          if( pidOut ) *pidOut = pi.dwProcessId;
        }
      }
      DeleteProcThreadAttributeList( attrs );
    }

    for( int i = 0; i < 3; ++i ) SetHandleInformation( inherit[i], HANDLE_FLAG_INHERIT, 0 );
    CloseHandle( parent ); // the helper holds its own inherited copy
    return proc;
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

std::wstring SSTobiiGaze::HelperPath()
{
  wchar_t path[MAX_PATH] = {};
  DWORD n = GetModuleFileNameW( nullptr, path, ARRAYSIZE( path ) );
  if( n == 0 || n >= ARRAYSIZE( path ) ) return std::wstring();
  std::wstring p = path;
  size_t slash = p.find_last_of( L'\\' );
  if( slash == std::wstring::npos ) return std::wstring();
  return p.substr( 0, slash + 1 ) + SSGAZE_HELPER_EXE_NAME;
}

bool SSTobiiGaze::HelperUsable()
{
  if( !IsWow64() ) return false; // x64 build, or 32-bit Windows: never (REQ-F96)
  std::wstring p = HelperPath();
  if( p.empty() ) return false;
  DWORD attr = GetFileAttributesW( p.c_str() );
  return attr != INVALID_FILE_ATTRIBUTES && !( attr & FILE_ATTRIBUTE_DIRECTORY );
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

  ResetLiveState();
}

void SSTobiiGaze::Wake()
{
  if( m_wakeEvent ) SetEvent( m_wakeEvent );
}

void SSTobiiGaze::StoreSample( POINT screenPt, DWORD tick )
{
  std::lock_guard<std::mutex> lock( m_mutex );
  m_latest.screenPoint = screenPt;
  m_latest.timestamp = tick;
  m_latest.valid = true;
}

void SSTobiiGaze::ResetLiveState()
{
  m_deviceOpen.store( false );
  m_anyCallback.store( false );
  std::lock_guard<std::mutex> lock( m_mutex );
  m_latest.valid = false;
}

void SSTobiiGaze::OnEngineGazePoint( float normX, float normY )
{
  StoreSample( MapNormalized( normX, normY ), GetTickCount() );
}

void SSTobiiGaze::OnEngineDeviceOpen( bool open )
{
  if( open ) m_deviceOpen.store( true );
  else ResetLiveState();
}

void SSTobiiGaze::OnEngineStatus( bool usable, int major, int minor, int revision, int build )
{
  std::lock_guard<std::mutex> lock( m_statusMutex );
  m_status.engineUsable = usable;
  m_status.versionMajor = major; m_status.versionMinor = minor;
  m_status.versionRevision = revision; m_status.versionBuild = build;
}

void SSTobiiGaze::OnEngineDeviceCreateResult( int err )
{
  std::lock_guard<std::mutex> lock( m_statusMutex );
  m_status.haveCreateResult = true;
  m_status.lastDeviceCreateError = err;
}

SSTobiiGaze::Status SSTobiiGaze::GetStatus() const
{
  std::lock_guard<std::mutex> lock( m_statusMutex );
  return m_status;
}

bool SSTobiiGaze::HasLiveGaze() const
{
  std::lock_guard<std::mutex> lock( m_mutex );
  return m_latest.valid && ( GetTickCount() - m_latest.timestamp < kMaxAgeMs );
}

bool SSTobiiGaze::IsTracking() const
{
  return m_deviceOpen.load() && m_anyCallback.load() &&
    ( GetTickCount() - m_lastCallbackTick.load() < kTrackingWindowMs );
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
  {
    std::lock_guard<std::mutex> lock( m_statusMutex );
    m_status.route = Route::InProcess;
  }

  // REQ-F96: only a 32-bit app on 64-bit Windows with the helper installed
  // gives up on the in-process route when the only engine is 64-bit.
  InProcessSink sink( this );
  HANDLE stops[1] = { m_stopEvent };
  SSStreamEngineRunResult r = SSStreamEngineRun( sink, stops, 1, m_wakeEvent, HelperUsable() );
  ResetLiveState();

  if( r == SSStreamEngineRunResult::OnlyOtherArchitecture && m_running.load() )
    RunHelperLoop();
}

void SSTobiiGaze::RunHelperLoop()
{
  {
    std::lock_guard<std::mutex> lock( m_statusMutex );
    m_status = Status{};
    m_status.route = Route::Helper;
  }

  // Unnamed objects: nothing for other processes to find (EULA §1.3 / REQ-F98).
  HANDLE section = CreateFileMappingW( INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE,
    0, sizeof( SSGazeHelperRecord ), nullptr );
  SSGazeHelperRecord * view = section
    ? (SSGazeHelperRecord *) MapViewOfFile( section, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof( SSGazeHelperRecord ) )
    : nullptr;
  HANDLE helperStop = CreateEvent( nullptr, TRUE, FALSE, nullptr );
  HANDLE helperWake = CreateEvent( nullptr, FALSE, FALSE, nullptr );
  HANDLE job = CreateJobObjectW( nullptr, nullptr );
  if( job )
  {
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION li = {};
    li.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if( !SetInformationJobObject( job, JobObjectExtendedLimitInformation, &li, sizeof( li ) ) )
    {
      CloseHandle( job );
      job = nullptr;
    }
  }

  const std::wstring exe = HelperPath();
  HANDLE proc = nullptr;
  DWORD launchTick = 0;
  DWORD nextLaunch = GetTickCount();
  int quickExits = 0;
  DWORD lastSampleTick = 0;

  if( view && helperStop && helperWake )
  {
    HANDLE waits[2] = { m_stopEvent, m_wakeEvent };
    while( m_running.load() )
    {
      DWORD now = GetTickCount();

      if( !proc && (LONG) ( now - nextLaunch ) >= 0 )
      {
        ZeroMemory( view, sizeof( *view ) );
        ResetEvent( helperStop );
        DWORD pid = 0;
        proc = LaunchHelper( exe, section, helperStop, helperWake, job, &pid );
        if( proc )
        {
          launchTick = now;
          std::lock_guard<std::mutex> lock( m_statusMutex );
          m_status.helperRunning = true;
          m_status.helperPid = pid;
        }
        else
        {
          nextLaunch = now + kRetryMs;
        }
      }

      if( proc && WaitForSingleObject( proc, 0 ) == WAIT_OBJECT_0 )
      {
        // REQ-F99: an unexpected exit is retried; quick repeated exits back off.
        CloseHandle( proc );
        proc = nullptr;
        ResetLiveState();
        quickExits = ( now - launchTick >= kStableRunMs ) ? 1 : quickExits + 1;
        nextLaunch = now + ( quickExits >= kCrashesBeforeBackoff ? kBackoffMs : kRetryMs );
        std::lock_guard<std::mutex> lock( m_statusMutex );
        m_status.helperRunning = false;
        m_status.helperPid = 0;
        ++m_status.helperExits;
      }

      if( proc )
      {
        SSGazeHelperRecord rec;
        if( SSGazeHelperRead( view, &rec ) )
        {
          bool fresh = ( now - rec.heartbeatTick < kHeartbeatMaxAgeMs );
          bool open = fresh && rec.deviceOpen != 0;
          m_deviceOpen.store( open );
          if( open && rec.anyCallback )
          {
            m_lastCallbackTick.store( rec.lastCallbackTick );
            m_anyCallback.store( true );
          }
          else
          {
            m_anyCallback.store( false );
          }

          if( open && rec.sampleValid && rec.sampleTick != lastSampleTick )
          {
            lastSampleTick = rec.sampleTick;
            StoreSample( MapNormalized( rec.normX, rec.normY ), rec.sampleTick );
          }
          else if( !open )
          {
            std::lock_guard<std::mutex> lock( m_mutex );
            m_latest.valid = false;
          }

          std::lock_guard<std::mutex> lock( m_statusMutex );
          m_status.engineUsable = rec.engineUsable != 0;
          m_status.versionMajor = rec.engineMajor; m_status.versionMinor = rec.engineMinor;
          m_status.versionRevision = rec.engineRevision; m_status.versionBuild = rec.engineBuild;
          m_status.haveCreateResult = rec.lastDeviceCreateError != 0 || open;
          m_status.lastDeviceCreateError = rec.lastDeviceCreateError;
        }
      }

      DWORD w = WaitForMultipleObjects( 2, waits, FALSE, kPollMs );
      if( w == WAIT_OBJECT_0 || w == WAIT_FAILED ) break;
      if( w == WAIT_OBJECT_0 + 1 ) SetEvent( helperWake ); // REQ-F99: forward Wake()
    }
  }
  else
  {
    // Could not set up the shared record: stay idle until stop (no gaze).
    WaitForSingleObject( m_stopEvent, INFINITE );
  }

  if( proc )
  {
    SetEvent( helperStop );
    if( WaitForSingleObject( proc, kHelperStopWaitMs ) != WAIT_OBJECT_0 && !job )
      TerminateProcess( proc, 1 );
    CloseHandle( proc );
  }
  if( job ) CloseHandle( job ); // kill-on-close ends anything still running
  if( view ) UnmapViewOfFile( view );
  if( section ) CloseHandle( section );
  if( helperStop ) CloseHandle( helperStop );
  if( helperWake ) CloseHandle( helperWake );
  ResetLiveState();
  std::lock_guard<std::mutex> lock( m_statusMutex );
  m_status.helperRunning = false;
  m_status.helperPid = 0;
}
