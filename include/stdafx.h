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

// Board resource subfolder: a board style "title" is sanitized into a folder
// name under %LocalAppData%\SimonSays\ that is searched first for that board's
// icons/audio (board-style.spec.md / import-export.spec.md).
#define BOARD_RESOURCE_FOLDER_MAX_NAME 64

// Default resource folder: the shared container for board resources.
// Untitled boards' icons/audio live directly here; titled boards nest one
// level deeper as DEFAULT_RESOURCE_FOLDER_NAME\<sanitized name> (see
// BOARD_RESOURCE_FOLDER_MAX_NAME / board-style.spec.md STY-F58). Replaces
// loose storage directly in %LocalAppData%\SimonSays\, which remains a
// permanent read fallback for resources placed there by earlier versions
// (sound.spec.md SND-F10 / import-export.spec.md PORT-F31-F33).
#define DEFAULT_RESOURCE_FOLDER_NAME L"resources"

// Diagnostic dumps/logs (e.g. the gaze/HID dump — dwell.spec.md REQ-F80) go
// under this subfolder of %LocalAppData%\SimonSays\, keeping the root clean.
// Write-only: nothing reads these back, so no search order is needed and
// existing root-level dumps from earlier versions are left in place.
#define DEBUG_FOLDER_NAME L"debug"

// Suggested home for saved board files (.ssc/.ssz) under
// %LocalAppData%\SimonSays\. Currently just a folder that is guaranteed to
// exist (see EnsureAppDataFoldersExist) and the export dialog's initial
// directory; the app does not otherwise browse/manage files inside it.
#define BOARDS_FOLDER_NAME L"boards"

#define DEFAULT_BACKUP_FILE L"backup.ssz" // default export file name for backup

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
  bool autoresizeCategoryWindow;
  bool minimizeCategoryWindowAutomatically;
  bool increaseVolumeWhenPlaying;
  bool reduceOtherAudioWhenPlaying;
  bool stopPreviousPlayback;
  bool showTouchKeyboard;
  bool showQuickButtons;      // quick access buttons in the main window (boards + settings)

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
  { L"Portuguese",            L"Português", false, MAKELANGID( LANG_PORTUGUESE, SUBLANG_PORTUGUESE ),           L"Este é um texto de exemplo em português com as definições de voz selecionadas" },
  { L"Portuguese (Brazil)",   L"Português (Brasil)", false, MAKELANGID( LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN ), L"Este é um texto de exemplo em português com as configurações de voz selecionadas" },
  { L"Russian",               L"Русский",   false, MAKELANGID( LANG_RUSSIAN, SUBLANG_RUSSIAN_RUSSIA ),          L"Это пример текста на русском языке с выбранными настройками голоса" },
  { L"Spanish",               L"Español",   false, MAKELANGID( LANG_SPANISH, SUBLANG_SPANISH ),                 L"Este es un texto de ejemplo en español con la configuración de voz seleccionada" },
  { L"Valencian",             L"Valencià",  false, MAKELANGID( LANG_CATALAN, SUBLANG_VALENCIAN_VALENCIA ),      L"Aquest és un text d'exemple en valencià amb la configuració de veu seleccionada" }
};

// ---------------------------------------------------------------------------
// Localized UI string IDs
//
// Keys into the per-language tables in localized_strings.h, resolved by
// GetLocalizedString (a by-value lookup: no ordering, range or arithmetic
// assumption anywhere). Nothing persists these numbers — the registry, the
// .ssc/.ssz formats and the translation scripts all work by NAME — so a block
// can be renumbered whenever it needs to be.
//
// One block of 100 per UI area, with free numbers inside each block so a new
// string can join its neighbours instead of being appended at the end of the
// file:
//
//   100-199  Main window & category window
//   200-299  Settings dialog
//   300-399  Tray icon & menu
//   400-499  About & errors
//   500-599  Categories & phrases (add / edit / delete)
//   600-699  Import & export
//   700-799  Gaze / Dwell-click
//   800-899  Board & category styles
//   900-999  Help & shared text
// ---------------------------------------------------------------------------

// --- 100-199  Main window & category window --------------------------------
#define PLAY_BUTTON_TEXT_ID                           100
#define PLAY_BUTTON_TEXT_PLAYING_ID                   101
#define CATEGORIES_BUTTON_TEXT_ID                     102

#define CATEGORY_SHORTCUTS_TEXT_ID                    150  // strip between categories and phrases

// --- 200-299  Settings dialog ----------------------------------------------
// Listed in dialog order, top to bottom (resources/SimonSays.rc,
// IDD_SETTINGS_DIALOG), so this block reads like the window itself.

#define SETTINGS_DIALOG_TITLE_TEXT_ID                 200

// Default text and the toggles under it (210-219)
#define SETTINGS_DEFAULT_TEXT_LABEL_ID                210
#define SETTINGS_USE_DEFAULT_TEXT_ID                  211
#define SETTINGS_SHOW_TOUCH_KEYBOARD_ID               212
#define SETTINGS_SHOW_QUICK_BUTTONS_ID                213

// Language, voice and voice test (220-229)
#define SETTINGS_LANGUAGE_LABEL_ID                    220
#define SETTINGS_VOICE_LABEL_ID                       221
#define SETTINGS_TEST_VOICE_BUTTON_ID                 222

// Volume and rate sliders (230-239)
#define SETTINGS_VOLUME_LABEL_ID                      230
#define SETTINGS_RATE_LABEL_ID                        231

// Category window and phrase selection (240-249)
#define SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID          240
#define SETTINGS_REMEMBER_CATEGORY_WINDOW_ID          241
#define SETTINGS_AUTORESIZE_CATEGORY_WINDOW_ID        242
#define SETTINGS_SPEAK_ON_CLICK_ID                    243

// Playback behavior (250-259)
#define SETTINGS_STOP_PREVIOUS_PLAYBACK_ID            250
#define SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID      251
#define SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID   252

// Dialog buttons (260-269)
#define SETTINGS_OK_BUTTON_ID                         260
#define SETTINGS_CANCEL_BUTTON_ID                     261

// --- 300-399  Tray icon & menu ---------------------------------------------
#define TRAYICON_TOOLTIP_ID                           300

// Menu entries, in menu order (310-329) — settings.spec.md SET-F31
#define TRAYICON_SHOW_ID                              310
#define TRAYICON_HIDE_ID                              311
#define TRAYICON_SETTINGS_ID                          312
#define TRAYICON_DWELL_ID                             313
#define TRAYICON_WEB_ID                               314
#define TRAYICON_ABOUT_ID                             315
#define TRAYICON_FEEDBACK_ID                          316
#define TRAYICON_EXIT_ID                              317

// --- 400-499  About & errors -----------------------------------------------
#define ABOUT_TITLE_ID                               400
#define ABOUT_1_ID                                    401
#define ABOUT_2_ID                                    402

#define ERROR_TITLE_ID                               450
#define ERROR_TASKBAR_POSITION_ID                     451

// --- 500-599  Categories & phrases -----------------------------------------
// Add dialog (500-509)
#define ADD_DIALOG_CATEGORY_TITLE_ID                  500
#define ADD_DIALOG_PHRASE_TITLE_ID                    501
#define ADD_DIALOG_CATEGORY_TEXT_LABEL_ID             502
#define ADD_DIALOG_PHRASE_TEXT_LABEL_ID               503
#define ADD_DIALOG_OK_BUTTON_ID                       504
#define ADD_DIALOG_CANCEL_BUTTON_ID                   505
#define NEW_PHRASE_DEFAULT_TEXT_ID                    506

// Edit dialog (510-519)
#define EDIT_DIALOG_CATEGORY_TITLE_ID                 510
#define EDIT_DIALOG_PHRASE_TITLE_ID                   511
#define EDIT_DIALOG_TEXT_LABEL_ID                     512
#define EDIT_DIALOG_OK_BUTTON_ID                      513
#define EDIT_DIALOG_CANCEL_BUTTON_ID                  514

// Name conflict (520-529)
#define CATEGORY_NAME_CONFLICT_TITLE_ID               520
#define CATEGORY_NAME_CONFLICT_MESSAGE_ID             521

// Deletion confirmations (530-549) — title first, then message parts
#define DELETE_CATEGORY_CONFIRMATION_TITLE_ID         530
#define DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID      531
#define DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID      532
#define DELETE_PHRASE_CONFIRMATION_TITLE_ID           535
#define DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID        536
#define DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID        537
#define DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID   540
#define DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID 541
#define DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID 542

// --- 600-699  Import & export ----------------------------------------------
// Import (600-649)
#define IMPORT_CATEGORIES_DIALOG_TITLE_ID             600
#define IMPORT_CATEGORIES_DIALOG_FILTER_ID            601
#define IMPORT_CATEGORY_OVERWRITE_TITLE_ID            605
#define IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID         606
#define IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID         607
#define IMPORT_SUCCESS_TITLE_ID                       610
#define IMPORT_SUCCESS_MESSAGE_ID                     611
#define IMPORT_FAILURE_TITLE_ID                       612
#define IMPORT_FAILURE_MESSAGE_ID                     613

// Export (650-699)
#define EXPORT_CATEGORIES_DIALOG_TITLE_ID             650
#define EXPORT_CATEGORIES_DIALOG_FILTER_ID            651
#define EXPORT_CATEGORY_CONFIRMATION_TITLE_ID         655
#define EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID      656
#define EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID      657
#define EXPORT_SUCCESS_TITLE_ID                       660
#define EXPORT_SUCCESS_MESSAGE_ID                     661
#define EXPORT_FAILURE_TITLE_ID                       662
#define EXPORT_FAILURE_MESSAGE_ID                     663

// --- 700-799  Gaze / Dwell-click (dwell.spec.md) ---------------------------
#define DWELL_DIALOG_TITLE_ID                         700
#define DWELL_INTRO_ID                                701

// Activation mode (710-719)
#define DWELL_MODE_GROUP_ID                           710
#define DWELL_MODE_AUTO_ID                            711
#define DWELL_MODE_MOUSE_ID                           712
#define DWELL_MODE_HID_ID                             713
#define DWELL_MODE_OFF_ID                             714

// Tuning (720-729)
#define DWELL_TIME_LABEL_ID                           720
#define DWELL_TOLERANCE_LABEL_ID                      721
#define DWELL_COOLDOWN_LABEL_ID                       722
#define DWELL_COLOR_BUTTON_ID                         723

// Detection probe & status (730-749)
#define DWELL_DETECT_GROUP_ID                         730
#define DWELL_PROBE_LOOK_ID                           731
#define DWELL_PROBE_MOUSE_ID                          732
#define DWELL_STATUS_INITIAL_ID                       735
#define DWELL_STATUS_GAZE_ID                          736
#define DWELL_STATUS_MOUSE_ID                         737
#define DWELL_STATUS_LOOK_HINT_ID                     738
#define DWELL_HID_UNAVAILABLE_ID                      739

// Passive detection signals (750-769)
#define DWELL_SIGNALS_GROUP_ID                        750
#define DWELL_SIGNAL_HID_LABEL_ID                     751
#define DWELL_SIGNAL_TOOL_LABEL_ID                    752
#define DWELL_SIGNAL_WEC_LABEL_ID                     753
#define DWELL_SIGNAL_YES_ID                           755
#define DWELL_SIGNAL_NO_ID                            756
#define DWELL_SIGNAL_NONE_ID                          757
#define DWELL_SIGNAL_HID_LIVE_ID                      758
#define DWELL_SIGNAL_HID_IDLE_ID                      759

// Buttons (770-779)
#define DWELL_APPLY_BUTTON_ID                         770
#define DWELL_RESET_BUTTON_ID                         771

// --- 800-899  Board & category styles (board-style.spec.md STY-F53) --------
#define IMPORT_BOARD_STYLE_REPLACE_TITLE_ID           800
#define IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID         801
#define BOARD_STYLE_CHANGED_TITLE_ID                  810
#define BOARD_STYLE_CHANGED_MESSAGE_ID                811

// --- 900-999  Help & shared text -------------------------------------------
// HELP_CONTENT_ID is retired: the in-app help window was replaced by the
// generated HTML pages shipped in <exe dir>\help\ (F1 opens one in the
// browser). The #define stays only because HelpWindow.cpp still compiles
// against it; no language table carries the string any more, so a lookup
// returns "".
#define HELP_CONTENT_ID                               900  // retired, see above
#define HELP_CONTENT_FILE_NAME_ID                     901  // written by scripts/build_help_html.ps1
#define AMPERSAND_REPLACEMENT_ID                      950  // export filenames (localization.spec.md)

#endif // stdafx_h