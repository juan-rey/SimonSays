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
  std::vector<AudioOutputDevice> outputDevices; // "Sound output" list (SET-F15)
  HWND hListenTooltip = NULL;
  bool noVoiceForLanguage = false;
  bool accepted = false;
};

// Item data of the "Sound output" combo: an index into outputDevices, or one of these.
#define OUTPUT_DEVICE_ITEM_DEFAULT ( (LPARAM) -1 )      // Windows default
#define OUTPUT_DEVICE_ITEM_DISCONNECTED ( (LPARAM) -2 ) // saved device, not connected now

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
      std::wstring boardStyle;
      m_categories = RegistryManager::LoadCategoriesFromRegistry( context.tempSettings.language, &boardStyle );

      if( m_categoryWindow )
      {
        m_categoryWindow->UpdateCategories( m_categories, context.tempSettings.language, -1, boardStyle );
      }

      UpdateUILanguage( context.tempSettings.language );
    }

    if( m_categoryWindow )
    {
      m_categoryWindow->SetMinimizeWhenLosingFocus( context.tempSettings.minimizeCategoryWindowAutomatically );
      m_categoryWindow->SetRememberWindowSize( context.tempSettings.rememberCategoryWindowSize );
      m_categoryWindow->SetAutoResize( context.tempSettings.autoresizeCategoryWindow );
    }

    m_settings = context.tempSettings;
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

// Fills the "Sound output" combo (settings.spec.md SET-F15): Windows default,
// then the devices from EnumerateOutputDevices (virtual cables first), and the
// selected device as "<name> (not connected)" when it is not present, so
// opening Settings and pressing OK never silently drops the choice.
static void PopulateOutputDeviceCombo( HWND hDlg, SettingsDialogContext * ctx, const std::wstring & language,
  const std::wstring & selectedId, const std::wstring & selectedName )
{
  HWND hCombo = GetDlgItem( hDlg, IDC_SETTINGS_OUTPUT_DEVICE_COMBO );
  if( !hCombo ) return;
  SendMessage( hCombo, CB_RESETCONTENT, 0, 0 );

  int idx = (int) SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM) GetLocalizedString( SETTINGS_OUTPUT_DEVICE_DEFAULT_ID, language ) );
  SendMessage( hCombo, CB_SETITEMDATA, idx, OUTPUT_DEVICE_ITEM_DEFAULT );
  int selectedIndex = idx;
  bool selectedFound = selectedId.empty();

  for( size_t i = 0; i < ctx->outputDevices.size(); ++i )
  {
    const AudioOutputDevice & device = ctx->outputDevices[i];
    bool isSelected = !selectedId.empty() && _wcsicmp( device.id.c_str(), selectedId.c_str() ) == 0;
#ifdef SETTINGS_OUTPUT_DEVICE_SHORT_LIST
    if( !device.isVirtualCable && !isSelected ) continue;
#endif
    idx = (int) SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM) device.name.c_str() );
    SendMessage( hCombo, CB_SETITEMDATA, idx, (LPARAM) i );
    if( isSelected )
    {
      selectedIndex = idx;
      selectedFound = true;
    }
  }

  if( !selectedFound )
  {
    std::wstring label = ( selectedName.empty() ? selectedId : selectedName ) + L" " + GetLocalizedString( SETTINGS_OUTPUT_DEVICE_NOT_CONNECTED_ID, language );
    idx = (int) SendMessage( hCombo, CB_ADDSTRING, 0, (LPARAM) label.c_str() );
    SendMessage( hCombo, CB_SETITEMDATA, idx, OUTPUT_DEVICE_ITEM_DISCONNECTED );
    selectedIndex = idx;
  }

  SendMessage( hCombo, CB_SETCURSEL, selectedIndex, 0 );
}

// Reads the "Sound output" combo; empty id = Windows default.
static void GetSelectedOutputDevice( HWND hDlg, SettingsDialogContext * ctx, std::wstring & id, std::wstring & name )
{
  id = ctx->tempSettings.outputDeviceId;
  name = ctx->tempSettings.outputDeviceName;
  HWND hCombo = GetDlgItem( hDlg, IDC_SETTINGS_OUTPUT_DEVICE_COMBO );
  int sel = hCombo ? (int) SendMessage( hCombo, CB_GETCURSEL, 0, 0 ) : CB_ERR;
  if( sel == CB_ERR ) return;

  LPARAM data = SendMessage( hCombo, CB_GETITEMDATA, sel, 0 );
  if( data == OUTPUT_DEVICE_ITEM_DISCONNECTED )
    return; // keep the saved, currently disconnected device
  if( data >= 0 && (size_t) data < ctx->outputDevices.size() )
  {
    id = ctx->outputDevices[(size_t) data].id;
    name = ctx->outputDevices[(size_t) data].name;
  }
  else
  {
    id.clear();
    name.clear();
  }
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
  SetDlgItemText( hDlg, IDC_SETTINGS_SHOW_QUICK_BUTTONS, GetLocalizedString( SETTINGS_SHOW_QUICK_BUTTONS_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_LANGUAGE, GetLocalizedString( SETTINGS_LANGUAGE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_VOICE, GetLocalizedString( SETTINGS_VOICE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_TEST_VOICE, GetLocalizedString( SETTINGS_TEST_VOICE_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_VOLUME, GetLocalizedString( SETTINGS_VOLUME_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_RATE, GetLocalizedString( SETTINGS_RATE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_SPEAK_ON_CLICK, GetLocalizedString( SETTINGS_SPEAK_ON_CLICK_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_REMEMBER_CATEGORY_WINDOW, GetLocalizedString( SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_MINIMIZE_CATEGORY_WINDOW, GetLocalizedString( SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_AUTORESIZE_CATEGORY_WINDOW, GetLocalizedString( SETTINGS_AUTORESIZE_CATEGORY_WINDOW_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING, GetLocalizedString( SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING, GetLocalizedString( SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_STOP_PREVIOUS_PLAYBACK, GetLocalizedString( SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_SHOW_TOUCH_KEYBOARD, GetLocalizedString( SETTINGS_SHOW_TOUCH_KEYBOARD_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LABEL_OUTPUT_DEVICE, GetLocalizedString( SETTINGS_OUTPUT_DEVICE_LABEL_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_LISTEN_SETTINGS, GetLocalizedString( SETTINGS_LISTEN_SETTINGS_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDOK, GetLocalizedString( SETTINGS_OK_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDCANCEL, GetLocalizedString( SETTINGS_CANCEL_BUTTON_ID, language ) );
  SetDlgItemText( hDlg, IDC_SETTINGS_HELP, GetLocalizedString( SETTINGS_HELP_BUTTON_ID, language ) );

  SettingsDialogContext * ctx = reinterpret_cast<SettingsDialogContext *>( GetWindowLongPtr( hDlg, GWLP_USERDATA ) );
  if( ctx )
  {
    // Re-fill the output list so "Windows default" / "(not connected)" follow the language, keeping the selection.
    std::wstring id = ctx->tempSettings.outputDeviceId;
    std::wstring name = ctx->tempSettings.outputDeviceName;
    if( SendDlgItemMessage( hDlg, IDC_SETTINGS_OUTPUT_DEVICE_COMBO, CB_GETCOUNT, 0, 0 ) > 0 )
      GetSelectedOutputDevice( hDlg, ctx, id, name );
    PopulateOutputDeviceCombo( hDlg, ctx, language, id, name );

    if( ctx->hListenTooltip )
    {
      TOOLINFO ti = {};
      ti.cbSize = TTTOOLINFOW_V2_SIZE;
      ti.hwnd = hDlg;
      ti.uId = (UINT_PTR) GetDlgItem( hDlg, IDC_SETTINGS_LISTEN_SETTINGS );
      ti.lpszText = (LPWSTR) GetLocalizedString( SETTINGS_LISTEN_SETTINGS_TOOLTIP_ID, language );
      SendMessage( ctx->hListenTooltip, TTM_UPDATETIPTEXT, 0, (LPARAM) &ti );
    }
  }
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
        // Lift the control's 30,000-character default (CAT-N06).
        SendDlgItemMessage( hDlg, IDC_SETTINGS_DEFAULT_TEXT, EM_SETLIMITTEXT, 0, 0 );
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
        SendDlgItemMessage( hDlg, IDC_SETTINGS_AUTORESIZE_CATEGORY_WINDOW, BM_SETCHECK,
          ctx->tempSettings.autoresizeCategoryWindow ? BST_CHECKED : BST_UNCHECKED, 0 );
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
        SendDlgItemMessage( hDlg, IDC_SETTINGS_SHOW_QUICK_BUTTONS, BM_SETCHECK,
          ctx->tempSettings.showQuickButtons ? BST_CHECKED : BST_UNCHECKED, 0 );

        // "Sound output" list (filled by UpdateSettingsDialogLocalization) and the
        // "Listen settings..." tooltip explaining what to do in the Windows dialog.
        ctx->outputDevices = PlaybackEngine::EnumerateOutputDevices();
        ctx->hListenTooltip = CreateWindowEx( 0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP,
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hDlg, NULL, GetModuleHandle( NULL ), NULL );
        if( ctx->hListenTooltip )
        {
          TOOLINFO ti = {};
          ti.cbSize = TTTOOLINFOW_V2_SIZE;
          ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
          ti.hwnd = hDlg;
          ti.uId = (UINT_PTR) GetDlgItem( hDlg, IDC_SETTINGS_LISTEN_SETTINGS );
          ti.lpszText = (LPWSTR) L"";
          SendMessage( ctx->hListenTooltip, TTM_ADDTOOL, 0, (LPARAM) &ti );
          SendMessage( ctx->hListenTooltip, TTM_SETMAXTIPWIDTH, 0, 320 );
          SendMessage( ctx->hListenTooltip, TTM_SETDELAYTIME, TTDT_AUTOPOP, 30000 );
        }

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

            // Configure the preview voice on the UI thread, then hand ownership to a
            // detached worker so the synchronous Speak/WaitUntilDone doesn't freeze
            // the settings dialog. The worker is responsible for releasing the voice.
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

              // Preview on the device chosen in the dialog, so the cable routing can be checked from here.
              std::wstring outputId, outputName;
              GetSelectedOutputDevice( hDlg, ctx, outputId, outputName );
              UINT waveOutId = 0;
              if( PlaybackEngine::FindWaveOutDeviceId( outputId, waveOutId ) )
              {
                ISpMMSysAudio * pAudio = nullptr;
                if( SUCCEEDED( CoCreateInstance( CLSID_SpMMAudioOut, nullptr, CLSCTX_ALL, IID_ISpMMSysAudio, (void **) &pAudio ) ) && pAudio )
                {
                  if( SUCCEEDED( pAudio->SetDeviceId( waveOutId ) ) )
                    previewVoice->SetOutput( pAudio, TRUE );
                  pAudio->Release();
                }
              }

              std::thread( [previewVoice, sample = std::move( sampleText )]()
                {
                  CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED );
                  previewVoice->Speak( sample.c_str(), SPF_IS_NOT_XML, nullptr );
                  previewVoice->WaitUntilDone( INFINITE );
                  previewVoice->Release();
                  CoUninitialize();
                } ).detach();
            }
            return TRUE;
          }

          case IDOK:
          {
            // Sized from the control, not a fixed buffer: a pasted default
            // text longer than the old 1024 wchar was silently truncated.
            ctx->tempSettings.defaultText = ReadEditControlText( GetDlgItem( hDlg, IDC_SETTINGS_DEFAULT_TEXT ) );
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
            ctx->tempSettings.autoresizeCategoryWindow = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_AUTORESIZE_CATEGORY_WINDOW, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.minimizeCategoryWindowAutomatically = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_MINIMIZE_CATEGORY_WINDOW, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.increaseVolumeWhenPlaying = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.reduceOtherAudioWhenPlaying = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.stopPreviousPlayback = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_STOP_PREVIOUS_PLAYBACK, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.showTouchKeyboard = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_SHOW_TOUCH_KEYBOARD, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            ctx->tempSettings.showQuickButtons = ( SendDlgItemMessage( hDlg, IDC_SETTINGS_SHOW_QUICK_BUTTONS, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            GetSelectedOutputDevice( hDlg, ctx, ctx->tempSettings.outputDeviceId, ctx->tempSettings.outputDeviceName );

            ctx->accepted = true;
            EndDialog( hDlg, IDOK );
            return TRUE;
          }

          case IDCANCEL:
            EndDialog( hDlg, IDCANCEL );
            return TRUE;

          case IDC_SETTINGS_LISTEN_SETTINGS:
            // Classic Sound control panel, Recording tab: where "Listen to this device"
            // lives. Windows offers apps no supported way to change it, so the user does.
            ShellExecute( hDlg, L"open", L"control.exe", L"mmsys.cpl,,1", NULL, SW_SHOWNORMAL );
            return TRUE;

          case IDC_SETTINGS_HELP:
            // In the language picked in the dialog, so it matches its text (SET-F17).
            OpenHelpPage( GetSelectedLanguageForLocalization( hDlg, ctx ) );
            return TRUE;
        }
        break;
      }

      case WM_HELP:
        // F1 while the modal dialog is open (the main window's accelerator can't see it).
        OpenHelpPage( GetSelectedLanguageForLocalization( hDlg, ctx ) );
        return TRUE;

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
