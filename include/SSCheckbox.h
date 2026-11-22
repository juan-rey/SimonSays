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

// A checkbox that gaze dwell can press (ssbutton.spec.md §6.11, BTN-F84 to
// BTN-F88). An SSButton with no border whose whole rect is the target: a
// square box, ticked when checked, then the label. Right-to-left through
// WS_EX_LAYOUTRTL, like SSButton. It answers BM_GETCHECK, BM_SETCHECK and
// BM_CLICK, so IsDlgButtonChecked and CheckDlgButton work on it, and it tells
// screen readers it is a checkbox and whether it is checked.
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
  // BS_VCENTER unless other BS_ alignment is given). `autoCheck`: activation
  // flips the state, as BS_AUTOCHECKBOX does; false leaves it to the parent,
  // as BS_CHECKBOX does.
  HWND Create( HWND hwndParent, HINSTANCE hInstance, int id,
    const std::wstring & text,
    int x, int y, int w, int h,
    DWORD style, DWORD exStyle = 0, bool autoCheck = true );

  void SetCheck( bool checked );
  bool GetCheck() const { return m_checked; }

  // BTN-F88: replaces every BS_AUTOCHECKBOX and BS_CHECKBOX of a dialog by an
  // SSCheckbox with the same id, rect, text, font, state, enabled/visible
  // state and Tab position. Dwell arms SSBUTTON_DIALOG_DWELL_ARM_MS after the
  // call. The dialog owns them (freed on WM_NCDESTROY). Call in WM_INITDIALOG
  // before the checkboxes are set, tooltips attached or handles kept.
  static void ConvertDialogCheckboxes( HWND hDlg );

protected:
  bool HandleMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT & result ) override;
  void OnBeforeClick() override;
  void PaintMark( HDC hdc, RECT & contentRc, bool isEnabled, COLORREF textColor ) override;
  bool AlwaysRearm() const override { return true; } // BTN-F86: one look, one change

private:
  void UpdateAccessibility( bool clear = false ); // BTN-F87

  bool m_checked = false;
  bool m_autoCheck = true;
};

#endif // SSCheckbox_h
