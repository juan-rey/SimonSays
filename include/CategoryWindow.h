/*
   CategoryWindow.h: CategoryWindow class header file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef CategoryWindow_h
#define CategoryWindow_h

#include "MainWindow.h"
#include "stdafx.h"
#include "RegistryManager.h"
#include "SSButton.h"
#include "BoardStyle.h"

#define CATEGORY_BUTTON_WIDTH 120
#define CATEGORY_BUTTON_HEIGHT 40
#define CATEGORY_BUTTON_MARGIN 10
#define PHRASE_BUTTON_WIDTH CATEGORY_BUTTON_WIDTH
#define PHRASE_BUTTON_HEIGHT CATEGORY_BUTTON_HEIGHT
#define PHRASE_BUTTON_MARGIN CATEGORY_BUTTON_MARGIN

#define NORMAL_BUTTON_STYLE ( WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_MULTILINE )
#define FLAT_BUTTON_STYLE ( NORMAL_BUTTON_STYLE | BS_FLAT )


class MainWindow;

class CategoryWindow
{
public:
  CategoryWindow( MainWindow * mainWindow, bool rememberWindowSize = true, bool minimizeWhenLosingFocus = true );
  ~CategoryWindow();

  bool Create( HINSTANCE hInstance );
  void UpdateCategories( const std::vector<Category> & categories, std::wstring language, int selectedCategory = -1, const std::wstring & boardStyle = L"" );
  bool IsVisible();
  void Show();
  void Hide();
  void EditLastSelection();
  void EditBoardStyle();
  void DeleteLastSelection();
  void DeleteAllCategories();               
  void MoveSelection( int delta );
  void AddAfterSelection();
  void ImportCategories( std::wstring filePath = L"" );
  void ExportCategories();
  void ZoomIn();
  void ZoomOut();
  void ResetZoom();
  void SetMinimizeWhenLosingFocus( bool minimize ) { m_minimizeWhenLosingFocus = minimize; }
  void SetRememberWindowSize( bool rememberSize ) { m_rememberWindowSize = rememberSize; }
  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  HWND GetHwnd() const { return m_hwnd; }

private:
  void RefreshLayout();
  void LayoutCalcs();
  // Board & category styles (docs/specs/board-style.spec.md §6.5):
  // ApplyBoardStyle parses m_boardStyleRaw and refreshes window colors/DWM,
  // separator brush, button metrics/configs/styles, and fonts.
  void ApplyBoardStyle();
  // Rebuilds m_icoFileFolders: board resource subfolder (when any) first, then
  // app-data root, working dir, exe dir (import-export.spec.md lookup order).
  void RebuildResourceSearchFolders();
  void UpdatePhraseMetricsForCategory( int categoryIndex );
  void UpdateSeparatorStyles();
  void RebuildFonts();
  // Text shown in the strip between the separators: the board's caption when
  // set, otherwise the localized shortcuts hint (STY-F56).
  std::wstring EffectiveDisplayText() const;
  HFONT CreateStyledFont( const StyleProps & props, bool bold ) const;
  SSButtonConfig CategoryButtonConfigFor( size_t categoryIndex ) const;
  StyleProps PhrasePropsForCategory( int categoryIndex ) const;
  void CreateCategoryButtons();
  void UpdateButtonIcons();
  void UpdatePhraseButtonIcons();
  void SafeTextResize();
  void CreatePhraseButtons( const Category & category );
  void OnCategorySelected( int categoryIndex );
  void OnPhraseSelected( int phraseIndex );
  bool ShowEditDialog( std::wstring & text, bool add = false );
  static INT_PTR CALLBACK EditDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );

  HWND m_hwnd = NULL;
  HINSTANCE m_hInstance = NULL;
  MainWindow * m_mainWindow;
  HWND m_hVerticalSeparatorL = NULL;
  HWND m_hVerticalSeparatorR = NULL;
  HWND m_hDisplayText = NULL;
  HFONT m_hCategoryButtonFont = NULL;
  HFONT m_hSelectedCategoryButtonFont = NULL;
  HFONT m_hPhraseButtonFont = NULL;
  HFONT m_hDisplayTextFont = NULL;
  COLORREF m_textColor = RGB( 0, 0, 0 );
  COLORREF m_effectiveBgColor = RGB( 0, 0, 0 ); // styled board background, or taskbar color
  HBRUSH m_backgroundBrush = NULL;
  HBRUSH m_separatorBrush = NULL; // only when separator-color is styled (STY-F42)

  std::wstring m_language;
  std::wstring m_boardStyleRaw; // raw $$board style list; re-emitted on every save (board-style.spec.md)
  bool m_minimizeWhenLosingFocus;
  bool m_rememberWindowSize;
  bool m_rtlLayout = false;
  std::vector<Category> m_categories;
  std::vector<SSButton> m_categoryButtons;
  std::vector<SSButton> m_phraseButtons;
  int m_selectedCategoryIndex = -1;
  bool m_categorySelectedLast = true;
  int m_selectedPhraseIndex = -1;
  BoardStyle m_boardStyle;                 // parsed from m_boardStyleRaw
  SSButtonConfig m_categoryButtonConfig;   // board-level; per-category colors layered in CategoryButtonConfigFor
  SSButtonConfig m_phraseButtonConfig;     // effective for the selected category
  DWORD m_categoryButtonStyle = NORMAL_BUTTON_STYLE; // + styled BS_* alignment flags
  DWORD m_phraseButtonStyle = NORMAL_BUTTON_STYLE;

  std::vector<std::wstring> m_icoFileFolders;
  std::wstring m_boardResourceFolder; // title-derived subfolder of the active board style; empty = none

  int m_category_button_width = CATEGORY_BUTTON_WIDTH;
  int m_category_button_height = CATEGORY_BUTTON_HEIGHT;
  int m_category_button_margin = CATEGORY_BUTTON_MARGIN;
  int m_default_window_width = 3 * ( CATEGORY_BUTTON_WIDTH + CATEGORY_BUTTON_MARGIN ) + CATEGORY_BUTTON_MARGIN + 14;
  int m_default_window_height = 494;
  int m_phrase_button_width = PHRASE_BUTTON_WIDTH;
  int m_phrase_button_height = PHRASE_BUTTON_HEIGHT;
  int m_phrase_button_margin = PHRASE_BUTTON_MARGIN;

  float m_zoom_factor = 1.0f;

  inline int real_category_button_width() const { return static_cast<int>( m_category_button_width * m_zoom_factor ); }
  inline int real_category_button_height() const { return static_cast<int>( m_category_button_height * m_zoom_factor ); }
  inline int real_category_button_margin() const { return static_cast<int>( m_category_button_margin * m_zoom_factor ); }
  inline int real_phrase_button_width() const { return static_cast<int>( m_phrase_button_width * m_zoom_factor ); }
  inline int real_phrase_button_height() const { return static_cast<int>( m_phrase_button_height * m_zoom_factor ); }
  inline int real_phrase_button_margin() const { return static_cast<int>( m_phrase_button_margin * m_zoom_factor ); }

  int m_categories_per_row = 1;
  int m_free_inner_category_buttons_margin = 0;
  int m_vertical_separator_y = m_category_button_margin + m_category_button_height + m_category_button_margin;
  int m_vertical_separator_width = m_category_button_width;
  int m_phrase_buttons_start_y = ( 2 * m_category_button_margin ) + 2 + ( 1 * ( m_category_button_height + m_category_button_margin ) );
  int m_phrases_per_row = 1;
  int m_free_inner_phrase_buttons_margin = 0;
  SIZE m_display_text_size = { 0, 0 };
  int m_display_text_y = m_phrase_buttons_start_y + ( 2 * m_phrase_button_margin ) + ( 1 * ( m_phrase_button_height + m_phrase_button_margin ) );
  int m_display_text_x = m_category_button_margin;
};

#endif // CategoryWindow_h