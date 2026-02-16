#pragma once


#ifndef CategoryWindow_h
#define CategoryWindow_h

#include "MainWindow.h"
#include "stdafx.h"
#include "RegistryManager.h"

#define CATEGORY_BUTTON_WIDTH 120
#define CATEGORY_BUTTON_HEIGHT 40
#define CATEGORY_BUTTON_MARGIN 10
#define PHRASE_BUTTON_WIDTH CATEGORY_BUTTON_WIDTH
#define PHRASE_BUTTON_HEIGHT CATEGORY_BUTTON_HEIGHT
#define PHRASE_BUTTON_MARGIN CATEGORY_BUTTON_MARGIN


class MainWindow;

class CategoryWindow
{
public:
  CategoryWindow( MainWindow * mainWindow, bool rememberWindowSize = true, bool minimizeWhenLosingFocus = true );
  ~CategoryWindow();

  bool Create( HINSTANCE hInstance );
  void UpdateCategories( const std::vector<Category> & categories );
  void UpdateUILanguage( const std::wstring language );
  bool IsVisible();
  void Show();
  void Hide();
  void SetMinimizeWhenLosingFocus( bool minimize ) { m_minimizeWhenLosingFocus = minimize; }
  void SetRememberWindowSize( bool rememberSize ) { m_rememberWindowSize = rememberSize; }
  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
  void RefreshLayout();
  bool RegisterWindowClass( HINSTANCE hInstance );
  void CreateCategoryButtons();
  void CreatePhraseButtons( const Category & category );
  void ClearPhraseButtons();
  void OnCategorySelected( int categoryIndex );
  void OnPhraseSelected( int phraseIndex );

  HWND m_hwnd;
  HINSTANCE m_hInstance;
  MainWindow * m_mainWindow;
  HWND m_hseparator;
  HFONT m_hCategoryButtonFont = NULL;
  HFONT m_hSelectedCategoryButtonFont = NULL;
  HFONT m_hPhraseButtonFont = NULL;


  bool m_minimizeWhenLosingFocus;
  bool m_rememberWindowSize;
  std::vector<Category> m_categories;
  std::vector<HWND> m_categoryButtons;
  std::vector<HWND> m_phraseButtons;
  int m_selectedCategoryIndex;

  int m_category_button_width = CATEGORY_BUTTON_WIDTH;
  int m_category_button_height = CATEGORY_BUTTON_HEIGHT;
  int m_category_button_margin = CATEGORY_BUTTON_MARGIN;
  int m_default_window_width = 3 * ( CATEGORY_BUTTON_WIDTH + CATEGORY_BUTTON_MARGIN ) + CATEGORY_BUTTON_MARGIN + 14;
  int m_default_window_height = 494;
  int m_phrase_button_width = PHRASE_BUTTON_WIDTH;
  int m_phrase_button_height = PHRASE_BUTTON_HEIGHT;
  int m_phrase_button_margin = PHRASE_BUTTON_MARGIN;
};

#endif // CategoryWindow_h