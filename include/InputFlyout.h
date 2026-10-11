/*
   InputFlyout.h: InputFlyout class header file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays - Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef InputFlyout_h
#define InputFlyout_h

#include "stdafx.h"
#include "utils.h"

// The pop-up that holds the main window's input box (settings.spec.md
// SET-F62/F63, taskbar.spec.md TBR-F31). The edit control is created by
// MainWindow as a child of this window and lives here for good; the bar only
// shows a preview. Notifications from the edit (WM_COMMAND, Enter, Esc) are
// forwarded to the owner.
class InputFlyout
{
public:
  InputFlyout() = default;
  ~InputFlyout();

  bool Create( HINSTANCE hInstance, HWND hwndOwner );
  HWND GetHwnd() const { return m_hwnd; }
  void SetEdit( HWND hEdit ) { m_hEdit = hEdit; }

  // Shows the flyout next to the taskbar beside `preview` (screen rect of the
  // bar's preview), activates it and puts the caret at the end of the text.
  // `restoreTo` is the window Close( true ) gives the focus back to.
  void Open( const RECT & preview, const RECT & bar, const TaskbarGeometry & taskbar, HWND restoreTo );
  // Hides the flyout; with restoreFocus, first hands the focus back to the
  // window that had it before (when it still exists).
  void Close( bool restoreFocus );
  bool IsOpen() const { return m_hwnd && IsWindowVisible( m_hwnd ); }

  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
  void LayoutEdit();

  HWND m_hwnd = NULL;
  HWND m_hwndOwner = NULL;
  HWND m_hEdit = NULL;
  HWND m_hwndRestore = NULL;
  HBRUSH m_hBackground = NULL;
};

#endif
