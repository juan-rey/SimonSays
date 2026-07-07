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

// Reads gaze from Tobii trackers through the legacy Stream Engine C API
// (tobii_stream_engine.dll), for devices whose "Tobii Eye Tracker HID"
// interface denies user-mode opens (4C, PCEye5, Eye Tracker 5 — see
// dwell.spec.md §17/§18 and AC-16).
//
// The DLL is NEVER shipped with SimonSays: at runtime the reader locates the
// copy installed by the user's own Tobii software (next to the running
// tobii* / tdx.* / platform_runtime_is* processes), LoadLibrary's it, and
// resolves the needed exports dynamically. No build-time dependency exists
// (REQ-N02 holds), and gaze is used strictly as live user input (the legacy
// license's "Interactive Use") — never stored or transmitted.
//
// The read thread is self-healing like SSGazeReader: missing DLL, engine or
// device makes it idle and retry; a failing connection is re-established.
// When no valid gaze is produced, HasLiveGaze() stays false and the provider
// chain transparently falls back to the HID reader or the cursor, so this
// backend can never break the app (REQ-F93 / REQ-N04: the vendor's own
// software must keep working — connect failures are retried, never fatal).
class SSTobiiGaze
{
public:
  static SSTobiiGaze & Instance();

  bool Start(); // spin up the engine thread (idempotent)
  void Stop();  // signal stop, join the thread, tear the engine down

  bool HasLiveGaze() const;                 // a valid sample arrived within the max-age window
  bool GetLatest( GazeSample * out ) const; // most recent sample (virtual-desktop px)

  // Pulse the engine thread to re-attempt DLL/device setup immediately (e.g.
  // on WM_DEVICECHANGE) instead of waiting for the next retry tick.
  void Wake();

  // Internal: invoked from the Stream Engine gaze-point callback with a valid
  // normalized (0..1) point on the tracker's display. Not for product code.
  void OnEngineGazePoint( float normX, float normY );

private:
  SSTobiiGaze() = default;
  ~SSTobiiGaze();
  SSTobiiGaze( const SSTobiiGaze & ) = delete;
  SSTobiiGaze & operator=( const SSTobiiGaze & ) = delete;

  void ReadLoop();
  void StoreSample( POINT screenPt );

  std::atomic<bool>  m_running{ false };
  std::thread        m_thread;
  HANDLE             m_stopEvent = nullptr; // manual-reset: ends the loop
  HANDLE             m_wakeEvent = nullptr; // auto-reset: interrupts the retry wait

  mutable std::mutex m_mutex;
  GazeSample         m_latest; // guarded; virtual-desktop pixels
};

#endif // SSTobiiGaze_h
