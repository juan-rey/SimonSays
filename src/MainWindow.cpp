#include "MainWindow.h"
#include "CategoryWindow.h"
#include "resource.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <sapi.h>
#pragma warning(disable:4996) 
#include <sphelper.h>
#pragma warning(default: 4996)


MainWindow::MainWindow()
  : m_hwnd( NULL ), m_hEditControl( NULL ), m_hPlayButton( NULL ), m_hCategoryButton( NULL ),
  m_hInstance( NULL ), m_categoryWindow( nullptr ), m_settings( RegistryManager::LoadSettingsFromRegistry() )
{
  ZeroMemory( &m_nid, sizeof( m_nid ) );

  HRESULT hr = CoCreateInstance( CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **) &pVoice );

  if( !m_settings.voice.empty() )
  {
    // Set the voice based on the saved setting
    ISpObjectToken * token;
    hr = SpGetTokenFromId( m_settings.voice.c_str(), &token, FALSE );

    if( SUCCEEDED( hr ) )
    {
      pVoice->SetVoice( token );
    }
  }
  pVoice->SetVolume( m_settings.volume );
  pVoice->SetRate( m_settings.rate );

}

MainWindow::~MainWindow()
{
  if( m_categoryWindow )
  {
    m_categoryWindow.reset();
  }

  //RemoveTrayIcon();

  // Release ISpVoice object
  if( pVoice )
    pVoice->Release();
}

bool MainWindow::Create( HINSTANCE hInstance, int nCmdShow )
{
  RECT rc;
  int width = 400;
  int height = 45;

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

  SetLayeredWindowAttributes( m_hwnd, RGB( 0, 0, 0 ), 0, LWA_COLORKEY );
  SetWindowPos( m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
  ShowWindow( m_hwnd, nCmdShow );

  if( !CreateTaskbarControls() )
  {
    return false;
  }

  //CreateTrayIcon();

  m_categories = RegistryManager::LoadCategoriesFromRegistry( m_settings.language );
  //m_currentLanguage = RegistryManager::GetSystemLanguage();

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
  SetWindowText( m_hPlayButton, PLAY_BUTTON_TEXT_PLAYING );

  wchar_t buffer[2048];
  GetWindowText( m_hEditControl, buffer, sizeof( buffer ) / sizeof( wchar_t ) );

  std::wstring text = buffer;
  std::wstring delim = SOUND_NOTE_DELIMITER;

  auto trim = []( std::wstring & s )
    {
      // trim whitespace on both ends
      while( !s.empty() && iswspace( s.front() ) ) s.erase( 0, 1 );
      while( !s.empty() && iswspace( s.back() ) ) s.pop_back();
    };

  size_t pos = 0;
  while( pos < text.length() )
  {
    size_t start = text.find( delim, pos );
    if( start == std::wstring::npos )
    {
      // remaining speech
      std::wstring segment = text.substr( pos );
      trim( segment );
      if( !segment.empty() && pVoice )
      {
        HRESULT hr = pVoice->Speak( segment.c_str(), 0, nullptr );
        if( SUCCEEDED( hr ) ) pVoice->WaitUntilDone( INFINITE );
      }
      break;
    }

    if( start > pos )
    {
      std::wstring segment = text.substr( pos, start - pos );
      trim( segment );
      if( !segment.empty() && pVoice )
      {
        HRESULT hr = pVoice->Speak( segment.c_str(), 0, nullptr );
        if( SUCCEEDED( hr ) ) pVoice->WaitUntilDone( INFINITE );
      }
    }

    size_t end = text.find( delim, start + delim.length() );
    if( end == std::wstring::npos )
    {
      // unmatched delimiter - treat rest as speech
      std::wstring segment = text.substr( start );
      trim( segment );
      if( !segment.empty() && pVoice )
      {
        HRESULT hr = pVoice->Speak( segment.c_str(), 0, nullptr );
        if( SUCCEEDED( hr ) ) pVoice->WaitUntilDone( INFINITE );
      }
      break;
    }

    // extract filename between delimiters
    std::wstring filename = text.substr( start + delim.length(), end - ( start + delim.length() ) );
    trim( filename );
    if( !filename.empty() )
    {
      if( filename.find_last_of( L"." ) != std::wstring::npos )
      {
        std::wstring ext = filename.substr( filename.find_last_of( L"." ) + 1 );
        for( auto & c : ext ) c = towlower( c );
        if( ext == L"wav" || ext == L"mid" || ext == L"midi" )
        {
          // play sound file asynchronously
          PlaySound( filename.c_str(), NULL, SND_FILENAME | SND_SYNC );
        }
      }
    }

    pos = end + delim.length();
  }

  SetWindowText( m_hPlayButton, PLAY_BUTTON_TEXT );
}

void MainWindow::AddTextToEditControl( const std::wstring & text )
{
  if( !m_hEditControl ) return;
  wchar_t buffer[512];
  GetWindowText( m_hEditControl, buffer, 512 );
  std::wstring currentText = buffer;
  if( !currentText.empty() )
  {
    currentText += L" ";
  }
  currentText += text;
  SetWindowText( m_hEditControl, currentText.c_str() );
  UpdateWindow( m_hEditControl );
}

void MainWindow::SetEditControlText( const std::wstring & text )
{
  if( !m_hEditControl ) return;
  SetWindowText( m_hEditControl, text.c_str() );
  UpdateWindow( m_hEditControl );
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
        //pThis->HandleTrayMessage( lParam );
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
  wc.hbrBackground = (HBRUSH) GetStockObject( BLACK_BRUSH );
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
    WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
    margin + buttonWidth + 10, margin, editWidth, buttonHeight,
    m_hwnd,
    (HMENU) IDC_EDIT_PHRASE,
    m_hInstance,
    NULL
  );

  if( !m_hEditControl ) return false;

  m_hPlayButton = CreateWindow(
    L"BUTTON",
    PLAY_BUTTON_TEXT,
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON /*| BS_FLAT */,
    margin + buttonWidth + 10 + editWidth + 10, margin, buttonWidth, buttonHeight,
    m_hwnd,
    (HMENU) IDC_BUTTON_PLAY,
    m_hInstance,
    NULL
  );

  if( !m_hPlayButton ) return false;

  NONCLIENTMETRICS ncm = {};
  SystemParametersInfo( SPI_GETNONCLIENTMETRICS, sizeof( NONCLIENTMETRICS ), &ncm, 0 );
  //ncm.lfMenuFont.lfWeight = 600;
  HFONT message_font = CreateFontIndirect( &ncm.lfMenuFont );

  SendMessage( m_hCategoryButton, WM_SETFONT, (WPARAM) message_font, TRUE );
  SendMessage( m_hEditControl, WM_SETFONT, (WPARAM) message_font, TRUE );
  SendMessage( m_hPlayButton, WM_SETFONT, (WPARAM) message_font, TRUE );

  // Center text vertically in edit control
  HDC hdc = GetDC( m_hEditControl );
  // Get the font used in the edit control
  HFONT hFont = (HFONT) SendMessage( m_hEditControl, WM_GETFONT, 0, 0 );
  HFONT hOldFont = (HFONT) SelectObject( hdc, hFont );

  TEXTMETRIC tm;
  GetTextMetrics( hdc, &tm );
  int iTextHeight = tm.tmHeight;

  // Restore the original font and release the device context
  SelectObject( hdc, hOldFont );
  ReleaseDC( m_hEditControl, hdc );

  GetClientRect( m_hEditControl, &rect );
  rect.top = ( rect.bottom - iTextHeight ) / 2;
  rect.bottom -= rect.top;
  SendMessage( m_hEditControl, EM_SETRECT, 0, (LPARAM) &rect );
  if( m_settings.useDefaultText )
    SetEditControlText( m_settings.defaultText );

  return true;
}

/*
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
*/