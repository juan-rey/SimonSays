/*
   SSRadioButton.cpp: SSRadioButton, a dwell-able radio button built on SSCheckbox.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSRadioButton.h"
#include <oleacc.h> // ROLE_SYSTEM_RADIOBUTTON (the GUIDs are defined in SSCheckbox.cpp)
#include <vector>

namespace
{
  bool IsRadio( HWND h )
  {
    return ( SendMessage( h, WM_GETDLGCODE, 0, 0 ) & DLGC_RADIOBUTTON ) != 0;
  }

  bool CanTakeChoice( HWND h )
  {
    return IsWindowEnabled( h ) && ( GetWindowLongPtr( h, GWL_STYLE ) & WS_VISIBLE ) != 0;
  }

  // BTN-F90: the radio buttons of `hwnd`'s group, in Tab order. The group
  // runs from the nearest sibling with WS_GROUP at or before it to the next
  // one, as the dialog manager counts it.
  std::vector<HWND> GroupRadios( HWND hwnd )
  {
    std::vector<HWND> radios;
    if( !hwnd ) return radios;
    HWND start = hwnd;
    while( !( GetWindowLongPtr( start, GWL_STYLE ) & WS_GROUP ) )
    {
      HWND previous = GetWindow( start, GW_HWNDPREV );
      if( !previous ) break;
      start = previous;
    }
    for( HWND h = start; h; h = GetWindow( h, GW_HWNDNEXT ) )
    {
      if( h != start && ( GetWindowLongPtr( h, GWL_STYLE ) & WS_GROUP ) ) break;
      if( IsRadio( h ) ) radios.push_back( h );
    }
    return radios;
  }

  bool IsRadioType( LONG_PTR type ) { return type == BS_AUTORADIOBUTTON || type == BS_RADIOBUTTON; }
  std::unique_ptr<SSCheckbox> MakeRadio() { return std::make_unique<SSRadioButton>(); }
}

// BTN-F90: an automatic radio button unchooses the rest of its group, then
// takes the choice (in that order, so the Tab stop ends on it). Choosing the
// chosen one changes nothing; the parent still hears BN_CLICKED.
void SSRadioButton::OnBeforeClick()
{
  if( !IsAutoCheck() ) return;
  for( HWND h : GroupRadios( Hwnd() ) )
    if( h != Hwnd() && SendMessage( h, BM_GETCHECK, 0, 0 ) == BST_CHECKED )
      SendMessage( h, BM_SETCHECK, BST_UNCHECKED, 0 );
  SetCheck( true );
}

void SSRadioButton::OnCheckChanged()
{
  UpdateGroupTabStops();
}

// BTN-F91: only the chosen radio button of a group is a Tab stop, so Tab
// lands on the choice; with none chosen, the first that can take it.
void SSRadioButton::UpdateGroupTabStops()
{
  const std::vector<HWND> radios = GroupRadios( Hwnd() );
  if( radios.empty() ) return;
  HWND stop = nullptr;
  for( HWND h : radios )
    if( SendMessage( h, BM_GETCHECK, 0, 0 ) == BST_CHECKED ) { stop = h; break; }
  if( !stop )
    for( HWND h : radios )
      if( CanTakeChoice( h ) ) { stop = h; break; }
  if( !stop ) stop = radios.front();
  for( HWND h : radios )
  {
    const LONG_PTR style = GetWindowLongPtr( h, GWL_STYLE );
    const LONG_PTR wanted = h == stop ? ( style | WS_TABSTOP ) : ( style & ~(LONG_PTR) WS_TABSTOP );
    if( wanted != style ) SetWindowLongPtr( h, GWL_STYLE, wanted );
  }
}

// BTN-F91: Up and Left go to the previous radio button of the group that is
// enabled and visible, Down and Right to the next, wrapping round; the focus
// moves there and it is chosen, as with standard radio buttons.
void SSRadioButton::MoveInGroup( bool forward )
{
  const std::vector<HWND> radios = GroupRadios( Hwnd() );
  const int n = (int) radios.size();
  int self = -1;
  for( int i = 0; i < n; ++i )
    if( radios[ i ] == Hwnd() ) { self = i; break; }
  if( self < 0 || n < 2 ) return;

  HWND target = nullptr;
  for( int k = 1; k < n && !target; ++k )
  {
    HWND h = radios[ ( self + ( forward ? k : n - k ) ) % n ];
    if( CanTakeChoice( h ) ) target = h;
  }
  if( !target ) return;

  HWND parent = GetParent( Hwnd() );
  wchar_t cls[ 16 ] = {};
  if( parent && GetClassNameW( parent, cls, ARRAYSIZE( cls ) ) && wcscmp( cls, L"#32770" ) == 0 )
    SendMessage( parent, WM_NEXTDLGCTL, (WPARAM) target, TRUE );
  else
    ::SetFocus( target );
  SendMessage( target, BM_CLICK, 0, 0 ); // may destroy this window: nothing after it
}

bool SSRadioButton::HandleMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT & result )
{
  switch( uMsg )
  {
    case WM_GETDLGCODE:
      // As a standard radio button, plus SSButton's DLGC_WANTALLKEYS (its own
      // Tab, Esc and F-key handling, BTN-F74).
      result = DLGC_RADIOBUTTON | DLGC_BUTTON | DLGC_WANTALLKEYS;
      return true;

    case WM_KEYDOWN:
      if( wParam == VK_UP || wParam == VK_LEFT || wParam == VK_DOWN || wParam == VK_RIGHT )
      {
        MoveInGroup( wParam == VK_DOWN || wParam == VK_RIGHT );
        result = 0;
        return true;
      }
      break;
  }
  return SSCheckbox::HandleMessage( hwnd, uMsg, wParam, lParam, result );
}

// BTN-F89: a circle in the text colour with the window colour inside, and a
// dot when chosen. Symmetric, so a right-to-left window needs nothing more.
void SSRadioButton::DrawMark( HDC hdc, const RECT & box, int size, bool isEnabled, COLORREF textColor )
{
  const int line = max( 1, size / 13 );
  HPEN pen = CreatePen( PS_SOLID | PS_INSIDEFRAME, line, textColor );
  HBRUSH inside = CreateSolidBrush( GetSysColor( isEnabled ? COLOR_WINDOW : COLOR_BTNFACE ) );
  HPEN oldPen = (HPEN) SelectObject( hdc, pen );
  HBRUSH oldBrush = (HBRUSH) SelectObject( hdc, inside );
  Ellipse( hdc, box.left, box.top, box.right, box.bottom );

  if( GetCheck() )
  {
    const int inset = max( line + 1, MulDiv( size, 3, 10 ) );
    HBRUSH dot = CreateSolidBrush( textColor );
    SelectObject( hdc, GetStockObject( NULL_PEN ) );
    SelectObject( hdc, dot );
    // With no pen Ellipse leaves out the right and bottom edges: one more.
    Ellipse( hdc, box.left + inset, box.top + inset, box.right - inset + 1, box.bottom - inset + 1 );
    SelectObject( hdc, inside );
    DeleteObject( dot );
  }

  SelectObject( hdc, oldBrush );
  SelectObject( hdc, oldPen );
  DeleteObject( inside );
  DeleteObject( pen );
}

LONG SSRadioButton::AccessibleRole() const
{
  return ROLE_SYSTEM_RADIOBUTTON; // BTN-F89, read through BTN-F87
}

void SSRadioButton::ConvertDialogRadioButtons( HWND hDlg )
{
  // The converter calls OnCheckChanged on each once all are in place, which
  // sets the Tab stops of the converted groups (BTN-F91).
  ConvertDialogControls( hDlg, IsRadioType, MakeRadio );
}
