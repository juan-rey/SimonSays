#pragma once

#ifndef MainWindow_h
#define MainWindow_h

#include "CategoryWindow.h"
#include "stdafx.h"
#include "RegistryManager.h"
#include <sapi.h>

class CategoryWindow;

class MainWindow
{
public:
  MainWindow();
  ~MainWindow();

  HWND GetHwnd() const { return m_hwnd; }
  bool Create( HINSTANCE hInstance, int nCmdShow );
  void RunMessageLoop();
  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

  void PlayCurrentText();
  void AddTextToEditControl( const std::wstring & text );
  void SetEditControlText( const std::wstring & text );

private:
  void ShowHideCategoryWindow();
  void UpdateTaskbarUI();
  void UpdateUILanguage( const std::wstring language );
  bool RegisterWindowClass( HINSTANCE hInstance );
  bool CreateTaskbarControls();
  void ShowSettingsDialog();
  void ApplyVoiceSettings();
  static INT_PTR CALLBACK SettingsDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
  static LRESULT CALLBACK LowLevelMouseProc( int nCode, WPARAM wParam, LPARAM lParam );
  void CreateTrayIcon();
  void RemoveTrayIcon();
  void ShowContextMenu( HWND hwnd, POINT pt );
  
  HWND m_hwnd;

  ISpVoice * pVoice = nullptr;
  HWND m_hPlayButton;
  HWND m_hCategoryButton;
  HWND m_hEditControl;
  HINSTANCE m_hInstance;
  POINT m_inButtonPoint;
  NOTIFYICONDATA m_nid;
  std::unique_ptr<CategoryWindow> m_categoryWindow;
  //std::unique_ptr<SimonGame> m_simonGame;
  Settings m_settings;
  std::vector<Category> m_categories;
  HACCEL m_hAccel;
  //std::wstring m_currentLanguage;
};

#endif