/*
   MessageDialog.cpp: SimonSays message box (10 pt, dwell-enabled buttons).

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays - Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "MessageDialog.h"
#include "SSButton.h"
#include "utils.h"
#include "resource.h"
#include <array>

namespace
{
  // Dwell starts counting only this long after the box opens (LOC-F32): the box
  // appears mid-screen, possibly right where the user is already looking.
  constexpr UINT MESSAGE_DIALOG_DWELL_ARM_MS = 750;
  constexpr UINT_PTR TIMER_ARM_DWELL = 1;

  // Layout, in dialog units of the 10 pt template font.
  constexpr int MARGIN_DLU = 10;
  constexpr int ICON_TEXT_GAP_DLU = 10;
  constexpr int MAX_TEXT_WIDTH_DLU = 300;
  constexpr int MIN_CLIENT_WIDTH_DLU = 150;
  constexpr int TEXT_BUTTONS_GAP_DLU = 14;
  constexpr int BUTTON_HEIGHT_DLU = 22;      // bigger than the usual 14: easier gaze target
  constexpr int BUTTON_MIN_WIDTH_DLU = 64;
  constexpr int BUTTON_TEXT_PADDING_DLU = 12; // each side
  constexpr int BUTTON_GAP_DLU = 8;

  constexpr int IDC_MESSAGE_ICON = 100;
  constexpr int IDC_MESSAGE_TEXT = 101;

  struct MessageDialogContext
  {
    const wchar_t * text = L"";
    const wchar_t * caption = L"";
    UINT flags = 0;
    std::wstring language;
    std::array<int, 3> buttonIds = { 0, 0, 0 }; // left to right (mirrored in RTL)
    int buttonCount = 0;
    int defaultIndex = 0;
    int escapeResult = 0;                        // 0 = Esc and the close button do nothing
    std::array<SSButton, 3> buttons;             // must outlive the dialog HWND
  };

  // Fills the button set from MB_TYPEMASK; false for a set this dialog does not offer.
  bool ResolveButtons( MessageDialogContext & ctx )
  {
    switch( ctx.flags & MB_TYPEMASK )
    {
      case MB_OK:          ctx.buttonIds = { IDOK, 0, 0 };            ctx.buttonCount = 1; ctx.escapeResult = IDOK;     break;
      case MB_OKCANCEL:    ctx.buttonIds = { IDOK, IDCANCEL, 0 };     ctx.buttonCount = 2; ctx.escapeResult = IDCANCEL; break;
      case MB_YESNO:       ctx.buttonIds = { IDYES, IDNO, 0 };        ctx.buttonCount = 2; ctx.escapeResult = 0;        break;
      case MB_YESNOCANCEL: ctx.buttonIds = { IDYES, IDNO, IDCANCEL }; ctx.buttonCount = 3; ctx.escapeResult = IDCANCEL; break;
      default: return false;
    }
    int def = 0;
    switch( ctx.flags & MB_DEFMASK )
    {
      case MB_DEFBUTTON2: def = 1; break;
      case MB_DEFBUTTON3: def = 2; break;
    }
    ctx.defaultIndex = ( def < ctx.buttonCount ) ? def : 0;
    return true;
  }

  const wchar_t * ButtonLabel( int id, const std::wstring & language )
  {
    switch( id )
    {
      case IDOK:     return GetLocalizedString( SETTINGS_OK_BUTTON_ID, language );
      case IDCANCEL: return GetLocalizedString( SETTINGS_CANCEL_BUTTON_ID, language );
      case IDYES:    return GetLocalizedString( MSGBOX_YES_BUTTON_ID, language );
      case IDNO:     return GetLocalizedString( MSGBOX_NO_BUTTON_ID, language );
    }
    return L"";
  }

  LPCWSTR SystemIconFor( UINT flags )
  {
    switch( flags & MB_ICONMASK )
    {
      case MB_ICONERROR:       return IDI_ERROR;
      case MB_ICONWARNING:     return IDI_WARNING;
      case MB_ICONINFORMATION: return IDI_INFORMATION;
      case MB_ICONQUESTION:    return IDI_QUESTION;
    }
    return nullptr;
  }

  int DluX( HWND hDlg, int dlu ) { RECT r = { 0, 0, dlu, 0 }; MapDialogRect( hDlg, &r ); return r.right; }
  int DluY( HWND hDlg, int dlu ) { RECT r = { 0, 0, 0, dlu }; MapDialogRect( hDlg, &r ); return r.bottom; }

  SIZE MeasureText( HWND hDlg, HFONT font, const wchar_t * text, int maxWidth, UINT format )
  {
    RECT r = { 0, 0, maxWidth, 0 };
    HDC dc = GetDC( hDlg );
    HGDIOBJ old = SelectObject( dc, font );
    DrawText( dc, text, -1, &r, DT_CALCRECT | DT_NOPREFIX | format );
    SelectObject( dc, old );
    ReleaseDC( hDlg, dc );
    return { r.right - r.left, r.bottom - r.top };
  }

  // Lays the controls out for the measured text and sizes the dialog, centered
  // on the work area of the owner's monitor (as MessageBox does).
  void LayOut( HWND hDlg, MessageDialogContext & ctx, HFONT font, HWND hIcon, HWND hText )
  {
    const int margin = DluX( hDlg, MARGIN_DLU );
    const int marginY = DluY( hDlg, MARGIN_DLU );
    const int iconW = hIcon ? GetSystemMetrics( SM_CXICON ) : 0;
    const int iconH = hIcon ? GetSystemMetrics( SM_CYICON ) : 0;
    const int iconGap = hIcon ? DluX( hDlg, ICON_TEXT_GAP_DLU ) : 0;

    SIZE text = MeasureText( hDlg, font, ctx.text, DluX( hDlg, MAX_TEXT_WIDTH_DLU ),
      DT_WORDBREAK | DT_EXPANDTABS | DT_EDITCONTROL );

    // All buttons share the width of the widest label.
    int buttonW = DluX( hDlg, BUTTON_MIN_WIDTH_DLU );
    for( int i = 0; i < ctx.buttonCount; ++i )
    {
      SIZE label = MeasureText( hDlg, font, ButtonLabel( ctx.buttonIds[i], ctx.language ), 0, DT_SINGLELINE );
      buttonW = max( buttonW, (int) label.cx + 2 * DluX( hDlg, BUTTON_TEXT_PADDING_DLU ) );
    }
    const int buttonH = DluY( hDlg, BUTTON_HEIGHT_DLU );
    const int buttonGap = DluX( hDlg, BUTTON_GAP_DLU );
    const int buttonsW = ctx.buttonCount * buttonW + ( ctx.buttonCount - 1 ) * buttonGap;

    const int contentW = iconW + iconGap + text.cx;
    const int clientW = max( max( contentW, buttonsW ) + 2 * margin, DluX( hDlg, MIN_CLIENT_WIDTH_DLU ) );
    const int topH = max( iconH, (int) text.cy );
    const int buttonsY = marginY + topH + DluY( hDlg, TEXT_BUTTONS_GAP_DLU );
    int clientH = buttonsY + buttonH + marginY;

    // Frame size for this client area, kept inside the work area.
    MONITORINFO mi = { sizeof( mi ) };
    HWND owner = GetWindow( hDlg, GW_OWNER );
    HMONITOR mon = owner ? MonitorFromWindow( owner, MONITOR_DEFAULTTONEAREST ) : nullptr;
    if( !mon )
    {
      POINT pt; GetCursorPos( &pt );
      mon = MonitorFromPoint( pt, MONITOR_DEFAULTTONEAREST );
    }
    GetMonitorInfo( mon, &mi );
    const RECT & work = mi.rcWork;

    RECT frame = { 0, 0, clientW, clientH };
    AdjustWindowRectEx( &frame, (DWORD) GetWindowLongPtr( hDlg, GWL_STYLE ), FALSE, (DWORD) GetWindowLongPtr( hDlg, GWL_EXSTYLE ) );
    int frameW = frame.right - frame.left;
    int frameH = frame.bottom - frame.top;
    const int workH = work.bottom - work.top;
    int textH = text.cy;
    if( frameH > workH ) // too tall: the text is clipped (no current message is this long)
    {
      const int excess = frameH - workH;
      textH = max( 0, textH - excess );
      clientH -= excess;
      frameH = workH;
    }
    const int x = work.left + ( ( work.right - work.left ) - frameW ) / 2;
    const int y = work.top + ( workH - frameH ) / 2;
    SetWindowPos( hDlg, nullptr, x, y, frameW, frameH, SWP_NOZORDER | SWP_NOACTIVATE );

    // Children: coordinates are left-to-right; a mirrored dialog flips them.
    int left = margin;
    if( hIcon )
    {
      MoveWindow( hIcon, left, marginY, iconW, iconH, FALSE );
      left += iconW + iconGap;
    }
    const int textTop = marginY + ( ( iconH > textH ) ? ( iconH - textH ) / 2 : 0 );
    MoveWindow( hText, left, textTop, clientW - left - margin, textH, FALSE );

    // Buttons bottom-right, like MessageBox; buttonsY moves up if the text was clipped.
    const int btnY = clientH - marginY - buttonH;
    int bx = clientW - margin - buttonsW;
    for( int i = 0; i < ctx.buttonCount; ++i )
    {
      ctx.buttons[i].SetPos( bx, btnY, buttonW, buttonH );
      bx += buttonW + buttonGap;
    }
  }

  INT_PTR CALLBACK MessageDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
  {
    MessageDialogContext * ctx = reinterpret_cast<MessageDialogContext *>( GetWindowLongPtr( hDlg, GWLP_USERDATA ) );
    switch( message )
    {
      case WM_INITDIALOG:
      {
        ctx = reinterpret_cast<MessageDialogContext *>( lParam );
        SetWindowLongPtr( hDlg, GWLP_USERDATA, (LONG_PTR) ctx );
        const bool isRtl = IsLanguageRTL( ctx->language );
        // Mirror before creating the children so their positions mirror too.
        updateRtlExStyle( hDlg, isRtl );
        SetWindowText( hDlg, ctx->caption );

        HINSTANCE hInst = (HINSTANCE) GetWindowLongPtr( hDlg, GWLP_HINSTANCE );
        HFONT font = (HFONT) SendMessage( hDlg, WM_GETFONT, 0, 0 );

        HWND hIcon = nullptr;
        if( LPCWSTR iconId = SystemIconFor( ctx->flags ) )
        {
          hIcon = CreateWindowEx( 0, L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | SS_ICON,
            0, 0, 0, 0, hDlg, (HMENU) (INT_PTR) IDC_MESSAGE_ICON, hInst, nullptr );
          SendMessage( hIcon, STM_SETICON, (WPARAM) LoadIcon( nullptr, iconId ), 0 );
        }
        HWND hText = CreateWindowEx( 0, L"STATIC", ctx->text, WS_CHILD | WS_VISIBLE | SS_NOPREFIX | SS_EDITCONTROL,
          0, 0, 0, 0, hDlg, (HMENU) (INT_PTR) IDC_MESSAGE_TEXT, hInst, nullptr );
        SendMessage( hText, WM_SETFONT, (WPARAM) font, FALSE );

        SSButtonConfig cfg;
        cfg.borderStyle = SSButtonBorderStyle::Rounded;
        cfg.cornerRadius = 8;
        cfg.borderWidth = 2;
        for( int i = 0; i < ctx->buttonCount; ++i )
        {
          ctx->buttons[i].Create( hDlg, hInst, ctx->buttonIds[i], ButtonLabel( ctx->buttonIds[i], ctx->language ),
            0, 0, 10, 10, WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CENTER | BS_VCENTER, 0, cfg );
          if( font ) ctx->buttons[i].SetFont( font );
          ctx->buttons[i].SetDwellEnabled( false ); // armed by TIMER_ARM_DWELL
        }
        EnumChildWindows( hDlg, ApplyRtlStylesCallback, isRtl ? 1 : 0 );

        LayOut( hDlg, *ctx, font, hIcon, hText );

        if( ctx->escapeResult == 0 ) // Yes/No: no Esc, no close button (as MessageBox)
          EnableMenuItem( GetSystemMenu( hDlg, FALSE ), SC_CLOSE, MF_BYCOMMAND | MF_GRAYED );

        SetTimer( hDlg, TIMER_ARM_DWELL, MESSAGE_DIALOG_DWELL_ARM_MS, nullptr );
        MessageBeep( ctx->flags & MB_ICONMASK );
        if( ctx->flags & MB_SETFOREGROUND )
          SetForegroundWindow( hDlg );
        ctx->buttons[ctx->defaultIndex].SetFocus();
        return FALSE; // focus set by hand
      }

      case WM_TIMER:
        if( wParam == TIMER_ARM_DWELL && ctx )
        {
          KillTimer( hDlg, TIMER_ARM_DWELL );
          for( int i = 0; i < ctx->buttonCount; ++i )
            ctx->buttons[i].SetDwellEnabled( true );
          return TRUE;
        }
        break;

      case WM_COMMAND:
      {
        if( !ctx ) break;
        const int id = LOWORD( wParam );
        // Esc (dialog manager, or an SSButton forwarding it) and the close button.
        if( id == IDCANCEL && lParam == 0 )
        {
          if( ctx->escapeResult != 0 )
          {
            KillTimer( hDlg, TIMER_ARM_DWELL );
            EndDialog( hDlg, ctx->escapeResult );
          }
          return TRUE;
        }
        for( int i = 0; i < ctx->buttonCount; ++i )
        {
          if( ctx->buttonIds[i] == id )
          {
            KillTimer( hDlg, TIMER_ARM_DWELL );
            EndDialog( hDlg, id );
            return TRUE;
          }
        }
        break;
      }
    }
    return FALSE;
  }
}

int ShowMessageDialog( HWND owner, const wchar_t * text, const wchar_t * caption,
  UINT flags, const std::wstring & language )
{
  MessageDialogContext ctx;
  ctx.text = text ? text : L"";
  ctx.caption = caption ? caption : L"";
  ctx.flags = flags;
  ctx.language = language;
  if( !ResolveButtons( ctx ) )
    return MessageBoxEx( owner, ctx.text, ctx.caption, GetMessageBoxFlagsForLanguage( language, flags ), GetLangIdFromLanguageString( language ) );

  INT_PTR result = DialogBoxParam( GetModuleHandle( nullptr ), MAKEINTRESOURCE( IDD_MESSAGE_DIALOG ), owner,
    MessageDialogProc, (LPARAM) &ctx );
  if( result <= 0 ) // the template failed to load: never lose the question
    return MessageBoxEx( owner, ctx.text, ctx.caption, GetMessageBoxFlagsForLanguage( language, flags ), GetLangIdFromLanguageString( language ) );
  return (int) result;
}
