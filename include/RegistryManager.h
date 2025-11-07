#pragma once


#ifndef RegistryManager_h
#define RegistryManager_h

#include "stdafx.h"

class RegistryManager
{
public:
  static std::wstring GetSystemLanguage();
  static std::vector<Category> LoadCategoriesFromRegistry();
  static bool SaveCategoriesToRegistry( const std::vector<Category> & categories );
  static std::wstring GetRegistryPath();

private:
  static std::wstring GetLanguageSpecificPath( const std::wstring & language );
  static Category ParseCategoryFromRegistryData( const std::wstring & categoryName, const std::wstring & data );
  static std::wstring SerializeCategoryForRegistry( const Category & category );
};

#endif // RegistryManager_h