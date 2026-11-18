/*
   SSDialogUtils.h: dialog helpers shared with other hosts of the shared dialogs.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSDialogUtils_h
#define SSDialogUtils_h

#include <windows.h>

// Slider and right-to-left helpers used by the shared Gaze / Dwell-click
// window (dwell.spec.md REQ-F109) and by SimonSays' own dialogs. Moved
// unchanged from utils.cpp so that a second host can compile the shared window
// without SimonSays' utilities; utils.h includes this header, so SimonSays'
// callers are unchanged.

void ConfigureSlider( HWND hDlg, int sliderId, int minValue, int maxValue, int initialValue, int tickMarks = 10 );
void SyncSliderToEdit( HWND hDlg, int sliderId, int editId, BOOL isSigned );
void SyncEditToSlider( HWND hDlg, int editId, int sliderId, BOOL isSigned, int minValue = 0, int maxValue = 0 );

void updateRtlExStyle( HWND hCtrl, bool isRtl, LONG_PTR baseExStyle = 0 );
void updateEditAlignment( HWND hEdit, bool isRtl );
BOOL CALLBACK ApplyRtlStylesCallback( HWND hwnd, LPARAM lParam );

#endif // SSDialogUtils_h
