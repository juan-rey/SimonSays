/*
   MainWindow.cpp: MainWindow class source file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
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
  bool noVoiceForLanguage = false;
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
#define FAST_TIMER_CHECK_ZORDER_INTERVAL 400

LRESULT CALLBACK EditSubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
  UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
  switch( uMsg )
  {
    case WM_KEYDOWN:
      if( wParam == VK_RETURN )
      {
        // The user pressed Enter!
        SendMessage( GetParent( hWnd ), WM_EDIT_CONTROL_ENTER_PRESSED, 0, 0 );

        // IMPORTANT: Return 0 to tell Windows "I handled this."
        // This prevents the annoying "Beep" sound.
        return 0;
      }
      break;

    case WM_CHAR:
      if( wParam == VK_RETURN )
      {
        // This is what stops the "Beep"
        // We eat the character message so the Edit control never sees it.
        return 0;
      }
      break;

    case WM_NCDESTROY:
      // Clean up the subclass when the window is destroyed
      RemoveWindowSubclass( hWnd, EditSubclassProc, uIdSubclass );
      break;
  }
  return DefSubclassProc( hWnd, uMsg, wParam, lParam );
}

MainWindow::MainWindow()
  : m_hwnd( NULL ), m_hEditControl( NULL ), m_hPlayButton( NULL ), m_hCategoryButton( NULL ),
  m_hInstance( NULL ), m_categoryWindow( nullptr ), m_helpWindow( nullptr ), m_settings( RegistryManager::LoadSettingsFromRegistry() ), m_hAccel( NULL )
{
  ZeroMemory( &m_nid, sizeof( m_nid ) );
}

MainWindow::~MainWindow()
{
  RegistryManager::SaveRunInfoToRegistry( GetProductVersionString() );

  if( m_playbackEngine )
  {
    m_playbackEngine.reset();
  }

  if( m_categoryWindow )
  {
    m_categoryWindow.reset();
  }

  if( m_helpWindow )
  {
    m_helpWindow.reset();
  }

  RemoveTrayIcon();

  if( m_hRegularFont )
  {
    DeleteObject( m_hRegularFont );
    m_hRegularFont = NULL;
  }

  if( m_hBoldFont )
  {
    DeleteObject( m_hBoldFont );
    m_hBoldFont = NULL;
  }
}

bool MainWindow::Create( HINSTANCE hInstance, int nCmdShow )
{
  RECT rc;
  int width = MW_DEFAULT_WINDOW_WIDTH;
  int height = MW_DEFAULT_WINDOW_HEIGHT;
  int x = 0;

  GetWindowRect( GetDesktopWindow(), &rc );

  int y = ( rc.bottom - height ) - 0;

  if( IsTaskbarAtBottom() )
  {
    int startButtonXPosition = GetStartButtonXPosition();
    if( startButtonXPosition > -1 && startButtonXPosition < 99 )
    {
      // If the Start button is found and is within a reasonable range (to avoid false positives), align to the left of the Start button
      int systemTrayXPosition = GetSystemTrayXPosition();
      if( systemTrayXPosition > 0 )
      {
        x = systemTrayXPosition - width - TRAY_MARGIN; // 24px margin from the tray
      }
      else
      {
        x = rc.right - width - FALLBACK_TRAY_WIDTH - TRAY_MARGIN; // Fallback to right align if we can't find the tray
      }
    }
  }
  else
  {
    ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( ERROR_TASKBAR_POSITION_ID, m_settings.language ), GetLocalizedString( ERROR_TITTLE_ID, m_settings.language ), MB_OK | MB_ICONERROR, m_settings.language );
    return false; // Currently only supports bottom taskbar
  }

  m_hInstance = hInstance;

  if( !RegisterWindowClass( hInstance ) )
  {
    return false;
  }

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

  m_playbackEngine = std::make_unique<PlaybackEngine>( m_hwnd, m_settings.voice,
    CLAMPED_VOICE_VOLUME( m_settings.volume ), CLAMPED_VOICE_RATE( m_settings.rate ) );
  m_playbackEngine->SetAudioDuckingSettings( m_settings.increaseVolumeWhenPlaying, m_settings.reduceOtherAudioWhenPlaying );

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
  m_categoryWindow->UpdateCategories( m_categories, m_settings.language );

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

  std::wstring versionInRegistry = RegistryManager::GetLastRunVersionFromRegistry();
  if( ( !versionInRegistry.empty() && versionInRegistry != GetProductVersionString() ) || RegistryManager::GetVersionRunCount() < 3 )
  {
    // Show "What's New" after update or for the first few runs to highlight new features
    ShowHelpWindow();
  }

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

void MainWindow::OnCategoryWindowHidden()
{
  if( m_hCategoryButton )
  {
    if( m_hBoldFont )
      SendMessage( m_hCategoryButton, WM_SETFONT, (WPARAM) m_hBoldFont, TRUE );
    SetWindowText( m_hCategoryButton, ( std::wstring( UP_ARROW ) + L" " + GetLocalizedString( CATEGORIES_BUTTON_TEXT_ID, m_settings.language ) ).c_str() );
  }
}

void MainWindow::ShowHideCategoryWindow()
{
  if( m_categoryWindow )
  {
    if( !m_categoryWindow->IsVisible() )
    {
      m_categoryWindow->Show();
      if( m_hCategoryButton )
      {
        if( m_hRegularFont )
          SendMessage( m_hCategoryButton, WM_SETFONT, (WPARAM) m_hRegularFont, TRUE );
        SetWindowText( m_hCategoryButton, ( std::wstring( DOWN_ARROW ) + L" " + GetLocalizedString( CATEGORIES_BUTTON_TEXT_ID, m_settings.language ) ).c_str() );
      }
    }
    else
    {
      m_categoryWindow->Hide();
      OnCategoryWindowHidden();
    }
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
  bool isRtl = IsLanguageRTL( language );

  if( m_hPlayButton )
  {
    updateRtlExStyle( m_hPlayButton, isRtl );
    SetWindowText( m_hPlayButton, GetLocalizedString( PLAY_BUTTON_TEXT_ID, language ) );
  }

  if( m_hCategoryButton )
  {
    updateRtlExStyle( m_hCategoryButton, isRtl );
    SetWindowText( m_hCategoryButton, GetLocalizedString( CATEGORIES_BUTTON_TEXT_ID, language ) );
  }

  if( m_hEditControl )
  {
    updateEditAlignment( m_hEditControl, isRtl );
    InvalidateRect( m_hEditControl, NULL, TRUE );
    UpdateWindow( m_hEditControl );
  }
}

void MainWindow::PlayCurrentText()
{
  if( !m_hEditControl || !m_playbackEngine ) return;
  wchar_t buffer[2048];
  GetWindowText( m_hEditControl, buffer, sizeof( buffer ) / sizeof( wchar_t ) );

  std::wstring text = buffer;
  if( !text.empty() )
  {
    m_playbackEngine->QueueText( text, m_settings.stopPreviousPlayback );
  }
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

        if( wmId == ID_HELP_OPEN )
        {
          pThis->ShowHelpWindow();
          break;
        }
        else if( wmId == ID_SETTINGS_OPEN )
        {
          pThis->ShowSettingsDialog();
          break;
        }
        else if( wmId == ID_ADD_AFTER_SELECTION )
        {
          if( pThis->m_categoryWindow )
          {
            if( !pThis->m_categoryWindow->IsVisible() )
              pThis->m_categoryWindow->Show();
            pThis->m_categoryWindow->AddAfterSelection();
          }
          break;
        }
        else if( wmId == ID_EDIT_LAST_SELECTION )
        {
          if( pThis->m_categoryWindow )
          {
            if( !pThis->m_categoryWindow->IsVisible() )
              pThis->m_categoryWindow->Show();
            pThis->m_categoryWindow->EditLastSelection();
          }
          break;
        }
        else if( wmId == ID_MOVE_SELECTION_PREV )
        {
          if( pThis->m_categoryWindow )
          {
            if( !pThis->m_categoryWindow->IsVisible() )
              pThis->m_categoryWindow->Show();
            pThis->m_categoryWindow->MoveSelection( IsLanguageRTL( pThis->m_settings.language ) ? 1 : -1 );
          }
          break;
        }
        else if( wmId == ID_MOVE_SELECTION_NEXT )
        {
          if( pThis->m_categoryWindow )
          {
            if( !pThis->m_categoryWindow->IsVisible() )
              pThis->m_categoryWindow->Show();
            pThis->m_categoryWindow->MoveSelection( IsLanguageRTL( pThis->m_settings.language ) ? -1 : 1 );
          }
          break;
        }
        else if( wmId == ID_DELETE_LAST_SELECTION )
        {
          if( pThis->m_categoryWindow )
          {
            if( !pThis->m_categoryWindow->IsVisible() )
              pThis->m_categoryWindow->Show();
            pThis->m_categoryWindow->DeleteLastSelection();
          }
          break;
        }
        else if( wmId == ID_IMPORT_FILE )
        {
          if( pThis->m_categoryWindow )
          {
            if( !pThis->m_categoryWindow->IsVisible() )
              pThis->m_categoryWindow->Show();
            pThis->m_categoryWindow->ImportCategories();
          }
          break;
        }
        else if( wmId == ID_EXPORT_FILE )
        {
          if( pThis->m_categoryWindow )
          {
            if( !pThis->m_categoryWindow->IsVisible() )
              pThis->m_categoryWindow->Show();
            pThis->m_categoryWindow->ExportCategories();
          }
          break;
        }
        else if( wmId == ID_TRAY_SHOW_HIDE )
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
        else if( wmId == ID_TRAY_SETTINGS )
        {
          pThis->ShowSettingsDialog();
          break;
        }
        else if( wmId == ID_TRAY_WEB )
        {
          ShellExecute( NULL, L"open", L"https://simonsays.chat/", NULL, NULL, SW_SHOWNORMAL );
          break;
        }
        else if( wmId == ID_TRAY_ABOUT )
        {
          // Show about dialog
          ShowLocalizedMessageBox( hwnd, ( GetLocalizedString( ABOUT_1_ID, pThis->m_settings.language ) + GetProductVersionString() + GetLocalizedString( ABOUT_2_ID, pThis->m_settings.language ) ).c_str(), GetLocalizedString( ABOUT_TITTLE_ID, pThis->m_settings.language ), MB_OK | MB_ICONINFORMATION, pThis->m_settings.language );
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
            if( pThis->m_playbackEngine && pThis->m_playbackEngine->IsPlaying() )
              pThis->m_playbackEngine->Stop();
            else
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
        else if( wmEvent == EN_SETFOCUS )
        {
          if( wmId == IDC_EDIT_PHRASE )
          {
            if( pThis->m_settings.showTouchKeyboard )
            {
              RECT rcMainWindow, rcCategoryWindow = { 0,0,0,0 };
              GetWindowRect( hwnd, &rcMainWindow );
              if( pThis->m_categoryWindow )
                GetWindowRect( pThis->m_categoryWindow->GetHwnd(), &rcCategoryWindow );

              if( rcCategoryWindow.right > rcMainWindow.right ) // If the category window is open and overlaps the main window, show the keyboard near the category window
                ShowTouchKeyboard( pThis->m_categoryWindow->GetHwnd(), { 2, 1 }, 5 );// Show the keyboard to the right and alligned to the bottom of the category window
              else
                ShowTouchKeyboard( hwnd, { 2,-2 }, 5 ); // Show the keyboard to the right and above the main window
            }
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

      case WM_COPYDATA:
      {
        const COPYDATASTRUCT * cds = reinterpret_cast<const COPYDATASTRUCT *>( lParam );
        if( cds && cds->dwData == SIMONSAYS_COPYDATA_IMPORT_SSC && cds->lpData && cds->cbData >= sizeof( wchar_t ) )
        {
          const wchar_t * filePath = reinterpret_cast<const wchar_t *>( cds->lpData );
          if( pThis->m_categoryWindow )
          {
            if( !pThis->m_categoryWindow->IsVisible() )
              pThis->m_categoryWindow->Show();
            pThis->m_categoryWindow->ImportCategories( filePath );
          }
          return TRUE;
        }
      }
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

      case WM_PLAYBACK_STARTED:
        if( pThis->m_hPlayButton )
          SetWindowText( pThis->m_hPlayButton, GetLocalizedString( PLAY_BUTTON_TEXT_PLAYING_ID, pThis->m_settings.language ) );
        break;

      case WM_PLAYBACK_FINISHED:
        if( pThis->m_hPlayButton )
          SetWindowText( pThis->m_hPlayButton, GetLocalizedString( PLAY_BUTTON_TEXT_ID, pThis->m_settings.language ) );
        break;

      case WM_EDIT_CONTROL_ENTER_PRESSED:
      {
        pThis->PlayCurrentText();
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

  int vertMargin = ( rect.bottom - m_buttonHeight ) / 2;
  int editWidth = rect.right - 4 * m_horizontalMargin - ( m_categoryButtonWidth + m_playButtonWidth );

  m_hCategoryButton = CreateWindowEx(
    IsLanguageRTL( m_settings.language ) ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0,
    L"BUTTON",
    ( std::wstring( DOWN_ARROW ) + L" " + GetLocalizedString( CATEGORIES_BUTTON_TEXT_ID, m_settings.language ) ).c_str(),
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    m_horizontalMargin, vertMargin, m_categoryButtonWidth, m_buttonHeight,
    m_hwnd,
    (HMENU) IDC_BUTTON_CATEGORIES,
    m_hInstance,
    NULL
  );

  if( !m_hCategoryButton ) return false;

  m_hEditControl = CreateWindowEx(
    IsLanguageRTL( m_settings.language ) ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING | WS_EX_CLIENTEDGE ) : WS_EX_CLIENTEDGE,
    L"EDIT",
    L"",
    WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL,
    m_horizontalMargin + m_categoryButtonWidth + m_horizontalMargin, vertMargin, editWidth, m_buttonHeight,
    m_hwnd,
    (HMENU) IDC_EDIT_PHRASE,
    m_hInstance,
    NULL
  );

  if( !m_hEditControl ) return false;

  SetWindowSubclass( m_hEditControl, EditSubclassProc, 0, 0 );

  m_hPlayButton = CreateWindowEx(
    IsLanguageRTL( m_settings.language ) ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0,
    L"BUTTON",
    GetLocalizedString( PLAY_BUTTON_TEXT_ID, m_settings.language ),
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON /*| BS_FLAT */,
    m_horizontalMargin + m_categoryButtonWidth + m_horizontalMargin + editWidth + m_horizontalMargin, vertMargin, m_playButtonWidth, m_buttonHeight,
    m_hwnd,
    (HMENU) IDC_BUTTON_PLAY,
    m_hInstance,
    NULL
  );

  if( !m_hPlayButton ) return false;

  NONCLIENTMETRICS ncm = {};
  ncm.cbSize = sizeof( NONCLIENTMETRICS );
  SystemParametersInfo( SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0 );

  if( m_hRegularFont )
  {
    DeleteObject( m_hRegularFont );
    m_hRegularFont = NULL;
  }

  if( m_hBoldFont )
  {
    DeleteObject( m_hBoldFont );
    m_hBoldFont = NULL;
  }

  m_hRegularFont = CreateFontIndirect( &ncm.lfMenuFont );

  SendMessage( m_hCategoryButton, WM_SETFONT, (WPARAM) m_hRegularFont, TRUE );
  SendMessage( m_hPlayButton, WM_SETFONT, (WPARAM) m_hRegularFont, TRUE );

  ncm.lfMenuFont.lfWeight = FW_SEMIBOLD;
  m_hBoldFont = CreateFontIndirect( &ncm.lfMenuFont );
  SendMessage( m_hEditControl, WM_SETFONT, (WPARAM) m_hBoldFont, TRUE );
  CenterEditTextVertically( m_hEditControl );
  if( m_settings.useDefaultText )
  {
    if( m_hEditControl )
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
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_SHOW_HIDE, GetLocalizedString( IsWindowVisible( m_hwnd ) ? TRAYICON_HIDE_ID : TRAYICON_SHOW_ID, m_settings.language ) );
    InsertMenu( hMenu, -1, ( m_showingSettingDialog ) ? ( MF_BYPOSITION | MF_STRING | MF_DISABLED ) : ( MF_BYPOSITION | MF_STRING ), ID_TRAY_SETTINGS, GetLocalizedString( TRAYICON_SETTINGS_ID, m_settings.language ) );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_WEB, GetLocalizedString( TRAYICON_WEB_ID, m_settings.language ) );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_ABOUT, GetLocalizedString( TRAYICON_ABOUT_ID, m_settings.language ) );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_EXIT, GetLocalizedString( TRAYICON_EXIT_ID, m_settings.language ) );

    SetForegroundWindow( hwnd );
    TrackPopupMenu( hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL );
    DestroyMenu( hMenu );
  }
}

void MainWindow::ApplyVoiceSettings()
{
  if( !m_playbackEngine ) return;

  m_playbackEngine->Stop();

  m_playbackEngine->SetAudioDuckingSettings( m_settings.increaseVolumeWhenPlaying, m_settings.reduceOtherAudioWhenPlaying );
  m_playbackEngine->SetVoiceSettings( m_settings.voice, CLAMPED_VOICE_VOLUME( m_settings.volume ), CLAMPED_VOICE_RATE( m_settings.rate ) );
}

void MainWindow::ShowSettingsDialog()
{
  if( m_showingSettingDialog )
    return;

  m_showingSettingDialog = true;
  SettingsDialogContext context;
  context.owner = this;
  context.tempSettings = m_settings;
  context.voices = RegistryManager::PopulateAvaibleVoicesFromRegistry( context.tempSettings.language.empty() ? GetSystemLanguage() : context.tempSettings.language );
  if( context.voices.empty() )
  {
    context.noVoiceForLanguage = true;
    context.voices = RegistryManager::PopulateAvaibleVoicesFromRegistry();
  }
  else
  {
    context.noVoiceForLanguage = false;
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
        m_categoryWindow->UpdateCategories( m_categories, context.tempSettings.language );
      }

      UpdateUILanguage( context.tempSettings.language );

      if( m_helpWindow && m_helpWindow->IsVisible() )
      {
        m_helpWindow->SetLanguage( context.tempSettings.language );
      }
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
      if( m_hEditControl )
      {
        SetWindowText( m_hEditControl, m_settings.defaultText.c_str() );
        UpdateWindow( m_hEditControl );
      }
    }
  }
  m_showingSettingDialog = false;
}

void MainWindow::ShowHelpWindow()
{
  if( !m_helpWindow )
  {
    m_helpWindow = std::make_unique<HelpWindow>();
    m_helpWindow->Create( m_hInstance );
  }
  m_helpWindow->SetLanguage( m_settings.language );
  m_helpWindow->Show();
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
  bool isRtl = IsLanguageRTL( language );
  updateRtlExStyle( hDlg, isRtl );
  EnumChildWindows( hDlg, ApplyRtlStylesCallback, isRtl );

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
  SetDlgItemText( hDlg, IDC_SETTINGS_STOP_PREVIOUS_PLAYBACK, GetLocalizedString( SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_SHOW_TOUCH_KEYBOARD, GetLocalizedString( SETTINGS_SHOW_TOUCH_KEYBOARD_ID, language ) );
  SetDlgItemText( hDlg, IDOK, GetLocalizedString( SETTINGS_OK_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDCANCEL, GetLocalizedString( SETTINGS_CANCEL_BUTTON_ID, language ) );
}

INT_PTR CALLBACK MainWindow::SettingsDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
  SettingsDialogContext * ctx = nullptr;
  if( message == WM_INITDIALOG )
  {
    ctx = reinterpret_cast<SettingsDialogContext *>( lParam );
    if( !ctx ) return FALSE;
    SetWindowLongPtr( hDlg, GWLP_USERDATA, (LONG_PTR) ctx );
  }
  else
  {
    ctx = reinterpret_cast<SettingsDialogContext *>( GetWindowLongPtr( hDlg, GWLP_USERDATA ) );
  }

  if( ctx )
  {
    switch( message )
    {
      case WM_INITDIALOG:
      {
        SetDlgItemText( hDlg, IDC_SETTINGS_DEFAULT_TEXT, ctx->tempSettings.defaultText.c_str() );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_USE_DEFAULT_TEXT, BM_SETCHECK,
          ctx->tempSettings.useDefaultText ? BST_CHECKED : BST_UNCHECKED, 0 );

        if( ctx->tempSettings.voice.empty() )
        {
          LPWSTR pszTokenId = NULL;
          if( SUCCEEDED( SpGetDefaultTokenIdFromCategoryId( SPCAT_VOICES, &pszTokenId ) ) )
          {
            ctx->tempSettings.voice = pszTokenId;
            CoTaskMemFree( pszTokenId );
          }
        }

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
        SendDlgItemMessage( hDlg, IDC_SETTINGS_STOP_PREVIOUS_PLAYBACK, BM_SETCHECK,
          ctx->tempSettings.stopPreviousPlayback ? BST_CHECKED : BST_UNCHECKED, 0 );
        SendDlgItemMessage( hDlg, IDC_SETTINGS_SHOW_TOUCH_KEYBOARD, BM_SETCHECK,
          ctx->tempSettings.showTouchKeyboard ? BST_CHECKED : BST_UNCHECKED, 0 );
        UpdateSettingsDialogLocalization( hDlg, GetSelectedLanguageForLocalization( hDlg, ctx ) );
        return TRUE;
      }

      case WM_COMMAND:
      {
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
            ctx->noVoiceForLanguage = true;
            ctx->voices = RegistryManager::PopulateAvaibleVoicesFromRegistry();
          }
          else
          {
            ctx->noVoiceForLanguage = false;
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

              previewVoice->Speak( sampleText.c_str(), SPF_IS_NOT_XML, nullptr );
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
            ctx->tempSettings.stopPreviousPlayback = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_STOP_PREVIOUS_PLAYBACK, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.showTouchKeyboard = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_SHOW_TOUCH_KEYBOARD, BM_GETCHECK, 0, 0 ) == BST_CHECKED );

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

      case WM_CTLCOLORLISTBOX:
      {
        HDC hdcStatic = (HDC) wParam;
        if( ctx->noVoiceForLanguage )
        {
          COMBOBOXINFO info;
          info.cbSize = sizeof( info );
          SendMessage( GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO ), CB_GETCOMBOBOXINFO, 0, (LPARAM) &info );
          if( info.hwndList == ( (HWND) lParam ) )
          {
            SetTextColor( hdcStatic, RGB( 255, 0, 0 ) );
            SetBkMode( hdcStatic, TRANSPARENT );
          }
        }
        return (INT_PTR) GetStockObject( WHITE_BRUSH );
      }


      case WM_CTLCOLOREDIT:
      {
        HDC hdcStatic = (HDC) wParam;
        if( GetDlgItem( hDlg, IDC_SETTINGS_VOICE_COMBO ) == (HWND) lParam && ctx->noVoiceForLanguage )
        {
          SetTextColor( hdcStatic, RGB( 255, 0, 0 ) );
          SetBkMode( hdcStatic, TRANSPARENT );
        }
        return (INT_PTR) GetStockObject( WHITE_BRUSH );
      }
    }
  }

  return FALSE;
}
