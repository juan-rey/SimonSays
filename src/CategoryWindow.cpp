#include "CategoryWindow.h"

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")


// Get taskbar color (dark/light theme)
COLORREF GetTaskbarColor()
{
  HKEY hKey;
  DWORD value = 0;
  DWORD size = sizeof( DWORD );

  // Check if dark mode is enabled
  if( RegOpenKeyExW( HKEY_CURRENT_USER,
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
    0, KEY_READ, &hKey ) == ERROR_SUCCESS )
  {

    RegQueryValueExW( hKey, L"SystemUsesLightTheme", NULL, NULL,
      (LPBYTE) &value, &size );
    RegCloseKey( hKey );
  }

  // If dark mode is enabled (value == 0), use dark color
  if( value == 0 )
  {
    return RGB( 32, 32, 32 ); // Typical Windows 11 dark mode color
  }
  else
  {
    return RGB( 243, 243, 243 ); // Typical Windows 11 light mode color
  }
}

CategoryWindow::CategoryWindow( MainWindow * mainWindow )
  : m_hwnd( NULL ), m_mainWindow( mainWindow ), m_selectedCategoryIndex( -1 )
{
}

CategoryWindow::~CategoryWindow()
{
  if( m_hwnd )
  {
    DestroyWindow( m_hwnd );
  }
}

bool CategoryWindow::Create( HINSTANCE hInstance )
{
  RECT rc;
  int width = 800;
  int height = 478;
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
  width = rc.right - rc.left + 14;
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

void CategoryWindow::Show()
{
  if( m_hwnd )
  {
    ShowWindow( m_hwnd, SW_SHOW );
    UpdateWindow( m_hwnd );
    SetForegroundWindow( m_hwnd );
  }
}

void CategoryWindow::Hide()
{
  if( m_hwnd )
  {
    ShowWindow( m_hwnd, SW_HIDE );
  }
}

void CategoryWindow::UpdateCategories( const std::vector<Category> & categories )
{
  m_categories = categories;
  CreateCategoryButtons();
  RefreshLayout();
  OnCategorySelected( 0 );
}

void CategoryWindow::RefreshLayout()
{
  if( !m_hwnd ) return;

  RECT rect;
  GetClientRect( m_hwnd, &rect );

  int categoriesPerRow = ( rect.right - m_button_margin ) / ( m_button_width + m_button_margin );
  if( categoriesPerRow < 1 ) categoriesPerRow = 1;
  int freeInnerCategoriesMargin = ( categoriesPerRow < 2 ) ? 0 : ( rect.right - ( categoriesPerRow * ( m_button_width + m_button_margin ) ) - m_button_margin ) / ( categoriesPerRow - 1 );

  for( int i = 0; i < m_categoryButtons.size(); i++ )
  {
    int row = i / categoriesPerRow;
    int col = i % categoriesPerRow;

    int x = m_button_margin + col * ( m_button_width + m_button_margin + freeInnerCategoriesMargin );
    int y = m_button_margin + row * ( m_button_height + m_button_margin );

    SetWindowPos( m_categoryButtons[i], NULL, x, y, m_button_width, m_button_height,
      SWP_NOZORDER | SWP_NOACTIVATE );
  }

  if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categories.size() )
  {
    int phraseStartY = m_button_margin + int( ( m_categories.size() / categoriesPerRow ) + 1 ) * ( m_button_height + m_button_margin );

    int phrasesPerRow = ( rect.right - m_button_margin ) / ( m_button_width + m_button_margin );
    if( phrasesPerRow < 1 ) phrasesPerRow = 1;
    int freeInnerPhrasesMargin = ( phrasesPerRow < 2 ) ? 0 : ( rect.right - ( phrasesPerRow * ( m_button_width + m_button_margin ) ) - m_button_margin ) / ( phrasesPerRow - 1 );

    for( int i = 0; i < m_phraseButtons.size(); i++ )
    {
      int row = i / phrasesPerRow;
      int col = i % phrasesPerRow;

      int x = m_button_margin + col * ( m_button_width + m_button_margin + freeInnerPhrasesMargin );
      int y = phraseStartY + m_button_margin + row * ( m_button_height + m_button_margin );

      SetWindowPos( m_phraseButtons[i], NULL, x, y, m_button_width, m_button_height,
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
          pThis->m_button_margin * 2 + pThis->m_button_width,
          pThis->m_button_margin * 2 + pThis->m_button_height };
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

  NONCLIENTMETRICS ncm = {};
  SystemParametersInfo( SPI_GETNONCLIENTMETRICS, sizeof( NONCLIENTMETRICS ), &ncm, 0 );
  HFONT message_font = CreateFontIndirect( &ncm.lfMessageFont );

  for( size_t i = 0; i < m_categories.size(); i++ )
  {
    HWND hButton = CreateWindow(
      L"BUTTON",
      m_categories[i].name.c_str(),
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_MULTILINE,
      0, 0, m_button_width, m_button_height,
      m_hwnd,
      (HMENU) ( 1000 + i ),
      m_hInstance,
      NULL
    );

    if( hButton )
    {
      m_categoryButtons.push_back( hButton );
      SendMessage( hButton, WM_SETFONT, (WPARAM) message_font, TRUE );
    }
  }
}

void CategoryWindow::CreatePhraseButtons( const Category & category )
{
  ClearPhraseButtons();

  NONCLIENTMETRICS ncm = {};
  SystemParametersInfo( SPI_GETNONCLIENTMETRICS, sizeof( NONCLIENTMETRICS ), &ncm, 0 );
  HFONT message_font = CreateFontIndirect( &ncm.lfMessageFont );

  for( size_t i = 0; i < category.phrases.size(); i++ )
  {
    HWND hButton = CreateWindow(
      L"BUTTON",
      category.phrases[i].audioFile.empty() ? ( category.phrases[i].text.c_str() ) : ( SOUND_NOTE_DELIMITER + category.phrases[i].text + SOUND_NOTE_DELIMITER ).c_str(),
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_MULTILINE,
      0, 0, m_button_width, m_button_height,
      m_hwnd,
      (HMENU) ( 2000 + i ),
      m_hInstance,
      NULL
    );

    if( hButton )
    {
      m_phraseButtons.push_back( hButton );
      SendMessage( hButton, WM_SETFONT, (WPARAM) message_font, TRUE );
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
    m_selectedCategoryIndex = categoryIndex;
    CreatePhraseButtons( m_categories[categoryIndex] );
    RefreshLayout();
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
        m_mainWindow->PlayCurrentText();
      }
    }

    SetFocus( m_categoryButtons[m_selectedCategoryIndex] );
  }
}