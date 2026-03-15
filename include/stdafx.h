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

//Δ˄˅↑↓▲▼
#define UP_ARROW L"▲"
#define DOWN_ARROW L"▼"
#define SOUND_NOTE_DELIMITER L"♫" // Note: SOUND_NOTE_DELIMITER should be a single character for easier parsing, but it can be changed if needed.
#define CATEGORY_PHRASE_SEPARATOR L"|" // Note: CATEGORY_PHRASE_SEPARATOR should be a single character for easier parsing, but it can be changed if needed.
#define AUDIO_FILE_SEPARATOR L"::"
#define AUDIO_FILE_SEPARATOR_LENGTH 2
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
  bool speakDirectlyWhenClickingPhrase;
  bool rememberCategoryWindowSize;
  bool minimizeCategoryWindowAutomatically;
  bool increaseVolumeWhenPlaying;
  bool reduceOtherAudioWhenPlaying;
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
#define EXPORT_SUCCESS_MESSAGE_ID 57
#define EXPORT_SUCCESS_TITLE_ID 58
#define EXPORT_FAILURE_MESSAGE_ID 59
#define EXPORT_FAILURE_TITLE_ID 60
#define IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID 61
#define IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID 62
#define IMPORT_CATEGORY_OVERWRITE_TITLE_ID 63
#define IMPORT_SUCCESS_MESSAGE_ID 64
#define IMPORT_SUCCESS_TITLE_ID 65
#define IMPORT_FAILURE_MESSAGE_ID 66
#define IMPORT_FAILURE_TITLE_ID 67
#define EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID 68
#define EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID 69
#define EXPORT_CATEGORY_CONFIRMATION_TITLE_ID 70
#define CATEGORY_SHORTCUTS_TEXT_ID 71
#define HELP_CONTENT_ID 72

#endif // stdafx_h