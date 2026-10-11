/*
   InputFlyout.cpp: InputFlyout class source file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays - Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "InputFlyout.h"
#include <dwmapi.h>

#define INPUT_FLYOUT_CLASS L"SimonSaysInputFlyout"
#define INPUT_FLYOUT_MARGIN 6            // around the edit control
#define INPUT_FLYOUT_LINES 3             // text lines visible in the edit control
#define INPUT_FLYOUT_MIN_WIDTH 480       // horizontal taskbar: at least this wide (TBR-F31)
#define INPUT_FLYOUT_VERTICAL_WIDTH 420  // vertical taskbar width (TBR-F31)

InputFlyout::~InputFlyout()
{
  if( m_hwnd && IsWindow( m_hwnd ) )
    DestroyWindow( m_hwnd );
  m_hwnd = NULL;
  if( m_hBackground )
    DeleteObject( m_hBackground );
  m_hBackground = NULL;
}

bool InputFlyout::Create( HINSTANCE hInstance, HWND hwndOwner )
{
  m_hwndOwner = hwndOwner;
  const COLORREF taskbarColor = GetTaskbarColor();

  static bool s_classRegistered = false;
  if( !s_classRegistered )
  {
    WNDCLASS wc = {};
    wc.lpfnWndProc = InputFlyout::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = INPUT_FLYOUT_CLASS;
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    if( !RegisterClass( &wc ) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS )
      return false;
    s_classRegistered = true;
  }
  m_hBackground = CreateSolidBrush( taskbarColor );

  // Owned by the bar: never in the taskbar, destroyed with it, kept above it.
  m_hwnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW, INPUT_FLYOUT_CLASS, L"SimonSays - Input",
    WS_POPUP | WS_BORDER, 0, 0, INPUT_FLYOUT_MIN_WIDTH, 80, hwndOwner, NULL, hInstance, this );
  if( !m_hwnd )
    return false;

  DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
  DwmSetWindowAttribute( m_hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof( preference ) );
  return true;
}

void InputFlyout::Open( const RECT & preview, const RECT & bar, const TaskbarGeometry & taskbar, HWND restoreTo )
{
  if( !m_hwnd || !m_hEdit ) return;
  m_hwndRestore = restoreTo;

  // Height: INPUT_FLYOUT_LINES lines of the edit's font, plus its client edge
  // and the margins.
  int lineHeight = 16;
  HDC hdc = GetDC( m_hEdit );
  HFONT hFont = (HFONT) SendMessage( m_hEdit, WM_GETFONT, 0, 0 );
  HFONT hOldFont = hFont ? (HFONT) SelectObject( hdc, hFont ) : NULL;
  TEXTMETRIC tm = {};
  if( GetTextMetrics( hdc, &tm ) )
    lineHeight = tm.tmHeight;
  if( hOldFont ) SelectObject( hdc, hOldFont );
  ReleaseDC( m_hEdit, hdc );

  RECT frame = { 0, 0, 0, INPUT_FLYOUT_LINES * lineHeight + 2 * INPUT_FLYOUT_MARGIN + 4 };
  AdjustWindowRectEx( &frame, WS_POPUP | WS_BORDER, FALSE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW );
  const RECT & work = taskbar.work;
  const int workWidth = work.right - work.left;
  int height = frame.bottom - frame.top;
  int width = taskbar.IsVertical() ? INPUT_FLYOUT_VERTICAL_WIDTH : max( (int) ( bar.right - bar.left ), INPUT_FLYOUT_MIN_WIDTH );
  width = min( width, workWidth );

  // Outside the taskbar, so the bar (and Play) stay visible (TBR-F31).
  int x, y;
  switch( taskbar.edge )
  {
    case ABE_TOP:   x = preview.left - INPUT_FLYOUT_MARGIN; y = taskbar.taskbar.bottom; break;
    case ABE_LEFT:  x = taskbar.taskbar.right; y = preview.top - INPUT_FLYOUT_MARGIN; break;
    case ABE_RIGHT: x = taskbar.taskbar.left - width; y = preview.top - INPUT_FLYOUT_MARGIN; break;
    default:        x = preview.left - INPUT_FLYOUT_MARGIN; y = taskbar.taskbar.top - height; break;
  }
  x = max( (int) work.left, min( x, (int) work.right - width ) );
  y = max( (int) work.top, min( y, (int) work.bottom - height ) );

  SetWindowPos( m_hwnd, HWND_TOPMOST, x, y, width, height, SWP_SHOWWINDOW );
  LayoutEdit();
  SetForegroundWindow( m_hwnd );
  SetFocus( m_hEdit );
  const int length = GetWindowTextLength( m_hEdit );
  SendMessage( m_hEdit, EM_SETSEL, length, length );
  SendMessage( m_hEdit, EM_SCROLLCARET, 0, 0 );
}

void InputFlyout::Close( bool restoreFocus )
{
  if( !IsOpen() ) return;
  // Hand the focus back first: hiding the active window would otherwise let
  // Windows activate one of ours (SET-F63).
  if( restoreFocus && m_hwndRestore && IsWindow( m_hwndRestore ) )
    SetForegroundWindow( m_hwndRestore );
  ShowWindow( m_hwnd, SW_HIDE );
}

void InputFlyout::LayoutEdit()
{
  if( !m_hEdit ) return;
  RECT rc;
  GetClientRect( m_hwnd, &rc );
  MoveWindow( m_hEdit, INPUT_FLYOUT_MARGIN, INPUT_FLYOUT_MARGIN,
    max( 0, (int) rc.right - 2 * INPUT_FLYOUT_MARGIN ), max( 0, (int) rc.bottom - 2 * INPUT_FLYOUT_MARGIN ), TRUE );
}

LRESULT CALLBACK InputFlyout::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  InputFlyout * pThis = nullptr;
  if( uMsg == WM_NCCREATE )
  {
    pThis = (InputFlyout *) ( (CREATESTRUCT *) lParam )->lpCreateParams;
    SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
  }
  else
  {
    pThis = (InputFlyout *) GetWindowLongPtr( hwnd, GWLP_USERDATA );
  }
  if( !pThis )
    return DefWindowProc( hwnd, uMsg, wParam, lParam );

  switch( uMsg )
  {
    case WM_ACTIVATE:
      // Clicking anywhere else closes it; the focus goes where the user clicked.
      if( LOWORD( wParam ) == WA_INACTIVE && IsWindowVisible( hwnd ) )
        ShowWindow( hwnd, SW_HIDE );
      break;

    case WM_SIZE:
      pThis->LayoutEdit();
      break;

    case WM_ERASEBKGND:
    {
      RECT rc;
      GetClientRect( hwnd, &rc );
      FillRect( (HDC) wParam, &rc, pThis->m_hBackground );
      return 1;
    }

    // The edit's notifications belong to the bar (EN_CHANGE refreshes the
    // preview, EN_SETFOCUS shows the touch keyboard), as do Enter and Esc.
    case WM_COMMAND:
    case WM_EDIT_CONTROL_ENTER_PRESSED:
    case WM_EDIT_CONTROL_ESCAPE_PRESSED:
      if( pThis->m_hwndOwner )
        return SendMessage( pThis->m_hwndOwner, uMsg, wParam, lParam );
      break;

    case WM_CLOSE:
      pThis->Close( true );
      return 0;
  }
  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
