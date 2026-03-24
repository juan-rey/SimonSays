/*
   HelpWindow.h: HelpWindow class header file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef HelpWindow_h
#define HelpWindow_h

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
  void SetLanguage( const std::wstring & language );

  static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  static LRESULT CALLBACK EditSubclassProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData );

private:
  bool RegisterWindowClass( HINSTANCE hInstance );
  void UpdateLayout();

  HWND m_hwnd = NULL;
  HWND m_hContent = NULL;
  HINSTANCE m_hInstance = NULL;
  HFONT m_hFont = NULL;
  COLORREF m_textColor = RGB( 0, 0, 0 );
  HBRUSH m_backgroundBrush = NULL;
  std::wstring m_language;

  int m_defaultWidth = 640;
  int m_defaultHeight = 320;
};

#endif // HelpWindow_h