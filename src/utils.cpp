/*
   utils.cpp: utils source file.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "utils.h"
#include "localized_strings.h"
#include "stdafx.h"
#include <commctrl.h>
#include <vector>
#include <windows.h>
#include <shellapi.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <fstream>
#include <sstream>
#include <map>
#include <cstring>
#include <commdlg.h>
#include <shlobj.h> // Required for SHGetKnownFolderPath
#include "miniz.h"  // Single-file Zip library used for the .ssz bundle format
#include "BoardStyle.h" // board title -> resource subfolder derivation

// Plain UTF-8 <-> wide conversion via WinAPI. Replaces std::codecvt_utf8 which
// is deprecated since C++17 and slated for removal.
static std::string Utf8FromWide( const std::wstring & s )
{
  if( s.empty() ) return {};
  int needed = WideCharToMultiByte( CP_UTF8, 0, s.data(), (int) s.size(), nullptr, 0, nullptr, nullptr );
  std::string out( (size_t) needed, '\0' );
  WideCharToMultiByte( CP_UTF8, 0, s.data(), (int) s.size(), &out[0], needed, nullptr, nullptr );
  return out;
}

static std::wstring WideFromUtf8( const std::string & s )
{
  if( s.empty() ) return {};
  int needed = MultiByteToWideChar( CP_UTF8, 0, s.data(), (int) s.size(), nullptr, 0 );
  std::wstring out( (size_t) needed, L'\0' );
  MultiByteToWideChar( CP_UTF8, 0, s.data(), (int) s.size(), &out[0], needed );
  return out;
}

std::wstring ReplaceAll( std::wstring str, const std::wstring & from, const std::wstring & to )
{
  size_t start_pos = 0;
  while( ( start_pos = str.find( from, start_pos ) ) != std::wstring::npos )
  {
    str.replace( start_pos, from.length(), to );
    start_pos += to.length();
  }
  return str;
}

void trim( std::wstring & s )
{
  // trim whitespace on both ends
  while( !s.empty() && iswspace( s.front() ) ) s.erase( 0, 1 );
  while( !s.empty() && iswspace( s.back() ) ) s.pop_back();
}


std::wstring SerializeCategory( const Category & category )
{
  return category.icon.empty() ? category.name : category.icon + ICON_SEPARATOR + category.name;
}

std::wstring SerializeCategoryWithStyle( const Category & category )
{
  return SerializeCategory( category ) + ( category.style.empty() ? L"" : AUDIO_FILE_SEPARATOR + category.style );
}

Category DeserializeCategory( const std::wstring & data )
{
  Category category;
  std::wstring rest = data;

  size_t iconSeparatorPos = rest.find( ICON_SEPARATOR );
  if( iconSeparatorPos != std::wstring::npos )
  {
    category.icon = rest.substr( 0, iconSeparatorPos );
    rest = rest.substr( iconSeparatorPos + ICON_SEPARATOR_LENGTH );
  }

  // Optional "::<style>" suffix (STY-F31). Stored value names never carry it,
  // so this only splits edit-dialog input (and any legacy name containing "::",
  // a documented ambiguity — see board-style.spec.md §14).
  size_t styleSeparatorPos = rest.find( AUDIO_FILE_SEPARATOR );
  if( styleSeparatorPos != std::wstring::npos )
  {
    category.style = rest.substr( styleSeparatorPos + AUDIO_FILE_SEPARATOR_LENGTH );
    rest = rest.substr( 0, styleSeparatorPos );
  }

  category.name = rest;
  return category;
}

// Appends a style list to another, making sure the joint doesn't glue two
// pairs together when the first list lacks its trailing ';'.
static void AppendStyleList( std::wstring & style, const std::wstring & addition )
{
  if( addition.empty() ) return;
  if( !style.empty() && style.back() != L';' ) style += L';';
  style += addition;
}

std::wstring SerializeCategoryData( const Category & category )
{
  std::wstring result;

  if( !category.style.empty() )
    result = STYLE_TOKEN_PREFIX + category.style;

  for( const auto & phrase : category.phrases )
  {
    if( !result.empty() )
    {
      result += CATEGORY_PHRASE_SEPARATOR;
    }
    result += SerializePhrase( phrase );
  }

  return result;
}

void ParseCategoryData( Category & category, const std::wstring & data )
{
  std::wistringstream stream( data );
  std::wstring token;

  while( std::getline( stream, token, CATEGORY_PHRASE_SEPARATOR[0] ) )
  {
    if( token.empty() ) continue;

    if( token.compare( 0, STYLE_TOKEN_PREFIX_LENGTH, STYLE_TOKEN_PREFIX ) == 0 )
      AppendStyleList( category.style, token.substr( STYLE_TOKEN_PREFIX_LENGTH ) ); // style token, not a phrase (STY-F32)
    else
      category.phrases.push_back( DeserializePhrase( token ) );
  }
}

std::wstring ExtractBoardStyleFromData( const std::wstring & data )
{
  std::wstring boardStyle;
  std::wistringstream stream( data );
  std::wstring token;

  while( std::getline( stream, token, CATEGORY_PHRASE_SEPARATOR[0] ) )
  {
    if( token.empty() ) continue;
    if( token.compare( 0, STYLE_TOKEN_PREFIX_LENGTH, STYLE_TOKEN_PREFIX ) == 0 )
      token = token.substr( STYLE_TOKEN_PREFIX_LENGTH ); // leading "$$" accepted and stripped
    AppendStyleList( boardStyle, token );
  }

  return boardStyle;
}

std::wstring SerializePhrase( const Phrase & phrase )
{
  return ( phrase.icon.empty() ? phrase.text : phrase.icon + ICON_SEPARATOR + phrase.text ) + ( phrase.audioFile.empty() ? L"" : AUDIO_FILE_SEPARATOR + phrase.audioFile );
}

Phrase DeserializePhrase( const std::wstring & data )
{
  Phrase phrase;

  size_t iconSeparatorPos = data.find( ICON_SEPARATOR );
  size_t textStartPos = 0;
  if( iconSeparatorPos != std::wstring::npos )
  {
    phrase.icon = data.substr( 0, iconSeparatorPos );
    textStartPos = iconSeparatorPos + ICON_SEPARATOR_LENGTH;
  }

  size_t audioSeparatorPos = data.find( AUDIO_FILE_SEPARATOR );
  if( audioSeparatorPos != std::wstring::npos )
  {
    phrase.text = data.substr( textStartPos, audioSeparatorPos - textStartPos );
    phrase.audioFile = data.substr( audioSeparatorPos + AUDIO_FILE_SEPARATOR_LENGTH );
  }
  else
  {
    phrase.text = data.substr( textStartPos );
  }

  return phrase;
}

std::wstring PhraseToButtonText( const Phrase & phrase )
{
  if( phrase.audioFile.empty() )
  {
    return phrase.text;
  }
  else
  {
    return ( SOUND_NOTE_DELIMITER + phrase.text + SOUND_NOTE_DELIMITER );
  }
}

std::wstring GetISODateString()
{
  SYSTEMTIME st;
  GetLocalTime( &st );
  wchar_t buffer[100];
  swprintf_s( buffer, L"%04d%02d%02d", st.wYear, st.wMonth, st.wDay );
  return std::wstring( buffer );
}

std::wstring GetUserNameString()
{
  wchar_t buffer[256];
  DWORD size = sizeof( buffer ) / sizeof( wchar_t );
  if( GetUserName( buffer, &size ) )
  {
    return std::wstring( buffer );
  }
  return L"";
}

// ---------------------------------------------------------------------------
// Shared (in-memory) serialization for both the plain .ssc and the .ssz bundle.
// ---------------------------------------------------------------------------

// Serializes categories to the V1 .ssc byte stream (UTF-8 BOM + header + lines).
// A non-empty boardStyle is emitted as a "$$board=$$<style>" line right after
// the header (board-style.spec.md §8.3).
static std::string SerializeCategoriesToUtf8( const std::vector<Category> & categories, const std::wstring & boardStyle )
{
  std::string out;
  out += "\xEF\xBB\xBF";              // UTF-8 BOM so external tools auto-detect encoding
  out += "SIMONSAYS_CATEGORIES_V1\n";

  if( !boardStyle.empty() )
  {
    out += Utf8FromWide( std::wstring( BOARD_STYLE_CATEGORY_NAME ) );
    out += '=';
    out += Utf8FromWide( STYLE_TOKEN_PREFIX + boardStyle );
    out += '\n';
  }

  for( const auto & category : categories )
  {
    out += Utf8FromWide( SerializeCategory( category ) );
    out += '=';
    out += Utf8FromWide( SerializeCategoryData( category ) );
    out += '\n';
  }
  return out;
}

// Parses the V1 .ssc byte stream. Returns false if the header is missing/wrong.
// The reserved $$board line is extracted into outBoardStyle (when provided) and
// never lands in outCategories; other "$$"-named entries are dropped
// defensively (STY-F20/F23).
static bool ParseCategoriesFromUtf8( const std::string & content, std::vector<Category> & outCategories, std::wstring * outBoardStyle )
{
  outCategories.clear();
  if( outBoardStyle ) outBoardStyle->clear();
  std::istringstream file( content );
  std::string line;
  if( !std::getline( file, line ) ) return false;

  // Strip optional UTF-8 BOM from the first line.
  if( line.size() >= 3 &&
    (unsigned char) line[0] == 0xEF &&
    (unsigned char) line[1] == 0xBB &&
    (unsigned char) line[2] == 0xBF )
    line.erase( 0, 3 );
  if( !line.empty() && line.back() == '\r' ) line.pop_back();

  if( line != "SIMONSAYS_CATEGORIES_V1" ) return false;

  while( std::getline( file, line ) )
  {
    if( !line.empty() && line.back() == '\r' ) line.pop_back();
    if( line.empty() ) continue;

    size_t sepPos = line.find( '=' );
    if( sepPos == std::string::npos ) continue;

    Category cat = DeserializeCategory( WideFromUtf8( line.substr( 0, sepPos ) ) );
    std::wstring data = WideFromUtf8( line.substr( sepPos + 1 ) );

    if( cat.name.compare( 0, STYLE_TOKEN_PREFIX_LENGTH, STYLE_TOKEN_PREFIX ) == 0 )
    {
      if( cat.name == BOARD_STYLE_CATEGORY_NAME && outBoardStyle )
        *outBoardStyle = ExtractBoardStyleFromData( data );
      continue; // reserved name: never a real category
    }

    ParseCategoryData( cat, data );
    outCategories.push_back( cat );
  }

  return true;
}

bool ExportCategoriesToFile( const std::vector<Category> & categories, const std::wstring & filePath, const std::wstring & boardStyle )
{
  const std::string data = SerializeCategoriesToUtf8( categories, boardStyle );
  std::ofstream file( filePath, std::ios::binary );
  if( !file ) return false;
  file.write( data.data(), (std::streamsize) data.size() );
  return file.good();
}

bool ImportCategoriesFromFile( const std::wstring & filePath, std::vector<Category> & outCategories, std::wstring * outBoardStyle )
{
  std::ifstream file( filePath, std::ios::binary );
  if( !file ) return false;
  std::stringstream buffer;
  buffer << file.rdbuf();
  return ParseCategoriesFromUtf8( buffer.str(), outCategories, outBoardStyle );
}

// ---------------------------------------------------------------------------
// .ssz (Zip bundle) helpers
// ---------------------------------------------------------------------------

bool StringEndsWithCI( const std::wstring & s, const std::wstring & suffix )
{
  if( s.size() < suffix.size() ) return false;
  return _wcsicmp( s.c_str() + ( s.size() - suffix.size() ), suffix.c_str() ) == 0;
}

static std::wstring ToLowerW( std::wstring s )
{
  for( auto & c : s ) c = (wchar_t) towlower( c );
  return s;
}

static std::wstring BaseNameW( const std::wstring & path )
{
  size_t p = path.find_last_of( L"\\/" );
  return ( p == std::wstring::npos ) ? path : path.substr( p + 1 );
}

// File-based icon formats (see utils.h). Kept in sync with the SSButton
// StandardIcon loader: .ico → HICON; .png/.jpg/.jpeg → WIC decode.
bool HasSupportedIconExt( const std::wstring & name )
{
  return StringEndsWithCI( name, L".ico" ) ||
    StringEndsWithCI( name, L".png" ) ||
    StringEndsWithCI( name, L".jpg" ) ||
    StringEndsWithCI( name, L".jpeg" );
}

// Only these resource kinds are ever bundled or accepted on import.
static bool HasAllowedResourceExt( const std::wstring & name )
{
  return HasSupportedIconExt( name ) ||
    StringEndsWithCI( name, L".wav" ) ||
    StringEndsWithCI( name, L".mp3" );
}

static bool ReadFileBytes( const std::wstring & path, std::string & out, unsigned long long maxBytes )
{
  std::ifstream f( path, std::ios::binary | std::ios::ate );
  if( !f ) return false;
  std::streamoff size = f.tellg();
  if( size < 0 ) return false;
  if( ( unsigned long long ) size > maxBytes ) return false;
  out.resize( (size_t) size );
  if( size > 0 )
  {
    f.seekg( 0, std::ios::beg );
    f.read( &out[0], size );
    if( f.gcount() != size ) return false;
  }
  return true;
}

static bool WriteFileBytes( const std::wstring & path, const void * data, size_t size )
{
  std::ofstream f( path, std::ios::binary | std::ios::trunc );
  if( !f ) return false;
  if( size > 0 ) f.write( (const char *) data, (std::streamsize) size );
  return f.good();
}

// Folders to search for bundled resources. The active board's resource
// subfolder (when any) always comes first, then the app-data root; when
// appDataOnly is false the lookup is widened to the working and executable
// directories (mirroring CategoryWindow's icon lookup) to also carry
// resources shipped alongside the app.
static std::vector<std::wstring> BuildResourceSearchFolders( const std::wstring & resourceFolder, bool appDataOnly, const std::wstring & boardResourceFolder = L"" )
{
  std::vector<std::wstring> folders;
  if( !boardResourceFolder.empty() ) folders.push_back( boardResourceFolder );
  if( !resourceFolder.empty() && resourceFolder != boardResourceFolder ) folders.push_back( resourceFolder );
  if( appDataOnly ) return folders;
  std::wstring wd = GetWorkingDirectory();
  if( !wd.empty() && wd != resourceFolder ) folders.push_back( wd );
  std::wstring ed = GetExecutableDirectory();
  if( !ed.empty() && ed != resourceFolder && ed != wd ) folders.push_back( ed );
  return folders;
}

std::wstring SanitizeBoardFolderName( const std::wstring & title )
{
  std::wstring name;
  name.reserve( title.size() );
  for( wchar_t c : title )
  {
    if( c < 0x20 ) continue;                             // control characters
    if( wcschr( L"\\/:*?\"<>|", c ) ) continue;          // path separators / reserved chars
    name += ( c == L' ' ) ? L'_' : c;
  }
  // Leading dots could form "."/".."; trailing dots are invalid in Win32 folder
  // names (spaces were already mapped to '_' above).
  while( !name.empty() && name.front() == L'.' ) name.erase( 0, 1 );
  while( !name.empty() && name.back() == L'.' ) name.pop_back();
  if( name.size() > BOARD_RESOURCE_FOLDER_MAX_NAME )
  {
    name.resize( BOARD_RESOURCE_FOLDER_MAX_NAME );
    while( !name.empty() && name.back() == L'.' ) name.pop_back();
  }

  // Reserved DOS device names map to devices even as folder names (the part
  // before the first dot is what matters: "NUL.board" is still NUL).
  static const wchar_t * kReserved[] = {
    L"CON", L"PRN", L"AUX", L"NUL",
    L"COM1", L"COM2", L"COM3", L"COM4", L"COM5", L"COM6", L"COM7", L"COM8", L"COM9",
    L"LPT1", L"LPT2", L"LPT3", L"LPT4", L"LPT5", L"LPT6", L"LPT7", L"LPT8", L"LPT9" };
  const std::wstring base = name.substr( 0, name.find( L'.' ) );
  for( const wchar_t * reserved : kReserved )
  {
    if( _wcsicmp( base.c_str(), reserved ) == 0 ) return L"";
  }
  return name;
}

std::wstring GetBoardResourceFolder( const std::wstring & boardStyle )
{
  if( boardStyle.empty() ) return L"";
  BoardStyle parsed;
  ParseBoardStyleList( boardStyle, parsed );
  // An explicit resource-folder overrides the title as the folder-name source;
  // when it sanitizes to empty there is deliberately NO fallback to the title
  // (the author overrode the title on purpose).
  const std::wstring name = SanitizeBoardFolderName(
    parsed.window.resourceFolder.empty() ? parsed.window.title : parsed.window.resourceFolder );
  if( name.empty() ) return L"";
  const std::wstring root = GetAppDataCustomFolder( APP_NAME );
  if( root.empty() ) return L"";
  return root + L"\\" + name;
}

bool MergeMoveFolder( const std::wstring & oldFolder, const std::wstring & newFolder )
{
  if( oldFolder.empty() || newFolder.empty() ) return false;
  if( _wcsicmp( oldFolder.c_str(), newFolder.c_str() ) == 0 ) return true;

  const DWORD attrs = GetFileAttributesW( oldFolder.c_str() );
  if( attrs == INVALID_FILE_ATTRIBUTES || !( attrs & FILE_ATTRIBUTE_DIRECTORY ) )
    return true; // nothing to move

  // Fast path: plain rename when the target does not exist yet.
  if( GetFileAttributesW( newFolder.c_str() ) == INVALID_FILE_ATTRIBUTES &&
    MoveFileW( oldFolder.c_str(), newFolder.c_str() ) )
    return true;

  // Merge: move files one by one without overwriting; failures (name clash,
  // locked file) stay behind in the old folder — never destructive.
  CreateDirectoryW( newFolder.c_str(), nullptr );
  bool allMoved = true;
  WIN32_FIND_DATAW fd;
  HANDLE hFind = FindFirstFileW( ( oldFolder + L"\\*" ).c_str(), &fd );
  if( hFind != INVALID_HANDLE_VALUE )
  {
    do
    {
      if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) continue; // board folders are flat
      const std::wstring src = oldFolder + L"\\" + fd.cFileName;
      const std::wstring dst = newFolder + L"\\" + fd.cFileName;
      if( !MoveFileW( src.c_str(), dst.c_str() ) ) allMoved = false;
    } while( FindNextFileW( hFind, &fd ) );
    FindClose( hFind );
  }
  if( allMoved ) RemoveDirectoryW( oldFolder.c_str() );
  return allMoved;
}

// Resolves a resource reference strictly within the allowed folders (by base
// name). Deliberately does NOT accept the ref as an arbitrary absolute/relative
// path, so a file outside the allowed folders is never bundled.
static std::wstring ResolveResourcePath( const std::wstring & ref, const std::vector<std::wstring> & folders )
{
  const std::wstring base = BaseNameW( ref );
  for( const auto & f : folders )
  {
    std::wstring full = f;
    if( !full.empty() && full.back() != L'\\' && full.back() != L'/' ) full += L'\\';
    full += base;
    if( FileExists( full ) ) return full;
  }
  return L"";
}

// Distinct resource basenames (icons/audio) that actually resolve to a file.
static std::map<std::wstring, std::wstring> CollectResources(
  const std::vector<Category> & categories, const std::vector<std::wstring> & folders )
{
  std::map<std::wstring, std::wstring> resources; // basename -> resolved source path
  auto consider = [&]( const std::wstring & ref )
    {
      if( ref.empty() || !HasAllowedResourceExt( ref ) ) return;
      std::wstring src = ResolveResourcePath( ref, folders );
      if( !src.empty() ) resources[BaseNameW( ref )] = src;
    };
  for( const auto & cat : categories )
  {
    consider( cat.icon );
    for( const auto & ph : cat.phrases )
    {
      consider( ph.icon );
      consider( ph.audioFile );
    }
  }
  return resources;
}

bool CategoriesHaveBundledResources( const std::vector<Category> & categories, const std::wstring & resourceFolder, bool appDataOnly, const std::wstring & boardResourceFolder )
{
  return !CollectResources( categories, BuildResourceSearchFolders( resourceFolder, appDataOnly, boardResourceFolder ) ).empty();
}

bool IsZipArchive( const std::wstring & filePath )
{
  std::ifstream f( filePath, std::ios::binary );
  if( !f ) return false;
  unsigned char sig[4] = { 0, 0, 0, 0 };
  f.read( (char *) sig, 4 );
  if( f.gcount() < 4 ) return false;
  return sig[0] == 'P' && sig[1] == 'K' && sig[2] == 0x03 && sig[3] == 0x04;
}

// Validates a Zip entry name (raw UTF-8 bytes) against path-traversal tricks.
static bool IsSafeZipEntryName( const char * name )
{
  if( !name || !*name ) return false;
  if( name[0] == '/' ) return false;                    // absolute (posix-style)
  for( const char * p = name; *p; ++p )
  {
    unsigned char c = (unsigned char) *p;
    if( c < 0x20 ) return false;                        // control characters
    if( c == '\\' ) return false;                       // backslash separator / traversal
    if( c == ':' ) return false;                        // drive letter / alternate data stream
  }
  if( strstr( name, ".." ) != nullptr ) return false;   // any parent-directory traversal
  return true;
}

static std::wstring CreateUniqueTempDir()
{
  wchar_t tempPath[MAX_PATH];
  DWORD n = GetTempPathW( MAX_PATH, tempPath );
  if( n == 0 || n > MAX_PATH ) return L"";
  for( int attempt = 0; attempt < 16; ++attempt )
  {
    std::wstring dir = std::wstring( tempPath ) + L"SimonSays_ssz_" +
      std::to_wstring( GetCurrentProcessId() ) + L"_" +
      std::to_wstring( GetTickCount64() + (ULONGLONG) attempt );
    if( CreateDirectoryW( dir.c_str(), nullptr ) ) return dir;
  }
  return L"";
}

bool ExportCategoriesToSsz( const std::vector<Category> & categories, const std::wstring & filePath, const std::wstring & resourceFolder, bool appDataOnly, const std::wstring & boardStyle, const std::wstring & boardResourceFolder )
{
  const std::vector<std::wstring> folders = BuildResourceSearchFolders( resourceFolder, appDataOnly, boardResourceFolder );
  const std::map<std::wstring, std::wstring> resources = CollectResources( categories, folders );

  // Rewrite resolved references to bare basenames so they resolve from the
  // resource folder after import on any machine.
  std::vector<Category> rewritten = categories;
  auto rewriteRef = [&]( std::wstring & field )
    {
      if( field.empty() || !HasAllowedResourceExt( field ) ) return;
      std::wstring base = BaseNameW( field );
      if( resources.find( base ) != resources.end() ) field = base;
    };
  for( auto & cat : rewritten )
  {
    rewriteRef( cat.icon );
    for( auto & ph : cat.phrases )
    {
      rewriteRef( ph.icon );
      rewriteRef( ph.audioFile );
    }
  }

  const std::string sscData = SerializeCategoriesToUtf8( rewritten, boardStyle );

  mz_zip_archive zip;
  memset( &zip, 0, sizeof( zip ) );
  if( !mz_zip_writer_init_heap( &zip, 0, 0 ) ) return false;

  bool ok = mz_zip_writer_add_mem( &zip, SSZ_SSC_ENTRY_NAME, sscData.data(), sscData.size(), MZ_DEFAULT_COMPRESSION ) != 0;

  for( auto it = resources.begin(); ok && it != resources.end(); ++it )
  {
    std::string bytes;
    if( !ReadFileBytes( it->second, bytes, SSZ_MAX_ENTRY_UNCOMPRESSED ) ) continue; // skip unreadable/oversized
    const std::string entryName = std::string( SSZ_RESOURCE_PREFIX ) + Utf8FromWide( it->first );
    ok = mz_zip_writer_add_mem( &zip, entryName.c_str(), bytes.data(), bytes.size(), MZ_DEFAULT_COMPRESSION ) != 0;
  }

  void * pBuf = nullptr;
  size_t bufSize = 0;
  if( ok ) ok = mz_zip_writer_finalize_heap_archive( &zip, &pBuf, &bufSize ) != 0;
  mz_zip_writer_end( &zip );

  if( !ok )
  {
    if( pBuf ) mz_free( pBuf );
    return false;
  }

  std::ofstream out( filePath, std::ios::binary );
  bool wrote = false;
  if( out )
  {
    out.write( (const char *) pBuf, (std::streamsize) bufSize );
    wrote = out.good();
  }
  mz_free( pBuf );

  if( !wrote )
  {
    DeleteFileW( filePath.c_str() );
    return false;
  }
  return true;
}

void DiscardPendingSszResources( PendingSszResources & pending )
{
  if( !pending.tempDir.empty() )
  {
    for( const auto & leaf : pending.leafNames )
      DeleteFileW( ( pending.tempDir + L"\\" + leaf ).c_str() );
    RemoveDirectoryW( pending.tempDir.c_str() );
  }
  pending.tempDir.clear();
  pending.leafNames.clear();
}

bool CommitPendingSszResources( PendingSszResources & pending, const std::wstring & targetFolder )
{
  if( pending.leafNames.empty() )
  {
    DiscardPendingSszResources( pending ); // nothing to install; just clean the temp dir
    return true;
  }

  bool ok = !targetFolder.empty();
  if( ok )
  {
    // The target may be a board subfolder whose parent (the app-data root)
    // does not exist yet on a fresh machine — create parent, then target.
    const size_t slash = targetFolder.find_last_of( L"\\/" );
    if( slash != std::wstring::npos )
      CreateDirectoryW( targetFolder.substr( 0, slash ).c_str(), nullptr );
    CreateDirectoryW( targetFolder.c_str(), nullptr );

    for( const auto & leaf : pending.leafNames )
    {
      const std::wstring src = pending.tempDir + L"\\" + leaf;
      const std::wstring dst = targetFolder + L"\\" + leaf;
      if( !CopyFileW( src.c_str(), dst.c_str(), FALSE ) ) { ok = false; break; }
    }
  }
  DiscardPendingSszResources( pending );
  return ok;
}

bool ImportCategoriesFromSsz( const std::wstring & filePath, const std::wstring & resourceFolder,
  std::vector<Category> & outCategories, std::wstring & errorDetail, std::wstring * outBoardStyle,
  const std::wstring & boardResourceFolder, PendingSszResources * outPending )
{
  outCategories.clear();
  errorDetail.clear();

  std::string archive;
  if( !ReadFileBytes( filePath, archive, SSZ_MAX_TOTAL_UNCOMPRESSED ) )
  {
    errorDetail = L"archive unreadable or too large";
    return false;
  }

  mz_zip_archive zip;
  memset( &zip, 0, sizeof( zip ) );
  if( !mz_zip_reader_init_mem( &zip, archive.data(), archive.size(), 0 ) )
  {
    errorDetail = L"not a valid zip archive";
    return false;
  }
  struct ReaderGuard { mz_zip_archive * z; ~ReaderGuard() { mz_zip_reader_end( z ); } } readerGuard{ &zip };

  const mz_uint numFiles = mz_zip_reader_get_num_files( &zip );
  if( numFiles == 0 || numFiles > SSZ_MAX_ENTRIES )
  {
    errorDetail = L"entry count out of range";
    return false;
  }

  int sscIndex = -1;
  unsigned long long totalUncomp = 0;
  std::map<std::wstring, std::wstring> presentResources;  // lowercased basename -> actual basename
  std::map<std::wstring, int> resourceIndexByName;        // actual basename -> entry index

  for( mz_uint i = 0; i < numFiles; ++i )
  {
    mz_zip_archive_file_stat st;
    if( !mz_zip_reader_file_stat( &zip, i, &st ) )
    {
      errorDetail = L"corrupt central directory";
      return false;
    }

    if( mz_zip_reader_is_file_a_directory( &zip, i ) ) continue; // ignore directory entries

    // Reject symlinks (Unix mode is in the high 16 bits of the external attributes).
    if( ( ( st.m_external_attr >> 16 ) & 0xF000 ) == 0xA000 )
    {
      errorDetail = L"symlink entry rejected";
      return false;
    }

    if( !IsSafeZipEntryName( st.m_filename ) )
    {
      errorDetail = L"unsafe entry name";
      return false;
    }

    // Per-entry and aggregate anti zip-bomb limits.
    if( st.m_uncomp_size > SSZ_MAX_ENTRY_UNCOMPRESSED )
    {
      errorDetail = L"entry too large";
      return false;
    }
    totalUncomp += st.m_uncomp_size;
    if( totalUncomp > SSZ_MAX_TOTAL_UNCOMPRESSED )
    {
      errorDetail = L"archive too large uncompressed";
      return false;
    }
    if( st.m_comp_size > 0 && ( st.m_uncomp_size / st.m_comp_size ) > SSZ_MAX_COMPRESSION_RATIO )
    {
      errorDetail = L"suspicious compression ratio";
      return false;
    }

    const std::string name = st.m_filename;
    if( name == SSZ_SSC_ENTRY_NAME )
    {
      if( sscIndex >= 0 ) { errorDetail = L"multiple .ssc entries"; return false; }
      sscIndex = (int) i;
    }
    else if( name.rfind( SSZ_RESOURCE_PREFIX, 0 ) == 0 )
    {
      const std::string leaf = name.substr( strlen( SSZ_RESOURCE_PREFIX ) );
      if( leaf.empty() || leaf.find( '/' ) != std::string::npos )
      {
        errorDetail = L"nested resource path rejected";
        return false;
      }
      const std::wstring wleaf = WideFromUtf8( leaf );
      if( !HasAllowedResourceExt( wleaf ) )
      {
        errorDetail = L"disallowed resource extension";
        return false;
      }
      presentResources[ToLowerW( wleaf )] = wleaf;
      resourceIndexByName[wleaf] = (int) i;
    }
    else
    {
      errorDetail = L"unexpected entry";
      return false;
    }
  }

  if( sscIndex < 0 )
  {
    errorDetail = L"no categories.ssc in archive";
    return false;
  }

  // Extract and parse the .ssc straight from memory.
  size_t sscSize = 0;
  void * sscBuf = mz_zip_reader_extract_to_heap( &zip, (mz_uint) sscIndex, &sscSize, 0 );
  if( !sscBuf )
  {
    errorDetail = L"failed to extract categories.ssc";
    return false;
  }
  std::string sscContent( (const char *) sscBuf, sscSize );
  mz_free( sscBuf );

  std::vector<Category> parsed;
  if( !ParseCategoriesFromUtf8( sscContent, parsed, outBoardStyle ) )
  {
    errorDetail = L"invalid categories.ssc content";
    return false;
  }

  // Cross-reference each resource reference:
  //   1. bundled in the archive            -> keep (use the extracted basename)
  //   2. not bundled but resolvable locally -> keep (e.g. resources shipped in the
  //      exe/working folder, which app-data-only export deliberately omits)
  //   3. neither                            -> strip (policy B)
  // The local lookup uses the full folder set so it matches how the app resolves
  // resources at runtime (PlaybackEngine / CategoryWindow icon lookup).
  const std::vector<std::wstring> localFolders = BuildResourceSearchFolders( resourceFolder, /*appDataOnly=*/false, boardResourceFolder );
  auto reconcile = [&]( std::wstring & field )
    {
      if( field.empty() || !HasAllowedResourceExt( field ) ) return;
      auto it = presentResources.find( ToLowerW( BaseNameW( field ) ) );
      if( it != presentResources.end() )
        field = it->second;                                            // bundled
      else if( !ResolveResourcePath( BaseNameW( field ), localFolders ).empty() )
        field = BaseNameW( field );                                    // resolvable locally -> keep
      else
        field.clear();                                                 // dangling -> strip
    };
  for( auto & cat : parsed )
  {
    reconcile( cat.icon );
    for( auto & ph : cat.phrases )
    {
      reconcile( ph.icon );
      reconcile( ph.audioFile );
    }
  }

  // Two-phase commit: extract resources to a temp dir; installation happens
  // afterwards — below when the caller did not ask for a pending handoff, or
  // in the caller's CommitPendingSszResources when it did (so the target can
  // be chosen after the board-style adoption decision).
  std::wstring tempDir = CreateUniqueTempDir();
  if( tempDir.empty() )
  {
    errorDetail = L"cannot create temp directory";
    return false;
  }

  std::vector<std::wstring> tempFiles;
  std::vector<std::wstring> leafNames;
  bool extractOk = true;

  for( auto it = resourceIndexByName.begin(); extractOk && it != resourceIndexByName.end(); ++it )
  {
    size_t sz = 0;
    void * buf = mz_zip_reader_extract_to_heap( &zip, (mz_uint) it->second, &sz, 0 );
    if( !buf ) { extractOk = false; break; }

    const std::wstring destPath = tempDir + L"\\" + it->first;
    // Defense-in-depth: the resolved path must stay inside tempDir.
    wchar_t full[MAX_PATH];
    if( GetFullPathNameW( destPath.c_str(), MAX_PATH, full, nullptr ) == 0 ||
      _wcsnicmp( full, tempDir.c_str(), tempDir.size() ) != 0 )
    {
      mz_free( buf );
      extractOk = false;
      break;
    }

    extractOk = WriteFileBytes( destPath, buf, sz );
    mz_free( buf );
    if( extractOk )
    {
      tempFiles.push_back( destPath );
      leafNames.push_back( it->first );
    }
  }

  if( !extractOk )
  {
    for( const auto & f : tempFiles ) DeleteFileW( f.c_str() );
    RemoveDirectoryW( tempDir.c_str() );
    errorDetail = L"failed to extract resources";
    return false;
  }

  PendingSszResources pending;
  pending.tempDir = tempDir;
  pending.leafNames = leafNames;

  if( outPending )
  {
    *outPending = std::move( pending ); // caller commits (or discards) later
  }
  else if( !CommitPendingSszResources( pending, resourceFolder ) )
  {
    errorDetail = L"failed to install resources";
    return false;
  }

  outCategories = std::move( parsed );
  return true;
}

std::wstring PromptExportCategoriesFilePath( HWND owner, const std::wstring & language, const std::wstring & suggestedFileName, const std::wstring & defaultExt )
{
  wchar_t fileName[MAX_PATH] = L"";
  wcsncpy_s( fileName, suggestedFileName.c_str(), MAX_PATH - 1 );

  // GetSaveFileName appends the extension of the *selected filter's first
  // pattern* when the user types none; lpstrDefExt is only a fallback for the
  // "*.*" filter. A combined "*.ssc;*.ssz" filter therefore always forces .ssc.
  // Build the filter with the preferred format's group first so the dialog's
  // auto-appended extension matches defaultExt (the user can still switch).
  const bool ssz = ( _wcsicmp( defaultExt.c_str(), L"ssz" ) == 0 );
  std::wstring filter;
  auto addGroup = [&filter]( const wchar_t * label, const wchar_t * pattern )
    {
      filter.append( label );   filter.push_back( L'\0' );
      filter.append( pattern ); filter.push_back( L'\0' );
    };
  if( ssz )
  {
    addGroup( L"SimonSays Categories Bundle (*.ssz)", L"*.ssz" );
    addGroup( L"SimonSays Categories (*.ssc)", L"*.ssc" );
  }
  else
  {
    addGroup( L"SimonSays Categories (*.ssc)", L"*.ssc" );
    addGroup( L"SimonSays Categories Bundle (*.ssz)", L"*.ssz" );
  }
  addGroup( L"All Files", L"*.*" );
  filter.push_back( L'\0' ); // second NUL terminates the filter list

  OPENFILENAMEW ofn;
  ZeroMemory( &ofn, sizeof( ofn ) );
  ofn.lStructSize = sizeof( ofn );
  ofn.hwndOwner = owner;
  ofn.lpstrFilter = filter.c_str();
  ofn.nFilterIndex = 1;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
  ofn.lpstrDefExt = defaultExt.c_str();
  ofn.lpstrFile = fileName;
  ofn.lpstrTitle = GetLocalizedString( EXPORT_CATEGORIES_DIALOG_TITLE_ID, language );
  if( GetSaveFileName( &ofn ) )
  {
    return std::wstring( fileName );
  }
  return L"";
}

std::wstring PromptImportCategoriesFilePath( HWND owner, const std::wstring & language )
{
  wchar_t fileName[MAX_PATH] = L"";
  OPENFILENAMEW ofn;
  ZeroMemory( &ofn, sizeof( ofn ) );
  ofn.lStructSize = sizeof( ofn );
  ofn.hwndOwner = owner;
  ofn.lpstrFilter = GetLocalizedString( IMPORT_CATEGORIES_DIALOG_FILTER_ID, language );
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR;
  ofn.lpstrDefExt = L"ssc";
  ofn.lpstrFile = fileName;
  ofn.lpstrTitle = GetLocalizedString( IMPORT_CATEGORIES_DIALOG_TITLE_ID, language );
  ofn.Flags |= OFN_FILEMUSTEXIST;
  if( GetOpenFileName( &ofn ) )
  {
    return std::wstring( fileName );
  }
  return L"";
}

std::wstring GetSystemLanguage()
{
  wchar_t langBuffer[LOCALE_NAME_MAX_LENGTH];
  if( !GetUserDefaultLocaleName( langBuffer, LOCALE_NAME_MAX_LENGTH ) )
    return L"English";

  // Normalize to lowercase so case-sensitive prefix matches work against the
  // Windows convention (e.g. GetUserDefaultLocaleName returns "ca-ES-valencia"
  // with capital "ES", which previously prevented the valencian branch from firing).
  std::wstring lang( langBuffer );
  for( auto & c : lang ) c = (wchar_t) towlower( c );

  // Two-letter ISO 639-1 prefix → SUPPORTED_LANGUAGES English name.
  if( lang.find( L"ar" ) == 0 ) return L"Arabic";
  if( lang.find( L"eu" ) == 0 ) return L"Basque";
  if( lang.find( L"ca" ) == 0 )
  {
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

const wchar_t * GetLocalizedString( int stringId, std::wstring language )
{
  if( language.empty() )
  {
    language = GetSystemLanguage();
  }

  for( const auto & langPair : LOCALIZED_STRINGS )
  {
    if( langPair.first == language )
    {
      for( const auto & strPair : langPair.second )
      {
        if( strPair.first == stringId )
        {
          return strPair.second;
        }
      }
      break;
    }
  }

  if( language != L"English" )
  {
    return GetLocalizedString( stringId, L"English" );
  }
  else
  {
    return L"";
  }
}

std::wstring ReplaceAmpersandLocalized( const std::wstring & str, const std::wstring & language )
{
  std::wstring amp = GetLocalizedString( AMPERSAND_REPLACEMENT_ID, language );
  return ReplaceAll( str, L"&", amp );
}

bool IsLanguageRTL( const std::wstring & language )
{
  if( language.empty() )
  {
    return IsLanguageRTL( GetSystemLanguage() );
  }

  for( const auto & langPair : SUPPORTED_LANGUAGES )
  {
    if( langPair.EnglishName == language )
    {
      return langPair.IsRTL;
    }
  }
  return false;
}

std::wstring GetProductVersionString()
{
  // 1. Get the path of the current module
  wchar_t szFilePath[MAX_PATH];
  GetModuleFileName( NULL, szFilePath, MAX_PATH );

  // 2. Get the size of the version info
  DWORD dwHandle = 0;
  DWORD dwSize = GetFileVersionInfoSize( szFilePath, &dwHandle );
  if( dwSize == 0 ) return L"";

  // 3. Retrieve the version info block
  std::vector<BYTE> buffer( dwSize );
  if( !GetFileVersionInfo( szFilePath, dwHandle, dwSize, buffer.data() ) )
  {
    return L"";
  }

  // 4. Look up the translation ID (Language/Code Page)
  struct LANGANDCODEPAGE
  {
    WORD wLanguage;
    WORD wCodePage;
  } *lpTranslate;

  UINT cbTranslate = 0;
  if( !VerQueryValue( buffer.data(), L"\\VarFileInfo\\Translation", (LPVOID *) &lpTranslate, &cbTranslate ) )
  {
    return L"";
  }

  // 5. Build the query path for the ProductVersion string
  // Format: \StringFileInfo\LangIDCodePage\ProductVersion
  wchar_t subBlock[64];
  swprintf_s( subBlock, L"\\StringFileInfo\\%04x%04x\\ProductVersion",
    lpTranslate[0].wLanguage, lpTranslate[0].wCodePage );

  // 6. Retrieve the string from the buffer
  wchar_t * lpVersionStr = nullptr;
  UINT uiLen = 0;
  if( VerQueryValue( buffer.data(), subBlock, (LPVOID *) &lpVersionStr, &uiLen ) && lpVersionStr )
  {
    return std::wstring( lpVersionStr );
  }

  return L"";
}

std::wstring GetAppDataCustomFolder( const std::wstring & appName )
{
  PWSTR path_tmp;

  // Get the base AppData/Local path
  HRESULT hr = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, NULL, &path_tmp );

  if( FAILED( hr ) )
  {
    return L"";
  }

  // Convert wide string to standard string
  std::wstring path( path_tmp );

  // Free the memory allocated by Windows
  CoTaskMemFree( path_tmp );

  return path + L"\\" + appName;
}

std::wstring GetExecutableDirectory()
{
  wchar_t buffer[MAX_PATH] = L"";
  DWORD len = GetModuleFileName( NULL, buffer, MAX_PATH );
  // 0 == failure; MAX_PATH with ERROR_INSUFFICIENT_BUFFER == truncated.
  if( len == 0 || len == MAX_PATH ) return L"";

  std::wstring path( buffer, len );
  size_t lastSlash = path.find_last_of( L"\\/" );
  if( lastSlash == std::wstring::npos ) return L"";
  path.resize( lastSlash + 1 );
  // The directory that contained our module is essentially guaranteed to
  // exist — the GetFileAttributes check was paranoia. Keep it cheap.
  return ( GetFileAttributes( path.c_str() ) != INVALID_FILE_ATTRIBUTES ) ? path : L"";
}

std::wstring GetWorkingDirectory()
{
  // First call with NULL buffer queries the required size (incl. terminator).
  DWORD needed = GetCurrentDirectory( 0, nullptr );
  if( needed == 0 ) return L"";
  std::wstring path( needed, L'\0' );
  DWORD written = GetCurrentDirectory( needed, &path[0] );
  if( written == 0 || written >= needed ) return L"";
  path.resize( written );
  return path;
}

bool FileExists( const std::wstring & path )
{
  DWORD attributes = GetFileAttributes( path.c_str() );
  return ( attributes != INVALID_FILE_ATTRIBUTES && !( attributes & FILE_ATTRIBUTE_DIRECTORY ) );
}

std::wstring GetLanguageStringFromLangId( LANGID langId )
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

std::wstring GetLanguageNativeName( std::wstring language )
{
  if( language.empty() )
  {
    language = GetSystemLanguage();
  }

  for( const auto & lang : SUPPORTED_LANGUAGES )
  {
    if( lang.EnglishName == language )
    {
      return lang.NativeName;
    }
  }
  return language;
}

LANGID GetLangIdFromLanguageString( std::wstring language )
{
  if( language.empty() )
  {
    language = GetSystemLanguage();
  }

  for( const auto & lang : SUPPORTED_LANGUAGES )
  {
    if( lang.EnglishName == language )
    {
      return lang.LanguageId;
    }
  }
  return MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL );
}

UINT GetMessageBoxFlagsForLanguage( const std::wstring & language, UINT baseFlags )
{
  return baseFlags | ( IsLanguageRTL( language ) ? ( MB_RTLREADING | MB_RIGHT ) : 0 );
}

int ShowLocalizedMessageBox( HWND hwnd, const wchar_t * text, const wchar_t * caption, UINT baseFlags, const std::wstring & language )
{
  return MessageBoxEx( hwnd, text, caption, GetMessageBoxFlagsForLanguage( language, baseFlags ), GetLangIdFromLanguageString( language ) );
}

static COLORREF DarkenColor( COLORREF c, bool lightTheme )
{
  COLORREF t = lightTheme ? RGB( 243, 243, 243 ) : RGB( 41, 41, 41 );
  float cfactor = lightTheme ? 1.0f : 1.5f;
  float tfactor = lightTheme ? 0.4f : 1.0f;
  float dfactor = lightTheme ? cfactor + tfactor + 0.4f : cfactor + tfactor + 0.2f;

  int r = GetRValue( c );
  int g = GetGValue( c );
  int b = GetBValue( c );


  r = min( 255, max( 0, (int) ( ( ( r * cfactor ) + ( GetRValue( t ) * tfactor ) ) / dfactor ) ) );
  g = min( 255, max( 0, (int) ( ( ( g * cfactor ) + ( GetGValue( t ) * tfactor ) ) / dfactor ) ) );
  b = min( 255, max( 0, (int) ( ( ( b * cfactor ) + ( GetBValue( t ) * tfactor ) ) / dfactor ) ) );
  return RGB( r, g, b );
}

// Get taskbar color (dark/light theme)
COLORREF GetTaskbarColor()
{
  HKEY hKey;
  DWORD colorPrevalence = 0;
  DWORD systemUsesLightTheme = 0;
  DWORD appsUseLightTheme = 0;
  DWORD dwSize = sizeof( DWORD );

  // 1. Check if "Show accent color on Start and taskbar" is enabled
  if( RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey ) == ERROR_SUCCESS )
  {
    RegQueryValueEx( hKey, L"ColorPrevalence", NULL, NULL, (LPBYTE) &colorPrevalence, &dwSize );
    dwSize = sizeof( DWORD );
    RegQueryValueEx( hKey, L"SystemUsesLightTheme", NULL, NULL, (LPBYTE) &systemUsesLightTheme, &dwSize );
    dwSize = sizeof( DWORD );
    RegQueryValueEx( hKey, L"AppsUseLightTheme", NULL, NULL, (LPBYTE) &appsUseLightTheme, &dwSize );
    RegCloseKey( hKey );
  }

  // 2. If ColorPrevalence is 1, fetch the custom accent color
  if( colorPrevalence == 1 )
  {
    return DarkenColor( GetAccentColor(), systemUsesLightTheme == 1 || appsUseLightTheme == 1 );
  }

  // 3. Fallback: If ColorPrevalence is 0, Windows uses standard Dark or Light colors
  if( systemUsesLightTheme == 1 )
  {
    // Default Windows Light Taskbar color (Off-white/Gray)
    return RGB( 243, 243, 243 );
  }
  else
  {
    // Default Windows Dark Taskbar color (Charcoal/Black)
    return RGB( 41, 41, 41 ); // Typical Windows 11 dark mode color
  }
}

COLORREF GetAccentColor()
{
  HKEY hKey;
  DWORD dwColor;
  DWORD dwSize = sizeof( DWORD );

  if( RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\DWM", 0, KEY_READ, &hKey ) == ERROR_SUCCESS )
  {
    dwSize = sizeof( dwColor );
    if( RegQueryValueEx( hKey, L"AccentColor", NULL, NULL, (LPBYTE) &dwColor, &dwSize ) == ERROR_SUCCESS )
    {
      RegCloseKey( hKey );
      // Convert ABGR (Registry) to standard RGB COLORREF
      BYTE r = ( dwColor & 0x000000FF );
      BYTE g = ( dwColor & 0x0000FF00 ) >> 8;
      BYTE b = ( dwColor & 0x00FF0000 ) >> 16;
      return RGB( r, g, b );
    }
    RegCloseKey( hKey );
  }

  if( RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Accent", 0, KEY_READ, &hKey ) == ERROR_SUCCESS )
  {
    dwSize = sizeof( dwColor );
    if( RegQueryValueEx( hKey, L"AccentColor", NULL, NULL, (LPBYTE) &dwColor, &dwSize ) == ERROR_SUCCESS )
    {
      RegCloseKey( hKey );
      // Convert ABGR (Registry) to standard RGB COLORREF
      BYTE r = ( dwColor & 0x000000FF );
      BYTE g = ( dwColor & 0x0000FF00 ) >> 8;
      BYTE b = ( dwColor & 0x00FF0000 ) >> 16;
      return RGB( r, g, b );
    }
    RegCloseKey( hKey );
  }

  if( RegOpenKeyEx( HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Accent", 0, KEY_READ, &hKey ) == ERROR_SUCCESS )
  {
    dwSize = sizeof( dwColor );
    if( RegQueryValueEx( hKey, L"AccentColorMenu", NULL, NULL, (LPBYTE) &dwColor, &dwSize ) == ERROR_SUCCESS )
    {
      RegCloseKey( hKey );
      // Convert ABGR (Registry) to standard RGB COLORREF
      BYTE r = ( dwColor & 0x000000FF );
      BYTE g = ( dwColor & 0x0000FF00 ) >> 8;
      BYTE b = ( dwColor & 0x00FF0000 ) >> 16;
      return RGB( r, g, b );
    }
    RegCloseKey( hKey );
  }

  // Fallback to a default color (e.g., Windows Blue) if registry read fails
  return RGB( 0, 120, 215 );
}

SIZE GetTextDimensions( HWND hwnd, const wchar_t * text )
{
  HDC hdc = GetDC( hwnd );
  SIZE size = { 0, 0 };


  HFONT hFont = (HFONT) SendMessage( hwnd, WM_GETFONT, 0, 0 );
  HFONT hOldFont = (HFONT) SelectObject( hdc, hFont );
  GetTextExtentPoint32( hdc, text, lstrlen( text ), &size );
  SelectObject( hdc, hOldFont );
  ReleaseDC( hwnd, hdc );

  return size;
}

void CenterEditTextVertically( HWND hEdit )
{
  // Center text vertically in edit control
  HDC hdc = GetDC( hEdit );
  // Get the font used in the edit control
  HFONT hFont = (HFONT) SendMessage( hEdit, WM_GETFONT, 0, 0 );
  HFONT hOldFont = (HFONT) SelectObject( hdc, hFont );

  TEXTMETRIC tm;
  GetTextMetrics( hdc, &tm );
  int iTextHeight = tm.tmHeight;

  // Restore the original font and release the device context
  SelectObject( hdc, hOldFont );
  ReleaseDC( hEdit, hdc );

  RECT rect;
  GetClientRect( hEdit, &rect );
  rect.top = ( rect.bottom - iTextHeight ) / 2;
  rect.bottom -= rect.top;
  rect.left += 2; // small left margin
  rect.right -= 2; // small right margin    
  SendMessage( hEdit, EM_SETRECT, 0, (LPARAM) &rect );
}

void ConfigureSlider( HWND hDlg, int sliderId, int minValue, int maxValue, int initialValue, int tickMarks )
{
  HWND hSlider = GetDlgItem( hDlg, sliderId );
  if( !hSlider ) return;
  SendMessage( hSlider, TBM_SETRANGE, FALSE, MAKELONG( minValue, maxValue ) );
  int tickFreq = max( 1, ( maxValue - minValue ) / max( 1, tickMarks ) );
  SendMessage( hSlider, TBM_SETTICFREQ, tickFreq, 0 );
  SendMessage( hSlider, TBM_SETPOS, TRUE, initialValue );
}

void SyncSliderToEdit( HWND hDlg, int sliderId, int editId, BOOL isSigned )
{
  HWND hSlider = GetDlgItem( hDlg, sliderId );
  if( !hSlider ) return;
  int pos = (int) SendMessage( hSlider, TBM_GETPOS, 0, 0 );
  SetDlgItemInt( hDlg, editId, pos, isSigned );
}

void SyncEditToSlider( HWND hDlg, int editId, int sliderId, BOOL isSigned, int minValue, int maxValue )
{
  BOOL translated = FALSE;
  int value = (int) GetDlgItemInt( hDlg, editId, &translated, isSigned );
  if( !translated ) return;
  value = ( maxValue > minValue ) ? min( maxValue, max( minValue, value ) ) : ( value );
  SendDlgItemMessage( hDlg, sliderId, TBM_SETPOS, TRUE, value );
}


// Function to check if a window is the Taskbar or a child of the Taskbar
/*
bool IsTaskbarWindow( HWND hwnd )
{
  while( hwnd )
  {
    OutputDebugString( ( L"Window: " + std::to_wstring( (uintptr_t) hwnd ) + L"\n" ).c_str() );

    wchar_t className[256] = {};
    GetClassName( hwnd, className, 256 );
    OutputDebugString( className );
    OutputDebugString( L"\n" );
    OutputDebugString( ( L"GetParent window: " + std::to_wstring( (uintptr_t) GetParent( hwnd ) ) + L"\n" ).c_str() );
    OutputDebugString( ( L"GetAncestor window: " + std::to_wstring( (uintptr_t) GetAncestor( hwnd, GA_ROOTOWNER ) ) + L"\n" ).c_str() );

    if( wcscmp( className, L"Shell_TrayWnd" ) == 0 ||
      wcscmp( className, L"Shell_SecondaryTrayWnd" ) == 0 ) // Windows.UI.Core.CoreWindow
    {
      OutputDebugString( ( L"Taskbar window found: " + std::to_wstring( (uintptr_t) hwnd ) + L"\n" ).c_str() );
      OutputDebugString( ( L"Shell window found: " + std::to_wstring( (uintptr_t) GetShellWindow() ) + L"\n" ).c_str() );
      OutputDebugString( ( L"Shell_TrayWnd window found: " + std::to_wstring( (uintptr_t) FindWindow( L"Shell_TrayWnd", NULL ) ) + L"\n" ).c_str() );
      return true;
    }
    hwnd = GetParent( hwnd );
  }
  return false;
}
*/

/**
 * Checks if the Windows Taskbar is currently positioned at the bottom of the screen.
 * * @return true if the taskbar is at the bottom, false otherwise.
 */
bool IsTaskbarAtBottom()
{
  APPBARDATA appBarData;
  // Initialize the structure size
  appBarData.cbSize = sizeof( APPBARDATA );

  // Get the handle to the taskbar. "Shell_TrayWnd" is the class name for the taskbar.
  appBarData.hWnd = FindWindow( L"Shell_TrayWnd", NULL );

  if( appBarData.hWnd != NULL )
  {
    // Retrieve the taskbar's position and bounding rectangle
    if( SHAppBarMessage( ABM_GETTASKBARPOS, &appBarData ) )
    {
      // ABE_BOTTOM is a constant defined in shellapi.h representing the bottom edge
      return ( appBarData.uEdge == ABE_BOTTOM );
    }
  }

  // Default return if the handle or message fails
  return false;
}

/**
 * Retrieves the horizontal (X) screen coordinate of the Start button.
 * This works for both Windows 10 (separate button) and Windows 11 (integrated taskbar).
 * * @return The X coordinate of the top-left corner of the Start button,
 * or -1 if the button could not be found.
 */
int GetStartButtonXPosition()
{
  // In Windows 10/11, the Start button is often a child of the taskbar 
  // or has the class name "Start" (Win 10) or "Button" (Win 11)

  // First, try finding the Windows 10 style Start button
  HWND hWndStart = FindWindow( L"Button", L"Start" );

  // If not found (Windows 11 or custom shell), try the class name "Start"
  if( !hWndStart )
  {
    hWndStart = FindWindow( L"Start", NULL );
  }

  // Fallback: On some Win 11 builds, the button is part of the "Shell_TrayWnd" 
  // and we might need to find the child window
  if( !hWndStart )
  {
    HWND hWndTray = FindWindow( L"Shell_TrayWnd", NULL );
    hWndStart = FindWindowEx( hWndTray, NULL, L"Start", NULL );
  }

  if( hWndStart )
  {
    RECT rect;
    // GetWindowRect retrieves the dimensions of the bounding rectangle 
    // in screen coordinates
    if( GetWindowRect( hWndStart, &rect ) )
    {
      return rect.left;
    }
  }

  return -1; // Could not determine position
}

/**
 * Retrieves the horizontal (X) screen coordinate of the System Tray (Notification Area).
 * This area contains the clock, volume, network, and background app icons.
 * * @return The X coordinate of the top-left corner of the tray area,
 * or -1 if the area could not be found.
 */
int GetSystemTrayXPosition()
{
  // 1. Find the main Taskbar window
  HWND hWndTray = FindWindow( L"Shell_TrayWnd", NULL );

  if( hWndTray )
  {
    // 2. Find the Notification Area window inside the Taskbar
    // The class name for this specific area is "TrayNotifyWnd"
    HWND hWndNotify = FindWindowEx( hWndTray, NULL, L"TrayNotifyWnd", NULL );

    if( hWndNotify )
    {
      RECT rect;
      // Get the bounding rectangle in screen coordinates
      if( GetWindowRect( hWndNotify, &rect ) )
      {
        return rect.left;
      }
    }
  }

  return -1; // Could not find the system tray
}

void updateRtlExStyle( HWND hCtrl, bool isRtl, LONG_PTR baseExStyle )
{
  if( !hCtrl ) return;
  LONG_PTR exStyle = GetWindowLongPtr( hCtrl, GWL_EXSTYLE );
  LONG_PTR rtlFlags = WS_EX_LAYOUTRTL | WS_EX_RTLREADING;
  LONG_PTR newExStyle = ( isRtl ? ( baseExStyle | exStyle | rtlFlags ) : ( ( baseExStyle | exStyle ) & ~rtlFlags ) );
  if( newExStyle != exStyle )
  {
    SetWindowLongPtr( hCtrl, GWL_EXSTYLE, newExStyle );
    SetWindowPos( hCtrl, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED );
  }
}

void updateEditAlignment( HWND hEdit, bool isRtl )
{
  if( !hEdit ) return;
  LONG_PTR style = GetWindowLongPtr( hEdit, GWL_STYLE );
  LONG_PTR exStyle = GetWindowLongPtr( hEdit, GWL_EXSTYLE );
  LONG_PTR newStyle = style;
  if( isRtl )
  {
    newStyle &= ~( ES_LEFT | ES_CENTER );
    newStyle |= ES_RIGHT;
  }
  else
  {
    newStyle &= ~( ES_RIGHT | ES_CENTER );
    newStyle |= ES_LEFT;
  }

  if( newStyle != style )
  {
    SetWindowLongPtr( hEdit, GWL_STYLE, newStyle );
    SetWindowPos( hEdit, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED );
  }

  updateRtlExStyle( hEdit, isRtl, exStyle );
}

BOOL CALLBACK ApplyRtlStylesCallback( HWND hwnd, LPARAM lParam )
{
  // Use the W-form to stay consistent with the rest of the Unicode-only codebase
  // and to avoid the ANSI thunk's truncation surprises on non-ASCII class names.
  // lParam is treated as a bool — callers must pass 0 or 1 only.
  wchar_t className[64];
  GetClassNameW( hwnd, className, ARRAYSIZE( className ) );

  const bool isRtl = ( lParam != 0 );
  if( _wcsicmp( className, L"Edit" ) == 0 )
    updateEditAlignment( hwnd, isRtl );
  else
    updateRtlExStyle( hwnd, isRtl );

  return TRUE;
}

HWND FindTouchKeyboardWindow()
{
  // Guard the WS_DISABLED check: GetWindowLongPtr on a NULL HWND is undefined.
  HWND hKeyboard = FindWindow( L"IPTip_Main_Window", NULL );
  bool needFallback = !hKeyboard;
  if( hKeyboard )
  {
    LONG_PTR style = GetWindowLongPtr( hKeyboard, GWL_STYLE );
    needFallback = ( style & WS_DISABLED ) != 0;
  }
  if( needFallback )
    hKeyboard = FindWindow( L"ApplicationFrameWindow", NULL ); // Win 11 fallback
  return hKeyboard;
}

struct ITipInvocation: IUnknown
{
  virtual HRESULT STDMETHODCALLTYPE Toggle( HWND wnd ) = 0;
};

static bool IsTouchKeyboardVisible()
{
  HWND hKeyboard = FindTouchKeyboardWindow();
  if( !hKeyboard || !IsWindowVisible( hKeyboard ) )
    return false;

  // The keyboard window can be cloaked (hidden by DWM) even when IsWindowVisible returns TRUE
  DWORD cloaked = 0;
  HRESULT hr = DwmGetWindowAttribute( hKeyboard, DWMWA_CLOAKED, &cloaked, sizeof( cloaked ) );
  if( SUCCEEDED( hr ) && cloaked != 0 )
    return false;

  return true;
}

static void PositionTouchKeyboard( HWND hwndNear, SIZE alignment, int margin )
{
  if( !hwndNear ) return;

  HWND hKeyboard = FindTouchKeyboardWindow();
  if( !hKeyboard ) return;

  RECT rcMain, rcKeyboard;
  if( !GetWindowRect( hwndNear, &rcMain ) ) return;
  if( !GetWindowRect( hKeyboard, &rcKeyboard ) ) return;

  int kbWidth = rcKeyboard.right - rcKeyboard.left;
  int kbHeight = rcKeyboard.bottom - rcKeyboard.top;

  int x = 0;
  switch( alignment.cx )
  {
    case -2: x = rcMain.left - kbWidth - margin; break; // Place to the left of the main window
    case -1: x = rcMain.left; break; // Align left edges
    case  0: x = rcMain.left + ( ( rcMain.right - rcMain.left ) - kbWidth ) / 2; break; // Center horizontally
    case  1: x = rcMain.right - kbWidth; break; // Align right edges
    case  2: x = rcMain.right + margin; break; // Place to the right of the main window
    default: x = rcMain.left - kbWidth - margin;
  }

  if( alignment.cy > 2 )
    x = rcMain.right + margin;

  int y = 0;

  switch( alignment.cy )
  {
    case -2: y = rcMain.top - kbHeight - margin; break; // Place above the main window
    case -1: y = rcMain.top; break; // Align top edges
    case  0: y = rcMain.top + ( ( rcMain.bottom - rcMain.top ) - kbHeight ) / 2; break; // Center vertically
    case  1: y = rcMain.bottom - kbHeight; break; // Align bottom edges
    case  2: y = rcMain.bottom + margin; break; // Place below the main window
    default: y = rcMain.top - kbHeight - margin;
  }

  if( alignment.cy > 2 )
    y = rcMain.bottom + margin;

  // Clamp to screen bounds
  RECT rcDesktop;
  GetWindowRect( GetDesktopWindow(), &rcDesktop );
  if( x + kbWidth > rcDesktop.right )
    x = rcDesktop.right - kbWidth;
  if( x < rcDesktop.left )
    x = rcDesktop.left;
  if( y < rcDesktop.top )
    y = rcDesktop.top;

  SetCursorPos( x + ( kbWidth / 2 ), y + ( kbHeight / 2 ) ); // Move cursor to the center of the keyboard for better touch experience
  SetWindowPos( hKeyboard, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
  ShowWindow( hKeyboard, SW_SHOWNA );
}

void ShowTouchKeyboard( HWND hwndNear, SIZE alignment, int margin )
{
  // alignment specifies where to place the keyboard relative to the main window:
  // cx -2 => Place to the left of the window
  // cx -1 => Align left edges
  // cx  0 => Center horizontally
  // cx  1 => Align right edges
  // cx  2 => Place to the right of the window
  // cy -2 => Place above the  window
  // cy -1 => Align top edges
  // cy  0 => Center vertically
  // cy  1 => Align bottom edges
  // cy  2 => Place below the window

  // Ensure TabTip.exe process is running
  HWND hKeyboard = FindTouchKeyboardWindow();

  if( !hKeyboard )
  {
    ShellExecute( NULL, L"open",
      L"C:\\Program Files\\Common Files\\microsoft shared\\ink\\TabTip.exe",
      NULL, NULL, SW_SHOWNORMAL );
    return;
  }

  // Only toggle if the keyboard is not already visible (Toggle would hide it otherwise)
  if( !IsTouchKeyboardVisible() )
  {
    // {4CE576FA-83DC-4f88-951C-9D0782B4E376}
    static const GUID CLSID_UIHostNoLaunch =
    { 0x4CE576FA, 0x83DC, 0x4f88, { 0x95, 0x1C, 0x9D, 0x07, 0x82, 0xB4, 0xE3, 0x76 } };

    // {37c994e7-432b-4834-a2f7-dce1f13b834b}
    static const GUID IID_ITipInvocation =
    { 0x37c994e7, 0x432b, 0x4834, { 0xa2, 0xf7, 0xdc, 0xe1, 0xf1, 0x3b, 0x83, 0x4b } };

    ITipInvocation * pTip = nullptr;
    HRESULT hr = CoCreateInstance( CLSID_UIHostNoLaunch, nullptr,
      CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER,
      IID_ITipInvocation, (void **) &pTip );
    if( SUCCEEDED( hr ) && pTip )
    {
      pTip->Toggle( GetDesktopWindow() );
      pTip->Release();
    }
  }

  PositionTouchKeyboard( hwndNear, alignment, margin );
}

void SimulateClickReal( HWND hwnd, int localX, int localY )
{
  // 0.Save the original cursor position to restore it later
  POINT originalPos;
  GetCursorPos( &originalPos );

  // 1. Bring the window to the foreground (highly recommended for SendInput)
  SetForegroundWindow( hwnd );

  // 2. Convert local window coordinates to absolute screen coordinates
  POINT pt = { localX, localY };
  ClientToScreen( hwnd, &pt );

  // 3. Move the physical cursor to the target position
  SetCursorPos( pt.x, pt.y );

  // 4. Prepare the mouse input structure
  INPUT inputs[2] = {};

  // Mouse Down
  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

  // Mouse Up
  inputs[1].type = INPUT_MOUSE;
  inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

  // 5. Send the input events to the operating system
  SendInput( 2, inputs, sizeof( INPUT ) );

  // 6. Restore the original cursor position  
  SetCursorPos( originalPos.x, originalPos.y );
}

void SimulateNonActivatingClickOnWindow( HWND hwnd )
{
  // 0.Save the original cursor position to restore it later
  POINT originalPos;
  GetCursorPos( &originalPos );

  // 1. Bring the window to the foreground (highly recommended for SendInput)
  SetForegroundWindow( hwnd );

  // 2. Convert local window coordinates to absolute screen coordinates
  RECT rect;
  GetWindowRect( hwnd, &rect );

  // 3. Move the physical cursor to the center of the window
  SetCursorPos( rect.left + ( rect.right - rect.left ) / 2, rect.top + 2 );

  // 4. Prepare the mouse input structure
  INPUT inputs[2] = {};

  // Mouse Down
  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

  // Mouse Up
  inputs[1].type = INPUT_MOUSE;
  inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

  // 5. Send the input events to the operating system
  SendInput( 2, inputs, sizeof( INPUT ) );

  // 6. Restore the original cursor position  
  inputs[0].type = INPUT_MOUSE;
  inputs[0].mi.mouseData = 0;
  inputs[0].mi.time = 0;
  inputs[0].mi.dx = originalPos.x * ( 65536 / GetSystemMetrics( SM_CXSCREEN ) );//x being coord in pixels
  inputs[0].mi.dy = originalPos.y * ( 65536 / GetSystemMetrics( SM_CYSCREEN ) );//y being 
  inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;
  SendInput( 1, inputs, sizeof( INPUT ) );
  SetCursorPos( originalPos.x, originalPos.y );

}