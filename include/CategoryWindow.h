#pragma once


#ifndef CategoryWindow_h
#define CategoryWindow_h

#include "MainWindow.h"
#include "stdafx.h"
#include "RegistryManager.h"

#define DEFAULT_BUTTON_WIDTH 120
#define DEFAULT_BUTTON_HEIGHT 40
#define DEFAULT_BUTTON_MARGIN 10

class MainWindow;

class CategoryWindow
{
public:
  CategoryWindow( MainWindow * mainWindow );
  ~CategoryWindow();

  bool Create( HINSTANCE hInstance );
  void Show();
  void Hide();
  void UpdateCategories( const std::vector<Category> & categories );
  void RefreshLayout();

  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
  bool RegisterWindowClass( HINSTANCE hInstance );
  void CreateCategoryButtons();
  void CreatePhraseButtons( const Category & category );
  void ClearPhraseButtons();
  void OnCategorySelected( int categoryIndex );
  void OnPhraseSelected( int phraseIndex );

  HWND m_hwnd;
  HINSTANCE m_hInstance;
  MainWindow * m_mainWindow;

  std::vector<Category> m_categories;
  std::vector<HWND> m_categoryButtons;
  std::vector<HWND> m_phraseButtons;
  int m_selectedCategoryIndex;

  int m_button_width = DEFAULT_BUTTON_WIDTH;
  int m_button_height = DEFAULT_BUTTON_HEIGHT;
  int m_button_margin = DEFAULT_BUTTON_MARGIN;
};

#endif // CategoryWindow_h