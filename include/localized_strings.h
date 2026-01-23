#ifndef LOCALIZED_STRINGS_H
#define LOCALIZED_STRINGS_H

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
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Click to show" },
  { TRAYICON_SHOW_HIDE_ID, L"Show/Hide" },
  { TRAYICON_EXIT_ID, L"Exit" }
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
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar automáticamente a xanela de categorías" },
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
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Parla la frase immediatament quan la seleciones" },
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

#endif // LOCALIZED_STRINGS_H