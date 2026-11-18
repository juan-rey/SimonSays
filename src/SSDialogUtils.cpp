/*
   SSDialogUtils.cpp: dialog helpers shared with other hosts of the shared dialogs.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "stdafx.h"
#include "SSDialogUtils.h"
#include <commctrl.h>

// Moved unchanged from utils.cpp (dwell.spec.md REQ-F109).

void ConfigureSlider( HWND hDlg, int sliderId, int minValue, int maxValue, int initialValue, int tickMarks )
{
  HWND hSlider = GetDlgItem( hDlg, sliderId );
  if( !hSlider ) return;
  SendMessage( hSlider, TBM_SETRANGE, FALSE, MAKELONG( minValue, maxValue ) );
  int tickFreq = max( 1, ( maxValue - minValue ) / max( 1, tickMarks ) );
  SendMessage( hSlider, TBM_SETTICFREQ, tickFreq, 0 );
  SendMessage( hSlider, TBM_SETPOS, TRUE, initialValue );
}

void SyncSliderToEdit( HWND hDlg, int sliderId, int editId, BOOL isSigned )
{
  HWND hSlider = GetDlgItem( hDlg, sliderId );
  if( !hSlider ) return;
  int pos = (int) SendMessage( hSlider, TBM_GETPOS, 0, 0 );
  SetDlgItemInt( hDlg, editId, pos, isSigned );
}

void SyncEditToSlider( HWND hDlg, int editId, int sliderId, BOOL isSigned, int minValue, int maxValue )
{
  BOOL translated = FALSE;
  int value = (int) GetDlgItemInt( hDlg, editId, &translated, isSigned );
  if( !translated ) return;
  value = ( maxValue > minValue ) ? min( maxValue, max( minValue, value ) ) : ( value );
  SendDlgItemMessage( hDlg, sliderId, TBM_SETPOS, TRUE, value );
}

void updateRtlExStyle( HWND hCtrl, bool isRtl, LONG_PTR baseExStyle )
{
  if( !hCtrl ) return;
  LONG_PTR exStyle = GetWindowLongPtr( hCtrl, GWL_EXSTYLE );
  LONG_PTR rtlFlags = WS_EX_LAYOUTRTL | WS_EX_RTLREADING;
  LONG_PTR newExStyle = ( isRtl ? ( baseExStyle | exStyle | rtlFlags ) : ( ( baseExStyle | exStyle ) & ~rtlFlags ) );
  if( newExStyle != exStyle )
  {
    SetWindowLongPtr( hCtrl, GWL_EXSTYLE, newExStyle );
    SetWindowPos( hCtrl, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED );
  }
}

void updateEditAlignment( HWND hEdit, bool isRtl )
{
  if( !hEdit ) return;
  LONG_PTR style = GetWindowLongPtr( hEdit, GWL_STYLE );
  LONG_PTR exStyle = GetWindowLongPtr( hEdit, GWL_EXSTYLE );
  LONG_PTR newStyle = style;
  if( isRtl )
  {
    newStyle &= ~( ES_LEFT | ES_CENTER );
    newStyle |= ES_RIGHT;
  }
  else
  {
    newStyle &= ~( ES_RIGHT | ES_CENTER );
    newStyle |= ES_LEFT;
  }

  if( newStyle != style )
  {
    SetWindowLongPtr( hEdit, GWL_STYLE, newStyle );
    SetWindowPos( hEdit, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED );
  }

  updateRtlExStyle( hEdit, isRtl, exStyle );
}

BOOL CALLBACK ApplyRtlStylesCallback( HWND hwnd, LPARAM lParam )
{
  // Use the W-form to stay consistent with the rest of the Unicode-only codebase
  // and to avoid the ANSI thunk's truncation surprises on non-ASCII class names.
  // lParam is treated as a bool — callers must pass 0 or 1 only.
  wchar_t className[64];
  GetClassNameW( hwnd, className, ARRAYSIZE( className ) );

  const bool isRtl = ( lParam != 0 );
  if( _wcsicmp( className, L"Edit" ) == 0 )
    updateEditAlignment( hwnd, isRtl );
  else
    updateRtlExStyle( hwnd, isRtl );

  return TRUE;
}
