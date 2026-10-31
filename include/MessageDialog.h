/*
   MessageDialog.h: SimonSays message box (10 pt, dwell-enabled buttons).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays - Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef MessageDialog_h
#define MessageDialog_h

#include "stdafx.h"

// Drop-in replacement for MessageBoxEx used by ShowLocalizedMessageBox
// (localization.spec.md LOC-F31/F32). Honors MB_OK, MB_OKCANCEL, MB_YESNO,
// MB_YESNOCANCEL; MB_ICONERROR/WARNING/INFORMATION/QUESTION; MB_DEFBUTTON1-3;
// MB_SETFOREGROUND. Returns IDOK / IDCANCEL / IDYES / IDNO like MessageBox.
// Any other button set falls back to MessageBoxEx. The text and button labels
// follow `language` (right-to-left languages are mirrored). The buttons are
// SSButtons, so dwell-click can answer the box; dwell arms only after
// MESSAGE_DIALOG_DWELL_ARM_MS so a box opening under the user's gaze does not
// answer itself.
int ShowMessageDialog( HWND owner, const wchar_t * text, const wchar_t * caption,
  UINT flags, const std::wstring & language );

#endif // MessageDialog_h
