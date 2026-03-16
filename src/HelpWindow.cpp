#include "HelpWindow.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

static const wchar_t * HELP_WINDOW_CLASS = L"SimonSaysHelpWindow";

HelpWindow::HelpWindow() = default;

HelpWindow::~HelpWindow()
{
  if( m_backgroundBrush )
  {
    DeleteObject( m_backgroundBrush );
    m_backgroundBrush = NULL;
  }

  if( m_hFont )
  {
    DeleteObject( m_hFont );
    m_hFont = NULL;
  }

  if( m_hwnd )
  {
    DestroyWindow( m_hwnd );
    m_hwnd = NULL;
  }
}

bool HelpWindow::RegisterWindowClass( HINSTANCE hInstance )
{
  WNDCLASS wc = {};
  wc.lpfnWndProc = HelpWindow::WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = HELP_WINDOW_CLASS;
  wc.hbrBackground = (HBRUSH) CreateSolidBrush( GetTaskbarColor() );
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  return RegisterClass( &wc ) != 0;
}

bool HelpWindow::Create( HINSTANCE hInstance )
{
  m_hInstance = hInstance;

  if( !RegisterWindowClass( hInstance ) )
  {
    return false;
  }

  m_backgroundBrush = CreateSolidBrush( GetTaskbarColor() );

  RECT rcDesktop;
  GetWindowRect( GetDesktopWindow(), &rcDesktop );
  int x = ( ( rcDesktop.right - rcDesktop.left ) - m_defaultWidth ) / 2;
  int y = ( ( rcDesktop.bottom - rcDesktop.top ) - m_defaultHeight ) / 2;

  m_hwnd = CreateWindowEx(
    WS_EX_LAYERED,
    HELP_WINDOW_CLASS,
    L"SimonSays - Help",
    WS_POPUP | WS_THICKFRAME,
    x, y,
    m_defaultWidth, m_defaultHeight,
    NULL,
    NULL,
    hInstance,
    this
  );

  if( !m_hwnd )
  {
    return false;
  }

  DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
  DwmSetWindowAttribute( m_hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof( preference ) );

  BOOL useDarkMode = TRUE;
  if( GetRValue( GetTaskbarColor() ) > 128 ) useDarkMode = FALSE;
  DwmSetWindowAttribute( m_hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof( useDarkMode ) );

  COLORREF backgroundColor = GetTaskbarColor();
  DwmSetWindowAttribute( m_hwnd, DWMWA_CAPTION_COLOR, &backgroundColor, sizeof( backgroundColor ) );
  
  SetLayeredWindowAttributes( m_hwnd, 0, 239, LWA_ALPHA );

  if( GetRValue( GetTaskbarColor() ) < 128 )
  {
    m_textColor = RGB( 255, 255, 255 );
  }
  else
  {
    m_textColor = RGB( 0, 0, 0 );
  }

  LOGFONT lf = {};
  SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( LOGFONT ), &lf, 0 );
  m_hFont = CreateFontIndirect( &lf );

  m_hContent = CreateWindowEx(
    0,
    L"EDIT",
    L"",
    WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL,
    12, 12,
    m_defaultWidth - 24,
    m_defaultHeight - 24,
    m_hwnd,
    NULL,
    m_hInstance,
    NULL
  );

  SetWindowSubclass( m_hContent, EditSubclassProc, 0, 0 );

  if( m_hContent && m_hFont )
  {
    SendMessage( m_hContent, WM_SETFONT, (WPARAM) m_hFont, TRUE );
  }

  UpdateLayout();
  SetWindowText( m_hContent, ReplaceAll( GetLocalizedString( HELP_CONTENT_ID, m_language ), L"\n", L"\r\n" ).c_str() );
  ShowWindow( m_hwnd, SW_SHOW );
  UpdateWindow( m_hwnd );
  return true;
}

void HelpWindow::Show()
{
  if( m_hwnd )
  {
    ShowWindow( m_hwnd, SW_SHOW );
    UpdateWindow( m_hwnd );
    SetForegroundWindow( m_hwnd );
  }
}

void HelpWindow::Hide()
{
  if( m_hwnd )
  {
    ShowWindow( m_hwnd, SW_HIDE );
  }
}

bool HelpWindow::IsVisible() const
{
  return m_hwnd && IsWindowVisible( m_hwnd );
}

void HelpWindow::SetLanguage( const std::wstring & language )
{
  bool isRtl = IsLanguageRTL( language );
  updateRtlExStyle( m_hwnd, isRtl );
  EnumChildWindows( m_hwnd, ApplyRtlStylesCallback, isRtl );
  m_language = language;
  if( m_hContent )
  {
    SetWindowText( m_hContent, ReplaceAll( GetLocalizedString( HELP_CONTENT_ID, m_language ), L"\n", L"\r\n" ).c_str() );
    InvalidateRect( m_hContent, NULL, TRUE );
  }
}

void HelpWindow::UpdateLayout()
{
  if( !m_hwnd || !m_hContent ) return;

  RECT rc;
  GetClientRect( m_hwnd, &rc );
  int margin = 12;
  SetWindowPos( m_hContent, NULL, margin, margin,
    ( rc.right - rc.left ) - 2 * margin,
    ( rc.bottom - rc.top ) - 2 * margin,
    SWP_NOZORDER | SWP_NOACTIVATE );
  RedrawWindow( m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW );
}

LRESULT CALLBACK HelpWindow::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  HelpWindow * pThis = nullptr;

  if( uMsg == WM_CREATE )
  {
    CREATESTRUCT * pCreate = (CREATESTRUCT *) lParam;
    pThis = (HelpWindow *) pCreate->lpCreateParams;
    SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
  }
  else
  {
    pThis = (HelpWindow *) GetWindowLongPtr( hwnd, GWLP_USERDATA );
  }

  if( pThis )
  {
    switch( uMsg )
    {
      case WM_COMMAND:
        if( HIWORD( wParam ) == EN_VSCROLL )
        {
          InvalidateRect( (HWND) lParam, NULL, TRUE );
        }
        break;

      case WM_SIZE:
        pThis->UpdateLayout();
        break;

      case WM_CLOSE:
        pThis->Hide();
        return 0;

      case WM_NCHITTEST:
      {
        LRESULT hit = DefWindowProc( hwnd, uMsg, wParam, lParam );
        if( hit == HTCLIENT )
        {
          POINT pt = { (int) (short) LOWORD( lParam ), (int) (short) HIWORD( lParam ) };
          ScreenToClient( hwnd, &pt );

          if( pThis->m_hContent )
          {
            RECT rcContent;
            GetWindowRect( pThis->m_hContent, &rcContent );
            MapWindowPoints( NULL, hwnd, reinterpret_cast<LPPOINT>( &rcContent ), 2 );
            if( PtInRect( &rcContent, pt ) )
            {
              return HTCLIENT;
            }
          }

          return HTCAPTION;
        }
        return hit;
      }

      case WM_DESTROY:
        pThis->m_hwnd = NULL;
        break;

      case WM_ACTIVATE:
        if( LOWORD( wParam ) == WA_INACTIVE )
        {
          HWND hOtherWnd = (HWND) lParam;

          if( hOtherWnd != NULL )
          {
            DWORD myProcessId = GetCurrentProcessId();
            DWORD otherProcessId = 0;

            GetWindowThreadProcessId( hOtherWnd, &otherProcessId );

            if( myProcessId != otherProcessId )
            {
              ShowWindow( hwnd, SW_HIDE );
            }
          }
          else
          {
            ShowWindow( hwnd, SW_HIDE );
          }
        }
        break;

        //case WM_CTLCOLOREDIT:
      case WM_CTLCOLORSTATIC:
      {
        HDC hdcStatic = (HDC) wParam;
        SetTextColor( hdcStatic, pThis->m_textColor );
        SetBkMode( hdcStatic, TRANSPARENT );
        return (INT_PTR) pThis->m_backgroundBrush;
      }
    }
  }

  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK HelpWindow::EditSubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
  switch( uMsg )
  {
    case WM_VSCROLL:
    case WM_MOUSEWHEEL:
    {
      // 1. First, call the default procedure to handle the scroll and update the control's state
      LRESULT res = DefSubclassProc( hWnd, uMsg, wParam, lParam );

      // 2. Force the control to redraw immediately with the new scroll position  
      InvalidateRect( hWnd, NULL, TRUE );
      UpdateWindow( hWnd );

      return res;
    }
    case WM_NCDESTROY:
      RemoveWindowSubclass( hWnd, EditSubclassProc, uIdSubclass );
      break;
  }
  return DefSubclassProc( hWnd, uMsg, wParam, lParam );
}
