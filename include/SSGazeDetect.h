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
  bool         externalToolRunning = false; // any known eye-control process is running (cursor tool OR vendor service)
  bool         cursorToolRunning = false; // a tool that may drive the cursor is running (REQ-F28 presence)
  bool         windowsEyeControl = false; // Windows Eye Control appears to be enabled
  bool         toolKnownToClick = false; // a running tool issues its own clicks
  std::wstring activeToolName;             // display name of the detected tool (empty if none)
};

// One row of the known-tools table.
struct EyeTrackingTool
{
  std::wstring processName; // matched process image name (lower-case), or prefix
  std::wstring displayName; // human-readable name for the UI
  bool         knownToClick = false; // tool can issue its own clicks (-> ExternalClick)
  // REQ-F31: true for tools that can move the Windows cursor (EasyClick, OptiKey,
  // TD Control, EyeAssist, ...); false for vendor background services of
  // trackers SimonSays reads directly (Tobii Service / Experience / Core,
  // platform runtime), which never drive the cursor and so must not enable
  // Mouse dwell on their own (REQ-F28).
  bool         drivesCursor = false;
};

// HID enumeration filtered on Usage Page 0x0012 / Usage 0x0001 (Eye Tracker).
// Handles are opened with access 0 (query only, no reads) so the device is never
// blocked from the external tool. Returns true if at least one is present.
bool IsHidEyeTrackerPresent();

// Toolhelp32 scan of EVERY running process against the known-tools table
// (case-insensitive). Fills *out with the most relevant match — a clicking
// tool, else a cursor tool, else a vendor service — and returns true; returns
// false if none run. Optional flags report whether ANY running match drives the
// cursor / issues its own clicks (a Tobii service listed first no longer hides
// a cursor or clicking tool running alongside it).
bool DetectActiveEyeTrackingTool( EyeTrackingTool * out, bool * anyDrivesCursor = nullptr, bool * anyClicks = nullptr );

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
