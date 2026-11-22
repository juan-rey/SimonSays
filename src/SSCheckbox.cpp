/*
   SSCheckbox.cpp: SSCheckbox, a dwell-able checkbox built on SSButton.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSCheckbox.h"
#include <initguid.h> // defines the GUIDs below in this file (selectany)
#include <oleacc.h>   // IAccPropServices, CLSID_AccPropServices, PROPID_ACC_* (BTN-F87)
#include <commctrl.h> // SetWindowSubclass (BTN-F88 owner)
#include <vector>
#pragma comment(lib, "oleacc.lib")

SSCheckbox::~SSCheckbox()
{
  // The HWND goes in SSButton's destructor; its WM_DESTROY clears the
  // accessibility annotation while this part still exists.
  if( Hwnd() && IsWindow( Hwnd() ) )
    DestroyWindow( Hwnd() );
}

HWND SSCheckbox::Create( HWND hwndParent, HINSTANCE hInstance, int id,
  const std::wstring & text, int x, int y, int w, int h,
  DWORD style, DWORD exStyle, bool autoCheck )
{
  m_autoCheck = autoCheck;
  // BTN-F84: no border; the label at the start, centred vertically, unless
  // the caller asked for another alignment.
  if( ( style & ( BS_LEFT | BS_RIGHT | BS_CENTER ) ) == 0 ) style |= BS_LEFT;
  if( ( style & ( BS_TOP | BS_BOTTOM | BS_VCENTER ) ) == 0 ) style |= BS_VCENTER;
  SSButtonConfig cfg;
  cfg.borderWidth = 0;
  HWND hwnd = SSButton::Create( hwndParent, hInstance, id, text, x, y, w, h, style, exStyle, cfg );
  if( hwnd )
    UpdateAccessibility();
  return hwnd;
}

void SSCheckbox::SetCheck( bool checked )
{
  if( checked == m_checked ) return;
  m_checked = checked;
  if( Hwnd() )
  {
    InvalidateRect( Hwnd(), nullptr, FALSE );
    UpdateAccessibility();
    NotifyWinEvent( EVENT_OBJECT_STATECHANGE, Hwnd(), OBJID_CLIENT, CHILDID_SELF );
  }
  OnCheckChanged();
}

// BTN-F92: a control the user must not reach by gaze stays off through the
// dialog's arming (DIALOG_CHECKBOXES_ARM_TIMER_ID).
void SSCheckbox::SetDwellAllowed( bool allowed )
{
  m_dwellAllowed = allowed;
  if( !allowed )
    SetDwellEnabled( false );
}

// BTN-F85: BS_AUTOCHECKBOX flips its state before the parent hears of it.
void SSCheckbox::OnBeforeClick()
{
  if( m_autoCheck )
    SetCheck( !m_checked );
}

bool SSCheckbox::HandleMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT & result )
{
  switch( uMsg )
  {
    // BTN-F85: the standard checkbox messages.
    case BM_GETCHECK:
      result = m_checked ? BST_CHECKED : BST_UNCHECKED;
      return true;
    case BM_SETCHECK:
      SetCheck( wParam == BST_CHECKED );
      result = 0;
      return true;
    case BM_CLICK:
      if( IsWindowEnabled( hwnd ) )
        Activate( SSButtonActivation::Mouse ); // as a click; the handler may destroy the checkbox
      result = 0;
      return true;

    // BTN-F86: Enter is the dialog's default button, as for a standard
    // checkbox; only Space toggles it.
    case WM_KEYDOWN:
      if( wParam == VK_RETURN )
      {
        HWND parent = GetParent( hwnd );
        if( parent )
        {
          const LRESULT def = SendMessage( parent, DM_GETDEFID, 0, 0 );
          const int id = HIWORD( def ) == DC_HASDEFID ? LOWORD( def ) : IDOK;
          SendMessage( parent, WM_COMMAND, MAKEWPARAM( id, BN_CLICKED ), (LPARAM) GetDlgItem( parent, id ) );
        }
        result = 0;
        return true;
      }
      break;
    case WM_KEYUP:
      if( wParam == VK_RETURN )
      {
        result = 0;
        return true;
      }
      break;

    // BTN-F87: the state screen readers see follows focus and enabling
    // (both already changed when these arrive).
    case WM_SETFOCUS:
    case WM_KILLFOCUS:
    case WM_ENABLE:
      UpdateAccessibility();
      break;

    case WM_DESTROY:
      UpdateAccessibility( true );
      break;
  }
  return false; // SSButton handles it too
}

// BTN-F84: a square as tall as the label's capitals, at the start of the
// content, then a gap; the label gets the rest. DrawMark fills it.
void SSCheckbox::PaintMark( HDC hdc, RECT & contentRc, bool isEnabled, COLORREF textColor )
{
  HFONT font = Hwnd() ? (HFONT) SendMessage( Hwnd(), WM_GETFONT, 0, 0 ) : nullptr;
  HFONT oldFont = font ? (HFONT) SelectObject( hdc, font ) : nullptr;
  TEXTMETRIC tm = {};
  GetTextMetrics( hdc, &tm );
  if( oldFont ) SelectObject( hdc, oldFont );

  const int height = contentRc.bottom - contentRc.top;
  int size = max( 9, MulDiv( tm.tmHeight, 4, 5 ) );
  size = min( size, max( 0, height - 2 ) );
  if( size <= 0 ) return;

  RECT box;
  box.left = contentRc.left + 1;
  box.top = contentRc.top + ( height - size ) / 2;
  box.right = box.left + size;
  box.bottom = box.top + size;

  DrawMark( hdc, box, size, isEnabled, textColor );

  contentRc.left = box.right + max( 4, size / 2 );
}

// BTN-F84: the box, the window colour inside and the text colour around, with
// a tick when checked. The tick keeps its shape under a mirrored
// (right-to-left) DC.
void SSCheckbox::DrawMark( HDC hdc, const RECT & box, int size, bool isEnabled, COLORREF textColor )
{
  const int line = max( 1, size / 13 );
  HBRUSH inside = CreateSolidBrush( GetSysColor( isEnabled ? COLOR_WINDOW : COLOR_BTNFACE ) );
  FillRect( hdc, &box, inside );
  DeleteObject( inside );
  HBRUSH frame = CreateSolidBrush( textColor );
  for( int i = 0; i < line; ++i )
  {
    RECT r = { box.left + i, box.top + i, box.right - i, box.bottom - i };
    FrameRect( hdc, &r, frame );
  }
  DeleteObject( frame );

  if( m_checked )
  {
    // A tick: from the left middle down to the bottom third, then up to the
    // top right. A right-to-left window shows what is drawn here mirrored
    // (whether this DC is mirrored or the copy to the screen flips it), so
    // the x coordinates are flipped inside the box and the tick still reads
    // left to right, as Windows draws it.
    const bool mirrored = Hwnd() && ( GetWindowLongPtr( Hwnd(), GWL_EXSTYLE ) & WS_EX_LAYOUTRTL ) != 0;
    auto X = [ & ]( int numerator ) { const int x = box.left + MulDiv( size, numerator, 100 ); return mirrored ? box.left + box.right - x : x; };
    auto Y = [ & ]( int numerator ) { return box.top + MulDiv( size, numerator, 100 ); };
    POINT tick[ 3 ] = { { X( 22 ), Y( 52 ) }, { X( 42 ), Y( 72 ) }, { X( 78 ), Y( 30 ) } };
    LOGBRUSH lb = { BS_SOLID, textColor, 0 };
    HPEN pen = ExtCreatePen( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, max( 2, size / 7 ), &lb, 0, nullptr );
    HPEN oldPen = (HPEN) SelectObject( hdc, pen );
    Polyline( hdc, tick, 3 );
    SelectObject( hdc, oldPen );
    DeleteObject( pen );
  }
}

LONG SSCheckbox::AccessibleRole() const
{
  return ROLE_SYSTEM_CHECKBUTTON;
}

// BTN-F87: Windows' annotation service tells screen readers this is a
// checkbox (or, through AccessibleRole, a radio button) and its state;
// without COM on the thread it stays a button.
void SSCheckbox::UpdateAccessibility( bool clear )
{
  HWND hwnd = Hwnd();
  if( !hwnd ) return;
  IAccPropServices * services = nullptr;
  if( FAILED( CoCreateInstance( CLSID_AccPropServices, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS( &services ) ) ) || !services )
    return;
  if( clear )
  {
    MSAAPROPID props[] = { PROPID_ACC_ROLE, PROPID_ACC_STATE };
    services->ClearHwndProps( hwnd, OBJID_CLIENT, CHILDID_SELF, props, ARRAYSIZE( props ) );
  }
  else
  {
    VARIANT v;
    VariantInit( &v );
    v.vt = VT_I4;
    v.lVal = AccessibleRole();
    services->SetHwndProp( hwnd, OBJID_CLIENT, CHILDID_SELF, PROPID_ACC_ROLE, v );
    LONG state = STATE_SYSTEM_FOCUSABLE;
    if( m_checked ) state |= STATE_SYSTEM_CHECKED;
    if( !IsWindowEnabled( hwnd ) ) state |= STATE_SYSTEM_UNAVAILABLE;
    if( GetFocus() == hwnd ) state |= STATE_SYSTEM_FOCUSED;
    v.lVal = state;
    services->SetHwndProp( hwnd, OBJID_CLIENT, CHILDID_SELF, PROPID_ACC_STATE, v );
  }
  services->Release();
}

// -------------------------------------------------------------------------
// BTN-F88, BTN-F93: dialog checkboxes and radio buttons -> SSCheckboxes and
// SSRadioButtons, owned by the dialog
// -------------------------------------------------------------------------
namespace
{
  constexpr UINT_PTR DIALOG_CHECKBOXES_SUBCLASS_ID = 0x5B88;
  constexpr UINT_PTR DIALOG_CHECKBOXES_ARM_TIMER_ID = 0x5B88; // unlikely to clash with a host's timers

  struct DialogCheckboxes
  {
    std::vector<std::unique_ptr<SSCheckbox>> boxes; // unique_ptr: an SSCheckbox must not move once created
  };

  // Owns the converted controls for the dialog's lifetime and arms their dwell.
  LRESULT CALLBACK DialogCheckboxesSubclassProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam,
    UINT_PTR subclassId, DWORD_PTR refData )
  {
    DialogCheckboxes * owned = reinterpret_cast<DialogCheckboxes *>( refData );
    switch( msg )
    {
      case WM_TIMER:
        if( wParam == DIALOG_CHECKBOXES_ARM_TIMER_ID )
        {
          KillTimer( hDlg, DIALOG_CHECKBOXES_ARM_TIMER_ID );
          for( auto & b : owned->boxes )
            if( b->IsDwellAllowed() ) // BTN-F92
              b->SetDwellEnabled( true );
          return 0;
        }
        break;

      case WM_NCDESTROY:
        // The child HWNDs are already gone; the destructors cope.
        KillTimer( hDlg, DIALOG_CHECKBOXES_ARM_TIMER_ID );
        RemoveWindowSubclass( hDlg, DialogCheckboxesSubclassProc, subclassId );
        delete owned;
        break;
    }
    return DefSubclassProc( hDlg, msg, wParam, lParam );
  }

  // The dialog's owner: the one an earlier conversion installed, or a new one.
  DialogCheckboxes * DialogOwner( HWND hDlg, bool create )
  {
    DWORD_PTR ref = 0;
    if( GetWindowSubclass( hDlg, DialogCheckboxesSubclassProc, DIALOG_CHECKBOXES_SUBCLASS_ID, &ref ) && ref )
      return reinterpret_cast<DialogCheckboxes *>( ref );
    if( !create ) return nullptr;
    auto * owned = new DialogCheckboxes();
    if( !SetWindowSubclass( hDlg, DialogCheckboxesSubclassProc, DIALOG_CHECKBOXES_SUBCLASS_ID, (DWORD_PTR) owned ) )
    {
      delete owned;
      return nullptr;
    }
    return owned;
  }

  bool IsCheckboxType( LONG_PTR type ) { return type == BS_AUTOCHECKBOX || type == BS_CHECKBOX; }
  std::unique_ptr<SSCheckbox> MakeCheckbox() { return std::make_unique<SSCheckbox>(); }
}

void SSCheckbox::ConvertDialogCheckboxes( HWND hDlg )
{
  ConvertDialogControls( hDlg, IsCheckboxType, MakeCheckbox );
}

void SSCheckbox::ConvertDialogControls( HWND hDlg, bool ( *matches )( LONG_PTR type ),
  std::unique_ptr<SSCheckbox> ( *make )() )
{
  if( !hDlg ) return;

  std::vector<HWND> found;
  for( HWND h = GetWindow( hDlg, GW_CHILD ); h; h = GetWindow( h, GW_HWNDNEXT ) )
  {
    wchar_t cls[ 16 ] = {};
    if( !GetClassNameW( h, cls, ARRAYSIZE( cls ) ) || _wcsicmp( cls, L"Button" ) != 0 )
      continue;
    if( matches( GetWindowLongPtr( h, GWL_STYLE ) & BS_TYPEMASK ) )
      found.push_back( h );
  }
  if( found.empty() ) return;

  // The owner first: without it nothing is converted (the standard controls stay).
  DialogCheckboxes * owned = DialogOwner( hDlg, true );
  if( !owned ) return;
  HINSTANCE hInst = (HINSTANCE) GetWindowLongPtr( hDlg, GWLP_HINSTANCE );

  std::vector<SSCheckbox *> added;
  for( HWND old : found )
  {
    const int id = GetDlgCtrlID( old );
    const LONG_PTR style = GetWindowLongPtr( old, GWL_STYLE );
    const LONG_PTR exStyle = GetWindowLongPtr( old, GWL_EXSTYLE );
    const LONG_PTR type = style & BS_TYPEMASK;

    // Whole-rect mapping so a mirrored dialog gives the right left edge.
    RECT rc;
    ::GetWindowRect( old, &rc );
    MapWindowPoints( HWND_DESKTOP, hDlg, reinterpret_cast<POINT *>( &rc ), 2 );
    const int x = min( rc.left, rc.right ), w = abs( rc.right - rc.left );

    wchar_t text[ SSBUTTON_MAX_TEXT_LENGTH ] = {};
    GetWindowTextW( old, text, ARRAYSIZE( text ) );
    HFONT font = (HFONT) SendMessage( old, WM_GETFONT, 0, 0 );
    const bool checked = SendMessage( old, BM_GETCHECK, 0, 0 ) == BST_CHECKED;

    DWORD newStyle = WS_CHILD | BS_LEFT | BS_VCENTER;
    newStyle |= (DWORD) ( style & ( WS_VISIBLE | WS_TABSTOP | WS_GROUP | WS_DISABLED | BS_MULTILINE ) );

    std::unique_ptr<SSCheckbox> box = make();
    HWND created = box->Create( hDlg, hInst, id, text, x, rc.top, w, rc.bottom - rc.top,
      newStyle, (DWORD) ( exStyle & ( WS_EX_LAYOUTRTL | WS_EX_RTLREADING ) ),
      type == BS_AUTOCHECKBOX || type == BS_AUTORADIOBUTTON );
    if( !created ) continue; // keep the standard control rather than lose it
    if( font ) box->SetFont( font, false );
    box->SetDwellEnabled( false ); // armed by DIALOG_CHECKBOXES_ARM_TIMER_ID

    // Same Tab position: insert right after the original, then remove it.
    SetWindowPos( created, old, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
    DestroyWindow( old );
    box->SetCheck( checked ); // after the original is gone: a radio button looks at its group

    added.push_back( box.get() );
    owned->boxes.push_back( std::move( box ) );
  }

  // Each state was set while later originals still existed: let each control
  // look at the finished dialog once (an SSRadioButton places its group's Tab
  // stop, BTN-F91).
  for( SSCheckbox * b : added )
    b->OnCheckChanged();

  if( !added.empty() )
    SetTimer( hDlg, DIALOG_CHECKBOXES_ARM_TIMER_ID, SSBUTTON_DIALOG_DWELL_ARM_MS, nullptr );
  // (An empty owner is still freed by the subclass on WM_NCDESTROY.)
}

bool SSCheckbox::AllowDialogItemDwell( HWND hDlg, int id, bool allowed )
{
  DialogCheckboxes * owned = hDlg ? DialogOwner( hDlg, false ) : nullptr;
  if( !owned ) return false;
  HWND item = GetDlgItem( hDlg, id );
  for( auto & b : owned->boxes )
  {
    if( item && b->Hwnd() == item )
    {
      b->SetDwellAllowed( allowed );
      return true;
    }
  }
  return false;
}
