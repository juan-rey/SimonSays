#include "MainWindow.h"
#include "CategoryWindow.h"
#include "resource.h"
#include "utils.h"
#include "localized_strings.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <sapi.h>
#pragma warning(disable:4996) 
#include <sphelper.h>
#pragma warning(default: 4996)
#pragma comment(lib, "version.lib")

struct SettingsDialogContext
{
  MainWindow * owner = nullptr;
  Settings tempSettings;
  std::vector<VoiceInfo> voices;
  std::vector<LanguageInfo> languages;
  bool accepted = false;
};

// When the taskbar is clicked MainWindow can lose its Z-order and get covered by the taskbar.
// Track Z-order and force MainWindow back on top when that happens.
// This is a global variable to store the MainWindow handle for the hook to access.
HWND g_hwndMain = NULL;
// We also store the Taskbar handle for easier access
HWND g_hwndShell_TrayWnd = NULL;
// Hook handles
HHOOK g_hMouseHook = NULL;

// Timer definitions
#define TIMER_CHECK_ZORDER 1
#define SLOW_TIMER_CHECK_ZORDER_INTERVAL 5000
#define FAST_TIMER_CHECK_ZORDER_INTERVAL 500

MainWindow::MainWindow()
  : m_hwnd( NULL ), m_hEditControl( NULL ), m_hPlayButton( NULL ), m_hCategoryButton( NULL ),
  m_hInstance( NULL ), m_categoryWindow( nullptr ), m_settings( RegistryManager::LoadSettingsFromRegistry() ), m_hAccel( NULL )
{
  ZeroMemory( &m_nid, sizeof( m_nid ) );

  HRESULT hr = CoCreateInstance( CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **) &m_pVoice );
  ApplyVoiceSettings();

  std::wstring versionInRegistry = RegistryManager::GetLastRunVersionToRegistry();
  if( !versionInRegistry.empty() && versionInRegistry != GetProductVersionString() )
  {
    // Version has changed since last run
  }

  // Workaround for Aholab voice not speaking immediately the first time
  if( m_settings.voice.find( L"Aholab" ) != std::wstring::npos && m_pVoice )
  {
    m_pVoice->Speak( L" ", SPF_ASYNC | SPF_IS_NOT_XML, nullptr );
  }
}

MainWindow::~MainWindow()
{
  if( m_categoryWindow )
  {
    m_categoryWindow.reset();
  }

  RemoveTrayIcon();

  // Release ISpVoice object
  if( m_pVoice )
    m_pVoice->Release();

  RegistryManager::SaveVersionToRegistry( GetProductVersionString() );
}

bool MainWindow::Create( HINSTANCE hInstance, int nCmdShow )
{
  RECT rc;
  int width = 400;
  int height = 46;

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

  SetLayeredWindowAttributes( m_hwnd, GetTaskbarColor() /*RGB( 0, 0, 0 )*/, 0, LWA_COLORKEY );
  SetWindowPos( m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
  ShowWindow( m_hwnd, nCmdShow );

  if( !CreateTaskbarControls() )
  {
    return false;
  }

  CreateTrayIcon();

  m_categories = RegistryManager::LoadCategoriesFromRegistry( m_settings.language );

  m_categoryWindow = std::make_unique<CategoryWindow>( this, m_settings.rememberCategoryWindowSize, m_settings.minimizeCategoryWindowAutomatically );
  if( !m_categoryWindow->Create( hInstance ) )
  {
    return false;
  }
  m_categoryWindow->UpdateCategories( m_categories );

  ShowWindow( m_hwnd, nCmdShow );
  UpdateWindow( m_hwnd );

  if( !m_hAccel )
  {
    m_hAccel = LoadAccelerators( m_hInstance, MAKEINTRESOURCE( IDR_MAINACCEL ) );
  }

  // Calculate center point of category button to ensure Taskbar is over it when checking Z Order
  GetWindowRect( m_hCategoryButton, &rc );
  m_inButtonPoint.x = rc.left + ( rc.right - rc.left ) / 2;
  m_inButtonPoint.y = rc.top + ( rc.bottom - rc.top ) / 2;

  return true;
}

void MainWindow::RunMessageLoop()
{
  MSG msg = {};
  while( GetMessage( &msg, NULL, 0, 0 ) )
  {
    if( !m_hAccel || !TranslateAccelerator( m_hwnd, m_hAccel, &msg ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
  }
}

void MainWindow::ShowHideCategoryWindow()
{
  if( m_categoryWindow )
  {
    if( !m_categoryWindow->IsVisible() )
      m_categoryWindow->Show();
    else
      m_categoryWindow->Hide();
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

void MainWindow::UpdateUILanguage( const std::wstring language )
{
  if( m_hPlayButton )
  {
    SetWindowText( m_hPlayButton, GetLocalizedString( PLAY_BUTTON_TEXT_ID, language ) );
  }
  if( m_hCategoryButton )
  {
    SetWindowText( m_hCategoryButton, GetLocalizedString( CATEGORIES_BUTTON_TEXT_ID, language ) );
  }
  if( m_categoryWindow )
  {
    m_categoryWindow->UpdateUILanguage( language );
  }
}

void MainWindow::PlayCurrentText()
{
  if( !m_hEditControl ) return;
  SetWindowText( m_hPlayButton, GetLocalizedString( PLAY_BUTTON_TEXT_PLAYING_ID, m_settings.language ) );

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
      if( !segment.empty() && m_pVoice )
      {
        HRESULT hr = m_pVoice->Speak( segment.c_str(), 0, nullptr );
        if( SUCCEEDED( hr ) ) m_pVoice->WaitUntilDone( INFINITE );
      }
      break;
    }

    if( start > pos )
    {
      std::wstring segment = text.substr( pos, start - pos );
      trim( segment );
      if( !segment.empty() && m_pVoice )
      {
        HRESULT hr = m_pVoice->Speak( segment.c_str(), 0, nullptr );
        if( SUCCEEDED( hr ) ) m_pVoice->WaitUntilDone( INFINITE );
      }
    }

    size_t end = text.find( delim, start + delim.length() );
    if( end == std::wstring::npos )
    {
      // unmatched delimiter - treat rest as speech
      std::wstring segment = text.substr( start );
      trim( segment );
      if( !segment.empty() && m_pVoice )
      {
        HRESULT hr = m_pVoice->Speak( segment.c_str(), 0, nullptr );
        if( SUCCEEDED( hr ) ) m_pVoice->WaitUntilDone( INFINITE );
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

  SetWindowText( m_hPlayButton, GetLocalizedString( PLAY_BUTTON_TEXT_ID, m_settings.language ) );
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
  if( m_settings.speakDirectlyWhenClickingPhrase )
  {
    PlayCurrentText();
  }
}

// Low Level Mouse Hook to detect clicks on the Taskbar
LRESULT CALLBACK MainWindow::LowLevelMouseProc( int nCode, WPARAM wParam, LPARAM lParam )
{
  if( nCode == HC_ACTION )
  {
    if( wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN || wParam == WM_MBUTTONDOWN || wParam == WM_XBUTTONDOWN || wParam == WM_LBUTTONUP )
    {
      const MSLLHOOKSTRUCT * mouseInfo = reinterpret_cast<MSLLHOOKSTRUCT *>( lParam );
      HWND hwndTarget = WindowFromPoint( mouseInfo->pt );
      // Taskbar children (e.g., secondary tray windows) are handled by walking parents
      //HWND hwndRoot = GetAncestor( hwndTarget, GA_ROOT );
      //if( IsTaskbarWindowS( hwndRoot ? hwndRoot : hwndTarget ) )
      if( hwndTarget )
      {
        bool isTaskbar = false;
        while( hwndTarget && !isTaskbar )
        {
          if( hwndTarget == g_hwndShell_TrayWnd )
          {
            isTaskbar = true;
          }
          else
          {
            hwndTarget = GetParent( hwndTarget );
          }
        }
        if( isTaskbar )
        {
          OutputDebugString( L"Taskbar click detected\n" );
          if( g_hwndMain )
          {
            PostMessage( g_hwndMain, WM_TIMER, TIMER_CHECK_ZORDER, 0 );
          }
        }
      }
    }
  }
  return CallNextHookEx( g_hMouseHook, nCode, wParam, lParam );
}

LRESULT CALLBACK MainWindow::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  static UINT_PTR s_slowZOrderCheckTimerId = 0;
  static UINT_PTR s_fastZOrderCheckTimerId = 0;
  MainWindow * pThis = nullptr;

  if( uMsg == WM_CREATE )
  {
    CREATESTRUCT * pCreate = (CREATESTRUCT *) lParam;
    pThis = (MainWindow *) pCreate->lpCreateParams;
    SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
    // Start slow timer to check Z Order
    s_slowZOrderCheckTimerId = SetTimer( hwnd, TIMER_CHECK_ZORDER, SLOW_TIMER_CHECK_ZORDER_INTERVAL, NULL );
    g_hwndMain = hwnd;
    // Store Taskbar handle
    g_hwndShell_TrayWnd = FindWindow( L"Shell_TrayWnd", NULL );
    // Set Low Level Mouse Hook to detect clicks on the Taskbar
    g_hMouseHook = SetWindowsHookEx( WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle( NULL ), 0 );
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

        if( wmId == ID_SETTINGS_OPEN )
        {
          pThis->ShowSettingsDialog();
          break;
        }
        else if( wmId == ID_TRAY_SHOW )
        {
          if( !IsWindowVisible( hwnd ) )
          {
            ShowWindow( hwnd, SW_SHOW );
            SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
            PostMessage( hwnd, WM_TIMER, TIMER_CHECK_ZORDER, 0 );
          }
          else
          {
            ShowWindow( hwnd, SW_HIDE );
          }
          break;
        }
        else if( wmId == ID_TRAY_EXIT )
        {
          DestroyWindow( hwnd );
          break;
        }

        if( wmEvent == BN_CLICKED )
        {
          if( wmId == IDC_BUTTON_PLAY )
          {
            pThis->PlayCurrentText();
          }
          else if( wmId == IDC_BUTTON_CATEGORIES )
          {
            pThis->ShowHideCategoryWindow();
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
        switch( lParam )
        {
          case WM_LBUTTONUP:
          case WM_LBUTTONDBLCLK:
            ShowWindow( hwnd, SW_SHOW );
            SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
            PostMessage( hwnd, WM_TIMER, TIMER_CHECK_ZORDER, 0 );
            break;

          case WM_RBUTTONUP:
          {
            POINT pt;
            GetCursorPos( &pt );
            pThis->ShowContextMenu( hwnd, pt );
          }
          break;
        }
        break;

      case WM_SIZE:
        pThis->UpdateTaskbarUI();
        break;

      case WM_DESTROY:
        if( s_slowZOrderCheckTimerId ) KillTimer( hwnd, s_slowZOrderCheckTimerId );
        if( s_fastZOrderCheckTimerId ) KillTimer( hwnd, s_fastZOrderCheckTimerId );
        if( g_hMouseHook )
        {
          UnhookWindowsHookEx( g_hMouseHook );
          g_hMouseHook = NULL;
        }
        PostQuitMessage( 0 );
        break;

      case WM_TIMER:
        if( wParam == TIMER_CHECK_ZORDER ) // Check Z Order Timer
        {
          HWND hwndForeground = GetForegroundWindow();
          while( hwndForeground && GetParent( hwndForeground ) )
          {
            hwndForeground = GetParent( hwndForeground );
          }

          if( hwndForeground != hwnd && IsWindowVisible( hwnd ) ) // If our window is not focused and visible perform the check
          {
            OutputDebugString( L"Checing Z Order\n" );
            // Check if our window is still on top of the taskbar
            HWND hwndAtPoint = WindowFromPoint( pThis->m_inButtonPoint );
            if( hwndAtPoint ) // Window found at the center of the Category Button
            {
              wchar_t className[256];
              OutputDebugString( ( L"Window from point: " + std::to_wstring( (uintptr_t) hwndAtPoint ) + L"\n" ).c_str() );
              GetClassName( hwndAtPoint, className, 256 );
              OutputDebugString( ( L"Class name: " + std::wstring( className ) + L"\n" ).c_str() );
              // Check if the window at that point is the Taskbar or a child of the Taskbar
              if( wcscmp( className, L"Shell_TrayWnd" ) == 0 )
              {
                SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
                // The above is not enough sometimes, we need to use a fast timer to keep checking Z Order
                // specially when Start Menu is opened. The only way to revert this is click on another application window and repeat the process
                if( !s_fastZOrderCheckTimerId )
                {
                  s_fastZOrderCheckTimerId = SetTimer( hwnd, TIMER_CHECK_ZORDER, FAST_TIMER_CHECK_ZORDER_INTERVAL, NULL );
                }
                // I have found that clicking the "^" of the "TrayNotifyWnd" helps to restore Z Order
                // so I have to research how to do that programmatically
                //PostMessage( hwnd, WM_TRAYICON, WM_LBUTTONUP, 0 );
              }
              else // Our window is on top
              {
                // Stop fast timer if running
                if( s_fastZOrderCheckTimerId )
                {
                  KillTimer( hwnd, s_fastZOrderCheckTimerId );
                  s_fastZOrderCheckTimerId = 0;
                }
              }
            }
            SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
          }
          else // If our window is focused or not visible, stop fast timer if running
          {
            if( s_fastZOrderCheckTimerId )
            {
              KillTimer( hwnd, s_fastZOrderCheckTimerId );
              s_fastZOrderCheckTimerId = 0;
            }
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
  wc.hbrBackground = (HBRUSH) CreateSolidBrush( GetTaskbarColor() ); // GetStockObject( BLACK_BRUSH );
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );

  return RegisterClass( &wc ) != 0;
}

bool MainWindow::CreateTaskbarControls()
{
  if( !m_hwnd ) return false;

  RECT rect;
  GetClientRect( m_hwnd, &rect );

  int buttonWidth = 80;
  int buttonHeight = 32;
  int horMargin = 10;
  int vertMargin = ( rect.bottom - buttonHeight ) / 2;
  int editWidth = rect.right - 4 * horMargin - 2 * buttonWidth;

  m_hCategoryButton = CreateWindow(
    L"BUTTON",
    GetLocalizedString( CATEGORIES_BUTTON_TEXT_ID, m_settings.language ),
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    horMargin, vertMargin, buttonWidth, buttonHeight,
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
    horMargin + buttonWidth + horMargin, vertMargin, editWidth, buttonHeight,
    m_hwnd,
    (HMENU) IDC_EDIT_PHRASE,
    m_hInstance,
    NULL
  );

  if( !m_hEditControl ) return false;

  m_hPlayButton = CreateWindow(
    L"BUTTON",
    GetLocalizedString( PLAY_BUTTON_TEXT_ID, m_settings.language ),
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON /*| BS_FLAT */,
    horMargin + buttonWidth + horMargin + editWidth + horMargin, vertMargin, buttonWidth, buttonHeight,
    m_hwnd,
    (HMENU) IDC_BUTTON_PLAY,
    m_hInstance,
    NULL
  );

  if( !m_hPlayButton ) return false;

  NONCLIENTMETRICS ncm = {};
  SystemParametersInfo( SPI_GETNONCLIENTMETRICS, sizeof( NONCLIENTMETRICS ), &ncm, 0 );
  HFONT message_font = CreateFontIndirect( &ncm.lfMenuFont );

  SendMessage( m_hCategoryButton, WM_SETFONT, (WPARAM) message_font, TRUE );
  SendMessage( m_hPlayButton, WM_SETFONT, (WPARAM) message_font, TRUE );

  ncm.lfMenuFont.lfWeight = 600;
  message_font = CreateFontIndirect( &ncm.lfMenuFont );
  SendMessage( m_hEditControl, WM_SETFONT, (WPARAM) message_font, TRUE );
  CenterEditTextVertically( m_hEditControl );
  if( m_settings.useDefaultText )
  {
    if( !m_hEditControl )
    {
      SetWindowText( m_hEditControl, m_settings.defaultText.c_str() );
      UpdateWindow( m_hEditControl );
    }
  }

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

  wcscpy_s( m_nid.szTip, GetLocalizedString( TRAYICON_TOOLTIP_ID, m_settings.language ) );

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
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_SHOW, GetLocalizedString( TRAYICON_SHOW_HIDE_ID, m_settings.language ) );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_EXIT, GetLocalizedString( TRAYICON_EXIT_ID, m_settings.language ) );

    SetForegroundWindow( hwnd );
    TrackPopupMenu( hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL );
    DestroyMenu( hMenu );
  }
}

void MainWindow::ApplyVoiceSettings()
{
  if( !m_pVoice ) return;

  if( !m_settings.voice.empty() )
  {
    ISpObjectToken * token = nullptr;
    if( SUCCEEDED( SpGetTokenFromId( m_settings.voice.c_str(), &token, FALSE ) ) )
    {
      m_pVoice->SetVoice( token );
      token->Release();
    }
  }

  m_pVoice->SetVolume( CLAMPED_VOICE_VOLUME( m_settings.volume ) );
  m_pVoice->SetRate( CLAMPED_VOICE_RATE( m_settings.rate ) );
}

void MainWindow::ShowSettingsDialog()
{
  SettingsDialogContext context;
  context.owner = this;
  context.tempSettings = m_settings;
  context.voices = RegistryManager::PopulateAvaibleVoicesFromRegistry( context.tempSettings.language.empty() ? GetSystemLanguage() : context.tempSettings.language );
  if( context.voices.empty() )
  {
    context.voices = RegistryManager::PopulateAvaibleVoicesFromRegistry();
  }
  context.languages.assign( SUPPORTED_LANGUAGES.begin(), SUPPORTED_LANGUAGES.end() );

  DialogBoxParam( m_hInstance, MAKEINTRESOURCE( IDD_SETTINGS_DIALOG ), m_hwnd, MainWindow::SettingsDialogProc, (LPARAM) &context );

  if( context.accepted )
  {
    if( context.tempSettings.language != m_settings.language )
    {
      m_categories = RegistryManager::LoadCategoriesFromRegistry( context.tempSettings.language );
      if( m_categoryWindow )
      {
        m_categoryWindow->UpdateCategories( m_categories );
        //m_categoryWindow->UpdateUILanguage( context.tempSettings.language );
      }
      UpdateUILanguage( context.tempSettings.language );
    }

    if( m_categoryWindow )
    {
      m_categoryWindow->SetMinimizeWhenLosingFocus( context.tempSettings.minimizeCategoryWindowAutomatically );
      m_categoryWindow->SetRememberWindowSize( context.tempSettings.rememberCategoryWindowSize );
    }
    m_settings = context.tempSettings;
    m_settings.volume = CLAMPED_VOICE_VOLUME( m_settings.volume );
    m_settings.rate = CLAMPED_VOICE_RATE( m_settings.rate );
    RegistryManager::SaveSettingsToRegistry( m_settings );
    ApplyVoiceSettings();

    if( m_settings.useDefaultText )
    {
      if( !m_hEditControl )
      {
        SetWindowText( m_hEditControl, m_settings.defaultText.c_str() );
        UpdateWindow( m_hEditControl );
      }
    }
  }
}

void MainWindow::PopulateLanguageCombo( HWND hDlg, SettingsDialogContext * ctx )
{
  if( ctx->languages.empty() )
  {
    ctx->languages.assign( SUPPORTED_LANGUAGES.begin(), SUPPORTED_LANGUAGES.end() );
  }

  HWND hCombo = GetDlgItem( hDlg, IDC_SETTINGS_LANGUAGE_COMBO );
  if( !hCombo ) return;

  if( ctx->tempSettings.language.empty() )
  {
    ctx->tempSettings.language = GetSystemLanguage();
  }

  int selectedIndex = -1;
  for( size_t i = 0; i < ctx->languages.size(); ++i )
  {
    const auto & info = ctx->languages[i];
    int idx = (int) SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM) info.NativeName.c_str() );
    SendMessage( hCombo, CB_SETITEMDATA, idx, (LPARAM) i );
    if( selectedIndex == -1 && _wcsicmp( info.EnglishName.c_str(), ctx->tempSettings.language.c_str() ) == 0 )
    {
      selectedIndex = idx;
    }
  }

  if( selectedIndex == -1 && SendMessage( hCombo, CB_GETCOUNT, 0, 0 ) > 0 )
  {
    selectedIndex = 0;
  }

  if( selectedIndex != -1 )
  {
    SendMessage( hCombo, CB_SETCURSEL, selectedIndex, 0 );
  }
}

std::wstring MainWindow::GetSelectedLanguageForLocalization( HWND hDlg, SettingsDialogContext * ctx )
{
  if( !ctx ) return L"";
  HWND hLanguageCombo = GetDlgItem( hDlg, IDC_SETTINGS_LANGUAGE_COMBO );
  if( hLanguageCombo )
  {
    int sel = (int) SendMessage( hLanguageCombo, CB_GETCURSEL, 0, 0 );
    if( sel != CB_ERR )
    {
      size_t langIndex = (size_t) SendMessage( hLanguageCombo, CB_GETITEMDATA, sel, 0 );
      if( langIndex < ctx->languages.size() )
      {
        return ctx->languages[langIndex].EnglishName;
      }
    }
  }
  return ctx->tempSettings.language;
}

void MainWindow::UpdateSettingsDialogLocalization( HWND hDlg, const std::wstring & language )
{
  if( !hDlg ) return;
  SetWindowText( hDlg, GetLocalizedString( SETTINGS_DIALOG_TITLE_TEXT_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_DEFAULT_TEXT, GetLocalizedString( SETTINGS_DEFAULT_TEXT_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_USE_DEFAULT_TEXT, GetLocalizedString( SETTINGS_USE_DEFAULT_TEXT_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_LANGUAGE, GetLocalizedString( SETTINGS_LANGUAGE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_VOICE, GetLocalizedString( SETTINGS_VOICE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_TEST_VOICE, GetLocalizedString( SETTINGS_TEST_VOICE_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_VOLUME, GetLocalizedString( SETTINGS_VOLUME_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_RATE, GetLocalizedString( SETTINGS_RATE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_SPEAK_ON_CLICK, GetLocalizedString( SETTINGS_SPEAK_ON_CLICK_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_REMEMBER_CATEGORY_WINDOW, GetLocalizedString( SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_MINIMIZE_CATEGORY_WINDOW, GetLocalizedString( SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING, GetLocalizedString( SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING, GetLocalizedString( SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, language ) );
  SetDlgItemText( hDlg, IDOK, GetLocalizedString( SETTINGS_OK_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDCANCEL, GetLocalizedString( SETTINGS_CANCEL_BUTTON_ID, language ) );
}

INT_PTR CALLBACK MainWindow::SettingsDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
  switch( message )
  {
    case WM_INITDIALOG:
    {
      auto * ctx = reinterpret_cast<SettingsDialogContext *>( lParam );
      if( !ctx ) return FALSE;
      SetWindowLongPtr( hDlg, GWLP_USERDATA, (LONG_PTR) ctx );

      SetDlgItemText( hDlg, IDC_SETTINGS_DEFAULT_TEXT, ctx->tempSettings.defaultText.c_str() );
      SendDlgItemMessage( hDlg, IDC_SETTINGS_USE_DEFAULT_TEXT, BM_SETCHECK,
        ctx->tempSettings.useDefaultText ? BST_CHECKED : BST_UNCHECKED, 0 );

      HWND hVoiceCombo = GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO );
      int selectedIndex = -1;
      for( size_t i = 0; i < ctx->voices.size(); ++i )
      {
        int idx = (int) SendMessage( hVoiceCombo, CB_ADDSTRING, 0, (LPARAM) ctx->voices[i].name.c_str() );
        SendMessage( hVoiceCombo, CB_SETITEMDATA, idx, (LPARAM) i );
        if( selectedIndex == -1 && ctx->tempSettings.voice == ctx->voices[i].key )
        {
          selectedIndex = idx;
        }
      }

      if( SendMessage( hVoiceCombo, CB_GETCOUNT, 0, 0 ) > 0 )
      {
        SendMessage( hVoiceCombo, CB_SETCURSEL, selectedIndex == -1 ? 0 : selectedIndex, 0 );
      }

      PopulateLanguageCombo( hDlg, ctx );

      int clampedVolume = CLAMPED_VOICE_VOLUME( ctx->tempSettings.volume );
      ConfigureSlider( hDlg, IDC_SETTINGS_VOLUME_SLIDER, SIMONSAYS_SETTINGS_MIN_VOICE_VOLUME, SIMONSAYS_SETTINGS_MAX_VOICE_VOLUME, clampedVolume );
      SyncSliderToEdit( hDlg, IDC_SETTINGS_VOLUME_SLIDER, IDC_SETTINGS_VOLUME_EDIT, FALSE );

      int clampedRate = CLAMPED_VOICE_RATE( ctx->tempSettings.rate );
      ConfigureSlider( hDlg, IDC_SETTINGS_RATE_SLIDER, SIMONSAYS_SETTINGS_MIN_VOICE_RATE, SIMONSAYS_SETTINGS_MAX_VOICE_RATE, clampedRate );
      SyncSliderToEdit( hDlg, IDC_SETTINGS_RATE_SLIDER, IDC_SETTINGS_RATE_EDIT, TRUE );
      SendDlgItemMessage( hDlg, IDC_SETTINGS_SPEAK_ON_CLICK, BM_SETCHECK,
        ctx->tempSettings.speakDirectlyWhenClickingPhrase ? BST_CHECKED : BST_UNCHECKED, 0 );
      SendDlgItemMessage( hDlg, IDC_SETTINGS_REMEMBER_CATEGORY_WINDOW, BM_SETCHECK,
        ctx->tempSettings.rememberCategoryWindowSize ? BST_CHECKED : BST_UNCHECKED, 0 );
      SendDlgItemMessage( hDlg, IDC_SETTINGS_MINIMIZE_CATEGORY_WINDOW, BM_SETCHECK,
        ctx->tempSettings.minimizeCategoryWindowAutomatically ? BST_CHECKED : BST_UNCHECKED, 0 );
      SendDlgItemMessage( hDlg, IDC_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING, BM_SETCHECK,
        ctx->tempSettings.increaseVolumeWhenPlaying ? BST_CHECKED : BST_UNCHECKED, 0 );
      SendDlgItemMessage( hDlg, IDC_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING, BM_SETCHECK,
        ctx->tempSettings.reduceOtherAudioWhenPlaying ? BST_CHECKED : BST_UNCHECKED, 0 );
      UpdateSettingsDialogLocalization( hDlg, GetSelectedLanguageForLocalization( hDlg, ctx ) );
      return TRUE;
    }

    case WM_COMMAND:
    {
      auto * ctx = reinterpret_cast<SettingsDialogContext *>( GetWindowLongPtr( hDlg, GWLP_USERDATA ) );
      if( !ctx ) break;

      WORD controlId = LOWORD( wParam );
      WORD notifyCode = HIWORD( wParam );

      if( notifyCode == EN_CHANGE )
      {
        HWND hFocus = GetFocus();
        if( controlId == IDC_SETTINGS_VOLUME_EDIT && hFocus == GetDlgItem( hDlg, IDC_SETTINGS_VOLUME_EDIT ) )
        {
          SyncEditToSlider( hDlg, IDC_SETTINGS_VOLUME_EDIT, IDC_SETTINGS_VOLUME_SLIDER, FALSE, SIMONSAYS_SETTINGS_MIN_VOICE_VOLUME, SIMONSAYS_SETTINGS_MAX_VOICE_VOLUME );
          return TRUE;
        }
        if( controlId == IDC_SETTINGS_RATE_EDIT && hFocus == GetDlgItem( hDlg, IDC_SETTINGS_RATE_EDIT ) )
        {
          SyncEditToSlider( hDlg, IDC_SETTINGS_RATE_EDIT, IDC_SETTINGS_RATE_SLIDER, TRUE, SIMONSAYS_SETTINGS_MIN_VOICE_RATE, SIMONSAYS_SETTINGS_MAX_VOICE_RATE );
          return TRUE;
        }
      }
      if( controlId == IDC_SETTINGS_LANGUAGE_COMBO && notifyCode == CBN_SELCHANGE )
      {
        std::wstring previous_voice = ctx->tempSettings.voice;

        HWND hVoiceCombo = GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO );
        if( hVoiceCombo )
        {
          int sel = (int) SendMessage( hVoiceCombo, CB_GETCURSEL, 0, 0 );
          if( sel != CB_ERR )
          {
            size_t voiceIndex = (size_t) SendMessage( hVoiceCombo, CB_GETITEMDATA, sel, 0 );
            if( voiceIndex < ctx->voices.size() )
            {
              if( !ctx->voices[voiceIndex].key.empty() )
              {
                previous_voice = ctx->voices[voiceIndex].key;
              }
            }
          }
        }

        UpdateSettingsDialogLocalization( hDlg, GetSelectedLanguageForLocalization( hDlg, ctx ) );
        ctx->voices = RegistryManager::PopulateAvaibleVoicesFromRegistry( GetSelectedLanguageForLocalization( hDlg, ctx ) );
        if( ctx->voices.empty() )
        {
          ctx->voices = RegistryManager::PopulateAvaibleVoicesFromRegistry();
        }



        SendMessage( hVoiceCombo, CB_RESETCONTENT, 0, 0 );

        int selectedIndex = -1;
        for( size_t i = 0; i < ctx->voices.size(); ++i )
        {
          int idx = (int) SendMessage( hVoiceCombo, CB_ADDSTRING, 0, (LPARAM) ctx->voices[i].name.c_str() );
          SendMessage( hVoiceCombo, CB_SETITEMDATA, idx, (LPARAM) i );
          if( previous_voice == ctx->voices[i].key )
          {
            selectedIndex = idx;
          }
          else if( selectedIndex == -1 )
          {
            if( ctx->tempSettings.voice == ctx->voices[i].key )
            {
              selectedIndex = idx;
            }
          }
        }

        if( SendMessage( hVoiceCombo, CB_GETCOUNT, 0, 0 ) > 0 )
        {
          SendMessage( hVoiceCombo, CB_SETCURSEL, selectedIndex == -1 ? 0 : selectedIndex, 0 );
        }

        return TRUE;
      }

      switch( controlId )
      {
        case IDC_SETTINGS_TEST_VOICE:
        {
          std::wstring sampleText;
          HWND hLanguageCombo = GetDlgItem( hDlg, IDC_SETTINGS_LANGUAGE_COMBO );
          if( hLanguageCombo )
          {
            int sel = (int) SendMessage( hLanguageCombo, CB_GETCURSEL, 0, 0 );
            if( sel != CB_ERR )
            {
              size_t langIndex = (size_t) SendMessage( hLanguageCombo, CB_GETITEMDATA, sel, 0 );
              if( langIndex < ctx->languages.size() )
              {
                sampleText = ctx->languages[langIndex].VoiceTestSampleText;
              }
            }
          }

          if( sampleText.empty() )
          {
            sampleText = ctx->tempSettings.defaultText;
          }
          if( sampleText.empty() )
          {
            sampleText = L"This is a sample phrase.";
          }

          std::wstring voiceKey = ctx->tempSettings.voice;
          HWND hVoiceCombo = GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO );
          if( hVoiceCombo )
          {
            int sel = (int) SendMessage( hVoiceCombo, CB_GETCURSEL, 0, 0 );
            if( sel != CB_ERR )
            {
              size_t voiceIndex = (size_t) SendMessage( hVoiceCombo, CB_GETITEMDATA, sel, 0 );
              if( voiceIndex < ctx->voices.size() )
              {
                voiceKey = ctx->voices[voiceIndex].key;
              }
            }
          }

          ISpVoice * previewVoice = nullptr;
          if( SUCCEEDED( CoCreateInstance( CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **) &previewVoice ) ) && previewVoice )
          {
            if( !voiceKey.empty() )
            {
              ISpObjectToken * token = nullptr;
              if( SUCCEEDED( SpGetTokenFromId( voiceKey.c_str(), &token, FALSE ) ) )
              {
                previewVoice->SetVoice( token );
                token->Release();
              }
            }

            int volumeValue = (int) SendDlgItemMessage( hDlg, IDC_SETTINGS_VOLUME_SLIDER, TBM_GETPOS, 0, 0 );
            previewVoice->SetVolume( CLAMPED_VOICE_VOLUME( volumeValue ) );

            int rateValue = (int) SendDlgItemMessage( hDlg, IDC_SETTINGS_RATE_SLIDER, TBM_GETPOS, 0, 0 );
            previewVoice->SetRate( CLAMPED_VOICE_RATE( rateValue ) );

            previewVoice->Speak( sampleText.c_str(), SPF_DEFAULT, nullptr );
            previewVoice->WaitUntilDone( INFINITE );
            previewVoice->Release();
          }
          return TRUE;
        }

        case IDOK:
        {
          wchar_t buffer[1024];
          GetDlgItemText( hDlg, IDC_SETTINGS_DEFAULT_TEXT, buffer, ARRAYSIZE( buffer ) );
          ctx->tempSettings.defaultText = buffer;
          ctx->tempSettings.useDefaultText = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_USE_DEFAULT_TEXT, BM_GETCHECK, 0, 0 ) == BST_CHECKED );

          HWND hLanguageCombo = GetDlgItem( hDlg, IDC_SETTINGS_LANGUAGE_COMBO );
          if( hLanguageCombo )
          {
            int sel = (int) SendMessage( hLanguageCombo, CB_GETCURSEL, 0, 0 );
            if( sel != CB_ERR )
            {
              size_t langIndex = (size_t) SendMessage( hLanguageCombo, CB_GETITEMDATA, sel, 0 );
              if( langIndex < ctx->languages.size() )
              {
                ctx->tempSettings.language = ctx->languages[langIndex].EnglishName;
                if( GetSystemLanguage() == ctx->languages[langIndex].EnglishName )
                  ctx->tempSettings.language = L"";
              }
            }
          }

          HWND hVoiceCombo = GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO );
          int sel = (int) SendMessage( hVoiceCombo, CB_GETCURSEL, 0, 0 );
          if( sel != CB_ERR )
          {
            size_t voiceIndex = (size_t) SendMessage( hVoiceCombo, CB_GETITEMDATA, sel, 0 );
            if( voiceIndex < ctx->voices.size() )
            {
              ctx->tempSettings.voice = ctx->voices[voiceIndex].key;
            }
          }

          int volumeValue = (int) SendDlgItemMessage( hDlg, IDC_SETTINGS_VOLUME_SLIDER, TBM_GETPOS, 0, 0 );
          ctx->tempSettings.volume = CLAMPED_VOICE_VOLUME( volumeValue );

          int rateValue = (int) SendDlgItemMessage( hDlg, IDC_SETTINGS_RATE_SLIDER, TBM_GETPOS, 0, 0 );
          ctx->tempSettings.rate = CLAMPED_VOICE_RATE( rateValue );
          ctx->tempSettings.speakDirectlyWhenClickingPhrase = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_SPEAK_ON_CLICK, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
          ctx->tempSettings.rememberCategoryWindowSize = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_REMEMBER_CATEGORY_WINDOW, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
          ctx->tempSettings.minimizeCategoryWindowAutomatically = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_MINIMIZE_CATEGORY_WINDOW, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
          ctx->tempSettings.increaseVolumeWhenPlaying = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
          ctx->tempSettings.reduceOtherAudioWhenPlaying = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING, BM_GETCHECK, 0, 0 ) == BST_CHECKED );

          ctx->accepted = true;
          EndDialog( hDlg, IDOK );
          return TRUE;
        }

        case IDCANCEL:
          EndDialog( hDlg, IDCANCEL );
          return TRUE;
      }
      break;
    }

    case WM_HSCROLL:
    {
      HWND hCtrl = (HWND) lParam;
      if( hCtrl == GetDlgItem( hDlg, IDC_SETTINGS_VOLUME_SLIDER ) )
      {
        SyncSliderToEdit( hDlg, IDC_SETTINGS_VOLUME_SLIDER, IDC_SETTINGS_VOLUME_EDIT, FALSE );
        return TRUE;
      }
      if( hCtrl == GetDlgItem( hDlg, IDC_SETTINGS_RATE_SLIDER ) )
      {
        SyncSliderToEdit( hDlg, IDC_SETTINGS_RATE_SLIDER, IDC_SETTINGS_RATE_EDIT, TRUE );
        return TRUE;
      }
      break;
    }
  }

  return FALSE;
}
