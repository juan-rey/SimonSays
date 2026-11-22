/*
   SSCheckbox.h: SSCheckbox, a dwell-able checkbox built on SSButton.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSCheckbox_h
#define SSCheckbox_h

#include "SSButton.h"
#include <memory>

// A checkbox that gaze dwell can press (ssbutton.spec.md §6.11, BTN-F84 to
// BTN-F88). An SSButton with no border whose whole rect is the target: a
// square box, ticked when checked, then the label. Right-to-left through
// WS_EX_LAYOUTRTL, like SSButton. It answers BM_GETCHECK, BM_SETCHECK and
// BM_CLICK, so IsDlgButtonChecked and CheckDlgButton work on it, and it tells
// screen readers it is a checkbox and whether it is checked. SSRadioButton
// derives from it (§6.12).
class SSCheckbox : public SSButton
{
public:
  SSCheckbox() = default;
  ~SSCheckbox() override;
  // Kept by pointer: the HWND points back at this address.
  SSCheckbox( SSCheckbox && ) = delete;
  SSCheckbox( const SSCheckbox & ) = delete;
  SSCheckbox & operator=( const SSCheckbox & ) = delete;

  // As SSButton::Create, with no border and the label at the start (BS_LEFT,
  // BS_VCENTER unless other BS_ alignment is given). With BS_PUSHLIKE it keeps
  // SSButton's border and background, the mark and label inside (BTN-F94).
  // `autoCheck`: activation flips the state, as BS_AUTOCHECKBOX does; false
  // leaves it to the parent, as BS_CHECKBOX does.
  HWND Create( HWND hwndParent, HINSTANCE hInstance, int id,
    const std::wstring & text,
    int x, int y, int w, int h,
    DWORD style, DWORD exStyle = 0, bool autoCheck = true );

  void SetCheck( bool checked );
  bool GetCheck() const { return m_checked; }

  // BTN-F92: false keeps dwell off for good, through the dialog's delayed
  // arming too; the mouse and the keyboard still work. True only lifts the
  // ban: dwell comes back with SetDwellEnabled or the dialog's arming.
  void SetDwellAllowed( bool allowed );
  bool IsDwellAllowed() const { return m_dwellAllowed; }

  // BTN-F88: replaces every BS_AUTOCHECKBOX and BS_CHECKBOX of a dialog by an
  // SSCheckbox with the same id, rect, text, font, state, enabled/visible
  // state, BS_PUSHLIKE and Tab position. Dwell arms SSBUTTON_DIALOG_DWELL_ARM_MS after the
  // call. The dialog owns them (freed on WM_NCDESTROY). Call in WM_INITDIALOG
  // before the checkboxes are set, tooltips attached or handles kept.
  static void ConvertDialogCheckboxes( HWND hDlg );

  // BTN-F92: SetDwellAllowed on a control a dialog converter made (checkbox or
  // radio button). False when `id` is not one of them.
  static bool AllowDialogItemDwell( HWND hDlg, int id, bool allowed );

protected:
  bool HandleMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT & result ) override;
  void OnBeforeClick() override;
  void PaintMark( HDC hdc, RECT & contentRc, bool isEnabled, COLORREF textColor ) override;
  bool AlwaysRearm() const override { return true; } // BTN-F86: one look, one change

  // BTN-F89 hooks for SSRadioButton.
  virtual LONG AccessibleRole() const;   // ROLE_SYSTEM_CHECKBUTTON here
  // Draws the mark in `box` (a square `size` px wide, already placed).
  virtual void DrawMark( HDC hdc, const RECT & box, int size, bool isEnabled, COLORREF textColor );
  virtual void OnCheckChanged() {}       // after SetCheck changed the state
  bool IsAutoCheck() const { return m_autoCheck; }

  // The converters' shared body: every "Button" child whose BS_ type
  // `matches` becomes `make()`, which the dialog then owns (BTN-F88, BTN-F93).
  static void ConvertDialogControls( HWND hDlg, bool ( *matches )( LONG_PTR type ),
    std::unique_ptr<SSCheckbox> ( *make )() );

private:
  void UpdateAccessibility( bool clear = false ); // BTN-F87

  bool m_checked = false;
  bool m_autoCheck = true;
  bool m_dwellAllowed = true;
};

#endif // SSCheckbox_h
