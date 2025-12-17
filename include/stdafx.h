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
  std::wstring VoiceTestSampleText;
};

static const wchar_t * CLASS_NAME = L"SimonSaysMainWindow";
static const wchar_t * CATEGORY_WINDOW_CLASS = L"SimonSaysCategoryWindow";
static const wchar_t * TRAY_TOOLTIP = L"SimonSays - Click to show categories";
static const std::vector<LanguageInfo> SUPPORTED_LANGUAGES = {
  { L"English",     L"English",      L"This is an example text in English with the selected voice settings" },
  { L"French",      L"Français",     L"Ceci est un texte d'exemple en français avec les paramètres vocaux sélectionnés" },
  { L"German",      L"Deutsch",      L"Dies ist ein Beispieltext auf Deutsch mit den ausgewählten Stimmeinstellungen" },
  { L"Italian",     L"Italiano",     L"Questo è un testo di esempio in italiano con le impostazioni vocali selezionate" },
  { L"Portuguese",  L"Português",    L"Este é um texto de exemplo em português com as configurações de voz selecionadas" },
  { L"Spanish",     L"Español",      L"Este es un texto de ejemplo en español con la configuración de voz seleccionada" }
  /*{ L"Chinese (Simplified)", L"简体中文", L"这是一个使用所选语音设置的中文示例文本" },
  {L"Japanese",    L"日本語",        L"これは選択された音声設定を使用した日本語のサンプルテキストです"},
  { L"Korean",      L"한국어",        L"이것은 선택된 음성 설정을 사용한 한국어 예제 텍스트입니다" },
  { L"Russian",     L"Русский",      L"Это пример текста на русском языке с выбранными настройками голоса" },
  { L"Hebrew",      L"עברית",        L"זהו טקסט לדוגמה בעברית עם הגדרות הקול שנבחרו" },
  { L"Hindi",       L"हिन्दी",        L"यह चयनित वॉइस सेटिंग्स के साथ हिंदी में एक उदाहरण पाठ है" },
  { L"Arabic",      L"العربية",      L"هذا نص تجريبي باللغة العربية مع إعدادات الصوت المحددة" },*/
};


#endif // stdafx_h