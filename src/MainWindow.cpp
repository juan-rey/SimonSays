#include "MainWindow.h"
#include "CategoryWindow.h"
#include "resource.h"



MainWindow::MainWindow()
  : m_hwnd( NULL ), m_hEditControl( NULL ), m_hPlayButton( NULL ), m_hCategoryButton( NULL ),
  m_hInstance( NULL ), m_categoryWindow( nullptr )
{
  ZeroMemory( &m_nid, sizeof( m_nid ) );


  // Crear una instancia del motor TTS (ISpVoice)

  HRESULT hr = CoCreateInstance( CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **) &pVoice );
}

MainWindow::~MainWindow()
{
  if( m_categoryWindow )
  {
    m_categoryWindow.reset();
  }

  RemoveTrayIcon();

  // Release ISpVoice object
  if( pVoice )
    pVoice->Release();
}

bool MainWindow::Create( HINSTANCE hInstance, int nCmdShow )
{
  int width = 400;
  int height = 45;
  RECT rc;
  m_hInstance = hInstance;

  if( !RegisterWindowClass( hInstance ) )
  {
    return false;
  }

  GetWindowRect( GetDesktopWindow(), &rc );
  int x = 0;
  int y = ( rc.bottom - height ) - 0;
  m_hwnd = CreateWindowEx(
    WS_EX_LAYERED | WS_EX_TOPMOST,
    CLASS_NAME,
    L"SimonSays",
    WS_POPUP | WS_VISIBLE,
    x, y,
    width, height,
    NULL,
    NULL,
    hInstance,
    this
  );

  if( !m_hwnd )
  {
    return false;
  }
  LOGBRUSH logBrush;
  GetObject( (HBRUSH) COLOR_BACKGROUND, sizeof( LOGBRUSH ), &logBrush );
  COLORREF Color = logBrush.lbColor;
  SetLayeredWindowAttributes( m_hwnd, Color, 255, LWA_ALPHA | LWA_COLORKEY );
  SetWindowPos( m_hwnd, HWND_TOPMOST, 0, 0, 0, 0,
    SWP_NOMOVE | SWP_NOSIZE );

  ShowWindow( m_hwnd, nCmdShow );

  if( !CreateTaskbarControls() )
  {
    return false;
  }

  CreateTrayIcon();

  m_categories = RegistryManager::LoadCategoriesFromRegistry();
  m_currentLanguage = RegistryManager::GetSystemLanguage();

  m_categoryWindow = std::make_unique<CategoryWindow>( this );
  if( !m_categoryWindow->Create( hInstance ) )
  {
    return false;
  }
  m_categoryWindow->UpdateCategories( m_categories );

  ShowWindow( m_hwnd, nCmdShow );
  UpdateWindow( m_hwnd );

  return true;
}

void MainWindow::RunMessageLoop()
{
  MSG msg = {};
  while( GetMessage( &msg, NULL, 0, 0 ) )
  {
    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }
}

void MainWindow::ShowCategoryWindow()
{
  if( m_categoryWindow )
  {
    m_categoryWindow->Show();
  }
}

void MainWindow::UpdateTaskbarUI()
{
  if( m_hEditControl )
  {
    InvalidateRect( m_hEditControl, NULL, TRUE );
    UpdateWindow( m_hEditControl );
  }
  if( m_hPlayButton )
  {
    InvalidateRect( m_hPlayButton, NULL, TRUE );
    UpdateWindow( m_hPlayButton );
  }
  if( m_hCategoryButton )
  {
    InvalidateRect( m_hCategoryButton, NULL, TRUE );
    UpdateWindow( m_hCategoryButton );
  }
}

void MainWindow::PlayCurrentText()
{
  if( !m_hEditControl ) return;

  wchar_t buffer[256];
  GetWindowText( m_hEditControl, buffer, 256 );

  HRESULT hr = pVoice->Speak( buffer, 0, nullptr );
  if( SUCCEEDED( hr ) )
  {
    pVoice->WaitUntilDone( INFINITE );
  }
}
#define TIMER_CHECK_ZORDER 1

LRESULT CALLBACK MainWindow::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  static UINT_PTR timerId = 0;
  MainWindow * pThis = nullptr;

  if( uMsg == WM_CREATE )
  {
    CREATESTRUCT * pCreate = (CREATESTRUCT *) lParam;
    pThis = (MainWindow *) pCreate->lpCreateParams;
    SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
    timerId = SetTimer( hwnd, TIMER_CHECK_ZORDER, 5000, NULL );
  }
  else
  {
    pThis = (MainWindow *) GetWindowLongPtr( hwnd, GWLP_USERDATA );
  }

  if( pThis )
  {
    switch( uMsg )
    {
      case WM_COMMAND:
      {
        int wmId = LOWORD( wParam );
        int wmEvent = HIWORD( wParam );

        if( wmEvent == BN_CLICKED )
        {
          if( wmId == IDC_BUTTON_PLAY )
          {
            pThis->PlayCurrentText();
          }
          else if( wmId == IDC_BUTTON_CATEGORIES )
          {
            pThis->ShowCategoryWindow();
          }
        }
        else if( wmEvent == EN_CHANGE )
        {
          if( wmId == IDC_EDIT_PHRASE )
          {
            pThis->UpdateTaskbarUI();
          }
        }
      }
      break;

      case WM_TRAYICON:
        pThis->HandleTrayMessage( lParam );
        break;

      case WM_SIZE:
        pThis->UpdateTaskbarUI();
        break;

      case WM_DESTROY:
        if( timerId ) KillTimer( hwnd, timerId );
        PostQuitMessage( 0 );
        break;

      case WM_TIMER:
        if( wParam == TIMER_CHECK_ZORDER )
        {
          /*HWND hwndActive = GetForegroundWindow();
          wchar_t className[256];
          GetClassName( hwndActive, className, 256 );

          if( wcscmp( className, CLASS_NAME ) != 0 )*/
          if( GetForegroundWindow() != hwnd )
          {
            OutputDebugString( L"Detectado\n" );
            SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
          }
        }
        break;

      default:
        return DefWindowProc( hwnd, uMsg, wParam, lParam );
    }
  }

  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

bool MainWindow::RegisterWindowClass( HINSTANCE hInstance )
{
  WNDCLASS wc = {};
  wc.lpfnWndProc = MainWindow::WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  wc.hbrBackground = (HBRUSH) ( COLOR_BACKGROUND + 1 );// ( COLOR_WINDOW + 1 );
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );

  return RegisterClass( &wc ) != 0;
}

bool MainWindow::CreateTaskbarControls()
{
  if( !m_hwnd ) return false;

  RECT rect;
  GetClientRect( m_hwnd, &rect );

  int buttonWidth = 80;
  int buttonHeight = 30;
  int margin = 10;
  int editWidth = rect.right - 2 * margin - 2 * buttonWidth - 20;

  m_hCategoryButton = CreateWindow(
    L"BUTTON",
    L"Categories",
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    margin, margin, buttonWidth, buttonHeight,
    m_hwnd,
    (HMENU) IDC_BUTTON_CATEGORIES,
    m_hInstance,
    NULL
  );

  if( !m_hCategoryButton ) return false;

  m_hEditControl = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    L"EDIT",
    L"",
    WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
    margin + buttonWidth + 10, margin, editWidth, buttonHeight,
    m_hwnd,
    (HMENU) IDC_EDIT_PHRASE,
    m_hInstance,
    NULL
  );

  if( !m_hEditControl ) return false;

  m_hPlayButton = CreateWindow(
    L"BUTTON",
    L"Play",
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    margin + buttonWidth + 10 + editWidth + 10, margin, buttonWidth, buttonHeight,
    m_hwnd,
    (HMENU) IDC_BUTTON_PLAY,
    m_hInstance,
    NULL
  );

  if( !m_hPlayButton ) return false;

  SendMessage( m_hCategoryButton, WM_SETFONT, (WPARAM) GetStockObject( DEFAULT_GUI_FONT ), TRUE );
  SendMessage( m_hEditControl, WM_SETFONT, (WPARAM) GetStockObject( DEFAULT_GUI_FONT ), TRUE );
  SendMessage( m_hPlayButton, WM_SETFONT, (WPARAM) GetStockObject( DEFAULT_GUI_FONT ), TRUE );

  return true;
}

void MainWindow::CreateTrayIcon()
{
  m_nid.cbSize = sizeof( NOTIFYICONDATA );
  m_nid.hWnd = m_hwnd;
  m_nid.uID = 1;
  m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  m_nid.uCallbackMessage = WM_TRAYICON;
  m_nid.hIcon = LoadIcon( m_hInstance, MAKEINTRESOURCE( IDI_SIMONSAYS ) );

  if( m_nid.hIcon == NULL )
  {
    m_nid.hIcon = LoadIcon( NULL, IDI_APPLICATION );
  }

  wcscpy_s( m_nid.szTip, TRAY_TOOLTIP );

  Shell_NotifyIcon( NIM_ADD, &m_nid );
}

void MainWindow::RemoveTrayIcon()
{
  if( m_nid.cbSize > 0 )
  {
    Shell_NotifyIcon( NIM_DELETE, &m_nid );
  }
}

void MainWindow::ShowContextMenu( HWND hwnd, POINT pt )
{
  HMENU hMenu = CreatePopupMenu();
  if( hMenu )
  {
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_SHOW, L"Show Categories" );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_EXIT, L"Exit" );

    SetForegroundWindow( hwnd );
    TrackPopupMenu( hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL );
    DestroyMenu( hMenu );
  }
}

void MainWindow::HandleTrayMessage( UINT message )
{
  switch( message )
  {
    case WM_LBUTTONDBLCLK:
      ShowCategoryWindow();
      break;

    case WM_RBUTTONUP:
    {
      POINT pt;
      GetCursorPos( &pt );
      ShowContextMenu( m_hwnd, pt );
    }
    break;

    case WM_COMMAND:
    {
      int wmId = LOWORD( message );
      if( wmId == ID_TRAY_SHOW )
      {
        ShowCategoryWindow();
      }
      else if( wmId == ID_TRAY_EXIT )
      {
        DestroyWindow( m_hwnd );
      }
    }
    break;
  }
}