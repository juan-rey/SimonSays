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

struct Settings
{
  std::wstring language;
  std::wstring defaultText;
  bool useDefaultText;
  std::wstring voice;
  int volume;
  int rate;
};

struct VoiceInfo
{
  std::wstring name;
  std::wstring key;
  std::wstring language;
};

struct LanguageInfo
{
  std::wstring EnglishName;
  std::wstring NativeName;
};

static const wchar_t * CLASS_NAME = L"SimonSaysMainWindow";
static const wchar_t * CATEGORY_WINDOW_CLASS = L"SimonSaysCategoryWindow";
static const wchar_t * TRAY_TOOLTIP = L"SimonSays - Click to show categories";
static const std::vector<LanguageInfo> SUPPORTED_LANGUAGES = {
  { L"English",     L"English"    },
  { L"French",      L"Français"   },
  { L"German",      L"Deutsch"    },
  { L"Italian",     L"Italiano"   },
  { L"Portuguese",  L"Português"  },
  { L"Spanish",     L"Español"    }
  /*  { L"Chinese (Simplified)", L"简体中文" },
  { L"Japanese", L"日本語" },
  { L"Korean", L"한국어" },
  { L"Russian", L"Русский" },
  { L"Hebrew", L"עברית" },
  { L"Hindi", L"हिन्दी" },
  { L"Arabic", L"العربية" }, */
};

#endif // stdafx_h