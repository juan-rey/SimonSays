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
  std::wstring VoiceTestSampleText;
};

static const wchar_t * CLASS_NAME = L"SimonSaysMainWindow";
static const wchar_t * CATEGORY_WINDOW_CLASS = L"SimonSaysCategoryWindow";
static const wchar_t * TRAY_TOOLTIP = L"SimonSays - Click to show categories";

static const std::vector<LanguageInfo> SUPPORTED_LANGUAGES = {
  { L"Arabic",      L"العربية", true,     L"هذا نص تجريبي باللغة العربية مع إعدادات الصوت المحددة" },
  { L"Basque",      L"Euskara", false,     L"Hau da euskarazko adibide-testua hautatutako ahots-ezarpenekin" },
  { L"Catalan",     L"Català", false,      L"Aquest és un text d'example en català amb la configuració de veu seleccionada" },
  { L"Chinese (Simplified)", L"简体中文", false, L"这是一个使用所选语音设置的中文示例文本" },
  { L"English",     L"English", false,     L"This is an example text in English with the selected voice settings" },
  { L"French",      L"Français", false,    L"Ceci est un texte d'exemple en français avec les paramètres vocaux sélectionnés" },
  { L"Galician",    L"Galego", false,      L"Este é um texto de exemplo en galego coa configuração de voz seleccionada" },
  { L"German",      L"Deutsch", false,     L"Dies ist ein Beispieltext auf Deutsch mit den ausgewählten Stimmeinstellungen" },
  { L"Hebrew",      L"עברית", true,       L"זהו טקסט לדוגמה בעברית עם הגדרות הקול שנבחרו" },
  { L"Hindi",       L"हिन्दी", false,       L"यह चयनित वॉइस सेटिंग्स के साथ हिंदी में एक उदाहरण पाठ है" },
  { L"Italian",     L"Italiano", false,    L"Questo è un texto di esempio in italiano con le impostazioni vocalи selezionate" },
  { L"Japanese",    L"日本語", false,       L"これは選択された音声設定を使用した日本語のサンプルテキストです"},
  { L"Korean",      L"한국어", false,       L"이것은 선택된 음성 설정을 사용한 한국어 예제 텍스트입니다" },
  { L"Portuguese",  L"Português", false,   L"Este é um texto de exemplo em português com as configurações de voz selecionadas" },
  { L"Russian",     L"Русский", false,     L"Это пример текста на русском языке с выбранными настройками голоса" },
  { L"Spanish",     L"Español", false,     L"Este es un texto de ejemplo en español con la configuración de voz seleccionada" },
  { L"Valencian",   L"Valencià", false,    L"Aquest és un text d'exemple en valencià amb la configuració de veu seleccionada" }
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

#endif // stdafx_h