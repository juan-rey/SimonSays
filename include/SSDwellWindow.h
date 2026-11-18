/*
   SSDwellWindow.h: modal "Gaze / Dwell-click" settings window.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSDwellWindow_h
#define SSDwellWindow_h

#include "stdafx.h"
#include <string>

// The dwell values the window edits: the dwell fields of SimonSays' Settings,
// and nothing else (dwell.spec.md REQ-F109). Same meanings and defaults.
struct SSDwellValues
{
  int      modeSelection = DWELL_DEFAULT_MODE_SELECTION; // 0 Auto, 1 Off, 2 Mouse, 3 HID
  int      detectedMode = DWELL_DEFAULT_DETECTED_MODE;   // 0 Off, 1 Mouse, 2 HID, 3 ExternalClick
  int      timeMs = DWELL_DEFAULT_TIME_MS;
  int      toleranceRadius = DWELL_DEFAULT_TOLERANCE_PX;
  int      cooldownMs = DWELL_DEFAULT_COOLDOWN_MS;
  int      lockOnMs = DWELL_DEFAULT_LOCKON_MS;
  COLORREF progressColor = RGB( 0, 120, 215 );
  bool     rearmOnLeave = DWELL_DEFAULT_REARM_ON_LEAVE;
};

// What the window needs from the application hosting it (REQ-F109), so that
// it depends on none of SimonSays' utilities, string tables or Settings.
class SSDwellWindowHost
{
public:
  virtual ~SSDwellWindowHost() = default;
  // The text for one of SimonSays' string ids (the DWELL_*_ID values and
  // SETTINGS_OK_BUTTON_ID / SETTINGS_CANCEL_BUTTON_ID in stdafx.h).
  virtual std::wstring Text( int stringId ) = 0;
  // True for a right-to-left UI language.
  virtual bool IsRtl() = 0;
  // The progress colour Reset restores (REQ-F67).
  virtual COLORREF DefaultProgressColor() = 0;
  // Whether the window shows its progress-colour button (REQ-F62) and its
  // Save diagnostic report button (REQ-F102). SimonSays shows both.
  virtual bool ShowProgressColor() { return true; }
  virtual bool ShowDiagnosticReport() { return true; }
};

// The host-neutral entry point (REQ-F109): as below, with `values` in place of
// Settings and the host supplying text and defaults. True on OK, with
// `values` updated; false on Cancel, with the live config restored.
bool ShowDwellSettingsDialog( HWND owner, HINSTANCE hInstance, SSDwellValues & values, SSDwellWindowHost & host );

// Shows the modal gaze / dwell-click settings window. It lets the user pick the
// activation mode, tune the dwell timing/tolerance/cooldown and progress color,
// and run the two-probe detection that auto-selects the mode by observing how
// each probe button is activated (gaze dwell vs. physical click).
//
// Settings are edited live against SSDwellConfig so the probes react
// immediately. On OK the chosen values are written into `settings` (the dwell
// fields only) and true is returned; on Cancel the previous SSDwellConfig state
// is restored and false is returned. The caller persists `settings`.
// SimonSays' entry point; defined in SSDwellWindowSimonSays.cpp on top of the
// host-neutral one, with SimonSays' strings, RTL rule and accent colour.
bool ShowDwellSettingsDialog( HWND owner, HINSTANCE hInstance, Settings & settings );

#endif // SSDwellWindow_h
