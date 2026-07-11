/*
   RegistryManager.cpp: RegistryManager class source file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "RegistryManager.h"
#include "utils.h"
#include "default_phrases.h"
#include <stdexcept>
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
#define REG_SETTINGS_USE_DEFAULT_TEXT_VALUE ( SETTINGS_USE_DEFAULT_TEXT_BOOLEAN ) ? ( L"1" ) : ( L"0" )

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
#define REG_SETTINGS_DEFAULT_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_VALUE ( REG_SETTINGS_DEFAULT_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_BOOLEAN ) ? ( L"1" ) : ( L"0" )

#define REG_SETTINGS_REMEMBER_CATEGORY_WINDOW_SIZE_NAME L"Remember Category Window Size"
#define REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_BOOLEAN true
#define REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_VALUE ( REG_SETTINGS_DEFAULT_REMEMBER_CATEGORY_WINDOW_SIZE_BOOLEAN ) ? ( L"1" ) : ( L"0" )

#define REG_SETTINGS_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_NAME L"Minimize Category Window Automatically"
#define REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_BOOLEAN true
#define REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_VALUE ( REG_SETTINGS_DEFAULT_MINIMIZE_CATEGORY_WINDOW_AUTOMATICALLY_BOOLEAN ) ? ( L"1" ) : ( L"0" )

#define REG_SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_NAME L"Increase Volume When Playing"
#define REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_BOOLEAN true
#define REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_VALUE ( REG_SETTINGS_DEFAULT_INCREASE_VOLUME_WHEN_PLAYING_BOOLEAN ) ? ( L"1" ) : ( L"0" )

#define REG_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_NAME L"Reduce Other Audio When Playing"
#define REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_BOOLEAN true
#define REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_VALUE ( REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_BOOLEAN ) ? ( L"1" ) : ( L"0" )

#define REG_SETTINGS_STOP_PREVIOUS_PLAYBACK_NAME L"Stop Previous Playback"
#define REG_SETTINGS_DEFAULT_STOP_PREVIOUS_PLAYBACK_BOOLEAN false
#define REG_SETTINGS_DEFAULT_STOP_PREVIOUS_PLAYBACK_VALUE ( REG_SETTINGS_DEFAULT_STOP_PREVIOUS_PLAYBACK_BOOLEAN ) ? ( L"1" ) : ( L"0" )

#define REG_SETTINGS_SHOW_TOUCH_KEYBOARD_NAME L"Show Touch Keyboard"
#define REG_SETTINGS_DEFAULT_SHOW_TOUCH_KEYBOARD_BOOLEAN false
#define REG_SETTINGS_DEFAULT_SHOW_TOUCH_KEYBOARD_VALUE ( REG_SETTINGS_DEFAULT_SHOW_TOUCH_KEYBOARD_BOOLEAN ) ? ( L"1" ) : ( L"0" )

// Gaze dwell-click settings (see SSDwellConfig). Stored as REG_SZ like the rest.
// Numeric defaults live in stdafx.h (DWELL_DEFAULT_*) so the dwell window's
// Reset button uses the same values.
#define REG_SETTINGS_DWELL_MODE_SELECTION_NAME L"Dwell Mode Selection"
#define REG_SETTINGS_DEFAULT_DWELL_MODE_SELECTION DWELL_DEFAULT_MODE_SELECTION   // 0 Auto, 1 Off, 2 Mouse, 3 HID
#define REG_SETTINGS_DWELL_TIME_NAME L"Dwell Time Ms"
#define REG_SETTINGS_DEFAULT_DWELL_TIME DWELL_DEFAULT_TIME_MS
#define REG_SETTINGS_DWELL_TOLERANCE_NAME L"Dwell Tolerance Radius"
#define REG_SETTINGS_DEFAULT_DWELL_TOLERANCE DWELL_DEFAULT_TOLERANCE_PX
#define REG_SETTINGS_DWELL_COOLDOWN_NAME L"Dwell Cooldown Ms"
#define REG_SETTINGS_DEFAULT_DWELL_COOLDOWN DWELL_DEFAULT_COOLDOWN_MS
#define REG_SETTINGS_DWELL_PROGRESS_COLOR_NAME L"Dwell Progress Color"
#define REG_SETTINGS_DEFAULT_DWELL_PROGRESS_COLOR ( (DWORD) GetAccentColor() )
#define REG_SETTINGS_DWELL_DETECTED_MODE_NAME L"Dwell Detected Mode"
#define REG_SETTINGS_DEFAULT_DWELL_DETECTED_MODE DWELL_DEFAULT_DETECTED_MODE   // 0 Off, 1 Mouse, 2 HID, 3 ExternalClick

#define REG_SETTINGS_CATEGORY_WINDOW_SIZE_NAME L"Category Window Size"
#define REG_SETTINGS_VERSION_NAME L"Version"
#define REG_SETTINGS_VERSION_RUNS_NAME L"Version Runs"
#define REG_SETTINGS_TOTAL_RUNS_NAME L"Total Runs"
#define REG_SETTINGS_SELECTED_CATEGORY_NAME L"Selected Category"
#define REG_SETTINGS_ZOOM_FACTOR_NAME L"Zoom Factor"



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
  wchar_t subkeyName[REG_KEY_NAME_BUFFER_SIZE];
  DWORD subkeyNameSize;
  while( true )
  {
    subkeyNameSize = REG_KEY_NAME_BUFFER_SIZE * sizeof( wchar_t );
    result = RegEnumKeyEx( hKey, index, subkeyName, &subkeyNameSize, NULL, NULL, NULL, NULL );
    if( result == ERROR_NO_MORE_ITEMS ) break;
    if( result != ERROR_SUCCESS ) { index++; continue; }
    bool languageExists = false;

    for( size_t i = 0; i < languages.size(); i++ )
    {
      if( languages[i].EnglishName == subkeyName )
      {
        languageExists = true;
        i = languages.size();
      }
    }

    if( !languageExists )
    {
      LanguageInfo languageFound;
      languageFound.EnglishName = subkeyName;
      languageFound.NativeName = subkeyName;
      languageFound.IsRTL = false;
      languageFound.LanguageId = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL );
      languageFound.VoiceTestSampleText = languageFound.NativeName;
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

std::vector<Category> RegistryManager::LoadCategoriesFromRegistry( std::wstring language, std::wstring * outBoardStyle )
{
  std::vector<Category> categories;
  if( outBoardStyle ) outBoardStyle->clear();
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
    valueNameSize = REG_KEY_NAME_BUFFER_SIZE;      // characters, not bytes
    valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof( wchar_t ); // bytes
    memset( valueName, 0, sizeof( valueName ) );
    memset( valueData, 0, sizeof( valueData ) );
    result = RegEnumValue( hKey, index, valueName, &valueNameSize, nullptr, &valueType,
      reinterpret_cast<LPBYTE>( valueData ), &valueDataSize );

    if( result == ERROR_NO_MORE_ITEMS ) break;
    if( result != ERROR_SUCCESS ) { index++; continue; }

    if( valueType == REG_SZ )
    {
      std::wstring categoryName( valueName );
      std::wstring categoryData( valueData );

      // Reserved $$ names: $$board carries the board style, anything else
      // $$-prefixed is dropped defensively (board-style.spec.md STY-F20).
      if( categoryName.compare( 0, STYLE_TOKEN_PREFIX_LENGTH, STYLE_TOKEN_PREFIX ) == 0 )
      {
        if( categoryName == BOARD_STYLE_CATEGORY_NAME && outBoardStyle )
          *outBoardStyle = ExtractBoardStyleFromData( categoryData );
        index++;
        continue;
      }

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

Category RegistryManager::ParseCategoryFromRegistryData( const std::wstring & registryKeyName, const std::wstring & registryData )
{
  Category category = DeserializeCategory( registryKeyName );
  ParseCategoryData( category, registryData ); // fills phrases + extracts $$ style tokens
  return category;
}

bool RegistryManager::SaveCategoriesToRegistry( const std::vector<Category> & categories, std::wstring language, bool clearExisting, const std::wstring & boardStyle )
{
  if( language.empty() )
  {
    language = GetSystemLanguage();
  }
  std::wstring regPath = GetLanguageSpecificPath( language );

  HKEY hKey;
  DWORD disposition;

  if( clearExisting )
  {
    RegDeleteKey( HKEY_CURRENT_USER, regPath.c_str() );
  }

  LONG result = RegCreateKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, NULL,
    REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition );

  if( result != ERROR_SUCCESS )
  {
    return false;
  }

  for( const auto & category : categories )
  {
    if( category.name.compare( 0, STYLE_TOKEN_PREFIX_LENGTH, STYLE_TOKEN_PREFIX ) == 0 )
      continue; // reserved names never persist as categories (board-style.spec.md)

    std::wstring serializedData = SerializeCategoryForRegistry( category );

    result = RegSetValueEx( hKey, SerializeCategory( category ).c_str(), 0, REG_SZ,
      (LPBYTE) serializedData.c_str(),
      DWORD( serializedData.length() + 1 ) * sizeof( wchar_t ) );

    if( result != ERROR_SUCCESS )
    {
      RegCloseKey( hKey );
      return false;
    }
  }

  if( !boardStyle.empty() )
  {
    std::wstring boardData = STYLE_TOKEN_PREFIX + boardStyle;
    result = RegSetValueEx( hKey, BOARD_STYLE_CATEGORY_NAME, 0, REG_SZ,
      (LPBYTE) boardData.c_str(),
      DWORD( boardData.length() + 1 ) * sizeof( wchar_t ) );

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
  return SerializeCategoryData( category ); // style token first, then phrases
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
  m_Settings.stopPreviousPlayback = REG_SETTINGS_DEFAULT_STOP_PREVIOUS_PLAYBACK_BOOLEAN;
  m_Settings.showTouchKeyboard = REG_SETTINGS_DEFAULT_SHOW_TOUCH_KEYBOARD_BOOLEAN;
  m_Settings.dwellModeSelection = REG_SETTINGS_DEFAULT_DWELL_MODE_SELECTION;
  m_Settings.dwellTimeMs = REG_SETTINGS_DEFAULT_DWELL_TIME;
  m_Settings.dwellToleranceRadius = REG_SETTINGS_DEFAULT_DWELL_TOLERANCE;
  m_Settings.dwellCooldownMs = REG_SETTINGS_DEFAULT_DWELL_COOLDOWN;
  m_Settings.dwellProgressColor = REG_SETTINGS_DEFAULT_DWELL_PROGRESS_COLOR;
  m_Settings.dwellDetectedMode = REG_SETTINGS_DEFAULT_DWELL_DETECTED_MODE;


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
    valueNameSize = REG_KEY_NAME_BUFFER_SIZE;      // characters, not bytes
    valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof( wchar_t ); // bytes
    memset( valueName, 0, sizeof( valueName ) );
    memset( valueData, 0, sizeof( valueData ) );
    result = RegEnumValue( hKey, index, valueName, &valueNameSize, nullptr, &valueType,
      reinterpret_cast<LPBYTE>( valueData ), &valueDataSize );

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
        // Corrupt/non-numeric data would make std::stoi throw; on failure keep
        // the default seeded at the top of this function instead of crashing on load.
        try
        {
          m_Settings.volume = CLAMPED_VOICE_VOLUME( ( std::stoi( Data ) ) );
        }
        catch( const std::exception & )
        {
        }
      }
      else if( Name == REG_SETTINGS_VOICE_RATE_NAME )
      {
        try
        {
          m_Settings.rate = CLAMPED_VOICE_RATE( ( std::stoi( Data ) ) );
        }
        catch( const std::exception & )
        {
        }
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
      else if( Name == REG_SETTINGS_STOP_PREVIOUS_PLAYBACK_NAME )
      {
        m_Settings.stopPreviousPlayback = ( Data == L"1" );
      }
      else if( Name == REG_SETTINGS_SHOW_TOUCH_KEYBOARD_NAME )
      {
        m_Settings.showTouchKeyboard = ( Data == L"1" );
      }
      // Dwell-click values. wcstol/wcstoul don't throw on malformed data
      // (return 0), so a corrupt value falls back to a benign number.
      else if( Name == REG_SETTINGS_DWELL_MODE_SELECTION_NAME )
      {
        m_Settings.dwellModeSelection = (int) wcstol( Data.c_str(), nullptr, 10 );
      }
      else if( Name == REG_SETTINGS_DWELL_TIME_NAME )
      {
        m_Settings.dwellTimeMs = (int) wcstol( Data.c_str(), nullptr, 10 );
      }
      else if( Name == REG_SETTINGS_DWELL_TOLERANCE_NAME )
      {
        m_Settings.dwellToleranceRadius = (int) wcstol( Data.c_str(), nullptr, 10 );
      }
      else if( Name == REG_SETTINGS_DWELL_COOLDOWN_NAME )
      {
        m_Settings.dwellCooldownMs = (int) wcstol( Data.c_str(), nullptr, 10 );
      }
      else if( Name == REG_SETTINGS_DWELL_PROGRESS_COLOR_NAME )
      {
        m_Settings.dwellProgressColor = (COLORREF) wcstoul( Data.c_str(), nullptr, 10 );
      }
      else if( Name == REG_SETTINGS_DWELL_DETECTED_MODE_NAME )
      {
        m_Settings.dwellDetectedMode = (int) wcstol( Data.c_str(), nullptr, 10 );
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
      { REG_SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_NAME, REG_SETTINGS_DEFAULT_REDUCE_OTHER_AUDIO_WHEN_PLAYING_VALUE },
      { REG_SETTINGS_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_NAME, REG_SETTINGS_DEFAULT_SPEAK_DIRECTLY_WHEN_CLICKING_PHRASE_VALUE },
      { REG_SETTINGS_STOP_PREVIOUS_PLAYBACK_NAME, REG_SETTINGS_DEFAULT_STOP_PREVIOUS_PLAYBACK_VALUE },
      { REG_SETTINGS_SHOW_TOUCH_KEYBOARD_NAME, REG_SETTINGS_DEFAULT_SHOW_TOUCH_KEYBOARD_VALUE },
      { REG_SETTINGS_DWELL_MODE_SELECTION_NAME, std::to_wstring( REG_SETTINGS_DEFAULT_DWELL_MODE_SELECTION ) },
      { REG_SETTINGS_DWELL_TIME_NAME, std::to_wstring( REG_SETTINGS_DEFAULT_DWELL_TIME ) },
      { REG_SETTINGS_DWELL_TOLERANCE_NAME, std::to_wstring( REG_SETTINGS_DEFAULT_DWELL_TOLERANCE ) },
      { REG_SETTINGS_DWELL_COOLDOWN_NAME, std::to_wstring( REG_SETTINGS_DEFAULT_DWELL_COOLDOWN ) },
      { REG_SETTINGS_DWELL_PROGRESS_COLOR_NAME, std::to_wstring( (DWORD) REG_SETTINGS_DEFAULT_DWELL_PROGRESS_COLOR ) },
      { REG_SETTINGS_DWELL_DETECTED_MODE_NAME, std::to_wstring( REG_SETTINGS_DEFAULT_DWELL_DETECTED_MODE ) }
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
      std::wstring regPath = GetLanguageSpecificPath( langPair.first );

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

  // Stop Previous Playback
  std::wstring stopPreviousPlaybackStr = toSave.stopPreviousPlayback ? L"1" : L"0";
  result = RegSetValueEx( hKey, REG_SETTINGS_STOP_PREVIOUS_PLAYBACK_NAME, 0, REG_SZ,
    (LPBYTE) stopPreviousPlaybackStr.c_str(), DWORD( stopPreviousPlaybackStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Show Touch Keyboard
  std::wstring showTouchKeyboardStr = toSave.showTouchKeyboard ? L"1" : L"0";
  result = RegSetValueEx( hKey, REG_SETTINGS_SHOW_TOUCH_KEYBOARD_NAME, 0, REG_SZ,
    (LPBYTE) showTouchKeyboardStr.c_str(), DWORD( showTouchKeyboardStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  // Dwell-click values (stored as decimal strings, like the rest).
  std::wstring dwellModeStr = std::to_wstring( toSave.dwellModeSelection );
  result = RegSetValueEx( hKey, REG_SETTINGS_DWELL_MODE_SELECTION_NAME, 0, REG_SZ,
    (LPBYTE) dwellModeStr.c_str(), DWORD( dwellModeStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  std::wstring dwellTimeStr = std::to_wstring( toSave.dwellTimeMs );
  result = RegSetValueEx( hKey, REG_SETTINGS_DWELL_TIME_NAME, 0, REG_SZ,
    (LPBYTE) dwellTimeStr.c_str(), DWORD( dwellTimeStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  std::wstring dwellTolStr = std::to_wstring( toSave.dwellToleranceRadius );
  result = RegSetValueEx( hKey, REG_SETTINGS_DWELL_TOLERANCE_NAME, 0, REG_SZ,
    (LPBYTE) dwellTolStr.c_str(), DWORD( dwellTolStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  std::wstring dwellCooldownStr = std::to_wstring( toSave.dwellCooldownMs );
  result = RegSetValueEx( hKey, REG_SETTINGS_DWELL_COOLDOWN_NAME, 0, REG_SZ,
    (LPBYTE) dwellCooldownStr.c_str(), DWORD( dwellCooldownStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;

  if( toSave.dwellProgressColor != REG_SETTINGS_DEFAULT_DWELL_PROGRESS_COLOR )
  {
    std::wstring dwellColorStr = std::to_wstring( (DWORD) toSave.dwellProgressColor );
    result = RegSetValueEx( hKey, REG_SETTINGS_DWELL_PROGRESS_COLOR_NAME, 0, REG_SZ,
      (LPBYTE) dwellColorStr.c_str(), DWORD( dwellColorStr.length() + 1 ) * sizeof( wchar_t ) );
    if( result != ERROR_SUCCESS ) success = false;
  }
  else
  {
    // The color matches the accent-color default: remove any previously stored
    // custom value so the setting keeps following the Windows accent color (a
    // stale stored value would otherwise win on the next load). A missing
    // value is fine, so the return code is intentionally ignored.
    RegDeleteValue( hKey, REG_SETTINGS_DWELL_PROGRESS_COLOR_NAME );
  }

  std::wstring dwellDetectedStr = std::to_wstring( toSave.dwellDetectedMode );
  result = RegSetValueEx( hKey, REG_SETTINGS_DWELL_DETECTED_MODE_NAME, 0, REG_SZ,
    (LPBYTE) dwellDetectedStr.c_str(), DWORD( dwellDetectedStr.length() + 1 ) * sizeof( wchar_t ) );
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
  DWORD valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof( wchar_t );
  DWORD valueType;
  result = RegGetValue( hKey, NULL, REG_SETTINGS_CATEGORY_WINDOW_SIZE_NAME, RRF_RT_REG_SZ, &valueType,
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
    // Corrupt "<w>x<h>" data would make std::stoi throw; on failure fall back to
    // "no remembered size" (return false) instead of crashing on load.
    try
    {
      width = std::stoi( sizeStr.substr( 0, commaPos ) );
      height = std::stoi( sizeStr.substr( commaPos + 1 ) );
    }
    catch( const std::exception & )
    {
      RegCloseKey( hKey );
      return false;
    }
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
  result = RegSetValueEx( hKey, REG_SETTINGS_CATEGORY_WINDOW_SIZE_NAME, 0, REG_SZ,
    (LPBYTE) sizeStr.c_str(), DWORD( sizeStr.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;
  RegCloseKey( hKey );
  return success;
}

bool RegistryManager::SaveSelectedCategoryToRegistry( int category )
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
  std::wstring valueData = std::to_wstring( category );
  result = RegSetValueEx( hKey, REG_SETTINGS_SELECTED_CATEGORY_NAME, 0, REG_SZ,
    (LPBYTE) valueData.c_str(), DWORD( valueData.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;
  RegCloseKey( hKey );
  return success;
}

bool RegistryManager::SaveRunInfoToRegistry( std::wstring version )
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
  std::wstring valueData = std::to_wstring( 1 + GetApplicationRunCount() );
  result = RegSetValueEx( hKey, REG_SETTINGS_TOTAL_RUNS_NAME, 0, REG_SZ,
    (LPBYTE) valueData.c_str(), DWORD( valueData.length() + 1 ) * sizeof( wchar_t ) );
  if( result != ERROR_SUCCESS ) success = false;
  if( !version.empty() && version != GetLastRunVersionFromRegistry() ) // Only update version if it's different from the one already in registry, otherwise keep the old version (this way we keep the version of the first run in case of multiple runs with different versions
  {
    valueData = L"1";
    result = RegSetValueEx( hKey, REG_SETTINGS_VERSION_RUNS_NAME, 0, REG_SZ,
      (LPBYTE) valueData.c_str(), DWORD( valueData.length() + 1 ) * sizeof( wchar_t ) );
    if( result != ERROR_SUCCESS ) success = false;
    result = RegSetValueEx( hKey, REG_SETTINGS_VERSION_NAME, 0, REG_SZ,
      (LPBYTE) version.c_str(), DWORD( version.length() + 1 ) * sizeof( wchar_t ) );
    if( result != ERROR_SUCCESS ) success = false;
  }
  else
  {
    valueData = std::to_wstring( 1 + GetVersionRunCount() );
    result = RegSetValueEx( hKey, REG_SETTINGS_VERSION_RUNS_NAME, 0, REG_SZ,
      (LPBYTE) valueData.c_str(), DWORD( valueData.length() + 1 ) * sizeof( wchar_t ) );
  }
  RegCloseKey( hKey );
  return success;
}

int RegistryManager::GetVersionRunCount()
{
  int count = 0;
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );
  if( result == ERROR_SUCCESS )
  {
    wchar_t valueData[REG_KEY_DATA_BUFFER_SIZE];
    DWORD valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof( wchar_t );
    DWORD valueType;
    result = RegGetValue( hKey, NULL, REG_SETTINGS_VERSION_RUNS_NAME, RRF_RT_REG_SZ, &valueType,
      (LPBYTE) valueData, &valueDataSize );
    if( result == ERROR_SUCCESS )
    {
      // Keep the default (0) if the stored counter is corrupt/non-numeric.
      try
      {
        count = std::stoi( valueData );
      }
      catch( const std::exception & )
      {
      }
    }
    RegCloseKey( hKey );
  }
  return count;
}

int RegistryManager::GetApplicationRunCount()
{
  int count = 0;
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );
  if( result == ERROR_SUCCESS )
  {
    wchar_t valueData[REG_KEY_DATA_BUFFER_SIZE];
    DWORD valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof( wchar_t );
    DWORD valueType;
    result = RegGetValue( hKey, NULL, REG_SETTINGS_TOTAL_RUNS_NAME, RRF_RT_REG_SZ, &valueType,
      (LPBYTE) valueData, &valueDataSize );
    if( result == ERROR_SUCCESS )
    {
      // Keep the default (0) if the stored counter is corrupt/non-numeric.
      try
      {
        count = std::stoi( valueData );
      }
      catch( const std::exception & )
      {
      }
    }
    RegCloseKey( hKey );
  }
  return count;
}

std::wstring RegistryManager::GetLastRunVersionFromRegistry()
{
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );
  if( result != ERROR_SUCCESS )
  {
    return L"";
  }
  wchar_t valueData[REG_KEY_DATA_BUFFER_SIZE];
  DWORD valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof( wchar_t );
  DWORD valueType;
  result = RegGetValue( hKey, NULL, REG_SETTINGS_VERSION_NAME, RRF_RT_REG_SZ, &valueType,
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

int RegistryManager::GetSelectedCategoryFromRegistry()
{
  int category = -1;
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );
  if( result == ERROR_SUCCESS )
  {
    wchar_t valueData[REG_KEY_DATA_BUFFER_SIZE];
    DWORD valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof( wchar_t );
    DWORD valueType;
    result = RegGetValue( hKey, NULL, REG_SETTINGS_SELECTED_CATEGORY_NAME, RRF_RT_REG_SZ, &valueType,
      (LPBYTE) valueData, &valueDataSize );
    if( result == ERROR_SUCCESS )
    {
      // Keep the default (-1 = unset) if the stored index is corrupt/non-numeric.
      try
      {
        category = std::stoi( valueData );
      }
      catch( const std::exception & )
      {
      }
    }
    RegCloseKey( hKey );
  }
  return category;
}

bool RegistryManager::SaveZoomFactorToRegistry( float zoomFactor )
{
  bool success = true;
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_WRITE, &hKey );
  if( result == ERROR_SUCCESS )
  {
    std::wstring valueData = std::to_wstring( zoomFactor );
    result = RegSetValueEx( hKey, REG_SETTINGS_ZOOM_FACTOR_NAME, 0, REG_SZ,
      (LPBYTE) valueData.c_str(), DWORD( valueData.length() + 1 ) * sizeof( wchar_t ) );
    if( result != ERROR_SUCCESS ) success = false;
  }
  RegCloseKey( hKey );
  return success;
}

float RegistryManager::GetZoomFactorFromRegistry()
{
  float zoomFactor = 1.0f;
  std::wstring regPath = GetLastRunRegistryPath();
  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );
  if( result == ERROR_SUCCESS )
  {
    wchar_t valueData[REG_KEY_DATA_BUFFER_SIZE];
    DWORD valueDataSize = REG_KEY_DATA_BUFFER_SIZE * sizeof( wchar_t );
    DWORD valueType;
    result = RegGetValue( hKey, NULL, REG_SETTINGS_ZOOM_FACTOR_NAME, RRF_RT_REG_SZ, &valueType,
      (LPBYTE) valueData, &valueDataSize );
    if( result == ERROR_SUCCESS )
    {
      // Keep the default (1.0) if the stored zoom is corrupt/non-numeric.
      try
      {
        zoomFactor = std::stof( valueData );
      }
      catch( const std::exception & )
      {
      }
    }
    RegCloseKey( hKey );
  }
  return zoomFactor;
}