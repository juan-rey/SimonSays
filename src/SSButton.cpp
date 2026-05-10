/*
   SSButton.cpp: SSButton custom owner-drawn button class source file.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSButton.h"
#include <windowsx.h> // GET_X_LPARAM / GET_Y_LPARAM
#include <d2d1_1.h>   // pulls in d2d1.h; ID2D1Factory1 + ID2D1DeviceContext
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "msimg32.lib") // AlphaBlend

// Clamp-adjusted brightness: delta > 0 = lighter, delta < 0 = darker
static COLORREF AdjustBrightnessDelta( COLORREF c, int delta )
{
  int r = min( 255, max( 0, (int) GetRValue( c ) + delta ) );
  int g = min( 255, max( 0, (int) GetGValue( c ) + delta ) );
  int b = min( 255, max( 0, (int) GetBValue( c ) + delta ) );
  return RGB( r, g, b );
}

// Adjusts brightness by multiplying each channel by factor, then clamping to [0,255].
static COLORREF AdjustBrightnessFactor( COLORREF base, float factor )
{
  BYTE r = GetRValue( base );
  BYTE g = GetGValue( base );
  BYTE b = GetBValue( base );

  return RGB(
    (BYTE) std::min<float>( 255, (float) r * factor ),
    (BYTE) std::min<float>( 255, (float) g * factor ),
    (BYTE) std::min<float>( 255, (float) b * factor )
  );
}

// Maps BS_LEFT/RIGHT/CENTER/TOP/BOTTOM/VCENTER style bits to DT_ flags for
// DrawText. Vertical bits are only honored for single-line text — multi-line
// vertical alignment is computed manually after a DT_CALCRECT measure pass.
static UINT TextDtFlagsForStyle( DWORD style, bool isMultiline )
{
  UINT flags = DT_NOPREFIX;

  switch( style & ( BS_LEFT | BS_RIGHT ) )
  {
    case BS_LEFT:    flags |= DT_LEFT;   break;
    case BS_RIGHT:   flags |= DT_RIGHT;  break;
    case BS_CENTER:  // BS_LEFT | BS_RIGHT
    default:         flags |= DT_CENTER; break;
  }

  if( isMultiline )
  {
    flags |= DT_WORDBREAK;
  }
  else
  {
    flags |= DT_SINGLELINE;
    switch( style & ( BS_TOP | BS_BOTTOM ) )
    {
      case BS_TOP:     flags |= DT_TOP;     break;
      case BS_BOTTOM:  flags |= DT_BOTTOM;  break;
      case BS_VCENTER: // BS_TOP | BS_BOTTOM
      default:         flags |= DT_VCENTER; break;
    }
  }
  return flags;
}

// Carves an icon area out of contentRc (in/out) at the requested edge and
// shrinks contentRc to the remaining label area. iconSize is the square
// pixel size; padding is the gap between icon and label.
static void LayoutIconArea( SSButtonIconPosition pos, int iconSize, int padding,
  RECT & contentRc, RECT & iconRc )
{
  iconRc = contentRc;
  switch( pos )
  {
    case SSButtonIconPosition::Left:
      iconRc.right = iconRc.left + iconSize;
      contentRc.left = iconRc.right + padding;
      break;
    case SSButtonIconPosition::Right:
      iconRc.left = iconRc.right - iconSize;
      contentRc.right = iconRc.left - padding;
      break;
    case SSButtonIconPosition::Top:
      iconRc.bottom = iconRc.top + iconSize;
      contentRc.top = iconRc.bottom + padding;
      break;
    case SSButtonIconPosition::Bottom:
      iconRc.top = iconRc.bottom - iconSize;
      contentRc.bottom = iconRc.top - padding;
      break;
  }
}

// -------------------------------------------------------------------------
// Color emoji rendering (process-wide cache)
// -------------------------------------------------------------------------
//
// Pipeline: ID2D1DCRenderTarget (PREMULTIPLIED + GRAYSCALE AA) bound to a
// 32-bpp top-down DIB section we own → AlphaBlend onto the caller's HDC.
//
// Three things must hold simultaneously for color glyphs (Segoe UI Emoji's
// COLR/CPAL tables) to render through DCRT:
//   1. ID2D1Factory1 (basic ID2D1Factory's DCRT does not honor color fonts).
//   2. PREMULTIPLIED alpha mode (IGNORE silently degrades to monochrome).
//   3. GRAYSCALE text AA mode (ClearType cannot composite color glyphs onto
//      a transparent target).

static bool s_classRegistered = false;

static ID2D1Factory1 * s_pD2DFactory = nullptr;
static IDWriteFactory * s_pDWriteFactory = nullptr;
static IDWriteTextFormat * s_pTextFmt = nullptr;  // device-independent
static float                 s_textFmtSize = 0.0f;
static ID2D1DCRenderTarget * s_pDCRT = nullptr;
static HDC                   s_emojiDC = nullptr;
static HBITMAP               s_emojiBmp = nullptr;
static HBITMAP               s_emojiOldBmp = nullptr;
static int                   s_emojiW = 0;
static int                   s_emojiH = 0;

static void ReleaseEmojiStaging()
{
  if( s_pDCRT ) { s_pDCRT->Release(); s_pDCRT = nullptr; }
  if( s_emojiDC )
  {
    if( s_emojiOldBmp ) SelectObject( s_emojiDC, s_emojiOldBmp );
    DeleteDC( s_emojiDC );
    s_emojiDC = nullptr;
    s_emojiOldBmp = nullptr;
  }
  if( s_emojiBmp ) { DeleteObject( s_emojiBmp ); s_emojiBmp = nullptr; }
  s_emojiW = s_emojiH = 0;
}

static bool EnsureEmojiStaging( int w, int h )
{
  if( w <= 0 || h <= 0 ) return false;
  if( s_pDCRT && s_emojiBmp && s_emojiW == w && s_emojiH == h ) return true;
  ReleaseEmojiStaging();

  if( !s_pD2DFactory )
    D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &s_pD2DFactory );
  if( !s_pDWriteFactory )
    DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( IDWriteFactory ),
      reinterpret_cast<IUnknown **>( &s_pDWriteFactory ) );
  if( !s_pD2DFactory || !s_pDWriteFactory ) return false;

  BITMAPINFO bmi = {};
  bmi.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
  bmi.bmiHeader.biWidth = w;
  bmi.bmiHeader.biHeight = -h; // top-down
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  void * pBits = nullptr;
  s_emojiBmp = CreateDIBSection( nullptr, &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0 );
  if( !s_emojiBmp ) return false;

  s_emojiDC = CreateCompatibleDC( nullptr );
  if( !s_emojiDC ) { ReleaseEmojiStaging(); return false; }
  s_emojiOldBmp = (HBITMAP) SelectObject( s_emojiDC, s_emojiBmp );

  D2D1_RENDER_TARGET_PROPERTIES rtp = D2D1::RenderTargetProperties(
    D2D1_RENDER_TARGET_TYPE_DEFAULT,
    D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED ),
    96.0f, 96.0f ); // lock DPI: DIPs == pixels regardless of system scaling
  if( FAILED( s_pD2DFactory->CreateDCRenderTarget( &rtp, &s_pDCRT ) ) )
  {
    ReleaseEmojiStaging(); return false; // ← only releases staging, NOT the factories
  }
  s_pDCRT->SetTextAntialiasMode( D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE );

  s_emojiW = w;
  s_emojiH = h;
  return true;
}

// Renders emoji centered in emojiRc on hdc with color glyph support.
static void DrawEmoji( HDC hdc, const RECT & emojiRc, const std::wstring & emoji,
  int fontSize, bool isEnabled )
{
  const int w = emojiRc.right - emojiRc.left;
  const int h = emojiRc.bottom - emojiRc.top;
  if( w <= 0 || h <= 0 || emoji.empty() ) return;
  if( !EnsureEmojiStaging( w, h ) ) return;

  float fSize = (float) fontSize;
  if( !s_pTextFmt || s_textFmtSize != fSize )
  {
    if( s_pTextFmt ) { s_pTextFmt->Release(); s_pTextFmt = nullptr; }
    if( FAILED( s_pDWriteFactory->CreateTextFormat(
      L"Segoe UI Emoji", nullptr,
      DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
      fSize, L"en-us", &s_pTextFmt ) ) ) return;
    s_pTextFmt->SetTextAlignment( DWRITE_TEXT_ALIGNMENT_CENTER );
    s_pTextFmt->SetParagraphAlignment( DWRITE_PARAGRAPH_ALIGNMENT_CENTER );
    s_textFmtSize = fSize;
  }

  RECT bindRc = { 0, 0, w, h };
  if( FAILED( s_pDCRT->BindDC( s_emojiDC, &bindRc ) ) ) return;

  ID2D1SolidColorBrush * pBrush = nullptr;
  D2D1_COLOR_F fg = isEnabled ? D2D1::ColorF( D2D1::ColorF::Black )      // color glyphs override
    : D2D1::ColorF( 0.5f, 0.5f, 0.5f, 1.0f );  // grey monochrome fallback
  if( FAILED( s_pDCRT->CreateSolidColorBrush( fg, &pBrush ) ) ) return;

  s_pDCRT->BeginDraw();
  s_pDCRT->Clear( D2D1::ColorF( 0, 0, 0, 0 ) );

  // ENABLE_COLOR_FONT requires the ID2D1DeviceContext path. On Factory1 + Win 8.1+
  // the QI succeeds and color tables are honored.
  ID2D1DeviceContext * pDC = nullptr;
  D2D1_RECT_F layoutRc = D2D1::RectF( 0, 0, (float) w, (float) h );
  if( SUCCEEDED( s_pDCRT->QueryInterface( &pDC ) ) && pDC )
  {
    pDC->DrawText( emoji.c_str(), (UINT32) emoji.length(), s_pTextFmt,
      layoutRc, pBrush, D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT );
    pDC->Release();
  }
  else
  {
    s_pDCRT->DrawText( emoji.c_str(), (UINT32) emoji.length(), s_pTextFmt,
      layoutRc, pBrush ); // pre-8.1 fallback: monochrome
  }

  HRESULT hr = s_pDCRT->EndDraw();
  pBrush->Release();

  if( FAILED( hr ) )
  {
    if( hr == D2DERR_RECREATE_TARGET ) ReleaseEmojiStaging();
    return;
  }

  GdiFlush();
  BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
  AlphaBlend( hdc, emojiRc.left, emojiRc.top, w, h, s_emojiDC, 0, 0, w, h, bf );
}

// -------------------------------------------------------------------------
// Construction / destruction
// -------------------------------------------------------------------------

SSButton::SSButton() {}

SSButton::SSButton( SSButton && other ) noexcept
  : m_hwnd( other.m_hwnd ),
  m_hExternalFont( other.m_hExternalFont ),
  m_config( std::move( other.m_config ) ),
  m_style( other.m_style ),
  m_exStyle( other.m_exStyle ),
  m_pressed( other.m_pressed ),
  m_hovered( other.m_hovered ),
  m_focused( other.m_focused ),
  m_trackMouse( other.m_trackMouse ),
  m_keyDown( other.m_keyDown ),
  m_hIcon( other.m_hIcon ),
  m_backgroundColor( other.m_backgroundColor ),
  m_hoverColor( other.m_hoverColor ),
  m_pressedColor( other.m_pressedColor ),
  m_disabledColor( other.m_disabledColor ),
  m_textColor( other.m_textColor ),
  m_highlightBorderColor( other.m_highlightBorderColor ),
  m_lightBorderColor( other.m_lightBorderColor ),
  m_shadowBorderColor( other.m_shadowBorderColor ),
  m_darkShadowBorderColor( other.m_darkShadowBorderColor ),
  m_text( std::move( other.m_text ) )
{
  other.m_hwnd = nullptr; // prevent moved-from destructor from destroying the HWND
  other.m_hExternalFont = nullptr;
  other.m_hIcon = nullptr;
  if( m_hwnd )
    SetWindowLongPtr( m_hwnd, GWLP_USERDATA, (LONG_PTR) this ); // re-point to new address
}

SSButton::~SSButton()
{
  if( m_hwnd && IsWindow( m_hwnd ) )
    DestroyWindow( m_hwnd );
  ReleaseIcon();
}

void SSButton::ReleaseIcon()
{
  if( m_hIcon ) { DestroyIcon( m_hIcon ); m_hIcon = nullptr; }
}

// -------------------------------------------------------------------------
// Class registration
// -------------------------------------------------------------------------

bool SSButton::RegisterWindowClass( HINSTANCE hInstance )
{
  if( s_classRegistered ) return true;

  WNDCLASSEX wc = {};
  wc.cbSize = sizeof( WNDCLASSEX );
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  wc.lpfnWndProc = SSButton::WindowProc;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
  wc.hbrBackground = nullptr; // handled in WM_ERASEBKGND / WM_PAINT
  wc.lpszClassName = SSBUTTON_CLASS;

  if( RegisterClassEx( &wc ) )
  {
    s_classRegistered = true;
    return true;
  }
  // Already registered by a previous instance is not an error
  return ( GetLastError() == ERROR_CLASS_ALREADY_EXISTS );
}

// -------------------------------------------------------------------------
// Creation
// -------------------------------------------------------------------------

HWND SSButton::Create( HWND hwndParent, HINSTANCE hInstance, int id,
  const std::wstring & text,
  int x, int y, int w, int h,
  DWORD style, DWORD exStyle,
  const SSButtonConfig & config )
{
  RegisterWindowClass( hInstance );

  SetConfig( config );
  m_style = style;
  m_exStyle = exStyle;
  m_text = text;

  m_hwnd = CreateWindowEx(
    exStyle,
    SSBUTTON_CLASS,
    text.c_str(),
    style | WS_CHILD,
    x, y, w, h,
    hwndParent,
    (HMENU) (UINT_PTR) id,
    hInstance,
    this
  );

  return m_hwnd;
}

// -------------------------------------------------------------------------
// Public setters
// -------------------------------------------------------------------------

void SSButton::SetConfig( const SSButtonConfig & config )
{
  if( config.iconFileFullPath != m_config.iconFileFullPath )
  {
    ReleaseIcon();
    if( config.iconType == SSButtonIconType::StandardIcon && !config.iconFileFullPath.empty() )
    {
      int sz = ( config.iconSize > 0 ) ? config.iconSize : SSBUTTON_ICON_DEFAULT_SIZE;
      m_hIcon = (HICON) LoadImage( nullptr, config.iconFileFullPath.c_str(), IMAGE_ICON,
        sz, sz, LR_LOADFROMFILE | LR_DEFAULTCOLOR );
    }
  }
  // Switching icon TYPE away from StandardIcon: drop the loaded handle even if path is unchanged.
  else if( config.iconType != SSButtonIconType::StandardIcon )
  {
    ReleaseIcon();
  }

  m_config = config;

  if( m_config.bgType == SSButtonBackground::Color )
  {
    SetColors( m_config.bgColor );
  }
  else
  {
    SetSystemColors( m_config.textColorType == SSButtonTextColor::Default );
  }

  m_config.borderWidth = min( 2, max( 0, m_config.borderWidth ) ); // sanitize border width to avoid drawing glitches

  if( m_hwnd ) InvalidateRect( m_hwnd, nullptr, TRUE );
}

void SSButton::SetColors( COLORREF backgroundColor, COLORREF textColor, COLORREF hoverColor, COLORREF pressedColor, COLORREF disabledColor )
{
  if( backgroundColor != CLR_NONE )
  {
    m_config.bgType = SSButtonBackground::Color;
    m_config.bgColor = backgroundColor;
    m_backgroundColor = backgroundColor;
  }

  if( textColor != CLR_NONE )
  {
    m_config.textColorType = SSButtonTextColor::Custom;
    m_config.textColor = textColor;
    m_textColor = textColor;
  }

  if( m_config.bgType == SSButtonBackground::Color )
  {
    m_hoverColor = ( hoverColor == CLR_NONE ) ? AdjustBrightnessDelta( m_backgroundColor, 18 ) : hoverColor;
    m_pressedColor = ( pressedColor == CLR_NONE ) ? AdjustBrightnessDelta( m_backgroundColor, -25 ) : pressedColor;
    m_disabledColor = ( disabledColor == CLR_NONE ) ? AdjustBrightnessDelta( m_backgroundColor, 20 ) : disabledColor;

    m_highlightBorderColor = AdjustBrightnessFactor( m_backgroundColor, 1.45f );
    m_lightBorderColor = AdjustBrightnessFactor( m_backgroundColor, 1.15f );
    m_shadowBorderColor = AdjustBrightnessFactor( m_backgroundColor, 0.75f );
    m_darkShadowBorderColor = AdjustBrightnessFactor( m_backgroundColor, 0.55f );
  }

  Invalidate();
}


void SSButton::SetSystemColors( bool includeTextColor )
{
  m_config.bgType = SSButtonBackground::Default;
  if( includeTextColor )
    m_config.textColorType = SSButtonTextColor::Default;

  // Always cache an actual text color: the rounded-border focus rect reads
  // m_textColor for its pen, and CreatePen(CLR_NONE) yields a garbage color.
  m_textColor = ( m_config.textColorType == SSButtonTextColor::Custom )
    ? m_config.textColor
    : GetSysColor( COLOR_BTNTEXT );
  m_backgroundColor = GetSysColor( COLOR_BTNFACE );
  m_hoverColor = AdjustBrightnessDelta( m_backgroundColor, 18 );
  m_pressedColor = AdjustBrightnessDelta( m_backgroundColor, -25 );
  m_disabledColor = AdjustBrightnessDelta( m_backgroundColor, 20 );
  m_highlightBorderColor = GetSysColor( COLOR_BTNHIGHLIGHT );
  m_lightBorderColor = GetSysColor( COLOR_3DLIGHT );
  m_shadowBorderColor = GetSysColor( COLOR_BTNSHADOW );
  m_darkShadowBorderColor = GetSysColor( COLOR_3DDKSHADOW );

  Invalidate();
}

void SSButton::SetIcon( const std::wstring & iconFileFullPath, int iconSize, bool updateIconPosition, SSButtonIconPosition iconPosition )
{
  if( iconFileFullPath.empty() ) { NoIcon(); return; }

  m_config.iconType = SSButtonIconType::StandardIcon;
  m_config.iconFileFullPath = iconFileFullPath;
  m_config.iconSize = iconSize;
  if( updateIconPosition )
    m_config.iconPosition = iconPosition;
  ReleaseIcon();
  int sz = ( iconSize > 0 ) ? iconSize : SSBUTTON_ICON_DEFAULT_SIZE;
  m_hIcon = (HICON) LoadImage( nullptr, iconFileFullPath.c_str(), IMAGE_ICON,
    sz, sz, LR_LOADFROMFILE | LR_DEFAULTCOLOR );
  Invalidate();
}

void SSButton::SetEmoji( const std::wstring & emoji, int iconSize, bool updateIconPosition, SSButtonIconPosition iconPosition )
{
  if( emoji.empty() ) { NoIcon(); return; }

  m_config.iconType = SSButtonIconType::Emoji;
  m_config.emoji = emoji;
  if( updateIconPosition )
    m_config.iconPosition = iconPosition;
  m_config.iconSize = iconSize;
  ReleaseIcon(); // drop any previously loaded standard icon
  m_config.iconFileFullPath.clear();
  Invalidate();
}

void SSButton::NoIcon()
{
  m_config.iconType = SSButtonIconType::None;
  m_config.iconFileFullPath.clear();
  m_config.emoji.clear();
  ReleaseIcon();
  Invalidate();
}

void SSButton::SetFont( HFONT hFont, bool redraw )
{
  m_hExternalFont = hFont;
  if( m_hwnd )
    SendMessage( m_hwnd, WM_SETFONT, (WPARAM) hFont, redraw ? TRUE : FALSE );
}

void SSButton::SetText( const std::wstring & text )
{
  // SetWindowText triggers WM_SETTEXT which already updates m_text.
  if( m_hwnd ) SetWindowText( m_hwnd, text.c_str() );
  else        m_text = text; // no HWND yet — still keep the cached text in sync
}

void SSButton::SetStyle( DWORD style, bool reframe )
{
  if( !m_hwnd ) return;
  SetWindowLongPtr( m_hwnd, GWL_STYLE, (LONG_PTR) style );
  m_style = style;
  if( reframe )
    SetWindowPos( m_hwnd, nullptr, 0, 0, 0, 0,
      SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED );
  InvalidateRect( m_hwnd, nullptr, TRUE );
}

void SSButton::SetPos( int x, int y, int w, int h, UINT swpFlags )
{
  if( m_hwnd ) SetWindowPos( m_hwnd, nullptr, x, y, w, h, swpFlags );
}

void SSButton::Invalidate( bool eraseBackground )
{
  if( m_hwnd ) InvalidateRect( m_hwnd, nullptr, eraseBackground ? TRUE : FALSE );
}

void SSButton::SetFocus()
{
  if( m_hwnd ) ::SetFocus( m_hwnd );
}

void SSButton::SetEnabled( bool enabled )
{
  if( m_hwnd ) EnableWindow( m_hwnd, enabled ? TRUE : FALSE );
}

bool SSButton::IsEnabled() const
{
  return m_hwnd && IsWindowEnabled( m_hwnd ) != FALSE;
}

void SSButton::Show( bool show )
{
  if( m_hwnd ) ShowWindow( m_hwnd, show ? SW_SHOWNA : SW_HIDE );
}

void SSButton::Destroy()
{
  if( m_hwnd && IsWindow( m_hwnd ) )
    DestroyWindow( m_hwnd ); // WM_DESTROY clears m_hwnd
}

// -------------------------------------------------------------------------
// Private helpers
// -------------------------------------------------------------------------

COLORREF SSButton::ResolvedTextColor( bool isEnabled ) const
{
  if( !isEnabled ) return GetSysColor( COLOR_GRAYTEXT );
  if( m_config.textColorType == SSButtonTextColor::Custom ) return m_config.textColor;
  return GetSysColor( COLOR_BTNTEXT );
}

COLORREF SSButton::ResolvedBgColor( bool isEnabled ) const
{
  if( !isEnabled )  return m_disabledColor;
  if( m_pressed )   return m_pressedColor;
  if( m_hovered )   return m_hoverColor;
  return m_backgroundColor;
}

// Send BN_CLICKED to the parent, mirroring standard BUTTON behaviour
void SSButton::FireClick( HWND hwnd )
{
  HWND parent = GetParent( hwnd );
  if( parent )
    SendMessage( parent, WM_COMMAND,
      MAKEWPARAM( GetDlgCtrlID( hwnd ), BN_CLICKED ),
      (LPARAM) hwnd );
}

// -------------------------------------------------------------------------
// Paint
// -------------------------------------------------------------------------

void SSButton::Paint( HWND hwnd )
{
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint( hwnd, &ps );

  RECT rc;
  GetClientRect( hwnd, &rc );

  // Double-buffer to avoid flicker
  HDC     memDC = CreateCompatibleDC( hdc );
  HBITMAP memBmp = CreateCompatibleBitmap( hdc, rc.right, rc.bottom );
  HBITMAP oldBmp = (HBITMAP) SelectObject( memDC, memBmp );

  bool isEnabled = IsWindowEnabled( hwnd ) != FALSE;
  bool isFlat = ( m_style & BS_FLAT ) != 0;

  // ------------------------------------------------------------------
  // 0. Seed back-buffer with parent background so rounded corners
  //    show through rather than rendering as black.
  // ------------------------------------------------------------------
  if( m_config.borderStyle == SSButtonBorderStyle::Rounded )
  {
    HWND hwndParent = GetParent( hwnd );
    POINT ptOrg = { 0, 0 };
    MapWindowPoints( hwnd, hwndParent, &ptOrg, 1 );
    SetViewportOrgEx( memDC, -ptOrg.x, -ptOrg.y, nullptr );
    SendMessage( hwndParent, WM_ERASEBKGND, (WPARAM) memDC, 0 );
    SetViewportOrgEx( memDC, 0, 0, nullptr );
  }

  // ------------------------------------------------------------------
  // 1. Background
  // ------------------------------------------------------------------
  COLORREF c = ResolvedBgColor( isEnabled );
  HBRUSH br = CreateSolidBrush( c );
  if( m_config.borderStyle == SSButtonBorderStyle::Rounded )
  {
    SetBkMode( memDC, TRANSPARENT );
    HPEN hPen = CreatePen( PS_SOLID, 1, c );
    HPEN hOldPen = (HPEN) SelectObject( memDC, hPen );
    HBRUSH hOldBrush = (HBRUSH) SelectObject( memDC, br );
    RoundRect( memDC, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius * 2, m_config.cornerRadius * 2 );
    SelectObject( memDC, hOldBrush );
    SelectObject( memDC, hOldPen );
    DeleteObject( hPen );
  }
  else
    FillRect( memDC, &rc, br );
  DeleteObject( br );

  // ------------------------------------------------------------------
  // 2. Border
  // ------------------------------------------------------------------ 
  // Helper lambda to draw a rounded rect with a given pen colour
  auto OutlineRect = [&]( COLORREF outerLtColor, COLORREF outerRbColor, int l, int t, int r, int b, int d = 0, int penstyle = PS_SOLID )
    {
      HPEN outerRbPen = CreatePen( penstyle, 1, outerRbColor );
      HPEN oldPen = (HPEN) SelectObject( memDC, outerRbPen );
      MoveToEx( memDC, r + d - 1, t - d, nullptr );
      LineTo( memDC, r + d - 1, b + d - 1 );
      LineTo( memDC, l - d, b + d - 1 );
      LineTo( memDC, l - d, b + d - 1 - 1 );
      HPEN outerLtPen = CreatePen( penstyle, 1, outerLtColor );
      SelectObject( memDC, outerLtPen );
      LineTo( memDC, l - d, t - d );
      LineTo( memDC, r + d - 1, t - d );
      SelectObject( memDC, outerRbPen );
      LineTo( memDC, r + d - 1, t - d );
      SelectObject( memDC, oldPen );
      DeleteObject( outerRbPen );
      DeleteObject( outerLtPen );
    };

  auto OutlineRoundRect = [&]( COLORREF outerLtColor, COLORREF outerRbColor, int l, int t, int r, int b, int radius, int d = 0, int penstyle = PS_SOLID )
    {
      int rd = min( min( radius, ( b - t ) / 2 ), ( r - l ) / 2 ); // sanity check: radius should not exceed half of width or height
      rd += d; // allow caller to adjust radius for inner vs outer border
      HPEN outerRbPen = CreatePen( penstyle, 1, outerRbColor );
      HPEN oldPen = (HPEN) SelectObject( memDC, outerRbPen );
      MoveToEx( memDC, r + d - 1, t - d + rd, nullptr );
      AngleArc( memDC, r + d - 1 - rd, t - d + rd, rd, 0, 45 );
      MoveToEx( memDC, r + d - 1, t - d + rd, nullptr );
      LineTo( memDC, r + d - 1, b + d - rd );
      AngleArc( memDC, r + d - 1 - rd, b + d - 1 - rd, rd, 0, -90 );
      LineTo( memDC, l - d + rd, b + d - 1 );
      AngleArc( memDC, l - d + rd, b + d - 1 - rd, rd, 270, -45 );
      HPEN outerLtPen = CreatePen( penstyle, 1, outerLtColor );
      SelectObject( memDC, outerLtPen );
      AngleArc( memDC, l - d + rd, b + d - 1 - rd, rd, 225, -45 );
      LineTo( memDC, l - d, t - d + rd );
      AngleArc( memDC, l - d + rd, t - d + rd, rd, 180, -90 );
      LineTo( memDC, r + d - rd, t - d );
      AngleArc( memDC, r + d - 1 - rd, t - d + rd, rd, 90, -45 );
      SelectObject( memDC, oldPen );
      DeleteObject( outerRbPen );
      DeleteObject( outerLtPen );
    };

  if( m_config.borderWidth > 0 )
  {
    if( m_config.borderStyle == SSButtonBorderStyle::Rounded )
    {
      if( isFlat )
      {
        OutlineRoundRect( m_darkShadowBorderColor, m_darkShadowBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius );
        if( m_config.borderWidth > 1 )
          OutlineRoundRect( m_highlightBorderColor, m_highlightBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius, -1 );
      }
      else if( !m_pressed )
      {
        OutlineRoundRect( m_highlightBorderColor, m_darkShadowBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius );
        if( m_config.borderWidth > 1 )
          OutlineRoundRect( m_lightBorderColor, m_shadowBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius, -1 );
      }
      else
      {
        OutlineRoundRect( m_darkShadowBorderColor, m_highlightBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius );
        if( m_config.borderWidth > 1 )
          OutlineRoundRect( m_shadowBorderColor, m_lightBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius, -1 );
      }
    }
    else // Square
    {
      if( isFlat )
      {
        OutlineRect( m_darkShadowBorderColor, m_darkShadowBorderColor, rc.left, rc.top, rc.right, rc.bottom );
        if( m_config.borderWidth > 1 )
          OutlineRect( m_highlightBorderColor, m_highlightBorderColor, rc.left, rc.top, rc.right, rc.bottom, -1 );
      }
      else if( !m_pressed )
      {
        OutlineRect( m_highlightBorderColor, m_darkShadowBorderColor, rc.left, rc.top, rc.right, rc.bottom );
        if( m_config.borderWidth > 1 )
          OutlineRect( m_lightBorderColor, m_shadowBorderColor, rc.left, rc.top, rc.right, rc.bottom, -1 );
      }
      else
      {
        OutlineRect( m_darkShadowBorderColor, m_highlightBorderColor, rc.left, rc.top, rc.right, rc.bottom );
        if( m_config.borderWidth > 1 )
          OutlineRect( m_shadowBorderColor, m_lightBorderColor, rc.left, rc.top, rc.right, rc.bottom, -1 );
      }
    }
  }

  // ------------------------------------------------------------------
  // 3. Content rect (inset from border; shifted when pressed)
  // ------------------------------------------------------------------
  RECT contentRc = { rc.left + m_config.borderWidth, rc.top + m_config.borderWidth, rc.right - m_config.borderWidth, rc.bottom - m_config.borderWidth };
  if( m_pressed && m_config.borderStyle != SSButtonBorderStyle::Rounded )
    OffsetRect( &contentRc, 1, 1 );

  // ------------------------------------------------------------------
  // 4. Icon / emoji (positioned per m_config.iconPosition)
  // ------------------------------------------------------------------
  bool hasIcon = ( m_config.iconType == SSButtonIconType::StandardIcon && m_hIcon )
    || ( m_config.iconType == SSButtonIconType::Emoji && !m_config.emoji.empty() );
  if( hasIcon )
  {
    // Auto-size: derive from the dimension perpendicular to the icon edge so
    // the icon fits naturally without crowding out the label.
    int autoSize;
    if( m_config.iconPosition == SSButtonIconPosition::Top ||
      m_config.iconPosition == SSButtonIconPosition::Bottom )
      autoSize = ( contentRc.right - contentRc.left ) - 2 * ( m_config.iconPadding + m_config.borderWidth );
    else
      autoSize = ( contentRc.bottom - contentRc.top ) - 2 * ( m_config.iconPadding + m_config.borderWidth );

    int iconSize = m_config.iconSize > 0 ? m_config.iconSize : max( 0, autoSize );
    if( iconSize > 0 )
    {
      RECT iconRc;
      LayoutIconArea( m_config.iconPosition, iconSize, m_config.iconPadding, contentRc, iconRc );

      if( m_config.iconType == SSButtonIconType::StandardIcon )
      {
        // Center the square icon within iconRc (which may be wider/taller than iconSize).
        int iconX = iconRc.left + ( ( iconRc.right - iconRc.left ) - iconSize ) / 2;
        int iconY = iconRc.top + ( ( iconRc.bottom - iconRc.top ) - iconSize ) / 2;
        DrawIconEx( memDC, iconX, iconY, m_hIcon, iconSize, iconSize, 0, nullptr, DI_NORMAL );
      }
      else
      {
        DrawEmoji( memDC, iconRc, m_config.emoji, iconSize, isEnabled );
      }
    }
  }

  // ------------------------------------------------------------------
  // 5. Label text (BS_LEFT/RIGHT/CENTER + BS_TOP/BOTTOM/VCENTER honored)
  // ------------------------------------------------------------------
  if( !m_text.empty() )
  {
    HFONT oldFont = nullptr;
    if( m_hExternalFont ) oldFont = (HFONT) SelectObject( memDC, m_hExternalFont );

    SetTextColor( memDC, ResolvedTextColor( isEnabled ) );
    SetBkMode( memDC, TRANSPARENT );

    bool isMultiline = ( m_style & BS_MULTILINE ) != 0;
    UINT dtFlags = TextDtFlagsForStyle( m_style, isMultiline );

    if( isMultiline )
    {
      // Measure to compute the vertical alignment offset for the wrapped block.
      RECT measureRc = contentRc;
      DrawText( memDC, m_text.c_str(), -1, &measureRc, dtFlags | DT_CALCRECT );
      int blockH = measureRc.bottom - measureRc.top;
      int availH = contentRc.bottom - contentRc.top;

      RECT drawRc = contentRc;
      switch( m_style & ( BS_TOP | BS_BOTTOM ) )
      {
        case BS_TOP:    /* draw at top — no offset */               break;
        case BS_BOTTOM: drawRc.top += max( 0, availH - blockH );    break;
        case BS_VCENTER:
        default:        drawRc.top += max( 0, ( availH - blockH ) / 2 ); break;
      }
      DrawText( memDC, m_text.c_str(), -1, &drawRc, dtFlags );
    }
    else
    {
      DrawText( memDC, m_text.c_str(), -1, &contentRc, dtFlags );
    }

    if( oldFont ) SelectObject( memDC, oldFont );
  }

  // ------------------------------------------------------------------
  // 6. Focus rectangle
  // ------------------------------------------------------------------
  if( m_focused && isEnabled )
  {
    // Use the resolved text color (never CLR_NONE) so CreatePen always gets a real color.
    COLORREF focusColor = ResolvedTextColor( isEnabled );
    if( m_config.borderStyle != SSButtonBorderStyle::Rounded )
    {
      OutlineRect( focusColor, focusColor,
        rc.left, rc.top, rc.right, rc.bottom,
        -m_config.borderWidth - 1, PS_DOT );
    }
    else
    {
      OutlineRoundRect( focusColor, focusColor,
        rc.left, rc.top, rc.right, rc.bottom,
        m_config.cornerRadius, -m_config.borderWidth - 1, PS_DOT );
    }
  }

  BitBlt( hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY );

  SelectObject( memDC, oldBmp );
  DeleteObject( memBmp );
  DeleteDC( memDC );

  EndPaint( hwnd, &ps );
}

void SSButton::ReleaseSharedResources()
{
  if( s_pTextFmt ) { s_pTextFmt->Release();       s_pTextFmt = nullptr; }
  ReleaseEmojiStaging();
  if( s_pDWriteFactory ) { s_pDWriteFactory->Release(); s_pDWriteFactory = nullptr; }
  if( s_pD2DFactory ) { s_pD2DFactory->Release();    s_pD2DFactory = nullptr; }
  s_textFmtSize = 0.0f;
}

// -------------------------------------------------------------------------
// Window procedure
// -------------------------------------------------------------------------

LRESULT CALLBACK SSButton::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  SSButton * pThis = nullptr;

  if( uMsg == WM_CREATE )
  {
    CREATESTRUCT * pCreate = (CREATESTRUCT *) lParam;
    pThis = (SSButton *) pCreate->lpCreateParams;
    SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
    pThis->m_hwnd = hwnd;
  }
  else
  {
    pThis = (SSButton *) GetWindowLongPtr( hwnd, GWLP_USERDATA );
  }

  if( !pThis )
    return DefWindowProc( hwnd, uMsg, wParam, lParam );

  switch( uMsg )
  {
    // ------------------------------------------------------------------
    // Drawing
    // ------------------------------------------------------------------
    case WM_PAINT:
      pThis->Paint( hwnd );
      return 0;

    case WM_ERASEBKGND:
      return 1; // all background work happens in WM_PAINT

      // ------------------------------------------------------------------
      // Font / text
      // ------------------------------------------------------------------
    case WM_SETFONT:
      pThis->m_hExternalFont = (HFONT) wParam;
      if( LOWORD( lParam ) ) InvalidateRect( hwnd, nullptr, TRUE );
      return 0;

    case WM_GETFONT:
      return (LRESULT) pThis->m_hExternalFont;

    case WM_SETTEXT:
    {
      pThis->m_text = lParam ? (LPCWSTR) lParam : L""; // null lParam is legal: empty text
      LRESULT r = DefWindowProc( hwnd, uMsg, wParam, lParam );
      InvalidateRect( hwnd, nullptr, TRUE );
      return r;
    }

    // ------------------------------------------------------------------
    // Style changes (e.g. SetWindowLongPtr to switch FLAT_BUTTON_STYLE)
    // ------------------------------------------------------------------
    case WM_STYLECHANGED:
      pThis->m_style = (DWORD) GetWindowLongPtr( hwnd, GWL_STYLE );
      pThis->m_exStyle = (DWORD) GetWindowLongPtr( hwnd, GWL_EXSTYLE );
      InvalidateRect( hwnd, nullptr, TRUE );
      return 0;

      // ------------------------------------------------------------------
      // Theme / system color change — refresh derived colors
      // ------------------------------------------------------------------
    case WM_SYSCOLORCHANGE:
    case WM_THEMECHANGED:
      if( pThis->m_config.bgType == SSButtonBackground::Default )
        pThis->SetSystemColors( pThis->m_config.textColorType == SSButtonTextColor::Default );
      InvalidateRect( hwnd, nullptr, TRUE );
      return 0;

      // ------------------------------------------------------------------
      // Enable / disable
      // ------------------------------------------------------------------
    case WM_ENABLE:
      InvalidateRect( hwnd, nullptr, TRUE );
      return 0;

      // ------------------------------------------------------------------
      // Mouse interaction
      // ------------------------------------------------------------------
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
      if( IsWindowEnabled( hwnd ) )
      {
        SetCapture( hwnd );
        pThis->m_pressed = true;
        ::SetFocus( hwnd );
        InvalidateRect( hwnd, nullptr, FALSE );
      }
      return 0;

    case WM_LBUTTONUP:
      if( pThis->m_pressed )
      {
        ReleaseCapture();
        pThis->m_pressed = false;
        InvalidateRect( hwnd, nullptr, FALSE );

        POINT pt = { GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };
        RECT  rc;
        GetClientRect( hwnd, &rc );
        if( PtInRect( &rc, pt ) && IsWindowEnabled( hwnd ) )
          pThis->FireClick( hwnd );
      }
      return 0;

    case WM_CAPTURECHANGED:
      if( (HWND) lParam != hwnd )
      {
        pThis->m_pressed = false;
        InvalidateRect( hwnd, nullptr, FALSE );
      }
      return 0;

    case WM_MOUSEMOVE:
      if( !pThis->m_trackMouse )
      {
        TRACKMOUSEEVENT tme = { sizeof( TRACKMOUSEEVENT ), TME_LEAVE, hwnd, 0 };
        TrackMouseEvent( &tme );
        pThis->m_trackMouse = true;
        pThis->m_hovered = true;
        InvalidateRect( hwnd, nullptr, FALSE );
      }
      return 0;

    case WM_MOUSELEAVE:
      pThis->m_trackMouse = false;
      pThis->m_hovered = false;
      if( !pThis->m_keyDown ) pThis->m_pressed = false;
      InvalidateRect( hwnd, nullptr, FALSE );
      return 0;

      // ------------------------------------------------------------------
      // Keyboard interaction (Space / Enter activate the button)
      // ------------------------------------------------------------------
    case WM_KEYDOWN:
      if( ( wParam == VK_SPACE || wParam == VK_RETURN ) && !pThis->m_keyDown )
      {
        pThis->m_keyDown = true;
        pThis->m_pressed = true;
        InvalidateRect( hwnd, nullptr, FALSE );
        return 0;
      }
      // Forward function keys (F1-F24) to the parent (e.g. F3-F10 hotkeys in CategoryWindow).
      // Other keys (Tab, arrows, etc.) fall through to DefWindowProc / dialog manager.
      if( wParam >= VK_F1 && wParam <= VK_F24 )
      {
        HWND parent = GetParent( hwnd );
        if( parent ) SendMessage( parent, WM_KEYDOWN, wParam, lParam );
        return 0; // consumed: don't let DefWindowProc see it again
      }
      break;

    case WM_KEYUP:
      if( ( wParam == VK_SPACE || wParam == VK_RETURN ) && pThis->m_keyDown )
      {
        pThis->m_keyDown = false;
        pThis->m_pressed = false;
        InvalidateRect( hwnd, nullptr, FALSE );
        if( IsWindowEnabled( hwnd ) )
          pThis->FireClick( hwnd );
      }
      break;

      // ------------------------------------------------------------------
      // Focus
      // ------------------------------------------------------------------
    case WM_SETFOCUS:
      pThis->m_focused = true;
      InvalidateRect( hwnd, nullptr, FALSE );
      break;

    case WM_KILLFOCUS:
      pThis->m_focused = false;
      pThis->m_pressed = false;
      pThis->m_keyDown = false;
      InvalidateRect( hwnd, nullptr, FALSE );
      break;

      // ------------------------------------------------------------------
      // Dialog / keyboard integration
      // ------------------------------------------------------------------
    case WM_GETDLGCODE:
      // DLGC_BUTTON: identify as a push button to the dialog manager.
      // DLGC_WANTALLKEYS: deliver all WM_KEYDOWN messages (incl. F-keys) so
      // the WM_KEYDOWN forwarder above sees them. We deliberately do NOT
      // claim DLGC_WANTTAB so Tab still navigates between siblings.
      return DLGC_BUTTON | DLGC_WANTALLKEYS;

      // ------------------------------------------------------------------
      // Cleanup
      // ------------------------------------------------------------------
    case WM_DESTROY:
      pThis->m_hwnd = nullptr;
      break;
  }

  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
