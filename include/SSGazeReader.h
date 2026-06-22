/*
   SSGazeReader.h: direct HID eye-tracker reader (process-wide singleton).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSGazeReader_h
#define SSGazeReader_h

#include "stdafx.h"
#include "IGazeProvider.h"
#include <atomic>
#include <mutex>
#include <thread>

// Reads gaze directly from a HID eye-tracker (Usage Page 0x12 / Usage 0x01) on
// a background thread, for the two-step / no-cursor modes where the external
// tool does not move the Windows cursor. The device is opened NON-EXCLUSIVELY
// (FILE_SHARE_READ | FILE_SHARE_WRITE) so the host AAC tool is never blocked.
//
// The read thread is self-healing: if no device is present (or one is removed)
// it idles and retries, so connect/disconnect is handled without a restart.
//
// IMPORTANT: the gaze report field layout is provisional — it parses the
// HID eye-tracker value usages best-effort and must be validated against real
// hardware (see Dwell.md). When no valid gaze is produced, HasLiveGaze() stays
// false and the provider chain transparently falls back to the cursor, so an
// imperfect parser never breaks the app.
class SSGazeReader
{
public:
  static SSGazeReader & Instance();

  bool Start(); // spin up the read thread (idempotent)
  void Stop();  // signal stop, join the thread, close the device

  bool HasLiveGaze() const;                 // a valid sample arrived within the max-age window
  bool GetLatest( GazeSample * out ) const; // most recent sample (virtual-desktop px)

  // Test hook: feed a synthetic sample (virtual-desktop px) as if it came from
  // the device, so the detector / SSButton pipeline can be exercised without
  // hardware. Not used by product code.
  void InjectFakeSample( POINT screenPt );

  // Pulse the read thread to re-attempt opening immediately (e.g. on
  // WM_DEVICECHANGE) instead of waiting for the next retry tick.
  void Wake();

private:
  SSGazeReader() = default;
  ~SSGazeReader();
  SSGazeReader( const SSGazeReader & ) = delete;
  SSGazeReader & operator=( const SSGazeReader & ) = delete;

  void ReadLoop();
  void StoreSample( POINT screenPt );

  std::atomic<bool>  m_running{ false };
  std::thread        m_thread;
  HANDLE             m_stopEvent = nullptr; // manual-reset: ends the loop
  HANDLE             m_wakeEvent = nullptr; // auto-reset: interrupts the retry wait

  mutable std::mutex m_mutex;
  GazeSample         m_latest; // guarded; virtual-desktop pixels
};

#endif // SSGazeReader_h
