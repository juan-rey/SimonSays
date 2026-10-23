/*
   SSTobiiGaze.h: Tobii Stream Engine gaze reader (process-wide singleton).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSTobiiGaze_h
#define SSTobiiGaze_h

#include "stdafx.h"
#include "IGazeProvider.h"
#include <atomic>
#include <mutex>
#include <thread>
#include <string>

// Reads gaze from Tobii trackers through the legacy Stream Engine C API
// (tobii_stream_engine.dll), for devices whose "Tobii Eye Tracker HID"
// interface denies user-mode opens (4C, PCEye5, Eye Tracker 5 — see
// dwell.spec.md §17/§18 and AC-16).
//
// The DLL is NEVER shipped with SimonSays: at runtime the reader locates the
// copy installed by the user's own Tobii software (next to the running
// tobii* / tdx.* / platform_runtime_is* processes), LoadLibrary's it, and
// resolves the needed exports dynamically (SSStreamEngine.cpp). Nothing is
// redistributed, linked, or required at build time (REQ-N02 holds).
//
// Two routes (REQ-F96):
//  - in-process: the engine's bitness matches the app (4C / PCEye5 stacks ship
//    a 32-bit engine; the default build is 32-bit);
//  - 64-bit helper: a 32-bit app on 64-bit Windows that finds only a 64-bit
//    engine (Eye Tracker 5 with current Tobii software) starts
//    SimonSaysGaze64.exe, which runs the same engine code and shares one
//    in-memory record through an unnamed, handle-inherited section
//    (REQ-F97–F100, SSGazeHelperProtocol.h).
//
// Licence-relevant invariant (REQ-N08): gaze obtained through this provider is
// consumed as live user input only. It is never written to disk and never sent
// over a network or to another computing device. Do NOT add gaze logging or
// diagnostic dumps to this provider — GetStatus() reports status only
// (REQ-F101), never sensor-derived values.
//
// The read thread is self-healing like SSGazeReader: missing DLL, engine or
// device makes it idle and retry; a failing connection is re-established; a
// helper that exits is restarted (with back-off). When no valid gaze is
// produced, HasLiveGaze() stays false and the provider chain transparently
// falls back to the HID reader or the cursor, so this backend can never break
// the app (REQ-F93 / REQ-N04).
class SSTobiiGaze
{
public:
  static SSTobiiGaze & Instance();

  bool Start(); // spin up the engine thread (idempotent)
  void Stop();  // signal stop, join the thread, tear the engine / helper down

  bool HasLiveGaze() const;                 // a valid sample arrived within the max-age window
  bool GetLatest( GazeSample * out ) const; // most recent sample (virtual-desktop px)

  // REQ-F94: the engine delivered a gaze callback of ANY validity recently.
  // Only arrival times are kept, never coordinates (REQ-N08).
  bool IsTracking() const;
  bool IsDeviceOpen() const { return m_deviceOpen.load(); } // device created + subscribed

  // Pulse the engine thread (or the helper) to re-attempt DLL/device setup
  // immediately (e.g. on WM_DEVICECHANGE) instead of waiting for the retry tick.
  void Wake();

  // REQ-F101 status. Nothing here is derived from the tracker's sensors.
  enum class Route { NotStarted, InProcess, Helper };
  struct Status
  {
    Route route = Route::NotStarted;
    bool  helperRunning = false;
    DWORD helperPid = 0;
    int   helperExits = 0;         // helper processes that ended unexpectedly
    bool  engineUsable = false;    // a usable engine was loaded (in-process or in the helper)
    int   versionMajor = 0, versionMinor = 0, versionRevision = 0, versionBuild = 0;
    bool  haveCreateResult = false;
    int   lastDeviceCreateError = 0;
  };
  Status GetStatus() const;

  // Full path of SimonSaysGaze64.exe next to SimonSays.exe, and whether this
  // process is one that may use it (32-bit on 64-bit Windows, file present).
  static std::wstring HelperPath();
  static bool HelperUsable();

  // Internal: engine events (in-process route). Not for product code.
  void OnEngineCallback() { m_lastCallbackTick.store( GetTickCount() ); m_anyCallback.store( true ); }
  void OnEngineGazePoint( float normX, float normY );
  void OnEngineDeviceOpen( bool open );
  void OnEngineStatus( bool usable, int major, int minor, int revision, int build );
  void OnEngineDeviceCreateResult( int err );

private:
  SSTobiiGaze() = default;
  ~SSTobiiGaze();
  SSTobiiGaze( const SSTobiiGaze & ) = delete;
  SSTobiiGaze & operator=( const SSTobiiGaze & ) = delete;

  void ReadLoop();
  void RunHelperLoop();
  void StoreSample( POINT screenPt, DWORD tick );
  void ResetLiveState();

  std::atomic<bool>  m_running{ false };
  std::thread        m_thread;
  HANDLE             m_stopEvent = nullptr; // manual-reset: ends the loop
  HANDLE             m_wakeEvent = nullptr; // auto-reset: interrupts the retry wait

  std::atomic<bool>  m_deviceOpen{ false };
  std::atomic<DWORD> m_lastCallbackTick{ 0 };
  std::atomic<bool>  m_anyCallback{ false }; // m_lastCallbackTick is meaningful

  mutable std::mutex m_mutex;
  GazeSample         m_latest; // guarded; virtual-desktop pixels

  mutable std::mutex m_statusMutex;
  Status             m_status; // guarded
};

#endif // SSTobiiGaze_h
