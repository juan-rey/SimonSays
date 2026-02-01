#pragma once

#ifndef MainWindow_h
#define MainWindow_h

#include "CategoryWindow.h"
#include "stdafx.h"
#include <sapi.h>

class CategoryWindow;

struct SettingsDialogContext;

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
  static void UpdateSettingsDialogLocalization( HWND hDlg, const std::wstring & language );
  void UpdateTaskbarUI();
  void UpdateUILanguage( const std::wstring language );
  
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
};

#endif