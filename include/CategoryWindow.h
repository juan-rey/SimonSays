#pragma once


#ifndef CategoryWindow_h
#define CategoryWindow_h

#include "MainWindow.h"
#include "stdafx.h"
#include "RegistryManager.h"

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

  const int BUTTON_WIDTH = 120;
  const int BUTTON_HEIGHT = 40;
  const int BUTTON_MARGIN = 10;
  const int CATEGORY_SECTION_HEIGHT = 160;
};

#endif // CategoryWindow_h