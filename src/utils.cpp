#include "utils.h"
#include "localized_strings.h"
#include <commctrl.h>
#include <vector>
#include <windows.h>
#include <shellapi.h>

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

std::wstring GetSystemLanguage()
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

// Get taskbar color (dark/light theme)
COLORREF GetTaskbarColor()
{
  HKEY hKey;
  DWORD value = 0;
  DWORD size = sizeof( DWORD );

  // Check if dark mode is enabled
  if( RegOpenKeyExW( HKEY_CURRENT_USER,
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
    0, KEY_READ, &hKey ) == ERROR_SUCCESS )
  {

    RegQueryValueExW( hKey, L"SystemUsesLightTheme", NULL, NULL,
      (LPBYTE) &value, &size );
    RegCloseKey( hKey );
  }

  // If dark mode is enabled (value == 0), use dark color
  if( value == 0 )
  {
    return RGB( 32, 32, 32 ); // Typical Windows 11 dark mode color
  }
  else
  {
    return RGB( 243, 243, 243 ); // Typical Windows 11 light mode color
  }
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
