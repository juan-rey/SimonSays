/*
   SSButton.cpp: SSButton custom owner-drawn button class source file.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSButton.h"
#include <wingdi.h>
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
    m_text( other.m_text )
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

  if( m_hIcon )
  {
    DestroyIcon( m_hIcon );
    m_hIcon = nullptr;
  }
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
    if( m_hIcon )
    {
      DestroyIcon( m_hIcon );
      m_hIcon = nullptr;
    }

    if( config.iconType == SSButtonIconType::StandardIcon && !config.iconFileFullPath.empty() )
    {
      m_hIcon = (HICON) LoadImage( nullptr, config.iconFileFullPath.c_str(), IMAGE_ICON, // default to SSBUTTON_ICON_DEFAULT_SIZE if iconSize is 0
        ( config.iconSize > 0 ) ? config.iconSize : SSBUTTON_ICON_DEFAULT_SIZE, ( config.iconSize > 0 ) ? config.iconSize : SSBUTTON_ICON_DEFAULT_SIZE, LR_LOADFROMFILE | LR_DEFAULTCOLOR );
    }
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
    /*
    COLORREF base = ( m_config.bgType == SSButtonBackground::Color )
    ? m_config.bgColor
      : GetSysColor( COLOR_BTNFACE );
    COLORREF c = base;
    if( isEnabled )
    {
      if( m_pressed )
        c = AdjustBrightnessDelta( base, -25 );
      else if( m_hovered &&
        ( m_config.bgType == SSButtonBackground::Color || !isFlat ) )
        c = AdjustBrightnessDelta( base, 18 );
    }
    else if( m_config.bgType == SSButtonBackground::Color )
    {
      c = AdjustBrightnessDelta( base, 20 );
    }
    */
    if( hoverColor == CLR_NONE )
      m_hoverColor = AdjustBrightnessDelta( m_backgroundColor, 18 );
    else
      m_hoverColor = hoverColor;

    if( pressedColor == CLR_NONE )
      m_pressedColor = AdjustBrightnessDelta( m_backgroundColor, -25 );
    else
      m_pressedColor = pressedColor;

    if( disabledColor == CLR_NONE )
      m_disabledColor = AdjustBrightnessDelta( m_backgroundColor, 20 );
    else
      m_disabledColor = disabledColor;

    /*
        COLORREF hlColor, shColor, borderColor;
    if( !isEnabled )
    {
      hlColor = AdjustBrightnessDelta( GetSysColor( COLOR_BTNFACE ), 15 );
      shColor = GetSysColor( COLOR_GRAYTEXT );
      borderColor = GetSysColor( COLOR_GRAYTEXT );
    }
    else if( m_hovered )
    {
      hlColor = GetSysColor( COLOR_BTNHIGHLIGHT );
      shColor = GetSysColor( COLOR_HIGHLIGHT );
      borderColor = GetSysColor( COLOR_HIGHLIGHT );
    }
    else
    {
      hlColor = GetSysColor( COLOR_BTNHIGHLIGHT );
      shColor = GetSysColor( COLOR_BTNSHADOW );
      borderColor = GetSysColor( COLOR_BTNSHADOW );
    }
    */
    m_highlightBorderColor = AdjustBrightnessFactor( m_backgroundColor, 1.45f );
    m_lightBorderColor = AdjustBrightnessFactor( m_backgroundColor, 1.15f );
    m_shadowBorderColor = AdjustBrightnessFactor( m_backgroundColor, 0.75f );
    m_darkShadowBorderColor = AdjustBrightnessFactor( m_backgroundColor, 0.55f );
  }

  if( m_hwnd ) InvalidateRect( m_hwnd, nullptr, TRUE );
}


void SSButton::SetSystemColors( bool includeTextColor )
{
  m_config.bgType = SSButtonBackground::Default;
  if( includeTextColor )
  {
    m_config.textColorType = SSButtonTextColor::Default;
    m_textColor = GetSysColor( COLOR_BTNTEXT );
  }

  m_backgroundColor = GetSysColor( COLOR_BTNFACE );
  m_hoverColor = AdjustBrightnessDelta( m_backgroundColor, 18 );
  m_pressedColor = AdjustBrightnessDelta( m_backgroundColor, -25 );
  m_disabledColor = AdjustBrightnessDelta( m_backgroundColor, 20 );
  m_highlightBorderColor = GetSysColor( COLOR_BTNHIGHLIGHT );
  m_lightBorderColor = GetSysColor( COLOR_3DLIGHT );
  m_shadowBorderColor = GetSysColor( COLOR_BTNSHADOW );
  m_darkShadowBorderColor = GetSysColor( COLOR_3DDKSHADOW );

  if( m_hwnd ) InvalidateRect( m_hwnd, nullptr, TRUE );
}

void SSButton::SetIcon( const std::wstring & iconFileFullPath, int iconSize )
{
  m_config.iconType = SSButtonIconType::StandardIcon;
  m_config.iconFileFullPath = iconFileFullPath;
  m_config.iconSize = iconSize;
  if( m_hIcon )
  {
    DestroyIcon( m_hIcon );
    m_hIcon = nullptr;
  }
  m_hIcon = (HICON) LoadImage( nullptr, iconFileFullPath.c_str(), IMAGE_ICON, // default to SSBUTTON_ICON_DEFAULT_SIZE if iconSize is 0
    ( iconSize > 0 ) ? iconSize : SSBUTTON_ICON_DEFAULT_SIZE, ( iconSize > 0 ) ? iconSize : SSBUTTON_ICON_DEFAULT_SIZE, LR_LOADFROMFILE | LR_DEFAULTCOLOR );
  if( m_hwnd ) InvalidateRect( m_hwnd, nullptr, TRUE );
}

void SSButton::SetEmoji( const std::wstring & emoji, int iconSize )
{
  m_config.iconType = SSButtonIconType::Emoji;
  m_config.emoji = emoji;
  m_config.iconSize = iconSize;
  if( m_hwnd ) InvalidateRect( m_hwnd, nullptr, TRUE );
}

void SSButton::SetFont( HFONT hFont, bool redraw )
{
  m_hExternalFont = hFont;
  if( m_hwnd )
    SendMessage( m_hwnd, WM_SETFONT, (WPARAM) hFont, redraw ? TRUE : FALSE );
}

void SSButton::SetText( const std::wstring & text )
{
  if( m_hwnd ) SetWindowText( m_hwnd, text.c_str() );
  m_text = text;
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

// -------------------------------------------------------------------------
// Private helpers
// -------------------------------------------------------------------------

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
  COLORREF c = m_backgroundColor;
  if( isEnabled )
  {
    if( m_pressed )
      c = m_pressedColor;
    else if( m_hovered )
      c = m_hoverColor;
  }
  else
  {
    c = m_disabledColor;
  }

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
    if( m_config.borderStyle == SSButtonBorderStyle::SystemDefined )
    {
      RECT frameRc = rc;
      UINT flags = DFCS_BUTTONPUSH;
      if( m_pressed )   flags |= DFCS_PUSHED;
      if( !isEnabled )  flags |= DFCS_INACTIVE;
      DrawFrameControl( memDC, &frameRc, DFC_BUTTON, flags );
    }
    else if( m_config.borderStyle == SSButtonBorderStyle::Rounded )
    {
      if( isFlat )
      {
        OutlineRoundRect( m_darkShadowBorderColor, m_darkShadowBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius );
        OutlineRoundRect( m_highlightBorderColor, m_highlightBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius, -1 );
      }
      else if( !m_pressed )
      {
        OutlineRoundRect( m_highlightBorderColor, m_darkShadowBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius );
        OutlineRoundRect( m_lightBorderColor, m_shadowBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius, -1 );
      }
      else
      {
        OutlineRoundRect( m_darkShadowBorderColor, m_highlightBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius );
        OutlineRoundRect( m_shadowBorderColor, m_lightBorderColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius, -1 );
      }
    }
    else // Square
    {
      if( !isFlat )
      {
        RECT edgeRc = rc;
        DrawEdge( memDC, &edgeRc, m_pressed ? EDGE_SUNKEN : EDGE_RAISED, BF_RECT );
      }
      else
      {
        // BS_FLAT: thin 1-px border only
        HPEN   pen = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNSHADOW ) );
        HPEN   oldPen = (HPEN) SelectObject( memDC, pen );
        HBRUSH oldBr = (HBRUSH) SelectObject( memDC, GetStockObject( NULL_BRUSH ) );
        Rectangle( memDC, rc.left, rc.top, rc.right, rc.bottom );
        SelectObject( memDC, oldPen );
        SelectObject( memDC, oldBr );
        DeleteObject( pen );
      }
    }
  }

  // ------------------------------------------------------------------
  // 3. Content rect (inset from border; shifted when pressed)
  // ------------------------------------------------------------------
  RECT contentRc = { rc.left + 4, rc.top + 2, rc.right - 4, rc.bottom - 2 };
  if( m_pressed && m_config.borderStyle != SSButtonBorderStyle::Rounded )
    OffsetRect( &contentRc, 1, 1 );

  // ------------------------------------------------------------------
  // 4. Icon / emoji (rendered to the left of the label)
  // ------------------------------------------------------------------
  if( m_config.iconType == SSButtonIconType::StandardIcon && m_hIcon )
  {
    int iconSize = m_config.iconSize > 0 ? m_config.iconSize : ( contentRc.bottom - contentRc.top ) - ( 2 * m_config.iconPadding );
    int iconY = contentRc.top + ( ( contentRc.bottom - contentRc.top ) - iconSize ) / 2;
    DrawIconEx( memDC, contentRc.left, iconY,
      m_hIcon, iconSize, iconSize,
      0, nullptr, DI_NORMAL );
    contentRc.left += iconSize + m_config.iconPadding;
  }
  else if( m_config.iconType == SSButtonIconType::Emoji && !m_config.emoji.empty() )
  {
    int emojiSize = m_config.iconSize > 0 ? m_config.iconSize : ( contentRc.bottom - contentRc.top ) - ( 2 * m_config.iconPadding );
    RECT emojiRc = contentRc;
    emojiRc.right = emojiRc.left + emojiSize + m_config.iconPadding;
    DrawEmoji( memDC, emojiRc, m_config.emoji, emojiSize, isEnabled );
    contentRc.left += emojiSize + m_config.iconPadding;
  }

  // ------------------------------------------------------------------
  // 5. Label text
  // ------------------------------------------------------------------
  {
    if( !m_text.empty() )
    {
      HFONT oldFont = nullptr;
      if( m_hExternalFont ) oldFont = (HFONT) SelectObject( memDC, m_hExternalFont );

      COLORREF textColor;
      if( !isEnabled )
        textColor = GetSysColor( COLOR_GRAYTEXT );
      else if( m_config.textColorType == SSButtonTextColor::Custom )
        textColor = m_config.textColor;
      else
        textColor = GetSysColor( COLOR_BTNTEXT );

      SetTextColor( memDC, textColor );
      SetBkMode( memDC, TRANSPARENT );

      if( m_style & BS_MULTILINE )
      {
        // Measure first so we can vertically center the wrapped block
        RECT measureRc = contentRc;
        DrawText( memDC, m_text.c_str(), -1, &measureRc,
          DT_CENTER | DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX );

        int blockH = measureRc.bottom - measureRc.top;
        int availH = contentRc.bottom - contentRc.top;
        int topOffset = max( 0, ( availH - blockH ) / 2 );

        RECT drawRc = contentRc;
        drawRc.top += topOffset;
        DrawText( memDC, m_text.c_str(), -1, &drawRc,
          DT_CENTER | DT_WORDBREAK | DT_NOPREFIX );
      }
      else
      {
        DrawText( memDC, m_text.c_str(), -1, &contentRc,
          DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX );
      }

      if( oldFont ) SelectObject( memDC, oldFont );
    }
  }

  // ------------------------------------------------------------------
  // 6. Focus rectangle
  // ------------------------------------------------------------------
  if( m_focused && isEnabled )
  {
    if( m_config.borderStyle != SSButtonBorderStyle::Rounded )
    {
      RECT focusRc = { rc.left + 3, rc.top + 3, rc.right - 3, rc.bottom - 3 };
      DrawFocusRect( memDC, &focusRc );
    }
    else
    {
      OutlineRoundRect( m_textColor, m_textColor, rc.left, rc.top, rc.right, rc.bottom, m_config.cornerRadius, -3, PS_DOT );
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
      pThis->m_text = (LPCWSTR) lParam;
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
      // Forward all other keys to the parent (e.g. F3-F10 hotkeys in CategoryWindow)
      {
        HWND parent = GetParent( hwnd );
        if( parent ) SendMessage( parent, WM_KEYDOWN, wParam, lParam );
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
      return DLGC_BUTTON | DLGC_WANTCHARS;

      // ------------------------------------------------------------------
      // Cleanup
      // ------------------------------------------------------------------
    case WM_DESTROY:
      pThis->m_hwnd = nullptr;
      break;
  }

  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
