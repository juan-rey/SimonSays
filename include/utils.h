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
// SerializeCategory + the "::<style>" suffix for the edit dialog (STY-F31).
// SerializeCategory itself stays style-free: it is used as the registry value
// name, and styles must never land there (STY-F32).
std::wstring SerializeCategoryWithStyle( const Category & category );
Category DeserializeCategory( const std::wstring & data );
std::wstring SerializePhrase( const Phrase & phrase );
Phrase DeserializePhrase( const std::wstring & data );
// Category phrase-position data ("$$<style>|phrase|phrase..."): the style
// token, when present, is emitted first on save and extracted into
// Category::style on parse (board-style.spec.md §8.3).
std::wstring SerializeCategoryData( const Category & category );
void ParseCategoryData( Category & category, const std::wstring & data );
// Data of the reserved $$board value/line: style-list tokens joined by the
// phrase separator, each with an optional leading "$$" that is stripped.
std::wstring ExtractBoardStyleFromData( const std::wstring & data );
std::wstring PhraseToButtonText( const Phrase & phrase );
std::wstring GetISODateString();
std::wstring GetUserNameString();
bool ExportCategoriesToFile( const std::vector<Category> & categories, const std::wstring & filePath, const std::wstring & boardStyle = L"" );
bool ImportCategoriesFromFile( const std::wstring & filePath, std::vector<Category> & outCategories, std::wstring * outBoardStyle = nullptr );
// .ssz (Zip bundle) variants. The bundle contains the categories.ssc plus the
// referenced icon/audio resources; on import the resources are extracted into
// resourceFolder (typically %LocalAppData%\SimonSays).
// appDataOnly (default): only bundle resources found in resourceFolder
// (%LocalAppData%\SimonSays). Set false to also pick up resources from the
// working and executable directories.
// Board resource subfolder (title-derived): sanitizes a board title into a
// folder name (spaces -> '_', path/reserved characters stripped, ".."/device
// names rejected, trailing dots trimmed, length capped). Empty result = no
// board folder (resources live in the app-data root as before).
std::wstring SanitizeBoardFolderName( const std::wstring & title );
// %LocalAppData%\SimonSays\<sanitized title> for the given raw board style
// list, or L"" when the style has no usable title.
std::wstring GetBoardResourceFolder( const std::wstring & boardStyle );
// Moves the files of oldFolder into newFolder without overwriting existing
// ones (board rename, merge policy); removes oldFolder once emptied. Files
// that cannot be moved stay behind; never destructive. Returns true when the
// old folder was fully merged away.
bool MergeMoveFolder( const std::wstring & oldFolder, const std::wstring & newFolder );

// Resources extracted from a .ssz to a temp dir, awaiting a target folder —
// lets the caller decide the destination (board subfolder vs app-data root)
// after the board-style adoption prompt, keeping the two-phase commit.
struct PendingSszResources
{
  std::wstring tempDir;
  std::vector<std::wstring> leafNames;
};
bool CommitPendingSszResources( PendingSszResources & pending, const std::wstring & targetFolder ); // copies + cleans temp
void DiscardPendingSszResources( PendingSszResources & pending );                                   // cleans temp only

// boardResourceFolder (optional): the active board's resource subfolder; when
// non-empty it is searched FIRST — before the app-data root — so board-scoped
// resources are found, bundled, and reconciled like the app resolves them.
bool ExportCategoriesToSsz( const std::vector<Category> & categories, const std::wstring & filePath, const std::wstring & resourceFolder, bool appDataOnly = true, const std::wstring & boardStyle = L"", const std::wstring & boardResourceFolder = L"" );
// When outPending is given, extracted resources are handed back for a caller-
// side CommitPendingSszResources instead of being installed into resourceFolder.
bool ImportCategoriesFromSsz( const std::wstring & filePath, const std::wstring & resourceFolder, std::vector<Category> & outCategories, std::wstring & errorDetail, std::wstring * outBoardStyle = nullptr, const std::wstring & boardResourceFolder = L"", PendingSszResources * outPending = nullptr );
// True if any category/phrase references an icon (.ico) or audio (.wav/.mp3)
// file that actually exists on disk — used to auto-pick .ssz over plain .ssc.
// appDataOnly mirrors ExportCategoriesToSsz so the auto-format choice matches
// what will actually be bundled.
bool CategoriesHaveBundledResources( const std::vector<Category> & categories, const std::wstring & resourceFolder, bool appDataOnly = true, const std::wstring & boardResourceFolder = L"" );
// Sniffs the first bytes for the local-file-header Zip magic ("PK\x03\x04").
bool IsZipArchive( const std::wstring & filePath );
bool StringEndsWithCI( const std::wstring & s, const std::wstring & suffix );
// File-based icon formats accepted by the ## icon prefix and rendered by
// SSButton (.ico natively; .png/.jpg/.jpeg via WIC). Shared by the
// CategoryWindow icon routing and the import/export resource filter.
bool HasSupportedIconExt( const std::wstring & name );
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