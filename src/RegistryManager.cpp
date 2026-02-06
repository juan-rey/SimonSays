#include "RegistryManager.h"
#include "utils.h"
#include "default_phrases.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <sapi.h>
#pragma warning(disable:4996) // for SpHelper.h disabling deprecated warnings
#include <sphelper.h>
#pragma warning(default: 4996)

#define REG_KEY_NAME_BUFFER_SIZE 256
#define REG_KEY_DATA_BUFFER_SIZE 1024

// Settings registry value names and default values
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
#define REG_SETTINGS_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_NAME L"Speak Directly When Clicking Phrase"
#define REG_SETTINGS_DEFAULT_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_BOOLEAN true
#if !defined(REG_SETTINGS_DEFAULT_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_BOOLEAN) || REG_SETTINGS_DEFAULT_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_BOOLEAN
#define REG_SETTINGS_DEFAULT_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_VALUE L"1"
#else
#define REG_SETTINGS_DEFAULT_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_VALUE L"0"
#endif
#define REG_SETTINGS_REMEMBER_CATEGORY_WINDOW_SIZE_NAME L"Remember Category Window Size"
#define REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_BOOLEAN true
#if !defined(REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_BOOLEAN) || REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_BOOLEAN
#define REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_VALUE L"1"
#else
#define REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_VALUE L"0"
#endif
#define REG_SETTINGS_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_NAME L"Minimize Category Window Automatically"
#define REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_BOOLEAN true
#if !defined(REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_BOOLEAN) || REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_BOOLEAN
#define REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_VALUE L"1"
#else
#define REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_VALUE L"0"
#endif
#define REG_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_NAME L"Increase Volume When Playing"
#define REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_BOOLEAN true
#if !defined(REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_BOOLEAN) || REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_BOOLEAN
#define REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_VALUE L"1"
#else
#define REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_VALUE L"0"
#endif
#define REG_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_NAME L"Reduce Other Audio When Playing"
#define REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_BOOLEAN true
#if !defined(REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_BOOLEAN) || REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_BOOLEAN
#define REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_VALUE L"1"
#else
#define REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_VALUE L"0"
#endif

// Static member initialization
Settings RegistryManager::m_Settings; // DON'T DELETE THIS LINE

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
    valueNameSize = REG_KEY_NAME_BUFFER_SIZE * sizeof( wchar_t );
    result = RegEnumKeyEx( hKey, index, valueName, &valueNameSize, NULL, NULL, NULL, NULL );
    if( result == ERROR_NO_MORE_ITEMS ) break;
    if( result != ERROR_SUCCESS ) { index++; continue; }
    bool languageExists = false;

    for( int i = 0; i < languages.size(); i++ )
    {
      if( languages[i].EnglishName == valueName )
      {
        languageExists = true;
        i = (int) languages.size();
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

std::wstring RegistryManager::GetRegistryPath()
{
  return L"SOFTWARE\\SimonSays";
}

std::wstring RegistryManager::GetLastRunRegistryPath()
{
  return GetRegistryPath() + L"\\LastRun";
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
    valueNameSize = REG_KEY_NAME_BUFFER_SIZE * sizeof(wchar_t);
    valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof(wchar_t);

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
  HRESULT hr = SpEnumTokens( L"HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Speech\\Voices", nullptr, nullptr, &pEnum );
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

  hr = SpEnumTokens( L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech_OneCore\\Voices", nullptr, nullptr, &pEnum );
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
  m_Settings.speakDirectlyWhenClickingPhrase = REG_SETTINGS_DEFAULT_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_BOOLEAN;
  m_Settings.rememberCategoryWindowSize = REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_BOOLEAN;
  m_Settings.minimizeCategoryWindowAutomatically = REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_BOOLEAN;
  m_Settings.increaseVolumeWhenPlaying = REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_BOOLEAN;
  m_Settings.reduceOtherAudioWhenPlaying = REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_BOOLEAN;

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
    valueNameSize = REG_KEY_NAME_BUFFER_SIZE * sizeof(wchar_t);
    valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof(wchar_t);

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
      else if( Name == REG_SETTINGS_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_NAME )
      {
        m_Settings.speakDirectlyWhenClickingPhrase = ( Data == L"1" );
      }
      else if( Name == REG_SETTINGS_REMEMBER_CATEGORY_WINDOW_SIZE_NAME )
      {
        m_Settings.rememberCategoryWindowSize = ( Data == L"1" );
      }
      else if( Name == REG_SETTINGS_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_NAME )
      {
        m_Settings.minimizeCategoryWindowAutomatically = ( Data == L"1" );
      }
      else if( Name == REG_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_NAME )
      {
        m_Settings.increaseVolumeWhenPlaying = ( Data == L"1" );
      }
      else if( Name == REG_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_NAME )
      {
        m_Settings.reduceOtherAudioWhenPlaying = ( Data == L"1" );
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
      { REG_SETTINGS_VOICE_RATE_NAME, std::to_wstring( REG_SETTINGS_DEFAULT_VOICE_RATE_VALUE ) },
      { REG_SETTINGS_REMEMBER_CATEGORY_WINDOW_SIZE_NAME, REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_VALUE },
      { REG_SETTINGS_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_NAME, REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_VALUE },
      { REG_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_NAME, REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_VALUE },
      { REG_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_NAME, REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_VALUE }
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

  const auto allData = DEFAULT_FREQUENT_PHRASES_CATEGORIES_ALL_LANGUAGES;

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

  // Speak Directly When Clicking Phrase
  std::wstring speakDirectlyStr = toSave.speakDirectlyWhenClickingPhrase ? L"1" : L"0";
  result = RegSetValueEx( hKey, REG_SETTINGS_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_NAME, 0, REG_SZ,
    (LPBYTE) speakDirectlyStr.c_str(), DWORD( speakDirectlyStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Remember Category Window Size
  std::wstring rememberSizeStr = toSave.rememberCategoryWindowSize ? L"1" : L"0";
  result = RegSetValueEx( hKey, REG_SETTINGS_REMEMBER_CATEGORY_WINDOW_SIZE_NAME, 0, REG_SZ,
    (LPBYTE) rememberSizeStr.c_str(), DWORD( rememberSizeStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Minimize Category Window Automatically
  std::wstring minimizeAutoStr = toSave.minimizeCategoryWindowAutomatically ? L"1" : L"0";
  result = RegSetValueEx( hKey, REG_SETTINGS_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_NAME, 0, REG_SZ,
    (LPBYTE) minimizeAutoStr.c_str(), DWORD( minimizeAutoStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Increase Volume When Playing
  std::wstring increaseVolumeStr = toSave.increaseVolumeWhenPlaying ? L"1" : L"0";
  result = RegSetValueEx( hKey, REG_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_NAME, 0, REG_SZ,
    (LPBYTE) increaseVolumeStr.c_str(), DWORD( increaseVolumeStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Reduce Other Audio When Playing
  std::wstring reduceOtherAudioStr = toSave.reduceOtherAudioWhenPlaying ? L"1" : L"0";
  result = RegSetValueEx( hKey, REG_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_NAME, 0, REG_SZ,
    (LPBYTE) reduceOtherAudioStr.c_str(), DWORD( reduceOtherAudioStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  RegCloseKey( hKey );

  if( success )
  {
    m_Settings = toSave;
  }

  return success;
}

bool RegistryManager::LoadCategoryWindowSizeFromRegistry( int & width, int & height )
{
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );
  if( result != ERROR_SUCCESS )
  {
    return false;
  }
  wchar_t valueData[REG_KEY_DATA_BUFFER_SIZE];
  DWORD valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof(wchar_t);
  DWORD valueType;
  result = RegGetValue( hKey, NULL, L"Category Window Size", RRF_RT_REG_SZ, &valueType,
    (LPBYTE) valueData, &valueDataSize );
  if( result != ERROR_SUCCESS )
  {
    RegCloseKey( hKey );
    return false;
  }
  std::wstring sizeStr( valueData );
  size_t commaPos = sizeStr.find( L"x" );
  if( commaPos == std::wstring::npos )
  {
    RegCloseKey( hKey );
    return false;
  }
  else
  {
    width = std::stoi( sizeStr.substr( 0, commaPos ) );
    height = std::stoi( sizeStr.substr( commaPos + 1 ) );
  }

  RegCloseKey( hKey );

  return ( ( width > 0 ) && ( height > 0 ) );
}

bool RegistryManager::SaveCategoryWindowSizeToRegistry( int width, int height )
{
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  DWORD disposition;
  LONG result = RegCreateKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, NULL,
    REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition );
  if( result != ERROR_SUCCESS )
  {
    return false;
  }
  bool success = true;
  std::wstring sizeStr = std::to_wstring( width ) + L"x" + std::to_wstring( height );
  result = RegSetValueEx( hKey, L"Category Window Size", 0, REG_SZ,
    (LPBYTE) sizeStr.c_str(), DWORD( sizeStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;
  RegCloseKey( hKey );
  return success;
}

bool RegistryManager::SaveVersionToRegistry( std::wstring version )
{
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  DWORD disposition;
  LONG result = RegCreateKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, NULL,
    REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition );
  if( result != ERROR_SUCCESS )
  {
    return false;
  }
  bool success = true;
  result = RegSetValueEx( hKey, L"Version", 0, REG_SZ,
    (LPBYTE) version.c_str(), DWORD( version.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;
  RegCloseKey( hKey );
  return success;
}

std::wstring RegistryManager::GetLastRunVersionToRegistry()
{
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );
  if( result != ERROR_SUCCESS )
  {
    return L"";
  }
  wchar_t valueData[REG_KEY_DATA_BUFFER_SIZE];
  DWORD valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof(wchar_t);
  DWORD valueType;
  result = RegGetValue( hKey, NULL, L"Version", RRF_RT_REG_SZ, &valueType,
    (LPBYTE) valueData, &valueDataSize );
  if( result != ERROR_SUCCESS )
  {
    RegCloseKey( hKey );
    return L"";
  }
  std::wstring versionStr( valueData );
  RegCloseKey( hKey );
  return versionStr;
}