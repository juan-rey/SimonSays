#include "HelpWindow.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

static const wchar_t * HELP_WINDOW_CLASS = L"SimonSaysHelpWindow";

HelpWindow::HelpWindow() = default;

HelpWindow::~HelpWindow()
{
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

  SetLayeredWindowAttributes( m_hwnd, 0, 239, LWA_ALPHA );

  LOGFONT lf = {};
  SystemParametersInfo( SPI_GETICONTITLELOGFONT, sizeof( LOGFONT ), &lf, 0 );
  m_hFont = CreateFontIndirect( &lf );

  m_hContent = CreateWindowEx(
    0,
    L"STATIC",
    L"",
    WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX | SS_NOTIFY,
    12, 12,
    m_defaultWidth - 24,
    m_defaultHeight - 24,
    m_hwnd,
    NULL,
    m_hInstance,
    NULL
  );

  if( m_hContent && m_hFont )
  {
    SendMessage( m_hContent, WM_SETFONT, (WPARAM) m_hFont, TRUE );
  }

  UpdateLayout();
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

void HelpWindow::SetText( const std::wstring & text )
{
  m_text = text;
  if( m_hContent )
  {
    SetWindowText( m_hContent, m_text.c_str() );
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
          ShowWindow( hwnd, SW_HIDE );
        }
        break;

    }
  }

  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
