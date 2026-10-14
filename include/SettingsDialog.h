/*
   SettingsDialog.h: modal Settings dialog (F2).

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SettingsDialog_h
#define SettingsDialog_h

#include "stdafx.h"

// Shows the modal Settings dialog (settings.spec.md) seeded from `settings`.
// On OK the edited values are written into `settings` and true is returned; on
// Cancel `settings` is left untouched and false is returned. The caller
// persists and applies the result (language switch, voice, layout).
bool ShowSettingsDialog( HWND owner, HINSTANCE hInstance, Settings & settings );

#endif // SettingsDialog_h
