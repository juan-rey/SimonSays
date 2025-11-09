#include "RegistryManager.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

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
  return L"SOFTWARE\\SimonSays\\Phrases";
}

std::wstring RegistryManager::GetLanguageSpecificPath( const std::wstring & language )
{
  return GetRegistryPath() + L"\\" + language;
}

std::vector<Category> RegistryManager::LoadCategoriesFromRegistry()
{
  std::vector<Category> categories;
  std::wstring language = GetSystemLanguage();
  std::wstring regPath = GetLanguageSpecificPath( language );

  HKEY hKey;
  LONG result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );

  if( result != ERROR_SUCCESS )
  {
    regPath = GetLanguageSpecificPath( L"English" );
    result = RegOpenKeyEx( HKEY_CURRENT_USER, regPath.c_str(), 0, KEY_READ, &hKey );

    if( result != ERROR_SUCCESS )
    {
      Category defaultCategory;
      defaultCategory.name = L"Common Phrases";

      Phrase phrase1;
      phrase1.text = L"Hello";
      phrase1.audioFile = L"hello.wav";

      Phrase phrase2;
      phrase2.text = L"Thank you";
      phrase2.audioFile = L"thankyou.wav";

      Phrase phrase3;
      phrase3.text = L"Goodbye";
      phrase3.audioFile = L"goodbye.wav";

      defaultCategory.phrases.push_back( phrase1 );
      defaultCategory.phrases.push_back( phrase2 );
      defaultCategory.phrases.push_back( phrase3 );

      categories.push_back( defaultCategory );
      return categories;
    }
  }

  DWORD index = 0;
  wchar_t valueName[256];
  wchar_t valueData[1024];
  DWORD valueNameSize, valueDataSize, valueType;

  while( true )
  {
    valueNameSize = 256;
    valueDataSize = 1024;

    result = RegEnumValue( hKey, index, valueName, &valueNameSize, NULL, &valueType,
      (LPBYTE) valueData, &valueDataSize );

    if( result == ERROR_NO_MORE_ITEMS ) break;
    if( result != ERROR_SUCCESS ) continue;

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

  if( categories.empty() )
  {
    Category defaultCategory;
    defaultCategory.name = L"Common Phrases";

    Phrase phrase1;
    phrase1.text = L"Hello";
    phrase1.audioFile = L"hello.wav";

    Phrase phrase2;
    phrase2.text = L"Thank you";
    phrase2.audioFile = L"thankyou.wav";

    Phrase phrase3;
    phrase3.text = L"Goodbye";
    phrase3.audioFile = L"goodbye.wav";

    defaultCategory.phrases.push_back( phrase1 );
    defaultCategory.phrases.push_back( phrase2 );
    defaultCategory.phrases.push_back( phrase3 );

    categories.push_back( defaultCategory );
  }

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

bool RegistryManager::SaveCategoriesToRegistry( const std::vector<Category> & categories )
{
  std::wstring language = GetSystemLanguage();
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
    result += phrase.text + L"::" + phrase.audioFile;
  }

  return result;
}