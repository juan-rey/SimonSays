/*
   default_phrases.h: default_phrases header file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef default_phrases_h
#define default_phrases_h

/*
  Default phrases & categories — format and separators

  - Data structure (macro):
    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>>
    - Top level: pair<LanguageName, CategoriesVector>
      - LanguageName: the language name (English (Default), Arabic, etc.)
    - CategoriesVector: vector of pair<CategoryLabel, PhrasesString>
      - CategoryLabel: the button label for the category (may include an icon prefix)
      - PhrasesString: one wstring containing multiple phrase tokens separated by '|'

  - Category icon prefix:
    Format: <icon>##<category name>
    - <icon> may be an emoji or a file path to a .ico
    - '##' separates icon from visible category name
    - If no '##' is present the whole CategoryLabel is the name (no icon)

  - Phrase token format (each token inside PhrasesString, tokens separated by '|'):
    <icon>##<phrase text>::<audio file>
    - Tokens are split by the pipe character '|' into individual phrase entries
    - Optional parts:
      - Icon prefix: <icon>## (emoji or .ico path) placed before the text
      - Audio suffix: ::<audio filename or path> appended after the text
    - Examples:
      - L"🔔##Attention::notification.wav"  (emoji, text, audio)
      - L"Attention::notification.wav"      (text + audio)
      - L"👍##Sounds good!"                 (emoji + text)
      - L"Hello|Goodbye|Help"               (multiple plain phrases)

  - Suggested parsing order:
    1. Split the PhrasesString by '|' to get tokens.
    2. For each token, if '::' exists, split to extract the audio suffix.
    3. On the remaining left part, if '##' exists, split to extract the icon prefix and the phrase text.
    4. Remaining text is the spoken/displayed phrase.

  - Notes:
    - Icon file support requires .ico files for file-based icons.
    - Audio suffixes are filenames or paths (supported formats handled elsewhere).

  - Ordering conventions — the order written here is the order the user sees:
    Categories and phrases are seeded into the registry in the order listed
    below, and the category window renders them in registry enumeration order,
    so this file decides the on-screen layout of a fresh install (see
    docs/specs/persistence.spec.md, REG-F32). Position is access cost: with
    dwell / gaze activation a button further along the grid costs real time and
    effort to reach, so ordering is a usability decision, not cosmetics. Every
    category below is therefore split into named blocks ordered by
    **frequency x criticality** — what the user says most often, and what they
    cannot afford to reach late, comes first. Keep the order stable once a
    version ships — users build motor memory for button positions, and moving a
    phrase is more disruptive than it looks.
    Edits here reach new installs only: defaults are seeded when a language's
    registry key is missing and never overwrite existing phrases (REG-F20).

    Category order, the same in every language:
      Frequent Greetings, Conversation Phrases  - social core, every interaction
      Emergency, Care & Comfort, Health         - needs and safety
      Family & Home                             - the people around the user
      Social                                    - social extensions
      Time & Date, Business, Travel             - situational
      Sounds                                    - expressive; a fixed endpoint
    Emergency sits right after the core so it is reachable in the fewest
    selections. Care & Comfort follows it: those phrases are the
    highest-frequency content on the whole board for a user dependent on a
    caregiver, and they sit next to Health so a user in distress finds them
    together. There is no separate polite category: politeness lives in
    Conversation.

    Within "Frequent Greetings", four blocks:
      1. General first-contact greetings (hello / hi / welcome / come in), led
         by the partner-instruction phrase ("I can't speak, I use this app…").
         That phrase is said once per encounter with someone new, but it must
         be reachable the instant a stranger speaks first, so it takes the
         second-cheapest button on the whole board. It is the only default
         phrase carrying an icon (🔇) — its text is too long to read at a
         glance on a default-sized button, so the icon identifies it.
      2. Time of day, in order: morning -> afternoon -> evening -> night. The
         night form stays here even in languages where it doubles as a
         farewell, so the progression is not broken.
      3. Second-turn greetings — those normally said *after* an opening one:
         pleased-to-meet-you / good-to-see-you / long-time-no-see first, then
         how-are-you questions.
      4. Closings: direct farewells, then see-you-agains, then thanks-for-
         coming and well-wishes.

    Within "Conversation Phrases", by AAC priority:
      1. Core responses (yes / no / OK / maybe) — the highest-frequency items
         in any AAC vocabulary, so they cost the least to reach. OK is an
         acknowledgement ("understood, will do"), distinct from yes
         (agreement) and from "I understand" (comprehension) in block 3.
      2. Urgent need (help).
      3. Pace, repair and comprehension (wait / let me finish / I need a
         moment / slower please / can you repeat? / what? / I don't understand
         / I understand / I don't know). These hold the conversational floor
         against a partner who outpaces the user — the characteristic failure
         of AAC conversation — so they rank above politeness despite being
         less frequent.
      4. Reactions (that's great / that's a shame) — a complete conversational
         turn at one press, without composing a sentence.
      5. Politeness (please / thank you / you're welcome / sorry / excuse me).
         Where a language separates an apology from a sympathetic "I'm sorry"
         (Spanish Perdón vs Lo siento), both are kept.
      6. Prompting the partner (tell me).

    Within "Emergency Phrases", six blocks:
      1. Summon — the shortest, loudest calls (help / I need help).
      2. Airway and breathing, before anything else: for an ALS user this is
         both the most likely emergency and the least survivable delay.
      3. Directing a bystander to call (emergency number / ambulance /
         caregiver / family / police). The per-language emergency number is
         the local one (911 in English, 112 in Europe, 192 in Brazil, 15 in
         France).
      4. Injury and what happened (I fell / I am hurt / I'm bleeding).
      5. Scene hazards (fire / accident).
      6. Orientation (where is the hospital?).

    Within "Care & Comfort", six blocks — the caregiver-directed requests a
    user repeats many times a day, ordered by how often they are needed:
      1. Body needs (thirst / water / hunger / bathroom).
      2. Comfort and position (uncomfortable / sit me up / lay me down /
         turn me over / pillow / move my arm).
      3. Face and small assists (scratch my nose / wipe my mouth / glasses).
      4. Temperature (cold / hot / blanket).
      5. Rest (I'm tired / let me rest).
      6. Environment (light / too loud).
    The phrases address a family member or carer, so languages with a
    familiar/formal distinction use the familiar form here (see the note on
    tú/usted below).

    Within "Health Phrases", six blocks:
      1. Requests that start medical care (doctor / medication / painkiller).
      2. The pain report — the highest-frequency clinical complaint.
      3. Other symptoms, ordered by urgency: breathing, swallowing,
         dizziness, nausea, fever.
      4. Standing facts a clinician must be told (diagnosis / allergy /
         diabetes).
      5. Partner instruction ("talk to me, not to my carer") — clinical staff
         addressing the companion instead of the patient is a documented AAC
         problem.
      6. Resolution and orientation (I feel better / where is urgent care?).

    Within "Family & Home", seven blocks:
      1. People, as vocatives — the fastest way to get someone's attention.
         The spouse follows mother/father because for an adult user the
         spouse is usually the primary caregiver and the most-addressed
         person.
      2. Bonds (I love you / thank you for helping me).
      3. Presence requests (come here / can you help me? / stay with me /
         I'd like some time alone).
      4. Coming and going (I am home / I'm leaving / be right back).
      5. Household questions (is dinner ready? / where is my phone?).
      6. Home environment (TV / music).
      7. Closing (good night).

    Within "Social Phrases", six blocks:
      1. Answering "how are you?" — including the honest negative answer,
         which a board offering only the positive one cannot express.
      2. Stance (I agree / I don't agree).
      3. Reactions that keep a conversation alive (really? / that's funny /
         that's interesting / tell me more).
      4. Bonds (I missed you / I'm happy for you).
      5. Opting out (I'd rather not talk right now).
      6. Occasions (congratulations / well done / good luck / happy birthday
         / cheers) — everyday before occasional, as before.

    Within "Time & Date", four blocks:
      1. Questions (what time is it? / what day is it?).
      2. Relative answers (now / in a minute / soon / in an hour / later /
         not yet), chronological and ending on the negative. These answer
         "when?" and are far more frequent than naming a date.
      3. Parts of today (this morning / afternoon / evening / tonight).
      4. Calendar (today / tomorrow / yesterday / this weekend / next week).
    Languages whose evening and night forms coincide have one item fewer in
    block 3 — the same collapse noted for Frequent Greetings.

    Within "Business Phrases", four blocks — reframed around remote meetings,
    where an AAC user is most often outpaced:
      1. Is the audio working at all (can you hear me? / you're on mute):
         nothing else in the category matters until this is resolved.
      2. Claiming the floor (give me a moment to type / one at a time /
         repeat the question / I have a question).
      3. Moving the meeting (let's move on / let's end here).
      4. Courtesy, follow-through and scheduling.

    Within "Travel Phrases", six blocks:
      1. Opening with a stranger (do you speak English? / I don't
         understand) — it gates every exchange abroad.
      2. Bathroom, the most frequent single travel need.
      3. Access (wheelchair / ramp / accessible entrance / elevator): it
         decides whether the user can be in a place at all.
      4. Assistance and device power (luggage / where can I charge?) — a flat
         AAC device abroad is a travel emergency.
      5. Getting around (taxi / airport / where am I? / show me on the map).
      6. Transactions (how much? / is this seat taken?).

    Within "Sounds", three blocks:
      1. Attention (alert / beep / scream) — for a user with no voice a sound
         is how you summon someone from another room; the most functional use
         of the category.
      2. Conversational reactions (applause / laugh / aww / boo / drumroll /
         fail / evil laugh).
      3. Effects and pranks (doorbell / phone ring / shots / censorship).
    This order is functional and identical in every language, replacing the
    per-language alphabetical sort used before 0.7 — which was the one place
    the boards were not parallel across languages.

    Keep the sets parallel across languages — same blocks in the same order —
    so a board behaves the same whatever the UI language. Counts may differ by
    one or two where a language genuinely merges or splits a concept (the
    evening/night collapse, the Spanish Perdón / Lo siento split); the block
    structure never differs.

    Familiar vs formal address: in languages that distinguish them, the
    addressee decides the form. Family & Home, Care & Comfort, Social,
    Conversation and Frequent Greetings address family and carers and use the
    familiar form; Emergency, Health, Business and Travel address strangers
    and professionals and use the formal one. The partner-instruction phrase
    in Frequent Greetings is the documented exception — it is said to a
    stranger, so it is formal inside an otherwise familiar category.

    Gendered forms follow each language's own existing convention in this
    file: Spanish, Catalan, Valencian, Galician, Portuguese and Italian use
    the masculine default already used by "Estoy herido" / "Sou diabético";
    Hebrew and Hindi keep the forms they already use.

    Duplicates: no phrase may appear twice across Frequent Greetings,
    Conversation, Care & Comfort, Social and Business — under dwell every
    duplicate is a button paid for twice. Emergency, Health, Family & Home,
    Time & Date and Travel are the exception: they are opened *for* a
    situation and stay self-contained, so they may repeat a general phrase
    (Emergency's "Help", Travel's "I don't understand") rather than send the
    user to another category mid-situation.

    Size: the window reserves room for the largest category and never
    scrolls, so growing any category past the current largest one enlarges the
    window for every category (CategoryWindow.cpp; categories-phrases.spec.md
    CAT-F43). The largest category is 22 phrases (Frequent Greetings /
    Conversation Phrases); keep every category at or below that.
*/


#define DEFAULT_FREQUENT_PHRASES_CATEGORIES_ALL_LANGUAGES \
  std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>>{ \
    { L"Arabic", { \
      { L"👋🏻##تحيات متكررة", L"مرحبا|🔇##لا أستطيع الكلام، أستخدم تطبيقا للتواصل. من فضلك اصبر علي.|أهلا|هلا|أهلا وسهلا|تفضل بالدخول|صباح الخير|مساء الخير|ليلة سعيدة|تشرفت بلقائك|سعيد برؤيتك|لم أرك منذ زمن|كيف حالك؟|مع السلامة|وداعاً|إلى اللقاء|أراك لاحقاً|أراك قريباً|شكرا لحضورك|طابت أوقاتك|نم جيدا" }, \
      { L"🗣️##عبارات محادثة", L"نعم|لا|حسنا|ربما|مساعدة|انتظر|دعني أكمل|أحتاج لحظة|أبطأ من فضلك|هل يمكنك التكرار؟|ماذا؟|لا أفهم|أنا أفهم|لا أعرف|هذا رائع|يا للأسف|من فضلك|شكرا|عفوا|آسف|عذرا|قل لي" }, \
      { L"🚨##عبارات الطوارئ", L"مساعدة|أحتاج مساعدة|لا أستطيع التنفس|أشعر بالاختناق|تحقق من جهاز التنفس|اتصل بـ 112|اتصل بالإسعاف|اتصل بمقدم الرعاية|اتصل بعائلتي|اتصل بالشرطة|لقد سقطت|أنا مصاب|أنا أنزف|حالة طوارئ طبية|حريق|هناك حادث|أين المستشفى؟" }, \
      { L"🤲##الرعاية والراحة", L"أنا عطشان|ماء من فضلك|أنا جائع|أحتاج إلى الحمام|أنا غير مرتاح|أجلسني|أرقدني|أقلبني|عدل الوسادة|حرك ذراعي|احكك أنفي|امسح فمي|نظارتي من فضلك|أشعر بالبرد|أشعر بالحر|بطانية من فضلك|أنا متعب|دعني أرتاح|أضئ النور|الصوت عال جدا" }, \
      { L"🏥##عبارات صحية", L"أحتاج طبيبا|أحتاج دواء|أحتاج مسكن الألم|أنا أتألم|الألم يزداد|ألم هنا|أواجه صعوبة في التنفس|لا أستطيع البلع|أشعر بالدوار|أشعر بالغثيان|لدي حمى|لدي مرض التصلب الجانبي الضموري|لدي حساسية من...|أنا مريض بالسكري|تحدث معي، لا مع مقدم الرعاية|أشعر بتحسن|أين الرعاية العاجلة؟" }, \
      { L"🏠##العائلة والمنزل", L"أمي|أبي|زوجتي|زوجي|أخي|أختي|أحبك|شكرا لمساعدتي|تعال من فضلك|هل يمكنك مساعدتي؟|ابق معي|أريد أن أبقى وحدي قليلا|أنا في المنزل|أنا مغادر|سأعود حالا|هل العشاء جاهز؟|أين هاتفي؟|شغل التلفاز|شغل الموسيقى|تصبح على خير" }, \
      { L"👥##عبارات اجتماعية", L"أنا بخير، شكرا|لست بحال جيدة اليوم|وأنت؟|أنا أوافق|لا أوافق|حقا؟|هذا مضحك|هذا مثير للاهتمام|أخبرني المزيد|افتقدتك|أنا سعيد لك|أفضل عدم الحديث الآن|مبروك|أحسنت|حظا سعيدا|عيد ميلاد سعيد|في صحتك" }, \
      { L"⏰##الوقت والتاريخ", L"كم الساعة؟|ما هو اليوم؟|الآن|بعد دقيقة|قريبا|خلال ساعة|لاحقا|ليس بعد|هذا الصباح|بعد الظهر|هذا المساء|الليلة|اليوم|غدا|أمس|نهاية الأسبوع|الأسبوع القادم" }, \
      { L"💼##عبارات الأعمال", L"هل تسمعني؟|الميكروفون مكتوم|أعطني لحظة للكتابة|تحدثوا واحدا تلو الآخر|هل يمكنك تكرار السؤال؟|لدي سؤال|لننتقل|لنتوقف هنا|شكرا لوقتك|أتطلع لذلك|يرجى إرسال التفاصيل|يرجى كتابته|لنحدد موعدا للاجتماع|هل يمكننا إعادة الجدولة؟|سأتابع الأمر" }, \
      { L"✈️##عبارات السفر", L"هل تتحدث الإنجليزية؟|لا أفهم|أين الحمام؟|هل هناك حمام مجهز؟|أستخدم كرسيا متحركا|هل هناك مدخل للكرسي المتحرك؟|أين المدخل المجهز؟|هل هناك مصعد؟|أحتاج مساعدة مع الأمتعة|أين أشحن جهازي؟|أحتاج سيارة أجرة|خذني إلى المطار|أين أنا؟|هل يمكنك أن تريني على الخريطة؟|كم يكلف هذا؟|هل هذا المقعد مأخوذ؟" }, \
      { L"🔊##أصوات", L"تنبيه::alert.wav|بيب::beep.wav|صرخة::scream.wav|تصفيق::applause.wav|ضحك::laugh.wav|آآه::aww.wav|استهجان::boo.wav|قرع الطبول::drumroll.wav|فشل::fail.wav|ضحكة شريرة::evil_laugh.wav|جرس الباب::doorbell.wav|رنين الهاتف::phone_ring.wav|ألعاب نارية::shots.wav|رقابة::censor.wav" } \
    } }, \
    { L"Basque", { \
      { L"👋🏻##Agur Ohikoak", L"Kaixo|🔇##Ezin dut hitz egin, aplikazio bat erabiltzen dut komunikatzeko. Izan pazientzia.|Aupa|Kaixo lagunok|Ongi etorri|Sartu, mesedez|Egun on|Arratsalde on|Gabon|Atsegin dut ezagutzea|Pozten naiz zu ikusteaz|Aspaldiko!|Zer moduz?|Agur|Gero arte|Laster arte|Bihar arte|Eskerrik asko etortzeagatik|Ondo izan|Ondo lo egin" }, \
      { L"🗣️##Elkarrizketa Esaldiak", L"Bai|Ez|Ondo|Agian|Laguntza|Itxaron|Utzi bukatzen|Une bat behar dut|Motelago, mesedez|Errepika dezakezu?|Zer?|Ez dut ulertzen|Ulertzen dut|Ez dakit|Zoragarria|Zer pena|Mesedez|Eskerrik asko|Ez horregatik|Barkatu|Sentitzen dut|Esaidazu" }, \
      { L"🚨##Larrialdiko Esaldiak", L"Laguntza|Laguntza behar dut|Ezin dut arnasa hartu|Itotzen ari naiz|Egiaztatu nire arnasgailua|Deitu 112ra|Deitu anbulantzia bat|Deitu nire zaintzaileari|Deitu nire familiari|Deitu poliziari|Erori naiz|Zaurituta nago|Odola darit|Larrialdi medikoa|Sua|Istripua dago|Non dago ospitalea?" }, \
      { L"🤲##Zaintza eta Erosotasuna", L"Egarri naiz|Ura, mesedez|Gose naiz|Komunera joan behar dut|Deseroso nago|Altxa nazazu|Etzan nazazu|Biratu nazazu|Egokitu burkoa|Mugitu nire besoa|Hazka egin sudurrean|Garbitu nire ahoa|Nire betaurrekoak, mesedez|Hotz naiz|Bero naiz|Manta bat, mesedez|Nekatuta nago|Utzi atseden hartzen|Piztu argia|Oso ozen dago" }, \
      { L"🏥##Osasun Esaldiak", L"Medikua behar dut|Sendagaia behar dut|Nire analgesikoa behar dut|Min dut|Mina okerrera doa|Min hemen|Arnasa hartzeko arazoak ditut|Ezin dut irentsi|Zorabioa dut|Goragalea dut|Sukarra dut|ELA dut|Alergia dut...|Diabetikoa naiz|Hitz egin niri, ez nire zaintzaileari|Hobeto nago|Non dago larrialdi-zerbitzua?" }, \
      { L"🏠##Familia eta Etxea", L"Ama|Aita|Nire emaztea|Nire senarra|Anaia|Arreba|Maite zaitut|Eskerrik asko laguntzeagatik|Etorri, mesedez|Lagundu dezakezu?|Gelditu nirekin|Bakarrik egon nahi dut pixka bat|Etxean nago|Banoa|Berehala itzuliko naiz|Afaria prest al dago?|Non dago nire telefonoa?|Piztu telebista|Jarri musika|Gabon" }, \
      { L"👥##Esaldi Sozialak", L"Ondo nago, eskerrik asko|Gaur ez nago oso ondo|Eta zu?|Ados nago|Ez nago ados|Benetan?|Zein dibertigarria|Interesgarria da|Kontatu gehiago|Faltan sumatu zaitut|Pozten naiz zugatik|Orain nahiago dut ez hitz egin|Zorionak|Oso ondo|Zorte on|Zorionak zure urtebetetzean|Topa" }, \
      { L"⏰##Ordua eta Data", L"Zer ordu da?|Zer egun da gaur?|Orain|Minutu batean|Laster|Ordubete barru|Gero|Oraindik ez|Gaur goizean|Gaur arratsaldean|Gaur gauean|Gaur|Bihar|Atzo|Asteburuan|Hurrengo astean" }, \
      { L"💼##Negozio Esaldiak", L"Entzuten nauzu?|Mikrofonoa isilduta duzu|Emadazu une bat idazteko|Hitz egin banaka|Galdera errepika dezakezu?|Galdera bat dut|Jarraitu dezagun|Hemen amaitu dezagun|Eskerrik asko zure denboragatik|Irrikitan nago|Mesedez bidali xehetasunak|Jarri idatziz, mesedez|Egin dezagun bilera bat|Aldatu dezakegu data?|Jarraipena egingo dut" }, \
      { L"✈️##Bidaia Esaldiak", L"Ingelesez hitz egiten duzu?|Ez dut ulertzen|Non dago komuna?|Komun irisgarria dago?|Gurpil-aulkia erabiltzen dut|Gurpil-aulkirako sarbidea dago?|Non dago sarrera irisgarria?|Igogailua dago?|Laguntza behar dut maletekin|Non kargatu dezaket nire gailua?|Taxi bat behar dut|Eraman nazazu aireportura|Non nago?|Mapan erakutsi ahal didazu?|Zenbat balio du?|Eserleku hau hartuta dago?" }, \
      { L"🔊##Soinuak", L"Alerta::alert.wav|Bip::beep.wav|Garrasia::scream.wav|Txaloak::applause.wav|Barreak::laugh.wav|Ooo::aww.wav|Txistuak::boo.wav|Danborrada::drumroll.wav|Hutsegitea::fail.wav|Irri zitala::evil_laugh.wav|Ate-txirrina::doorbell.wav|Telefonoa::phone_ring.wav|Tiroketak::shots.wav|Zentsura::censor.wav" } \
    } }, \
    { L"Catalan", { \
      { L"👋🏻##Salutacions Freqüents", L"Hola|🔇##No puc parlar, faig servir una aplicació per comunicar-me. Tingui paciència.|Bones|Ei|Benvingut|Passa, si us plau|Bon dia|Bona tarda|Bon vespre|Bona nit|Encantat de conèixer-te|Encantat de veure't|Quant de temps|Què tal va?|Adéu|Adeu-siau|Fins ara|Fins després|Fins demà|Gràcies per venir|Cuida't|Que descansis" }, \
      { L"🗣️##Frases de Conversa", L"Sí|No|D'acord|Potser|Ajuda|Espera|Deixa'm acabar|Necessito un moment|Més a poc a poc, si us plau|Ho pot repetir?|Què?|No ho entenc|Ho entenc|No ho sé|Que bé|Quina llàstima|Si us plau|Gràcies|De res|Perdó|Em sap greu|Digues" }, \
      { L"🚨##Frases d'Emergència", L"Ajuda|Necessito ajuda|No puc respirar|M'estic ofegant|Revisi el meu respirador|Truqui al 112|Truqui a una ambulància|Truqui al meu cuidador|Truqui a la meva família|Truqui a la policia|He caigut|Estic ferit|Estic sagnant|Emergència mèdica|Foc|Hi ha un accident|On és l'hospital?" }, \
      { L"🤲##Cura i Confort", L"Tinc set|Aigua, si us plau|Tinc gana|Necessito anar al bany|Estic incòmode|Aixeca'm|Estira'm|Gira'm|Ajusta el coixí|Mou-me el braç|Rasca'm el nas|Neteja'm la boca|Les meves ulleres, si us plau|Tinc fred|Tinc calor|Una manta, si us plau|Estic cansat|Deixa'm descansar|Encén el llum|Està molt alt" }, \
      { L"🏥##Frases de Salut", L"Necessito un metge|Necessito medicació|Necessito l'analgèsic|Em fa mal|El dolor va a pitjor|Mal aquí|Tinc dificultat per respirar|No puc empassar|Em marejo|Tinc nàusees|Tinc febre|Tinc ELA|Sóc al·lèrgic a...|Sóc diabètic|Parli'm a mi, no al meu cuidador|Em sento millor|On és urgències?" }, \
      { L"🏠##Família i Casa", L"Mare|Pare|La meva dona|El meu marit|Germà|Germana|T'estimo|Gràcies per ajudar-me|Vine, si us plau|Em pots ajudar?|Queda't amb mi|Voldria estar sol una estona|Sóc a casa|Me'n vaig|Ara torno|El sopar és a punt?|On és el meu telèfon?|Encén la tele|Posa música|Bona nit" }, \
      { L"👥##Frases Socials", L"Estic bé, gràcies|Avui no estic gaire bé|I tu?|Hi estic d'acord|No hi estic d'acord|De debò?|Quina gràcia|És interessant|Explica-m'ho|T'he trobat a faltar|Me n'alegro per tu|Ara prefereixo no parlar|Felicitats|Molt bé|Bona sort|Per molts anys|Salut" }, \
      { L"⏰##Hora i Data", L"Quina hora és?|Quin dia és avui?|Ara|En un minut|Aviat|D'aquí a una hora|Més tard|Encara no|Aquest matí|Aquesta tarda|Aquesta nit|Avui|Demà|Ahir|El cap de setmana|La setmana que ve" }, \
      { L"💼##Frases de Negocis", L"Em sent?|Té el micròfon silenciat|Doni'm un moment per escriure|Parlin d'un en un|Pot repetir la pregunta?|Tinc una pregunta|Seguim|Acabem aquí|Gràcies pel seu temps|Amb ganes|Si us plau, enviï els detalls|Posi-ho per escrit, si us plau|Programem una reunió|Podem reprogramar?|Faré seguiment" }, \
      { L"✈️##Frases de Viatge", L"Parla anglès?|No ho entenc|On és el lavabo?|Hi ha un lavabo accessible?|Faig servir cadira de rodes|Hi ha accés per a cadira de rodes?|On és l'entrada accessible?|Hi ha ascensor?|Necessito ajuda amb l'equipatge|On puc carregar el meu dispositiu?|Necessito un taxi|Porti'm a l'aeroport|On sóc?|M'ho pot mostrar al mapa?|Quant costa?|Aquest seient està ocupat?" }, \
      { L"🔊##Sons", L"Alerta::alert.wav|Bip::beep.wav|Crit::scream.wav|Aplaudiments::applause.wav|Rialla::laugh.wav|Ooh::aww.wav|Escridassada::boo.wav|Roc de tambor::drumroll.wav|Fallo::fail.wav|Rialla malvada::evil_laugh.wav|Timbre::doorbell.wav|Telèfon::phone_ring.wav|Tiroteig::shots.wav|Censura::censor.wav" } \
    } }, \
    { L"Chinese (Simplified)", { \
      { L"👋🏻##常见问候", L"你好|🔇##我不能说话，我用应用程序交流。请耐心等待。|您好|嗨|嘿|欢迎|请进|早上好|下午好|晚上好|晚安|很高兴认识你|见到你真好|好久不见|你好吗？|再见|拜拜|待会见|下次见|谢谢你来|保重|好好休息" }, \
      { L"🗣️##会话短语", L"是|不是|好的|也许|帮帮我|等一下|让我说完|我需要一点时间|请说慢一点|你能再说一遍吗|什么？|我不明白|我明白|我不知道|太好了|真可惜|请|谢谢|不客气|对不起|不好意思|告诉我" }, \
      { L"🚨##紧急用语", L"救命|我需要帮助|我无法呼吸|我噎住了|请检查我的呼吸机|请拨打112|请叫救护车|请联系我的照护者|请联系我的家人|报警|我摔倒了|我受伤了|我在流血|医疗紧急情况|着火了|发生事故了|医院在哪里？" }, \
      { L"🤲##照护与舒适", L"我渴了|请给我水|我饿了|我要上厕所|我不舒服|扶我坐起来|让我躺下|帮我翻身|调整枕头|移动我的手臂|帮我挠鼻子|帮我擦嘴|请给我眼镜|我冷|我热|请给我毯子|我累了|让我休息|开灯|太大声了" }, \
      { L"🏥##健康用语", L"我需要医生|我需要药物|我需要止痛药|我很痛|疼痛加重了|这里疼痛|我呼吸困难|我无法吞咽|我感到头晕|我想吐|我发烧了|我患有肌萎缩侧索硬化症|我对...过敏|我是一名糖尿病患者|请对我说话，不要对我的照护者说|我感觉好些了|急诊在哪里？" }, \
      { L"🏠##家庭与家", L"妈妈|爸爸|我的妻子|我的丈夫|哥哥|姐姐|我爱你|谢谢你帮我|请过来|你能帮我吗|陪着我|我想独处一会儿|我在家|我走了|马上回来|晚饭好了吗|我的手机在哪里|开电视|放点音乐|晚安" }, \
      { L"👥##社交用语", L"我很好，谢谢|今天不太好|你呢？|我同意|我不同意|真的吗？|很有趣|很有意思|多说一点|我很想你|我为你高兴|现在我不想说话|恭喜|做得好|祝你好运|生日快乐|干杯" }, \
      { L"⏰##时间与日期", L"现在几点？|今天星期几？|现在|一分钟后|很快|一小时后|稍后|还没有|今天早上|今天下午|今天傍晚|今晚|今天|明天|昨天|这个周末|下周" }, \
      { L"💼##商务用语", L"能听到我吗？|你的麦克风静音了|请给我一点时间打字|请一个一个说|能重复一下问题吗？|我有一个问题|我们继续|就到这里吧|感谢你的时间|期待合作|请发送详细信息|请以书面形式发送|我们安排一次会议|可以改期吗|我会跟进" }, \
      { L"✈️##旅行用语", L"你会说英语吗|我不明白|洗手间在哪里？|有无障碍洗手间吗？|我使用轮椅|有轮椅通道吗？|无障碍入口在哪里？|有电梯吗？|我需要行李帮助|哪里可以给设备充电？|我需要一辆出租车|带我去机场|我在哪里|你能在地图上指给我看吗|多少钱|这个座位有人吗" }, \
      { L"🔊##声音", L"警报::alert.wav|哔声::beep.wav|尖叫声::scream.wav|掌声::applause.wav|笑声::laugh.wav|噢::aww.wav|嘘声::boo.wav|鼓声::drumroll.wav|失败::fail.wav|邪恶的笑声::evil_laugh.wav|门铃::doorbell.wav|电话铃声::phone_ring.wav|枪声::shots.wav|审查::censor.wav" } \
    } }, \
    { L"English", { \
      { L"👋🏻##Frequent Greetings", L"Hello|🔇##I can't speak. I use this app to talk, please be patient.|Hi there|Hey|Welcome|Please come in|Good morning|Good afternoon|Good evening|Good night|Nice to meet you|Great to see you|Long time no see|How's it going?|Bye|Goodbye|See you later|See you soon|Talk soon|Thanks for coming|Take care|Sleep well" }, \
      { L"🗣️##Conversation Phrases", L"Yes|No|OK|Maybe|Help!|Wait|Let me finish|I need a moment|Slower, please|Can you repeat?|What?|I don't understand|I understand|I don't know|That's great|That's a shame|Please|Thank you!|You're welcome!|Sorry|Excuse me|Tell me" }, \
      { L"🚨##Emergency Phrases", L"Help|I need help|I can't breathe|I'm choking|Check my ventilator|Call 911|Call an ambulance|Call my caregiver|Call my family|Call the police|I fell|I am hurt|I'm bleeding|Medical emergency|Fire|There's an accident|Where is the hospital?" }, \
      { L"🤲##Care & Comfort", L"I'm thirsty|Water, please|I'm hungry|I need the bathroom|I'm uncomfortable|Sit me up|Lay me down|Turn me over|Adjust my pillow|Move my arm|Scratch my nose|Wipe my mouth|My glasses, please|I'm cold|I'm hot|A blanket, please|I'm tired|Let me rest|Turn on the light|Too loud" }, \
      { L"🏥##Health Phrases", L"I need a doctor|I need medication|I need my pain medication|I'm in pain|The pain is getting worse|Pain here|I'm having trouble breathing|I can't swallow|I feel dizzy|I feel nauseous|I have a fever|I have ALS|I am allergic to...|I am diabetic|Talk to me, not to my carer|I feel better|Where is urgent care?" }, \
      { L"🏠##Family & Home", L"Mom|Dad|Wife|Husband|Brother|Sister|I love you|Thank you for helping me|Come here, please|Can you help me?|Stay with me|I'd like some time alone|I am home|I'm leaving|Be right back|Is dinner ready?|Where is my phone?|Turn on the TV|Put on some music|Good night" }, \
      { L"👥##Social Phrases", L"I'm fine, thanks|Not so good today|How about you?|I agree|I don't agree|Really?|That's funny|That's interesting|Tell me more|I missed you|I'm happy for you|I'd rather not talk right now|Congratulations|Well done|Good luck|Happy birthday|Cheers!" }, \
      { L"⏰##Time & Date", L"What time is it?|What day is it?|Now|In a minute|Soon|In an hour|Later|Not yet|This morning|This afternoon|This evening|Tonight|Today|Tomorrow|Yesterday|This weekend|Next week" }, \
      { L"💼##Business Phrases", L"Can you hear me?|You're on mute|Give me a moment to type|Please speak one at a time|Please repeat the question|I have a question|Let's move on|Let's end here|Thank you for your time|Looking forward to it|Please send details|Please put it in writing|Let's schedule a meeting|Can we reschedule?|I will follow up" }, \
      { L"✈️##Travel Phrases", L"Do you speak English?|I don't understand|Where is the bathroom?|Is there an accessible bathroom?|I use a wheelchair|Is there wheelchair access?|Where is the accessible entrance?|Is there an elevator?|I need help with my luggage|Where can I charge my device?|I need a taxi|Take me to the airport|Where am I?|Can you show me on the map?|How much does it cost?|Is this seat taken?" }, \
      { L"🔊##Sounds", L"Alert::alert.wav|Beep::beep.wav|Scream::scream.wav|Applause::applause.wav|Laugh::laugh.wav|Aww::aww.wav|Boo::boo.wav|Drumroll::drumroll.wav|Fail::fail.wav|Evil laugh::evil_laugh.wav|Doorbell::doorbell.wav|Phone ring::phone_ring.wav|Shots::shots.wav|Censorship::censor.wav" } \
    } }, \
    { L"French", { \
      { L"👋🏻##Salutations Fréquentes", L"Bonjour|🔇##Je ne peux pas parler, j'utilise une application pour communiquer. Merci de votre patience.|Salut|Coucou|Bienvenue|Entrez, s'il vous plaît|Bon matin|Bon après-midi|Bonsoir|Bonne nuit|Enchanté|Ravi de vous voir|Ça fait longtemps|Comment ça va?|Au revoir|À bientôt|À plus|À tout à l'heure|À demain|Merci d'être venu|Prends soin de toi|Dors bien" }, \
      { L"🗣️##Phrases de Conversation", L"Oui|Non|D'accord|Peut-être|Aidez-moi|Attendez|Laissez-moi finir|J'ai besoin d'un moment|Plus lentement, s'il vous plaît|Pouvez-vous répéter?|Quoi ?|Je ne comprends pas|Je comprends|Je ne sais pas|C'est super|C'est dommage|S'il vous plaît|Merci|De rien|Pardon|Excusez-moi|Dis-moi" }, \
      { L"🚨##Phrases d'Urgence", L"Au secours|J'ai besoin d'aide|Je ne peux pas respirer|Je m'étouffe|Vérifiez mon respirateur|Appelez le 15|Appelez une ambulance|Appelez mon aidant|Appelez ma famille|Appelez la police|Je suis tombé|Je suis blessé|Je saigne|Urgence médicale|Incendie|Il y a un accident|Où est l'hôpital?" }, \
      { L"🤲##Soins & Confort", L"J'ai soif|De l'eau, s'il te plaît|J'ai faim|Je dois aller aux toilettes|Je suis mal installé|Redresse-moi|Allonge-moi|Tourne-moi|Ajuste mon oreiller|Bouge mon bras|Gratte-moi le nez|Essuie-moi la bouche|Mes lunettes, s'il te plaît|J'ai froid|J'ai chaud|Une couverture, s'il te plaît|Je suis fatigué|Laisse-moi me reposer|Allume la lumière|C'est trop fort" }, \
      { L"🏥##Santé", L"J'ai besoin d'un médecin|J'ai besoin de médicaments|J'ai besoin de mon antidouleur|J'ai mal|La douleur augmente|Douleur ici|J'ai du mal à respirer|Je n'arrive pas à avaler|Je me sens étourdi|J'ai la nausée|J'ai de la fièvre|J'ai la SLA|Je suis allergique à...|Je suis diabétique|Parlez-moi, pas à mon aidant|Je me sens mieux|Où sont les urgences?" }, \
      { L"🏠##Famille & Maison", L"Maman|Papa|Ma femme|Mon mari|Frère|Soeur|Je t'aime|Merci de m'aider|Viens, s'il te plaît|Tu peux m'aider?|Reste avec moi|J'aimerais être seul un moment|Je suis à la maison|Je pars|Je reviens|Le dîner est-il prêt?|Où est mon téléphone?|Allume la télé|Mets de la musique|Bonne nuit" }, \
      { L"👥##Social", L"Je vais bien, merci|Pas très bien aujourd'hui|Et toi?|Je suis d'accord|Je ne suis pas d'accord|Vraiment?|C'est drôle|C'est intéressant|Racontez-moi|Tu m'as manqué|Je suis content pour toi|Je préfère ne pas parler maintenant|Félicitations|Bien joué|Bonne chance|Joyeux anniversaire|Santé" }, \
      { L"⏰##Temps & Date", L"Quelle heure est-il?|Quel jour sommes-nous?|Maintenant|Dans une minute|Bientôt|Dans une heure|Plus tard|Pas encore|Ce matin|Cet après-midi|Ce soir|Cette nuit|Aujourd'hui|Demain|Hier|Ce week-end|La semaine prochaine" }, \
      { L"💼##Phrases d'Affaires", L"Vous m'entendez?|Votre micro est coupé|Laissez-moi un moment pour écrire|Parlez un par un|Pouvez-vous répéter la question?|J'ai une question|Passons à la suite|Terminons ici|Merci pour votre temps|Au plaisir|Veuillez envoyer les détails|Mettez-le par écrit, s'il vous plaît|Planifions une réunion|Pouvons-nous reprogrammer?|Je ferai le suivi" }, \
      { L"✈️##Phrases de Voyage", L"Parlez-vous anglais?|Je ne comprends pas|Où sont les toilettes?|Y a-t-il des toilettes accessibles?|J'utilise un fauteuil roulant|Y a-t-il un accès pour fauteuil roulant?|Où est l'entrée accessible?|Y a-t-il un ascenseur?|J'ai besoin d'aide avec mes bagages|Où puis-je recharger mon appareil?|J'ai besoin d'un taxi|Emmenez-moi à l'aéroport|Où suis-je?|Pouvez-vous me montrer sur la carte?|Combien ça coûte?|Ce siège est-il libre?" }, \
      { L"🔊##Sons", L"Alerte::alert.wav|Bip::beep.wav|Cri::scream.wav|Applaudissements::applause.wav|Rire::laugh.wav|Ooh::aww.wav|Bouh::boo.wav|Roulement de tambour::drumroll.wav|Échec::fail.wav|Rire diabolique::evil_laugh.wav|Sonnette::doorbell.wav|Téléphone::phone_ring.wav|Tirs::shots.wav|Censure::censor.wav" } \
    } }, \
    { L"Galician", { \
      { L"👋🏻##Saúdos Frecuentes", L"Ola|🔇##Non podo falar, uso unha aplicación para comunicarme. Teña paciencia.|Boas|Ei|Benvido|Pasa, por favor|Bos días|Boas tardes|Boas noites|Boa noite|Encantado de coñecerte|Aledome de verte|Hai canto tempo!|Que tal?|Adeus|Chao|Ata logo|Ata pronto|Ata mañá|Grazas por vir|Coidate|Que descanses" }, \
      { L"🗣️##Frases de Conversa", L"Si|Non|Vale|Quizais|Axuda|Agarda|Déixame acabar|Necesito un momento|Máis amodo, por favor|Pode repetir?|Que?|Non entendo|Entendo|Non o sei|Que ben|Que pena|Por favor|Grazas|De nada|Perdón|Síntoo|Cóntame" }, \
      { L"🚨##Frases de Emerxencia", L"Axuda|Necesito axuda|Non podo respirar|Estou afogando|Revise o meu respirador|Chame ao 112|Chame a unha ambulancia|Chame ao meu coidador|Chame á miña familia|Chame á policía|Caín|Estou ferido|Estou sangrando|Emerxencia médica|Lume|Hai un accidente|Onde está o hospital?" }, \
      { L"🤲##Coidado e Confort", L"Teño sede|Auga, por favor|Teño fame|Necesito ir ao baño|Estou incómodo|Séntame|Deítame|Dáme a volta|Axusta a almofada|Móveme o brazo|Ráscame o nariz|Límpame a boca|As miñas lentes, por favor|Teño frío|Teño calor|Unha manta, por favor|Estou canso|Déixame descansar|Acende a luz|Está moi alto" }, \
      { L"🏥##Frases de Saúde", L"Necesito un médico|Necesito medicación|Necesito o analxésico|Dóeme|A dor vai a peor|Doi aquí|Teño dificultade para respirar|Non podo engulir|Síntome mareado|Teño náuseas|Teño febre|Teño ELA|Son alérxico a...|Son diabético|Fáleme a min, non ao meu coidador|Síntome mellor|Onde está urxencias?" }, \
      { L"🏠##Familia e Fogar", L"Nai|Pai|A miña muller|O meu home|Irmán|Irmá|Quérote|Grazas por axudarme|Ven, por favor|Pódesme axudar?|Queda comigo|Quero estar só un rato|Estou na casa|Voume|Volvo axiña|A cea está lista?|Onde está o meu teléfono?|Acende a tele|Pon música|Boas noites" }, \
      { L"👥##Frases Sociais", L"Estou ben, grazas|Hoxe non estou moi ben|E ti?|Estou de acordo|Non estou de acordo|De verdade?|Que graza|É interesante|Cóntame máis|Boteite de menos|Alégrome por ti|Agora prefiro non falar|Parabéns|Moi ben|Boa sorte|Feliz aniversario|Saúde" }, \
      { L"⏰##Hora e Data", L"Que hora é?|Que día é hoxe?|Agora|Nun minuto|Pronto|Nunha hora|Máis tarde|Aínda non|Esta mañá|Esta tarde|Esta noite|Hoxe|Mañá|Onte|A fin de semana|A vindeira semana" }, \
      { L"💼##Frases de Negocios", L"Óeme?|Ten o micrófono silenciado|Déame un momento para escribir|Falen dun en un|Pode repetir a pregunta?|Teño unha pregunta|Sigamos|Acabemos aquí|Grazas polo seu tempo|Con ganas|Por favor envíe os detalles|Póñao por escrito, por favor|Programemos unha reunión|Podemos reprogramar?|Farei seguimento" }, \
      { L"✈️##Frases de Viaxe", L"Fala inglés?|Non entendo|Onde está o baño?|Hai un baño accesible?|Uso cadeira de rodas|Hai acceso para cadeira de rodas?|Onde está a entrada accesible?|Hai ascensor?|Necesito axuda coa equipaxe|Onde podo cargar o meu dispositivo?|Necesito un taxi|Léveme ao aeroporto|Onde estou?|Pode mostralo no mapa?|Canto custa?|Este asento está ocupado?" }, \
      { L"🔊##Sons", L"Alerta::alert.wav|Bip::beep.wav|Berro::scream.wav|Aplausos::applause.wav|Risos::laugh.wav|Ooh::aww.wav|Abucheo::boo.wav|Redobre de tambor::drumroll.wav|Fallo::fail.wav|Risa malvada::evil_laugh.wav|Campá::doorbell.wav|Teléfono::phone_ring.wav|Tiroteo::shots.wav|Censura::censor.wav" } \
    } }, \
    { L"German", { \
      { L"👋🏻##Häufige Grüße", L"Hallo|🔇##Ich kann nicht sprechen, ich benutze eine App zum Kommunizieren. Bitte haben Sie Geduld.|Hi|Hey|Willkommen|Kommen Sie herein|Guten Morgen|Guten Tag|Guten Abend|Gute Nacht|Freut mich, dich kennenzulernen|Schön dich zu sehen|Lange nicht gesehen|Wie geht's?|Auf Wiedersehen|Tschüss|Bis später|Bis bald|Bis morgen|Danke fürs Kommen|Mach's gut|Schlaf gut" }, \
      { L"🗣️##Konversationssätze", L"Ja|Nein|Okay|Vielleicht|Hilfe|Warte|Lass mich ausreden|Ich brauche einen Moment|Langsamer, bitte|Können Sie das wiederholen?|Was?|Ich verstehe nicht|Ich verstehe|Ich weiß nicht|Das ist toll|Das ist schade|Bitte|Danke|Gern geschehen|Entschuldigung|Es tut mir leid|Sag mir" }, \
      { L"🚨##Notfallsätze", L"Hilfe|Ich brauche Hilfe|Ich kann nicht atmen|Ich ersticke|Prüfen Sie mein Beatmungsgerät|Rufen Sie 112 an|Rufen Sie einen Rettungswagen|Rufen Sie meine Pflegekraft|Rufen Sie meine Familie|Rufen Sie die Polizei|Ich bin gestürzt|Ich bin verletzt|Ich blute|Medizinischer Notfall|Feuer|Es gab einen Unfall|Wo ist das Krankenhaus?" }, \
      { L"🤲##Pflege & Komfort", L"Ich habe Durst|Wasser, bitte|Ich habe Hunger|Ich muss auf die Toilette|Ich liege unbequem|Setz mich auf|Leg mich hin|Dreh mich um|Richte mein Kissen|Beweg meinen Arm|Kratz meine Nase|Wisch mir den Mund ab|Meine Brille, bitte|Mir ist kalt|Mir ist heiß|Eine Decke, bitte|Ich bin müde|Lass mich ausruhen|Mach das Licht an|Zu laut" }, \
      { L"🏥##Gesundheit", L"Ich brauche einen Arzt|Ich brauche Medikamente|Ich brauche mein Schmerzmittel|Ich habe Schmerzen|Die Schmerzen werden schlimmer|Schmerz hier|Ich habe Atembeschwerden|Ich kann nicht schlucken|Mir ist schwindelig|Mir ist übel|Ich habe Fieber|Ich habe ALS|Ich bin allergisch gegen...|Ich bin Diabetiker|Sprechen Sie mit mir, nicht mit meiner Pflegekraft|Mir geht es besser|Wo ist die Notaufnahme?" }, \
      { L"🏠##Familie & Zuhause", L"Mama|Papa|Meine Frau|Mein Mann|Bruder|Schwester|Ich liebe dich|Danke, dass du mir hilfst|Komm bitte her|Kannst du mir helfen?|Bleib bei mir|Ich möchte etwas allein sein|Ich bin zu Hause|Ich gehe|Bin gleich zurück|Ist das Abendessen fertig?|Wo ist mein Telefon?|Mach den Fernseher an|Mach Musik an|Gute Nacht" }, \
      { L"👥##Soziales", L"Mir geht's gut, danke|Heute nicht so gut|Und dir?|Ich stimme zu|Ich stimme nicht zu|Wirklich?|Das ist lustig|Das ist interessant|Erzähl mir mehr|Ich habe dich vermisst|Ich freue mich für dich|Ich möchte jetzt nicht reden|Glückwunsch|Gut gemacht|Viel Glück|Alles Gute zum Geburtstag|Prost" }, \
      { L"⏰##Zeit & Datum", L"Wie spät ist es?|Welcher Tag ist heute?|Jetzt|In einer Minute|Bald|In einer Stunde|Später|Noch nicht|Heute Morgen|Heute Nachmittag|Heute Abend|Heute Nacht|Heute|Morgen|Gestern|Dieses Wochenende|Nächste Woche" }, \
      { L"💼##Geschäftliche Sätze", L"Können Sie mich hören?|Ihr Mikrofon ist stumm|Geben Sie mir einen Moment zum Schreiben|Bitte einer nach dem anderen|Können Sie die Frage wiederholen?|Ich habe eine Frage|Machen wir weiter|Beenden wir hier|Danke für Ihre Zeit|Ich freue mich darauf|Bitte senden Sie Details|Bitte schriftlich|Lassen Sie uns ein Treffen planen|Können wir neu terminieren?|Ich melde mich" }, \
      { L"✈️##Reisesätze", L"Sprechen Sie Englisch?|Ich verstehe nicht|Wo ist die Toilette?|Gibt es eine barrierefreie Toilette?|Ich benutze einen Rollstuhl|Gibt es einen rollstuhlgerechten Zugang?|Wo ist der barrierefreie Eingang?|Gibt es einen Aufzug?|Ich brauche Hilfe mit dem Gepäck|Wo kann ich mein Gerät laden?|Ich brauche ein Taxi|Bringen Sie mich zum Flughafen|Wo bin ich?|Könnten Sie es mir auf der Karte zeigen?|Wie viel kostet das?|Ist dieser Platz frei?" }, \
      { L"🔊##Sounds", L"Alarm::alert.wav|Piepton::beep.wav|Schrei::scream.wav|Applaus::applause.wav|Lachen::laugh.wav|Aww::aww.wav|Uuuh::boo.wav|Trommelwirbel::drumroll.wav|Fehler::fail.wav|Teuflisches Lachen::evil_laugh.wav|Türklingel::doorbell.wav|Telefonklingeln::phone_ring.wav|Schüsse::shots.wav|Zensur::censor.wav" } \
    } }, \
    { L"Hebrew", { \
      { L"👋🏻##ברכות נפוצות", L"שלום|🔇##אני לא יכול לדבר, אני משתמש באפליקציה כדי לתקשר. אנא התאזרו בסבלנות.|היי|אהלן|ברוכים הבאים|היכנס בבקשה|בוקר טוב|צהריים טובים|ערב טוב|לילה טוב|נעים מאוד|שמח לראות אותך|מזמן לא נפגשנו|מה קורה?|להתראות|ביי|נדבר בקרוב|נתראה מחר|תודה שבאת|שמור על עצמך|שינה טובה" }, \
      { L"🗣️##ביטויי שיחה", L"כן|לא|בסדר|אולי|עזרה|רגע|תן לי לסיים|אני צריך רגע|לאט יותר, בבקשה|אפשר לחזור על זה?|מה?|אני לא מבין|אני מבין|אני לא יודע|זה נהדר|חבל|בבקשה|תודה|אין בעד מה|סליחה|אני מצטער/ת|תגיד לי" }, \
      { L"🚨##ביטויי חירום", L"עזרה|אני צריך עזרה|אני לא מצליח לנשום|אני נחנק|בדקו את מכשיר הנשימה שלי|תתקשרו ל-112|תקראו לאמבולנס|תתקשרו למטפל שלי|תתקשרו למשפחה שלי|תתקשרו למשטרה|נפלתי|נפצעתי|אני מדמם|מקרה חירום רפואי|שריפה|קרתה תאונה|איפה בית החולים?" }, \
      { L"🤲##טיפול ונוחות", L"אני צמא|מים, בבקשה|אני רעב|אני צריך לשירותים|לא נוח לי|הושיבו אותי|השכיבו אותי|הפכו אותי|סדרו את הכרית|הזיזו את היד שלי|גרדו לי את האף|נגבו לי את הפה|המשקפיים שלי, בבקשה|קר לי|חם לי|שמיכה, בבקשה|אני עייף|תנו לי לנוח|הדליקו את האור|רועש מדי" }, \
      { L"🏥##ביטויי בריאות", L"אני צריך רופא|אני צריך תרופות|אני צריך משכך כאבים|יש לי כאבים|הכאב מחמיר|כאב פה|קשה לי לנשום|אני לא מצליח לבלוע|אני מרגיש סחרחורת|יש לי בחילה|יש לי חום|יש לי ALS|אני אלרגי ל...|אני חולה סוכרת|דברו איתי, לא עם המטפל שלי|אני מרגיש טוב יותר|איפה מיון?" }, \
      { L"🏠##משפחה ובית", L"אמא|אבא|אשתי|בעלי|אח|אחות|אני אוהב אותך|תודה שאתה עוזר לי|בוא בבקשה|אתה יכול לעזור לי?|תישאר איתי|אני רוצה להיות לבד קצת|אני בבית|אני יוצא/ת|מיד חוזר/ת|הארוחה מוכנה?|איפה הטלפון שלי?|תדליק את הטלוויזיה|תשים מוזיקה|לילה טוב" }, \
      { L"👥##ביטויים חברתיים", L"אני בסדר, תודה|היום לא כל כך טוב|ואתה?|אני מסכים|אני לא מסכים|באמת?|זה מצחיק|מעניין|ספר/י לי עוד|התגעגעתי אליך|אני שמח בשמחתך|עכשיו אני מעדיף לא לדבר|מזל טוב|כל הכבוד|בהצלחה|יום הולדת שמח|לחיים" }, \
      { L"⏰##זמן ותאריך", L"מה השעה?|איזה יום היום?|עכשיו|בעוד דקה|בקרוב|בעוד שעה|אחר כך|עוד לא|הבוקר|אחר הצהריים|הערב|הלילה|היום|מחר|אתמול|בסוף השבוע|שבוע הבא" }, \
      { L"💼##ביטויים עסקיים", L"אתם שומעים אותי?|המיקרופון שלך מושתק|תנו לי רגע להקליד|דברו אחד בכל פעם|אפשר לחזור על השאלה?|יש לי שאלה|נמשיך|נסיים כאן|תודה על הזמן שלך|מצפה לזה|בבקשה שלחו פרטים|בבקשה שלחו בכתב|בוא נקבע פגישה|אפשר לדחות?|אחזור אליך" }, \
      { L"✈️##ביטויי נסיעות", L"אתה מדבר אנגלית?|אני לא מבין|איפה השירותים?|יש שירותים נגישים?|אני משתמש בכיסא גלגלים|יש נגישות לכיסא גלגלים?|איפה הכניסה הנגישה?|יש מעלית?|אני צריך עזרה עם המזוודות|איפה אפשר להטעין את המכשיר?|אני צריך מונית|קח אותי לשדה התעופה|איפה אני?|אפשר להראות לי במפה?|כמה זה עולה?|המקום הזה תפוס?" }, \
      { L"🔊##צלילים", L"התראה::alert.wav|ביפ::beep.wav|צרחות::scream.wav|מחיאות כפיים::applause.wav|צחוק::laugh.wav|אוו::aww.wav|בוז::boo.wav|תיפוף תופים::drumroll.wav|כישלון::fail.wav|צחוק רשע::evil_laugh.wav|פעמון דלת::doorbell.wav|טלפון מצלצל::phone_ring.wav|ירי::shots.wav|צנזורה::censor.wav" } \
    } }, \
    { L"Hindi", { \
      { L"👋🏻##अक्सर अभिवादन", L"नमस्ते|🔇##मैं बोल नहीं सकता, मैं संवाद के लिए एक ऐप का उपयोग करता हूँ। कृपया धैर्य रखें।|नमस्कार|प्रणाम|हाय|स्वागत है|कृपया अंदर आइए|शुभ प्रभात|शुभ संध्या|शुभ रात्रि|आपसे मिलकर खुशी हुई|आपको देखकर अच्छा लगा|काफी समय हो गया|कैसे हो?|अलविदा|बाय|जल्दी मिलते हैं|फिर मिलेंगे|कल मिलते हैं|आने के लिए धन्यवाद|ध्यान रखना|अच्छी नींद लो" }, \
      { L"🗣️##बातचीत के वाक्य", L"हाँ|नहीं|ठीक है|शायद|मदद|रुको|मुझे पूरा करने दो|मुझे एक पल चाहिए|कृपया धीरे बोलिए|क्या आप दोहरा सकते हैं?|क्या?|मैं नहीं समझा/समझी|मैं समझ गया/गई|मुझे नहीं पता|बहुत अच्छा|अफ़सोस की बात है|कृपया|धन्यवाद|कोई बात नहीं|माफ़ कीजिए|मुझे खेद है|मुझे बताओ" }, \
      { L"🚨##आपातकालीन वाक्य", L"मदद|मुझे मदद चाहिए|मैं साँस नहीं ले पा रहा/रही|मेरा दम घुट रहा है|मेरा वेंटिलेटर जाँचें|112 पर कॉल करें|एम्बुलेंस बुलाएँ|मेरे देखभालकर्ता को बुलाएँ|मेरे परिवार को फ़ोन करें|पुलिस को बुलाएँ|मैं गिर गया/गई|मैं घायल हूँ|मुझे खून बह रहा है|चिकित्सा आपातकाल|आग|दुर्घटना हुई है|अस्पताल कहाँ है?" }, \
      { L"🤲##देखभाल और आराम", L"मुझे प्यास लगी है|पानी चाहिए|मुझे भूख लगी है|मुझे शौचालय जाना है|मुझे असहज लग रहा है|मुझे बैठाओ|मुझे लिटाओ|मुझे पलटो|तकिया ठीक करो|मेरा हाथ हिलाओ|मेरी नाक खुजाओ|मेरा मुँह पोंछो|मेरा चश्मा दो|मुझे ठंड लग रही है|मुझे गर्मी लग रही है|एक कंबल दो|मैं थक गया/गई हूँ|मुझे आराम करने दो|बत्ती जलाओ|आवाज़ बहुत तेज़ है" }, \
      { L"🏥##स्वास्थ्य वाक्य", L"मुझे डॉक्टर चाहिए|मुझे दवा चाहिए|मुझे दर्द की दवा चाहिए|मुझे दर्द है|दर्द बढ़ रहा है|यहां दर्द है|मुझे साँस लेने में परेशानी है|मैं निगल नहीं सकता|मुझे चक्कर आ रहे हैं|मुझे मितली आ रही है|मुझे बुखार है|मुझे ए.एल.एस. है|मुझे ... से एलर्जी है|मैं मधुमेह का रोगी हूँ|मुझसे बात करें, मेरे देखभालकर्ता से नहीं|मुझे बेहतर लग रहा है|आपातकाल कहाँ है?" }, \
      { L"🏠##परिवार और घर", L"माँ|पिता|मेरी पत्नी|मेरे पति|भाई|बहन|मैं तुमसे प्यार करता हूँ|मेरी मदद के लिए धन्यवाद|कृपया यहाँ आओ|क्या तुम मेरी मदद कर सकते हो?|मेरे साथ रहो|मुझे कुछ देर अकेला रहना है|मैं घर पर हूँ|मैं जा रहा/रही हूँ|अभी आता/आती हूँ|क्या खाना तैयार है?|मेरा फ़ोन कहाँ है?|टीवी चालू करो|संगीत लगाओ|शुभ रात्रि" }, \
      { L"👥##सामाजिक वाक्य", L"मैं ठीक हूँ, धन्यवाद|आज तबीयत ठीक नहीं है|और आप?|मैं सहमत हूँ|मैं सहमत नहीं हूँ|सच में?|यह मज़ेदार है|यह रोचक है|और बताइए|मुझे आपकी याद आई|मुझे आपके लिए खुशी है|अभी बात नहीं करना चाहता|बधाई|शाबाश|शुभकामनाएँ|जन्मदिन मुबारक|चियर्स" }, \
      { L"⏰##समय और तारीख", L"समय क्या हुआ है?|आज कौन सा दिन है?|अभी|एक मिनट में|जल्दी ही|एक घंटे में|बाद में|अभी नहीं|आज सुबह|आज दोपहर|आज शाम|आज रात|आज|कल|बीता कल|इस सप्ताहांत|अगले हफ्ते" }, \
      { L"💼##व्यवसाय वाक्य", L"क्या आप मुझे सुन सकते हैं?|आपका माइक बंद है|मुझे लिखने के लिए एक पल दें|कृपया एक-एक करके बोलें|क्या आप प्रश्न दोहरा सकते हैं?|मेरा एक प्रश्न है|आगे बढ़ें|यहीं समाप्त करें|आपके समय के लिए धन्यवाद|मैं उत्सुक हूँ|कृपया विवरण भेजें|कृपया लिखित में भेजें|आइए मीटिंग तय करें|क्या हम समय बदल सकते हैं?|मैं फॉलो-अप करूँगा/करूँगी" }, \
      { L"✈️##यात्रा वाक्य", L"क्या आप अंग्रेज़ी बोलते हैं?|मुझे समझ नहीं आया|शौचालय कहाँ है?|क्या सुलभ शौचालय है?|मैं व्हीलचेयर का उपयोग करता हूँ|क्या व्हीलचेयर की सुविधा है?|सुलभ प्रवेश कहाँ है?|क्या लिफ्ट है?|मुझे सामान में मदद चाहिए|मैं अपना उपकरण कहाँ चार्ज करूँ?|मुझे टैक्सी चाहिए|मुझे हवाई अड्डे ले चलें|मैं कहाँ हूँ?|क्या आप नक्शे पर दिखा सकते हैं?|यह कितने का है?|क्या यह सीट खाली है?" }, \
      { L"🔊##ध्वनियाँ", L"अलर्ट::alert.wav|बीप::beep.wav|चीख::scream.wav|तालियाँ::applause.wav|हँसी::laugh.wav|आह::aww.wav|बू::boo.wav|ड्रमरोल::drumroll.wav|असफलता::fail.wav|शैतानी हँसी::evil_laugh.wav|डोरबेल::doorbell.wav|फ़ोन की घंटी::phone_ring.wav|गोलीबारी::shots.wav|सेंसरशिप::censor.wav" } \
    } }, \
    { L"Italian", { \
      { L"👋🏻##Saluti Frequenti", L"Ciao|🔇##Non riesco a parlare, uso un'applicazione per comunicare. Abbia pazienza.|Ehi|Salve|Benvenuto|Prego, entra|Buongiorno|Buon pomeriggio|Buonasera|Buonanotte|Piacere di conoscerti|Che piacere vederti|È da tanto!|Come va?|Arrivederci|A presto|A dopo|A domani|Alla prossima|Grazie per essere venuto|Stammi bene|Riposa bene" }, \
      { L"🗣️##Frasi di Conversazione", L"Sì|No|Va bene|Forse|Aiuto|Aspetta|Lasciami finire|Ho bisogno di un momento|Più lentamente, per favore|Puoi ripetere?|Cosa?|Non capisco|Capisco|Non lo so|Che bello|Che peccato|Per favore|Grazie|Prego|Scusa|Mi dispiace|Dimmi" }, \
      { L"🚨##Frasi di Emergenza", L"Aiuto|Ho bisogno di aiuto|Non riesco a respirare|Sto soffocando|Controlli il mio respiratore|Chiamate il 112|Chiamate un'ambulanza|Chiamate il mio assistente|Chiamate la mia famiglia|Chiamate la polizia|Sono caduto|Sono ferito|Sto sanguinando|Emergenza medica|Incendio|C'è un incidente|Dov'è l'ospedale?" }, \
      { L"🤲##Cura & Comfort", L"Ho sete|Acqua, per favore|Ho fame|Devo andare in bagno|Sono scomodo|Mettimi seduto|Sdraiami|Girami|Sistema il cuscino|Muovi il mio braccio|Grattami il naso|Pulisci la mia bocca|I miei occhiali, per favore|Ho freddo|Ho caldo|Una coperta, per favore|Sono stanco|Lasciami riposare|Accendi la luce|È troppo forte" }, \
      { L"🏥##Salute", L"Ho bisogno di un medico|Ho bisogno di medicine|Ho bisogno dell'antidolorifico|Ho dolore|Il dolore sta peggiorando|Dolore qui|Ho difficoltà a respirare|Non riesco a deglutire|Mi sento stordito|Ho la nausea|Ho la febbre|Ho la SLA|Sono allergico a...|Sono diabetico|Parli con me, non con il mio assistente|Mi sento meglio|Dov'è il pronto soccorso?" }, \
      { L"🏠##Famiglia & Casa", L"Mamma|Papà|Mia moglie|Mio marito|Fratello|Sorella|Ti voglio bene|Grazie per aiutarmi|Vieni, per favore|Mi puoi aiutare?|Resta con me|Vorrei stare un po' da solo|Sono a casa|Sto uscendo|Torno subito|La cena è pronta?|Dov'è il mio telefono?|Accendi la TV|Metti un po' di musica|Buonanotte" }, \
      { L"👥##Sociale", L"Sto bene, grazie|Oggi non molto bene|E tu?|Sono d'accordo|Non sono d'accordo|Davvero?|Che divertente|Interessante|Dimmi di più|Mi sei mancato|Sono felice per te|Ora preferisco non parlare|Congratulazioni|Ben fatto|In bocca al lupo|Buon compleanno|Salute" }, \
      { L"⏰##Tempo & Data", L"Che ora è?|Che giorno è oggi?|Adesso|Tra un minuto|Presto|Tra un'ora|Più tardi|Non ancora|Questa mattina|Questo pomeriggio|Questa sera|Stanotte|Oggi|Domani|Ieri|Questo fine settimana|La prossima settimana" }, \
      { L"💼##Frasi di Affari", L"Mi sentite?|Ha il microfono muto|Datemi un momento per scrivere|Parlate uno alla volta|Può ripetere la domanda?|Ho una domanda|Andiamo avanti|Chiudiamo qui|Grazie per il suo tempo|Non vedo l'ora|Per favore invii i dettagli|Lo metta per scritto, per favore|Programmiamo una riunione|Possiamo riprogrammare?|Farò seguito" }, \
      { L"✈️##Frasi di Viaggio", L"Parli inglese?|Non capisco|Dov'è il bagno?|C'è un bagno accessibile?|Uso una sedia a rotelle|C'è accesso per sedia a rotelle?|Dov'è l'ingresso accessibile?|C'è un ascensore?|Ho bisogno di aiuto con i bagagli|Dove posso caricare il mio dispositivo?|Ho bisogno di un taxi|Portami all'aeroporto|Dove sono?|Puoi mostrarmelo sulla mappa?|Quanto costa?|Questo posto è libero?" }, \
      { L"🔊##Suoni", L"Allarme::alert.wav|Beep::beep.wav|Urlo::scream.wav|Applausi::applause.wav|Risata::laugh.wav|Ooh::aww.wav|Buu::boo.wav|Rullo di tamburi::drumroll.wav|Fallimento::fail.wav|Risata diabolica::evil_laugh.wav|Campanello::doorbell.wav|Suoneria::phone_ring.wav|Spari::shots.wav|Censura::censor.wav" } \
    } }, \
    { L"Japanese", { \
      { L"👋🏻##よくある挨拶", L"こんにちは|🔇##話せません。アプリで伝えています。少しお待ちください。|やあ|ハロー|もしもし|いらっしゃい|どうぞお入りください|おはようございます|こんばんは|おやすみなさい|はじめまして|会えてうれしいです|久しぶり|お元気ですか？|さようなら|またね|じゃあね|また後で|また明日|来てくれてありがとう|お気をつけて|よく休んでください" }, \
      { L"🗣️##会話フレーズ", L"はい|いいえ|オーケー|たぶん|助けて|待って|最後まで言わせて|少し時間が必要です|もっとゆっくり話してください|もう一度言ってください|何？|わかりません|わかります|知りません|よかったですね|残念ですね|お願いします|ありがとうございます|どういたしまして|ごめんなさい|すみません|教えて" }, \
      { L"🚨##緊急時のフレーズ", L"助けて|助けが必要です|息ができません|のどが詰まりました|人工呼吸器を確認してください|112に電話してください|救急車を呼んでください|介護者に連絡してください|家族に連絡してください|警察を呼んでください|転倒しました|けがをしました|出血しています|医療の緊急事態|火事です|事故がありました|病院はどこですか？" }, \
      { L"🤲##ケアと快適さ", L"のどが渇きました|水をください|お腹が空きました|トイレに行きたいです|楽な姿勢ではありません|起こしてください|寝かせてください|体の向きを変えてください|枕を直してください|腕を動かしてください|鼻をかいてください|口をふいてください|眼鏡をください|寒いです|暑いです|毛布をください|疲れました|休ませてください|電気をつけてください|音が大きすぎます" }, \
      { L"🏥##健康のフレーズ", L"医者が必要です|薬剤が必要です|痛み止めが必要です|痛みがあります|痛みがひどくなっています|痛みがここに|息が苦しいです|飲み込めません|めまいがします|吐き気がします|熱があります|ALSです|私は...にアレルギーがあります|糖尿病です|介護者ではなく私に話してください|良くなりました|救急はどこですか？" }, \
      { L"🏠##家族と家", L"お母さん|お父さん|妻|夫|兄弟|姉妹|愛しています|助けてくれてありがとう|来てください|手伝ってもらえますか|一緒にいてください|少し一人にしてください|家にいます|出かけます|すぐ戻ります|夕食はできていますか|携帯はどこですか|テレビをつけて|音楽をかけて|おやすみなさい" }, \
      { L"👥##社交のフレーズ", L"元気です、ありがとう|今日はあまり良くありません|あなたはどうですか？|賛成です|賛成できません|本当に？|おもしろいですね|興味深いですね|もっと教えて|会いたかったです|うれしく思います|今は話したくありません|おめでとう|よくできました|頑張って|誕生日おめでとう|乾杯" }, \
      { L"⏰##時間と日付", L"今何時ですか？|今日は何曜日ですか？|今|1分後|もうすぐ|1時間後|後で|まだです|今朝|今日の午後|今日の夕方|今夜|今日|明日|昨日|今週末|来週" }, \
      { L"💼##ビジネスのフレーズ", L"聞こえますか？|マイクがミュートです|入力する時間をください|一人ずつ話してください|質問をもう一度お願いします|質問があります|次に進みましょう|ここで終わりましょう|お時間ありがとうございます|楽しみにしています|詳細を送ってください|書面でお願いします|会議を設定しましょう|変更できますか|後ほど連絡します" }, \
      { L"✈️##旅行のフレーズ", L"英語は話せますか|わかりません|トイレはどこですか？|バリアフリーのトイレはありますか？|車椅子を使っています|車椅子で入れますか？|バリアフリーの入口はどこですか？|エレベーターはありますか？|荷物を手伝ってください|どこで機器を充電できますか？|タクシーが必要です|空港へ連れて行ってください|ここはどこですか|地図で示してもらえますか|いくらですか|この席は空いていますか" }, \
      { L"🔊##音", L"警告::alert.wav|ビープ::beep.wav|悲鳴::scream.wav|拍手::applause.wav|笑い::laugh.wav|アウ::aww.wav|ブーイング::boo.wav|ドラムロール::drumroll.wav|失敗::fail.wav|悪役の笑い::evil_laugh.wav|ドアベル::doorbell.wav|電話ベル::phone_ring.wav|銃声::shots.wav|検閲::censor.wav" } \
    } }, \
    { L"Korean", { \
      { L"👋🏻##자주 하는 인사", L"안녕하세요|🔇##말을 할 수 없어서 앱으로 이야기합니다. 조금만 기다려 주세요.|안녕|하이|헤이|어서 오세요|들어오세요|좋은 아침|좋은 오후|좋은 저녁|잘 자|처음 뵙겠습니다|만나서 반가워요|오랜만이에요|잘 지냈어?|안녕히 가세요|잘 가|또 봐요|나중에 봐|와 주셔서 감사합니다|조심히 가|잘 쉬세요" }, \
      { L"🗣️##대화 문장", L"네|아니요|좋아요|아마도|도와주세요|잠깐만요|끝까지 말하게 해 주세요|잠시만요|조금 천천히 말해 주세요|다시 말씀해 주시겠어요?|뭐?|이해하지 못했어요|이해합니다|모르겠어요|잘됐네요|아쉽네요|부탁합니다|감사합니다|천만에요|죄송합니다|실례합니다|말해 줘" }, \
      { L"🚨##긴급 상황", L"도와주세요|도움이 필요해요|숨을 쉴 수 없어요|목이 막혔어요|호흡기를 확인해 주세요|112에 전화해 주세요|구급차를 불러 주세요|간병인에게 연락해 주세요|가족에게 연락해 주세요|경찰을 불러 주세요|넘어졌어요|다쳤어요|피가 나요|의료 응급상황|불이 났어요|사고가 났어요|병원이 어디예요?" }, \
      { L"🤲##돌봄과 편안함", L"목이 말라요|물 주세요|배가 고파요|화장실에 가야 해요|불편해요|앉혀 주세요|눕혀 주세요|돌려 주세요|베개를 고쳐 주세요|팔을 옮겨 주세요|코를 긁어 주세요|입을 닦아 주세요|안경 주세요|추워요|더워요|담요 주세요|피곤해요|쉬게 해 주세요|불 켜 주세요|너무 시끄러워요" }, \
      { L"🏥##건강", L"의사가 필요해요|약물이 필요해요|진통제가 필요해요|아파요|통증이 심해지고 있어요|여기가 아파요|숨쉬기 힘들어요|삼킬 수 없어요|어지러워요|메스꺼워요|열이 있어요|저는 루게릭병 환자예요|나는 ...에 알레르기가 있어요|나는 당뇨병 환자예요|간병인이 아니라 저에게 말해 주세요|좀 나아졌어요|응급실이 어디예요?" }, \
      { L"🏠##가족과 집", L"엄마|아빠|아내|남편|형|동생|사랑해요|도와줘서 고마워요|와 주세요|도와줄 수 있어요?|같이 있어 주세요|잠시 혼자 있고 싶어요|집에 있어요|나 갈게요|금방 올게요|저녁 준비됐어요?|내 핸드폰 어디에요?|TV 켜 주세요|음악 좀 틀어 주세요|안녕히 주무세요" }, \
      { L"👥##사회", L"잘 지내요, 감사합니다|오늘은 별로예요|당신은요?|동의해요|동의하지 않아요|정말요?|재미있네요|흥미롭네요|더 이야기해 주세요|보고 싶었어요|잘돼서 기뻐요|지금은 말하고 싶지 않아요|축하합니다|잘했어요|행운을 빌어요|생일 축하합니다|건배" }, \
      { L"⏰##시간과 날짜", L"지금 몇 시에요?|오늘 무슨 요일이에요?|지금|잠시 후|곧|한 시간 후|나중에|아직 아니에요|오늘 아침|오늘 오후|오늘 저녁|오늘 밤|오늘|내일|어제|이번 주말|다음 주" }, \
      { L"💼##비즈니스", L"제 말 들리세요?|마이크가 꺼져 있어요|입력할 시간을 주세요|한 분씩 말씀해 주세요|질문을 다시 말씀해 주시겠어요?|질문이 있어요|다음으로 넘어가요|여기서 마칠게요|시간 내주셔서 감사합니다|기대하고 있습니다|자세한 내용을 보내 주세요|서면으로 보내 주세요|회의 일정을 잡읍시다|일정을 변경할 수 있을까요?|추후에 다시 연락드리겠습니다" }, \
      { L"✈️##여행", L"영어 하세요?|이해가 안 돼요|화장실이 어디예요?|장애인 화장실이 있어요?|저는 휠체어를 사용해요|휠체어 접근이 가능해요?|장애인 출입구가 어디예요?|엘리베이터가 있어요?|짐 좀 도와주세요|기기를 어디서 충전할 수 있어요?|택시가 필요해요|공항으로 데려다 주세요|여기가 어디예요?|지도에서 보여 주실 수 있나요?|얼마예요|이 자리 비었나요?" }, \
      { L"🔊##소리", L"경고::alert.wav|삐 소리::beep.wav|비명::scream.wav|박수::applause.wav|웃음::laugh.wav|아::aww.wav|부우::boo.wav|드럼롤::drumroll.wav|실패::fail.wav|악당의 웃음::evil_laugh.wav|초인종::doorbell.wav|전화벨::phone_ring.wav|총소리::shots.wav|검열::censor.wav" } \
    } }, \
    { L"Portuguese", { \
      { L"👋🏻##Saudações Frequentes", L"Olá|🔇##Não consigo falar, uso uma aplicação para comunicar. Tenha paciência.|Boas|Olá a todos|Bem-vindo|Entre, por favor|Bom dia|Boa tarde|Boa noite|Muito prazer|Que bom ver-te|Há quanto tempo!|Tudo bem?|Adeus|Tchau|Até logo|Até já|Até amanhã|Obrigado por vir|Cuida-te|Descansa bem" }, \
      { L"🗣️##Frases de Conversa", L"Sim|Não|Está bem|Talvez|Ajuda|Espera|Deixa-me acabar|Preciso de um momento|Mais devagar, por favor|Pode repetir?|O quê?|Não percebo|Percebo|Não sei|Que bom|Que pena|Por favor|Obrigado|De nada|Desculpe|Com licença|Diga-me" }, \
      { L"🚨##Frases de Emergência", L"Socorro|Preciso de ajuda|Não consigo respirar|Estou a sufocar|Verifique o meu ventilador|Ligue para o 112|Chame uma ambulância|Ligue ao meu cuidador|Ligue à minha família|Chame a polícia|Caí|Estou ferido|Estou a sangrar|Emergência médica|Fogo|Houve um acidente|Onde é o hospital?" }, \
      { L"🤲##Cuidado & Conforto", L"Tenho sede|Água, por favor|Tenho fome|Preciso de ir à casa de banho|Estou desconfortável|Senta-me|Deita-me|Vira-me|Ajusta a almofada|Move o meu braço|Coça-me o nariz|Limpa-me a boca|Os meus óculos, por favor|Tenho frio|Tenho calor|Uma manta, por favor|Estou cansado|Deixa-me descansar|Acende a luz|Está muito alto" }, \
      { L"🏥##Saúde", L"Preciso de um médico|Preciso de medicação|Preciso do meu analgésico|Tenho dores|A dor está a piorar|Dói-me aqui|Tenho dificuldade em respirar|Não consigo engolir|Sinto tonturas|Estou com náuseas|Tenho febre|Tenho ELA|Sou alérgico a...|Sou diabético|Fale comigo, não com o meu cuidador|Sinto-me melhor|Onde ficam as urgências?" }, \
      { L"🏠##Família & Casa", L"Mãe|Pai|A minha mulher|O meu marido|Irmão|Irmã|Amo-te|Obrigado por me ajudares|Vem, por favor|Podes ajudar-me?|Fica comigo|Gostaria de estar sozinho um pouco|Estou em casa|Estou a sair|Volto já|O jantar está pronto?|Onde está o meu telemóvel?|Liga a televisão|Põe música|Boa noite" }, \
      { L"👥##Social", L"Estou bem, obrigado|Hoje não estou muito bem|E tu?|Concordo|Não concordo|A sério?|Tem graça|Isso é interessante|Conte-me mais|Senti a tua falta|Estou contente por ti|Agora prefiro não falar|Parabéns|Muito bem|Boa sorte|Feliz aniversário|Saúde" }, \
      { L"⏰##Hora & Data", L"Que horas são?|Que dia é hoje?|Agora|Num minuto|Em breve|Daqui a uma hora|Mais tarde|Ainda não|Esta manhã|Esta tarde|Esta noite|Hoje|Amanhã|Ontem|Este fim de semana|Na próxima semana" }, \
      { L"💼##Frases de Negócios", L"Está a ouvir-me?|Tem o microfone desligado|Dê-me um momento para escrever|Falem um de cada vez|Pode repetir a pergunta?|Tenho uma pergunta|Vamos avançar|Terminemos aqui|Obrigado pelo seu tempo|Fico a aguardar|Por favor, envie os detalhes|Ponha por escrito, por favor|Vamos marcar uma reunião|Podemos remarcar?|Vou dar seguimento" }, \
      { L"✈️##Frases de Viagem", L"Fala inglês?|Não percebo|Onde fica a casa de banho?|Há uma casa de banho acessível?|Uso cadeira de rodas|Há acesso para cadeira de rodas?|Onde fica a entrada acessível?|Há elevador?|Preciso de ajuda com a bagagem|Onde posso carregar o meu dispositivo?|Preciso de um táxi|Leve-me ao aeroporto|Onde estou?|Pode mostrar-me no mapa?|Quanto custa?|Este lugar está ocupado?" }, \
      { L"🔊##Sons", L"Alerta::alert.wav|Bip::beep.wav|Grito::scream.wav|Aplausos::applause.wav|Riso::laugh.wav|Ooh::aww.wav|Vaia::boo.wav|Rufar de tambores::drumroll.wav|Falha::fail.wav|Gargalhada maléfica::evil_laugh.wav|Campainha::doorbell.wav|Toque do telefone::phone_ring.wav|Disparos::shots.wav|Censura::censor.wav" } \
    } }, \
    { L"Portuguese (Brazil)", { \
      { L"👋🏻##Saudações Frequentes", L"Olá|🔇##Não consigo falar, uso um aplicativo para me comunicar. Tenha paciência.|Oi|E aí|Oi gente|Bem-vindo|Entre, por favor|Bom dia|Boa tarde|Boa noite|Prazer em conhecê-lo|Prazer em vê-lo|Quanto tempo!|Como vai?|Tchau|Até logo|Até mais|Até amanhã|Falou|Obrigado por vir|Se cuida|Durma bem" }, \
      { L"🗣️##Frases de Conversa", L"Sim|Não|Tá bom|Talvez|Ajuda|Espera|Deixa eu terminar|Preciso de um momento|Mais devagar, por favor|Pode repetir?|O quê?|Não entendo|Entendo|Não sei|Que bom|Que pena|Por favor|Obrigado|De nada|Desculpe|Com licença|Me diga" }, \
      { L"🚨##Frases de Emergência", L"Socorro|Preciso de ajuda|Não consigo respirar|Estou engasgando|Verifique meu ventilador|Ligue para 192|Chame uma ambulância|Ligue para meu cuidador|Ligue para minha família|Chame a polícia|Eu caí|Estou ferido|Estou sangrando|Emergência médica|Fogo|Houve um acidente|Onde é o hospital?" }, \
      { L"🤲##Cuidado & Conforto", L"Estou com sede|Água, por favor|Estou com fome|Preciso ir ao banheiro|Estou desconfortável|Me sente|Me deite|Me vire|Ajuste meu travesseiro|Mova meu braço|Coce meu nariz|Limpe minha boca|Meus óculos, por favor|Estou com frio|Estou com calor|Uma coberta, por favor|Estou cansado|Deixe-me descansar|Acenda a luz|Está muito alto" }, \
      { L"🏥##Saúde", L"Preciso de um médico|Preciso de medicação|Preciso do meu analgésico|Estou com dor|A dor está piorando|Dor aqui|Tenho dificuldade para respirar|Não consigo engolir|Estou com tontura|Estou com náusea|Tenho febre|Tenho ELA|Sou alérgico a...|Sou diabético|Fale comigo, não com meu cuidador|Estou me sentindo melhor|Onde fica o pronto-socorro?" }, \
      { L"🏠##Família & Casa", L"Mãe|Pai|Minha esposa|Meu marido|Irmão|Irmã|Eu te amo|Obrigado por me ajudar|Venha, por favor|Pode me ajudar?|Fique comigo|Quero ficar um pouco sozinho|Estou em casa|Estou saindo|Volto já|O jantar está pronto?|Onde está o meu celular?|Ligue a TV|Coloque uma música|Boa noite" }, \
      { L"👥##Social", L"Estou bem, obrigado|Hoje não estou muito bem|E você?|Concordo|Não concordo|Sério?|Que engraçado|Isso é interessante|Me conte mais|Senti sua falta|Estou feliz por você|Agora prefiro não conversar|Parabéns|Muito bem|Boa sorte|Feliz aniversário|Saúde" }, \
      { L"⏰##Tempo & Data", L"Que horas são?|Que dia é hoje?|Agora|Em um minuto|Logo|Daqui a uma hora|Mais tarde|Ainda não|Esta manhã|Esta tarde|Esta noite|Hoje|Amanhã|Ontem|Este fim de semana|Próxima semana" }, \
      { L"💼##Frases de Negócios", L"Está me ouvindo?|Seu microfone está mudo|Me dê um momento para escrever|Falem um por vez|Pode repetir a pergunta?|Tenho uma pergunta|Vamos seguir|Vamos encerrar aqui|Obrigado pelo seu tempo|Aguardando com expectativa|Por favor envie os detalhes|Coloque por escrito, por favor|Agendemos uma reunião|Podemos reagendar?|Vou acompanhar" }, \
      { L"✈️##Frases de Viagem", L"Fala inglês?|Não entendo|Onde fica o banheiro?|Tem banheiro acessível?|Uso cadeira de rodas|Tem acesso para cadeira de rodas?|Onde fica a entrada acessível?|Tem elevador?|Preciso de ajuda com a bagagem|Onde posso carregar meu dispositivo?|Preciso de um táxi|Me leve ao aeroporto|Onde estou?|Pode me mostrar no mapa?|Quanto custa?|Este lugar está ocupado?" }, \
      { L"🔊##Sons", L"Alerta::alert.wav|Beep::beep.wav|Grito::scream.wav|Aplausos::applause.wav|Riso::laugh.wav|Ooh::aww.wav|Uhu::boo.wav|Rufar de tambores::drumroll.wav|Falha::fail.wav|Risada maligna::evil_laugh.wav|Campainha::doorbell.wav|Toque do telefone::phone_ring.wav|Disparos::shots.wav|Censura::censor.wav" } \
    } }, \
    { L"Russian", { \
      { L"👋🏻##Частые приветствия", L"Привет|🔇##Я не могу говорить, я пользуюсь приложением для общения. Пожалуйста, будьте терпеливы.|Здравствуйте|Хай|Добро пожаловать|Входите, пожалуйста|Доброе утро|Добрый день|Добрый вечер|Спокойной ночи|Рад познакомиться|Рад тебя видеть|Давно не виделись|Как дела?|Пока|До свидания|До скорого|Увидимся|До завтра|Спасибо, что пришли|Береги себя|Спи хорошо" }, \
      { L"🗣️##Разговорные фразы", L"Да|Нет|Хорошо|Может быть|Помогите|Подождите|Дайте мне закончить|Мне нужна минутка|Медленнее, пожалуйста|Можете повторить?|Что?|Я не понимаю|Я понимаю|Я не знаю|Это здорово|Как жаль|Пожалуйста|Спасибо|Не за что|Извините|Мне очень жаль|Скажи мне" }, \
      { L"🚨##Экстренные фразы", L"Помогите|Мне нужна помощь|Я не могу дышать|Я задыхаюсь|Проверьте мой аппарат дыхания|Позвоните 112|Вызовите скорую|Позвоните моей сиделке|Позвоните моей семье|Позвоните в полицию|Я упал|Я ранен(а)|У меня кровотечение|Медицинская помощь|Пожар|Произошла авария|Где больница?" }, \
      { L"🤲##Забота и комфорт", L"Я хочу пить|Воды, пожалуйста|Я голоден|Мне нужно в туалет|Мне неудобно|Посади меня|Положи меня|Переверни меня|Поправь подушку|Подвинь мою руку|Почеши мне нос|Вытри мне рот|Мои очки, пожалуйста|Мне холодно|Мне жарко|Одеяло, пожалуйста|Я устал|Дай мне отдохнуть|Включи свет|Слишком громко" }, \
      { L"🏥##Фразы о здоровье", L"Мне нужен врач|Мне нужны лекарства|Мне нужно обезболивающее|У меня болит|Боль усиливается|Боль здесь|Мне трудно дышать|Я не могу глотать|У меня кружится голова|Меня тошнит|У меня температура|У меня БАС|У меня аллергия на...|Я диабетик|Говорите со мной, а не с моей сиделкой|Мне лучше|Где неотложка?" }, \
      { L"🏠##Семья и дом", L"Мама|Папа|Моя жена|Мой муж|Брат|Сестра|Я тебя люблю|Спасибо, что помогаешь мне|Подойди, пожалуйста|Можешь мне помочь?|Останься со мной|Я хочу побыть один|Я дома|Я ухожу|Скоро вернусь|Ужин готов?|Где мой телефон?|Включи телевизор|Включи музыку|Спокойной ночи" }, \
      { L"👥##Социальные фразы", L"Я в порядке, спасибо|Сегодня не очень|А ты?|Я согласен|Я не согласен|Правда?|Это смешно|Это интересно|Расскажите подробнее|Я скучал по тебе|Я рад за тебя|Сейчас не хочу говорить|Поздравляю|Отлично|Удачи|С днём рождения|За здоровье" }, \
      { L"⏰##Время и дата", L"Который час?|Какой сегодня день?|Сейчас|Через минуту|Скоро|Через час|Позже|Ещё нет|Сегодня утром|Сегодня днём|Сегодня вечером|Сегодня ночью|Сегодня|Завтра|Вчера|На этих выходных|На следующей неделе" }, \
      { L"💼##Деловые фразы", L"Вы меня слышите?|У вас выключен микрофон|Дайте мне минуту напечатать|Говорите по одному|Можете повторить вопрос?|У меня вопрос|Идём дальше|Давайте закончим|Спасибо за ваше время|С нетерпением жду|Пожалуйста, пришлите детали|Пришлите это в письменном виде|Давайте назначим встречу|Можем перенести?|Я свяжусь позже" }, \
      { L"✈️##Фразы для путешествий", L"Вы говорите по-английски?|Я не понимаю|Где туалет?|Есть доступный туалет?|Я пользуюсь инвалидной коляской|Есть доступ для инвалидной коляски?|Где доступный вход?|Есть лифт?|Мне нужна помощь с багажом|Где я могу зарядить устройство?|Мне нужно такси|Отвезите меня в аэропорт|Где я?|Можете показать на карте?|Сколько это стоит?|Это место занято?" }, \
      { L"🔊##Звуки", L"Предупреждение::alert.wav|Бип!::beep.wav|Крик::scream.wav|Аплодисменты::applause.wav|Смех::laugh.wav|Ооо::aww.wav|Буу::boo.wav|Барабанная дробь::drumroll.wav|Провал::fail.wav|Злобный смех::evil_laugh.wav|Дверной звонок::doorbell.wav|Звонок телефона::phone_ring.wav|Выстрелы::shots.wav|Цензура::censor.wav" } \
    } }, \
    { L"Spanish", { \
      { L"👋🏻##Saludos Frecuentes", L"¡Hola!|🔇##No puedo hablar, uso una aplicación para comunicarme. Tenga paciencia.|¡Hey!|¡Buenas!|¡Bienvenido!|Pasa, por favor|¡Buenos días!|¡Buenas tardes!|¡Buenas noches!|Encantado de conocerte|Encantado de verte|Cuánto tiempo|¿Qué tal?|¡Adiós!|¡Chao!|¡Hasta luego!|¡Hasta pronto!|¡Hasta mañana!|Seguimos en contacto|Gracias por venir|¡Cuídate!|Que descanses" }, \
      { L"🗣️##Frases de Conversación", L"Sí|No|Vale|Tal vez|Ayuda|¡Espera!|Déjame terminar|Necesito un momento|Más despacio, por favor|¿Puedes repetir?|¿Qué?|No entiendo|Entiendo|No sé|¡Qué bien!|¡Qué mal!|Por favor|¡Muchas gracias!|De nada|Perdón|Lo siento|Dime" }, \
      { L"🚨##Frases de Emergencia", L"Ayuda|Necesito ayuda|No puedo respirar|Me estoy atragantando|Revise mi respirador|Llame al 112|Llame a una ambulancia|Llame a mi cuidador|Llame a mi familia|Llame a la policía|Me he caído|Estoy herido|Estoy sangrando|Emergencia médica|Incendio|Hay un accidente|¿Dónde está el hospital?" }, \
      { L"🤲##Cuidado y Confort", L"Tengo sed|Agua, por favor|Tengo hambre|Necesito ir al baño|Estoy incómodo|Siéntame|Túmbame|Dame la vuelta|Ajusta la almohada|Muéveme el brazo|Ráscame la nariz|Límpiame la boca|Mis gafas, por favor|Tengo frío|Tengo calor|Una manta, por favor|Estoy cansado|Déjame descansar|Enciende la luz|Está muy alto" }, \
      { L"🏥##Salud", L"Necesito un doctor|Necesito medicación|Necesito mi analgésico|Me duele|El dolor va a peor|Dolor aquí|Tengo dificultad para respirar|No puedo tragar|Me siento mareado|Tengo náuseas|Tengo fiebre|Tengo ELA|Soy alérgico a...|Soy diabético|Hábleme a mí, no a mi cuidador|Me siento mejor|¿Dónde está urgencias?" }, \
      { L"🏠##Familia y Hogar", L"Mamá|Papá|Mi mujer|Mi marido|Hermano|Hermana|Te quiero|Gracias por ayudarme|Ven, por favor|¿Me ayudas?|Quédate conmigo|Quiero estar solo un rato|Estoy en casa|Me voy|Ahora vuelvo|¿La cena está lista?|¿Dónde está mi teléfono?|Enciende la tele|Pon música|Buenas noches" }, \
      { L"👥##Social", L"Estoy bien, gracias|Hoy no estoy muy bien|¿Y tú?|Estoy de acuerdo|No estoy de acuerdo|¿En serio?|Qué gracia|Qué interesante|Cuéntame más|Te he echado de menos|Me alegro por ti|Ahora prefiero no hablar|Felicidades|Bien hecho|Buena suerte|Feliz cumpleaños|¡Salud!" }, \
      { L"⏰##Tiempo y Fecha", L"¿Qué hora es?|¿Qué día es hoy?|Ahora|En un minuto|Pronto|En una hora|Más tarde|Todavía no|Esta mañana|Esta tarde|Esta noche|Hoy|Mañana|Ayer|El fin de semana|La próxima semana" }, \
      { L"💼##Frases de Negocios", L"¿Me oye?|Tiene el micrófono silenciado|Deme un momento para escribir|Hablen de uno en uno|¿Puede repetir la pregunta?|Tengo una pregunta|Sigamos|Terminemos aquí|Gracias por su tiempo|Espero con interés|Por favor envíe los detalles|Póngalo por escrito, por favor|Agendemos una reunión|¿Podemos reprogramar?|Le daré seguimiento" }, \
      { L"✈️##Frases de Viaje", L"¿Habla inglés?|No entiendo|¿Dónde está el baño?|¿Hay un baño accesible?|Uso silla de ruedas|¿Hay acceso para silla de ruedas?|¿Dónde está la entrada accesible?|¿Hay ascensor?|Necesito ayuda con el equipaje|¿Dónde puedo cargar mi dispositivo?|Necesito un taxi|Lléveme al aeropuerto|¿Dónde estoy?|¿Puede mostrarme en el mapa?|¿Cuánto cuesta?|¿Está ocupado este asiento?" }, \
      { L"🔊##Sonidos", L"Alerta::alert.wav|Bip::beep.wav|Grito::scream.wav|Aplausos::applause.wav|Risa::laugh.wav|Ooh::aww.wav|Abucheo::boo.wav|Redoble de tambor::drumroll.wav|Error::fail.wav|Risa maligna::evil_laugh.wav|Timbre::doorbell.wav|Teléfono::phone_ring.wav|Disparos::shots.wav|Censura::censor.wav" } \
    } }, \
    { L"Valencian", { \
      { L"👋🏻##Salutacions Freqüentes", L"Hola|🔇##No puc parlar, faig servir una aplicació per a comunicar-me. Tinga paciència.|Bones|Ei|Benvingut|Passa, per favor|Bon dia|Bona vesprada|Bona nit|Encantat de conéixer-te|Encantat de veure't|Quant de temps|Com va?|Adéu|Adeu-siau|Fins ara|Fins després|Fins demà|Gràcies per vindre|Cuida't|Que descanses" }, \
      { L"🗣️##Frases de Conversa", L"Sí|No|D'acord|Potser|Ajuda|Espera|Deixa'm acabar|Necessite un moment|Més a poc a poc, per favor|Ho pot repetir?|Què?|No ho entenc|Ho entenc|No ho sé|Que bé|Quina llàstima|Per favor|Gràcies|De res|Perdó|Em sap greu|Digues" }, \
      { L"🚨##Frases d'Emergència", L"Ajuda|Necessite ajuda|No puc respirar|M'estic ofegant|Revisen el meu respirador|Truqueu al 112|Truqueu a una ambulància|Truqueu al meu cuidador|Truqueu a la meua família|Truqueu a la policia|M'he caigut|Estic ferit|Estic sagnant|Emergència mèdica|Foc|Hi ha un accident|On és l'hospital?" }, \
      { L"🤲##Cura i Confort", L"Tinc set|Aigua, per favor|Tinc fam|Necessite anar al bany|Estic incòmode|Alça'm|Estira'm|Gira'm|Ajusta el coixí|Mou-me el braç|Rasca'm el nas|Neteja'm la boca|Les meues ulleres, per favor|Tinc fred|Tinc calor|Una manta, per favor|Estic cansat|Deixa'm descansar|Encén el llum|Està molt alt" }, \
      { L"🏥##Frases de Salut", L"Necessite un metge|Necessite medicació|Necessite l'analgèsic|Em fa mal|El dolor va a pitjor|Em fa mal ací|Tinc dificultat per respirar|No puc engolir|Em marege|Tinc nàusees|Tinc febre|Tinc ELA|Sóc al·lèrgic a...|Sóc diabètic|Parle'm a mi, no al meu cuidador|Em sent millor|On és urgències?" }, \
      { L"🏠##Família i Casa", L"Mare|Pare|La meua dona|El meu marit|Germà|Germana|T'estime|Gràcies per ajudar-me|Vine, per favor|Em pots ajudar?|Queda't amb mi|Voldria estar sol una estona|Sóc a casa|Me'n vaig|Ara torne|El sopar està a punt?|On és el meu telèfon?|Encén la tele|Posa música|Bona nit" }, \
      { L"👥##Frases Socials", L"Estic bé, gràcies|Hui no estic massa bé|I tu?|Hi estic d'acord|No hi estic d'acord|De veres?|Quina gràcia|És interessant|Conta'm més|T'he trobat a faltar|Me n'alegre per tu|Ara preferisc no parlar|Enhorabona|Molt bé|Bona sort|Feliç aniversari|Salut" }, \
      { L"⏰##Hora i Data", L"Quina hora és?|Quin dia és hui?|Ara|En un minut|Prompte|D'ací a una hora|Més tard|Encara no|Este matí|Esta vesprada|Esta nit|Hui|Demà|Ahir|El cap de setmana|La setmana que ve" }, \
      { L"💼##Frases de Negocis", L"Em sent?|Té el micròfon silenciat|Doneu-me un moment per a escriure|Parleu d'un en un|Pot repetir la pregunta?|Tinc una pregunta|Seguim|Acabem ací|Gràcies pel seu temps|Amb ganes|Per favor envieu els detalls|Poseu-ho per escrit, per favor|Programem una reunió|Podem reprogramar?|Faré seguiment" }, \
      { L"✈️##Frases de Viatge", L"Parla anglés?|No ho entenc|On és el lavabo?|Hi ha un lavabo accessible?|Faig servir cadira de rodes|Hi ha accés per a cadira de rodes?|On és l'entrada accessible?|Hi ha ascensor?|Necessite ajuda amb l'equipatge|On puc carregar el meu dispositiu?|Necessite un taxi|Porta'm a l'aeroport|On sóc?|M'ho pot mostrar al mapa?|Quant costa?|Este seient està ocupat?" }, \
      { L"🔊##Sons", L"Alerta::alert.wav|Bip::beep.wav|Crit::scream.wav|Aplaudiments::applause.wav|Rialla::laugh.wav|Ooh::aww.wav|Escridassada::boo.wav|Roc de tambor::drumroll.wav|Fallo::fail.wav|Rialla malvada::evil_laugh.wav|Timbre::doorbell.wav|Telèfon::phone_ring.wav|Tiroteig::shots.wav|Censura::censor.wav" } \
    } } \
  }

#endif // default_phrases_h
