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

  bool Create( HINSTANCE hInstance, int nCmdShow );
  void RunMessageLoop();

  void ShowCategoryWindow();
  void UpdateTaskbarUI();
  void PlayCurrentText();

  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  HWND m_hEditControl;
  HWND m_hwnd;

private:
  bool RegisterWindowClass( HINSTANCE hInstance );
  bool CreateTaskbarControls();
  void CreateTrayIcon();
  void RemoveTrayIcon();
  void ShowContextMenu( HWND hwnd, POINT pt );
  void HandleTrayMessage( UINT message );

  ISpVoice * pVoice = nullptr;
  HWND m_hPlayButton;
  HWND m_hCategoryButton;
  HINSTANCE m_hInstance;

  NOTIFYICONDATA m_nid;
  std::unique_ptr<CategoryWindow> m_categoryWindow;
  //std::unique_ptr<SimonGame> m_simonGame;

  std::vector<Category> m_categories;
  std::wstring m_currentLanguage;
};

#endif