#include "RegistryManager.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <sapi.h>
#pragma warning(disable:4996) 
#include <sphelper.h>
#pragma warning(default: 4996)

#define REG_KEY_NAME_BUFFER_SIZE 256
#define REG_KEY_DATA_BUFFER_SIZE 1024

#define REG_SETTINGS_LANGUAGE_NAME L"Language"
#define REG_SETTINGS_DEFAULT_LANGUAGE_VALUE GetSystemLanguage()
#define REG_SETTINGS_USE_DEFAULT_TEXT_NAME L"Use Default Text"
#define SETTINGS_USE_DEFAULT_TEXT_BOOLEAN false
#if SETTINGS_USE_DEFAULT_TEXT_BOOLEAN != true
#define REG_SETTINGS_USE_DEFAULT_TEXT_VALUE L"0"
#else
#define REG_SETTINGS_USE_DEFAULT_TEXT_VALUE L"1"
#endif
#define REG_SETTINGS_DEFAULT_TEXT_NAME L"Default Text"
#define REG_SETTINGS_DEFAULT_TEXT_VALUE L""
#define REG_SETTINGS_SELECTED_VOICE_NAME L"Voice Key"
#define REG_SETTINGS_DEFAULT_SELECTED_VOICE_VALUE L""
#define REG_SETTINGS_VOICE_VOLUME_NAME L"Voice Volume"
#define REG_SETTINGS_DEFAULT_VOICE_VOLUME_VALUE SIMONSAYS_SETTINGS_MAX_VOICE_VOLUME
#define REG_SETTINGS_VOICE_RATE_NAME L"Voice Rate"
#define REG_SETTINGS_DEFAULT_VOICE_RATE_VALUE 0

Settings RegistryManager::m_Settings;

std::wstring RegistryManager::GetLanguageStringFromLangId( LANGID langId )
{
  WORD primaryLangId = PRIMARYLANGID( langId );

  switch( primaryLangId )
  {
    case LANG_ARABIC: return L"Arabic";
    case LANG_BASQUE: return L"Basque";
    case LANG_BULGARIAN: return L"Bulgarian";
    case LANG_CATALAN: return L"Catalan";
    case LANG_CHINESE: return L"Chinese";
    case LANG_CZECH: return L"Czech";
    case LANG_DANISH: return L"Danish";
    case LANG_DUTCH: return L"Dutch";
    case LANG_ENGLISH: return L"English";
    case LANG_FINNISH: return L"Finnish";
    case LANG_FRENCH: return L"French";
    case LANG_GALICIAN: return L"Galician";
    case LANG_GERMAN: return L"German";
    case LANG_GREEK: return L"Greek";
    case LANG_HEBREW: return L"Hebrew";
    case LANG_HINDI: return L"Hindi";
    case LANG_HUNGARIAN: return L"Hungarian";
    case LANG_ICELANDIC: return L"Icelandic";
    case LANG_ITALIAN: return L"Italian";
    case LANG_JAPANESE: return L"Japanese";
    case LANG_KOREAN: return L"Korean";
    case LANG_NORWEGIAN: return L"Norwegian";
    case LANG_POLISH: return L"Polish";
    case LANG_PORTUGUESE: return L"Portuguese";
    case LANG_ROMANIAN: return L"Romanian";
    case LANG_RUSSIAN: return L"Russian";
    case LANG_SPANISH: return L"Spanish";
    case LANG_SWEDISH: return L"Swedish";
    case LANG_THAI: return L"Thai";
    case LANG_TURKISH: return L"Turkish";
    case LANG_UKRAINIAN: return L"Ukrainian";
      //case LANG_VALENCIAN: return L"Valencian";
    case LANG_VIETNAMESE: return L"Vietnamese";
    default: return L"Unknown";
  }
}

std::vector<LanguageInfo> RegistryManager::GetPhrasesLanguagesInRegistry()
{
  std::vector<LanguageInfo> languages = SUPPORTED_LANGUAGES;
  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, GetPhrasesRegistryPath().c_str(), 0, KEY_READ, &hKey );
  if( result != ERROR_SUCCESS )
  {
    InstallDefaultPhrases();
    result = RegOpenKeyEx( HKEY_CURRENT_USER, GetPhrasesRegistryPath().c_str(), 0, KEY_READ, &hKey );
  }
  if( result != ERROR_SUCCESS )
    return languages;
  DWORD index = 0;
  wchar_t valueName[REG_KEY_NAME_BUFFER_SIZE];
  DWORD valueNameSize;
  while( true )
  {
    valueNameSize = REG_KEY_NAME_BUFFER_SIZE;
    result = RegEnumKeyEx( hKey, index, valueName, &valueNameSize, NULL, NULL, NULL, NULL );
    if( result == ERROR_NO_MORE_ITEMS ) break;
    if( result != ERROR_SUCCESS ) { index++; continue; }
    bool languageExists = false;

    for( int i = 0; i < languages.size(); i++ )
    {
      if( languages[i].EnglishName == valueName )
      {
        languageExists = true;
        i = languages.size();
      }
    }

    if( !languageExists )
    {
      LanguageInfo languageFound;
      languageFound.EnglishName = valueName;
      languageFound.NativeName = valueName;
      languages.push_back( languageFound );
    }

    index++;
  }
  RegCloseKey( hKey );
  return languages;
}

std::wstring RegistryManager::GetSystemLanguage()
{
  wchar_t langBuffer[LOCALE_NAME_MAX_LENGTH];
  if( GetUserDefaultLocaleName( langBuffer, LOCALE_NAME_MAX_LENGTH ) )
  {
    std::wstring lang( langBuffer );

    // Normalize common locales to the language names used by SUPPORTED_LANGUAGES.
    if( lang.find( L"ar" ) == 0 ) return L"Arabic";
    if( lang.find( L"eu" ) == 0 ) return L"Basque";
    if( lang.find( L"ca" ) == 0 )
    {
      // Valencian is commonly represented as ca-ES-valencia.
      if( lang.find( L"ca-es-valencia" ) == 0 ) return L"Valencian";
      return L"Catalan";
    }
    if( lang.find( L"zh" ) == 0 ) return L"Chinese (Simplified)";
    if( lang.find( L"en" ) == 0 ) return L"English";
    if( lang.find( L"fr" ) == 0 ) return L"French";
    if( lang.find( L"gl" ) == 0 ) return L"Galician";
    if( lang.find( L"de" ) == 0 ) return L"German";
    if( lang.find( L"he" ) == 0 || lang.find( L"iw" ) == 0 ) return L"Hebrew";
    if( lang.find( L"hi" ) == 0 ) return L"Hindi";
    if( lang.find( L"it" ) == 0 ) return L"Italian";
    if( lang.find( L"ja" ) == 0 ) return L"Japanese";
    if( lang.find( L"ko" ) == 0 ) return L"Korean";
    if( lang.find( L"pt" ) == 0 ) return L"Portuguese";
    if( lang.find( L"ru" ) == 0 ) return L"Russian";
    if( lang.find( L"es" ) == 0 ) return L"Spanish";

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
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );

  if( result != ERROR_SUCCESS )
  {
    InstallDefaultPhrases( language );
    result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );
  }

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

std::vector<VoiceInfo> RegistryManager::PopulateAvaibleVoicesFromRegistry( std::wstring languageFilter )
{
  std::vector<VoiceInfo> voices;
  ISpObjectToken * pToken = nullptr;
  IEnumSpObjectTokens * pEnum = nullptr;
  HRESULT hr = SpEnumTokens( L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech_OneCore\\Voices", nullptr, nullptr, &pEnum );
  if( !SUCCEEDED( hr ) )
  {
    hr = SpEnumTokens( SPCAT_VOICES, nullptr, nullptr, &pEnum );
  }

  if( SUCCEEDED( hr ) && pEnum )
  {
    ULONG count = 0;
    hr = pEnum->GetCount( &count );
    if( SUCCEEDED( hr ) )
    {
      for( ULONG i = 0; i < count; i++ )
      {
        hr = pEnum->Item( i, &pToken );
        if( SUCCEEDED( hr ) && pToken )
        {
          wchar_t * pszId = nullptr;
          hr = pToken->GetId( &pszId );
          if( SUCCEEDED( hr ) && pszId )
          {
            LANGID langId;
            WCHAR * pszDesc = nullptr;
            SpGetDescription( pToken, &pszDesc );
            SpGetLanguageFromToken( pToken, &langId );
            std::wstring language = GetLanguageStringFromLangId( langId );
            if( languageFilter.empty() || StrStrIW( language.c_str(), languageFilter.c_str() ) != NULL )
            {
              VoiceInfo voice;
              voice.name = pszDesc ? pszDesc : pszId;
              voice.key = pszId;
              voice.language = language;
              voices.push_back( voice );
            }
            CoTaskMemFree( pszId );
            CoTaskMemFree( pszDesc );
          }
          pToken->Release();
          pToken = nullptr;
        }
      }
    }
    pEnum->Release();
    pEnum = nullptr;
  }

  return voices;
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
      DWORD( serializedData.length() + 1 ) * sizeof( wchar_t ) );

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
  m_Settings.language = REG_SETTINGS_DEFAULT_LANGUAGE_VALUE;
  m_Settings.defaultText = REG_SETTINGS_DEFAULT_TEXT_VALUE;
  m_Settings.useDefaultText = SETTINGS_USE_DEFAULT_TEXT_BOOLEAN;
  m_Settings.voice = REG_SETTINGS_DEFAULT_SELECTED_VOICE_VALUE;
  m_Settings.volume = REG_SETTINGS_DEFAULT_VOICE_VOLUME_VALUE;
  m_Settings.rate = REG_SETTINGS_DEFAULT_VOICE_RATE_VALUE;

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

      if( Name == REG_SETTINGS_LANGUAGE_NAME )
      {
        m_Settings.language = Data;
      }
      else if( Name == REG_SETTINGS_DEFAULT_TEXT_NAME )
      {
        m_Settings.defaultText = Data;
      }
      else if( Name == REG_SETTINGS_USE_DEFAULT_TEXT_NAME )
      {
        m_Settings.useDefaultText = ( Data == L"1" );
      }
      else if( Name == REG_SETTINGS_SELECTED_VOICE_NAME )
      {
        m_Settings.voice = Data;
      }
      else if( Name == REG_SETTINGS_VOICE_VOLUME_NAME )
      {
        m_Settings.volume = CLAMPED_VOICE_VOLUME( ( std::stoi( Data ) ) );
      }
      else if( Name == REG_SETTINGS_VOICE_RATE_NAME )
      {
        m_Settings.rate = CLAMPED_VOICE_RATE( ( std::stoi( Data ) ) );
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
      { REG_SETTINGS_LANGUAGE_NAME, REG_SETTINGS_DEFAULT_LANGUAGE_VALUE },
      { REG_SETTINGS_USE_DEFAULT_TEXT_NAME, REG_SETTINGS_USE_DEFAULT_TEXT_VALUE },
      { REG_SETTINGS_DEFAULT_TEXT_NAME, REG_SETTINGS_DEFAULT_TEXT_VALUE },
      { REG_SETTINGS_SELECTED_VOICE_NAME, REG_SETTINGS_DEFAULT_SELECTED_VOICE_VALUE },
      { REG_SETTINGS_VOICE_VOLUME_NAME, std::to_wstring( REG_SETTINGS_DEFAULT_VOICE_VOLUME_VALUE ) },
      { REG_SETTINGS_VOICE_RATE_NAME, std::to_wstring( REG_SETTINGS_DEFAULT_VOICE_RATE_VALUE ) }
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

    result = RegSetValueEx( hKey, name.c_str(), 0, REG_SZ, (LPBYTE) value.c_str(), DWORD( value.length() + 1 ) * sizeof( wchar_t ) );
    if( result != ERROR_SUCCESS ) { /* continue setting other values */ }
  }

  RegCloseKey( hKey );

  return true;
}

bool RegistryManager::InstallDefaultPhrases( std::wstring language )
{

  if( language.empty() )
  {
    language = GetSystemLanguage();
  }

  // Create default categories and phrases in code and write them into registry under each language
  struct LangData { const wchar_t * lang; const wchar_t * keys; };

  const auto allData = SIMONSAYS_DEFAULT_PHRASES_ALLDATA;

  for( const auto & langPair : allData )
  {
    if( langPair.first == language || language.empty() )
    {
      std::wstring regPath = GetLanguageSpecificPath( language );

      HKEY hKey;
      DWORD disposition;
      LONG result = RegCreateKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition );
      if( result != ERROR_SUCCESS ) continue;

      for( const auto & kv : langPair.second )
      {
        const std::wstring & name = kv.first;
        const std::wstring & value = kv.second;

        result = RegSetValueEx( hKey, name.c_str(), 0, REG_SZ, (LPBYTE) value.c_str(), DWORD( value.length() + 1 ) * sizeof( wchar_t ) );
        if( result != ERROR_SUCCESS ) { /* continue setting other values */ }
      }

      RegCloseKey( hKey );
    }
  }

  return true;
}

bool RegistryManager::SaveSettingsToRegistry( const Settings & s )
{
  // Prepare values (clamp volume/rate)
  Settings toSave = s;
  toSave.volume = CLAMPED_VOICE_VOLUME( ( toSave.volume ) );
  toSave.rate = CLAMPED_VOICE_RATE( toSave.rate );

  std::wstring regPath = GetSettingsRegistryPath();
  HKEY hKey;
  DWORD disposition;
  LONG result = RegCreateKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, NULL,
    REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition );

  if( result != ERROR_SUCCESS )
  {
    return false;
  }

  bool success = true;

  // Language
  result = RegSetValueEx( hKey, REG_SETTINGS_LANGUAGE_NAME, 0, REG_SZ,
    (LPBYTE) toSave.language.c_str(), DWORD( toSave.language.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Use Default Text
  std::wstring useDefault = toSave.useDefaultText ? L"1" : L"0";
  result = RegSetValueEx( hKey, REG_SETTINGS_USE_DEFAULT_TEXT_NAME, 0, REG_SZ,
    (LPBYTE) useDefault.c_str(), DWORD( useDefault.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Default Text
  result = RegSetValueEx( hKey, REG_SETTINGS_DEFAULT_TEXT_NAME, 0, REG_SZ,
    (LPBYTE) toSave.defaultText.c_str(), DWORD( toSave.defaultText.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Selected voice
  result = RegSetValueEx( hKey, REG_SETTINGS_SELECTED_VOICE_NAME, 0, REG_SZ,
    (LPBYTE) toSave.voice.c_str(), DWORD( toSave.voice.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Voice volume
  std::wstring volStr = std::to_wstring( toSave.volume );
  result = RegSetValueEx( hKey, REG_SETTINGS_VOICE_VOLUME_NAME, 0, REG_SZ,
    (LPBYTE) volStr.c_str(), DWORD( volStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Voice rate
  std::wstring rateStr = std::to_wstring( toSave.rate );
  result = RegSetValueEx( hKey, REG_SETTINGS_VOICE_RATE_NAME, 0, REG_SZ,
    (LPBYTE) rateStr.c_str(), DWORD( rateStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  RegCloseKey( hKey );

  if( success )
  {
    m_Settings = toSave;
  }

  return success;
}