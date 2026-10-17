/*
   MainWindow.cpp: MainWindow class source file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "MainWindow.h"
#include "CategoryWindow.h"
#include "SSDwellWindow.h"
#include "SettingsDialog.h"
#include "GazeProviderChain.h"
#include "HidGazeProvider.h"
#include "TobiiGazeProvider.h"
#include "SSGazeReader.h"
#include "SSTobiiGaze.h"
#include "SSGazeDetect.h"
#include "SSDwellModeDetector.h"
#include "resource.h"
#include "utils.h"
#include "localized_strings.h"
#include <windows.h>
#include <dbt.h> // DBT_DEVNODES_CHANGED (WM_DEVICECHANGE)
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "version.lib")

// When the taskbar is clicked MainWindow can lose its Z-order and get covered by the taskbar.
// Track Z-order and force MainWindow back on top when that happens.
// This is a global variable to store the MainWindow handle for the hook to access.
HWND g_hwndMain = NULL;
// We also store the Taskbar handle for easier access
HWND g_hwndShell_TrayWnd = NULL;
// Hook handles
HHOOK g_hMouseHook = NULL;
// Last foreground window of another process seen at a mouse press: where the
// input flyout gives the focus back (settings.spec.md SET-F63).
HWND g_hwndForeignForeground = NULL;

static bool IsOwnProcessWindow( HWND hwnd )
{
  DWORD processId = 0;
  GetWindowThreadProcessId( hwnd, &processId );
  return processId == GetCurrentProcessId();
}

// Timer definitions
#define SLOW_TIMER_CHECK_ZORDER 1
#define FAST_TIMER_CHECK_ZORDER 2
#define SLOW_TIMER_CHECK_ZORDER_INTERVAL 5000
#define FAST_TIMER_CHECK_ZORDER_INTERVAL 400

// Dwell automatic-mode detection timers (only run while ModeSelection == Auto).
// (Timer id 3 was the retired ~30 Hz cursor-kinematics sampler; the classifier
// is dormant since spec v1.6 — direct-gaze liveness decides unconditionally.)
#define TIMER_DWELL_DETECT 3   // passive signals + decision evaluation
#define TIMER_DWELL_DEVCHANGE 4 // one-shot debounce for WM_DEVICECHANGE
#define TIMER_DWELL_GAZE 5      // routes HID gaze to the button under it (cursor doesn't move)
#define DWELL_DETECT_INTERVAL 3000 // ms — passive scan + Decide cadence
#define DWELL_GAZE_INTERVAL 50     // ms — HID gaze routing poll
#define DWELL_DEVCHANGE_DEBOUNCE 800 // ms — coalesce device-change bursts
#define DWELL_HYSTERESIS_COUNT 3   // consecutive equal decisions before switching

LRESULT CALLBACK EditSubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
  UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
  switch( uMsg )
  {
    case WM_KEYDOWN:
      if( wParam == VK_ESCAPE )
      {
        // Closes the input flyout (SET-F63).
        SendMessage( GetParent( hWnd ), WM_EDIT_CONTROL_ESCAPE_PRESSED, 0, 0 );
        return 0;
      }
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
      if( wParam == VK_RETURN || wParam == VK_ESCAPE )
      {
        // This is what stops the "Beep"
        // We eat the character message so the Edit control never sees it.
        return 0;
      }
      break;

    case WM_PASTE:
    {
      // Fold the clipboard before it lands (SET-F60): pasted prose carries
      // hard-wrap line breaks the user cannot see, and this box is single-line.
      // The breaks are inaudible anyway — SAPI treats them as whitespace — so
      // folding costs nothing and keeps what is shown equal to what is spoken.
      // Anything that is not Unicode text is left to the control.
      if( !IsClipboardFormatAvailable( CF_UNICODETEXT ) ) break;
      if( !OpenClipboard( hWnd ) ) break;

      std::wstring pasted;
      if( HANDLE hData = GetClipboardData( CF_UNICODETEXT ) )
      {
        if( const wchar_t * src = (const wchar_t *) GlobalLock( hData ) )
        {
          pasted = src;
          GlobalUnlock( hData );
        }
      }
      CloseClipboard();
      if( pasted.empty() ) break; // nothing usable — let the control try

      // bCanUndo = TRUE so Ctrl+Z still reverses the paste: handling WM_PASTE
      // ourselves would otherwise quietly cost the user their undo.
      const std::wstring folded = NormalizePhraseText( pasted );
      SendMessage( hWnd, EM_REPLACESEL, TRUE, (LPARAM) folded.c_str() );
      return 0;
    }

    case WM_NCDESTROY:
      // Clean up the subclass when the window is destroyed
      RemoveWindowSubclass( hWnd, EditSubclassProc, uIdSubclass );
      break;
  }
  return DefSubclassProc( hWnd, uMsg, wParam, lParam );
}

MainWindow::MainWindow()
  : m_hwnd( NULL ), m_hEditControl( NULL ),
  m_hInstance( NULL ), m_categoryWindow( nullptr ), m_settings( RegistryManager::LoadSettingsFromRegistry() ), m_hAccel( NULL )
{
  ZeroMemory( &m_nid, sizeof( m_nid ) );
}

MainWindow::~MainWindow()
{
  // Also covers a Create() that failed after EnsureTaskbarShown (no WM_DESTROY).
  RestoreTaskbarAutoHide();

  RegistryManager::SaveRunInfoToRegistry( GetProductVersionString() );

  if( m_playbackEngine )
  {
    m_playbackEngine.reset();
  }

  if( m_categoryWindow )
  {
    m_categoryWindow.reset();
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

  if( m_hSymbolFont )
  {
    DeleteObject( m_hSymbolFont );
    m_hSymbolFont = NULL;
  }
}

static void TraceTaskbarLayout( const TaskbarGeometry & taskbar, const RECT & bar )
{
  static const wchar_t * const edges[] = { L"left", L"top", L"right", L"bottom" };
  wchar_t line[320];
  swprintf_s( line, L"[Taskbar] found=%d edge=%s thickness=%d taskbar=(%d,%d,%d,%d) start=(%d,%d,%d,%d) tray=(%d,%d,%d,%d) bar=(%d,%d %dx%d)\n",
    taskbar.found ? 1 : 0, taskbar.edge < 4 ? edges[taskbar.edge] : L"?", taskbar.Thickness(),
    taskbar.taskbar.left, taskbar.taskbar.top, taskbar.taskbar.right, taskbar.taskbar.bottom,
    taskbar.start.left, taskbar.start.top, taskbar.start.right, taskbar.start.bottom,
    taskbar.tray.left, taskbar.tray.top, taskbar.tray.right, taskbar.tray.bottom,
    bar.left, bar.top, bar.right - bar.left, bar.bottom - bar.top );
  OutputDebugStringW( line );
}

bool MainWindow::Create( HINSTANCE hInstance, int nCmdShow )
{
  // Ensure the app-data root and its known subfolders (resources, debug,
  // boards) exist from first launch, independent of whether an import/export
  // ever happens.
  EnsureAppDataFoldersExist();

  // The bar sits on the taskbar, so keep the taskbar shown while running;
  // restored on exit (TBR-F10/F11).
  EnsureTaskbarShown();

  // The bar goes on the taskbar, whichever edge it is on (TBR-F01/F02).
  m_taskbar = GetTaskbarGeometry();
  RECT rcBar = ComputeBarRect();
  TraceTaskbarLayout( m_taskbar, rcBar );

  m_hInstance = hInstance;

  // Gaze dwell-click: install the direct-gaze providers (raw HID reader +
  // Tobii Stream Engine), bring the tracking provider chain online, and apply
  // the persisted dwell settings to the global config before any button is
  // created.
  GazeProviderChain::Instance().SetHidProvider( std::make_unique<HidGazeProvider>() );
  GazeProviderChain::Instance().SetTobiiProvider( std::make_unique<TobiiGazeProvider>() );
  GazeProviderChain::Instance().StartAll();
  ApplyDwellSettingsToConfig();
  // Auto-mode detection timers are started later in Create() once m_hwnd exists
  // (see SyncDwellTimers below).

  if( !RegisterWindowClass( hInstance ) )
  {
    return false;
  }

  m_hwnd = CreateWindowEx(
    WS_EX_LAYERED | WS_EX_TOPMOST,
    CLASS_NAME,
    L"SimonSays",
    WS_POPUP | WS_VISIBLE,
    rcBar.left, rcBar.top,
    rcBar.right - rcBar.left, rcBar.bottom - rcBar.top,
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
  m_playbackEngine->SetOutputDevice( m_settings.outputDeviceId );

  SetLayeredWindowAttributes( m_hwnd, GetTaskbarColor() /*RGB( 0, 0, 0 )*/, 0, LWA_COLORKEY );
  SetWindowPos( m_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
  ShowWindow( m_hwnd, nCmdShow );

  if( !CreateTaskbarControls() )
  {
    return false;
  }

  CreateTrayIcon();

  std::wstring boardStyle;
  m_categories = RegistryManager::LoadCategoriesFromRegistry( m_settings.language, &boardStyle );

  m_categoryWindow = std::make_unique<CategoryWindow>( this, m_settings.rememberCategoryWindowSize, m_settings.minimizeCategoryWindowAutomatically, m_settings.autoresizeCategoryWindow );
  if( !m_categoryWindow->Create( hInstance ) )
  {
    return false;
  }
  ShowWindow( m_hwnd, nCmdShow );
  UpdateWindow( m_hwnd );
  m_categoryWindow->UpdateCategories( m_categories, m_settings.language, RegistryManager::GetSelectedCategoryFromRegistry(), boardStyle );

  if( !m_hAccel )
  {
    m_hAccel = LoadAccelerators( m_hInstance, MAKEINTRESOURCE( IDR_MAINACCEL ) );
  }

  // Start the dwell Auto-mode detector timers if the user left selection on Auto.
  SyncDwellTimers();
  // Always-on gaze router: in HID modes the cursor doesn't move, so this poll
  // arms the button the user is looking at. Cheap; no-ops unless mode is HID.
  // Runs during the modal dwell window too (its probes need it), so it is not
  // gated by SyncDwellTimers.
  SetTimer( m_hwnd, TIMER_DWELL_GAZE, DWELL_GAZE_INTERVAL, NULL );

  std::wstring versionInRegistry = RegistryManager::GetLastRunVersionFromRegistry();
  if( ( !versionInRegistry.empty() && versionInRegistry != GetProductVersionString() ) || RegistryManager::GetVersionRunCount() < 3 )
  {
    // Show "What's New" after update or for the first few runs to highlight new features
    ShowHelpWindow();
  }

  // Refresh the boards shipped next to the exe into the user's boards folder
  // when the current language's board is missing or outdated (PORT-F41).
  SyncShippedBoards( m_settings.language );

  ShowWidgetsHintOnce();

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
  UpdateCategoryButtonText( false, m_settings.language );
}

// The arrow points toward the category window while it is hidden, and back
// toward the taskbar while it is shown (TBR-F23).
static const wchar_t * CategoryButtonArrow( UINT edge, bool categoryWindowShown )
{
  switch( edge )
  {
    case ABE_TOP:   return categoryWindowShown ? UP_ARROW : DOWN_ARROW;
    case ABE_LEFT:  return categoryWindowShown ? LEFT_ARROW : RIGHT_ARROW;
    case ABE_RIGHT: return categoryWindowShown ? RIGHT_ARROW : LEFT_ARROW;
    default:        return categoryWindowShown ? DOWN_ARROW : UP_ARROW;
  }
}

void MainWindow::UpdateCategoryButtonText( bool categoryWindowShown, const std::wstring & language )
{
  m_categoryWindowShownArrow = categoryWindowShown;
  if( !m_categoryButton.IsValid() )
    return;
  HFONT hFont = categoryWindowShown ? m_hRegularFont : m_hBoldFont;
  if( hFont )
    m_categoryButton.SetFont( hFont );
  const wchar_t * arrow = CategoryButtonArrow( m_taskbar.edge, categoryWindowShown );
  m_categoryButton.SetText( std::wstring( arrow ) + L" " + GetLocalizedString( CATEGORIES_BUTTON_TEXT_ID, language ) );

  // On a vertical taskbar no label fits: the arrow and a menu sign are drawn as
  // text in the symbol font (the sign tells it apart from Play's triangle) -
  // arrow first on a left taskbar, sign first on a right one - and the label
  // stays as the accessible name (TBR-F22, BTN-F47).
  std::wstring symbols;
  if( m_taskbar.IsVertical() )
  {
    const std::wstring sign = L"\u2261"; // identical-to sign
    symbols = ( m_taskbar.edge == ABE_LEFT ) ? arrow + sign : sign + arrow;
  }
  m_categoryButton.SetDisplayText( symbols, m_hSymbolFont );
}

// Play on a vertical taskbar: a play triangle, a stop square while speaking (pressing it stops);
// the localized label stays as the accessible name (TBR-F22, SET-N04).
void MainWindow::UpdatePlayButtonSymbol( bool playing )
{
  if( !m_playButton.IsValid() )
    return;
  const wchar_t * symbol = playing ? L"\u25A0" : L"\u25B6"; // stop square / play triangle
  m_playButton.SetDisplayText( m_taskbar.IsVertical() ? symbol : L"", m_hSymbolFont );
}

void MainWindow::OnBoardResourceFolderChanged( const std::wstring & folder )
{
  if( m_playbackEngine )
    m_playbackEngine->SetBoardResourceFolder( folder );
}

void MainWindow::ShowHideCategoryWindow()
{
  if( m_categoryWindow )
  {
    if( !m_categoryWindow->IsVisible() )
    {
      m_categoryWindow->Show();
      UpdateCategoryButtonText( true, m_settings.language );
    }
    else
    {
      m_categoryWindow->Hide();
      OnCategoryWindowHidden();
    }
  }
}

void MainWindow::UpdateInputPreview()
{
  // A multiline edit sends no EN_CHANGE for WM_SETTEXT, so every SetWindowText
  // on it is followed by this. Long text is clamped by the button (BTN-F46).
  if( m_hEditControl && m_previewButton.IsValid() )
    m_previewButton.SetText( ReadEditControlText( m_hEditControl ) );
}

void MainWindow::OpenInputFlyout()
{
  if( !m_inputFlyout || !m_previewButton.IsValid() )
    return;
  RECT rcPreview, rcBar;
  m_previewButton.GetWindowRect( rcPreview );
  GetWindowRect( m_hwnd, &rcBar );
  // Without a click (dwell, keyboard) the user's window is still in front.
  HWND restoreTo = GetForegroundWindow();
  if( !restoreTo || IsOwnProcessWindow( restoreTo ) )
    restoreTo = g_hwndForeignForeground;
  m_inputFlyout->Open( rcPreview, rcBar, m_taskbar, restoreTo );
}

void MainWindow::UpdateTaskbarUI()
{
  if( m_hEditControl )
  {
    InvalidateRect( m_hEditControl, NULL, TRUE );
    UpdateWindow( m_hEditControl );
  }
  UpdateInputPreview();
  if( m_playButton.IsValid() )
  {
    m_playButton.Invalidate();
  }
  if( m_categoryButton.IsValid() )
  {
    m_categoryButton.Invalidate();
  }
}

void MainWindow::UpdateUILanguage( const std::wstring language )
{
  bool isRtl = IsLanguageRTL( language );

  if( m_playButton.IsValid() )
  {
    m_playButton.updateRtlExStyle( isRtl );
    m_playButton.SetText( GetLocalizedString( PLAY_BUTTON_TEXT_ID, language ) );
  }

  if( m_categoryButton.IsValid() )
  {
    m_categoryButton.updateRtlExStyle( isRtl );
    UpdateCategoryButtonText( m_categoryWindowShownArrow, language );
  }

  // The quick access buttons draw no label (centered icon), but their text is
  // what screen readers announce, so it follows the UI language too (SET-N04).
  if( m_boardsButton.IsValid() )
  {
    m_boardsButton.updateRtlExStyle( isRtl );
    m_boardsButton.SetText( GetLocalizedString( IMPORT_CATEGORIES_DIALOG_TITLE_ID, language ) );
  }

  if( m_settingsButton.IsValid() )
  {
    m_settingsButton.updateRtlExStyle( isRtl );
    m_settingsButton.SetText( GetLocalizedString( SETTINGS_DIALOG_TITLE_TEXT_ID, language ) );
  }

  if( m_previewButton.IsValid() )
    m_previewButton.updateRtlExStyle( isRtl );

  if( m_hEditControl )
  {
    updateEditAlignment( m_hEditControl, isRtl );
    InvalidateRect( m_hEditControl, NULL, TRUE );
    UpdateWindow( m_hEditControl );
  }
}

// ReadEditControlText now lives in utils.cpp so the dialogs can use it too.

void MainWindow::PlayCurrentText()
{
  if( !m_hEditControl || !m_playbackEngine ) return;
  std::wstring text = ReadEditControlText( m_hEditControl );
  if( !text.empty() )
    m_playbackEngine->QueueText( text, m_settings.stopPreviousPlayback );
}

void MainWindow::AddTextToEditControl( const std::wstring & text )
{
  if( !m_hEditControl ) return;
  std::wstring currentText = ReadEditControlText( m_hEditControl );
  if( !currentText.empty() ) currentText += L" ";
  currentText += text;
  SetWindowText( m_hEditControl, currentText.c_str() );
  UpdateWindow( m_hEditControl );
  UpdateInputPreview();
}

void MainWindow::SetEditControlText( const std::wstring & text )
{
  if( !m_hEditControl ) return;
  SetWindowText( m_hEditControl, text.c_str() );
  UpdateWindow( m_hEditControl );
  UpdateInputPreview();
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
      // Still the window the user was in before this click activates ours.
      HWND hwndForeground = GetForegroundWindow();
      if( hwndForeground && !IsOwnProcessWindow( hwndForeground ) )
        g_hwndForeignForeground = hwndForeground;
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
            PostMessage( g_hwndMain, WM_TIMER, SLOW_TIMER_CHECK_ZORDER, 0 );
          }
        }
      }
    }
  }
  return CallNextHookEx( g_hMouseHook, nCode, wParam, lParam );
}

void MainWindow::EnsureTaskbarShown()
{
  if( !IsTaskbarAutoHide() )
    return;
  // Record before changing, so a crash still leaves a way back (TBR-F12). A
  // flag left by a previous unclean exit is kept, never overwritten.
  if( !RegistryManager::SetTaskbarAutoHideRestorePending( true ) )
    return; // without the record the user's choice could not be restored
  if( !SetTaskbarAutoHide( false ) )
    OutputDebugStringW( L"[Taskbar] Could not turn auto-hide off.\n" );
}

void MainWindow::RestoreTaskbarAutoHide()
{
  if( !RegistryManager::GetTaskbarAutoHideRestorePending() )
    return;
  // Keep the flag when the taskbar can't be reached (e.g. Explorer already gone
  // at log-off); the next clean exit restores it.
  if( SetTaskbarAutoHide( true ) )
    RegistryManager::SetTaskbarAutoHideRestorePending( false );
  else
    OutputDebugStringW( L"[Taskbar] Could not restore auto-hide; kept pending.\n" );
}

void MainWindow::ShowWidgetsHintOnce()
{
  if( RegistryManager::GetWidgetsHintShown() || !IsTaskbarWidgetsShown() )
    return;
  RegistryManager::SetWidgetsHintShown(); // once, whatever the answer
  if( ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( TASKBAR_WIDGETS_HINT_MESSAGE_ID, m_settings.language ),
    GetLocalizedString( TASKBAR_WIDGETS_HINT_TITLE_ID, m_settings.language ),
    MB_YESNO | MB_ICONINFORMATION, m_settings.language ) == IDYES )
  {
    ShellExecute( NULL, L"open", L"ms-settings:taskbar", NULL, NULL, SW_SHOWNORMAL );
  }
}

LRESULT CALLBACK MainWindow::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  static UINT_PTR s_slowZOrderCheckTimerId = 0;
  static UINT_PTR s_fastZOrderCheckTimerId = 0;
  static bool s_wasStartMenuOpen = false;
  MainWindow * pThis = nullptr;

  if( uMsg == WM_CREATE )
  {
    CREATESTRUCT * pCreate = (CREATESTRUCT *) lParam;
    pThis = (MainWindow *) pCreate->lpCreateParams;
    SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
    // Start slow timer to check Z Order
    s_slowZOrderCheckTimerId = SetTimer( hwnd, SLOW_TIMER_CHECK_ZORDER, SLOW_TIMER_CHECK_ZORDER_INTERVAL, NULL );
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

  // Explorer restarted: a new taskbar, maybe somewhere else (TBR-F40).
  static const UINT s_taskbarCreatedMessage = RegisterWindowMessage( L"TaskbarCreated" );
  if( pThis && s_taskbarCreatedMessage && uMsg == s_taskbarCreatedMessage )
  {
    g_hwndShell_TrayWnd = FindWindow( L"Shell_TrayWnd", NULL );
    pThis->RelayoutIfTaskbarChanged( true );
    return 0;
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
        else if( wmId == ID_DWELL_OPEN )
        {
          pThis->ShowDwellWindow();
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
            PostMessage( hwnd, WM_TIMER, SLOW_TIMER_CHECK_ZORDER, 0 );
          }
          else
          {
            if( pThis->m_categoryWindow )
            {
              pThis->m_categoryWindow->Hide();
              pThis->OnCategoryWindowHidden();
            }
            if( pThis->m_inputFlyout )
              pThis->m_inputFlyout->Close( false );
            ShowWindow( hwnd, SW_HIDE );
          }
          break;
        }
        else if( wmId == ID_TRAY_SETTINGS )
        {
          pThis->ShowSettingsDialog();
          break;
        }
        else if( wmId == ID_TRAY_DWELL )
        {
          pThis->ShowDwellWindow();
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
          ShowLocalizedMessageBox( hwnd, ( GetLocalizedString( ABOUT_1_ID, pThis->m_settings.language ) + GetProductVersionString() + GetLocalizedString( ABOUT_2_ID, pThis->m_settings.language ) ).c_str(), GetLocalizedString( ABOUT_TITLE_ID, pThis->m_settings.language ), MB_OK | MB_ICONINFORMATION, pThis->m_settings.language );
          break;
        }
        else if( wmId == ID_TRAY_FEEDBACK )
        {
          ShellExecute( NULL, L"open", L"https://forms.gle/KMbpEDYmwnFJUhgy8", NULL, NULL, SW_SHOWNORMAL );
          break;
        }
        else if( wmId == ID_TRAY_EXIT )
        {
          DestroyWindow( hwnd );
          break;
        }

        if( wmEvent == BN_CLICKED )
        {
          switch( wmId )
          {
            case IDC_BUTTON_PLAY:
            {
              if( pThis->m_playbackEngine && pThis->m_playbackEngine->IsPlaying() )
                pThis->m_playbackEngine->Stop();
              else
                pThis->PlayCurrentText();
            }
            break;
            case IDC_BUTTON_CATEGORIES:
            {
              pThis->ShowHideCategoryWindow();
            }
            break;
            case IDC_BUTTON_PREVIEW:
            {
              pThis->OpenInputFlyout();
            }
            break;
            case IDC_BUTTON_BOARDS:
            {
              if( pThis->m_categoryWindow )
              {
                pThis->m_categoryWindow->Show();
                pThis->m_categoryWindow->ImportCategories( GetBoardsFolder() );
              }
            }
            break;
            case IDC_BUTTON_SETTINGS:
            {
              pThis->ShowSettingsDialog();
            }
            break;
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
            if( pThis->m_settings.showTouchKeyboard && pThis->m_inputFlyout )
            {
              // Beside the input flyout, on the side away from the taskbar,
              // left/top edges aligned (TBR-F32).
              SIZE placement;
              switch( pThis->m_taskbar.edge )
              {
                case ABE_TOP:   placement = { -1, 2 }; break;
                case ABE_LEFT:  placement = { 2, -1 }; break;
                case ABE_RIGHT: placement = { -2, -1 }; break;
                default:        placement = { -1, -2 }; break;
              }
              ShowTouchKeyboard( pThis->m_inputFlyout->GetHwnd(), placement, 5 );
            }
          }
        }
      }
      break;

      case WM_TRAYICON:
      {
        switch( lParam )
        {
          case WM_LBUTTONDBLCLK:
          {
            ShowWindow( hwnd, SW_SHOW );
            SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
            PostMessage( hwnd, WM_TIMER, SLOW_TIMER_CHECK_ZORDER, 0 );
          }
          break;

          case WM_LBUTTONUP:
          case WM_RBUTTONUP:
          {
            POINT pt;
            GetCursorPos( &pt );
            pThis->ShowContextMenu( hwnd, pt );
          }
          break;
        }
      }
      break;

      case WM_SIZE:
      {
        pThis->UpdateTaskbarUI();
      }
      break;

      case WM_SETTINGCHANGE:
      case WM_DISPLAYCHANGE:
      {
        // The taskbar may have moved or resized (TBR-F40); the 5 s check
        // catches anything Explorer finishes after this message.
        pThis->RelayoutIfTaskbarChanged( false );
      }
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
            pThis->m_categoryWindow->ProcessSSFile( filePath ); // asks first (PORT-F13/F15)
          }
          return TRUE;
        }
      }
      break;

      case WM_DEVICECHANGE:
      {
        // A device arrived/left. Coalesce the burst, then re-probe HID + signals.
        if( wParam == DBT_DEVNODES_CHANGED )
          SetTimer( hwnd, TIMER_DWELL_DEVCHANGE, DWELL_DEVCHANGE_DEBOUNCE, NULL );
      }
      break;

      case WM_DESTROY:
      {
        if( s_slowZOrderCheckTimerId ) KillTimer( hwnd, s_slowZOrderCheckTimerId );
        if( s_fastZOrderCheckTimerId ) KillTimer( hwnd, s_fastZOrderCheckTimerId );
        KillTimer( hwnd, TIMER_DWELL_DETECT );
        KillTimer( hwnd, TIMER_DWELL_DEVCHANGE );
        KillTimer( hwnd, TIMER_DWELL_GAZE );
        if( g_hMouseHook )
        {
          UnhookWindowsHookEx( g_hMouseHook );
          g_hMouseHook = NULL;
        }
        GazeProviderChain::Instance().StopAll();
        pThis->RestoreTaskbarAutoHide();
        PostQuitMessage( 0 );
      }
      break;

      case WM_ENDSESSION:
      {
        // Log-off/shutdown may end the process without WM_DESTROY (TBR-F11).
        if( wParam )
          pThis->RestoreTaskbarAutoHide();
      }
      break;

      case WM_TIMER:
      {
        switch( wParam )
        {
          case SLOW_TIMER_CHECK_ZORDER:
          case FAST_TIMER_CHECK_ZORDER:
          {
            if( wParam == SLOW_TIMER_CHECK_ZORDER )
              pThis->RelayoutIfTaskbarChanged( false ); // TBR-F40: no timer of its own

            // Windows doesn't expect an application window over taskbar, 
            // so it doesn't preserve its Z order when clicking on taskbar or opening start menu 
            // We need to check if our window is still on top of the taskbar and if not, bring it back to the top
            HWND hwndForegroundParent = GetForegroundWindow();
            while( hwndForegroundParent && GetParent( hwndForegroundParent ) )
            {
              hwndForegroundParent = GetParent( hwndForegroundParent );
            }

            if( hwndForegroundParent != hwnd && IsWindowVisible( hwnd ) ) // If our window is not focused and visible perform the check
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
                    s_fastZOrderCheckTimerId = SetTimer( hwnd, FAST_TIMER_CHECK_ZORDER, FAST_TIMER_CHECK_ZORDER_INTERVAL, NULL );
                  }
                  // Check if the Start Menu is open
                  // The Start Menu is a window with class name "Windows.UI.Core.CoreWindow"
                  // If the Start Menu is open, MainWindow loses Z Order and is covered by the Taskbar and only clicking on another application window can restore Z Order. 
                  // We need a workaround to restore Z Order when the Start Menu is open or closed. 
                  // We can detect when the Start Menu is open or closed by checking the foreground window class name.
                  HWND hwndForeground = GetForegroundWindow();
                  if( hwndForeground != hwnd )
                  {
                    GetClassName( hwndForeground, className, 256 );
                    OutputDebugString( ( L"--------------------------------------Class name: " + std::wstring( className ) + L"\n" ).c_str() );
                    if( wcscmp( className, L"Windows.UI.Core.CoreWindow" ) == 0 ) // This is the Start Menu window, we need a workaround to restore Z Order
                    {
                      if( !s_wasStartMenuOpen )
                      {
                        OutputDebugString( L"Start Menu opened\n" );
                        // TODO: Find a workaround to restore Z Order when the Start Menu is open.
                      }
                      s_wasStartMenuOpen = true;
                    }
                    else
                    {
                      if( s_wasStartMenuOpen )
                      {
                        // The Start Menu was open and now is closed, we need to restore Z Order
                        // We can do this by simulating a click on the foreground window (app icon selected) but not activated properly
                        OutputDebugString( ( L"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxClass name: " + std::wstring( className ) + L"\n" ).c_str() );
                        s_wasStartMenuOpen = false;
                        if( hwndForeground != hwndAtPoint )
                        {
                          SimulateNonActivatingClickOnWindow( hwndForeground );
                        }
                        SetForegroundWindow( hwndForeground );
                      }
                    }
                  }
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
          case TIMER_DWELL_DETECT:
          {
            pThis->UpdateDwellPassiveSignals();
            pThis->EvaluateDwellAutoMode();
          }
          break;
          case TIMER_DWELL_DEVCHANGE:
          {
            // Debounced device-change: re-attempt HID open and Tobii engine setup
            // promptly and refresh the detector's signals/HID-live (runs
            // regardless of mode so the dwell window's HID option reflects the
            // current hardware).
            KillTimer( hwnd, TIMER_DWELL_DEVCHANGE );
            SSGazeReader::Instance().Wake();
            SSTobiiGaze::Instance().Wake();
            pThis->UpdateDwellPassiveSignals();
          }
          break;
          case TIMER_DWELL_GAZE:
          {
            // In HID dwell, route the gaze point to the button under it so dwell
            // arms without a moving cursor. (Mouse dwell still uses WM_MOUSEMOVE.)
            if( SSDwellConfig::Instance().GetActiveMode() == SSDwellMode::HidDwell )
            {
              GazeSample s;
              if( GazeProviderChain::Instance().GetTrackingSample( &s ) && s.valid )
                SSButton::RouteGaze( s.screenPoint );
            }
          }
          break;
        }
      }
      break;

      case WM_PLAYBACK_STARTED:
      {
        if( pThis->m_playButton.IsValid() )
          pThis->m_playButton.SetText( GetLocalizedString( PLAY_BUTTON_TEXT_PLAYING_ID, pThis->m_settings.language ) );
        pThis->UpdatePlayButtonSymbol( true );
      }
      break;

      case WM_PLAYBACK_FINISHED:
      {
        if( pThis->m_playButton.IsValid() )
          pThis->m_playButton.SetText( GetLocalizedString( PLAY_BUTTON_TEXT_ID, pThis->m_settings.language ) );
        pThis->UpdatePlayButtonSymbol( false );
      }
      break;

      case WM_PLAYBACK_OUTPUT_DEVICE_MISSING:
      {
        // The engine posts this once per absence and is already playing
        // through the Windows default device (sound.spec.md SND-F72).
        const std::wstring & language = pThis->m_settings.language;
        std::wstring name = pThis->m_settings.outputDeviceName.empty() ? pThis->m_settings.outputDeviceId : pThis->m_settings.outputDeviceName;
        std::wstring message = std::wstring( GetLocalizedString( OUTPUT_DEVICE_MISSING_MESSAGE1_ID, language ) )
          + L"\n\n" + name + L"\n\n" + GetLocalizedString( OUTPUT_DEVICE_MISSING_MESSAGE2_ID, language );
        ShowLocalizedMessageBox( hwnd, message.c_str(), GetLocalizedString( ERROR_TITLE_ID, language ), MB_OK | MB_ICONWARNING, language );
      }
      break;

      case WM_EDIT_CONTROL_ENTER_PRESSED:
      {
        // Speak, close the flyout and leave the focus on Play, so the phrase
        // can be repeated or stopped from the keyboard (SET-F63).
        pThis->PlayCurrentText();
        if( pThis->m_inputFlyout )
          pThis->m_inputFlyout->Close( false );
        SetForegroundWindow( hwnd );
        pThis->m_playButton.SetFocus();
      }
      break;

      case WM_EDIT_CONTROL_ESCAPE_PRESSED:
      {
        if( pThis->m_inputFlyout )
          pThis->m_inputFlyout->Close( true );
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

static bool IsThinTaskbar( const TaskbarGeometry & taskbar )
{
  return !taskbar.IsVertical() && taskbar.Thickness() < MW_THIN_TASKBAR_THICKNESS;
}

int MainWindow::QuickButtonsWidth() const
{
  if( !m_settings.showQuickButtons )
    return 0;
  // Stacked on a regular taskbar; side by side where there is no height for
  // two (Thin tier, SET-F50) and in the vertical stack.
  return ( IsThinTaskbar( m_taskbar ) || m_taskbar.IsVertical() ) ? 2 * m_iconButtonSize + 2 : m_iconButtonSize;
}

// Screen rect of the bar for m_taskbar (TBR-F02/F20/F22). With the Start button
// near the start of the taskbar (left- or top-aligned icons) the bar sits just
// before the notification area; otherwise (centered icons) at the very start of
// the taskbar, as wide as the room before the Start button allows.
RECT MainWindow::ComputeBarRect() const
{
  const TaskbarGeometry & g = m_taskbar;
  const RECT & tb = g.taskbar;
  const bool hasStart = !IsRectEmpty( &g.start );
  const bool hasTray = !IsRectEmpty( &g.tray );
  RECT bar = {};

  if( g.IsVertical() )
  {
    const int width = max( g.Thickness() - 4, 2 * m_iconButtonSize + 2 + 2 * MW_VERTICAL_MARGIN );
    const int length = 4 * MW_VERTICAL_MARGIN + 2 * m_buttonHeight + MW_VERTICAL_EDIT_HEIGHT +
      ( m_settings.showQuickButtons ? m_iconButtonSize + MW_VERTICAL_MARGIN : 0 );
    int y = tb.top;
    if( hasStart && g.start.top - tb.top < MW_START_NEAR_EDGE )
    {
      const int end = hasTray ? g.tray.top - TRAY_MARGIN : tb.bottom - FALLBACK_TRAY_WIDTH - TRAY_MARGIN;
      y = max( (int) g.start.bottom, end - length );
    }
    bar.left = tb.left + ( g.Thickness() - width ) / 2;
    bar.top = y;
    bar.right = bar.left + width;
    bar.bottom = y + length;
  }
  else
  {
    const int height = max( g.Thickness() - 2, 24 );
    const int minWidth = 4 * m_horizontalMargin + m_categoryButtonWidth + m_playButtonWidth + QuickButtonsWidth() + MW_MIN_EDIT_WIDTH;
    const int preferredWidth = max( MW_DEFAULT_WINDOW_WIDTH, minWidth );
    int x = tb.left;
    int width = preferredWidth;
    if( hasStart && g.start.left - tb.left < MW_START_NEAR_EDGE )
    {
      const int end = hasTray ? g.tray.left - TRAY_MARGIN : tb.right - FALLBACK_TRAY_WIDTH - TRAY_MARGIN;
      width = max( minWidth, min( preferredWidth, end - ( g.start.right + TRAY_MARGIN ) ) );
      x = end - width;
    }
    else if( hasStart && g.start.left > tb.left )
    {
      width = max( minWidth, min( preferredWidth, g.start.left - TRAY_MARGIN - tb.left ) );
    }
    bar.left = x;
    bar.top = tb.top + ( g.Thickness() - height ) / 2;
    bar.right = x + width;
    bar.bottom = bar.top + height;
  }
  return bar;
}

// Recreates the bar's fonts when the tier changes: the menu font, or ~11/12 of
// it on a thin or vertical taskbar (TBR-F21).
void MainWindow::UpdateTaskbarFonts( bool compact )
{
  if( m_fontsCompact == (int) compact && m_hRegularFont && m_hBoldFont )
    return;

  NONCLIENTMETRICS ncm = {};
  ncm.cbSize = sizeof( NONCLIENTMETRICS );
  SystemParametersInfo( SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0 );
  if( compact )
    ncm.lfMenuFont.lfHeight = MulDiv( ncm.lfMenuFont.lfHeight, 11, 12 );

  HFONT hOldRegular = m_hRegularFont;
  HFONT hOldBold = m_hBoldFont;
  HFONT hOldSymbol = m_hSymbolFont;
  m_hRegularFont = CreateFontIndirect( &ncm.lfMenuFont );
  ncm.lfMenuFont.lfWeight = FW_SEMIBOLD;
  m_hBoldFont = CreateFontIndirect( &ncm.lfMenuFont );
  m_hSymbolFont = NULL;
  if( compact )
  {
    // The vertical stack's button symbols, ~1.6x the bar font (TBR-F22).
    ncm.lfMenuFont.lfHeight = MulDiv( ncm.lfMenuFont.lfHeight, 8, 5 );
    ncm.lfMenuFont.lfWeight = FW_NORMAL;
    m_hSymbolFont = CreateFontIndirect( &ncm.lfMenuFont );
  }
  m_fontsCompact = compact ? 1 : 0;

  // Hand the controls the new fonts before the old ones are deleted.
  m_playButton.SetFont( m_hRegularFont );
  m_categoryButton.SetFont( m_categoryWindowShownArrow ? m_hRegularFont : m_hBoldFont );
  if( m_hEditControl )
    SendMessage( m_hEditControl, WM_SETFONT, (WPARAM) m_hBoldFont, TRUE );
  m_previewButton.SetFont( m_hBoldFont );

  // The symbols are re-applied with the new font by the layout that follows.
  m_categoryButton.SetDisplayText( L"" );
  m_playButton.SetDisplayText( L"" );

  if( hOldRegular ) DeleteObject( hOldRegular );
  if( hOldBold ) DeleteObject( hOldBold );
  if( hOldSymbol ) DeleteObject( hOldSymbol );
}

// Lays the controls out inside the bar for m_taskbar: one row on a horizontal
// taskbar (Regular or Thin tier, TBR-F21), a stack on a vertical one (TBR-F22).
bool MainWindow::UpdateTaskbarControlsPosition()
{
  if( !m_hwnd ) return false;
  RECT rect;
  GetClientRect( m_hwnd, &rect );
  const bool vertical = m_taskbar.IsVertical();
  const bool thin = IsThinTaskbar( m_taskbar );
  const bool showQuick = m_settings.showQuickButtons;
  const int icon = m_iconButtonSize;

  UpdateTaskbarFonts( thin || vertical );

  if( m_boardsButton.IsValid() )
    m_boardsButton.Show( showQuick );
  if( m_settingsButton.IsValid() )
    m_settingsButton.Show( showQuick );

  if( vertical )
  {
    const int margin = MW_VERTICAL_MARGIN;
    const int width = rect.right - 2 * margin;
    int y = margin;
    m_categoryButton.SetPos( margin, y, width, m_buttonHeight );
    y += m_buttonHeight + margin;
    if( showQuick )
    {
      const int quickX = rect.right / 2 - icon - 1;
      m_boardsButton.SetPos( quickX, y, icon, icon );
      m_settingsButton.SetPos( quickX + icon + 2, y, icon, icon );
      y += icon + margin;
    }
    // Two wrapped lines from the top.
    m_previewButton.SetStyle( WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_LEFT | BS_TOP | BS_MULTILINE );
    m_previewButton.SetPos( margin, y, width, MW_VERTICAL_EDIT_HEIGHT );
    y += MW_VERTICAL_EDIT_HEIGHT + margin;
    m_playButton.SetPos( margin, y, width, m_buttonHeight );
  }
  else
  {
    const int margin = m_horizontalMargin;
    const int buttonHeight = thin ? rect.bottom - 6 : rect.bottom - 10; // 36 on a 48 px taskbar
    const int vertMargin = ( rect.bottom - buttonHeight ) / 2;
    const int quickWidth = QuickButtonsWidth();
    const int editWidth = max( 0, rect.right - 4 * margin - ( m_categoryButtonWidth + m_playButtonWidth ) - quickWidth );
    m_categoryButton.SetPos( margin, vertMargin, m_categoryButtonWidth, buttonHeight );
    if( showQuick )
    {
      const int quickX = margin + m_categoryButtonWidth + margin / 2;
      if( thin )
      {
        const int quickY = ( rect.bottom - icon ) / 2;
        m_boardsButton.SetPos( quickX, quickY, icon, icon );
        m_settingsButton.SetPos( quickX + icon + 2, quickY, icon, icon );
      }
      else
      {
        const int quickY = ( rect.bottom - 2 * icon ) / 2;
        m_boardsButton.SetPos( quickX, quickY, icon, icon );
        m_settingsButton.SetPos( quickX, quickY + icon, icon, icon );
      }
    }
    m_previewButton.SetStyle( WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_LEFT );
    m_previewButton.SetPos( rect.right - ( margin + m_playButtonWidth ) - ( editWidth + margin ), vertMargin, editWidth, buttonHeight );
    m_playButton.SetPos( rect.right - ( margin + m_playButtonWidth ), vertMargin, m_playButtonWidth, buttonHeight );
  }

  UpdateCategoryButtonText( m_categoryWindowShownArrow, m_settings.language );
  UpdatePlayButtonSymbol( m_playbackEngine && m_playbackEngine->IsPlaying() );

  // The Z-order check probes the middle of the Categories button (TBR-F03).
  RECT rc;
  if( m_categoryButton.GetWindowRect( rc ) )
  {
    m_inButtonPoint.x = rc.left + ( rc.right - rc.left ) / 2;
    m_inButtonPoint.y = rc.top + ( rc.bottom - rc.top ) / 2;
  }
  return true;
}

void MainWindow::RelayoutIfTaskbarChanged( bool force )
{
  if( !m_hwnd ) return;
  // Only ask Explorer (ABM_GETTASKBARPOS) once a window rect has changed.
  if( !force && !TaskbarWindowsChanged( m_taskbar ) )
    return;
  const TaskbarGeometry taskbar = GetTaskbarGeometry();
  if( !force && SameTaskbarGeometry( taskbar, m_taskbar ) )
    return;
  const bool edgeChanged = taskbar.edge != m_taskbar.edge;
  const bool taskbarMoved = edgeChanged || !EqualRect( &taskbar.taskbar, &m_taskbar.taskbar );
  m_taskbar = taskbar;

  if( m_inputFlyout )
    m_inputFlyout->Close( false ); // its anchor is about to move (SET-F63)

  RECT oldBar = {};
  GetWindowRect( m_hwnd, &oldBar );
  const RECT bar = ComputeBarRect();
  TraceTaskbarLayout( m_taskbar, bar );
  // Controls are moved, never recreated, so the typed text and focus survive.
  SetWindowPos( m_hwnd, NULL, bar.left, bar.top, bar.right - bar.left, bar.bottom - bar.top, SWP_NOZORDER | SWP_NOACTIVATE );
  UpdateTaskbarControlsPosition();

  // The category window follows the bar's position (alignment, room, edge) but
  // not a change of width alone; a new edge re-places it even after a drag (TBR-F30).
  const bool barMoved = bar.left != oldBar.left || bar.top != oldBar.top;
  if( m_categoryWindow && ( taskbarMoved || barMoved ) )
    m_categoryWindow->PlaceNextToBar( edgeChanged );
}

bool MainWindow::CreateTaskbarControls()
{
  if( !m_hwnd ) return false;

  // Created at placeholder positions; UpdateTaskbarControlsPosition lays them
  // out for the taskbar's edge and tier at the end.
  if( !m_categoryButton.Create(
    m_hwnd, m_hInstance, IDC_BUTTON_CATEGORIES,
    GetLocalizedString( CATEGORIES_BUTTON_TEXT_ID, m_settings.language ),
    0, 0, m_categoryButtonWidth, m_buttonHeight,
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    IsLanguageRTL( m_settings.language ) ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0 ) )
  {
    return false;
  }


  if( !m_boardsButton.Create(
    m_hwnd, m_hInstance, IDC_BUTTON_BOARDS,
    L"",
    0, 0, m_iconButtonSize, m_iconButtonSize,
    WS_TABSTOP | WS_CHILD | BS_PUSHBUTTON | ( m_settings.showQuickButtons ? WS_VISIBLE : 0 ),
    IsLanguageRTL( m_settings.language ) ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0 ) )
  {
    return false;
  }
  m_boardsButton.SetColors( GetTaskbarColor() );
  m_boardsButton.SetCornerRadius( m_iconButtonSize / 2 );
  m_boardsButton.SetBorderWidth( 0 );
  m_boardsButton.SetEmoji( L"\U0001F4C2", m_iconButtonSize * 2 / 3, true, SSButtonIconPosition::Center ); // 📂
  // The centered icon suppresses the label, but the text still names the button
  // for screen readers / UI Automation (SET-N04).
  m_boardsButton.SetText( GetLocalizedString( IMPORT_CATEGORIES_DIALOG_TITLE_ID, m_settings.language ) );

  if( !m_settingsButton.Create(
    m_hwnd, m_hInstance, IDC_BUTTON_SETTINGS,
    L"",
    0, 0, m_iconButtonSize, m_iconButtonSize,
    WS_TABSTOP | WS_CHILD | BS_PUSHBUTTON | ( m_settings.showQuickButtons ? WS_VISIBLE : 0 ),
    IsLanguageRTL( m_settings.language ) ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0 ) )
  {
    return false;
  }
  m_settingsButton.SetColors( GetTaskbarColor() );
  m_settingsButton.SetCornerRadius( m_iconButtonSize / 2 );
  m_settingsButton.SetBorderWidth( 0 );
  m_settingsButton.SetEmoji( L"\U00002699", m_iconButtonSize * 2 / 3, true, SSButtonIconPosition::Center ); // ⚙️
  m_settingsButton.SetText( GetLocalizedString( SETTINGS_DIALOG_TITLE_TEXT_ID, m_settings.language ) );

  // The bar's preview of the text; clicking, dwelling on or pressing it opens
  // the input flyout, where the edit control lives (SET-F62).
  if( !m_previewButton.Create(
    m_hwnd, m_hInstance, IDC_BUTTON_PREVIEW,
    L"",
    0, 0, MW_MIN_EDIT_WIDTH, m_buttonHeight,
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_LEFT,
    IsLanguageRTL( m_settings.language ) ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0 ) )
  {
    return false;
  }
  m_previewButton.SetColors( GetSysColor( COLOR_WINDOW ), GetSysColor( COLOR_WINDOWTEXT ) );
  m_previewButton.SetSquared();
  m_previewButton.SetBorderWidth( 1 );

  m_inputFlyout = std::make_unique<InputFlyout>();
  if( !m_inputFlyout->Create( m_hInstance, m_hwnd ) )
    return false;

  m_hEditControl = CreateWindowEx(
    IsLanguageRTL( m_settings.language ) ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING | WS_EX_CLIENTEDGE ) : WS_EX_CLIENTEDGE,
    L"EDIT",
    L"",
    // Wrapping multiline box with a draggable scrollbar (SET-F61). The three
    // style bits are all load-bearing and were each settled by measurement:
    //   ES_MULTILINE   - a plain single-line edit renders nothing past ~40k
    //                    characters, so a long paste went in but vanished.
    //   no ES_AUTOHSCROLL - its absence is what turns wrapping ON. Wrapping
    //                    keeps every visual line short, which is why this
    //                    configuration renders at any length.
    //   ES_AUTOVSCROLL + WS_VSCROLL - without vertical scrolling a multiline
    //                    edit refuses text that would overflow its height
    //                    (1,000 accepted, 10,000 rejected outright); WS_VSCROLL
    //                    also gives the scrollbar you can drag to review.
    // ES_WANTRETURN stays off: EditSubclassProc turns VK_RETURN into "speak".
    WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
    0, 0, MW_MIN_EDIT_WIDTH, m_buttonHeight,
    m_inputFlyout->GetHwnd(), // lives in the flyout (SET-F62)
    (HMENU) IDC_EDIT_PHRASE,
    m_hInstance,
    NULL
  );

  if( !m_hEditControl ) return false;

  // Without this an edit control silently caps what can be pasted into it at
  // its 30,000-character default — the text never reaches our code to be read
  // or normalized (CAT-N06). Safe to lift only because the registry read grows
  // to the value (persistence.spec.md REG-F41).
  SendMessage( m_hEditControl, EM_SETLIMITTEXT, 0, 0 );

  SetWindowSubclass( m_hEditControl, EditSubclassProc, 0, 0 );
  m_inputFlyout->SetEdit( m_hEditControl );

  if( !m_playButton.Create(
    m_hwnd, m_hInstance, IDC_BUTTON_PLAY,
    GetLocalizedString( PLAY_BUTTON_TEXT_ID, m_settings.language ),
    0, 0, m_playButtonWidth, m_buttonHeight,
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    IsLanguageRTL( m_settings.language ) ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0 ) )
  {
    return false;
  }

  UpdateTaskbarControlsPosition(); // fonts, positions, Categories label
  if( m_settings.useDefaultText )
  {
    if( m_hEditControl )
    {
      SetWindowText( m_hEditControl, m_settings.defaultText.c_str() );
      UpdateWindow( m_hEditControl );
      UpdateInputPreview();
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
    InsertMenu( hMenu, -1, ( m_showingSettingDialog ) ? ( MF_BYPOSITION | MF_STRING | MF_DISABLED ) : ( MF_BYPOSITION | MF_STRING ), ID_TRAY_DWELL, GetLocalizedString( TRAYICON_DWELL_ID, m_settings.language ) );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_WEB, GetLocalizedString( TRAYICON_WEB_ID, m_settings.language ) );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_ABOUT, GetLocalizedString( TRAYICON_ABOUT_ID, m_settings.language ) );
    InsertMenu( hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_FEEDBACK, GetLocalizedString( TRAYICON_FEEDBACK_ID, m_settings.language ) );
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
  m_playbackEngine->SetOutputDevice( m_settings.outputDeviceId );
  m_playbackEngine->SetVoiceSettings( m_settings.voice, CLAMPED_VOICE_VOLUME( m_settings.volume ), CLAMPED_VOICE_RATE( m_settings.rate ) );
}

void MainWindow::ShowSettingsDialog()
{
  if( m_showingSettingDialog )
    return;

  m_showingSettingDialog = true;
  Settings working = m_settings;
  if( ::ShowSettingsDialog( m_hwnd, m_hInstance, working ) )
  {
    if( working.language != m_settings.language )
    {
      std::wstring boardStyle;
      m_categories = RegistryManager::LoadCategoriesFromRegistry( working.language, &boardStyle );

      if( m_categoryWindow )
      {
        m_categoryWindow->UpdateCategories( m_categories, working.language, -1, boardStyle );
      }

      UpdateUILanguage( working.language );
      // Bring the new language's shipped boards in now, not at the next start (PORT-F41).
      SyncShippedBoards( working.language );
    }

    if( m_categoryWindow )
    {
      m_categoryWindow->SetMinimizeWhenLosingFocus( working.minimizeCategoryWindowAutomatically );
      m_categoryWindow->SetRememberWindowSize( working.rememberCategoryWindowSize );
      m_categoryWindow->SetAutoResize( working.autoresizeCategoryWindow );
    }

    m_settings = working;
    m_settings.volume = CLAMPED_VOICE_VOLUME( m_settings.volume );
    m_settings.rate = CLAMPED_VOICE_RATE( m_settings.rate );
    RegistryManager::SaveSettingsToRegistry( m_settings );
    ApplyVoiceSettings();
    // The quick access buttons change the bar's minimum width (TBR-F20).
    RelayoutIfTaskbarChanged( true );

    if( m_settings.useDefaultText )
    {
      if( m_hEditControl )
      {
        SetWindowText( m_hEditControl, m_settings.defaultText.c_str() );
        UpdateWindow( m_hEditControl );
        UpdateInputPreview();
      }
    }
  }
  m_showingSettingDialog = false;
}

void MainWindow::ApplyDwellSettingsToConfig()
{
  SSDwellConfig & c = SSDwellConfig::Instance();
  c.SetDwellTimeMs( (UINT) m_settings.dwellTimeMs );
  c.SetToleranceRadius( (UINT) m_settings.dwellToleranceRadius );
  c.SetCooldownMs( (UINT) m_settings.dwellCooldownMs );
  c.SetProgressColor( m_settings.dwellProgressColor );
  c.ApplyDetectedMode( (SSDwellMode) m_settings.dwellDetectedMode );
  c.SetModeSelection( (SSDwellModeSelection) m_settings.dwellModeSelection );
}

void MainWindow::ShowDwellWindow()
{
  if( m_showingSettingDialog )
    return;

  m_showingSettingDialog = true;

  Settings working = m_settings;
  if( ShowDwellSettingsDialog( m_hwnd, m_hInstance, working ) )
  {
    m_settings = working;
    RegistryManager::SaveSettingsToRegistry( m_settings );
    ApplyDwellSettingsToConfig(); // re-assert final state (probe forcing may have changed it)
  }
  m_showingSettingDialog = false;
  // The selection may have changed to/from Auto: refresh the detector timers.
  m_dwellDecisionStreak = 0;
  SyncDwellTimers();
}

// Starts the Auto-mode detector timer when the user selection is Auto, and
// stops it otherwise (forced modes don't need detection). Idempotent.
void MainWindow::SyncDwellTimers()
{
  if( !m_hwnd ) return;
  if( SSDwellConfig::Instance().GetModeSelection() == SSDwellModeSelection::Auto )
  {
    SetTimer( m_hwnd, TIMER_DWELL_DETECT, DWELL_DETECT_INTERVAL, NULL );
    // Refresh the passive scan now so the first Decide() doesn't run on
    // signals up to one TIMER_DWELL_DETECT tick (~3 s) stale.
    UpdateDwellPassiveSignals();
  }
  else
  {
    KillTimer( m_hwnd, TIMER_DWELL_DETECT );
  }
}

void MainWindow::UpdateDwellPassiveSignals()
{
  EyeTrackingStatus st = DetectEyeTracking();
  SSDwellModeDetector::PassiveSignals ps;
  ps.hidDevicePresent = st.hidDevicePresent;
  ps.externalToolRunning = st.externalToolRunning;
  ps.windowsEyeControl = st.windowsEyeControl;
  ps.toolKnownToClick = st.toolKnownToClick;
  SSDwellModeDetector::Instance().UpdatePassive( ps );
  SSDwellModeDetector::Instance().SetHidLive( GazeProviderChain::Instance().HidLive() );
}

void MainWindow::EvaluateDwellAutoMode()
{
  SSDwellConfig & cfg = SSDwellConfig::Instance();
  if( cfg.GetModeSelection() != SSDwellModeSelection::Auto )
    return;

  SSDwellModeDetector::Decision d = SSDwellModeDetector::Instance().Decide();

  // Hysteresis: only switch once the same decision repeats N times.
  if( d.mode == m_dwellPendingMode )
    ++m_dwellDecisionStreak;
  else
  {
    m_dwellPendingMode = d.mode;
    m_dwellDecisionStreak = 1;
  }

  if( m_dwellDecisionStreak >= DWELL_HYSTERESIS_COUNT && cfg.GetDetectedMode() != d.mode )
  {
    cfg.ApplyDetectedMode( d.mode );
    // Persist so Auto restores the last resolved mode on next launch. Hysteresis
    // makes switches infrequent, so this isn't a hot write path.
    m_settings.dwellDetectedMode = (int) d.mode;
    RegistryManager::SaveSettingsToRegistry( m_settings );
  }
}

// Opens the localized HTML help page shipped next to the executable (F1).
// The file name per language comes from HELP_CONTENT_FILE_NAME_ID, which
// scripts/build_help_html.ps1 writes when it generates the pages. If that
// page is missing (a partial install, or a language whose page has not been
// generated yet) we fall back to the English one rather than doing nothing:
// silence on F1 is the worst outcome for a user who needs help.
void MainWindow::ShowHelpWindow()
{
  OpenHelpPage( m_settings.language );
}
