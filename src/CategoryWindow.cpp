#include "CategoryWindow.h"
#include "utils.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#define NORMAL_BUTTON_STYLE ( WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_MULTILINE )
#define FLAT_BUTTON_STYLE ( WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_MULTILINE | BS_FLAT )

CategoryWindow::CategoryWindow( MainWindow * mainWindow, bool savedWindowSize, bool minimizeWhenLosingFocus )
  : m_hwnd( NULL ), m_hseparator( NULL ), m_mainWindow( mainWindow ), m_selectedCategoryIndex( -1 ), m_rememberWindowSize( savedWindowSize ), m_minimizeWhenLosingFocus( minimizeWhenLosingFocus )
{
}

CategoryWindow::~CategoryWindow()
{
  if( m_hwnd && m_rememberWindowSize )
  {
    RECT rc;
    GetWindowRect( m_hwnd, &rc );
    RegistryManager::SaveCategoryWindowSizeToRegistry( rc.right - rc.left, rc.bottom - rc.top );
  }

  if( m_hCategoryButtonFont )
  {
    DeleteObject( m_hCategoryButtonFont );
    m_hCategoryButtonFont = NULL;
  }

  if( m_hSelectedCategoryButtonFont )
  {
    DeleteObject( m_hSelectedCategoryButtonFont );
    m_hSelectedCategoryButtonFont = NULL;
  }

  if( m_hPhraseButtonFont )
  {
    DeleteObject( m_hPhraseButtonFont );
    m_hPhraseButtonFont = NULL;
  }

  if( m_hwnd )
  {
    DestroyWindow( m_hwnd );
  }
}

bool CategoryWindow::Create( HINSTANCE hInstance )
{
  RECT rc;
  int width = m_default_window_width;
  int height = m_default_window_height;
  m_hInstance = hInstance;

  WNDCLASS wc = {};
  wc.lpfnWndProc = CategoryWindow::WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CATEGORY_WINDOW_CLASS;
  wc.hbrBackground = (HBRUSH) ( COLOR_BTNFACE + 1 );
  wc.hbrBackground = CreateSolidBrush( GetTaskbarColor() );
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );

  if( !RegisterClass( &wc ) )
  {
    return false;
  }

  GetWindowRect( m_mainWindow->GetHwnd(), &rc );
  if( !m_rememberWindowSize || !RegistryManager::LoadCategoryWindowSizeFromRegistry( width, height ) )
  {
    width = rc.right - rc.left + 14;
    height = m_default_window_height;
  }

  int x = rc.left - 4;
  int y = ( rc.top - height ) - 2;
  DWORD style = WS_POPUP | WS_THICKFRAME;
  m_hwnd = CreateWindowEx(
    WS_EX_LAYERED,
    CATEGORY_WINDOW_CLASS,
    L"SimonSays - Categories",
    style,
    x, y,
    width, height,
    NULL,
    NULL,
    hInstance,
    this
  );

  if( !m_hwnd )
  {
    return false;
  }

  DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
  DwmSetWindowAttribute( m_hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof( preference ) );

  // Apply dark theme if necessary
  BOOL useDarkMode = TRUE;
  if( GetRValue( GetTaskbarColor() ) > 128 ) useDarkMode = FALSE;
  DwmSetWindowAttribute( m_hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof( useDarkMode ) );

  SetLayeredWindowAttributes( m_hwnd, 0, 239, LWA_ALPHA );
  ShowWindow( m_hwnd, SW_SHOW );
  UpdateWindow( m_hwnd );

  return true;
}

bool CategoryWindow::IsVisible()
{
  if( m_hwnd )
  {
    return IsWindowVisible( m_hwnd ) != FALSE;
  }
  return false;
}

void CategoryWindow::Show()
{
  if( m_hwnd )
  {
    ShowWindow( m_hwnd, SW_SHOW );
    UpdateWindow( m_hwnd );
    SetForegroundWindow( m_hwnd );

    if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categoryButtons.size() )
    {
      SetFocus( m_categoryButtons[m_selectedCategoryIndex] );
    }
  }
}

void CategoryWindow::Hide()
{
  if( m_hwnd )
  {
    ShowWindow( m_hwnd, SW_HIDE );
  }
}

void CategoryWindow::UpdateCategories( const std::vector<Category> & categories, bool rtl )
{
  m_categories = categories;
  m_rtlLayout = rtl;
  CreateCategoryButtons();
  RefreshLayout();
  OnCategorySelected( 0 );
}

void CategoryWindow::UpdateUILanguage( const std::wstring language )
{

}

void CategoryWindow::RefreshLayout()
{
  if( !m_hwnd ) return;

  RECT rect;
  GetClientRect( m_hwnd, &rect );

  int categoriesPerRow = ( rect.right - m_category_button_margin ) / ( m_category_button_width + m_category_button_margin );
  if( categoriesPerRow < 1 ) categoriesPerRow = 1;
  int freeInnerCategoriesMargin = ( categoriesPerRow < 2 ) ? 0 : ( rect.right - ( categoriesPerRow * ( m_category_button_width + m_category_button_margin ) ) - m_category_button_margin ) / ( categoriesPerRow - 1 );

  for( int i = 0; i < m_categoryButtons.size(); i++ )
  {
    int row = i / categoriesPerRow;
    int col = i % categoriesPerRow;

    if( m_rtlLayout )
    {
      col = ( categoriesPerRow - 1 ) - col;
    }

    int x = m_category_button_margin + col * ( m_category_button_width + m_category_button_margin + freeInnerCategoriesMargin );
    int y = m_category_button_margin + row * ( m_category_button_height + m_category_button_margin );

    SetWindowPos( m_categoryButtons[i], NULL, x, y, m_category_button_width, m_category_button_height,
      SWP_NOZORDER | SWP_NOACTIVATE );
  }

  SetWindowPos( m_hseparator, NULL,
    m_category_button_margin,
    m_category_button_margin + ( int( ( m_categories.size() / categoriesPerRow ) + 1 ) * ( m_category_button_height + m_category_button_margin ) ),
    rect.right - 2 * m_category_button_margin,
    2,
    SWP_NOZORDER | SWP_NOACTIVATE );

  if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categories.size() )
  {
    int phraseStartY = ( 2 * m_category_button_margin ) + 2 + ( int( ( m_categories.size() / categoriesPerRow ) + 1 ) * ( m_category_button_height + m_category_button_margin ) );

    int phrasesPerRow = ( rect.right - m_phrase_button_margin ) / ( m_phrase_button_width + m_phrase_button_margin );
    if( phrasesPerRow < 1 ) phrasesPerRow = 1;
    int freeInnerPhrasesMargin = ( phrasesPerRow < 2 ) ? 0 : ( rect.right - ( phrasesPerRow * ( m_phrase_button_width + m_phrase_button_margin ) ) - m_phrase_button_margin ) / ( phrasesPerRow - 1 );

    for( int i = 0; i < m_phraseButtons.size(); i++ )
    {
      int row = i / phrasesPerRow;
      int col = i % phrasesPerRow;

      if( m_rtlLayout )
      {
        col = ( phrasesPerRow - 1 ) - col;
      }

      int x = m_phrase_button_margin + col * ( m_phrase_button_width + m_phrase_button_margin + freeInnerPhrasesMargin );
      int y = phraseStartY + row * ( m_phrase_button_height + m_phrase_button_margin );

      SetWindowPos( m_phraseButtons[i], NULL, x, y, m_phrase_button_width, m_phrase_button_height,
        SWP_NOZORDER | SWP_NOACTIVATE );
    }
  }

  RedrawWindow( m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW );
}

LRESULT CALLBACK CategoryWindow::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  CategoryWindow * pThis = nullptr;

  if( uMsg == WM_CREATE )
  {
    CREATESTRUCT * pCreate = (CREATESTRUCT *) lParam;
    pThis = (CategoryWindow *) pCreate->lpCreateParams;
    SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
  }
  else
  {
    pThis = (CategoryWindow *) GetWindowLongPtr( hwnd, GWLP_USERDATA );
  }

  if( pThis )
  {
    switch( uMsg )
    {
      case WM_COMMAND:
      {
        int wmId = LOWORD( wParam );
        int wmEvent = HIWORD( wParam );

        if( wmEvent == BN_CLICKED )
        {
          if( wmId >= 1000 && wmId < 2000 )
          {
            pThis->OnCategorySelected( wmId - 1000 );
          }
          else if( wmId >= 2000 && wmId < 3000 )
          {
            pThis->OnPhraseSelected( wmId - 2000 );
          }
        }
      }
      break;

      case WM_SIZE:
        pThis->RefreshLayout();
        break;

      case WM_GETMINMAXINFO:
      {
        MINMAXINFO * mmi = reinterpret_cast<MINMAXINFO *>( lParam );
        RECT minRect = { 0, 0,
          pThis->m_category_button_margin * 2 + pThis->m_category_button_width,
          pThis->m_category_button_margin * 2 + pThis->m_category_button_height };
        DWORD style = static_cast<DWORD>( GetWindowLongPtr( hwnd, GWL_STYLE ) );
        DWORD exStyle = static_cast<DWORD>( GetWindowLongPtr( hwnd, GWL_EXSTYLE ) );
        AdjustWindowRectEx( &minRect, style, FALSE, exStyle );
        mmi->ptMinTrackSize.x = minRect.right - minRect.left;
        mmi->ptMinTrackSize.y = minRect.bottom - minRect.top;
        return 0;
      }

      case WM_NCHITTEST: // Allow dragging the window from client area
      {
        LRESULT hit = DefWindowProc( hwnd, uMsg, wParam, lParam );
        if( hit == HTCLIENT )
        {
          return HTCAPTION;
        }
        return hit;
      }

      case WM_DESTROY:
        pThis->m_hwnd = NULL;
        break;

      case WM_ACTIVATE:
        if( LOWORD( wParam ) == WA_INACTIVE )
        {
          if( pThis->m_minimizeWhenLosingFocus )
            ShowWindow( hwnd, SW_HIDE );
        }
        break;

      default:
        return DefWindowProc( hwnd, uMsg, wParam, lParam );
    }
  }

  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

void CategoryWindow::CreateCategoryButtons()
{
  for( auto & button : m_categoryButtons )
  {
    if( button ) DestroyWindow( button );
  }
  m_categoryButtons.clear();

  if( !m_hCategoryButtonFont )
  {
    NONCLIENTMETRICS ncm = {};
    ncm.cbSize = sizeof( NONCLIENTMETRICS );
    SystemParametersInfo( SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0 );
    m_hCategoryButtonFont = CreateFontIndirect( &ncm.lfMessageFont );
  }

  for( size_t i = 0; i < m_categories.size(); i++ )
  {
    HWND hButton = CreateWindowEx(
      m_rtlLayout ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0,
      L"BUTTON",
      ReplaceAll( m_categories[i].name, L"&", L"&&" ).c_str(),
      NORMAL_BUTTON_STYLE,
      0, 0, m_category_button_width, m_category_button_height,
      m_hwnd,
      (HMENU) ( 1000 + i ),
      m_hInstance,
      NULL
    );

    if( hButton )
    {
      m_categoryButtons.push_back( hButton );
      SendMessage( hButton, WM_SETFONT, (WPARAM) m_hCategoryButtonFont, TRUE );
    }
  }

  if( !m_hseparator )
  {
    m_hseparator = CreateWindowEx(
      0,
      L"STATIC",
      NULL,
      WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ,
      0, 0, m_category_button_width, 2,
      m_hwnd,
      NULL,
      m_hInstance,
      NULL
    );
  }
}

void CategoryWindow::CreatePhraseButtons( const Category & category )
{
  ClearPhraseButtons();

  if( !m_hPhraseButtonFont )
  {
    NONCLIENTMETRICS ncm = {};
    ncm.cbSize = sizeof( NONCLIENTMETRICS );
    SystemParametersInfo( SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0 );
    m_hPhraseButtonFont = CreateFontIndirect( &ncm.lfMessageFont );
  }


  for( size_t i = 0; i < category.phrases.size(); i++ )
  {
    HWND hButton = CreateWindowEx(
      m_rtlLayout ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0,
      L"BUTTON",
      category.phrases[i].audioFile.empty() ? ( ReplaceAll( category.phrases[i].text, L"&", L"&&" ).c_str() ) : ReplaceAll( SOUND_NOTE_DELIMITER + category.phrases[i].text + SOUND_NOTE_DELIMITER, L"&", L"&&" ).c_str(),
      NORMAL_BUTTON_STYLE,
      0, 0, m_phrase_button_width, m_phrase_button_height,
      m_hwnd,
      (HMENU) ( 2000 + i ),
      m_hInstance,
      NULL
    );

    if( hButton )
    {
      m_phraseButtons.push_back( hButton );
      SendMessage( hButton, WM_SETFONT, (WPARAM) m_hPhraseButtonFont, TRUE );
    }
  }
}

void CategoryWindow::ClearPhraseButtons()
{
  for( auto & button : m_phraseButtons )
  {
    if( button ) DestroyWindow( button );
  }
  m_phraseButtons.clear();
}

void CategoryWindow::OnCategorySelected( int categoryIndex )
{
  if( categoryIndex >= 0 && categoryIndex < (int) m_categories.size() )
  {
    if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categoryButtons.size() )
    {
      SendMessage( m_categoryButtons[m_selectedCategoryIndex], WM_SETFONT, (WPARAM) m_hCategoryButtonFont, TRUE );
      SetWindowLongPtr( m_categoryButtons[m_selectedCategoryIndex], GWL_STYLE, (LONG_PTR) NORMAL_BUTTON_STYLE );
      SetWindowPos( m_categoryButtons[m_selectedCategoryIndex], NULL, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED );
      InvalidateRect( m_categoryButtons[m_selectedCategoryIndex], NULL, TRUE );
    }

    if( !m_hSelectedCategoryButtonFont )
    {
      NONCLIENTMETRICS ncm = {};
      ncm.cbSize = sizeof( NONCLIENTMETRICS );
      SystemParametersInfo( SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0 );
      ncm.lfMessageFont.lfWeight = FW_BOLD;
      m_hSelectedCategoryButtonFont = CreateFontIndirect( &ncm.lfMessageFont );
    }

    m_selectedCategoryIndex = categoryIndex;
    CreatePhraseButtons( m_categories[categoryIndex] );
    RefreshLayout();
    SendMessage( m_categoryButtons[m_selectedCategoryIndex], WM_SETFONT, (WPARAM) m_hSelectedCategoryButtonFont, TRUE );
    SetWindowLongPtr( m_categoryButtons[m_selectedCategoryIndex], GWL_STYLE, (LONG_PTR) FLAT_BUTTON_STYLE );
    SetWindowPos( m_categoryButtons[m_selectedCategoryIndex], NULL, 0, 0, 0, 0,
      SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED );
    InvalidateRect( m_categoryButtons[m_selectedCategoryIndex], NULL, TRUE );
    SetFocus( m_categoryButtons[m_selectedCategoryIndex] );
  }
}

void CategoryWindow::OnPhraseSelected( int phraseIndex )
{
  if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categories.size() )
  {
    const Category & selectedCategory = m_categories[m_selectedCategoryIndex];

    if( phraseIndex >= 0 && phraseIndex < (int) selectedCategory.phrases.size() )
    {
      const Phrase & selectedPhrase = selectedCategory.phrases[phraseIndex];

      if( m_mainWindow )
      {
        m_mainWindow->SetEditControlText( selectedPhrase.audioFile.empty() ? ( selectedPhrase.text ) : ( SOUND_NOTE_DELIMITER + selectedPhrase.audioFile + SOUND_NOTE_DELIMITER ) );
        //m_mainWindow->PlayCurrentText();
      }
    }

    SetFocus( m_categoryButtons[m_selectedCategoryIndex] );
  }
}