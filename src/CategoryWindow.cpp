/*
   CategoryWindow.cpp: CategoryWindow class source file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "CategoryWindow.h"
#include "utils.h"
#include "resource.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

struct EditDialogContext
{
  std::wstring * text;
  std::wstring language;
  bool categorySelectedLast;
  bool add;
};

// NORMAL_BUTTON_STYLE / FLAT_BUTTON_STYLE live in CategoryWindow.h (needed for
// member defaults); styled boards OR the BS_* alignment flags on top of them.
#define MAX_ZOOM_FACTOR 2.0f
#define MIN_ZOOM_FACTOR 0.5f
#define ZOOM_STEP 0.1f
#define MAX_SAVED_ZOOM_FACTOR 1.5f
#define MIN_SAVED_ZOOM_FACTOR 0.8f

// Applies an icon string to a button. Heuristic: a dot ⇒ file path (only .ico
// is rendered as an icon, anything else clears the icon); no dot ⇒ emoji.
// When `icon` is empty and `defaultIfEmpty` is true, falls back to whatever
// is already configured in `config`.
// File-scope helper (was previously a misleading `auto = [&]` lambda — the
// capture was unused because nothing is in scope at file level).
static void SetSSButtonIcon( SSButton & button, const std::wstring & icon,
  const SSButtonConfig & config, const  std::vector<std::wstring> & icoFileFolders, bool useFallback = true )
{
  if( !icon.empty() )
  {
    if( icon.find( L'.' ) != std::wstring::npos )
    {
      if( icon.find( L".ico" ) != std::wstring::npos )
      {
        std::wstring fullPath = icon;

        if( !FileExists( fullPath ) )
        {
          for( const auto & folder : icoFileFolders )
          {
            fullPath = folder + L"\\" + icon;
            if( FileExists( fullPath ) )
            {
              break;
            }
            else
            {
              fullPath.clear();
            }
          }
        }

        if( fullPath.empty() )
        {
          button.NoIcon();
        }
        else
        {
          button.SetIcon( fullPath, config.iconSize );
        }
      }
      else
        button.NoIcon();
    }
    else
    {
      button.SetEmoji( icon, config.iconSize );
    }
    return;
  }

  if( !useFallback ) return;


  switch( config.iconType )
  {
    case SSButtonIconType::None:         button.NoIcon();                                       break;
    case SSButtonIconType::Emoji:        button.SetEmoji( config.emoji, config.iconSize );      break;
    case SSButtonIconType::StandardIcon:
    {
      if( config.iconFileFullPath.find( L".ico" ) != std::wstring::npos )
      {
        std::wstring fullPath = config.iconFileFullPath;

        if( !FileExists( fullPath ) )
        {
          for( const auto & folder : icoFileFolders )
          {
            fullPath = folder + L"\\" + config.iconFileFullPath;
            if( FileExists( fullPath ) )
            {
              break;
            }
            else
            {
              fullPath.clear();
            }
          }
        }

        if( fullPath.empty() )
        {
          button.NoIcon();
        }
        else
        {
          button.SetIcon( fullPath, config.iconSize );
        }
      }
      else
        button.NoIcon();
    }
    break;
  }
}

// ---------------------------------------------------------------------------
// Board & category style application (docs/specs/board-style.spec.md §6.5)
// ---------------------------------------------------------------------------

// Maps resolved style properties onto an SSButtonConfig. Colors switch the
// config to custom mode; when a background is styled with no text color
// anywhere in the cascade, a contrasting text color is derived (STY-F41).
static SSButtonConfig MakeButtonConfig( const StyleProps & props )
{
  SSButtonConfig cfg; // safe defaults; SSButtonConfig's own values are the % bases

  if( props.hasBackground )
  {
    cfg.bgType = SSButtonBackground::Color;
    cfg.bgColor = props.background;
  }
  if( props.hasTextColor )
  {
    cfg.textColorType = SSButtonTextColor::Custom;
    cfg.textColor = props.textColor;
  }
  else if( props.hasBackground )
  {
    cfg.textColorType = SSButtonTextColor::Custom;
    cfg.textColor = ContrastTextColorFor( props.background );
  }

  if( props.cornerRadius.set )
  {
    int radius = ResolveStyleSize( props.cornerRadius, cfg.cornerRadius, /*allowZero=*/true );
    cfg.borderStyle = ( radius > 0 ) ? SSButtonBorderStyle::Rounded : SSButtonBorderStyle::Square;
    if( radius > 0 ) cfg.cornerRadius = radius;
  }
  if( props.borderWidth.set )
    cfg.borderWidth = ResolveStyleSize( props.borderWidth, cfg.borderWidth, /*allowZero=*/true ); // SSButton clamps [0,2]
  if( props.iconSize.set )
    cfg.iconSize = ResolveStyleSize( props.iconSize, SSBUTTON_ICON_DEFAULT_SIZE, /*allowZero=*/true ); // 0 == auto

  switch( props.iconPosition )
  {
    case StyleIconPos::Left:   cfg.iconPosition = SSButtonIconPosition::Left;   break;
    case StyleIconPos::Right:  cfg.iconPosition = SSButtonIconPosition::Right;  break;
    case StyleIconPos::Top:    cfg.iconPosition = SSButtonIconPosition::Top;    break;
    case StyleIconPos::Bottom: cfg.iconPosition = SSButtonIconPosition::Bottom; break;
    case StyleIconPos::Center: cfg.iconPosition = SSButtonIconPosition::Center; break;
    default: break; // NotSet: keep default
  }

  return cfg;
}

// text-layout keywords -> BS_* alignment flags (STY-F22).
static DWORD AlignmentStyleFlags( const StyleProps & props )
{
  DWORD flags = 0;
  switch( props.textHAlign )
  {
    case StyleHAlign::Left:   flags |= BS_LEFT;   break;
    case StyleHAlign::Center: flags |= BS_CENTER; break;
    case StyleHAlign::Right:  flags |= BS_RIGHT;  break;
    default: break;
  }
  switch( props.textVAlign )
  {
    case StyleVAlign::Top:    flags |= BS_TOP;     break;
    case StyleVAlign::Middle: flags |= BS_VCENTER; break;
    case StyleVAlign::Bottom: flags |= BS_BOTTOM;  break;
    default: break;
  }
  return flags;
}

CategoryWindow::CategoryWindow( MainWindow * mainWindow, bool savedWindowSize, bool minimizeWhenLosingFocus )
  : m_hwnd( NULL ), m_hVerticalSeparatorL( NULL ), m_mainWindow( mainWindow ), m_rememberWindowSize( savedWindowSize ), m_minimizeWhenLosingFocus( minimizeWhenLosingFocus )
{
  m_icoFileFolders.push_back( GetAppDataCustomFolder( APP_NAME ) );
  if( GetWorkingDirectory() != GetExecutableDirectory() ) // avoid duplicates if both are the same
    m_icoFileFolders.push_back( GetWorkingDirectory() );
  m_icoFileFolders.push_back( GetExecutableDirectory() );
}

CategoryWindow::~CategoryWindow()
{
  RegistryManager::SaveSelectedCategoryToRegistry( m_selectedCategoryIndex );

  if( m_rememberWindowSize )
  {
    RegistryManager::SaveZoomFactorToRegistry( m_zoom_factor );
    if( m_hwnd )
    {
      RECT rc;
      GetWindowRect( m_hwnd, &rc );
      RegistryManager::SaveCategoryWindowSizeToRegistry( rc.right - rc.left, rc.bottom - rc.top );
    }
  }

  if( m_backgroundBrush )
  {
    DeleteObject( m_backgroundBrush );
    m_backgroundBrush = NULL;
  }

  if( m_separatorBrush )
  {
    DeleteObject( m_separatorBrush );
    m_separatorBrush = NULL;
  }

  if( m_hDisplayTextFont )
  {
    DeleteObject( m_hDisplayTextFont );
    m_hDisplayTextFont = NULL;
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

  // Cache the taskbar color once — used for class brush, instance brush, and text color.
  const COLORREF taskbarColor = GetTaskbarColor();
  m_textColor = ( GetRValue( taskbarColor ) < 128 ) ? RGB( 255, 255, 255 ) : RGB( 0, 0, 0 );

  // Class-bound brush: GDI gives ownership to the class on RegisterClass, but
  // on subsequent calls RegisterClass returns 0 (ERROR_CLASS_ALREADY_EXISTS)
  // and the new brush would leak. Skip recreating it if the class already exists.
  static bool s_classRegistered = false;
  if( !s_classRegistered )
  {
    WNDCLASS wc = {};
    wc.lpfnWndProc = CategoryWindow::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CATEGORY_WINDOW_CLASS;
    wc.hbrBackground = CreateSolidBrush( taskbarColor ); // owned by class for the process lifetime
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    if( !RegisterClass( &wc ) )
    {
      if( GetLastError() != ERROR_CLASS_ALREADY_EXISTS )
      {
        DeleteObject( wc.hbrBackground );
        return false;
      }
      DeleteObject( wc.hbrBackground ); // class is already registered with its own brush
    }
    s_classRegistered = true;
  }

  GetWindowRect( m_mainWindow->GetHwnd(), &rc );
  if( !m_rememberWindowSize || !RegistryManager::LoadCategoryWindowSizeFromRegistry( width, height ) )
  {
    width = rc.right - rc.left + 14;
    height = m_default_window_height;
  }

  if( m_rememberWindowSize )
  {
    float temp_zoom_factor = RegistryManager::GetZoomFactorFromRegistry();
    if( temp_zoom_factor < MIN_ZOOM_FACTOR || temp_zoom_factor > MAX_ZOOM_FACTOR )
      temp_zoom_factor = 1.0f;
    m_zoom_factor = max( MIN_SAVED_ZOOM_FACTOR, min( MAX_SAVED_ZOOM_FACTOR, temp_zoom_factor ) );
  }

  int x = rc.left - 4;
  int y = ( rc.top - height );
  DWORD style = WS_POPUP | WS_THICKFRAME;
  m_hwnd = CreateWindowEx(
    WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
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

  m_backgroundBrush = CreateSolidBrush( taskbarColor );

  DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
  DwmSetWindowAttribute( m_hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof( preference ) );

  BOOL useDarkMode = ( GetRValue( taskbarColor ) > 128 ) ? FALSE : TRUE;
  DwmSetWindowAttribute( m_hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof( useDarkMode ) );
  DwmSetWindowAttribute( m_hwnd, DWMWA_CAPTION_COLOR, &taskbarColor, sizeof( taskbarColor ) );

  SetLayeredWindowAttributes( m_hwnd, 0, 239, LWA_ALPHA );

  if( !m_hSelectedCategoryButtonFont )
    m_hSelectedCategoryButtonFont = CreateStyledFont( m_boardStyle.categoryButtons, true );
  SendMessage( m_hwnd, WM_SETFONT, (WPARAM) m_hSelectedCategoryButtonFont, TRUE );

  // NOTE: deliberately do NOT ShowWindow here — caller (MainWindow) calls
  // UpdateCategories() afterward, which populates the buttons. Showing here
  // produced a brief flash of an empty window. Show() handles it instead.
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
    //UpdateWindow( m_hwnd );
    //SetForegroundWindow( m_hwnd );

    if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categoryButtons.size() )
    {
      m_categoryButtons[m_selectedCategoryIndex].SetFocus();
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

void CategoryWindow::UpdateCategories( const std::vector<Category> & categories, std::wstring language, int selectedCategory, const std::wstring & boardStyle )
{
  if( selectedCategory < 0 || selectedCategory >= (int) categories.size() )
  {
    selectedCategory = 0;
  }
  m_language = language;
  m_boardStyleRaw = boardStyle;
  if( m_zoom_factor != 1.0f && m_categories.size() > 0 ) // it is not the first time here and zoom may be unpleasant, so reset the zoom factor to 1.0f
  {
    m_zoom_factor = 1.0f;
    SafeTextResize();
  }
  m_categories = categories;
  m_rtlLayout = IsLanguageRTL( m_language );
  ApplyBoardStyle(); // parse the board style and refresh colors/metrics/fonts before (re)creating buttons
  m_display_text_size = GetTextDimensions( m_hDisplayText ? m_hDisplayText : m_hwnd, EffectiveDisplayText().c_str() );
  CreateCategoryButtons(); // creates/refreshes m_hDisplayText with the effective caption text
  OnCategorySelected( selectedCategory );
  ShowWindow( m_hwnd, SW_SHOW );
  UpdateButtonIcons();
}

void CategoryWindow::LayoutCalcs()
{
  if( !m_hwnd ) return;

  // Placeholder for layout calculations
  RECT rect;
  GetClientRect( m_hwnd, &rect );

  m_categories_per_row = ( rect.right - real_category_button_margin() ) / ( real_category_button_width() + real_category_button_margin() );
  if( m_categories_per_row < 1 ) m_categories_per_row = 1;
  m_free_inner_category_buttons_margin = ( m_categories_per_row < 2 ) ? 0 : ( rect.right - ( m_categories_per_row * ( real_category_button_width() + real_category_button_margin() ) ) - real_category_button_margin() ) / ( m_categories_per_row - 1 );

  //m_display_text_size = GetTextDimensions( m_hDisplayText ? m_hDisplayText : m_hwnd, GetLocalizedString( CATEGORY_SHORTCUTS_TEXT_ID, m_language ) );
  m_vertical_separator_width = ( rect.right - 4 * real_category_button_margin() - m_display_text_size.cx ) / 2;
  m_vertical_separator_y = real_category_button_margin() + ( CEILING_DIV( m_categories.size(), m_categories_per_row ) * ( real_category_button_height() + real_category_button_margin() ) );
  m_display_text_x = real_category_button_margin() * 2 + m_vertical_separator_width;
  m_display_text_y = m_vertical_separator_y - ( m_display_text_size.cy / 2 );

  m_phrase_buttons_start_y = ( 2 * real_category_button_margin() ) + 2 + ( CEILING_DIV( m_categories.size(), m_categories_per_row ) * ( real_category_button_height() + real_category_button_margin() ) );
  m_phrases_per_row = ( rect.right - real_phrase_button_margin() ) / ( real_phrase_button_width() + real_phrase_button_margin() );
  if( m_phrases_per_row < 1 ) m_phrases_per_row = 1;
  m_free_inner_phrase_buttons_margin = ( m_phrases_per_row < 2 ) ? 0 : ( rect.right - ( m_phrases_per_row * ( real_phrase_button_width() + real_phrase_button_margin() ) ) - real_phrase_button_margin() ) / ( m_phrases_per_row - 1 );

}

StyleProps CategoryWindow::PhrasePropsForCategory( int categoryIndex ) const
{
  StyleProps props = m_boardStyle.phraseButtons;

  if( categoryIndex >= 0 && categoryIndex < (int) m_categories.size() && !m_categories[categoryIndex].style.empty() )
  {
    CategoryStyle categoryStyle;
    ParseCategoryStyleList( m_categories[categoryIndex].style, categoryStyle );

    // STY-F12: the category's own-button colors flow to its phrase buttons
    // before the category's explicit phrase-* overrides are applied.
    StyleProps colorFlow;
    colorFlow.background = categoryStyle.ownButton.background;
    colorFlow.hasBackground = categoryStyle.ownButton.hasBackground;
    colorFlow.textColor = categoryStyle.ownButton.textColor;
    colorFlow.hasTextColor = categoryStyle.ownButton.hasTextColor;
    props = ResolveEffectiveStyle( props, colorFlow );
    props = ResolveEffectiveStyle( props, categoryStyle.phraseButtons );
  }

  return props;
}

SSButtonConfig CategoryWindow::CategoryButtonConfigFor( size_t categoryIndex ) const
{
  StyleProps props = m_boardStyle.categoryButtons;

  if( categoryIndex < m_categories.size() && !m_categories[categoryIndex].style.empty() )
  {
    CategoryStyle categoryStyle;
    ParseCategoryStyleList( m_categories[categoryIndex].style, categoryStyle );
    props = ResolveEffectiveStyle( props, categoryStyle.ownButton ); // colors only (STY-F30)
  }

  return MakeButtonConfig( props );
}

HFONT CategoryWindow::CreateStyledFont( const StyleProps & props, bool bold ) const
{
  NONCLIENTMETRICS ncm = {};
  ncm.cbSize = sizeof( NONCLIENTMETRICS );
  SystemParametersInfo( SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0 );
  LOGFONT lf = ncm.lfMessageFont;

  if( props.fontSize.set )
  {
    int defaultPx = ( lf.lfHeight < 0 ) ? -lf.lfHeight : lf.lfHeight;
    int px = props.fontSize.isPercent ? MulDiv( defaultPx, props.fontSize.value, 100 ) : props.fontSize.value;
    if( px > 0 ) lf.lfHeight = -px; // absolute font-size is pixels (STY-F45)
  }
  if( !props.fontName.empty() )
    wcsncpy_s( lf.lfFaceName, props.fontName.c_str(), _TRUNCATE );

  // text-weight overrides the message-font weight; a bold context (the selected
  // category marker) never drops below bold so selection stays visible (STY-F55).
  LONG weight = ( props.textWeight != 0 ) ? props.textWeight : lf.lfWeight;
  if( bold )
    weight = max( weight, FW_BOLD );
  lf.lfWeight = weight;

  lf.lfHeight = (LONG) ( lf.lfHeight * m_zoom_factor );
  lf.lfWidth = (LONG) ( lf.lfWidth * m_zoom_factor );

  return CreateFontIndirect( &lf );
}

std::wstring CategoryWindow::EffectiveDisplayText() const
{
  if( !m_boardStyle.window.caption.empty() )
    return m_boardStyle.window.caption;
  return GetLocalizedString( CATEGORY_SHORTCUTS_TEXT_ID, m_language );
}

void CategoryWindow::UpdatePhraseMetricsForCategory( int categoryIndex )
{
  StyleProps props = PhrasePropsForCategory( categoryIndex );

  m_phrase_button_width = ResolveStyleSize( props.width, PHRASE_BUTTON_WIDTH );
  m_phrase_button_height = ResolveStyleSize( props.height, PHRASE_BUTTON_HEIGHT );
  m_phrase_button_margin = ResolveStyleSize( props.margin, PHRASE_BUTTON_MARGIN, /*allowZero=*/true );
  m_phraseButtonConfig = MakeButtonConfig( props );
  m_phraseButtonStyle = NORMAL_BUTTON_STYLE | AlignmentStyleFlags( props );

  HFONT newPhraseFont = CreateStyledFont( props, false );
  for( auto & button : m_phraseButtons )
    button.SetFont( newPhraseFont, false );
  if( m_hPhraseButtonFont )
    DeleteObject( m_hPhraseButtonFont );
  m_hPhraseButtonFont = newPhraseFont;
}

void CategoryWindow::RebuildFonts()
{
  HFONT newCategoryFont = CreateStyledFont( m_boardStyle.categoryButtons, false );
  HFONT newSelectedFont = CreateStyledFont( m_boardStyle.categoryButtons, true );
  HFONT newDisplayFont = CreateStyledFont( m_boardStyle.window, true );

  size_t count = min( m_categoryButtons.size(), m_categories.size() );
  for( size_t i = 0; i < count; ++i )
    m_categoryButtons[i].SetFont( ( m_selectedCategoryIndex == (int) i ) ? newSelectedFont : newCategoryFont, false );

  if( m_hDisplayText )
    SendMessage( m_hDisplayText, WM_SETFONT, (WPARAM) newDisplayFont, TRUE );
  if( m_hwnd )
    SendMessage( m_hwnd, WM_SETFONT, (WPARAM) newDisplayFont, TRUE );
  m_display_text_size = GetTextDimensions( m_hDisplayText ? m_hDisplayText : m_hwnd, EffectiveDisplayText().c_str() );

  if( m_hCategoryButtonFont ) DeleteObject( m_hCategoryButtonFont );
  m_hCategoryButtonFont = newCategoryFont;
  if( m_hSelectedCategoryButtonFont ) DeleteObject( m_hSelectedCategoryButtonFont );
  m_hSelectedCategoryButtonFont = newSelectedFont;
  if( m_hDisplayTextFont ) DeleteObject( m_hDisplayTextFont );
  m_hDisplayTextFont = newDisplayFont;

  UpdatePhraseMetricsForCategory( m_selectedCategoryIndex ); // phrase font + metrics follow the selected category
}

void CategoryWindow::UpdateSeparatorStyles()
{
  // Styled separators are plain statics filled via WM_CTLCOLORSTATIC with the
  // separator brush; unstyled ones keep the etched system look (STY-F42).
  const DWORD style = WS_CHILD | WS_VISIBLE | ( m_separatorBrush ? 0 : SS_ETCHEDHORZ );
  if( m_hVerticalSeparatorL )
  {
    SetWindowLongPtr( m_hVerticalSeparatorL, GWL_STYLE, (LONG_PTR) style );
    InvalidateRect( m_hVerticalSeparatorL, NULL, TRUE );
  }
  if( m_hVerticalSeparatorR )
  {
    SetWindowLongPtr( m_hVerticalSeparatorR, GWL_STYLE, (LONG_PTR) style );
    InvalidateRect( m_hVerticalSeparatorR, NULL, TRUE );
  }
}

void CategoryWindow::ApplyBoardStyle()
{
  m_boardStyle = BoardStyle();
  ParseBoardStyleList( m_boardStyleRaw, m_boardStyle );

  // Window colors: styled background wins over the taskbar color (STY-F40/F41).
  const COLORREF taskbarColor = GetTaskbarColor();
  const bool styledBg = m_boardStyle.window.hasBackground;
  m_effectiveBgColor = styledBg ? m_boardStyle.window.background : taskbarColor;

  if( m_boardStyle.window.hasTextColor )
    m_textColor = m_boardStyle.window.textColor;
  else if( styledBg )
    m_textColor = ContrastTextColorFor( m_effectiveBgColor );
  else
    m_textColor = ( GetRValue( taskbarColor ) < 128 ) ? RGB( 255, 255, 255 ) : RGB( 0, 0, 0 );

  if( m_backgroundBrush )
    DeleteObject( m_backgroundBrush );
  m_backgroundBrush = CreateSolidBrush( m_effectiveBgColor );

  if( m_separatorBrush )
  {
    DeleteObject( m_separatorBrush );
    m_separatorBrush = NULL;
  }
  if( m_boardStyle.window.hasSeparatorColor )
    m_separatorBrush = CreateSolidBrush( m_boardStyle.window.separatorColor );
  UpdateSeparatorStyles();

  if( m_hwnd )
  {
    // Keep the original taskbar-based rule when unstyled; luminance otherwise.
    BOOL useDarkMode = styledBg
      ? ( ( ContrastTextColorFor( m_effectiveBgColor ) == RGB( 255, 255, 255 ) ) ? TRUE : FALSE )
      : ( ( GetRValue( taskbarColor ) > 128 ) ? FALSE : TRUE );
    DwmSetWindowAttribute( m_hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof( useDarkMode ) );
    DwmSetWindowAttribute( m_hwnd, DWMWA_CAPTION_COLOR, &m_effectiveBgColor, sizeof( m_effectiveBgColor ) );
  }

  // Category-button metrics/config/styles; zoom is applied later in real_*().
  m_category_button_width = ResolveStyleSize( m_boardStyle.categoryButtons.width, CATEGORY_BUTTON_WIDTH );
  m_category_button_height = ResolveStyleSize( m_boardStyle.categoryButtons.height, CATEGORY_BUTTON_HEIGHT );
  m_category_button_margin = ResolveStyleSize( m_boardStyle.categoryButtons.margin, CATEGORY_BUTTON_MARGIN, /*allowZero=*/true );
  m_categoryButtonConfig = MakeButtonConfig( m_boardStyle.categoryButtons );
  m_categoryButtonStyle = NORMAL_BUTTON_STYLE | AlignmentStyleFlags( m_boardStyle.categoryButtons );

  RebuildFonts(); // also refreshes phrase metrics/config/font for the selected category
}

void CategoryWindow::RefreshLayout()
{
  if( !m_hwnd ) return;

  LayoutCalcs();

  int row = 0;
  int col = 0;
  int x = 0;
  int y = 0;
  for( size_t i = 0; i < m_categoryButtons.size(); i++ )
  {
    row = i / m_categories_per_row;
    col = i % m_categories_per_row;

    if( m_rtlLayout )
    {
      col = ( m_categories_per_row - 1 ) - col;
    }

    x = real_category_button_margin() + col * ( real_category_button_width() + real_category_button_margin() + m_free_inner_category_buttons_margin );
    y = real_category_button_margin() + row * ( real_category_button_height() + real_category_button_margin() );

    m_categoryButtons[i].SetPos( x, y, real_category_button_width(), real_category_button_height() );
  }

  SetWindowPos( m_hVerticalSeparatorL, NULL,
    real_category_button_margin(),
    m_vertical_separator_y - 1,
    m_vertical_separator_width,
    2,
    SWP_NOZORDER | SWP_NOACTIVATE );

  SetWindowPos( m_hDisplayText, NULL,
    m_display_text_x,
    m_display_text_y,
    m_display_text_size.cx,
    m_display_text_size.cy,
    SWP_NOZORDER | SWP_NOACTIVATE );

  SetWindowPos( m_hVerticalSeparatorR, NULL,
    real_category_button_margin() * 3 + m_vertical_separator_width + m_display_text_size.cx,
    m_vertical_separator_y - 1,
    m_vertical_separator_width,
    2,
    SWP_NOZORDER | SWP_NOACTIVATE );

  if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categories.size() )
  {
    for( size_t i = 0; i < m_phraseButtons.size(); i++ )
    {
      row = i / m_phrases_per_row;
      col = i % m_phrases_per_row;

      if( m_rtlLayout )
      {
        col = ( m_phrases_per_row - 1 ) - col;
      }

      x = real_phrase_button_margin() + col * ( real_phrase_button_width() + real_phrase_button_margin() + m_free_inner_phrase_buttons_margin );
      y = m_phrase_buttons_start_y + row * ( real_phrase_button_height() + real_phrase_button_margin() );

      m_phraseButtons[i].SetPos( x, y, real_phrase_button_width(), real_phrase_button_height() );
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
            pThis->UpdatePhraseButtonIcons();
          }
          else if( wmId >= 2000 && wmId < 3000 )
          {
            pThis->OnPhraseSelected( wmId - 2000 );
          }
        }
      }
      break;

      case WM_SIZE: // Refresh layout on window resize
        pThis->RefreshLayout();
        break;

      case WM_KEYDOWN:
      {
        if( GetKeyState( VK_CONTROL ) & 0x8000 )
        {
          if( wParam == VK_OEM_PLUS || wParam == VK_ADD )
          {
            pThis->ZoomIn();
          }
          else if( wParam == VK_OEM_MINUS || wParam == VK_SUBTRACT )
          {
            pThis->ZoomOut();
          }
          else if( wParam == '0' )
          {
            // Ctrl + 0
            pThis->ResetZoom();
          }
          else if( wParam == VK_F8 )
          {
            pThis->DeleteAllCategories();
          }
          else if( wParam == VK_F9 )
          {
            pThis->DeleteAllCategories();
            pThis->ImportCategories();
          }
          break;
        }
        switch( wParam )
        {
          case VK_ESCAPE:
          {
            ShowWindow( hwnd, SW_HIDE );
            pThis->m_mainWindow->OnCategoryWindowHidden();
            return 0;
          }
          case VK_F4:
          {
            pThis->EditLastSelection();
            return 0;
          }
          case VK_F5:
          {
            pThis->MoveSelection( IsLanguageRTL( pThis->m_language ) ? 1 : -1 );
            return 0;
          }
          case VK_F6:
          {
            pThis->MoveSelection( IsLanguageRTL( pThis->m_language ) ? -1 : 1 );
            return 0;
          }
          case VK_F7:
          {
            pThis->AddAfterSelection();
            return 0;
          }
          case VK_F8:
          {
            pThis->DeleteLastSelection();
            return 0;
          }
          case VK_F9:
          {
            pThis->ImportCategories();
            return 0;
          }
          case VK_F10:
          {
            pThis->ExportCategories();
            return 0;
          }
          case VK_F11:
          {
            pThis->ZoomOut();
            return 0;
          }
          case VK_F12:
          {
            pThis->ZoomIn();
            return 0;
          }
        }
      }
      break;

      case WM_GETMINMAXINFO: // Set minimum size for the window
      {
        MINMAXINFO * mmi = reinterpret_cast<MINMAXINFO *>( lParam );
        RECT minRect = { 0, 0,
          pThis->real_category_button_margin() * 2 + pThis->real_category_button_width(),
          pThis->real_category_button_margin() * 2 + pThis->real_category_button_height() };
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

      case WM_CLOSE:
        ShowWindow( hwnd, SW_HIDE );
        pThis->m_mainWindow->OnCategoryWindowHidden();
        return 0;

      case WM_DESTROY:
        pThis->m_hwnd = NULL;
        break;

      case WM_ACTIVATE:
        if( LOWORD( wParam ) == WA_INACTIVE )
        {
          if( pThis->m_minimizeWhenLosingFocus )
          {
            HWND hOtherWnd = (HWND) lParam;

            if( hOtherWnd != NULL )
            {
              DWORD myProcessId = GetCurrentProcessId();
              DWORD otherProcessId = 0;

              GetWindowThreadProcessId( hOtherWnd, &otherProcessId );

              if( myProcessId != otherProcessId )
              {
                ShowWindow( hwnd, SW_HIDE );
                pThis->m_mainWindow->OnCategoryWindowHidden();
              }
            }
            else
            {
              ShowWindow( hwnd, SW_HIDE );
              pThis->m_mainWindow->OnCategoryWindowHidden();
            }
          }
        }
        break;

        // Paint the client background with the effective board color so styled
        // backgrounds work and SSButton rounded corners seed correctly (STY-F40).
      case WM_ERASEBKGND:
      {
        if( !pThis->m_backgroundBrush ) break; // pre-Create(): let the class brush handle it
        HDC hdc = (HDC) wParam;
        RECT rc;
        GetClientRect( hwnd, &rc );
        FillRect( hdc, &rc, pThis->m_backgroundBrush );
        return 1;
      }

      case WM_CTLCOLORSTATIC:
      {
        HWND hCtl = (HWND) lParam;
        if( pThis->m_separatorBrush &&
          ( hCtl == pThis->m_hVerticalSeparatorL || hCtl == pThis->m_hVerticalSeparatorR ) )
        {
          return (INT_PTR) pThis->m_separatorBrush; // styled separator bars (STY-F42)
        }
        HDC hdcStatic = (HDC) wParam;
        SetTextColor( hdcStatic, pThis->m_textColor );
        SetBkMode( hdcStatic, TRANSPARENT );
        return (INT_PTR) pThis->m_backgroundBrush;
      }

      // Theme / system color changed — re-apply the board style, which keeps a
      // styled background and falls back to the taskbar color otherwise.
      case WM_SYSCOLORCHANGE:
      case WM_THEMECHANGED:
      {
        pThis->ApplyBoardStyle();
        RedrawWindow( hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW );
        return 0;
      }

      default:
        return DefWindowProc( hwnd, uMsg, wParam, lParam );
    }
  }

  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

void CategoryWindow::CreateCategoryButtons()
{
  m_categoryButtons.clear();

  if( !m_hCategoryButtonFont )
    m_hCategoryButtonFont = CreateStyledFont( m_boardStyle.categoryButtons, false );

  if( !m_hSelectedCategoryButtonFont )
    m_hSelectedCategoryButtonFont = CreateStyledFont( m_boardStyle.categoryButtons, true );

  if( !m_hDisplayTextFont )
    m_hDisplayTextFont = CreateStyledFont( m_boardStyle.window, true );

  LayoutCalcs();

  int row = 0;
  int col = 0;
  int x = 0;
  int y = 0;

  for( size_t i = 0; i < m_categories.size(); i++ )
  {
    row = i / m_categories_per_row;
    col = i % m_categories_per_row;

    if( m_rtlLayout )
    {
      col = ( m_categories_per_row - 1 ) - col;
    }

    x = real_category_button_margin() + col * ( real_category_button_width() + real_category_button_margin() + m_free_inner_category_buttons_margin );
    y = real_category_button_margin() + row * ( real_category_button_height() + real_category_button_margin() );
    m_categoryButtons.emplace_back( SSButton() );
    m_categoryButtons.back().Create( m_hwnd, m_hInstance, 1000 + i,
      m_categories[i].name,
      x, y, real_category_button_width(), real_category_button_height(),
      m_categoryButtonStyle,
      m_rtlLayout ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0,
      CategoryButtonConfigFor( i ) );
    m_categoryButtons.back().SetFont( m_hCategoryButtonFont );
    // move icon setting afterward (UpdateButtonIcons) to avoid painting delay when creating the button
    // SetSSButtonIcon( m_categoryButtons.back(), m_categories[i].icon, m_buttonConfig, false );
  }


  if( !m_hDisplayText )
  {
    m_hDisplayText = CreateWindowEx(
      WS_EX_TRANSPARENT,
      L"STATIC",
      EffectiveDisplayText().c_str(),
      WS_CHILD | WS_VISIBLE | SS_CENTER,
      m_display_text_x,
      m_display_text_y,
      m_display_text_size.cx,
      m_display_text_size.cy,
      m_hwnd,
      NULL,
      m_hInstance,
      NULL
    );

    SendMessage( m_hDisplayText, WM_SETFONT, (WPARAM) m_hDisplayTextFont, TRUE );
  }
  else
  {
    // Refresh the strip text too: a board style change can swap the caption
    // (or revert to the shortcuts hint) without going through UpdateCategories.
    SetWindowText( m_hDisplayText, EffectiveDisplayText().c_str() );
    SetWindowPos( m_hDisplayText, NULL,
      m_display_text_x,
      m_display_text_y,
      m_display_text_size.cx,
      m_display_text_size.cy,
      SWP_NOZORDER | SWP_NOACTIVATE );
  }

  if( !m_hVerticalSeparatorL )
  {
    m_hVerticalSeparatorL = CreateWindowEx(
      0,
      L"STATIC",
      NULL,
      WS_CHILD | WS_VISIBLE | ( m_separatorBrush ? 0 : SS_ETCHEDHORZ ),
      real_category_button_margin(), m_vertical_separator_y - 1, m_vertical_separator_width, 2,
      m_hwnd,
      NULL,
      m_hInstance,
      NULL
    );
  }
  else
  {
    SetWindowPos( m_hVerticalSeparatorL, NULL,
      real_category_button_margin(),
      m_vertical_separator_y - 1,
      m_vertical_separator_width,
      2,
      SWP_NOZORDER | SWP_NOACTIVATE );
  }

  if( !m_hVerticalSeparatorR )
  {
    m_hVerticalSeparatorR = CreateWindowEx(
      0,
      L"STATIC",
      NULL,
      WS_CHILD | WS_VISIBLE | ( m_separatorBrush ? 0 : SS_ETCHEDHORZ ),
      real_category_button_margin() * 3 + m_vertical_separator_width + m_display_text_size.cx, m_vertical_separator_y - 1, m_vertical_separator_width, 2,
      m_hwnd,
      NULL,
      m_hInstance,
      NULL
    );
  }
  else
  {
    SetWindowPos( m_hVerticalSeparatorR, NULL,
      real_category_button_margin() * 3 + m_vertical_separator_width + m_display_text_size.cx,
      m_vertical_separator_y - 1,
      m_vertical_separator_width,
      2,
      SWP_NOZORDER | SWP_NOACTIVATE );
  }
}

void CategoryWindow::UpdatePhraseButtonIcons()
{
  RedrawWindow( m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW );
  if( m_selectedCategoryIndex < 0 || m_selectedCategoryIndex >= (int) m_categories.size() ) return;

  // Bound the iteration by both vectors — sizes can diverge briefly during
  // edit/move operations and indexing past the smaller would be OOB.
  const auto & phrases = m_categories[m_selectedCategoryIndex].phrases;
  const size_t count = min( m_phraseButtons.size(), phrases.size() );
  for( size_t i = 0; i < count; ++i )
    SetSSButtonIcon( m_phraseButtons[i], phrases[i].icon, m_phraseButtonConfig, m_icoFileFolders, false );
}

void CategoryWindow::UpdateButtonIcons()
{
  UpdatePhraseButtonIcons(); // already invalidates and refreshes phrase icons
  const size_t count = min( m_categoryButtons.size(), m_categories.size() );
  for( size_t i = 0; i < count; ++i )
    SetSSButtonIcon( m_categoryButtons[i], m_categories[i].icon, m_categoryButtonConfig, m_icoFileFolders, false );
}

void CategoryWindow::CreatePhraseButtons( const Category & category )
{
  m_phraseButtons.clear();

  if( !m_hPhraseButtonFont )
    m_hPhraseButtonFont = CreateStyledFont( PhrasePropsForCategory( m_selectedCategoryIndex ), false );

  int row = 0;
  int col = 0;
  int x = 0;
  int y = 0;
  for( size_t i = 0; i < category.phrases.size(); i++ )
  {
    row = i / m_phrases_per_row;
    col = i % m_phrases_per_row;

    if( m_rtlLayout )
    {
      col = ( m_phrases_per_row - 1 ) - col;
    }

    x = real_phrase_button_margin() + col * ( real_phrase_button_width() + real_phrase_button_margin() + m_free_inner_phrase_buttons_margin );
    y = m_phrase_buttons_start_y + row * ( real_phrase_button_height() + real_phrase_button_margin() );

    m_phraseButtons.emplace_back( SSButton() );
    m_phraseButtons.back().Create( m_hwnd, m_hInstance, 2000 + i,
      PhraseToButtonText( category.phrases[i] ).c_str(),
      x, y, real_phrase_button_width(), real_phrase_button_height(),
      m_phraseButtonStyle,
      m_rtlLayout ? ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) : 0,
      m_phraseButtonConfig );
    m_phraseButtons.back().SetFont( m_hPhraseButtonFont );
    // move icon setting afterward to avoid painting delay when creating buttons
    //SetSSButtonIcon( m_phraseButtons.back(), category.phrases[i].icon, m_buttonConfig, m_icoFileFolders, false );
  }
}

void CategoryWindow::OnCategorySelected( int categoryIndex )
{
  if( categoryIndex >= 0 && categoryIndex < (int) m_categories.size() )
  {
    if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categoryButtons.size() )
    {
      SSButton & prev = m_categoryButtons[m_selectedCategoryIndex];
      prev.SetFont( m_hCategoryButtonFont );
      prev.SetStyle( m_categoryButtonStyle );
    }

    m_selectedCategoryIndex = categoryIndex;
    m_categorySelectedLast = true;
    m_selectedPhraseIndex = -1;
    UpdatePhraseMetricsForCategory( categoryIndex ); // per-category phrase style/metrics (STY-F43)
    LayoutCalcs();
    CreatePhraseButtons( m_categories[categoryIndex] );
    SSButton & sel = m_categoryButtons[m_selectedCategoryIndex];
    sel.SetFont( m_hSelectedCategoryButtonFont );
    sel.SetStyle( m_categoryButtonStyle | BS_FLAT ); // selected marker: bold + flat (STY-F44)
    sel.SetFocus();
  }
}

bool CategoryWindow::ShowEditDialog( std::wstring & text, bool add )
{
  EditDialogContext ctx{ &text, m_language, m_categorySelectedLast, add };

  INT_PTR res = DialogBoxParam( m_hInstance, MAKEINTRESOURCE( IDD_EDIT_DIALOG ), m_hwnd, CategoryWindow::EditDialogProc, (LPARAM) &ctx );
  return ( res == IDOK );
}

INT_PTR CALLBACK CategoryWindow::EditDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
  switch( message )
  {
    case WM_INITDIALOG:
    {
      auto * ctx = reinterpret_cast<EditDialogContext *>( lParam );
      if( !ctx || !ctx->text ) return FALSE;
      SetWindowLongPtr( hDlg, GWLP_USERDATA, (LONG_PTR) ctx );
      SetDlgItemText( hDlg, IDC_EDIT_DIALOG_TEXT, ctx->text->c_str() );
      if( ctx->add )
      {
        SetWindowText( hDlg, GetLocalizedString( ( ctx->categorySelectedLast ? ADD_DIALOG_CATEGORY_TITLE_ID : ADD_DIALOG_PHRASE_TITLE_ID ), ctx->language ) );
        SetDlgItemText( hDlg, IDC_EDIT_DIALOG_LABEL_TEXT, GetLocalizedString( ( ctx->categorySelectedLast ? ADD_DIALOG_CATEGORY_TEXT_LABEL_ID : ADD_DIALOG_PHRASE_TEXT_LABEL_ID ), ctx->language ) );
        SetDlgItemText( hDlg, IDOK, GetLocalizedString( ADD_DIALOG_OK_BUTTON_ID, ctx->language ) );
        SetDlgItemText( hDlg, IDCANCEL, GetLocalizedString( ADD_DIALOG_CANCEL_BUTTON_ID, ctx->language ) );
      }
      else
      {
        SetWindowText( hDlg, GetLocalizedString( ( ctx->categorySelectedLast ? EDIT_DIALOG_CATEGORY_TITLE_ID : EDIT_DIALOG_PHRASE_TITLE_ID ), ctx->language ) );

        std::wstring labelText = std::wstring( GetLocalizedString( EDIT_DIALOG_TEXT_LABEL_ID, ctx->language ) ) + L"'" + *( ctx->text ) + L"'";
        SetDlgItemText( hDlg, IDC_EDIT_DIALOG_LABEL_TEXT, labelText.c_str() );
        SetDlgItemText( hDlg, IDOK, GetLocalizedString( EDIT_DIALOG_OK_BUTTON_ID, ctx->language ) );
        SetDlgItemText( hDlg, IDCANCEL, GetLocalizedString( EDIT_DIALOG_CANCEL_BUTTON_ID, ctx->language ) );
      }


      return TRUE;
    }
    case WM_COMMAND:
    {
      WORD id = LOWORD( wParam );
      if( id == IDOK )
      {
        auto * ctx = reinterpret_cast<EditDialogContext *>( GetWindowLongPtr( hDlg, GWLP_USERDATA ) );
        if( ctx && ctx->text )
        {
          wchar_t buffer[1024];
          GetDlgItemText( hDlg, IDC_EDIT_DIALOG_TEXT, buffer, ARRAYSIZE( buffer ) );
          *( ctx->text ) = buffer;
        }
        EndDialog( hDlg, IDOK );
        return TRUE;
      }
      else if( id == IDCANCEL )
      {
        EndDialog( hDlg, IDCANCEL );
        return TRUE;
      }
      break;
    }
  }
  return FALSE;
}

void CategoryWindow::EditLastSelection()
{
  if( !m_mainWindow ) return;

  if( m_selectedCategoryIndex < 0 || m_selectedCategoryIndex >= (int) m_categories.size() )
    return;

  bool previousValue = m_minimizeWhenLosingFocus;
  m_minimizeWhenLosingFocus = false; // Prevent the window from hiding when the edit dialog is opened

  if( m_categorySelectedLast )
  {
    std::wstring editable = SerializeCategoryWithStyle( m_categories[m_selectedCategoryIndex] );
    if( ShowEditDialog( editable ) )
    {
      Category tempCategory = DeserializeCategory( editable );
      // check if the category name was changed and does not conflict with existing category names
      bool validChange = ( ( tempCategory.name != m_categories[m_selectedCategoryIndex].name ) || ( tempCategory.icon != m_categories[m_selectedCategoryIndex].icon ) || ( tempCategory.style != m_categories[m_selectedCategoryIndex].style ) );
      // reserved "$$" names are not allowed as category names (board-style.spec.md STY-F23)
      if( tempCategory.name.compare( 0, STYLE_TOKEN_PREFIX_LENGTH, STYLE_TOKEN_PREFIX ) == 0 )
      {
        ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( CATEGORY_NAME_CONFLICT_MESSAGE_ID, m_language ), GetLocalizedString( CATEGORY_NAME_CONFLICT_TITLE_ID, m_language ), MB_OK | MB_ICONERROR, m_language );
        validChange = false;
      }
      for( size_t i = 0; i < m_categories.size(); i++ )
      {
        if( i != (size_t) m_selectedCategoryIndex && m_categories[i].name == tempCategory.name )
        {
          ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( CATEGORY_NAME_CONFLICT_MESSAGE_ID, m_language ), GetLocalizedString( CATEGORY_NAME_CONFLICT_TITLE_ID, m_language ), MB_OK | MB_ICONERROR, m_language );
          validChange = false;
        }
      }

      if( validChange )
      {
        m_categories[m_selectedCategoryIndex].name = tempCategory.name;
        m_categories[m_selectedCategoryIndex].icon = tempCategory.icon;
        m_categories[m_selectedCategoryIndex].style = tempCategory.style;
        if( m_selectedCategoryIndex < (int) m_categoryButtons.size() )
        {
          m_categoryButtons[m_selectedCategoryIndex].SetText( tempCategory.name );
          m_categoryButtons[m_selectedCategoryIndex].SetConfig( CategoryButtonConfigFor( m_selectedCategoryIndex ) );
          SetSSButtonIcon( m_categoryButtons[m_selectedCategoryIndex], tempCategory.icon, m_categoryButtonConfig, m_icoFileFolders );
        }
        // Re-select so the phrase buttons pick up a possibly-changed category style.
        OnCategorySelected( m_selectedCategoryIndex );
        UpdatePhraseButtonIcons();
        RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
      }
    }
  }
  else
  {
    if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categories.size() )
    {
      auto & category = m_categories[m_selectedCategoryIndex];
      if( m_selectedPhraseIndex >= 0 && m_selectedPhraseIndex < (int) category.phrases.size() )
      {
        auto & phrase = category.phrases[m_selectedPhraseIndex];
        std::wstring editable = SerializePhrase( phrase );
        if( ShowEditDialog( editable ) )
        {
          phrase = DeserializePhrase( editable );

          if( m_selectedPhraseIndex < (int) m_phraseButtons.size() )
          {
            m_phraseButtons[m_selectedPhraseIndex].SetText( PhraseToButtonText( phrase ) );
            SetSSButtonIcon( m_phraseButtons[m_selectedPhraseIndex], phrase.icon, m_phraseButtonConfig, m_icoFileFolders );
          }
          else
          {
            //RefreshLayout();
          }

          RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
          OnPhraseSelected( m_selectedPhraseIndex );
        }
      }
    }
  }

  m_minimizeWhenLosingFocus = previousValue;
}

void CategoryWindow::AddAfterSelection()
{
  bool previousValue = m_minimizeWhenLosingFocus;
  m_minimizeWhenLosingFocus = false;

  if( m_categorySelectedLast || m_selectedPhraseIndex == -1 )
  {
    // Add category after current selection (or at end if none)
    bool oldFlag = m_categorySelectedLast;
    m_categorySelectedLast = true;
    std::wstring editable;
    if( ShowEditDialog( editable, true ) && !editable.empty() )
    {
      Category newCat = DeserializeCategory( editable );

      // check if the category name does not conflict with existing category names
      bool validChange = true;
      // reserved "$$" names are not allowed as category names (board-style.spec.md STY-F23)
      if( newCat.name.compare( 0, STYLE_TOKEN_PREFIX_LENGTH, STYLE_TOKEN_PREFIX ) == 0 )
      {
        ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( CATEGORY_NAME_CONFLICT_MESSAGE_ID, m_language ), GetLocalizedString( CATEGORY_NAME_CONFLICT_TITLE_ID, m_language ), MB_OK | MB_ICONERROR, m_language );
        validChange = false;
      }
      for( size_t i = 0; i < m_categories.size(); i++ )
      {
        if( m_categories[i].name == newCat.name )
        {
          ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( CATEGORY_NAME_CONFLICT_MESSAGE_ID, m_language ), GetLocalizedString( CATEGORY_NAME_CONFLICT_TITLE_ID, m_language ), MB_OK | MB_ICONERROR, m_language );
          validChange = false;
        }
      }

      if( validChange )
      {
        size_t insertPos = ( m_selectedCategoryIndex >= 0 ) ? (size_t) m_selectedCategoryIndex + 1 : m_categories.size();
        Phrase newPhrase;
        newPhrase.text = GetLocalizedString( NEW_PHRASE_DEFAULT_TEXT_ID, m_language );
        m_categories.insert( m_categories.begin() + insertPos, newCat );
        m_categories[insertPos].phrases.push_back( newPhrase );

        CreateCategoryButtons();
        m_selectedCategoryIndex = (int) insertPos;
        m_selectedPhraseIndex = -1;
        m_categorySelectedLast = true;
        OnCategorySelected( m_selectedCategoryIndex );
        UpdateButtonIcons();
        RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
      }
    }
    m_categorySelectedLast = oldFlag;
  }
  else
  {
    // Add phrase after current phrase in the selected category
    if( m_selectedCategoryIndex < 0 || m_selectedCategoryIndex >= (int) m_categories.size() )
    {
      m_minimizeWhenLosingFocus = previousValue;
      return;
    }
    auto & category = m_categories[m_selectedCategoryIndex];

    bool oldFlag = m_categorySelectedLast;
    m_categorySelectedLast = false;
    std::wstring editable;
    if( ShowEditDialog( editable, true ) && !editable.empty() )
    {
      Phrase newPhrase = DeserializePhrase( editable );

      size_t insertPos = ( m_selectedPhraseIndex >= 0 ) ? (size_t) m_selectedPhraseIndex + 1 : category.phrases.size();
      category.phrases.insert( category.phrases.begin() + insertPos, newPhrase );
      m_selectedPhraseIndex = (int) insertPos;

      CreatePhraseButtons( category ); // already clears m_phraseButtons internally
      OnPhraseSelected( m_selectedPhraseIndex );
      UpdatePhraseButtonIcons();
      RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
    }
    m_categorySelectedLast = oldFlag;
  }

  m_minimizeWhenLosingFocus = previousValue;
}

void CategoryWindow::MoveSelection( int delta )
{
  if( delta == 0 ) return;
  if( m_selectedCategoryIndex < 0 || m_selectedCategoryIndex >= (int) m_categories.size() ) return;

  bool previousValue = m_minimizeWhenLosingFocus;
  m_minimizeWhenLosingFocus = false;

  if( m_categorySelectedLast )
  {
    int newIndex = m_selectedCategoryIndex + delta;
    if( newIndex >= 0 && newIndex < (int) m_categories.size() )
    {
      std::swap( m_categories[m_selectedCategoryIndex], m_categories[newIndex] );
      if( max( m_selectedCategoryIndex, newIndex ) < (int) m_categoryButtons.size() )
      {
        SSButton & prev = m_categoryButtons[m_selectedCategoryIndex];
        if( m_hCategoryButtonFont ) prev.SetFont( m_hCategoryButtonFont );
        prev.SetStyle( m_categoryButtonStyle, /*reframe=*/false );
        prev.SetText( m_categories[m_selectedCategoryIndex].name );
        prev.SetConfig( CategoryButtonConfigFor( m_selectedCategoryIndex ) ); // swapped: per-category colors follow the data
        SetSSButtonIcon( prev, m_categories[m_selectedCategoryIndex].icon, m_categoryButtonConfig, m_icoFileFolders );
        m_categoryButtons[newIndex].SetText( m_categories[newIndex].name );
        m_categoryButtons[newIndex].SetConfig( CategoryButtonConfigFor( newIndex ) );
        SetSSButtonIcon( m_categoryButtons[newIndex], m_categories[newIndex].icon, m_categoryButtonConfig, m_icoFileFolders );
        m_selectedCategoryIndex = newIndex;
        SSButton & sel = m_categoryButtons[m_selectedCategoryIndex];
        if( m_hSelectedCategoryButtonFont ) sel.SetFont( m_hSelectedCategoryButtonFont );
        sel.SetStyle( m_categoryButtonStyle | BS_FLAT );
        sel.SetFocus();
      }
      else
      {
        m_selectedCategoryIndex = newIndex;
        CreateCategoryButtons();
        OnCategorySelected( m_selectedCategoryIndex );
        UpdateButtonIcons();
      }
      RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
    }
  }
  else
  {
    auto & category = m_categories[m_selectedCategoryIndex];
    if( m_selectedPhraseIndex >= 0 && m_selectedPhraseIndex < (int) category.phrases.size() )
    {
      int newIndex = m_selectedPhraseIndex + delta;
      if( newIndex >= 0 && newIndex < (int) category.phrases.size() )
      {
        std::swap( category.phrases[m_selectedPhraseIndex], category.phrases[newIndex] );
        if( max( m_selectedPhraseIndex, newIndex ) < (int) m_phraseButtons.size() )
        {
          const auto & curPhrase = category.phrases[m_selectedPhraseIndex];
          m_phraseButtons[m_selectedPhraseIndex].SetText(
            curPhrase.audioFile.empty()
            ? curPhrase.text
            : ( SOUND_NOTE_DELIMITER + curPhrase.text + SOUND_NOTE_DELIMITER ) );
          SetSSButtonIcon( m_phraseButtons[m_selectedPhraseIndex], curPhrase.icon, m_phraseButtonConfig, m_icoFileFolders );
          m_phraseButtons[newIndex].SetText( PhraseToButtonText( category.phrases[newIndex] ) );
          SetSSButtonIcon( m_phraseButtons[newIndex], category.phrases[newIndex].icon, m_phraseButtonConfig, m_icoFileFolders );
        }
        else
        {
          CreatePhraseButtons( category );
        }
        m_selectedPhraseIndex = newIndex;
        RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
      }
    }
  }

  m_minimizeWhenLosingFocus = previousValue;
}

void CategoryWindow::DeleteLastSelection()
{
  if( m_selectedCategoryIndex < 0 || m_selectedCategoryIndex >= (int) m_categories.size() ) return;

  bool previousValue = m_minimizeWhenLosingFocus;
  m_minimizeWhenLosingFocus = false; // keep window visible while prompting

  if( m_categorySelectedLast )
  {
    const std::wstring & name = m_categories[m_selectedCategoryIndex].name;
    std::wstring prompt = GetLocalizedString( DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, m_language ) + name + GetLocalizedString( DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, m_language );
    if( ShowLocalizedMessageBox( m_hwnd, prompt.c_str(), GetLocalizedString( DELETE_CATEGORY_CONFIRMATION_TITLE_ID, m_language ), MB_YESNO | MB_ICONQUESTION, m_language ) == IDYES )
    {
      m_categories.erase( m_categories.begin() + m_selectedCategoryIndex );
      CreateCategoryButtons();
      m_phraseButtons.clear(); // no phrases shown until a category is re-selected below
      m_selectedCategoryIndex = -1;
      m_selectedPhraseIndex = -1;
      if( !m_categories.empty() )
      {
        OnCategorySelected( 0 );
      }
      UpdateButtonIcons();
      RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
    }
  }
  else
  {
    auto & category = m_categories[m_selectedCategoryIndex];
    if( m_selectedPhraseIndex >= 0 && m_selectedPhraseIndex < (int) category.phrases.size() )
    {
      const auto & phrase = category.phrases[m_selectedPhraseIndex];
      std::wstring display = PhraseToButtonText( phrase );
      std::wstring prompt = GetLocalizedString( DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, m_language ) + display + GetLocalizedString( DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, m_language );
      if( ShowLocalizedMessageBox( m_hwnd, prompt.c_str(), GetLocalizedString( DELETE_PHRASE_CONFIRMATION_TITLE_ID, m_language ), MB_YESNO | MB_ICONQUESTION, m_language ) == IDYES )
      {
        category.phrases.erase( category.phrases.begin() + m_selectedPhraseIndex );
        m_selectedPhraseIndex = -1;
        CreatePhraseButtons( category ); // clears internally
        RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
      }
    }
  }

  m_minimizeWhenLosingFocus = previousValue;
}


void CategoryWindow::DeleteAllCategories()
{
  if( m_categories.empty() ) return;
  bool previousValue = m_minimizeWhenLosingFocus;
  m_minimizeWhenLosingFocus = false; // keep window visible while prompting
  if( ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, m_language ), GetLocalizedString( DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, m_language ), MB_YESNO | MB_ICONQUESTION, m_language ) == IDYES )
  {
    if( ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, m_language ), GetLocalizedString( DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, m_language ), MB_YESNO | MB_ICONQUESTION, m_language ) == IDNO )
    {
      m_categories.clear();
      m_boardStyleRaw = L"";
      ApplyBoardStyle();
      CreateCategoryButtons();
      m_phraseButtons.clear();
      m_selectedCategoryIndex = -1;
      m_selectedPhraseIndex = -1;
      UpdateButtonIcons();
      RedrawWindow( m_hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW );
      RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
    }
  }
  m_minimizeWhenLosingFocus = previousValue;
}

void CategoryWindow::ImportCategories( std::wstring filePath )
{
  if( filePath.empty() ) // if no file path provided, prompt user to select file
    filePath = PromptImportCategoriesFilePath( m_hwnd, m_language );

  if( !filePath.empty() ) // if user selected a file
  {
    std::vector<Category> importedCategories;
    std::wstring importedBoardStyle;
    bool importedOk;
    if( IsZipArchive( filePath ) || StringEndsWithCI( filePath, L".ssz" ) )
    {
      std::wstring errorDetail;
      importedOk = ImportCategoriesFromSsz( filePath, GetAppDataCustomFolder( APP_NAME ), importedCategories, errorDetail, &importedBoardStyle );
      if( !importedOk && !errorDetail.empty() )
        OutputDebugStringW( ( L"[SSZ import] " + errorDetail + L"\n" ).c_str() );
    }
    else
    {
      importedOk = ImportCategoriesFromFile( filePath, importedCategories, &importedBoardStyle );
    }

    if( importedOk )
    {
      // Board style (STY-F53): apply silently when none exists locally; ask
      // before replacing an existing, different one. Identical styles need
      // neither prompt nor work.
      bool adoptedBoardStyle = false;
      if( !importedBoardStyle.empty() && importedBoardStyle != m_boardStyleRaw )
      {
        bool applyIncoming = m_boardStyleRaw.empty()
          || ( ShowLocalizedMessageBox( m_hwnd,
            GetLocalizedString( IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, m_language ),
            GetLocalizedString( IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, m_language ),
            MB_YESNO | MB_ICONQUESTION, m_language ) == IDYES );
        if( applyIncoming )
        {
          m_boardStyleRaw = importedBoardStyle;
          adoptedBoardStyle = true;
          ApplyBoardStyle();
        }
      }

      int importedCount = 0;
      while( !importedCategories.empty() )
      {
        // check if the category name does not conflict with existing category names
        bool duplicated = false;
        for( size_t i = 0; i < m_categories.size(); i++ )
        {
          if( m_categories[i].name == importedCategories[0].name )
          {
            duplicated = true;
            std::wstring prompt = GetLocalizedString( IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, m_language ) + importedCategories[0].name + GetLocalizedString( IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, m_language );
            if( ShowLocalizedMessageBox( m_hwnd, prompt.c_str(), GetLocalizedString( IMPORT_CATEGORY_OVERWRITE_TITLE_ID, m_language ), MB_YESNO | MB_ICONQUESTION, m_language ) == IDYES )
            {
              m_categories[i] = importedCategories[0];
              importedCount++;
            }
            else
            {

            }
            importedCategories.erase( importedCategories.begin() );
            i = m_categories.size(); // break loop
          }
        }

        if( !duplicated )
        {
          importedCount++;
          m_categories.push_back( importedCategories.front() );
          importedCategories.erase( importedCategories.begin() );
        }
      }

      // Something changed if categories were taken OR a new board style was
      // adopted (style-only import). Both cases refresh, persist, force a full
      // erase-redraw so a styled background/separator repaints in place, and
      // confirm — carrying the imported board's title/credits (STY-F57).
      if( importedCount || adoptedBoardStyle )
      {
        CreateCategoryButtons();
        OnCategorySelected( m_selectedCategoryIndex );
        UpdateButtonIcons();
        RegistryManager::SaveCategoriesToRegistry( m_categories, m_language, true, m_boardStyleRaw );
        RedrawWindow( m_hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW );

        // Title/credits come from the imported file's style (not the local one)
        // so they show even when the user declined to replace an existing style.
        std::wstring successMsg = GetLocalizedString( IMPORT_SUCCESS_MESSAGE_ID, m_language );
        if( !importedBoardStyle.empty() )
        {
          BoardStyle imported;
          ParseBoardStyleList( importedBoardStyle, imported );
          if( !imported.window.title.empty() )   successMsg += L"\n\n" + imported.window.title;
          if( !imported.window.credits.empty() ) successMsg += L"\n" + imported.window.credits;
        }
        ShowLocalizedMessageBox( m_hwnd, successMsg.c_str(), GetLocalizedString( IMPORT_SUCCESS_TITLE_ID, m_language ), MB_OK | MB_ICONINFORMATION, m_language );
      }
    }
    else
    {
      ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( IMPORT_FAILURE_MESSAGE_ID, m_language ), GetLocalizedString( IMPORT_FAILURE_TITLE_ID, m_language ), MB_OK | MB_ICONERROR, m_language );
    }
  }
}

void CategoryWindow::ExportCategories()
{
  std::vector<Category> singleCategory;
  bool exportAll = ( m_selectedCategoryIndex < 0 || m_selectedCategoryIndex >= (int) m_categories.size() );
  std::wstring suggestedFileName = GetISODateString() + L" " + ( m_boardStyle.window.title.empty() ? GetUserNameString() : m_boardStyle.window.title ) + L" " + GetLanguageNativeName( m_language );
  if( !exportAll )
  {
    std::wstring prompt = GetLocalizedString( EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, m_language ) + m_categories[m_selectedCategoryIndex].name + GetLocalizedString( EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, m_language );
    if( ShowLocalizedMessageBox( m_hwnd, prompt.c_str(), GetLocalizedString( EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, m_language ), MB_YESNO | MB_ICONQUESTION, m_language ) == IDYES )
    {
      singleCategory.push_back( m_categories[m_selectedCategoryIndex] );
      suggestedFileName = GetISODateString() + L" " + m_categories[m_selectedCategoryIndex].name + L" " + GetLanguageNativeName( m_language );
    }
    else
    {
      exportAll = true;
    }
  }
  suggestedFileName = ReplaceAmpersandLocalized( suggestedFileName, m_language ); // remove & from file name to avoid issues
  suggestedFileName = ReplaceAll( suggestedFileName, L" ", L"_" );

  const std::vector<Category> & toExport = exportAll ? m_categories : singleCategory;
  const std::wstring resourceFolder = GetAppDataCustomFolder( APP_NAME );
  // Auto-pick the bundle format: .ssz when there are icon/audio files to carry,
  // plain .ssc otherwise.
  const bool preferSsz = CategoriesHaveBundledResources( toExport, resourceFolder );

  std::wstring filePath = PromptExportCategoriesFilePath( m_hwnd, m_language, suggestedFileName, preferSsz ? L"ssz" : L"ssc" );
  if( !filePath.empty() )
  {
    // Honour an explicit extension typed by the user; otherwise use the auto choice.
    bool useSsz;
    if( StringEndsWithCI( filePath, L".ssz" ) ) useSsz = true;
    else if( StringEndsWithCI( filePath, L".ssc" ) ) useSsz = false;
    else useSsz = preferSsz;

    // Export-all carries the board style; export-selected only carries the
    // category's own style (board-style.spec.md STY-F52).
    const std::wstring boardStyleToExport = exportAll ? m_boardStyleRaw : std::wstring();
    const bool exportedOk = useSsz
      ? ExportCategoriesToSsz( toExport, filePath, resourceFolder, /*appDataOnly=*/true, boardStyleToExport )
      : ExportCategoriesToFile( toExport, filePath, boardStyleToExport );

    if( exportedOk )
    {
      ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( EXPORT_SUCCESS_MESSAGE_ID, m_language ), GetLocalizedString( EXPORT_SUCCESS_TITLE_ID, m_language ), MB_OK | MB_ICONINFORMATION, m_language );
    }
    else
    {
      ShowLocalizedMessageBox( m_hwnd, GetLocalizedString( EXPORT_FAILURE_MESSAGE_ID, m_language ), GetLocalizedString( EXPORT_FAILURE_TITLE_ID, m_language ), MB_OK | MB_ICONERROR, m_language );
    }
  }
}

void CategoryWindow::ResetZoom()
{
  if( m_zoom_factor != 1.0f )
  {
    m_zoom_factor = 1.0f;
    SafeTextResize();
    RefreshLayout();
    UpdateButtonIcons();
  }
}

void CategoryWindow::ZoomOut()
{
  if( m_zoom_factor > MIN_ZOOM_FACTOR )
  {
    m_zoom_factor = ( int( m_zoom_factor / ZOOM_STEP ) - 1 ) * ZOOM_STEP;
    SafeTextResize();
    RefreshLayout();
    UpdateButtonIcons();
  }
}

void CategoryWindow::ZoomIn()
{
  if( m_zoom_factor < MAX_ZOOM_FACTOR )
  {
    m_zoom_factor = ( int( m_zoom_factor / ZOOM_STEP ) + 1 ) * ZOOM_STEP;
    SafeTextResize();
    RefreshLayout();
    UpdateButtonIcons();
  }
}

void CategoryWindow::SafeTextResize()
{
  // Fonts are rebuilt from the styled properties x the current zoom factor and
  // re-applied to every live control before the old handles are deleted.
  RebuildFonts();
}

void CategoryWindow::OnPhraseSelected( int phraseIndex )
{
  if( m_selectedCategoryIndex >= 0 && m_selectedCategoryIndex < (int) m_categories.size() )
  {
    const Category & selectedCategory = m_categories[m_selectedCategoryIndex];

    if( phraseIndex >= 0 && phraseIndex < (int) selectedCategory.phrases.size() )
    {
      const Phrase & selectedPhrase = selectedCategory.phrases[phraseIndex];
      m_selectedPhraseIndex = phraseIndex;
      m_categorySelectedLast = false;

      if( m_mainWindow )
      {
        // Set the text to be played in the main window, using a special format if it's an audio file then USE AUDIO FILE NAME INSTEAD OF TEXT between SOUND_NOTE_DELIMITER to indicate it's an audio file
        m_mainWindow->SetEditControlText( selectedPhrase.audioFile.empty() ? ( selectedPhrase.text ) : ( SOUND_NOTE_DELIMITER + selectedPhrase.audioFile + SOUND_NOTE_DELIMITER ) );
      }
    }

    m_categoryButtons[m_selectedCategoryIndex].SetFocus();
  }
}