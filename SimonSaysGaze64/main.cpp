/*
   SimonSaysGaze64 main.cpp: 64-bit Tobii Stream Engine host for the 32-bit
   SimonSays app (dwell.spec.md REQ-F96–F100).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.

   SimonSays starts this helper only when it is 32-bit, runs on 64-bit Windows,
   and finds only a 64-bit tobii_stream_engine.dll (e.g. Tobii Eye Tracker 5
   with current Tobii software). The helper loads that user-installed DLL with
   the same code the app uses (SSStreamEngine.cpp) and publishes one gaze record
   in a section the app created and passed down by handle inheritance.

   Licence-relevant invariant (REQ-N08, Tobii EULA v3.0 §1.1 / §1.3): gaze is
   held only in that single in-memory record, overwritten on every update. It is
   never written to disk, never sent over a network or to another computing
   device, and the section has no name, so no other process can open it. The
   helper has no window, no console, no log file.

   Command line (handle values inherited from SimonSays):
     SimonSaysGaze64.exe --section <h> --stop <h> --wake <h> --parent <h>
   Run any other way, it exits immediately.
*/
#include <windows.h>
#include <shellapi.h>
#include <cwchar>
#include "SSStreamEngine.h"
#include "SSGazeHelperProtocol.h"

#pragma comment(lib, "shell32.lib")

namespace
{
  class SharedRecordSink : public ISSStreamEngineSink
  {
  public:
    explicit SharedRecordSink( SSGazeHelperRecord * r ) : m_r( r ) {}

    void OnPump() override
    {
      Write( [&] { m_r->heartbeatTick = GetTickCount(); } );
    }
    void OnDeviceOpen( bool open ) override
    {
      Write( [&]
        {
          m_r->deviceOpen = open ? 1 : 0;
          if( !open ) { m_r->anyCallback = 0; m_r->sampleValid = 0; }
        } );
    }
    void OnEngineCallback() override
    {
      Write( [&] { m_r->lastCallbackTick = GetTickCount(); m_r->anyCallback = 1; } );
    }
    void OnValidGazePoint( float x, float y ) override
    {
      Write( [&]
        {
          m_r->normX = x;
          m_r->normY = y;
          m_r->sampleTick = GetTickCount();
          m_r->sampleValid = 1;
        } );
    }
    void OnLoadReport( const SSStreamEngineLoadReport & rep ) override
    {
      Write( [&]
        {
          m_r->engineUsable = rep.usable ? 1 : 0;
          m_r->engineMajor = rep.versionMajor; m_r->engineMinor = rep.versionMinor;
          m_r->engineRevision = rep.versionRevision; m_r->engineBuild = rep.versionBuild;
        } );
    }
    void OnDeviceCreateResult( int err ) override
    {
      Write( [&] { m_r->lastDeviceCreateError = err; } );
    }

  private:
    template <typename F> void Write( F f )
    {
      SSGazeHelperBeginWrite( m_r );
      MemoryBarrier();
      f();
      MemoryBarrier();
      SSGazeHelperEndWrite( m_r );
    }
    SSGazeHelperRecord * m_r;
  };

  HANDLE ParseHandle( int argc, wchar_t ** argv, const wchar_t * name )
  {
    for( int i = 1; i + 1 < argc; ++i )
      if( wcscmp( argv[i], name ) == 0 )
        return (HANDLE) (ULONG_PTR) _wcstoui64( argv[i + 1], nullptr, 10 );
    return nullptr;
  }
}

int WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
  SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX | SEM_NOGPFAULTERRORBOX );

  int argc = 0;
  wchar_t ** argv = CommandLineToArgvW( GetCommandLineW(), &argc );
  if( !argv ) return 1;
  HANDLE section = ParseHandle( argc, argv, L"--section" );
  HANDLE stopEv = ParseHandle( argc, argv, L"--stop" );
  HANDLE wakeEv = ParseHandle( argc, argv, L"--wake" );
  HANDLE parent = ParseHandle( argc, argv, L"--parent" );
  LocalFree( argv );
  if( !section || !stopEv || !wakeEv || !parent ) return 1;

  SSGazeHelperRecord * rec = (SSGazeHelperRecord *) MapViewOfFile( section,
    FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof( SSGazeHelperRecord ) );
  if( !rec ) return 1;

  rec->version = kSSGazeHelperVersion;
  MemoryBarrier();
  rec->magic = kSSGazeHelperMagic;

  // Stop when SimonSays asks, or when SimonSays is gone (REQ-F99).
  SharedRecordSink sink( rec );
  HANDLE stops[2] = { stopEv, parent };
  SSStreamEngineRun( sink, stops, 2, wakeEv, false );

  UnmapViewOfFile( rec );
  CloseHandle( section );
  CloseHandle( stopEv );
  CloseHandle( wakeEv );
  CloseHandle( parent );
  return 0;
}
