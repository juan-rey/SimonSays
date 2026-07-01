/*
   utils.h: utils header file.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"
#include <windows.h>
#include <string>

#define CEILING_DIV( x, y )  ( ( ( x ) / ( y ) ) + ( ( ( x ) % ( y ) ) ? 1 : 0 ) )

std::wstring ReplaceAll( std::wstring str, const std::wstring & from, const std::wstring & to );
void trim( std::wstring & s );
std::wstring SerializeCategory( const Category & category );
Category DeserializeCategory( const std::wstring & data );
std::wstring SerializePhrase( const Phrase & phrase );
Phrase DeserializePhrase( const std::wstring & data );
std::wstring PhraseToButtonText( const Phrase & phrase );
std::wstring GetISODateString();
std::wstring GetUserNameString();
bool ExportCategoriesToFile( const std::vector<Category> & categories, const std::wstring & filePath );
bool ImportCategoriesFromFile( const std::wstring & filePath, std::vector<Category> & outCategories );
// .ssz (Zip bundle) variants. The bundle contains the categories.ssc plus the
// referenced icon/audio resources; on import the resources are extracted into
// resourceFolder (typically %LocalAppData%\SimonSays).
// appDataOnly (default): only bundle resources found in resourceFolder
// (%LocalAppData%\SimonSays). Set false to also pick up resources from the
// working and executable directories.
bool ExportCategoriesToSsz( const std::vector<Category> & categories, const std::wstring & filePath, const std::wstring & resourceFolder, bool appDataOnly = true );
bool ImportCategoriesFromSsz( const std::wstring & filePath, const std::wstring & resourceFolder, std::vector<Category> & outCategories, std::wstring & errorDetail );
// True if any category/phrase references an icon (.ico) or audio (.wav/.mp3)
// file that actually exists on disk — used to auto-pick .ssz over plain .ssc.
// appDataOnly mirrors ExportCategoriesToSsz so the auto-format choice matches
// what will actually be bundled.
bool CategoriesHaveBundledResources( const std::vector<Category> & categories, const std::wstring & resourceFolder, bool appDataOnly = true );
// Sniffs the first bytes for the local-file-header Zip magic ("PK\x03\x04").
bool IsZipArchive( const std::wstring & filePath );
bool StringEndsWithCI( const std::wstring & s, const std::wstring & suffix );
std::wstring PromptExportCategoriesFilePath( HWND owner, const std::wstring & language, const std::wstring & suggestedFileName = L"", const std::wstring & defaultExt = L"ssc" );
std::wstring PromptImportCategoriesFilePath( HWND owner, const std::wstring & language );
std::wstring GetSystemLanguage();
const wchar_t * GetLocalizedString( int stringId, std::wstring language );
std::wstring ReplaceAmpersandLocalized( const std::wstring & str, const std::wstring & language );
bool IsLanguageRTL( const std::wstring & language );
std::wstring GetProductVersionString();
std::wstring GetAppDataCustomFolder( const std::wstring & appName );
std::wstring GetExecutableDirectory();
std::wstring GetWorkingDirectory();
bool FileExists( const std::wstring & path );
std::wstring GetLanguageStringFromLangId( LANGID langId );
std::wstring GetLanguageNativeName( std::wstring language );
LANGID GetLangIdFromLanguageString( std::wstring language );
UINT GetMessageBoxFlagsForLanguage( const std::wstring & language, UINT baseFlags );
int ShowLocalizedMessageBox( HWND hwnd, const wchar_t * text, const wchar_t * caption, UINT baseFlags, const std::wstring & language );
COLORREF GetTaskbarColor();
COLORREF GetAccentColor();
SIZE GetTextDimensions( HWND hwnd, const wchar_t * text );
void CenterEditTextVertically( HWND hEdit );
void ConfigureSlider( HWND hDlg, int sliderId, int minValue, int maxValue, int initialValue, int tickMarks = 10 );
void SyncSliderToEdit( HWND hDlg, int sliderId, int editId, BOOL isSigned );
void SyncEditToSlider( HWND hDlg, int editId, int sliderId, BOOL isSigned, int minValue = 0, int maxValue = 0 );
bool IsTaskbarAtBottom();
int GetStartButtonXPosition();
int GetSystemTrayXPosition();
void updateRtlExStyle( HWND hCtrl, bool isRtl, LONG_PTR baseExStyle = 0 );
void updateEditAlignment( HWND hEdit, bool isRtl );
BOOL CALLBACK ApplyRtlStylesCallback( HWND hwnd, LPARAM lParam );
void ShowTouchKeyboard( HWND hwndNear = NULL, SIZE relativePlacement = { 0, 0 }, int margin = 0 );

#endif // UTILS_H