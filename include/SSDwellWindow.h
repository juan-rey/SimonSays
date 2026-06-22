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

// Shows the modal gaze / dwell-click settings window. It lets the user pick the
// activation mode, tune the dwell timing/tolerance/cooldown and progress color,
// and run the two-probe detection that auto-selects the mode by observing how
// each probe button is activated (gaze dwell vs. physical click).
//
// Settings are edited live against SSDwellConfig so the probes react
// immediately. On OK the chosen values are written into `settings` (the dwell
// fields only) and true is returned; on Cancel the previous SSDwellConfig state
// is restored and false is returned. The caller persists `settings`.
bool ShowDwellSettingsDialog( HWND owner, HINSTANCE hInstance, Settings & settings );

#endif // SSDwellWindow_h
