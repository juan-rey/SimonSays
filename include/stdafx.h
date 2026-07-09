/*
   stdafx.h: stdafx header file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
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

#define APP_NAME L"SimonSays"
//Δ˄˅↑↓▲▼
#define UP_ARROW L"▲"
#define DOWN_ARROW L"▼"
#define SOUND_NOTE_DELIMITER L"♫" // Note: SOUND_NOTE_DELIMITER should be a single character for easier parsing, but it can be changed if needed.
#define CATEGORY_PHRASE_SEPARATOR L"|" // Note: CATEGORY_PHRASE_SEPARATOR should be a single character for easier parsing, but it can be changed if needed.
#define AUDIO_FILE_SEPARATOR L"::"
#define AUDIO_FILE_SEPARATOR_LENGTH 2
#define ICON_SEPARATOR L"##"
#define ICON_SEPARATOR_LENGTH 2

// Board & category styles (see docs/specs/board-style.spec.md).
// BOARD_STYLE_CATEGORY_NAME is the reserved category name holding the board
// (global) style layer; STYLE_TOKEN_PREFIX marks style tokens stored in
// phrase-position data. User category names starting with the prefix are
// rejected. The category style suffix in the edit dialog reuses
// AUDIO_FILE_SEPARATOR ("::").
#define BOARD_STYLE_CATEGORY_NAME L"$$board"
#define STYLE_TOKEN_PREFIX L"$$"
#define STYLE_TOKEN_PREFIX_LENGTH 2

// .ssz bundle: a Zip archive holding exactly one categories.ssc at the root plus
// the referenced resources (icons / audio) under a "resources/" prefix. Entry
// names are raw bytes in Zip, so these are narrow (UTF-8) strings.
#define SSZ_SSC_ENTRY_NAME          "categories.ssc"
#define SSZ_RESOURCE_PREFIX         "resources/"
// Import hardening limits (defend against zip-bombs / malformed archives).
#define SSZ_MAX_ENTRIES             5000
#define SSZ_MAX_TOTAL_UNCOMPRESSED  ( 256ull * 1024 * 1024 )  // 256 MB across all entries
#define SSZ_MAX_ENTRY_UNCOMPRESSED  ( 64ull * 1024 * 1024 )   // 64 MB per single entry
#define SSZ_MAX_COMPRESSION_RATIO   100                       // per-entry uncompressed:compressed

#define WM_TRAYICON (WM_USER + 1)
#define WM_SHOW_CATEGORY_WINDOW (WM_USER + 2)
#define WM_PLAYBACK_STARTED (WM_USER + 3)
#define WM_PLAYBACK_FINISHED (WM_USER + 4)
#define WM_EDIT_CONTROL_ENTER_PRESSED (WM_USER + 5)
#define SIMONSAYS_COPYDATA_IMPORT_SSC 0x53534331 // "SSC1" in hex, used as an identifier for importing .ssc files via WM_COPYDATA

struct Phrase
{
  std::wstring text;
  std::wstring icon;
  std::wstring audioFile;
};

struct Category
{
  std::wstring name;
  std::wstring icon;
  std::wstring style; // raw "property:value;" list (board-style.spec.md); round-trips verbatim
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
  bool speakDirectlyWhenClickingPhrase;
  bool rememberCategoryWindowSize;
  bool minimizeCategoryWindowAutomatically;
  bool increaseVolumeWhenPlaying;
  bool reduceOtherAudioWhenPlaying;
  bool stopPreviousPlayback;
  bool showTouchKeyboard;

  // Gaze dwell-click (see SSDwellConfig). Stored as ints to keep this struct
  // free of the SSButton enums; values mirror SSDwellModeSelection / SSDwellMode.
  int      dwellModeSelection;  // 0 Auto, 1 Off, 2 Mouse, 3 HID
  int      dwellTimeMs;         // fixation time before firing (ms)
  int      dwellToleranceRadius;// jitter tolerance radius (px)
  int      dwellCooldownMs;     // post-activation dead time (ms)
  COLORREF dwellProgressColor;  // progress-indicator fill color
  int      dwellDetectedMode;   // last detected mode for Auto: 0 Off, 1 Mouse, 2 HID, 3 ExternalClick
};

// Gaze dwell-click first-run defaults. Single source shared by the registry
// load path (RegistryManager) and the dwell window's Reset button. The default
// progress color is dynamic (the Windows accent color via GetAccentColor()),
// so it has no constant here.
#define DWELL_DEFAULT_MODE_SELECTION 0 // Auto
#define DWELL_DEFAULT_TIME_MS 800
#define DWELL_DEFAULT_TOLERANCE_PX 35
#define DWELL_DEFAULT_COOLDOWN_MS 300
#define DWELL_DEFAULT_DETECTED_MODE 0  // Off

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
  bool IsRTL;
  LANGID LanguageId;
  std::wstring VoiceTestSampleText;
};

static const wchar_t * CLASS_NAME = L"SimonSaysMainWindow";
static const wchar_t * CATEGORY_WINDOW_CLASS = L"SimonSaysCategoryWindow";

static const std::vector<LanguageInfo> SUPPORTED_LANGUAGES = {
  { L"Arabic",                L"العربية",   true,  MAKELANGID( LANG_ARABIC, SUBLANG_DEFAULT ),                  L"هذا نص تجريبي باللغة العربية مع إعدادات الصوت المحددة" },
  { L"Basque",                L"Euskara",   false, MAKELANGID( LANG_BASQUE, SUBLANG_DEFAULT ),                  L"Hau da euskarazko adibide-testua hautatutako ahots-ezarpenekin" },
  { L"Catalan",               L"Català",    false, MAKELANGID( LANG_CATALAN, SUBLANG_DEFAULT ),                 L"Aquest és un text d'example en català amb la configuració de veu seleccionada" },
  { L"Chinese (Simplified)",  L"简体中文",   false, MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED ),      L"这是一个使用所选语音设置的中文示例文本" },
  { L"English",               L"English",   false, MAKELANGID( LANG_ENGLISH, SUBLANG_ENGLISH_US ),              L"This is an example text in English with the selected voice settings" },
  { L"French",                L"Français",  false, MAKELANGID( LANG_FRENCH, SUBLANG_FRENCH ),                   L"Ceci est un texte d'exemple en français avec les paramètres vocaux sélectionnés" },
  { L"Galician",              L"Galego",    false, MAKELANGID( LANG_GALICIAN, SUBLANG_DEFAULT ),                L"Este é um texto de exemplo en galego coa configuração de voz seleccionada" },
  { L"German",                L"Deutsch",   false, MAKELANGID( LANG_GERMAN, SUBLANG_GERMAN ),                   L"Dies ist ein Beispieltext auf Deutsch mit den ausgewählten Stimmeinstellungen" },
  { L"Hebrew",                L"עברית",     true,  MAKELANGID( LANG_HEBREW, SUBLANG_HEBREW_ISRAEL ),            L"זהו טקסט לדוגמה בעברית עם הגדרות הקול שנבחרו" },
  { L"Hindi",                 L"हिन्दी",       false, MAKELANGID( LANG_HINDI, SUBLANG_HINDI_INDIA ),               L"यह चयनित वॉइस सेटिंग्स के साथ हिंदी में एक उदाहरण पाठ है" },
  { L"Italian",               L"Italiano",  false, MAKELANGID( LANG_ITALIAN, SUBLANG_ITALIAN ),                 L"Questo è un texto di esempio in italiano con le impostazioni vocalи selezionate" },
  { L"Japanese",              L"日本語",     false, MAKELANGID( LANG_JAPANESE, SUBLANG_JAPANESE_JAPAN ),         L"これは選択された音声設定を使用した日本語のサンプルテキストです"},
  { L"Korean",                L"한국어",     false, MAKELANGID( LANG_KOREAN, SUBLANG_KOREAN ),                   L"이것은 선택된 음성 설정을 사용한 한국어 예제 텍스트입니다" },
  { L"Portuguese",            L"Português", false, MAKELANGID( LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN ), L"Este é um texto de exemplo em português com as configurações de voz selecionadas" },
  { L"Russian",               L"Русский",   false, MAKELANGID( LANG_RUSSIAN, SUBLANG_RUSSIAN_RUSSIA ),          L"Это пример текста на русском языке с выбранными настройками голоса" },
  { L"Spanish",               L"Español",   false, MAKELANGID( LANG_SPANISH, SUBLANG_SPANISH ),                 L"Este es un texto de ejemplo en español con la configuración de voz seleccionada" },
  { L"Valencian",             L"Valencià",  false, MAKELANGID( LANG_CATALAN, SUBLANG_VALENCIAN_VALENCIA ),      L"Aquest és un text d'exemple en valencià amb la configuració de veu seleccionada" }
};

#define PLAY_BUTTON_TEXT_ID 0
#define PLAY_BUTTON_TEXT_PLAYING_ID 1
#define CATEGORIES_BUTTON_TEXT_ID 2
#define SETTINGS_DIALOG_TITLE_TEXT_ID 10
#define SETTINGS_DEFAULT_TEXT_LABEL_ID 11
#define SETTINGS_USE_DEFAULT_TEXT_ID 12
#define SETTINGS_LANGUAGE_LABEL_ID 13
#define SETTINGS_VOICE_LABEL_ID 14
#define SETTINGS_TEST_VOICE_BUTTON_ID 15
#define SETTINGS_VOLUME_LABEL_ID 16
#define SETTINGS_RATE_LABEL_ID 17
#define SETTINGS_SPEAK_ON_CLICK_ID 18
#define SETTINGS_REMEMBER_CATEGORY_WINDOW_ID 19
#define SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID 20
#define SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID 21
#define SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID 22
#define SETTINGS_OK_BUTTON_ID 23
#define SETTINGS_CANCEL_BUTTON_ID 24
#define TRAYICON_TOOLTIP_ID 25
#define TRAYICON_SHOW_ID 26
#define TRAYICON_HIDE_ID 27
#define TRAYICON_SETTINGS_ID 28
#define TRAYICON_WEB_ID 29
#define TRAYICON_ABOUT_ID 30
#define TRAYICON_EXIT_ID 31
#define ABOUT_TITTLE_ID 32
#define ABOUT_1_ID 33
#define ABOUT_2_ID 34
#define ERROR_TITTLE_ID 35
#define ERROR_TASKBAR_POSITION_ID 36
#define EDIT_DIALOG_CATEGORY_TITLE_ID 37
#define EDIT_DIALOG_PHRASE_TITLE_ID 38
#define EDIT_DIALOG_TEXT_LABEL_ID 39
#define EDIT_DIALOG_OK_BUTTON_ID 40
#define EDIT_DIALOG_CANCEL_BUTTON_ID 41
#define CATEGORY_NAME_CONFLICT_MESSAGE_ID 42
#define CATEGORY_NAME_CONFLICT_TITLE_ID 43
#define DELETE_CATEGORY_CONFIRMATION_TITLE_ID 44
#define DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID 45
#define DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID 46
#define DELETE_PHRASE_CONFIRMATION_TITLE_ID 47
#define DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID 48
#define DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID 49
#define ADD_DIALOG_CATEGORY_TITLE_ID 50
#define ADD_DIALOG_PHRASE_TITLE_ID 51
#define ADD_DIALOG_CATEGORY_TEXT_LABEL_ID 52
#define ADD_DIALOG_PHRASE_TEXT_LABEL_ID 53
#define ADD_DIALOG_OK_BUTTON_ID 54
#define ADD_DIALOG_CANCEL_BUTTON_ID 55
#define NEW_PHRASE_DEFAULT_TEXT_ID 56
#define IMPORT_CATEGORIES_DIALOG_TITLE_ID 57
#define IMPORT_CATEGORIES_DIALOG_FILTER_ID 58
#define IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID 59
#define IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID 60
#define IMPORT_CATEGORY_OVERWRITE_TITLE_ID 61
#define IMPORT_SUCCESS_MESSAGE_ID 62
#define IMPORT_SUCCESS_TITLE_ID 63
#define IMPORT_FAILURE_MESSAGE_ID 64
#define IMPORT_FAILURE_TITLE_ID 65
#define EXPORT_CATEGORIES_DIALOG_TITLE_ID 66
#define EXPORT_CATEGORIES_DIALOG_FILTER_ID 67
#define EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID 68
#define EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID 69
#define EXPORT_CATEGORY_CONFIRMATION_TITLE_ID 70
#define EXPORT_SUCCESS_MESSAGE_ID 71
#define EXPORT_SUCCESS_TITLE_ID 72
#define EXPORT_FAILURE_MESSAGE_ID 73
#define EXPORT_FAILURE_TITLE_ID 74
#define CATEGORY_SHORTCUTS_TEXT_ID 75
#define AMPERSAND_REPLACEMENT_ID 76
#define SETTINGS_STOP_PREVIOUS_PLAYBACK_ID 77
#define SETTINGS_SHOW_TOUCH_KEYBOARD_ID 78
#define TRAYICON_FEEDBACK_ID 79
#define HELP_CONTENT_ID 80
// Gaze dwell-click window
#define DWELL_DIALOG_TITLE_ID 81
#define DWELL_INTRO_ID 82
#define DWELL_MODE_GROUP_ID 83
#define DWELL_MODE_AUTO_ID 84
#define DWELL_MODE_MOUSE_ID 85
#define DWELL_MODE_HID_ID 86
#define DWELL_MODE_OFF_ID 87
#define DWELL_TIME_LABEL_ID 88
#define DWELL_TOLERANCE_LABEL_ID 89
#define DWELL_COOLDOWN_LABEL_ID 90
#define DWELL_COLOR_BUTTON_ID 91
#define DWELL_DETECT_GROUP_ID 92
#define DWELL_PROBE_LOOK_ID 93
#define DWELL_PROBE_MOUSE_ID 94
#define DWELL_STATUS_INITIAL_ID 95
#define DWELL_STATUS_GAZE_ID 96
#define DWELL_STATUS_MOUSE_ID 97
#define DWELL_STATUS_LOOK_HINT_ID 98
#define DWELL_APPLY_BUTTON_ID 99
#define DWELL_HID_UNAVAILABLE_ID 100
#define TRAYICON_DWELL_ID 101
// Passive detection signals (dwell window)
#define DWELL_SIGNALS_GROUP_ID 102
#define DWELL_SIGNAL_HID_LABEL_ID 103
#define DWELL_SIGNAL_TOOL_LABEL_ID 104
#define DWELL_SIGNAL_WEC_LABEL_ID 105
#define DWELL_SIGNAL_YES_ID 106
#define DWELL_SIGNAL_NO_ID 107
#define DWELL_SIGNAL_NONE_ID 108
#define DWELL_SIGNAL_HID_LIVE_ID 109
#define DWELL_SIGNAL_HID_IDLE_ID 110
#define DWELL_RESET_BUTTON_ID 111
// Board & category styles (board-style.spec.md STY-F53)
#define IMPORT_BOARD_STYLE_REPLACE_TITLE_ID 112
#define IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID 113

#endif // stdafx_h