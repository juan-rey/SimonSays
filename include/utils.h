#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <string>

std::wstring ReplaceAll( std::wstring str, const std::wstring & from, const std::wstring & to );
std::wstring GetSystemLanguage();
const wchar_t * GetLocalizedString( int stringId, std::wstring language );
bool IsLanguageRTL( const std::wstring & language );
std::wstring GetProductVersionString();
std::wstring GetLanguageStringFromLangId( LANGID langId );
COLORREF GetTaskbarColor();
void CenterEditTextVertically( HWND hEdit );
void ConfigureSlider( HWND hDlg, int sliderId, int minValue, int maxValue, int initialValue, int tickMarks = 10 );
void SyncSliderToEdit( HWND hDlg, int sliderId, int editId, BOOL isSigned ); 
void SyncEditToSlider( HWND hDlg, int editId, int sliderId, BOOL isSigned, int minValue = 0, int maxValue = 0 );
bool IsTaskbarAtBottom();
int GetStartButtonXPosition();
int GetSystemTrayXPosition();

#endif // UTILS_H