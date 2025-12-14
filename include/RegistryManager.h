#pragma once


#ifndef RegistryManager_h
#define RegistryManager_h

#include "stdafx.h"

class RegistryManager
{
public:
  static std::wstring GetSystemLanguage();
  static std::wstring GetLanguageStringFromLangId( LANGID langId );
  static std::vector<Category> LoadCategoriesFromRegistry( std::wstring language = L"" );
  static std::vector<VoiceInfo> PopulateAvaibleVoicesFromRegistry( std::wstring languageFilter = L"" );
  static Settings LoadSettingsFromRegistry();
  static bool SaveSettingsToRegistry( const Settings & s );
  static bool SaveCategoriesToRegistry( const std::vector<Category> & categories, std::wstring language = L"" );
  static bool InstallDefaultSettings();
  static bool InstallDefaultPhrases();

private:
  static Settings m_Settings;
  static std::wstring GetRegistryPath();
  static std::wstring GetSettingsRegistryPath();
  static std::wstring GetPhrasesRegistryPath();
  static std::wstring GetLanguageSpecificPath( const std::wstring & language );
  static Category ParseCategoryFromRegistryData( const std::wstring & categoryName, const std::wstring & data );
  static std::wstring SerializeCategoryForRegistry( const Category & category );
};

#endif // RegistryManager_h