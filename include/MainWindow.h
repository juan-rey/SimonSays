/*
   MainWindow.h: MainWindow class header file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef MainWindow_h
#define MainWindow_h

#include "CategoryWindow.h"
#include "PlaybackEngine.h"
#include "SSButton.h"
#include "stdafx.h"
#include "utils.h"

class CategoryWindow;

struct SettingsDialogContext;

#define MW_DEFAULT_BUTTON_WIDTH 80
#define MW_DEFAULT_BUTTON_HEIGHT 36
#define MW_DEFAULT_ICON_BUTTON_SIZE 20
#define MW_DEFAULT_HORIZONTAL_MARGIN 10
#define MW_DEFAULT_WINDOW_WIDTH 400       // preferred bar width on a horizontal taskbar (TBR-F20)
#define MW_DEFAULT_WINDOW_HEIGHT 46
#define FALLBACK_TRAY_WIDTH 300
#define TRAY_MARGIN 24
#define MW_START_NEAR_EDGE 99             // Start within this of the taskbar's start = left/top-aligned (TBR-F02)
#define MW_THIN_TASKBAR_THICKNESS 44      // thinner horizontal taskbars use the Thin tier (TBR-F21)
#define MW_MIN_EDIT_WIDTH 80              // the edit box never shrinks below this (TBR-F20)
#define MW_VERTICAL_MARGIN 4              // gap between stacked controls on a vertical taskbar (TBR-F22)
#define MW_VERTICAL_EDIT_HEIGHT 40        // two text lines (TBR-F22)

class MainWindow
{
public:
  MainWindow();
  ~MainWindow();

  HWND GetHwnd() const { return m_hwnd; }
  const TaskbarGeometry & GetTaskbar() const { return m_taskbar; }
  bool Create( HINSTANCE hInstance, int nCmdShow );
  void RunMessageLoop();

  void PlayCurrentText();
  void AddTextToEditControl( const std::wstring & text );
  void SetEditControlText( const std::wstring & text );
  void OnCategoryWindowHidden();
  // Pushed by CategoryWindow::ApplyBoardStyle whenever the board style is
  // (re)applied: the title-derived resource subfolder (may be empty) feeds the
  // playback engine's sound-file lookup.
  void OnBoardResourceFolderChanged( const std::wstring & folder );
  //void OpenCategoriesFile( const std::wstring & filePath );

private:
  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  static INT_PTR CALLBACK SettingsDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
  static LRESULT CALLBACK LowLevelMouseProc( int nCode, WPARAM wParam, LPARAM lParam );

  void ApplyVoiceSettings();
  bool CreateTaskbarControls();
  bool UpdateTaskbarControlsPosition();
  RECT ComputeBarRect() const;                  // TBR-F02/F20/F22
  int  QuickButtonsWidth() const;
  void UpdateTaskbarFonts( bool compact );     // TBR-F21
  void UpdateCategoryButtonText( bool categoryWindowShown, const std::wstring & language ); // TBR-F22/F23
  void UpdatePlayButtonSymbol( bool playing ); // TBR-F22
  void RelayoutIfTaskbarChanged( bool force ); // TBR-F40
  void CreateTrayIcon();
  static std::wstring GetSelectedLanguageForLocalization( HWND hDlg, SettingsDialogContext * ctx );
  static void PopulateLanguageCombo( HWND hDlg, SettingsDialogContext * ctx );
  bool RegisterWindowClass( HINSTANCE hInstance );
  void RemoveTrayIcon();
  void ShowContextMenu( HWND hwnd, POINT pt );
  void ShowHideCategoryWindow();
  void ShowSettingsDialog();
  void ShowDwellWindow();
  void ApplyDwellSettingsToConfig(); // push m_settings dwell fields into SSDwellConfig
  void SyncDwellTimers();            // start/stop the Auto-mode detect timer per ModeSelection
  void UpdateDwellPassiveSignals();  // feed SSGazeDetect results to the detector
  void EvaluateDwellAutoMode();      // apply Decide() with hysteresis when Auto
  void ShowHelpWindow();
  void EnsureTaskbarShown();         // taskbar.spec.md TBR-F10/F12
  void RestoreTaskbarAutoHide();     // TBR-F11; idempotent
  void ShowWidgetsHintOnce();        // TBR-F13
  static void UpdateSettingsDialogLocalization( HWND hDlg, const std::wstring & language );
  void UpdateTaskbarUI();
  void UpdateUILanguage( const std::wstring language );


  int m_categoryButtonWidth = MW_DEFAULT_BUTTON_WIDTH + 20;
  int m_playButtonWidth = MW_DEFAULT_BUTTON_WIDTH;
  int m_buttonHeight = MW_DEFAULT_BUTTON_HEIGHT;
  int m_horizontalMargin = MW_DEFAULT_HORIZONTAL_MARGIN;
  int m_iconButtonSize = MW_DEFAULT_ICON_BUTTON_SIZE;
  TaskbarGeometry m_taskbar;
  int         m_fontsCompact = -1;  // -1 = fonts not created yet
  bool        m_categoryWindowShownArrow = true; // last state shown by the Categories arrow
  HFONT       m_hRegularFont = nullptr;
  HFONT       m_hBoldFont = nullptr;
  HFONT       m_hSymbolFont = nullptr; // vertical-taskbar button symbols (TBR-F22); null otherwise
  HACCEL      m_hAccel = nullptr;
  HINSTANCE   m_hInstance = nullptr;
  SSButton    m_categoryButton;
  SSButton    m_boardsButton;
  SSButton    m_settingsButton;
  HWND        m_hEditControl = nullptr;
  SSButton    m_playButton;
  HWND        m_hwnd = nullptr;
  std::unique_ptr<PlaybackEngine> m_playbackEngine;
  NOTIFYICONDATA m_nid = {};
  POINT          m_inButtonPoint = { 0, 0 };
  Settings       m_settings;
  bool           m_showingSettingDialog = false;
  SSDwellMode    m_dwellPendingMode = SSDwellMode::Off; // hysteresis: candidate Auto mode
  int            m_dwellDecisionStreak = 0;             // consecutive identical decisions
  std::unique_ptr<CategoryWindow> m_categoryWindow;
  std::vector<Category>           m_categories;
};

#endif