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
  { L"Italian",     L"Italiano", false,    L"Questo è un texto di esempio in italiano con le impostazioni vocali selezionate" },
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

static const std::vector<std::pair<int, const wchar_t *>> DEFAULT_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Play>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Settings" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Default text:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insert default text automatically" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Language:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voice:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Test Voice" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Rate (-10 to 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Speak phrase immediately when selecting" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Remember category window size" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimize category window automatically" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Increase SimonSays volume when playing" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Temporarily reduce other audio when playing" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel" }
};

static const std::vector<std::pair<int, const wchar_t *>> SPANISH_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Reproducir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorías" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuración" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texto predeterminado:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insertar texto predeterminado automáticamente" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voz:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Probar voz" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volumen (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocidad (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Hablar frase al seleccionarla" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Recordar tamaño de la ventana de categorías" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar ventana de categorías automáticamente" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumentar volumen de SimonSays al reproducir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Reducir temporalmente otros sonidos al reproducir" },
  { SETTINGS_OK_BUTTON_ID, L"Aceptar" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancelar" }
};

static const std::vector<std::pair<int, const wchar_t *>> ARABIC_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"تشغيل>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"الفئات" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"الإعدادات" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"النص الافتراضي:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"إدراج النص الافتراضي تلقائياً" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"اللغة:" },
  { SETTINGS_VOICE_LABEL_ID, L"الصوت:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"اختبار الصوت" },
  { SETTINGS_VOLUME_LABEL_ID, L"مستوى الصوت (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"السرعة (-10 إلى 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"نطق العبارة مباشرة عند التحديد" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"تذكر حجم نافذة الفئات" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"تصغير نافذة الفئات تلقائياً" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"زيادة مستوى صوت SimonSays أثناء التشغيل" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"خفض الأصوات الأخرى مؤقتاً أثناء التشغيل" },
  { SETTINGS_OK_BUTTON_ID, L"موافق" },
  { SETTINGS_CANCEL_BUTTON_ID, L"إلغاء" }
};

static const std::vector<std::pair<int, const wchar_t *>> BASQUE_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Erreproduzitu>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Kategoriak" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Ezarpenak" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Testu lehenetsia:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Txertatu testu lehenetsia automatikoki" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Hizkuntza:" },
  { SETTINGS_VOICE_LABEL_ID, L"Ahotsa:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Ahotsa probatu" },
  { SETTINGS_VOLUME_LABEL_ID, L"Bolumena (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Abiadura (-10etik 10era):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Hautatzean esaldia berehala esan" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Gogoratu kategoriaren leihoaren tamaina" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizatu kategoriaren leihoa automatikoki" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Handitu SimonSays bolumena erreproduzitzean" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Murriztu aldi baterako beste audioak erreproduzitzean" },
  { SETTINGS_OK_BUTTON_ID, L"Ados" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Utzi" }
};

static const std::vector<std::pair<int, const wchar_t *>> CATALAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Reproduir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuració" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Text predeterminat:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insereix el text predeterminat automàticament" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Veu:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Prova la veu" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volum (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocitat (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Parla la frase immediatament en seleccionar-la" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Recorda la mida de la finestra de categories" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimitza automàticament la finestra de categories" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Augmenta el volum de SimonSays en reproduir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Redueix temporalment altres sons en reproduir" },
  { SETTINGS_OK_BUTTON_ID, L"D'acord" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel·lar" }
};

static const std::vector<std::pair<int, const wchar_t *>> CHINESE_SIMPLIFIED_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"播放>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"类别" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"设置" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"默认文本：" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"自动插入默认文本" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"语言：" },
  { SETTINGS_VOICE_LABEL_ID, L"语音：" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"测试语音" },
  { SETTINGS_VOLUME_LABEL_ID, L"音量 (10-100)：" },
  { SETTINGS_RATE_LABEL_ID, L"语速 (-10 到 10)：" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"选择短语时立即朗读" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"记住分类窗口大小" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"自动最小化分类窗口" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"播放时提高 SimonSays 音量" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"播放时临时降低其他音频" },
  { SETTINGS_OK_BUTTON_ID, L"确定" },
  { SETTINGS_CANCEL_BUTTON_ID, L"取消" }
};

static const std::vector<std::pair<int, const wchar_t *>> FRENCH_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Lire>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Catégories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Paramètres" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texte par défaut :" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insérer automatiquement le texte par défaut" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Langue :" },
  { SETTINGS_VOICE_LABEL_ID, L"Voix :" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Tester la voix" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100) :" },
  { SETTINGS_RATE_LABEL_ID, L"Débit (-10 à 10) :" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Dire immédiatement la phrase lors de la sélection" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Mémoriser la taille de la fenêtre des catégories" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Réduire automatiquement la fenêtre des catégories" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Augmenter le volume de SimonSays pendant la lecture" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Réduire temporairement les autres sons pendant la lecture" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Annuler" }
};

static const std::vector<std::pair<int, const wchar_t *>> GALICIAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Reproducir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorías" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuración" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texto predeterminado:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Inserir automaticamente o texto predeterminado" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voz:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Probar voz" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocidade (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Falar a frase inmediatamente ao seleccionala" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Lembrar o tamaño da xanela de categorías" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar automaticamente a xanela de categorías" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumentar o volume de SimonSays ao reproducir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Reducir temporalmente outros sons ao reproducir" },
  { SETTINGS_OK_BUTTON_ID, L"Aceptar" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancelar" }
};

static const std::vector<std::pair<int, const wchar_t *>> GERMAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Abspielen>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Kategorien" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Einstellungen" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Standardtext:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Standardtext automatisch einfügen" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Sprache:" },
  { SETTINGS_VOICE_LABEL_ID, L"Stimme:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Stimme testen" },
  { SETTINGS_VOLUME_LABEL_ID, L"Lautstärke (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Geschwindigkeit (-10 bis 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Satz bei Auswahl sofort aussprechen" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Größe des Kategorienfensters merken" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Kategorienfenster automatisch minimieren" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Lautstärke von SimonSays beim Abspielen erhöhen" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Andere Audios beim Abspielen vorübergehend reduzieren" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Abbrechen" }
};

static const std::vector<std::pair<int, const wchar_t *>> HEBREW_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"נגן>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"קטגוריות" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"הגדרות" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"טקסט ברירת מחדל:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"הכנס טקסט ברירת מחדל אוטומטית" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"שפה:" },
  { SETTINGS_VOICE_LABEL_ID, L"קול:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"בדוק קול" },
  { SETTINGS_VOLUME_LABEL_ID, L"עוצמה (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"מהירות (-10 עד 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"אמור את המשפט מיד בעת הבחירה" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"זכור את גודל חלון הקטגוריות" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"מזער את חלון הקטגוריות אוטומטית" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"הגדל את עוצמת SimonSays בזמן הנגינה" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"הנמך זמנית צלילים אחרים בזמן הנגינה" },
  { SETTINGS_OK_BUTTON_ID, L"אישור" },
  { SETTINGS_CANCEL_BUTTON_ID, L"ביטול" }
};

static const std::vector<std::pair<int, const wchar_t *>> HINDI_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"चलाएँ>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"श्रेणियाँ" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"सेटिंग्स" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"डिफ़ॉल्ट पाठ:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"डिफ़ॉल्ट पाठ स्वतः जोड़ें" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"भाषा:" },
  { SETTINGS_VOICE_LABEL_ID, L"आवाज़:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"आवाज़ परीक्षण" },
  { SETTINGS_VOLUME_LABEL_ID, L"वॉल्यूम (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"गति (-10 से 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"चयन करते ही वाक्य बोलें" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"श्रेणी विंडो का आकार याद रखें" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"श्रेणी विंडो को स्वतः छोटा करें" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"चलाते समय SimonSays की आवाज़ बढ़ाएँ" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"चलाते समय अन्य ऑडियो अस्थायी रूप से घटाएँ" },
  { SETTINGS_OK_BUTTON_ID, L"ठीक" },
  { SETTINGS_CANCEL_BUTTON_ID, L"रद्द करें" }
};

static const std::vector<std::pair<int, const wchar_t *>> ITALIAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Riproduci>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorie" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Impostazioni" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Testo predefinito:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Inserisci automaticamente il testo predefinito" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Lingua:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voce:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Prova voce" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocità (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Pronuncia subito la frase quando la selezioni" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Ricorda la dimensione della finestra delle categorie" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Riduci automaticamente la finestra delle categorie" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumenta il volume di SimonSays durante la riproduzione" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Riduci temporaneamente gli altri audio durante la riproduzione" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Annulla" }
};

static const std::vector<std::pair<int, const wchar_t *>> JAPANESE_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"再生>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"カテゴリ" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"設定" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"既定のテキスト:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"既定のテキストを自動挿入" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"言語:" },
  { SETTINGS_VOICE_LABEL_ID, L"音声:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"音声をテスト" },
  { SETTINGS_VOLUME_LABEL_ID, L"音量 (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"速度 (-10 ～ 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"選択したらすぐにフレーズを話す" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"カテゴリ ウィンドウのサイズを記憶" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"カテゴリ ウィンドウを自動的に最小化" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"再生中に SimonSays の音量を上げる" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"再生中に他の音声を一時的に下げる" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"キャンセル" }
};

static const std::vector<std::pair<int, const wchar_t *>> KOREAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"재생>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"카테고리" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"설정" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"기본 텍스트:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"기본 텍스트 자동 삽입" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"언어:" },
  { SETTINGS_VOICE_LABEL_ID, L"음성:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"음성 테스트" },
  { SETTINGS_VOLUME_LABEL_ID, L"볼륨 (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"속도 (-10~10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"선택 시 문장을 바로 말하기" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"카테고리 창 크기 기억" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"카테고리 창 자동 최소화" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"재생 중 SimonSays 볼륨 증가" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"재생 중 다른 오디오 임시 감소" },
  { SETTINGS_OK_BUTTON_ID, L"확인" },
  { SETTINGS_CANCEL_BUTTON_ID, L"취소" }
};

static const std::vector<std::pair<int, const wchar_t *>> PORTUGUESE_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Reproduzir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorias" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configurações" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texto padrão:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Inserir texto padrão automaticamente" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voz:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Testar voz" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocidade (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Falar a frase imediatamente ao selecionar" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Lembrar o tamanho da janela de categorias" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar automaticamente a janela de categorias" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumentar o volume do SimonSays ao reproduzir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Reduzir temporariamente outros áudios ao reproduzir" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancelar" }
};

static const std::vector<std::pair<int, const wchar_t *>> RUSSIAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Воспроизвести>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Категории" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Настройки" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Текст по умолчанию:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Автоматически вставлять текст по умолчанию" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Язык:" },
  { SETTINGS_VOICE_LABEL_ID, L"Голос:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Проверить голос" },
  { SETTINGS_VOLUME_LABEL_ID, L"Громкость (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Скорость (-10 до 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Произносить фразу сразу при выборе" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Запоминать размер окна категорий" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Автоматически сворачивать окно категорий" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Повышать громкость SimonSays при воспроизведении" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Временно снижать другие звуки при воспроизведении" },
  { SETTINGS_OK_BUTTON_ID, L"ОК" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Отмена" }
};

static const std::vector<std::pair<int, const wchar_t *>> VALENCIAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Reproduir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuració" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Text predeterminat:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insereix el text predeterminat automàticament" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Veu:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Provar veu" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volum (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocitat (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Parla la frase immediatament quan la selecciones" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Recorda la mida de la finestra de categories" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimitza automàticament la finestra de categories" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Augmenta el volum de SimonSays en reproduir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Redueix temporalment altres sons en reproduir" },
  { SETTINGS_OK_BUTTON_ID, L"D'acord" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel·lar" }
};

static const  std::vector < std::pair < std::wstring, std::vector<std::pair<int, const wchar_t *>>>> LOCALIZED_STRINGS = {
  { L"English", DEFAULT_LOCALIZED_UI_STRINGS },
  { L"Spanish", SPANISH_LOCALIZED_UI_STRINGS },
  { L"Arabic", ARABIC_LOCALIZED_UI_STRINGS },
  { L"Basque", BASQUE_LOCALIZED_UI_STRINGS },
  { L"Catalan", CATALAN_LOCALIZED_UI_STRINGS },
  { L"Chinese (Simplified)", CHINESE_SIMPLIFIED_LOCALIZED_UI_STRINGS },
  { L"French", FRENCH_LOCALIZED_UI_STRINGS },
  { L"Galician", GALICIAN_LOCALIZED_UI_STRINGS },
  { L"German", GERMAN_LOCALIZED_UI_STRINGS },
  { L"Hebrew", HEBREW_LOCALIZED_UI_STRINGS },
  { L"Hindi", HINDI_LOCALIZED_UI_STRINGS },
  { L"Italian", ITALIAN_LOCALIZED_UI_STRINGS },
  { L"Japanese", JAPANESE_LOCALIZED_UI_STRINGS },
  { L"Korean", KOREAN_LOCALIZED_UI_STRINGS },
  { L"Portuguese", PORTUGUESE_LOCALIZED_UI_STRINGS },
  { L"Russian", RUSSIAN_LOCALIZED_UI_STRINGS },
  { L"Valencian", VALENCIAN_LOCALIZED_UI_STRINGS }
};

#define DEFAULT_FREQUENT_PHRASES_CATEGORIES_ALL_LANGUAGES \
  std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>>{ \
    { L"Arabic", { \
      { L"عبارات شائعة", L"مرحبا|نعم|لا|من فضلك|شكرا|آسف|عذرا|مساعدة|وداعا|لا أعرف|ربما|أنا أفهم|لا أفهم|هل يمكنك التكرار؟|أحتاج لحظة" }, \
      { L"تحيات متكررة", L"مرحبا|أهلا بك|صباح الخير|مساء الخير|مساء الخير|أهلا وسهلا|تشرفت بلقائك|كيف حالك؟|سعيد برؤيتك|لم نرك منذ وقت طويل" }, \
      { L"عبارات مهذبة", L"من فضلك|شكرا|عفوا|عذرا|هل يمكنني؟|هل يمكنك مساعدتي؟|أقدر ذلك|اعذرني|أتمنى لك يوما سعيدا|أعتذر" }, \
      { L"عبارات الطوارئ", L"مساعدة|اتصل بـ 112|أحتاج مساعدة|أنا مصاب|حالة طوارئ طبية|حريق|اتصل بالشرطة|لا أستطيع التنفس|هناك حادث|أين المستشفى" }, \
      { L"عبارات السفر", L"أين الحمام|كم يكلف هذا|لا أفهم|هل تتحدث الإنجليزية؟|أين أنا؟|أحتاج سيارة أجرة|خذني إلى المطار|كم الساعة|هل يمكنك أن تريني على الخريطة|هل هذا المقعد مأخوذ؟" }, \
      { L"عبارات الأعمال", L"صباح الخير|كيف حالك|تشرفت بلقائك|شكرا لوقتك|أقدر ذلك|أتطلع لذلك|يرجى إرسال التفاصيل|لنحدد موعدا للاجتماع|هل يمكننا إعادة الجدولة؟|سأتابع الأمر" }, \
      { L"عبارات اجتماعية", L"كيف حالك؟|ما الجديد؟|أنا بخير، شكرا|مبروك|عيد ميلاد سعيد|حظا سعيدا|أحسنت|هذا مثير للاهتمام|أخبرني المزيد|أراك قريبا" }, \
      { L"عبارات صحية", L"أحتاج طبيبا|لدي حساسية من...|لدي حمى|أنا مريض بالسكري|أحتاج دواء|أين الرعاية العاجلة|أشعر بالدوار|أواجه صعوبة في التنفس|ألم هنا" }, \
      { L"العائلة والمنزل", L"أمي|أبي|أخي|أختي|أنا في المنزل|أين هاتفي|هل العشاء جاهز؟|أنا مغادر|سأعود حالا|تصبح على خير" }, \
      { L"الوقت والتاريخ", L"كم الساعة|اليوم|غدا|أمس|خلال ساعة|الآن|لاحقا|هذا الصباح|هذا المساء|الأسبوع القادم" }, \
      { L"أصوات", L"تصفيق::applause.wav|ضحك::laugh.wav|بيب::beep.wav|رنين الهاتف::phone_ring.wav|جرس الباب::doorbell.wav|تنبيه::alert.wav" } \
    } }, \
    { L"Basque", { \
      { L"Esaldi Arruntak", L"Kaixo|Bai|Ez|Mesedez|Esk errik asko|Barkatu|Barkatu (mesedez)|Laguntza|Agur|Ez dakit|Agian|Ulertzen dut|Ez dut ulertzen|Errepika dezakezu?|Une bat behar dut" }, \
      { L"Agur Ohikoak", L"Kaixo|Aupa|Egun on|Arratsalde on|Gabon|Ongi etorri|Atsegin dut ezagutzea|Zer moduz?|Pozten naiz zu ikusteaz|Aspaldiko!" }, \
      { L"Esaldi Adeitsuak", L"Mesedez|Eskerrik asko|Ez horregatik|Barkatu|Ahal dut?|Lagundu ahal didazu?|Eskertzen dizut|Barkatu (mesedez)|Egun ona izan|Sentitzen dut" }, \
      { L"Larrialdiko Esaldiak", L"Laguntza|Deitu 112ra|Laguntza behar dut|Zaurituta nago|Larrialdi medikoa|Sua|Deitu poliziari|Ezin dut arnasa hartu|Istripua dago|Non dago ospitalea?" }, \
      { L"Bidaia Esaldiak", L"Non dago komuna?|Zenbat balio du?|Ez dut ulertzen|Ingelesez hitz egiten duzu?|Non nago?|Taxi bat behar dut|Eraman nazazu aireportura|Zer ordu da?|Mapan erakutsi ahal didazu?|Eserleku hau hartuta dago?" }, \
      { L"Negozio Esaldiak", L"Egun on|Zer moduz?|Atsegin dut ezagutzea|Eskerrik asko zure denboragatik|Eskertzen dut|Irrikitan nago|Mesedez bidali xehetasunak|Egin dezagun bilera bat|Aldatu dezakegu data?|Jarraipena egingo dut" }, \
      { L"Esaldi Sozialak", L"Zer moduz?|Ondo nago, eskerrik asko|Zorionak|Zorionak zure urtebetetzean|Zorte on|Oso ondo|Interesgarria da|Kontatu gehiago|Laster arte" }, \
      { L"Osasun Esaldiak", L"Medikua behar dut|Alergia dut...|Sukarra dut|Diabetikoa naiz|Sendagaia behar dut|Non dago larrialdi-zerbitzua?|Zorabioa dut|Arnasa hartzeko arazoak ditut|Min hemen" }, \
      { L"Familia eta Etxea", L"Ama|Aita|Anaia|Arreba|Etxean nago|Non dago nire telefonoa?|Afaria prest al dago?|Joango naiz|Berehala itzuliko naiz|Gabon" }, \
      { L"Ordua eta Data", L"Zer ordu da?|Gaur|Bihar|Atzo|Ordubete barru|Orain|Gero|Gaur goizean|Gaur arratsaldean|Hurrengo astean" }, \
      { L"Soinuak", L"Txaloak::applause.wav|Barreak::laugh.wav|Bip::beep.wav|Telefonoa::phone_ring.wav|Ate-txirrina::doorbell.wav|Alerta::alert.wav" } \
    } }, \
    { L"Catalan", { \
      { L"Frases Comunes", L"Hola|Sí|No|Si us plau|Gràcies|Perdó|Disculpi|Ajuda|Adéu|No ho sé|Potser|Ho entenc|No ho entenc|Ho pot repetir?|Necessito un moment" }, \
      { L"Salutacions Freqüents", L"Hola|Ei|Bon dia|Bona tarda|Bon vespre|Benvingut|Encantat de conèixer-te|Com estàs?|M'alegro de veure't|Quant de temps!" }, \
      { L"Frases Corteses", L"Si us plau|Gràcies|De res|Disculpi|Puc... ?|Em pot ajudar?|Li ho agraeixo|Perdoni|Que tingui un bon dia|Em sap greu" }, \
      { L"Frases d'Emergència", L"Ajuda|Truqui al 112|Necessito ajuda|Estic ferit|Emergència mèdica|Foc|Truqui a la policia|No puc respirar|Hi ha un accident|On és l'hospital?" }, \
      { L"Frases de Viatge", L"On és el lavabo?|Quant costa?|No ho entenc|Parla anglès?|On sóc?|Necessito un taxi|Porti'm a l'aeroport|Quina hora és?|M'ho pot mostrar al mapa?|Aquest seient està ocupat?" }, \
      { L"Frases de Negocis", L"Bon dia|Com està?|Encantat de conèixer-lo|Gràcies pel seu temps|Li ho agraeixo|Amb ganes|Si us plau, enviï els detalls|Programem una reunió|Podem reprogramar?|Faré seguiment" }, \
      { L"Frases Socials", L"Com estàs?|Estic bé, gràcies|Felicitats|Per molts anys|Bona sort|Molt bé|És interessant|Explica-m'ho|Fins aviat" }, \
      { L"Frases de Salut", L"Necessito un metge|Sóc al·lèrgic a...|Tinc febre|Sóc diabètic|Necessito medicació|On és urgències?|Em marejo|Tinc dificultat per respirar|Mal aquí" }, \
      { L"Família i Casa", L"Mare|Pare|Germà|Germana|Sóc a casa|On és el meu telèfon?|El sopar és a punt?|Me'n vaig|Ara torno|Bona nit" }, \
      { L"Hora i Data", L"Quina hora és?|Hui|Demà|Ahir|D'ací a una hora|Ara|Més tard|Este matí|Esta nit|La setmana que ve" }, \
      { L"Sons", L"Aplaudiments::applause.wav|Rialla::laugh.wav|Bip::beep.wav|Telèfon::phone_ring.wav|Timbre::doorbell.wav|Alerta::alert.wav" } \
    } }, \
    { L"Chinese (Simplified)", { \
      { L"常用短语", L"你好|是|不是|请|谢谢|对不起|不好意思|帮帮我|再见|我不知道|也许|我明白|我不明白|你能再说一遍吗|我需要一点时间" }, \
      { L"常见问候", L"你好|嗨|早上好|下午好|晚上好|欢迎|很高兴认识你|你好吗|很高兴见到你|好久不见" }, \
      { L"礼貌用语", L"请|谢谢|不客气|不好意思|可以吗|你能帮我吗|非常感谢|抱歉|祝你有美好的一天|我很抱歉" }, \
      { L"紧急用语", L"救命|请拨打112|我需要帮助|我受伤了|医疗紧急情况|着火了|报警|我无法呼吸|发生事故了|医院在哪里" }, \
      { L"旅行用语", L"洗手间在哪里|多少钱|我不明白|你会说英语吗|我在哪里|我需要一辆出租车|带我去机场|现在几点|你能在地图上指给我看吗|这个座位有人吗" }, \
      { L"商务用语", L"早上好|你好吗|很高兴认识你|感谢你的时间|非常感谢|期待合作|请发送详细信息|我们安排一次会议|可以改期吗|我会跟进" }, \
      { L"社交用语", L"你好吗|我很好，谢谢|恭喜|生日快乐|祝你好运|做得好|很有意思|多说一点|回头见" }, \
      { L"健康用语", L"我需要医生|我对...过敏|我发烧了|我是糖尿病患者|我需要药|急诊在哪里|我头晕|我呼吸困难|这里疼" }, \
      { L"家庭与家", L"妈妈|爸爸|哥哥|姐姐|我在家|我的手机在哪里|晚饭好了吗|我走了|马上回来|晚安" }, \
      { L"时间与日期", L"现在几点|今天|明天|昨天|一小时后|现在|稍后|今天早上|今天晚上|下周" }, \
      { L"声音", L"掌声::applause.wav|笑声::laugh.wav|哔声::beep.wav|电话铃声::phone_ring.wav|门铃::doorbell.wav|警报::alert.wav" } \
    } }, \
    { L"English", { \
      { L"Common Phrases", L"Hello|Yes|No|Please|Thank you|Sorry|Excuse me|Help|Goodbye|I don't know|Maybe|I understand|I don't understand|Can you repeat?|I need a moment" }, \
      { L"Frequent Greetings", L"Hello|Hi there|Good morning|Good afternoon|Good evening|Welcome|Nice to meet you|How are you?|Glad to see you|Long time no see" }, \
      { L"Polite Phrases", L"Please|Thank you|You're welcome|Excuse me|May I?|Could you help me?|I appreciate it|Pardon me|Have a nice day|I apologize" }, \
      { L"Emergency Phrases", L"Help|Call 911|I need help|I am hurt|Medical emergency|Fire|Call the police|I can't breathe|There's an accident|Where is the hospital" }, \
      { L"Travel Phrases", L"Where is the bathroom|How much does it cost|I don't understand|Do you speak English?|Where am I?|I need a taxi|Take me to the airport|What time is it|Can you show me on the map|Is this seat taken?" }, \
      { L"Business Phrases", L"Good morning|How are you|Nice to meet you|Thank you for your time|I appreciate it|Looking forward to it|Please send details|Let's schedule a meeting|Can we reschedule?|I will follow up" }, \
      { L"Social Phrases", L"How are you?|What's up?|I'm fine, thanks|Congratulations|Happy birthday|Good luck|Well done|That's interesting|Tell me more|See you soon" }, \
      { L"Health Phrases", L"I need a doctor|I am allergic to...|I have a fever|I am diabetic|I need medication|Where is urgent care|I feel dizzy|I am having trouble breathing|Pain here" }, \
      { L"Family & Home", L"Mom|Dad|Brother|Sister|I am home|Where is my phone|Is dinner ready?|I'm leaving|Be right back|Good night" }, \
      { L"Time & Date", L"What time is it|Today|Tomorrow|Yesterday|In an hour|Now|Later|This morning|This evening|Next week" }, \
      { L"Sounds", L"Applause::applause.wav|Laugh::laugh.wav|Beep::beep.wav|Phone ring::phone_ring.wav|Doorbell::doorbell.wav|Alert::alert.wav" } \
    } }, \
    { L"French", { \
      { L"Phrases Courantes", L"Bonjour|Oui|Non|S'il vous plaît|Merci|Pardon|Excusez-moi|Aidez-moi|Au revoir|Je ne sais pas|Peut-être|Je comprends|Je ne comprends pas|Pouvez-vous répéter?|J'ai besoin d'un moment" }, \
      { L"Salutations Fréquentes", L"Bonjour|Salut|Bon matin|Bon après-midi|Bonsoir|Bienvenue|Enchanté|Comment ça va?|Content de vous voir|Ça fait longtemps" }, \
      { L"Phrases Polies", L"S'il vous plaît|Merci|De rien|Excusez-moi|Puis-je?|Pouvez-vous m'aider?|Je vous remercie|Pardonnez-moi|Bonne journée|Je m'excuse" }, \
      { L"Phrases d'Urgence", L"Au secours|Appelez le 15|J'ai besoin d'aide|Je suis blessé|Urgence médicale|Incendie|Appelez la police|Je ne peux pas respirer|Il y a un accident|Où est l'hôpital?" }, \
      { L"Phrases de Voyage", L"Où sont les toilettes?|Combien ça coûte?|Je ne comprends pas|Parlez-vous anglais?|Où suis-je?|J'ai besoin d'un taxi|Emmenez-moi à l'aéroport|Quelle heure est-il?|Pouvez-vous me montrer sur la carte?|Ce siège est-il libre?" }, \
      { L"Phrases d'Affaires", L"Bonjour|Comment allez-vous?|Enchanté|Merci pour votre temps|Je l'apprécie|Au plaisir|Veuillez envoyer les détails|Planifions une réunion|Pouvons-nous reprogrammer?|Je ferai le suivi" }, \
      { L"Social", L"Ça va?|Je vais bien, merci|Félicitations|Joyeux anniversaire|Bonne chance|Bien joué|C'est intéressant|Racontez-moi|À bientôt" }, \
      { L"Santé", L"J'ai besoin d'un médecin|Je suis allergique à...|J'ai de la fièvre|Je suis diabétique|J'ai besoin de médicaments|Où est les urgences?|Je me sens étourdi|J'ai du mal à respirer|Douleur ici" }, \
      { L"Famille & Maison", L"Maman|Papa|Frère|Soeur|Je suis à la maison|Où est mon téléphone?|Le dîner est-il prêt?|Je pars|Je reviens|Bonne nuit" }, \
      { L"Temps & Date", L"Quelle heure est-il?|Aujourd'hui|Demain|Hier|Dans une heure|Maintenant|Plus tard|Ce matin|Ce soir|La semaine prochaine" }, \
      { L"Sons", L"Applaudissements::applause.wav|Rire::laugh.wav|Bip::beep.wav|Téléphone::phone_ring.wav|Sonnette::doorbell.wav|Alerte::alert.wav" } \
    } }, \
    { L"Galician", { \
      { L"Frases Comúns", L"Ola|Si|Non|Por favor|Grazas|Perdón|Desculpe|Axuda|Adeus|Non o sei|Quizais|Entendo|Non entendo|Pode repetir?|Necesito un momento" }, \
      { L"Saúdos Frecuentes", L"Ola|Ei|Bos días|Boas tardes|Boas noites|Benvido|Encantado de coñecerte|Como estás?|Alégrome de verte|Canto tempo!" }, \
      { L"Frases Corteses", L"Por favor|Grazas|De nada|Desculpe|Podo...?|Pode axudarme?|Agradézollo|Perdoe|Que teña un bo día|Síntoo" }, \
      { L"Frases de Emerxencia", L"Axuda|Chame ao 112|Necesito axuda|Estou ferido|Emerxencia médica|Lume|Chame á policía|Non podo respirar|Hai un accidente|Onde está o hospital?" }, \
      { L"Frases de Viaxe", L"Onde está o baño?|Canto custa?|Non entendo|Fala inglés?|Onde estou?|Necesito un taxi|Léveme ao aeroporto|Que hora é?|Pode mostralo no mapa?|Este asento está ocupado?" }, \
      { L"Frases de Negocios", L"Bos días|Como está?|Encantado de coñecelo|Grazas polo seu tempo|Agradézollo|Con ganas|Por favor envíe os detalles|Programemos unha reunión|Podemos reprogramar?|Farei seguimento" }, \
      { L"Frases Sociais", L"Como estás?|Estou ben, grazas|Parabéns|Feliz aniversario|Boa sorte|Moi ben|É interesante|Cóntame máis|Ata logo" }, \
      { L"Frases de Saúde", L"Necesito un médico|Son alérxico a...|Teño febre|Son diabético|Necesito medicación|Onde está urxencias?|Síntome mareado|Teño dificultade para respirar|Dói aquí" }, \
      { L"Familia e Fogar", L"Nai|Pai|Irmán|Irmá|Estou na casa|Onde está o meu teléfono?|A cea está lista?|Voume|Volvo axiña|Boas noites" }, \
      { L"Hora e Data", L"Que hora é?|Hoxe|Mañá|Onte|Nunha hora|Agora|Máis tarde|Esta mañá|Esta noite|A vindeira semana" }, \
      { L"Sons", L"Aplausos::applause.wav|Risos::laugh.wav|Bip::beep.wav|Teléfono::phone_ring.wav|Campá::doorbell.wav|Alerta::alert.wav" } \
    } }, \
    { L"German", { \
      { L"Allgemeine Sätze", L"Hallo|Ja|Nein|Bitte|Danke|Entschuldigung|Verzeihung|Hilfe|Auf Wiedersehen|Ich weiß nicht|Vielleicht|Ich verstehe|Ich verstehe nicht|Können Sie das wiederholen?|Ich brauche einen Moment" }, \
      { L"Häufige Grüße", L"Hallo|Guten Tag|Guten Morgen|Guten Nachmittag|Guten Abend|Willkommen|Freut mich|Wie geht's?|Schön dich zu sehen|Lange nicht gesehen" }, \
      { L"Höfliche Sätze", L"Bitte|Danke|Gern geschehen|Entschuldigung|Darf ich?|Können Sie mir helfen?|Ich danke Ihnen|Verzeihen Sie mir|Einen schönen Tag noch|Es tut mir leid" }, \
      { L"Notfall Sätze", L"Hilfe|Rufen Sie 112 an|Ich brauche Hilfe|Ich bin verletzt|Medizinischer Notfall|Feuer|Rufen Sie die Polizei|Ich kann nicht atmen|Es gab einen Unfall|Wo ist das Krankenhaus?" }, \
      { L"Reise Sätze", L"Wo ist die Toilette?|Wie viel kostet das?|Ich verstehe nicht|Sprechen Sie Englisch?|Wo bin ich?|Ich brauche ein Taxi|Bringen Sie mich zum Flughafen|Wie spät ist es?|Können Sie es mir auf der Karte zeigen?|Ist dieser Platz frei?" }, \
      { L"Geschäftliche Sätze", L"Guten Morgen|Wie geht es Ihnen?|Freut mich|Danke für Ihre Zeit|Ich schätze es|Ich freue mich darauf|Bitte senden Sie Details|Lassen Sie uns ein Treffen planen|Können wir neu terminieren?|Ich melde mich" }, \
      { L"Soziales", L"Wie geht's?|Mir geht's gut, danke|Glückwunsch|Alles Gute zum Geburtstag|Viel Glück|Gut gemacht|Das ist interessant|Erzähl mir mehr|Bis bald" }, \
      { L"Gesundheit", L"Ich brauche einen Arzt|Ich bin allergisch gegen...|Ich habe Fieber|Ich bin Diabetiker|Ich brauche Medikamente|Wo ist die Notaufnahme?|Mir ist schwindelig|Ich habe Atembeschwerden|Schmerz hier" }, \
      { L"Familie & Zuhause", L"Mama|Papa|Bruder|Schwester|Ich bin zu Hause|Wo ist mein Telefon?|Ist das Abendessen fertig?|Ich gehe|Bin gleich zurück|Gute Nacht" }, \
      { L"Zeit & Datum", L"Wie spät ist es?|Heute|Morgen|Gestern|In einer Stunde|Jetzt|Später|Heute Morgen|Heute Abend|Nächste Woche" }, \
      { L"Sounds", L"Applaus::applause.wav|Lachen::laugh.wav|Piepton::beep.wav|Telefonklingeln::phone_ring.wav|Türklingel::doorbell.wav|Alarm::alert.wav" } \
    } }, \
    { L"Hebrew", { \
      { L"ביטויים נפוצים", L"שלום|כן|לא|בבקשה|תודה|סליחה|סליחה|עזרה|להתראות|אני לא יודע|אולי|אני מבין|אני לא מבין|אפשר לחזור על זה?|אני צריך רגע" }, \
      { L"ברכות נפוצות", L"שלום|היי|בוקר טוב|צהריים טובים|ערב טוב|ברוכים הבאים|נעים מאוד|מה שלומך?|שמח לראות אותך|הרבה זמן לא התראינו" }, \
      { L"ביטויים מנומסים", L"בבקשה|תודה|בבקשה (אין בעד מה)|סליחה|אפשר?|אפשר לעזור לי?|אני מעריך/ה את זה|סליחה|יום נעים|אני מצטער/ת" }, \
      { L"ביטויי חירום", L"עזרה|תתקשרו ל-112|אני צריך עזרה|נפצעתי|מקרה חירום רפואי|שריפה|תתקשרו למשטרה|אני לא מצליח לנשום|תאונה הייתה|איפה בית החולים?" }, \
      { L"ביטויי נסיעות", L"איפה השירותים?|כמה זה עולה?|אני לא מבין|אתה מדבר אנגלית?|איפה אני?|אני צריך מונית|קח אותי לשדה התעופה|מה השעה?|אפשר להראות לי במפה?|המקום הזה תפוס?" }, \
      { L"ביטויים עסקיים", L"בוקר טוב|מה שלומך?|נעים מאוד|תודה על הזמן שלך|אני מעריך/ה את זה|מצפה לזה|בבקשה שלחו פרטים|בוא נקבע פגישה|אפשר לדחות?|אחזור אליך" }, \
      { L"Bיטויים חברתיים", L"מה שלומך?|אני בסדר, תודה|מזל טוב|יום הולדת שמח|בהצלחה|כל הכבוד|מעניין|ספר/י לי עוד|נתראה בקרוב" }, \
      { L"ביטויי בריאות", L"אני צריך רופא|אני אלרגי ל...|יש לי חום|אני חולה סוכרת|אני צריך תרופות|איפה מיון?|אני מרגיש סחרחורת|קשה לי לנשום|כואב כאן" }, \
      { L"משפחה ובית", L"אמא|אבא|אח|אחות|אני בבית|איפה הטלפון שלי?|הארוחה מוכנה?|אני יוצא/ת|מיד חוזר/ת|לילה טוב" }, \
      { L"זמן ותאריך", L"מה השעה?|היום|מחר|אתמול|בעוד שעה|עכשיו|אחר כך|הבוקר|הערב|שבוע הבא" }, \
      { L"צלילים", L"מחיאות כפיים::applause.wav|צחוק::laugh.wav|ביפ::beep.wav|טלפון מצלצל::phone_ring.wav|פעמון דלת::doorbell.wav|התראה::alert.wav" } \
    } }, \
    { L"Hindi", { \
      { L"सामान्य वाक्य", L"नमस्ते|हाँ|नहीं|कृपया|धन्यवाद|माफ़ कीजिए|क्षमा करें|मदद|अलविदा|मुझे नहीं पता|शायद|मैं समझ गया/गई|मैं नहीं समझा/समझी|क्या आप दोहरा सकते हैं?|मुझे एक पल चाहिए" }, \
      { L"अक्सर अभिवादन", L"नमस्ते|हाय|सुप्रभात|शुभ दोपहर|शुभ संध्या|स्वागत है|आपसे मिलकर खुशी हुई|आप कैसे हैं?|आपको देखकर खुशी हुई|बहुत दिन हो गए" }, \
      { L"विनम्र वाक्य", L"कृपया|धन्यवाद|कोई बात नहीं|माफ़ कीजिए|क्या मैं... ?|क्या आप मेरी मदद कर सकते हैं?|मैं आभारी हूँ|क्षमा करें|आपका दिन शुभ हो|मुझे खेद है" }, \
      { L"आपातकालीन वाक्य", L"मदद|112 पर कॉल करें|मुझे मदद चाहिए|मैं घायल हूँ|चिकित्सा आपातकाल|आग|पुलिस को बुलाएँ|मैं साँस नहीं ले पा रहा/रही|दुर्घटना हुई है|अस्पताल कहाँ है" }, \
      { L"यात्रा वाक्य", L"शौचालय कहाँ है|यह कितने का है|मुझे समझ नहीं आया|क्या आप अंग्रेज़ी बोलते हैं?|मैं कहाँ हूँ|मुझे टैक्सी चाहिए|मुझे हवाई अड्डे ले चलें|समय क्या हुआ है|क्या आप नक्शे पर दिखा सकते हैं?|क्या यह सीट खाली है" }, \
      { L"व्यवसाय वाक्य", L"सुप्रभात|आप कैसे हैं|आपसे मिलकर खुशी हुई|आपके समय के लिए धन्यवाद|मैं आभारी हूँ|मैं उत्सुक हूँ|कृपया विवरण भेजें|आइए बैठक तय करें|क्या हम समय बदल सकते हैं?|मैं फॉलो-अप करूँगा/करूँगी" }, \
      { L"सामाजिक वाक्य", L"आप कैसे हैं?|मैं ठीक हूँ, धन्यवाद|बधाई|जन्मदिन मुबारक|शुभकामनाएँ|बहुत अच्छा|यह रोचक है|और बताइए|फिर मिलेंगे" }, \
      { L"स्वास्थ्य वाक्य", L"मुझे डॉक्टर चाहिए|मुझे ... से एलर्जी है|मुझे बुखार है|मैं मधुमेह का रोगी हूँ|मुझे दवा चाहिए|आपातकाल कहाँ है|मुझे चक्कर आ रहे हैं|मुझे साँस लेने में परेशानी है|यहाँ दर्द है" }, \
      { L"परिवार और घर", L"माँ|पिता|भाई|बहन|मैं घर पर हूँ|मेरा फ़ोन कहाँ है|क्या खाना तैयार है|मैं जा रहा/रही हूँ|अभी आता/आती हूँ|शुभ रात्रि" }, \
      { L"समय और तारीख", L"समय क्या हुआ है|आज|कल|बीता कल|एक घंटे में|अभी|बाद में|आज सुबह|आज शाम|अगले हफ्ते" }, \
      { L"ध्वनियाँ", L"तालियाँ::applause.wav|हँसी::laugh.wav|बीप::beep.wav|फ़ोन की घंटी::phone_ring.wav|डोरबेल::doorbell.wav|अलर्ट::alert.wav" } \
    } }, \
    { L"Italian", { \
      { L"Frasi Comuni", L"Ciao|Sì|No|Per favore|Grazie|Scusa|Mi scusi|Aiuto|Arrivederci|Non lo so|Forse|Capisco|Non capisco|Puoi ripetere?|Ho bisogno di un momento" }, \
      { L"Saluti Frequenti", L"Ciao|Salve|Buongiorno|Buon pomeriggio|Buonasera|Benvenuto|Piacere|Come stai?|Felice di vederti|Da quanto tempo" }, \
      { L"Frasi Cortesi", L"Per favore|Grazie|Prego|Mi scusi|Posso?|Puoi aiutarmi?|Te ne sono grato|Scusami|Buona giornata|Mi scuso" }, \
      { L"Frasi di Emergenza", L"Aiuto|Chiamate il 112|Ho bisogno di aiuto|Sono ferito|Emergenza medica|Incendio|Chiamate la polizia|Non riesco a respirare|C'è un incidente|Dov'è l'ospedale?" }, \
      { L"Frasi di Viaggio", L"Dov'è il bagno?|Quanto costa?|Non capisco|Parli inglese?|Dove sono?|Ho bisogno di un taxi|Portami all'aeroporto|Che ora è?|Puoi mostrarmelo sulla mappa?|Questo posto è libero?" }, \
      { L"Frasi di Affari", L"Buongiorno|Come sta?|Piacere di conoscerla|Grazie per il suo tempo|La apprezzo|Non vedo l'ora|Per favore invii i dettagli|Programmiamo una riunione|Possiamo riprogrammare?|Farò seguito" }, \
      { L"Sociale", L"Come va?|Sto bene, grazie|Congratulazioni|Buon compleanno|In bocca al lupo|Ben fatto|Interessante|Dimmi di più|A presto" }, \
      { L"Salute", L"Ho bisogno di un medico|Sono allergico a...|Ho la febbre|Sono diabetico|Ho bisogno di medicine|Dov'è il pronto soccorso?|Mi sento stordito|Ho difficoltà a respirare|Dolore qui" }, \
      { L"Famiglia & Casa", L"Mamma|Papà|Fratello|Sorella|Sono a casa|Dov'è il mio telefono?|La cena è pronta?|Sto uscendo|Torno subito|Buonanotte" }, \
      { L"Tempo & Data", L"Che ora è?|Oggi|Domani|Ieri|Tra un'ora|Adesso|Più tardi|Questa mattina|Questa sera|La prossima settimana" }, \
      { L"Suoni", L"Applausi::applause.wav|Risata::laugh.wav|Beep::beep.wav|Suoneria::phone_ring.wav|Campanello::doorbell.wav|Allarme::alert.wav" } \
    } }, \
    { L"Japanese", { \
      { L"よく使うフレーズ", L"こんにちは|はい|いいえ|お願いします|ありがとうございます|ごめんなさい|すみません|助けて|さようなら|わかりません|たぶん|わかります|わかりません|もう一度言ってください|少し時間が必要です" }, \
      { L"よくある挨拶", L"こんにちは|やあ|おはようございます|こんにちは|こんばんは|ようこそ|はじめまして|お元気ですか|会えてうれしいです|お久しぶりです" }, \
      { L"丁寧なフレーズ", L"お願いします|ありがとうございます|どういたしまして|すみません|よろしいですか|手伝ってくれますか|助かります|失礼します|良い一日を|申し訳ありません" }, \
      { L"緊急時のフレーズ", L"助けて|112に電話してください|助けが必要です|けがをしました|医療の緊急事態|火事です|警察を呼んでください|息ができません|事故がありました|病院はどこですか" }, \
      { L"旅行のフレーズ", L"トイレはどこですか|いくらですか|わかりません|英語は話せますか|ここはどこですか|タクシーが必要です|空港へ連れて行ってください|今何時ですか|地図で示してもらえますか|この席は空いていますか" }, \
      { L"ビジネスのフレーズ", L"おはようございます|お元気ですか|はじめまして|お時間ありがとうございます|感謝します|楽しみにしています|詳細を送ってください|会議を設定しましょう|変更できますか|後ほど連絡します" }, \
      { L"社交のフレーズ", L"お元気ですか|元気です、ありがとう|おめでとう|誕生日おめでとう|頑張って|よくできました|興味深いですね|もっと教えて|また近いうちに" }, \
      { L"健康のフレーズ", L"医者が必要です|…にアレルギーがあります|熱があります|糖尿病です|薬が必要です|救急はどこですか|めまいがします|息苦しいです|ここが痛い" }, \
      { L"家族と家", L"お母さん|お父さん|兄弟|姉妹|家にいます|携帯はどこですか|夕食はできていますか|出かけます|すぐ戻ります|おやすみなさい" }, \
      { L"時間と日付", L"今何時ですか|今日|明日|昨日|1時間後|今|後で|今朝|今晩|来週" }, \
      { L"音", L"拍手::applause.wav|笑い::laugh.wav|ビープ::beep.wav|着信音::phone_ring.wav|ドアベル::doorbell.wav|警告::alert.wav" } \
    } }, \
    { L"Korean", { \
      { L"자주 쓰는 문장", L"안녕하세요|네|아니요|부탁합니다|감사합니다|죄송합니다|실례합니다|도와주세요|안녕히 가세요|모르겠어요|아마도|이해합니다|이해하지 못했어요|다시 말씀해 주시겠어요?|잠시만요" }, \
      { L"자주 하는 인사", L"안녕하세요|안녕|좋은 아침입니다|좋은 오후입니다|좋은 저녁입니다|환영합니다|만나서 반갑습니다|어떻게 지내세요?|반갑습니다|오랜만이에요" }, \
      { L"공손한 표현", L"부탁합니다|감사합니다|천만에요|실례합니다|괜찮을까요?|도와주실 수 있나요?|정말 감사합니다|죄송하지만|좋은 하루 되세요|사과드립니다" }, \
      { L"긴급 상황", L"도와주세요|112에 전화해 주세요|도움이 필요해요|다쳤어요|의료 응급상황|불이 났어요|경찰을 불러 주세요|숨을 쉴 수 없어요|사고가 났어요|병원이 어디예요" }, \
      { L"여행", L"화장실이 어디예요|얼마예요|이해가 안 돼요|영어 하세요?|여기가 어디예요?|택시가 필요해요|공항으로 데려다 주세요|지금 몇 시예요|지도에서 보여 주실 수 있나요?|이 자리 비었나요" }, \
      { L"비즈니스", L"좋은 아침입니다|어떻게 지내세요?|만나서 반갑습니다|시간 내주셔서 감사합니다|감사합니다|기대하고 있습니다|자세한 내용을 보내 주세요|회의 일정을 잡읍시다|일정을 변경할 수 있을까요?|추후에 다시 연락드리겠습니다" }, \
      { L"사회", L"어떻게 지내세요?|무슨 일 있어요?|잘 지내요, 감사합니다|축하합니다|생일 축하합니다|행운을 빌어요|잘했어요|흥미롭네요|더 이야기해 주세요|곧 봐요" }, \
      { L"건강", L"의사가 필요해요|...에 알레르기가 있어요|열이 있어요|당뇨가 있어요|약이 필요해요|응급실이 어디예요|어지러워요|숨쉬기 힘들어요|아픔이 있어요" }, \
      { L"가족과 집", L"엄마|아빠|형|동생|집에 있어요|내 핸드폰 어디에요?|저녁 준비됐어요?|나 갈게요|금방 올게요|안녕히 주무세요" }, \
      { L"시간과 날짜", L"지금 몇 시에요?|오늘|내일|어제|한 시간 후|지금|나중에|오늘 아침|오늘 저녁|다음 주" }, \
      { L"소리", L"박수::applause.wav|웃음::laugh.wav|삐 소리::beep.wav|전화벨::phone_ring.wav|초인종::doorbell.wav|경고::alert.wav" } \
    } }, \
    { L"Portuguese", { \
      { L"Frases Comuns", L"Olá|Sim|Não|Por favor|Obrigado|Desculpe|Com licença|Ajuda|Adeus|Não sei|Talvez|Entendo|Não entendo|Pode repetir?|Preciso de um momento" }, \
      { L"Saudações Frequentes", L"Olá|Oi|Bom dia|Boa tarde|Boa noite|Bem-vindo|Prazer em conhecê-lo|Como vai?|Bom ver você|Há quanto tempo" }, \
      { L"Frases Corteses", L"Por favor|Obrigado|De nada|Com licença|Posso?|Pode ajudar-me?|Agradeço|Perdoe-me|Tenha um bom dia|Peço desculpa" }, \
      { L"Frases de Emergência", L"Ajuda|Ligue para 112|Preciso de ajuda|Estou ferido|Emergência médica|Fogo|Chame a polícia|Não consigo respirar|Houve um acidente|Onde é o hospital?" }, \
      { L"Frases de Viagem", L"Onde fica a casa de banho?|Quanto custa?|Não entendo|Fala inglês?|Onde estou?|Preciso de um táxi|Leve-me ao aeroporto|Que horas são?|Pode mostrar-me no mapa?|Este lugar está ocupado?" }, \
      { L"Frases de Negócios", L"Bom dia|Como está?|Prazer em conhecê-lo|Obrigado pelo seu tempo|Agradeço|Aguardando com expectativa|Por favor envie os detalhes|Vamos agendar uma reunião|Podemos reagendar?|Vou acompanhar" }, \
      { L"Social", L"Como vai?|Estou bem, obrigado|Parabéns|Feliz aniversário|Boa sorte|Bem feito|Isso é interessante|Conte-me mais|Até breve" }, \
      { L"Saúde", L"Preciso de um médico|Sou alérgico a...|Tenho febre|Sou diabético|Preciso de medicação|Onde fica o pronto-socorro?|Sinto tonturas|Tenho dificuldade em respirar|Dor aqui" }, \
      { L"Família & Casa", L"Mãe|Pai|Irmão|Irmã|Estou em casa|Onde está o meu telemóvel?|O jantar está pronto?|Estou a sair|Volto já|Boa noite" }, \
      { L"Tempo & Data", L"Que horas são?|Hoje|Amanhã|Ontem|Daqui a uma hora|Agora|Mais tarde|Esta manhã|Esta noite|Próxima semana" }, \
      { L"Sons", L"Aplausos::applause.wav|Riso::laugh.wav|Beep::beep.wav|Toque do telefone::phone_ring.wav|Campainha::doorbell.wav|Alerta::alert.wav" } \
    } }, \
    { L"Russian", { \
      { L"Общие фразы", L"Привет|Да|Нет|Пожалуйста|Спасибо|Извините|Прошу прощения|Помогите|До свидания|Я не знаю|Может быть|Я понимаю|Я не понимаю|Можете повторить?|Мне нужна минутка" }, \
      { L"Частые приветствия", L"Привет|Здравствуйте|Доброе утро|Добрый день|Добрый вечер|Добро пожаловать|Приятно познакомиться|Как дела?|Рад вас видеть|Давно не виделись" }, \
      { L"Вежливые фразы", L"Пожалуйста|Спасибо|Не за что|Извините|Можно?|Вы можете мне помочь?|Я ценю это|Прошу прощения|Хорошего дня|Мне очень жаль" }, \
      { L"Экстренные фразы", L"Помогите|Позвоните 112|Мне нужна помощь|Я ранен(а)|Медицинская помощь|Пожар|Позвоните в полицию|Я не могу дышать|Произошла авария|Где больница?" }, \
      { L"Фразы для путешествий", L"Где туалет?|Сколько это стоит?|Я не понимаю|Вы говорите по-английски?|Где я?|Мне нужно такси|Отвезите меня в аэропорт|Который час?|Можете показать на карте?|Это место занято?" }, \
      { L"Деловые фразы", L"Доброе утро|Как дела?|Приятно познакомиться|Спасибо за ваше время|Я ценю это|С нетерпением жду|Пожалуйста, пришлите детали|Давайте назначим встречу|Можем перенести?|Я свяжусь позже" }, \
      { L"Социальные фразы", L"Как дела?|Я в порядке, спасибо|Поздравляю|С днём рождения|Удачи|Отлично|Это интересно|Расскажите подробнее|До скорого" }, \
      { L"Фразы о здоровье", L"Мне нужен врач|У меня аллергия на...|У меня температура|Я диабетик|Мне нужны лекарства|Где неотложка?|У меня кружится голова|Мне трудно дышать|Болит здесь" }, \
      { L"Семья и дом", L"Мама|Папа|Брат|Сестра|Я дома|Где мой телефон?|Ужин готов?|Я ухожу|Скоро вернусь|Спокойной ночи" }, \
      { L"Время и дата", L"Который час?|Сегодня|Завтра|Вчера|Через час|Сейчас|Позже|Сегодня утром|Сегодня вечером|На следующей неделе" }, \
      { L"Звуки", L"Аплодисменты::applause.wav|Смех::laugh.wav|Сигнал::beep.wav|Звонок телефона::phone_ring.wav|Дверной звонок::doorbell.wav|Предупреждение::alert.wav" } \
    } }, \
    { L"Spanish", { \
      { L"Frases Comunes", L"Hola|Sí|No|Por favor|Gracias|Perdón|Disculpe|Ayuda|Adiós|No sé|Tal vez|Entiendo|No entiendo|¿Puede repetir?|Necesito un momento" }, \
      { L"Saludos Frecuentes", L"Hola|¿Qué tal?|Buenos días|Buenas tardes|Buenas noches|Bienvenido|Mucho gusto|¿Cómo estás?|Me alegra verte|¡Cuánto tiempo sin verte!" }, \
      { L"Frases Corteses", L"Por favor|Gracias|De nada|Disculpe|¿Puedo?|¿Puede ayudarme?|Se lo agradezco|Perdóneme|Que tenga un buen día|Lo siento" }, \
      { L"Frases de Emergencia", L"Ayuda|Llame al 112|Necesito ayuda|Estoy herido|Emergencia médica|Incendio|Llame a la policía|No puedo respirar|Hay un accidente|¿Dónde está el hospital?" }, \
      { L"Frases de Viaje", L"¿Dónde está el baño?|¿Cuánto cuesta?|No entiendo|¿Habla inglés?|¿Dónde estoy?|Necesito un taxi|Lléveme al aeropuerto|¿Qué hora es?|¿Me lo puede mostrar en el mapa?|¿Está ocupado este asiento?" }, \
      { L"Frases de Negocios", L"Buenos días|¿Cómo está?|Mucho gusto|Gracias por su tiempo|Lo aprecio|Espero con interés|Por favor envíe los detalles|Agendemos una reunión|¿Podemos reprogramar?|Le daré seguimiento" }, \
      { L"Social", L"¿Qué tal?|Estoy bien, gracias|Felicidades|Feliz cumpleaños|Buena suerte|Bien hecho|Qué interesante|Cuéntame más|Nos vemos pronto" }, \
      { L"Salud", L"Necesito un doctor|Soy alérgico a...|Tengo fiebre|Soy diabético|Necesito medicación|¿Dónde está urgencias?|Me siento mareado|Tengo dificultad para respirar|Dolor aquí" }, \
      { L"Familia & Hogar", L"Mamá|Papá|Hermano|Hermana|Estoy en casa|¿Dónde está mi teléfono?|¿La cena está lista?|Me voy|Vuelvo enseguida|Buenas noches" }, \
      { L"Tiempo & Fecha", L"¿Qué hora es?|Hoy|Mañana|Ayer|En una hora|Ahora|Más tarde|Esta mañana|Esta noche|La próxima semana" }, \
      { L"Sonidos", L"Aplausos::applause.wav|Risa::laugh.wav|Tono::beep.wav|Teléfono::phone_ring.wav|Timbre::doorbell.wav|Alarma::alert.wav" } \
    } }, \
    { L"Valencian", { \
      { L"Frases Comunes", L"Hola|Sí|No|Per favor|Gràcies|Perdó|Disculpe|Ajuda|Adéu|No ho sé|Potser|Ho entenc|No ho entenc|Ho pot repetir?|Necessite un moment" }, \
      { L"Salutacions Freqüents", L"Hola|Ei|Bon dia|Bona vesprada|Bona nit|Benvingut|Encantat de conéixer-te|Com estàs?|M'alegro de veure't|Quant de temps!" }, \
      { L"Frases Corteses", L"Per favor|Gràcies|De res|Disculpe|Puc...?|Em pot ajudar?|Li ho agraïsc|Perdone|Que tinga un bon dia|Em sap greu" }, \
      { L"Frases d'Emergència", L"Ajuda|Truqueu al 112|Necessite ajuda|Estic ferit|Emergència mèdica|Foc|Truqueu a la policia|No puc respirar|Hi ha un accident|On és l'hospital?" }, \
      { L"Frases de Viatge", L"On és el lavabo?|Quant costa?|No ho entenc|Parla anglés?|On sóc?|Necessite un taxi|Porta'm a l'aeroport|Quina hora és?|M'ho pot mostrar al mapa?|Este seient està ocupat?" }, \
      { L"Frases de Negocis", L"Bon dia|Com està?|Encantat de conéixer-lo|Gràcies pel seu temps|Li ho agraïsc|Amb ganes|Per favor envieu els detalls|Programem una reunió|Podem reprogramar?|Faré seguiment" }, \
      { L"Frases Socials", L"Com estàs?|Estic bé, gràcies|Enhorabona|Feliç aniversari|Bona sorte|Molt bé|És interessant|Conta'm més|Fins prompte" }, \
      { L"Frases de Salut", L"Necessite un metge|Sóc al·lèrgic a...|Tinc febre|Sóc diabètic|Necessite medicació|On és urgències?|Em marege|Tinc dificultat per respirar|Em fa mal ací" }, \
      { L"Família i Casa", L"Mare|Pare|Germà|Germana|Sóc a casa|On és el meu telèfon?|El sopar està a punt?|Me'n vaig|Ara torne|Bona nit" }, \
      { L"Hora i Data", L"Quina hora és?|Hui|Demà|Ahir|D'ací a una hora|Ara|Més tard|Este matí|Esta nit|La setmana que ve" }, \
      { L"Sons", L"Aplaudiments::applause.wav|Rialla::laugh.wav|Bip::beep.wav|Telèfon::phone_ring.wav|Timbre::doorbell.wav|Alerta::alert.wav" } \
    } } \
  }

#endif // stdafx_h