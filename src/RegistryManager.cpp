#include "RegistryManager.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#define REG_KEY_NAME_BUFFER_SIZE 256
#define REG_KEY_DATA_BUFFER_SIZE 1024

#define SETTINGS_LANGUAGE_KEY L"Language"
#define SETTINGS_DEFAULT_TEXT_KEY L"Default Text"
#define SETTINGS_USE_DEFAULT_TEXT_KEY L"Use Default Text"
#define SETTINGS_USE_DEFAULT_TEXT_BOOLEAN false
#if SETTINGS_USE_DEFAULT_TEXT_BOOLEAN != true
#define SETTINGS_USE_DEFAULT_TEXT_VALUE L"0"
#else
#define SETTINGS_USE_DEFAULT_TEXT_VALUE L"1"
#endif
#define SETTINGS_DEFAULT_LANGUAGE_VALUE GetSystemLanguage()
#define SETTINGS_DEFAULT_TEXT_VALUE L""
#define SETTINGS_VOICE_KEY L"Voice Key"
#define SETTINGS_DEFAULT_VOICE_VALUE L""
#define SETTINGS_VOICE_VOLUME_KEY L"Voice Volume"
#define SETTINGS_DEFAULT_VOICE_VOLUME_VALUE 100
#define SETTINGS_VOICE_RATE_KEY L"Voice Rate"
#define SETTINGS_DEFAULT_VOICE_RATE_VALUE 0

Settings RegistryManager::m_Settings;

std::wstring RegistryManager::GetSystemLanguage()
{
  wchar_t langBuffer[LOCALE_NAME_MAX_LENGTH];
  if( GetUserDefaultLocaleName( langBuffer, LOCALE_NAME_MAX_LENGTH ) )
  {
    std::wstring lang( langBuffer );

    if( lang.find( L"en" ) == 0 ) return L"English";
    if( lang.find( L"es" ) == 0 ) return L"Spanish";
    if( lang.find( L"fr" ) == 0 ) return L"French";
    if( lang.find( L"de" ) == 0 ) return L"German";
    if( lang.find( L"it" ) == 0 ) return L"Italian";
    if( lang.find( L"pt" ) == 0 ) return L"Portuguese";
    if( lang.find( L"ru" ) == 0 ) return L"Russian";
    if( lang.find( L"ja" ) == 0 ) return L"Japanese";
    if( lang.find( L"zh" ) == 0 ) return L"Chinese";
    if( lang.find( L"ar" ) == 0 ) return L"Arabic";

    return L"English";
  }
  return L"English";
}

std::wstring RegistryManager::GetRegistryPath()
{
  return L"SOFTWARE\\SimonSays";
}

std::wstring RegistryManager::GetSettingsRegistryPath()
{
  return GetRegistryPath() + L"\\Settings";
}

std::wstring RegistryManager::GetPhrasesRegistryPath()
{
  return GetRegistryPath() + L"\\Phrases";
}

std::wstring RegistryManager::GetLanguageSpecificPath( const std::wstring & language )
{
  return GetPhrasesRegistryPath() + L"\\" + language;
}

std::vector<Category> RegistryManager::LoadCategoriesFromRegistry( std::wstring language )
{
  std::vector<Category> categories;
  if( language.empty() )
  {
    language = GetSystemLanguage();
  }
  std::wstring regPath = GetLanguageSpecificPath( language );

  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, GetPhrasesRegistryPath().c_str(), 0, KEY_READ, &hKey );

  if( result != ERROR_SUCCESS )
  {
    InstallDefaultPhrases();
  }
  else
  {
    RegCloseKey( hKey );
  }

  result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );

  if( result != ERROR_SUCCESS )
    return categories;

  DWORD index = 0;
  wchar_t valueName[REG_KEY_NAME_BUFFER_SIZE];
  wchar_t valueData[REG_KEY_DATA_BUFFER_SIZE];
  DWORD valueNameSize, valueDataSize, valueType;

  while( true )
  {
    valueNameSize = REG_KEY_NAME_BUFFER_SIZE;
    valueDataSize = REG_KEY_DATA_BUFFER_SIZE;

    result = RegEnumValue( hKey, index, valueName, &valueNameSize, NULL, &valueType,
      (LPBYTE) valueData, &valueDataSize );

    if( result == ERROR_NO_MORE_ITEMS ) break;
    if( result != ERROR_SUCCESS ) { index++; continue; }

    if( valueType == REG_SZ )
    {
      std::wstring categoryName( valueName );
      std::wstring categoryData( valueData );

      Category category = ParseCategoryFromRegistryData( categoryName, categoryData );
      if( !category.phrases.empty() )
      {
        categories.push_back( category );
      }
    }

    index++;
  }

  RegCloseKey( hKey );

  return categories;
}

Category RegistryManager::ParseCategoryFromRegistryData( const std::wstring & categoryName, const std::wstring & data )
{
  Category category;
  category.name = categoryName;

  std::wistringstream stream( data );
  std::wstring line;

  while( std::getline( stream, line, L'|' ) )
  {
    if( line.empty() ) continue;
    Phrase phrase;

    size_t pos1 = line.find( L"::" );

    if( pos1 != std::wstring::npos )
    {
      phrase.text = line.substr( 0, pos1 );
      phrase.audioFile = line.substr( pos1 + 2 );
    }
    else
    {
      phrase.text = line;
    }

    category.phrases.push_back( phrase );
  }

  return category;
}

bool RegistryManager::SaveCategoriesToRegistry( const std::vector<Category> & categories, std::wstring language )
{
  if( language.empty() )
  {
    language = GetSystemLanguage();
  }
  std::wstring regPath = GetLanguageSpecificPath( language );

  HKEY hKey;
  DWORD disposition;

  LONG result = RegCreateKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, NULL,
    REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition );

  if( result != ERROR_SUCCESS )
  {
    return false;
  }

  for( const auto & category : categories )
  {
    std::wstring serializedData = SerializeCategoryForRegistry( category );

    result = RegSetValueEx( hKey, category.name.c_str(), 0, REG_SZ,
      (LPBYTE) serializedData.c_str(),
      ( serializedData.length() + 1 ) * sizeof( wchar_t ) );

    if( result != ERROR_SUCCESS )
    {
      RegCloseKey( hKey );
      return false;
    }
  }

  RegCloseKey( hKey );
  return true;
}

std::wstring RegistryManager::SerializeCategoryForRegistry( const Category & category )
{
  std::wstring result;

  for( const auto & phrase : category.phrases )
  {
    if( !result.empty() )
    {
      result += L"|";
    }
    result += phrase.text;
    if( !phrase.audioFile.empty() )
    {
      result += L"::" + phrase.audioFile;
    }
  }

  return result;
}

Settings RegistryManager::LoadSettingsFromRegistry()
{
  m_Settings.language = SETTINGS_DEFAULT_LANGUAGE_VALUE;
  m_Settings.defaultText = SETTINGS_DEFAULT_TEXT_VALUE;
  m_Settings.useDefaultText = SETTINGS_USE_DEFAULT_TEXT_BOOLEAN;
  m_Settings.voice = SETTINGS_DEFAULT_VOICE_VALUE;
  m_Settings.volume = SETTINGS_DEFAULT_VOICE_VOLUME_VALUE;
  m_Settings.rate = SETTINGS_DEFAULT_VOICE_RATE_VALUE;

  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, GetSettingsRegistryPath().c_str(), 0, KEY_READ, &hKey );

  if( result != ERROR_SUCCESS )
  {
    InstallDefaultSettings();
    result = RegOpenKeyEx( HKEY_CURRENT_USER, GetSettingsRegistryPath().c_str(), 0, KEY_READ, &hKey );
    //if( result != ERROR_SUCCESS )

  }

  DWORD index = 0;
  wchar_t valueName[REG_KEY_NAME_BUFFER_SIZE];
  wchar_t valueData[REG_KEY_DATA_BUFFER_SIZE];
  DWORD valueNameSize, valueDataSize, valueType;

  while( true )
  {
    valueNameSize = REG_KEY_NAME_BUFFER_SIZE;
    valueDataSize = REG_KEY_DATA_BUFFER_SIZE;

    result = RegEnumValue( hKey, index, valueName, &valueNameSize, NULL, &valueType,
      (LPBYTE) valueData, &valueDataSize );

    if( result == ERROR_NO_MORE_ITEMS ) break;
    if( result != ERROR_SUCCESS ) { index++; continue; }

    if( valueType == REG_SZ )
    {
      std::wstring Name( valueName );
      std::wstring Data( valueData );

      if( Name == SETTINGS_LANGUAGE_KEY )
      {
        m_Settings.language = Data;
      }
      else if( Name == SETTINGS_DEFAULT_TEXT_KEY )
      {
        m_Settings.defaultText = Data;
      }
      else if( Name == SETTINGS_USE_DEFAULT_TEXT_KEY )
      {
        m_Settings.useDefaultText = ( Data == L"1" );
      }
      else if( Name == SETTINGS_VOICE_KEY )
      {
        m_Settings.voice = Data;
      }
      else if( Name == SETTINGS_VOICE_VOLUME_KEY )
      {
        m_Settings.volume = std::stoi( Data );
      }
      else if( Name == SETTINGS_VOICE_RATE_KEY )
      {
        m_Settings.rate = std::stoi( Data );
      }
    }

    index++;
  }

  RegCloseKey( hKey );

  return m_Settings;
}

bool RegistryManager::InstallDefaultSettings()
{
  const std::vector<std::pair<std::wstring, std::wstring>> default_settings_pairs = {
      { SETTINGS_LANGUAGE_KEY, SETTINGS_DEFAULT_LANGUAGE_VALUE },
      { SETTINGS_USE_DEFAULT_TEXT_KEY, SETTINGS_USE_DEFAULT_TEXT_VALUE },
      { SETTINGS_DEFAULT_TEXT_KEY, SETTINGS_DEFAULT_TEXT_VALUE },
      { SETTINGS_VOICE_KEY, SETTINGS_DEFAULT_VOICE_VALUE },
      { SETTINGS_VOICE_VOLUME_KEY, std::to_wstring( SETTINGS_DEFAULT_VOICE_VOLUME_VALUE ) },
      { SETTINGS_VOICE_RATE_KEY, std::to_wstring( SETTINGS_DEFAULT_VOICE_RATE_VALUE ) }
  };

  std::wstring regPath = GetSettingsRegistryPath();

  HKEY hKey;
  DWORD disposition;
  LONG result = RegCreateKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition );
  if( result != ERROR_SUCCESS ) return false;

  for( const auto & kv : default_settings_pairs )
  {
    const std::wstring & name = kv.first;
    const std::wstring & value = kv.second;

    result = RegSetValueEx( hKey, name.c_str(), 0, REG_SZ, (LPBYTE) value.c_str(), ( value.length() + 1 ) * sizeof( wchar_t ) );
    if( result != ERROR_SUCCESS ) { /* continue setting other values */ }
  }

  RegCloseKey( hKey );

  return true;
}

bool RegistryManager::InstallDefaultPhrases()
{
  // Create default categories and phrases in code and write them into registry under each language
  struct LangData { const wchar_t * lang; const wchar_t * keys; };

  const std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> allData = {
    { L"English", {
      { L"Common Phrases", L"Hello|Yes|No|Please|Thank you|Sorry|Excuse me|Help|Goodbye|I don't know|Maybe|I understand|I don't understand|Can you repeat?|I need a moment" },
      { L"Polite Phrases", L"Please|Thank you|You're welcome|Excuse me|May I?|Could you help me?|I appreciate it|Pardon me|Have a nice day|I apologize" },
      { L"Emergency Phrases", L"Help|Call911::call911.wav|I need help|I am hurt|Medical emergency|Fire|Call the police|I can't breathe|There's an accident|Where is the hospital" },
      { L"Travel Phrases", L"Where is the bathroom|How much does it cost|I don't understand|Do you speak English?|Where am I?|I need a taxi|Take me to the airport|What time is it|Can you show me on the map|Is this seat taken?" },
      { L"Business Phrases", L"Good morning|How are you|Nice to meet you|Thank you for your time|I appreciate it|Looking forward to it|Please send details|Let's schedule a meeting|Can we reschedule?|I will follow up" },
      { L"Social Phrases", L"How are you?|What's up?|I'm fine, thanks|Congratulations|Happy birthday|Good luck|Well done|That's interesting|Tell me more|See you soon" },
      { L"Health Phrases", L"I need a doctor|I am allergic to...|I have a fever|I am diabetic|I need medication|Where is urgent care|I feel dizzy|I am having trouble breathing|Pain here" },
      { L"Family & Home", L"Mom|Dad|Brother|Sister|I am home|Where is my phone|Is dinner ready?|I'm leaving|Be right back|Good night" },
      { L"Time & Date", L"What time is it|Today|Tomorrow|Yesterday|In an hour|Now|Later|This morning|This evening|Next week" },
      { L"Sounds", L"Applause::applause.wav|Laugh::laugh.wav|Beep::beep.wav|Phone ring::phone_ring.wav|Doorbell::doorbell.wav|Alert::alert.wav" }
    } },
    { L"Spanish", {
      { L"Frases Comunes", L"Hola|Sí|No|Por favor|Gracias|Perdón|Disculpe|Ayuda|Adiós|No sé|Tal vez|Entiendo|No entiendo|¿Puede repetir?|Necesito un momento" },
      { L"Frases Corteses", L"Por favor|Gracias|De nada|Disculpe|¿Puedo?|¿Puede ayudarme?|Se lo agradezco|Perdóneme|Que tenga un buen día|Lo siento" },
      { L"Frases de Emergencia", L"Ayuda|Llame al112::call112.wav|Necesito ayuda|Estoy herido|Emergencia médica|Incendio|Llame a la policía|No puedo respirar|Hay un accidente|¿Dónde está el hospital?" },
      { L"Frases de Viaje", L"¿Dónde está el baño?|¿Cuánto cuesta?|No entiendo|¿Habla inglés?|¿Dónde estoy?|Necesito un taxi|Lléveme al aeropuerto|¿Qué hora es?|¿Me lo puede mostrar en el mapa?|¿Está ocupado este asiento?" },
      { L"Frases de Negocios", L"Buenos días|¿Cómo está?|Mucho gusto|Gracias por su tiempo|Lo aprecio|Espero con interés|Por favor envíe los detalles|Agendemos una reunión|¿Podemos reprogramar?|Le daré seguimiento" },
      { L"Social", L"¿Qué tal?|Estoy bien, gracias|Felicidades|Feliz cumpleaños|Buena suerte|Bien hecho|Qué interesante|Cuéntame más|Nos vemos pronto" },
      { L"Salud", L"Necesito un doctor|Soy alérgico a...|Tengo fiebre|Soy diabético|Necesito medicación|¿Dónde está urgencias?|Me siento mareado|Tengo dificultad para respirar|Dolor aquí" },
      { L"Familia & Hogar", L"Mamá|Papá|Hermano|Hermana|Estoy en casa|¿Dónde está mi teléfono?|¿La cena está lista?|Me voy|Vuelvo enseguida|Buenas noches" },
      { L"Tiempo & Fecha", L"¿Qué hora es?|Hoy|Mañana|Ayer|En una hora|Ahora|Más tarde|Esta mañana|Esta noche|La próxima semana" },
      { L"Sonidos", L"Aplausos::applause.wav|Risa::laugh.wav|Tono::beep.wav|Teléfono::phone_ring.wav|Timbre::doorbell.wav|Alarma::alert.wav" }
    } },
    { L"French", {
      { L"Phrases Courantes", L"Bonjour|Oui|Non|S'il vous plaît|Merci|Pardon|Excusez-moi|Aidez-moi|Au revoir|Je ne sais pas|Peut-être|Je comprends|Je ne comprends pas|Pouvez-vous répéter?|J'ai besoin d'un moment" },
      { L"Phrases Polies", L"S'il vous plaît|Merci|De rien|Excusez-moi|Puis-je?|Pouvez-vous m'aider?|Je vous remercie|Pardonnez-moi|Bonne journée|Je m'excuse" },
      { L"Phrases d'Urgence", L"Au secours|Appelez le15::call15.wav|J'ai besoin d'aide|Je suis blessé|Urgence médicale|Incendie|Appelez la police|Je ne peux pas respirer|Il y a un accident|Où est l'hôpital?" },
      { L"Phrases de Voyage", L"Où sont les toilettes?|Combien ça coûte?|Je ne comprends pas|Parlez-vous anglais?|Où suis-je?|J'ai besoin d'un taxi|Emmenez-moi à l'aéroport|Quelle heure est-il?|Pouvez-vous me montrer sur la carte?|Ce siège est-il libre?" },
      { L"Phrases d'Affaires", L"Bonjour|Comment allez-vous?|Enchanté|Merci pour votre temps|Je l'apprécie|Au plaisir|Veuillez envoyer les détails|Planifions une réunion|Pouvons-nous reprogrammer?|Je ferai le suivi" },
      { L"Social", L"Ça va?|Je vais bien, merci|Félicitations|Joyeux anniversaire|Bonne chance|Bien joué|C'est intéressant|Racontez-moi|À bientôt" },
      { L"Santé", L"J'ai besoin d'un médecin|Je suis allergique à...|J'ai de la fièvre|Je suis diabétique|J'ai besoin de médicaments|Où est les urgences?|Je me sens étourdi|J'ai du mal à respirer|Douleur ici" },
      { L"Famille & Maison", L"Maman|Papa|Frère|Soeur|Je suis à la maison|Où est mon téléphone?|Le dîner est-il prêt?|Je pars|Je reviens|Bonne nuit" },
      { L"Temps & Date", L"Quelle heure est-il?|Aujourd'hui|Demain|Hier|Dans une heure|Maintenant|Plus tard|Ce matin|Ce soir|La semaine prochaine" },
      { L"Sons", L"Applaudissements::applause.wav|Rire::laugh.wav|Bip::beep.wav|Téléphone::phone_ring.wav|Sonnette::doorbell.wav|Alerte::alert.wav" }
    } },
    { L"German", {
      { L"Allgemeine Sätze", L"Hallo|Ja|Nein|Bitte|Danke|Entschuldigung|Verzeihung|Hilfe|Auf Wiedersehen|Ich weiß nicht|Vielleicht|Ich verstehe|Ich verstehe nicht|Können Sie das wiederholen?|Ich brauche einen Moment" },
      { L"Höfliche Sätze", L"Bitte|Danke|Gern geschehen|Entschuldigung|Darf ich?|Können Sie mir helfen?|Ich danke Ihnen|Verzeihen Sie mir|Einen schönen Tag noch|Es tut mir leid" },
      { L"Notfall Sätze", L"Hilfe|Rufen Sie112 an::call112.wav|Ich brauche Hilfe|Ich bin verletzt|Medizinischer Notfall|Feuer|Rufen Sie die Polizei|Ich kann nicht atmen|Es gab einen Unfall|Wo ist das Krankenhaus?" },
      { L"Reise Sätze", L"Wo ist die Toilette?|Wie viel kostet das?|Ich verstehe nicht|Sprechen Sie Englisch?|Wo bin ich?|Ich brauche ein Taxi|Bringen Sie mich zum Flughafen|Wie spät ist es?|Können Sie es mir auf der Karte zeigen?|Ist dieser Platz frei?" },
      { L"Geschäftliche Sätze", L"Guten Morgen|Wie geht es Ihnen?|Freut mich|Danke für Ihre Zeit|Ich schätze es|Ich freue mich darauf|Bitte senden Sie Details|Lassen Sie uns ein Treffen planen|Können wir neu terminieren?|Ich melde mich" },
      { L"Soziales", L"Wie geht's?|Mir geht's gut, danke|Glückwunsch|Alles Gute zum Geburtstag|Viel Glück|Gut gemacht|Das ist interessant|Erzähl mir mehr|Bis bald" },
      { L"Gesundheit", L"Ich brauche einen Arzt|Ich bin allergisch gegen...|Ich habe Fieber|Ich bin Diabetiker|Ich brauche Medikamente|Wo ist die Notaufnahme?|Mir ist schwindelig|Ich habe Atembeschwerden|Schmerz hier" },
      { L"Familie & Zuhause", L"Mama|Papa|Bruder|Schwester|Ich bin zu Hause|Wo ist mein Telefon?|Ist das Abendessen fertig?|Ich gehe|Bin gleich zurück|Gute Nacht" },
      { L"Zeit & Datum", L"Wie spät ist es?|Heute|Morgen|Gestern|In einer Stunde|Jetzt|Später|Heute Morgen|Heute Abend|Nächste Woche" },
      { L"Sounds", L"Applaus::applause.wav|Lachen::laugh.wav|Piepton::beep.wav|Telefonklingeln::phone_ring.wav|Türklingel::doorbell.wav|Alarm::alert.wav" }
    } },
    { L"Italian", {
      { L"Frasi Comuni", L"Ciao|Sì|No|Per favore|Grazie|Scusa|Mi scusi|Aiuto|Arrivederci|Non lo so|Forse|Capisco|Non capisco|Puoi ripetere?|Ho bisogno di un momento" },
      { L"Frasi Cortesi", L"Per favore|Grazie|Prego|Mi scusi|Posso?|Puoi aiutarmi?|Te ne sono grato|Scusami|Buona giornata|Mi scuso" },
      { L"Frasi di Emergenza", L"Aiuto|Chiamate il112::call112.wav|Ho bisogno di aiuto|Sono ferito|Emergenza medica|Incendio|Chiamate la polizia|Non riesco a respirare|C'è un incidente|Dov'è l'ospedale?" },
      { L"Frasi di Viaggio", L"Dov'è il bagno?|Quanto costa?|Non capisco|Parli inglese?|Dove sono?|Ho bisogno di un taxi|Portami all'aeroporto|Che ora è?|Puoi mostrarmelo sulla mappa?|Questo posto è libero?" },
      { L"Frasi di Affari", L"Buongiorno|Come sta?|Piacere di conoscerla|Grazie per il suo tempo|La apprezzo|Non vedo l'ora|Per favore invii i dettagli|Programmiamo una riunione|Possiamo riprogrammare?|Farò seguito" },
      { L"Sociale", L"Come va?|Sto bene, grazie|Congratulazioni|Buon compleanno|In bocca al lupo|Ben fatto|Interessante|Dimmi di più|A presto" },
      { L"Salute", L"Ho bisogno di un medico|Sono allergico a...|Ho la febbre|Sono diabetico|Ho bisogno di medicine|Dov'è il pronto soccorso?|Mi sento stordito|Ho difficoltà a respirare|Dolore qui" },
      { L"Famiglia & Casa", L"Mamma|Papà|Fratello|Sorella|Sono a casa|Dov'è il mio telefono?|La cena è pronta?|Sto uscendo|Torno subito|Buonanotte" },
      { L"Tempo & Data", L"Che ora è?|Oggi|Domani|Ieri|Tra un'ora|Adesso|Più tardi|Questa mattina|Questa sera|La prossima settimana" },
      { L"Suoni", L"Applausi::applause.wav|Risata::laugh.wav|Beep::beep.wav|Suoneria::phone_ring.wav|Campanello::doorbell.wav|Allarme::alert.wav" }
    } },
    { L"Portuguese", {
      { L"Frases Comuns", L"Olá|Sim|Não|Por favor|Obrigado|Desculpe|Com licença|Ajuda|Adeus|Não sei|Talvez|Entendo|Não entendo|Pode repetir?|Preciso de um momento" },
      { L"Frases Corteses", L"Por favor|Obrigado|De nada|Com licença|Posso?|Pode ajudar-me?|Agradeço|Perdoe-me|Tenha um bom dia|Peço desculpa" },
      { L"Frases de Emergência", L"Ajuda|Ligue para112::call112.wav|Preciso de ajuda|Estou ferido|Emergência médica|Fogo|Chame a polícia|Não consigo respirar|Houve um acidente|Onde é o hospital?" },
      { L"Frases de Viagem", L"Onde fica a casa de banho?|Quanto custa?|Não entendo|Fala inglês?|Onde estou?|Preciso de um táxi|Leve-me ao aeroporto|Que horas são?|Pode mostrar-me no mapa?|Este lugar está ocupado?" },
      { L"Frases de Negócios", L"Bom dia|Como está?|Prazer em conhecê-lo|Obrigado pelo seu tempo|Agradeço|Aguardando com expectativa|Por favor envie os detalhes|Vamos agendar uma reunião|Podemos reagendar?|Vou acompanhar" },
      { L"Social", L"Como vai?|Estou bem, obrigado|Parabéns|Feliz aniversário|Boa sorte|Bem feito|Isso é interessante|Conte-me mais|Até breve" },
      { L"Saúde", L"Preciso de um médico|Sou alérgico a...|Tenho febre|Sou diabético|Preciso de medicação|Onde fica o pronto-socorro?|Sinto tonturas|Tenho dificuldade em respirar|Dor aqui" },
      { L"Família & Casa", L"Mãe|Pai|Irmão|Irmã|Estou em casa|Onde está o meu telemóvel?|O jantar está pronto?|Estou a sair|Volto já|Boa noite" },
      { L"Tempo & Data", L"Que horas são?|Hoje|Amanhã|Ontem|Daqui a uma hora|Agora|Mais tarde|Esta manhã|Esta noite|Próxima semana" },
      { L"Sons", L"Aplausos::applause.wav|Riso::laugh.wav|Beep::beep.wav|Toque do telefone::phone_ring.wav|Campainha::doorbell.wav|Alerta::alert.wav" }
    } }
  };

  for( const auto & langPair : allData )
  {
    std::wstring language = langPair.first;
    std::wstring regPath = GetLanguageSpecificPath( language );

    HKEY hKey;
    DWORD disposition;
    LONG result = RegCreateKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition );
    if( result != ERROR_SUCCESS ) continue;

    for( const auto & kv : langPair.second )
    {
      const std::wstring & name = kv.first;
      const std::wstring & value = kv.second;

      result = RegSetValueEx( hKey, name.c_str(), 0, REG_SZ, (LPBYTE) value.c_str(), ( value.length() + 1 ) * sizeof( wchar_t ) );
      if( result != ERROR_SUCCESS ) { /* continue setting other values */ }
    }

    RegCloseKey( hKey );
  }

  return true;
}