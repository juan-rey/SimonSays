/*
   SSGazeDetect.h: passive detection of eye-tracking hardware / software.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSGazeDetect_h
#define SSGazeDetect_h

#include "stdafx.h"

// Passive signals about the eye-tracking environment. "Passive" means presence,
// not proof: a device or tool being present does not guarantee dwell will work,
// so these feed the (later) automatic detector and the settings status area —
// they never silently change behavior on their own.
struct EyeTrackingStatus
{
  bool         hidDevicePresent = false; // a HID eye-tracker (usage page 0x12) is present
  bool         externalToolRunning = false; // a known eye-control tool process is running
  bool         windowsEyeControl = false; // Windows Eye Control appears to be enabled
  bool         toolKnownToClick = false; // the running tool issues its own clicks
  std::wstring activeToolName;             // display name of the detected tool (empty if none)
};

// One row of the known-tools table.
struct EyeTrackingTool
{
  std::wstring processName; // matched process image name (lower-case), or prefix
  std::wstring displayName; // human-readable name for the UI
  bool         knownToClick = false; // tool can issue its own clicks (-> ExternalClick later)
};

// HID enumeration filtered on Usage Page 0x0012 / Usage 0x0001 (Eye Tracker).
// Handles are opened with access 0 (query only, no reads) so the device is never
// blocked from the external tool. Returns true if at least one is present.
bool IsHidEyeTrackerPresent();

// Toolhelp32 process scan against the known-tools table (case-insensitive).
// Fills *out with the first match and returns true; returns false if none run.
bool DetectActiveEyeTrackingTool( EyeTrackingTool * out );

// Windows Eye Control state: true when the Eye Control app (microsoft.ecapp.exe)
// is running, or when the persisted toggle is set in the HKCU CloudStore blob
// (ON/OFF byte marker verified on three machines). Missing key => not enabled.
bool IsWindowsEyeControlEnabled();

// Whether a detected tool is known to issue its own clicks on our buttons (used
// later to recommend ExternalClick so our dwell stays out of the way).
bool IsClickingTool( const EyeTrackingTool & tool );

// Aggregates all of the above into one snapshot.
EyeTrackingStatus DetectEyeTracking();

#endif // SSGazeDetect_h
