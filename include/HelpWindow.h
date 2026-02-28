#pragma once

#include "stdafx.h"
#include "utils.h"
#include <string>

class HelpWindow
{
public:
  HelpWindow();
  ~HelpWindow();

  bool Create( HINSTANCE hInstance );
  void Show();
  void Hide();
  bool IsVisible() const;
  void SetText( const std::wstring & text );

  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
  bool RegisterWindowClass( HINSTANCE hInstance );
  void UpdateLayout();

  HWND m_hwnd = NULL;
  HWND m_hContent = NULL;
  HINSTANCE m_hInstance = NULL;
  HFONT m_hFont = NULL;
  std::wstring m_text;

  int m_defaultWidth = 420;
  int m_defaultHeight = 320;
};
