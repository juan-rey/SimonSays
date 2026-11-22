/*
   SSRadioButton.h: SSRadioButton, a dwell-able radio button built on SSCheckbox.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef SSRadioButton_h
#define SSRadioButton_h

#include "SSCheckbox.h"

// A radio button that gaze dwell can choose (ssbutton.spec.md §6.12, BTN-F89
// to BTN-F93). An SSCheckbox drawn as a circle, with a dot when chosen. Its
// group runs from a control with WS_GROUP to the next one, as in a dialog.
// Choosing it (click, Space, BM_CLICK, dwell) unchooses the rest of its group
// when it is automatic (BS_AUTORADIOBUTTON); choosing the chosen one changes
// nothing. The arrow keys move through the group and choose; the Tab stop
// follows the choice. It answers DLGC_RADIOBUTTON, BM_GETCHECK and
// BM_SETCHECK, so CheckRadioButton and IsDlgButtonChecked work on it, and
// screen readers hear a radio button.
class SSRadioButton : public SSCheckbox
{
public:
  SSRadioButton() = default;

  // BTN-F93: as SSCheckbox::ConvertDialogCheckboxes, for BS_AUTORADIOBUTTON
  // and BS_RADIOBUTTON, groups kept; the Tab stop then sits on each group's
  // chosen one. Call in WM_INITDIALOG.
  static void ConvertDialogRadioButtons( HWND hDlg );

protected:
  bool HandleMessage( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT & result ) override;
  void OnBeforeClick() override;
  void DrawMark( HDC hdc, const RECT & box, int size, bool isEnabled, COLORREF textColor ) override;
  LONG AccessibleRole() const override;
  void OnCheckChanged() override;

private:
  void MoveInGroup( bool forward ); // BTN-F91: arrow keys
  void UpdateGroupTabStops();       // BTN-F91: the Tab stop on the chosen one
};

#endif // SSRadioButton_h
