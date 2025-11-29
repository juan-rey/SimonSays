#pragma once

#ifndef stdafx_h
#define stdafx_h

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <mmsystem.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "winmm.lib")

#define SOUND_NOTE_DELIMITER L"♫"
#define PLAY_BUTTON_TEXT L"Play>"
#define PLAY_BUTTON_TEXT_PLAYING L">"
#define WM_TRAYICON (WM_USER + 1)
#define WM_SHOW_CATEGORY_WINDOW (WM_USER + 2)

static const wchar_t * CLASS_NAME = L"SimonSaysMainWindow";
static const wchar_t * CATEGORY_WINDOW_CLASS = L"SimonSaysCategoryWindow";
static const wchar_t * TRAY_TOOLTIP = L"SimonSays - Click to show categories";

struct Phrase
{
  std::wstring text;
  std::wstring audioFile;
};

struct Category
{
  std::wstring name;
  std::vector<Phrase> phrases;
};

#endif // stdafx_h