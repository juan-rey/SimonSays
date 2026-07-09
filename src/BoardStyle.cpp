/*
   BoardStyle.cpp: board & category style parsing and cascade resolution.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "BoardStyle.h"

// ---------------------------------------------------------------------------
// Low-level value parsing (skip-on-error: every helper returns false and
// leaves the output untouched on malformed input, per STY-F03 / STY-N01)
// ---------------------------------------------------------------------------

static void TrimWs( std::wstring & s )
{
  size_t b = s.find_first_not_of( L" \t\r\n" );
  size_t e = s.find_last_not_of( L" \t\r\n" );
  s = ( b == std::wstring::npos ) ? L"" : s.substr( b, e - b + 1 );
}

static std::wstring ToLowerTrimmed( std::wstring s )
{
  TrimWs( s );
  for( auto & c : s ) c = (wchar_t) towlower( c );
  return s;
}

// "#RRGGBB" (case-insensitive hex) -> COLORREF
static bool ParseColorValue( const std::wstring & value, COLORREF & out )
{
  if( value.size() != 7 || value[0] != L'#' ) return false;
  int rgb[3];
  for( int i = 0; i < 3; i++ )
  {
    int hi = -1, lo = -1;
    wchar_t a = value[1 + i * 2], b = value[2 + i * 2];
    if( a >= L'0' && a <= L'9' ) hi = a - L'0';
    else if( a >= L'a' && a <= L'f' ) hi = 10 + a - L'a';
    else if( a >= L'A' && a <= L'F' ) hi = 10 + a - L'A';
    if( b >= L'0' && b <= L'9' ) lo = b - L'0';
    else if( b >= L'a' && b <= L'f' ) lo = 10 + b - L'a';
    else if( b >= L'A' && b <= L'F' ) lo = 10 + b - L'A';
    if( hi < 0 || lo < 0 ) return false;
    rgb[i] = hi * 16 + lo;
  }
  out = RGB( rgb[0], rgb[1], rgb[2] );
  return true;
}

// Non-negative integer, absolute ("12") or percent-of-default ("120%").
static bool ParseSizeValue( const std::wstring & value, StyleSize & out )
{
  if( value.empty() ) return false;
  std::wstring digits = value;
  bool isPercent = false;
  if( digits.back() == L'%' )
  {
    isPercent = true;
    digits.pop_back();
    TrimWs( digits );
  }
  if( digits.empty() || digits.size() > 6 ) return false; // sanity bound
  int parsed = 0;
  for( wchar_t c : digits )
  {
    if( c < L'0' || c > L'9' ) return false;
    parsed = parsed * 10 + ( c - L'0' );
  }
  out.value = parsed;
  out.isPercent = isPercent;
  out.set = true;
  return true;
}

static bool ParseIconPosValue( const std::wstring & value, StyleIconPos & out )
{
  std::wstring v = ToLowerTrimmed( value );
  if( v == L"left" ) { out = StyleIconPos::Left;   return true; }
  if( v == L"right" ) { out = StyleIconPos::Right;  return true; }
  if( v == L"top" ) { out = StyleIconPos::Top;    return true; }
  if( v == L"bottom" ) { out = StyleIconPos::Bottom; return true; }
  return false;
}

// "text-layout": one or two space-separated keywords, order free (STY-F22).
// Both alignments are applied only if every token is recognized.
static bool ParseTextLayoutValue( const std::wstring & value, StyleHAlign & outH, StyleVAlign & outV )
{
  StyleHAlign h = StyleHAlign::NotSet;
  StyleVAlign v = StyleVAlign::NotSet;
  int tokens = 0;

  std::wistringstream stream( value );
  std::wstring token;
  while( stream >> token )
  {
    if( ++tokens > 2 ) return false;
    std::wstring t = ToLowerTrimmed( token );
    if( t == L"left" ) h = StyleHAlign::Left;
    else if( t == L"center" ) h = StyleHAlign::Center;
    else if( t == L"right" ) h = StyleHAlign::Right;
    else if( t == L"top" ) v = StyleVAlign::Top;
    else if( t == L"middle" ) v = StyleVAlign::Middle;
    else if( t == L"bottom" ) v = StyleVAlign::Bottom;
    else return false;
  }
  if( tokens == 0 ) return false;

  if( h != StyleHAlign::NotSet ) outH = h;
  if( v != StyleVAlign::NotSet ) outV = v;
  return true;
}

// ---------------------------------------------------------------------------
// Property routing
// ---------------------------------------------------------------------------

// Applies one name/value pair to a StyleProps. `windowScope` selects the
// window property set (separator-color, no geometry) versus the button-group
// set. Returns false when the property does not exist in that scope or the
// value is malformed — the caller just skips the pair.
static bool ApplyProp( StyleProps & props, const std::wstring & name, const std::wstring & value, bool windowScope )
{
  if( name == L"background" ) return ParseColorValue( value, props.background ) ? ( props.hasBackground = true ) : false;
  if( name == L"text-color" ) return ParseColorValue( value, props.textColor ) ? ( props.hasTextColor = true ) : false;
  if( name == L"font-size" ) return ParseSizeValue( value, props.fontSize );
  if( name == L"font-name" )
  {
    if( value.empty() ) return false;
    props.fontName = value;
    return true;
  }

  if( windowScope )
  {
    if( name == L"separator-color" ) return ParseColorValue( value, props.separatorColor ) ? ( props.hasSeparatorColor = true ) : false;
    return false;
  }

  if( name == L"width" ) return ParseSizeValue( value, props.width );
  if( name == L"height" ) return ParseSizeValue( value, props.height );
  if( name == L"margin" ) return ParseSizeValue( value, props.margin );
  if( name == L"corner-radius" ) return ParseSizeValue( value, props.cornerRadius );
  if( name == L"border-width" ) return ParseSizeValue( value, props.borderWidth );
  if( name == L"icon-size" ) return ParseSizeValue( value, props.iconSize );
  if( name == L"icon-position" ) return ParseIconPosValue( value, props.iconPosition );
  if( name == L"text-layout" ) return ParseTextLayoutValue( value, props.textHAlign, props.textVAlign );

  return false;
}

// Splits `list` into pairs on ';', each pair at its first ':', and hands the
// trimmed lower-case name plus trimmed raw value to `route`. Malformed pairs
// (no ':', empty name) are skipped.
template <typename RouteFn>
static void ForEachPair( const std::wstring & list, RouteFn route )
{
  size_t pos = 0;
  while( pos <= list.size() )
  {
    size_t end = list.find( L';', pos );
    if( end == std::wstring::npos ) end = list.size();
    std::wstring pair = list.substr( pos, end - pos );
    pos = end + 1;

    size_t colon = pair.find( L':' );
    if( colon == std::wstring::npos ) continue;

    std::wstring name = ToLowerTrimmed( pair.substr( 0, colon ) );
    std::wstring value = pair.substr( colon + 1 );
    TrimWs( value );
    if( name.empty() ) continue;

    route( name, value );
  }
}

static bool StripPrefix( std::wstring & name, const wchar_t * prefix, size_t prefixLen )
{
  if( name.compare( 0, prefixLen, prefix ) != 0 ) return false;
  name = name.substr( prefixLen );
  return true;
}

void ParseBoardStyleList( const std::wstring & list, BoardStyle & out )
{
  ForEachPair( list, [&]( std::wstring name, const std::wstring & value )
    {
      if( StripPrefix( name, L"category-", 9 ) )
        ApplyProp( out.categoryButtons, name, value, false );
      else if( StripPrefix( name, L"phrase-", 7 ) )
        ApplyProp( out.phraseButtons, name, value, false );
      else
        ApplyProp( out.window, name, value, true );
    } );
}

void ParseCategoryStyleList( const std::wstring & list, CategoryStyle & out )
{
  ForEachPair( list, [&]( std::wstring name, const std::wstring & value )
    {
      if( StripPrefix( name, L"phrase-", 7 ) )
      {
        ApplyProp( out.phraseButtons, name, value, false );
      }
      else if( name == L"background" || name == L"text-color" )
      {
        // Only colors are stylable on the category's own button (STY-F30).
        ApplyProp( out.ownButton, name, value, false );
      }
      // anything else: skipped (STY-F03/F30)
    } );
}

int ResolveStyleSize( const StyleSize & size, int defaultValue, bool allowZero )
{
  if( !size.set ) return defaultValue;
  int value = size.isPercent ? MulDiv( defaultValue, size.value, 100 ) : size.value;
  if( value < 0 || ( value == 0 && !allowZero ) ) return defaultValue;
  return value;
}

COLORREF ContrastTextColorFor( COLORREF background )
{
  // Rec. 601 luma; dark backgrounds get white text, light ones black.
  int luminance = ( 299 * GetRValue( background ) + 587 * GetGValue( background ) + 114 * GetBValue( background ) ) / 1000;
  return ( luminance < 128 ) ? RGB( 255, 255, 255 ) : RGB( 0, 0, 0 );
}

StyleProps ResolveEffectiveStyle( const StyleProps & base, const StyleProps & over )
{
  StyleProps r = base;

  if( over.hasBackground ) { r.background = over.background;       r.hasBackground = true; }
  if( over.hasTextColor ) { r.textColor = over.textColor;         r.hasTextColor = true; }
  if( over.hasSeparatorColor ) { r.separatorColor = over.separatorColor; r.hasSeparatorColor = true; }

  if( over.width.set ) r.width = over.width;
  if( over.height.set ) r.height = over.height;
  if( over.margin.set ) r.margin = over.margin;
  if( over.cornerRadius.set ) r.cornerRadius = over.cornerRadius;
  if( over.borderWidth.set ) r.borderWidth = over.borderWidth;
  if( over.iconSize.set ) r.iconSize = over.iconSize;
  if( over.fontSize.set ) r.fontSize = over.fontSize;

  if( !over.fontName.empty() ) r.fontName = over.fontName;

  if( over.iconPosition != StyleIconPos::NotSet ) r.iconPosition = over.iconPosition;
  if( over.textHAlign != StyleHAlign::NotSet ) r.textHAlign = over.textHAlign;
  if( over.textVAlign != StyleVAlign::NotSet ) r.textVAlign = over.textVAlign;

  return r;
}
