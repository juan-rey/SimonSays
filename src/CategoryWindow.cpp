#include "CategoryWindow.h"

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

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
  int width = 800;
  int height = 600;
  RECT rc;
  m_hInstance = hInstance;

  WNDCLASS wc = {};
  wc.lpfnWndProc = CategoryWindow::WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CATEGORY_WINDOW_CLASS;
  wc.hbrBackground = (HBRUSH) ( COLOR_BTNFACE + 1 );
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );

  if( !RegisterClass( &wc ) )
  {
    return false;
  }

  GetWindowRect( m_mainWindow->m_hwnd, &rc );
  int x = 0;
  int y = ( rc.top - height ) - 4;
  m_hwnd = CreateWindowEx(
    WS_EX_LAYERED,
    CATEGORY_WINDOW_CLASS,
    L"SimonSays - Categories",
    WS_POPUP | WS_VISIBLE,
    //WS_OVERLAPPEDWINDOW,
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
  DwmSetWindowAttribute( m_hwnd, DWMWA_WINDOW_CORNER_PREFERENCE,
    &preference, sizeof( preference ) );

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
}

void CategoryWindow::RefreshLayout()
{
  if( !m_hwnd ) return;

  RECT rect;
  GetClientRect( m_hwnd, &rect );

  int categoriesPerRow = ( rect.right - BUTTON_MARGIN ) / ( BUTTON_WIDTH + BUTTON_MARGIN );
  if( categoriesPerRow < 1 ) categoriesPerRow = 1;

  for( size_t i = 0; i < m_categoryButtons.size(); i++ )
  {
    int row = i / categoriesPerRow;
    int col = i % categoriesPerRow;

    int x = BUTTON_MARGIN + col * ( BUTTON_WIDTH + BUTTON_MARGIN );
    int y = BUTTON_MARGIN + row * ( BUTTON_HEIGHT + BUTTON_MARGIN );

    SetWindowPos( m_categoryButtons[i], NULL, x, y, BUTTON_WIDTH, BUTTON_HEIGHT,
      SWP_NOZORDER | SWP_NOACTIVATE );
  }

  if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categories.size() )
  {
    int phraseStartY = CATEGORY_SECTION_HEIGHT;

    int phrasesPerRow = ( rect.right - BUTTON_MARGIN ) / ( BUTTON_WIDTH + BUTTON_MARGIN );
    if( phrasesPerRow < 1 ) phrasesPerRow = 1;

    for( size_t i = 0; i < m_phraseButtons.size(); i++ )
    {
      int row = i / phrasesPerRow;
      int col = i % phrasesPerRow;

      int x = BUTTON_MARGIN + col * ( BUTTON_WIDTH + BUTTON_MARGIN );
      int y = phraseStartY + BUTTON_MARGIN + row * ( BUTTON_HEIGHT + BUTTON_MARGIN );

      SetWindowPos( m_phraseButtons[i], NULL, x, y, BUTTON_WIDTH, BUTTON_HEIGHT,
        SWP_NOZORDER | SWP_NOACTIVATE );
    }
  }
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

  for( size_t i = 0; i < m_categories.size(); i++ )
  {
    HWND hButton = CreateWindow(
      L"BUTTON",
      m_categories[i].name.c_str(),
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
      0, 0, BUTTON_WIDTH, BUTTON_HEIGHT,
      m_hwnd,
      (HMENU) ( 1000 + i ),
      m_hInstance,
      NULL
    );

    if( hButton )
    {
      m_categoryButtons.push_back( hButton );
      SendMessage( hButton, WM_SETFONT, (WPARAM) GetStockObject( DEFAULT_GUI_FONT ), TRUE );
    }
  }
}

void CategoryWindow::CreatePhraseButtons( const Category & category )
{
  ClearPhraseButtons();

  for( size_t i = 0; i < category.phrases.size(); i++ )
  {
    HWND hButton = CreateWindow(
      L"BUTTON",
      category.phrases[i].text.c_str(),
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
      0, 0, BUTTON_WIDTH, BUTTON_HEIGHT,
      m_hwnd,
      (HMENU) ( 2000 + i ),
      m_hInstance,
      NULL
    );

    if( hButton )
    {
      m_phraseButtons.push_back( hButton );
      SendMessage( hButton, WM_SETFONT, (WPARAM) GetStockObject( DEFAULT_GUI_FONT ), TRUE );
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
        HWND hEdit = m_mainWindow->m_hEditControl;
        if( hEdit )
        {
          SetWindowText( hEdit, selectedPhrase.text.c_str() );
        }
        m_mainWindow->PlayCurrentText();
      }
    }
  }
}