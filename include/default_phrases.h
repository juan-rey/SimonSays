
#ifndef DEFAULT_PHRASES_H 
#define DEFAULT_PHRASES_H


#define DEFAULT_FREQUENT_PHRASES_CATEGORIES_ALL_LANGUAGES \
  std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>>{ \
    { L"Arabic", { \
      { L"عبارات محادثة", L"نعم|لا|من فضلك|شكرا|آسف|عذرا|مساعدة|لا أعرف|ربما|أنا أفهم|لا أفهم|هل يمكنك التكرار؟|أحتاج لحظة|هاي|ما الأمر؟|ماذا؟|قل لي" }, \
      { L"تحيات متكررة", L"مرحبا|أهلا|هلا|أهلا وسهلا|صباح الخير|مساء الخير|ليلة سعيدة|تشرفت بلقائك|كيف حالك؟|سعيد برؤيتك|طابت أوقاتك|مرحبا بالجميع|مع السلامة|وداعاً|إلى اللقاء|أراك لاحقاً|سلام|في أمان الله|أراك قريباً|تروح وترجع بالسلامة" }, \
      { L"عبارات مهذبة", L"من فضلك|شكرا|عفوا|عذرا|هل يمكنني؟|هل يمكنك مساعدتي؟|أقدر ذلك|اعذرني|أتمنى لك يوما سعيدب|أعتذر" }, \
      { L"عبارات الطوارئ", L"مساعدة|اتصل بـ 112|أحتاج مساعدة|أنا مصاب|حالة طوارئ طبية|حريق|اتصل بالشرطة|لا أستطيع التنفس|هناك حادث|أين المستشفى" }, \
      { L"عبارات السفر", L"أين الحمام|كم يكلف هذا|لا أفهم|هل تتحدث الإنجليزية؟|أين أنا؟|أحتاج سيارة أجرة|خذني إلى المطار|كم الساعة|هل يمكنك أن تريني على الخريطة|هل هذا المقعد مأخوذ؟" }, \
      { L"عبارات الأعمال", L"صباح الخير|كيف حالك|تشرفت بلقائك|شكرا لوقتك|أقدر ذلك|أتطلع لذلك|يرجى إرسال التفاصيل|لنحدد موعدا للاجتماع|هل يمكننا إعادة الجدولة؟|سأتابع الأمر" }, \
      { L"عبارات اجتماعية", L"كيف حالك؟|أنا بخير، شكرا|ما الجديد؟|مبروك|عيد ميلاد سعيد|حظا سعيدا|أحسنت|هذا مثير للاهتمام|أخبرني المزيد|أراك قريبا" }, \
      { L"عبارات صحية", L"أحتاج طبيبا|لدي حساسية من...|لدي حمى|أنا مريض بالسكري|أحتاج دواء|أين الرعاية العاجلة|أشعر بالدوار|أواجه صعوبة في التنفس|ألم هنا" }, \
      { L"العائلة والمنزل", L"أمي|أبي|أخي|أختي|أنا في المنزل|أين هاتفي|هل العشاء جاهز؟|أنا مغادر|سأعود حالا|تصبح على خير" }, \
      { L"الوقت والتاريخ", L"كم الساعة|اليوم|غدا|أمس|خلال ساعة|الآن|لاحقا|هذا الصباح|هذا المساء|الأسبوع القادم" }, \
      { L"أصوات", L"تصفيق::applause.wav|ضحك::laugh.wav|بيب::beep.wav|رنين الهاتف::phone_ring.wav|جرس الباب::doorbell.wav|تنبيه::alert.wav" } \
    } }, \
    { L"Basque", { \
      { L"Elkarrizketa Esaldiak", L"Bai|Ez|Agian|Mesedez|Esk errik asko|Barkatu|Barkatu (mesedez)|Laguntza|Ez dakit|Ulertzen dut|Ez dut ulertzen|Errepika dezakezu?|Une bat behar dut|Aupa|Zer moduz?|Zer?|Esaidazu" }, \
      { L"Agur Ohikoak", L"Kaixo|Aupa|Kaixo lagunok|Ongi etorri|Ondo etorri|Zer moduz?|Atsegin dut ezagutzea|Pozten naiz zu ikusteaz|Aspaldiko!|Egun on|Egun on guztioi|Arratsalde on|Gabón|Agur|Gero arte|Laster arte|Bihar arte|Ondo izan|Egon zaitez ondo|Besarkada bat" }, \
      { L"Esaldi Adeitsuak", L"Mesedez|Eskerrik asko|Ez horregatik|Barkatu|Barkatu (mesedez)|Ahal dut?|Lagundu ahal didazu?|Eskertzen dizut|Egun ona izan|Sentitzen dut" }, \
      { L"Larrialdiko Esaldiak", L"Laguntza|Deitu 112ra|Laguntza behar dut|Larrialdi medikoa|Zaurituta nago|Sua|Deitu poliziari|Ezin dut arnasa hartu|Istripua dago|Non dago ospitalea?" }, \
      { L"Bidaia Esaldiak", L"Non dago komuna?|Zenbat balio du?|Ez dut ulertzen|Ingelesez hitz egiten duzu?|Non nago?|Taxi bat behar dut|Eraman nazazu aireportura|Zer ordu da?|Mapan erakutsi ahal didazu?|Eserleku hau hartuta dago?" }, \
      { L"Negozio Esaldiak", L"Egun on|Zer moduz?|Atsegin dut ezagutzea|Eskerrik asko zure denboragatik|Eskertzen dut|Irrikitan nago|Mesedez bidali xehetasunak|Egin dezagun bilera bat|Aldatu dezakegu data?|Jarraipena egingo dut" }, \
      { L"Esaldi Sozialak", L"Zer moduz?|Ondo nago, eskerrik asko|Zorionak|Zorionak zure urtebetetzean|Zorte on|Oso ondo|Interesgarria da|Kontatu gehiago|Laster arte" }, \
      { L"Osasun Esaldiak", L"Medikua behar dut|Alergia dut...|Sukarra dut|Diabetikoa naiz|Sendagaia behar dut|Non dago larrialdi-zerbitzua?|Zorabioa dut|Arnasa hartzeko arazoak ditut|Min hemen" }, \
      { L"Familia eta Etxea", L"Ama|Aita|Anaia|Arreba|Etxean nago|Non dago nire telefonoa?|Afaria prest al dago?|Joango naiz|Berehala itzuliko naiz|Gabon" }, \
      { L"Ordua eta Data", L"Zer ordu da?|Gaur|Bihar|Atzo|Ordubete barru|Orain|Gero|Gaur goizean|Gaur arratsaldean|Hurrengo astean" }, \
      { L"Soinuak", L"Txaloak::applause.wav|Barreak::laugh.wav|Bip::beep.wav|Telefonoa::phone_ring.wav|Ate-txirrina::doorbell.wav|Alerta::alert.wav" } \
    } }, \
    { L"Catalan", { \
      { L"Frases de Conversa", L"Sí|No|Potser|Si us plau|Gràcies|Perdó|Disculpi|Ajuda|No ho sé|Ho entenc|No ho entenc|Ho pot repetir?|Necessito un moment|Ei|Què tal?|Què?|Digues" }, \
      { L"Salutacions Freqüents", L"Hola|Bones|Ei|Eiii|Benvingut|Encantat de veure't|Què tal va?|Quant de temps|Bon dia|Bona tarda|Bon vespre|Bona nit|Adéu|Adeu-siau|Fins ara|Fins després|Ens veiem|Fins demà|Cuida't|Un abraç" }, \
      { L"Frases Corteses", L"Si us plau|Gràcies|De res|Disculpi|Perdoni|Puc... ?|Em pot ajudar?|Li ho agraeixo|Que tingui un bon dia|Em sap greu" }, \
      { L"Frases d'Emergència", L"Ajuda|Truqui al 112|Necessito ajuda|Emergència mèdica|Estic ferit|Foc|Truqui a la policia|No puc respirar|Hi ha un accident|On és l'hospital?" }, \
      { L"Frases de Viatge", L"On és el lavabo?|Quant costa?|No ho entenc|Parla anglès?|On sóc?|Necessito un taxi|Porti'm a l'aeroport|Quina hora és?|M'ho pot mostrar al mapa?|Aquest seient està ocupat?" }, \
      { L"Frases de Negocis", L"Bon dia|Com està?|Encantat de conèixer-lo|Gràcies pel seu temps|Li ho agraeixo|Amb ganes|Si us plau, enviï els detalls|Programem una reunió|Podem reprogramar?|Faré seguiment" }, \
      { L"Frases Socials", L"Com estàs?|Estic bé, gràcies|Felicitats|Per molts anys|Bona sort|Molt bé|És interessant|Explica-m'ho|Fins aviat" }, \
      { L"Frases de Salut", L"Necessito un metge|Sóc al·lèrgic a...|Tinc febre|Sóc diabètic|Necessito medicació|On és urgències?|Em marejo|Tinc dificultat per respirar|Mal aquí" }, \
      { L"Família i Casa", L"Mare|Pare|Germà|Germana|Sóc a casa|On és el meu telèfon?|El sopar és a punt?|Me'n vaig|Ara torno|Bona nit" }, \
      { L"Hora i Data", L"Quina hora és?|Hui|Demà|Ahir|D'ací a una hora|Ara|Més tard|Este matí|Esta nit|La setmana que ve" }, \
      { L"Sons", L"Aplaudiments::applause.wav|Rialla::laugh.wav|Bip::beep.wav|Telèfon::phone_ring.wav|Timbre::doorbell.wav|Alerta::alert.wav" } \
    } }, \
    { L"Chinese (Simplified)", { \
      { L"会话短语", L"是|不是|也许|请|谢谢|对不起|不好意思|帮帮我|我不知道|我明白|我不明白|你能再说一遍吗|我需要一点时间|嘿|怎么了？|什么？|告诉我" }, \
      { L"常见问候", L"你好|您好|嗨|大家好|欢迎|见到你真好|好久不见|辛苦了|早上好|下午好|晚上好|晚安|嘿|再见|拜拜|待会见|回头见|下次见|保重|慢走" }, \
      { L"礼貌用语", L"请|谢谢|不客气|不好意思|可以吗|你能帮我吗|非常感谢|抱歉|祝你有美好的一天|我很抱歉" }, \
      { L"紧急用语", L"救命|请拨打112|我需要帮助|医疗紧急情况|我受伤了|着火了|报警|我无法呼吸|发生事故了|医院在哪里" }, \
      { L"旅行用语", L"洗手间在哪里|多少钱|我不明白|你会说英语吗|我在哪里|我需要一辆出租车|带我去机场|现在几点|你能在地图上指给我看吗|这个座位有人吗" }, \
      { L"商务用语", L"早上好|你好吗|很高兴认识你|感谢你的时间|非常感谢|期待合作|请发送详细信息|我们安排一次会议|可以改期吗|我会跟进" }, \
      { L"社交用语", L"你好吗|我很好，谢谢|恭喜|生日快乐|祝你好运|做得好|很有意思|多说一点|回头见" }, \
      { L"健康用语", L"我需要医生|我对...过敏|我发烧了|我是一名糖尿病患者|我需要药物|急诊在哪里|我感到头晕|我呼吸困难|这里疼痛" }, \
      { L"家庭与家", L"妈妈|爸爸|哥哥|姐姐|我在家|我的手机在哪里|晚饭好了吗|我走了|马上回来|晚安" }, \
      { L"时间与日期", L"现在几点|今天|明天|昨天|一小时后|现在|稍后|今天早上|今天晚上|下周" }, \
      { L"声音", L"掌声::applause.wav|笑声::laugh.wav|哔声::beep.wav|电话铃声::phone_ring.wav|门铃::doorbell.wav|警报::alert.wav" } \
    } }, \
    { L"English", { \
      { L"Conversation Phrases", L"Yes|No|Maybe|Please|Thank you!|You're welcome!|Sorry|Excuse me|Help!|I don't know|I understand|I don't understand|Can you repeat?|I need a moment|Hey|What's up?|What?|Tell me" }, \
      { L"Frequent Greetings", L"Hello|Hi there|Hey|Howdy|Yo|Welcome|Nice to meet you|Great to see you|Long time no see|How's it going?|Good morning|Good afternoon|Good evening|Good night|Bye|Goodbye|See you later|Catch you later|See you soon|Take care|Talk soon|Later!" }, \
      { L"Polite Phrases", L"Please|Thank you|You're welcome|Excuse me|May I?|Could you help me?|I appreciate it|Pardon me|Have a nice day|I apologize" }, \
      { L"Emergency Phrases", L"Help|Call 911|I need help|Medical emergency|I am hurt|Fire|Call the police|I can't breathe|There's an accident|Where is the hospital" }, \
      { L"Travel Phrases", L"Where is the bathroom|How much does it cost|I don't understand|Do you speak English?|Where am I?|I need a taxi|Take me to the airport|What time is it|Can you show me on the map|Is this seat taken?" }, \
      { L"Business Phrases", L"Good morning|How are you|Nice to meet you|Thank you for your time|I appreciate it|Looking forward to it|Please send details|Let's schedule a meeting|Can we reschedule?|I will follow up" }, \
      { L"Social Phrases", L"How are you?|What's up?|I'm fine, thanks|Congratulations|Happy birthday|Good luck|Well done|That's interesting|Tell me more|See you soon" }, \
      { L"Health Phrases", L"I need a doctor|I am allergic to...|I have a fever|I am diabetic|I need medication|Where is urgent care|I feel dizzy|I am having trouble breathing|Pain here" }, \
      { L"Family & Home", L"Mom|Dad|Brother|Sister|I am home|Where is my phone|Is dinner ready?|I'm leaving|Be right back|Good night" }, \
      { L"Time & Date", L"What time is it|Today|Tomorrow|Yesterday|In an hour|Now|Later|This morning|This evening|Next week" }, \
      { L"Sounds", L"Applause::applause.wav|Laugh::laugh.wav|Beep::beep.wav|Phone ring::phone_ring.wav|Doorbell::doorbell.wav|Alert::alert.wav" } \
    } }, \
    { L"French", { \
      { L"Phrases de Conversation", L"Oui|Non|Peut-être|S'il vous plaît|Merci|Pardon|Excusez-moi|Aidez-moi|Je ne sais pas|Je comprends|Je ne comprends pas|Pouvez-vous répéter?|J'ai besoin d'un moment|Hey|Quoi de neuf ?|Quoi ?|Dis-moi" }, \
      { L"Salutations Fréquentes", L"Bonjour|Salut|Coucou|Hey|Bienvenue|Enchanté|Comment ça va?|Ravi de vous voir|Ça fait longtemps|Bon matin|Bon après-midi|Bonsoir|Bonne soirée|Bonne nuit|Au revoir|Salut!|À bientôt|À plus|À tout à l'heure|À demain|Prends soin de toi|Bisous" }, \
      { L"Phrases Polies", L"S'il vous plaît|Merci|De rien|Excusez-moi|Pardonnez-moi|Puis-je?|Pouvez-vous m'aider?|Je vous remercie|Bonne journée|Je m'excuse" }, \
      { L"Phrases d'Urgence", L"Au secours|Appelez le 15|J'ai besoin d'aide|Urgence médicale|Je suis blessé|Incendie|Appelez la police|Je ne peux pas respirer|Il y a un accident|Où est l'hôpital?" }, \
      { L"Phrases de Voyage", L"Où sont les toilettes?|Combien ça coûte?|Je ne comprends pas|Parlez-vous anglais?|Où suis-je?|J'ai besoin d'un taxi|Emmenez-moi à l'aéroport|Quelle heure est-il?|Pouvez-vous me montrer sur la carte?|Ce siège est-il libre?" }, \
      { L"Phrases d'Affaires", L"Bonjour|Comment allez-vous?|Enchanté|Merci pour votre temps|Je l'apprécie|Au plaisir|Veuillez envoyer les détails|Planifions une réunion|Pouvons-nous reprogrammer?|Je ferai le suivi" }, \
      { L"Social", L"Ça va?|Je vais bien, merci|Félicitations|Joyeux anniversaire|Bonne chance|Bien joué|C'est intéressant|Racontez-moi|À bientôt" }, \
      { L"Santé", L"J'ai besoin d'un médecin|Je suis allergique à...|J'ai de la fièvre|Je suis diabétique|J'ai besoin de médicaments|Où est les urgences?|Je me sens étourdi|J'ai du mal à respirer|Douleur ici" }, \
      { L"Famille & Maison", L"Maman|Papa|Frère|Soeur|Je suis à la maison|Où est mon téléphone?|Le dîner est-il prêt?|Je pars|Je reviens|Bonne nuit" }, \
      { L"Temps & Date", L"Quelle heure est-il?|Aujourd'hui|Demain|Hier|Dans une heure|Maintenant|Plus tard|Ce matin|Ce soir|La semaine prochaine" }, \
      { L"Sons", L"Applaudissements::applause.wav|Rire::laugh.wav|Bip::beep.wav|Téléphone::phone_ring.wav|Sonnette::doorbell.wav|Alerte::alert.wav" } \
    } }, \
    { L"Galician", { \
      { L"Frases de Conversa", L"Si|Non|Quizais|Por favor|Grazas|Perdón|Desculpe|Axuda|Non o sei|Entendo|Non entendo|Pode repetir?|Necesito un momento|Ei|Que tal?|Que?|Cóntame" }, \
      { L"Saúdos Frecuentes", L"Ola|Boas|Ei|Benvido|Que tal?|Encantado de coñecerte|Aledome de verte|Hai canto tempo!|Bos días|Boas tardes|Boas noites|Boa noite|Adeus|Ata logo|Ata pronto|Ata mañá|Vémonos|Chao|Coidate|Bicos" }, \
      { L"Frases Corteses", L"Por favor|Grazas|De nada|Desculpe|Perdoe|Podo...?|Pode axudarme?|Agradézollo|Que teña un bo día|Síntoo" }, \
      { L"Frases de Emerxencia", L"Axuda|Chame ao 112|Necesito axuda|Emerxencia médica|Estou ferido|Lume|Chame á policía|Non podo respirar|Hai un accidente|Onde está o hospital?" }, \
      { L"Frases de Viaxe", L"Onde está o baño?|Canto custa?|Non entendo|Fala inglés?|Onde estou?|Necesito un taxi|Léveme ao aeroporto|Que hora é?|Pode mostralo no mapa?|Este asento está ocupado?" }, \
      { L"Frases de Negocios", L"Bos días|Como está?|Encantado de coñecelo|Grazas polo seu tempo|Agradézollo|Con ganas|Por favor envíe os detalles|Programemos unha reunión|Podemos reprogramar?|Farei seguimento" }, \
      { L"Frases Sociais", L"Como estás?|Estou ben, grazas|Parabéns|Feliz aniversario|Boa sorte|Moi ben|É interesante|Cóntame máis|Ata logo" }, \
      { L"Frases de Saúde", L"Necesito un médico|Son alérxico a...|Teño febre|Son diabético|Necesito medicación|Onde está urxencias?|Síntome mareado|Teño dificultade para respirar|Dói aquí" }, \
      { L"Familia e Fogar", L"Nai|Pai|Irmán|Irmá|Estou na casa|Onde está o meu teléfono?|A cea está lista?|Voume|Volvo axiña|Boas noites" }, \
      { L"Hora e Data", L"Que hora é?|Hoxe|Mañá|Onte|Nunha hora|Agora|Máis tarde|Esta mañá|Esta noite|A vindeira semana" }, \
      { L"Sons", L"Aplausos::applause.wav|Risos::laugh.wav|Bip::beep.wav|Teléfono::phone_ring.wav|Campá::doorbell.wav|Alerta::alert.wav" } \
    } }, \
    { L"German", { \
      { L"Konversationssätze", L"Ja|Nein|Vielleicht|Bitte|Danke|Entschuldigung|Verzeihung|Hilfe|Ich weiß nicht|Ich verstehe|Ich verstehe nicht|Können Sie das wiederholen?|Ich brauche einen Moment|Hey|Was geht?|Was?|Sag mir" }, \
      { L"Häufige Grüße", L"Hallo|Hi|Hey|Hallöchen|Servus|Moin|Grüß dich|Willkommen|Schön dich zu sehen|Lange nicht gesehen|Guten Morgen|Guten Abend|Schlaf gut|Auf Wiedersehen|Tschüss|Ciao|Mach's gut|Bis später|Bis bald|Bis morgen" }, \
      { L"Höfliche Sätze", L"Bitte|Danke|Gern geschehen|Entschuldigung|Verzeihen Sie mir|Darf ich?|Können Sie mir helfen?|Ich danke Ihnen|Einen schönen Tag noch|Es tut mir leid" }, \
      { L"Notfall Sätze", L"Hilfe|Rufen Sie 112 an|Ich brauche Hilfe|Medizinischer Notfall|Ich bin verletzt|Feuer|Rufen Sie die Polizei|Ich kann nicht atmen|Es gab einen Unfall|Wo ist das Krankenhaus?" }, \
      { L"Reise Sätze", L"Wo ist die Toilette?|Wie viel kostet das?|Ich verstehe nicht|Sprechen Sie Englisch?|Wo bin ich?|Ich brauche ein Taxi|Bringen Sie mich zum Flughafen|Wie spät ist es?|Können Sie es mir auf der Karte zeigen?|Ist dieser Platz frei?" }, \
      { L"Geschäftliche Sätze", L"Guten Morgen|Wie geht es Ihnen?|Freut mich|Danke für Ihre Zeit|Ich schätze es|Ich freue mich darauf|Bitte senden Sie Details|Lassen Sie uns ein Treffen planen|Können wir neu terminieren?|Ich melde mich" }, \
      { L"Soziales", L"Wie geht's?|Mir geht's gut, danke|Glückwunsch|Alles Gute zum Geburtstag|Viel Glück|Gut gemacht|Das ist interessant|Erzähl mir mehr|Bis bald" }, \
      { L"Gesundheit", L"Ich brauche einen Arzt|Ich bin allergisch gegen...|Ich habe Fieber|Ich bin Diabetiker|Ich brauche Medikamente|Wo ist die Notaufnahme?|Mir ist schwindelig|Ich habe Atembeschwerden|Schmerz hier" }, \
      { L"Familie & Zuhause", L"Mama|Papa|Bruder|Schwester|Ich bin zu Hause|Wo ist mein Telefon?|Ist das Abendessen fertig?|Ich gehe|Bin gleich zurück|Gute Nacht" }, \
      { L"Zeit & Datum", L"Wie spät ist es?|Heute|Morgen|Gestern|In einer Stunde|Jetzt|Später|Heute Morgen|Heute Abend|Nächste Woche" }, \
      { L"Sounds", L"Applaus::applause.wav|Lachen::laugh.wav|Piepton::beep.wav|Telefonklingeln::phone_ring.wav|Türklingel::doorbell.wav|Alarm::alert.wav" } \
    } }, \
    { L"Hebrew", { \
      { L"ביטויי שיחה", L"כן|לא|אולי|בבקשה|תודה|סליחה|שיילב|עזרה|אני לא יודע|אני מבין|אני לא מבין|אפשר לחזור על זה?|אני צריך רגע" }, \
      { L"ברכות נפוצות", L"שלום|היי|אהלן|ברוכים הבאים|נעים מאוד|מה קורה?|שמח לראות אותך|מזמן לא נפגשנו|בוקר טוב|ערב טוב|לילה טוב|להתראות|ביי|נדבר בקרוב|שמור על עצמך|תהיה בטוח|חיבוק|נשיקות" }, \
      { L"ביטויים מנומסים", L"בבקשה|תודה|בבקשה (אין בעד מה)|סליחה|אפשר?|אפשר לעזור לי?|אני מעריך/ה את זה|סליחה|יום נעים|אני מצטער/ת" }, \
      { L"ביטויי חירום", L"עזרה|תתקשרו ל-112|אני צריך עזרה|מקרה חירום רפואי|נפצעתי|שריפה|תתקשרו למשטרה|אני לא מצליח לנשום|תאונה הייתה|איפה בית החולים?" }, \
      { L"ביטויי נסיעות", L"איפה השירותים?|כמה זה עולה?|אני לא מבין|אתה מדבר אנגלית?|איפה אני?|אני צריך מונית|קח אותי לשדה התעופה|מה השעה?|אפשר להראות לי במפה?|המקום הזה תפוס?" }, \
      { L"ביטויים עסקיים", L"בוקר טוב|מה שלומך?|נעים מאוד|תודה על הזמן שלך|אני מעריך/ה את זה|מצפה לזה|בבקשה שלחו פרטים|בוא נקבע פגישה|אפשר לדחות?|אחזור אליך" }, \
      { L"Bיטויים חברתיים", L"מה שלומך?|אני בסדר, תודה|מזל טוב|יום הולדת שמח|בהצלחה|כל הכבוד|מעניין|ספר/י לי עוד|נתראה בקרוב" }, \
      { L"ביטויי בריאות", L"אני צריך רופא|אני אלרגי ל...|יש לי חום|אני חולה סוכרת|אני צריך תרופות|איפה מיון?|אני מרגיש סחרחורת|קשה לי לנשום|כואב כאן" }, \
      { L"משפחה ובית", L"אמא|אבא|אח|אחות|אני בבית|איפה הטלפון שלי?|הארוחה מוכנה?|אני יוצא/ת|מיד חוזר/ת|לילה טוב" }, \
      { L"זמן ותאריך", L"מה השעה?|היום|מחר|אתמול|בעוד שעה|עכשיו|אחר כך|הבוקר|הערב|שבוע הבא" }, \
      { L"צלילים", L"מחיאות כפיים::applause.wav|צחוק::laugh.wav|ביפ::beep.wav|טלפון מצלצל::phone_ring.wav|פעמון דלת::doorbell.wav|התראה::alert.wav" } \
    } }, \
    { L"Hindi", { \
      { L"बातचीत के वाक्य", L"हाँ|नहीं|शायद|कृपया|धन्यवाद|माफ़ कीजिए|क्षमा करें|मदद|मुझे नहीं पता|मैं समझ गया/गई|मैं नहीं समझा/समझी|क्या आप दोहरा सकते हैं?|मुझे एक पल चाहिए|अरे|क्या हाल है?|क्या?|मुझे बताओ" }, \
      { L"अक्सर अभिवादन", L"नमस्ते|नमस्कार|प्रणाम|हाय|अरे यार|कैसे हो?|आपसे मिलकर खुशी हुई|काफी समय हो गया|शुभ प्रभात|शुभ संध्या|शुभ रात्रि|अलविदा|बाय|चलता हूँ|जल्दी मिलते हैं|फिर मिलेंगे|कल मिलते हैं|फिर बातें करेंगे|ध्यान रखना|सी यू" }, \
      { L"विनम्र वाक्य", L"कृपया|धन्यवाद|कोई बात नहीं|माफ़ कीजिए|क्या मैं... ?|क्या आप मेरी मदद कर सकते हैं?|मैं आभारी हूँ|क्षमा करें|आपका दिन शुभ हो|मुझे खेद है" }, \
      { L"आपातकालीन वाक्य", L"मदद|112 पर कॉल करें|मुझे मदद चाहिए|चिकित्सा आपातकाल|मैं घायल हूँ|आग|पुलिस को बुलाएँ|मैं साँस नहीं ले पा रहा/रही|दुर्घटना हुई है|अस्पताल कहाँ है" }, \
      { L"यात्रा वाक्य", L"शौचालय कहाँ है|यह कितने का है|मुझे समझ नहीं आया|क्या आप अंग्रेज़ी बोलते हैं?|मैं कहाँ हूँ|मुझे टैक्सी चाहिए|मुझे हवाई अड्डे ले चलें|समय क्या हुआ है|क्या आप नक्शे पर दिखा सकते हैं|क्या यह सीट खाली है" }, \
      { L"व्यवसाय वाक्य", L"सुप्रभात|आप कैसे हैं|आपसे मिलकर खुशी हुई|आपके समय के लिए धन्यवाद|मैं आभारी हूँ|मैं उत्सुक हूँ|कृपया विवरण भेजें|आइए बैठक तय करें|क्या हम समय बदल सकते हैं?|मैं फॉलो-अप करूँगा/करूँगी" }, \
      { L"सामाजिक वाक्य", L"आप कैसे हैं?|मैं ठीक हूँ, धन्यवाद|बधाई|जन्मदिन मुबारक|शुभकामनाएँ|बहुत अच्छा|यह रोचक है|और बताइए|फिर मिलेंगे" }, \
      { L"स्वास्थ्य वाक्य", L"मुझे डॉक्टर चाहिए|मुझे ... से एलर्जी है|मुझे बुखार है|मैं मधुमेह का रोगी हूँ|मुझे दवा चाहिए|आपातकाल कहाँ है|मुझे चक्कर आ रहे हैं|मुझे साँस लेने में परेशानी है|यहाँ दर्द है" }, \
      { L"परिवार और घर", L"माँ|पिता|भाई|बहन|मैं घर पर हूँ|मेरे फ़ोन कहाँ है|क्या खाना तैयार है|मैं जा रहा/रही हूँ|अभी आता/आती हूँ|शुभ रात्रि" }, \
      { L"समय और तारीख", L"समय क्या हुआ है|आज|कल|बीता कल|एक घंटे में|अभी|बाद में|आज सुबह|आज शाम|अगले हफ्ते" }, \
      { L"ध्वनियाँ", L"तालियाँ::applause.wav|हँसी::laugh.wav|बीप::beep.wav|फ़ोन की घंटी::phone_ring.wav|डोरबेल::doorbell.wav|अलर्ट::alert.wav" } \
    } }, \
    { L"Italian", { \
      { L"Frasi di Conversazione", L"Sì|No|Forse|Per favore|Grazie|Scusa|Mi scusi|Aiuto|Non lo so|Capisco|Non capisco|Puoi ripetere?|Ho bisogno di un momento|Ehi|Che succede?|Cosa?|Dimmi" }, \
      { L"Saluti Frequenti", L"Ciao|Ehi|Salve|Benvenuto|Piacere di conoscerti|Che piacere vederti|È da tanto!|Come va?|Buongiorno|Buon pomeriggio|Buonasera|Buonanotte|Arrivederci|A presto|A dopo|Ci vediamo|A domani|Stammi bene|Un abbraccio|Alla prossima" }, \
      { L"Frasi Cortesi", L"Per favore|Grazie|Prego|Mi scusi|Scusami|Posso?|Puoi aiutarmi?|Te ne sono grato|Buona giornata|Mi scuso" }, \
      { L"Frasi di Emergenza", L"Aiuto|Chiamate il 112|Ho bisogno di aiuto|Emergenza medica|Sono ferito|Incendio|Chiamate la polizia|Non riesco a respirare|C'è un incidente|Dov'è l'ospedale?" }, \
      { L"Frasi di Viaggio", L"Dov'è il bagno?|Quanto costa?|Non capisco|Parli inglese?|Dove sono?|Ho bisogno di un taxi|Portami all'aeroporto|Che ora è?|Puoi mostrarmelo sulla mappa?|Questo posto è libero?" }, \
      { L"Frasi di Affari", L"Buongiorno|Come sta?|Piacere di conoscerla|Grazie per il suo tempo|La apprezzo|Non vedo l'ora|Per favore invii i dettagli|Programmiamo una riunione|Possiamo riprogrammare?|Farò seguito" }, \
      { L"Sociale", L"Come va?|Sto bene, grazie|Congratulazioni|Buon compleanno|In bocca al lupo|Ben fatto|Interessante|Dimmi di più|A presto" }, \
      { L"Salute", L"Ho bisogno di un medico|Sono allergico a...|Ho la febbre|Sono diabetico|Ho bisogno di medicine|Dov'è il pronto soccorso?|Mi sento stordito|Ho difficoltà a respirare|Dolore qui" }, \
      { L"Famiglia & Casa", L"Mamma|Papà|Fratello|Sorella|Sono a casa|Dov'è il mio telefono?|La cena è pronta?|Sto uscendo|Torno subito|Buonanotte" }, \
      { L"Tempo & Data", L"Che ora è?|Oggi|Domani|Ieri|Tra un'ora|Adesso|Più tardi|Questa mattina|Questa sera|La prossima settimana" }, \
      { L"Suoni", L"Applausi::applause.wav|Risata::laugh.wav|Beep::beep.wav|Suoneria::phone_ring.wav|Campanello::doorbell.wav|Allarme::alert.wav" } \
    } }, \
    { L"Japanese", { \
      { L"会話フレーズ", L"はい|いいえ|たぶん|お願いします|ありがとうございます|ごめんなさい|すみません|助けて|わかりません|わかります|わかりません|もう一度言ってください|少し時間が必要です|やあ|どうしたの？|何？|教えて" }, \
      { L"よくある挨拶", L"こんにちは|やあ|もしもし|ハロー|いらっしゃい|会えてうれしいです|久しぶり|お疲れさま|元気?|おはようございます|こんばんは|おやすみなさい|さようなら|またね|じゃあね|また後で|また明日|行ってきます|行ってらっしゃい|ではまた" }, \
      { L"丁寧なフレーズ", L"お願いします|ありがとうございます|どういたしまして|すみません|失礼します|よろしいですか|手伝ってくれますか|助かります|良い一日を|申し訳ありません" }, \
      { L"緊急時のフレーズ", L"助けて|112に電話してください|助けが必要です|医療の緊急事態|けがをしました|火事です|警察を呼んでください|息ができません|事故がありました|病院はどこですか" }, \
      { L"旅行のフレーズ", L"トイレはどこですか|いくらですか|わかりません|英語は話せますか|ここはどこですか|タクシーが必要です|空港へ連れて行ってください|今何時ですか|地図で示してもらえますか|この席は空いていますか" }, \
      { L"ビジネスのフレーズ", L"おはようございます|お元気ですか|はじめまして|お時間ありがとうございます|感謝します|楽しみにしています|詳細を送ってください|会議を設定しましょう|変更できますか|後ほど連絡します" }, \
      { L"社交のフレーズ", L"お元気ですか|元気です、ありがとう|おめでとう|誕生日おめでとう|頑張って|よくできました|興味深いですね|もっと教えて|また近いうちに" }, \
      { L"健康のフレーズ", L"医者が必要です|…にアレルギーがあります|熱があります|糖尿病です|薬が必要です|救急はどこですか|めまいがします|息苦しいです|ここが痛い" }, \
      { L"家族と家", L"お母さん|お父さん|兄弟|姉妹|家にいます|携帯はどこですか|夕食はできていますか|出かけます|すぐ戻ります|おやすみなさい" }, \
      { L"時間と日付", L"今何時ですか|今日|明日|昨日|1時間後|今|後で|今朝|今晩|来週" }, \
      { L"音", L"拍手::applause.wav|笑い::laugh.wav|ビープ::beep.wav|着信音::phone_ring.wav|ドアベル::doorbell.wav|警告::alert.wav" } \
    } }, \
    { L"Korean", { \
      { L"대화 문장", L"네|아니요|아마도|부탁합니다|감사합니다|죄송합니다|실례합니다|도와주세요|모르겠어요|이해합니다|이해하지 못했어요|다시 말씀해 주시겠어요?|잠시만요|헤이|뭐야?|뭐?|말해 줘" }, \
      { L"자주 하는 인사", L"안녕하세요|안녕|하이|헤이|어서 와|반가워요|반갑습니다|잘 지냈어?|오랜만이에요|좋은 아침|좋은 저녁|잘 자|즐거운 하루 되세요|수고해요|조심히 가|안녕히 가세요|잘 가|또 봐요|나중에 봐|금방 다시 봐" }, \
      { L"공손한 표현", L"부탁합니다|감사합니다|천만에요|실례합니다|괜찮을까요?|도와주실 수 있나요?|정말 감사합니다|죄송하지만|좋은 하루 되세요|사과드립니다" }, \
      { L"긴급 상황", L"도와주세요|112에 전화해 주세요|도움이 필요해요|의료 응급상황|다쳤어요|불이 났어요|경찰을 불러 주세요|숨을 쉴 수 없어요|사고가 났어요|병원이 어디예요" }, \
      { L"여행", L"화장실이 어디예요|얼마예요|이해가 안 돼요|영어 하세요?|여기가 어디예요?|택시가 필요해요|공항으로 데려다 주세요|지금 몇 시예요|지도에서 보여 주실 수 있나요?|이 자리 비었나요" }, \
      { L"비즈니스", L"좋은 아침입니다|어떻게 지내세요?|만나서 반갑습니다|시간 내주셔서 감사합니다|감사합니다|기대하고 있습니다|자세한 내용을 보내 주세요|회의 일정을 잡읍시다|일정을 변경할 수 있을까요?|추후에 다시 연락드리겠습니다" }, \
      { L"사회", L"어떻게 지내세요?|무슨 일 있어요?|잘 지내요, 감사합니다|축하합니다|생일 축하합니다|행운을 빌어요|잘했어요|흥미롭네요|더 이야기해 주세요|곧 봐요" }, \
      { L"건강", L"의사가 필요해요|...에 알레르기가 있어요|열이 있어요|당뇨가 있어요|약이 필요해요|응급실이 어디예요|어지러워요|숨쉬기 힘들어요|아픔이 있어요" }, \
      { L"가족과 집", L"엄마|아빠|형|동생|집에 있어요|내 핸드폰 어디에요?|저녁 준비됐어요?|나 갈게요|금방 올게요|안녕히 주무세요" }, \
      { L"시간과 날짜", L"지금 몇 시에요?|오늘|내일|어제|한 시간 후|지금|나중에|오늘 아침|오늘 저녁|다음 주" }, \
      { L"소리", L"박수::applause.wav|웃음::laugh.wav|삐 소리::beep.wav|전화벨::phone_ring.wav|초인종::doorbell.wav|경고::alert.wav" } \
    } }, \
    { L"Portuguese", { \
      { L"Frases de Conversa", L"Sim|Não|Talvez|Por favor|Obrigado|Desculpe|Com licença|Ajuda|Não sei|Entendo|Não entendo|Pode repetir?|Preciso de um momento|Ei|E aí?|O quê?|Me diga" }, \
      { L"Saudações Frequentes", L"Olá|Oi|E aí|Oi gente|Bem-vindo|Prazer em vê-lo|Quanto tempo!|Como vai?|Bom dia|Boa tarde|Boa noite|Tchau|Até logo|Até mais|Até amanhã|Nos vemos|Falou|Abraço|Se cuida|Beijos" }, \
      { L"Frases Corteses", L"Por favor|Obrigado|De nada|Com licença|Posso?|Pode ajudar-me?|Agradeço|Perdoe-me|Tenha um bom dia|Peço desculpa" }, \
      { L"Frases de Emergência", L"Ajuda|Ligue para 112|Preciso de ajuda|Emergência médica|Estou ferido|Fogo|Chame a polícia|Não consigo respirar|Houve um acidente|Onde é o hospital?" }, \
      { L"Frases de Viagem", L"Onde fica a casa de banho?|Quanto custa?|Não entendo|Fala inglês?|Onde estou?|Preciso de um táxi|Leve-me ao aeroporto|Que horas são?|Pode mostrar-me no mapa?|Este lugar está ocupado?" }, \
      { L"Frases de Negócios", L"Bom dia|Como está?|Prazer em conhecê-lo|Obrigado pelo seu tempo|Agradeço|Aguardando com expectativa|Por favor envie os detalhes|Vamos agendar uma reunião|Podemos reagendar?|Vou acompanhar" }, \
      { L"Social", L"Como vai?|Estou bem, obrigado|Parabéns|Feliz aniversário|Boa sorte|Bem feito|Isso é interessante|Conte-me mais|Até breve" }, \
      { L"Saúde", L"Preciso de um médico|Sou alérgico a...|Tenho febre|Sou diabético|Preciso de medicação|Onde fica o pronto-socorro?|Sinto tonturas|Tenho dificuldade em respirar|Dor aqui" }, \
      { L"Família & Casa", L"Mãe|Pai|Irmão|Irmã|Estou em casa|Onde está o meu telemóvel?|O jantar está pronto?|Estou a sair|Volto já|Boa noite" }, \
      { L"Tempo & Data", L"Que horas são?|Hoje|Amanhã|Ontem|Daqui a uma hora|Agora|Mais tarde|Esta manhã|Esta noite|Próxima semana" }, \
      { L"Sons", L"Aplausos::applause.wav|Riso::laugh.wav|Beep::beep.wav|Toque do telefone::phone_ring.wav|Campainha::doorbell.wav|Alerta::alert.wav" } \
    } }, \
    { L"Russian", { \
      { L"Разговорные фразы", L"Да|Нет|Может быть|Пожалуйста|Спасибо|Извините|Прошу прощения|Помогите|Я не знаю|Я понимаю|Я не понимаю|Можете повторить?|Мне нужна минутка|Эй|Что нового?|Что?|Скажи мне" }, \
      { L"Частые приветствия", L"Привет|Здравствуйте|Хай|Приветики|Добро пожаловать|Как дела?|Рад тебя видеть|Давно не виделись|Доброе утро|Добрый вечер|Спокойной ночи|Пока|До свидания|До скорого|Увидимся|До завтра|Чао|Береги себя|Всего хорошего" }, \
      { L"Вежливые фразы", L"Пожалуйста|Спасибо|Не за что|Извините|Прошу прощения|Можно?|Вы можете мне помочь?|Я ценю это|Хорошего дня|Мне очень жаль" }, \
      { L"Экстренные фразы", L"Помогите|Позвоните 112|Мне нужна помощь|Медицинская помощь|Я ранен(а)|Пожар|Позвоните в полицию|Я не могу дышать|Произошла авария|Где больница?" }, \
      { L"Фразы для путешествий", L"Где туалет?|Сколько это стоит?|Я не понимаю|Вы говорите по-английски?|Где я?|Мне нужно такси|Отвезите меня в аэропорт|Который час?|Можете показать на карте?|Это место занято?" }, \
      { L"Деловые фразы", L"Доброе утро|Как дела?|Приятно познакомиться|Спасибо за ваше время|Я ценю это|С нетерпением жду|Пожалуйста, пришлите детали|Давайте назначим встречу|Можем перенести?|Я свяжусь позже" }, \
      { L"Социальные фразы", L"Как дела?|Я в порядке, спасибо|Поздравляю|С днём рождения|Удачи|Отлично|Это интересно|Расскажите подробнее|До скорого" }, \
      { L"Фразы о здоровье", L"Мне нужен врач|У меня аллергия на...|У меня температура|Я диабетик|Мне нужны лекарства|Где неотложка?|У меня кружится голова|Я испытываю трудности с дыханием|Боль здесь" }, \
      { L"Семья и дом", L"Мама|Папа|Брат|Сестра|Я дома|Где мой телефон?|Ужин готов?|Я ухожу|Скоро вернусь|Спокойной ночи" }, \
      { L"Время и дата", L"Который час?|Сегодня|Завтра|Вчера|Через час|Сейчас|Позже|Сегодня утром|Сегодня вечером|На следующей неделе" }, \
      { L"Звуки", L"Аплодисменты::applause.wav|Смех::laugh.wav|Сигнал::beep.wav|Звонок телефона::phone_ring.wav|Дверной звонок::doorbell.wav|Предупреждение::alert.wav" } \
    } }, \
    { L"Spanish", { \
      { L"Frases de Conversación", L"Sí|No|Tal vez|¡Muy bien!|Por favor|¡Muchas gracias!|De nada|Perdón|Disculpa|¡Espera!|Ayuda|No sé|Entiendo|No entiendo|¿Puedes repetir?|Necesito un momento|¡Hey!|¿Qué pasa?|¿Qué?|Dime" }, \
      { L"Saludos Frecuentes", L"¡Hola!|¿Qué tal?|¿Qué hay?|¿Cómo vas?|¡Buenas!|¡Buenos días!|¡Buenas tardes!|¡Buenas noches!|Encantado de verte|Cuánto tiempo|¡Hey!|¡Adiós!|¡Chao!|¡Hasta luego!|¡Hasta pronto!|Nos vemos|¡Hasta mañana!|¡Cuídate!|¡Que te vaya bien!|Seguimos en contacto" }, \
      { L"Frases Corteses", L"Por favor|Gracias|De nada|Disculpe|Perdóneme|¿Puedo?|¿Puede ayudarme?|Se lo agradezco|Que tenga un buen día|Lo siento" }, \
      { L"Frases de Emergencia", L"Ayuda|Llame al 112|Necesito ayuda|Emergencia médica|Estoy herido|Incendio|Llame a la policía|No puedo respirar|Hay un accidente|¿Dónde está el hospital?" }, \
      { L"Frases de Viaje", L"¿Dónde está el baño?|¿Cuánto cuesta?|No entiendo|¿Habla inglés?|¿Dónde estoy?|Necesito un taxi|Lléveme al aeropuerto|¿Qué hora es?|¿Me lo puede mostrar en el mapa?|¿Está ocupado este asiento?" }, \
      { L"Frases de Negocios", L"Buenos días|¿Cómo está?|Mucho gusto|Gracias por su tiempo|Lo aprecio|Espero con interés|Por favor envíe los detalles|Agendemos una reunión|¿Podemos reprogramar?|Le daré seguimiento" }, \
      { L"Social", L"¿Qué tal?|Estoy bien, gracias|Felicidades|Feliz cumpleaños|Buena suerte|Bien hecho|Qué interesante|Cuéntame más|Nos vemos pronto" }, \
      { L"Salud", L"Necesito un doctor|Soy alérgico a...|Tengo fiebre|Soy diabético|Necesito medicación|¿Dónde está urgencias?|Me siento mareado|Tengo dificultad para respirar|Dolor aquí" }, \
      { L"Familia y Hogar", L"Mamá|Papá|Hermano|Hermana|Estoy en casa|¿Dónde está mi teléfono?|¿La cena está lista?|Me voy|Vuelvo enseguida|Buenas noches" }, \
      { L"Tiempo y Fecha", L"¿Qué hora es?|Hoy|Mañana|Ayer|En una hora|Ahora|Más tarde|Esta mañana|Esta noche|La próxima semana" }, \
      { L"Sonidos", L"Aplausos::applause.wav|Risa::laugh.wav|Tono::beep.wav|Teléfono::phone_ring.wav|Timbre::doorbell.wav|Alarma::alert.wav" } \
    } }, \
    { L"Valencian", { \
      { L"Frases de Conversa", L"Sí|No|Potser|Per favor|Gràcies|Perdó|Disculpe|Ajuda|No ho sé|Ho entenc|No ho entenc|Ho pot repetir?|Necessite un moment|Ei|Com va?|Què?|Digues" }, \
      { L"Salutacions Freqüents", L"Hola|Bones|Ei|Eiii|Benvingut|Encantat de veure't|Com va?|Quant de temps|Bon dia|Bona vesprada|Bona nit|Adéu|Adeu-siau|Fins ara|Fins després|Ens veiem|Fins demà|Cuida't|Un abraç" }, \
      { L"Frases Corteses", L"Per favor|Gràcies|De res|Disculpe|Perdone|Puc...?|Em pot ajudar?|Li ho agraïsc|Que tinga un bon dia|Em sap greu" }, \
      { L"Frases d'Emergència", L"Ajuda|Truqueu al 112|Necessite ajuda|Emergència mèdica|Estic ferit|Foc|Truqueu a la policia|No puc respirar|Hi ha un accident|On és l'hospital?" }, \
      { L"Frases de Viatge", L"On és el lavabo?|Quant costa?|No ho entenc|Parla anglés?|On sóc?|Necessite un taxi|Porta'm a l'aeroport|Quina hora és?|M'ho pot mostrar al mapa?|Este seient està ocupat?" }, \
      { L"Frases de Negocis", L"Bon dia|Com està?|Encantat de conéixer-lo|Gràcies pel seu temps|Li ho agraïsc|Amb ganes|Per favor envieu els detalls|Programem una reunió|Podem reprogramar?|Faré seguiment" }, \
      { L"Frases Socials", L"Com estàs?|Estic bé, gràcies|Enhorabona|Feliç aniversari|Bona sorte|Molt bé|És interessant|Conta'm més|Fins prompte" }, \
      { L"Frases de Salut", L"Necessite un metge|Sóc al·lèrgic a...|Tinc febre|Sóc diabètic|Necessite medicació|On és urgències?|Em marege|Tinc dificultat per respirar|Em fa mal ací" }, \
      { L"Família i Casa", L"Mare|Pare|Germà|Germana|Sóc a casa|On és el meu telèfon?|El sopar està a punt?|Me'n vaig|Ara torne|Bona nit" }, \
      { L"Hora i Data", L"Quina hora és?|Hui|Demà|Ahir|D'ací a una hora|Ara|Més tard|Este matí|Esta nit|La setmana que ve" }, \
      { L"Sons", L"Aplaudiments::applause.wav|Rialla::laugh.wav|Bip::beep.wav|Telèfon::phone_ring.wav|Timbre::doorbell.wav|Alerta::alert.wav" } \
    } } \
  }

#endif // DEFAULT_PHRASES_H