#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"
#include <windows.h>
#include <string>

#define CEILING_DIV( x, y )  ( ( ( x ) / ( y ) ) + ( ( ( x ) % ( y ) ) ? 1 : 0 )

std::wstring ReplaceAll( std::wstring str, const std::wstring & from, const std::wstring & to );
void trim( std::wstring & s );
std::wstring SerializePhrase( const Phrase & phrase );
Phrase DeserializePhrase( const std::wstring & data );
std::wstring PhraseToButtonText( const Phrase & phrase );
bool ExportCategoriesToFile( const std::vector<Category> & categories, const std::wstring & filePath );
bool ImportCategoriesFromFile( const std::wstring & filePath, std::vector<Category> & outCategories );
std::wstring PromptExportCategoriesFilePath( HWND owner = NULL );
std::wstring PromptImportCategoriesFilePath( HWND owner = NULL );
std::wstring GetSystemLanguage();
const wchar_t * GetLocalizedString( int stringId, std::wstring language );
bool IsLanguageRTL( const std::wstring & language );
std::wstring GetProductVersionString();
std::wstring GetLanguageStringFromLangId( LANGID langId );
LANGID GetLangIdFromLanguageString( const std::wstring & language );
UINT GetMessageBoxFlagsForLanguage( const std::wstring & language, UINT baseFlags );
int ShowLocalizedMessageBox( HWND hwnd, const wchar_t * text, const wchar_t * caption, UINT baseFlags, const std::wstring & language );
COLORREF GetTaskbarColor();
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

#endif // UTILS_H