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
#include "HelpWindow.h"
#include "stdafx.h"
#include <sapi.h>

class CategoryWindow;

struct SettingsDialogContext;

#define MW_DEFAULT_BUTTON_WIDTH 80
#define MW_DEFAULT_BUTTON_HEIGHT 32
#define MW_DEFAULT_HORIZONTAL_MARGIN 10
#define MW_DEFAULT_WINDOW_WIDTH 400
#define MW_DEFAULT_WINDOW_HEIGHT 46
#define FALLBACK_TRAY_WIDTH 300
#define TRAY_MARGIN 24

class MainWindow
{
public:
  MainWindow();
  ~MainWindow();

  HWND GetHwnd() const { return m_hwnd; }
  bool Create( HINSTANCE hInstance, int nCmdShow );
  void RunMessageLoop();

  void PlayCurrentText();
  void AddTextToEditControl( const std::wstring & text );
  void SetEditControlText( const std::wstring & text );
  void OnCategoryWindowHidden();
  //void OpenCategoriesFile( const std::wstring & filePath );

private:
  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  static INT_PTR CALLBACK SettingsDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
  static LRESULT CALLBACK LowLevelMouseProc( int nCode, WPARAM wParam, LPARAM lParam );

  void ApplyVoiceSettings();
  bool CreateTaskbarControls();
  void CreateTrayIcon();
  static std::wstring GetSelectedLanguageForLocalization( HWND hDlg, SettingsDialogContext * ctx );
  static void PopulateLanguageCombo( HWND hDlg, SettingsDialogContext * ctx );
  bool RegisterWindowClass( HINSTANCE hInstance );
  void RemoveTrayIcon();
  void ShowContextMenu( HWND hwnd, POINT pt );
  void ShowHideCategoryWindow();
  void ShowSettingsDialog();
  void ShowHelpWindow();
  static void UpdateSettingsDialogLocalization( HWND hDlg, const std::wstring & language );
  void UpdateTaskbarUI();
  void UpdateUILanguage( const std::wstring language );


  int m_categoryButtonWidth = MW_DEFAULT_BUTTON_WIDTH + 20;
  int m_playButtonWidth = MW_DEFAULT_BUTTON_WIDTH;
  int m_buttonHeight = MW_DEFAULT_BUTTON_HEIGHT;
  int m_horizontalMargin = MW_DEFAULT_HORIZONTAL_MARGIN;
  HFONT m_hRegularFont = NULL;
  HFONT m_hBoldFont = NULL;
  HACCEL m_hAccel;
  HINSTANCE m_hInstance;
  HWND m_hCategoryButton;
  HWND m_hEditControl;
  HWND m_hPlayButton;
  HWND m_hwnd;
  ISpVoice * m_pVoice = nullptr;
  NOTIFYICONDATA m_nid;
  POINT m_inButtonPoint;
  Settings m_settings;
  bool m_showingSettingDialog = false;
  std::unique_ptr<CategoryWindow> m_categoryWindow;
  std::vector<Category> m_categories;
  std::unique_ptr<HelpWindow> m_helpWindow;
};

#endif