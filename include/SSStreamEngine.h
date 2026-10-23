/*
   SSStreamEngine.h: shared Tobii Stream Engine session (used by the app and
   by the 64-bit gaze helper, SimonSaysGaze64.exe).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSStreamEngine_h
#define SSStreamEngine_h

// Deliberately free of stdafx.h: the helper project compiles this file too and
// must stay a minimal Windows-SDK-only executable.
#include <windows.h>
#include <string>
#include <vector>

// Licence-relevant invariant (REQ-N08): gaze obtained through the Stream
// Engine is consumed as live user input only. It is never written to disk and
// never sent over a network or to another computing device. Do NOT add gaze
// logging or diagnostic dumps here — the REQ-F80 dump is a HID-reader facility;
// the REQ-F101 Stream Engine section reports DLL / process status only.

// Result of trying one candidate tobii_stream_engine.dll (REQ-F96 / REQ-F101).
// Status only: nothing here comes from the tracker's sensors.
struct SSStreamEngineCandidate
{
  std::wstring path;
  std::wstring loadedFrom;     // actual module file when loaded (a bare name may resolve to an already-loaded copy)
  DWORD        loadError = 0; // GetLastError of LoadLibrary; 0 when it loaded
  bool         loaded = false;
  bool         exportsOk = false; // loaded AND exposes the exports we call
};

struct SSStreamEngineLoadReport
{
  std::vector<SSStreamEngineCandidate> candidates;
  bool usable = false;           // one candidate loaded with all exports
  bool otherArchitecture = false; // at least one failed with ERROR_BAD_EXE_FORMAT (193)
  int  versionMajor = 0, versionMinor = 0, versionRevision = 0, versionBuild = 0;
};

// Receives engine events on the session thread.
class ISSStreamEngineSink
{
public:
  virtual ~ISSStreamEngineSink() = default;
  virtual void OnDeviceOpen( bool open ) = 0;              // device created + subscribed / torn down
  virtual void OnEngineCallback() = 0;                     // gaze callback of ANY validity (REQ-F94)
  virtual void OnValidGazePoint( float normX, float normY ) = 0; // valid, normalized 0..1 (unclamped)
  virtual void OnPump() {}                                 // once per loop iteration (heartbeat)
  virtual void OnLoadReport( const SSStreamEngineLoadReport & ) {} // after each DLL load attempt
  virtual void OnDeviceCreateResult( int ) {}              // tobii_error_t of the last create (0 = ok)
};

enum class SSStreamEngineRunResult
{
  Stopped,                // a stop handle was signalled
  OnlyOtherArchitecture   // returnOnOtherArchitecture and only wrong-bitness copies exist
};

// Runs a self-healing Stream Engine session until any handle in stopHandles is
// signalled: locate + load the user-installed DLL, create the API, open the
// first enumerated device for Interactive Use, subscribe to gaze points, and
// pump callbacks (~100 Hz). Missing DLL / engine / device => idle and retry
// every 2 s; connection errors => reconnect, then rebuild. wakeEvent (may be
// null) cuts a retry wait short. When returnOnOtherArchitecture is true and a
// load attempt finds no usable copy but at least one copy of the other
// architecture, the function returns OnlyOtherArchitecture (REQ-F96).
SSStreamEngineRunResult SSStreamEngineRun( ISSStreamEngineSink & sink,
  const HANDLE * stopHandles, DWORD stopCount, HANDLE wakeEvent,
  bool returnOnOtherArchitecture );

// One-shot candidate scan for diagnostics (REQ-F101): tries each candidate
// with LoadLibrary, records the outcome, reads the engine version from the
// first usable copy, and unloads everything again.
SSStreamEngineLoadReport SSStreamEngineScanCandidates();

#endif // SSStreamEngine_h
