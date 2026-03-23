#include "MainWindow.h"
#include <windows.h>
#include <shellapi.h>

static std::wstring GetSscPathFromCommandLine()
{
  int argc = 0;
  LPWSTR * argv = CommandLineToArgvW( GetCommandLineW(), &argc );
  if( !argv )
  {
    return L"";
  }

  std::wstring filePath;
  for( int i = 1; i < argc; ++i )
  {
    const std::wstring arg = argv[i] ? argv[i] : L"";
    if( arg.length() > 4 && _wcsicmp( arg.c_str() + ( arg.length() - 4 ), L".ssc" ) == 0 )
    {
      filePath = arg;
      break;
    }
  }

  LocalFree( argv );
  return filePath;
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow )
{
  UNREFERENCED_PARAMETER( hPrevInstance );
  UNREFERENCED_PARAMETER( pCmdLine );

  const std::wstring sscPath = GetSscPathFromCommandLine();

  HANDLE hInstanceMutex = CreateMutex( nullptr, TRUE, L"SimonSaysSingletonMutex" );
  if( hInstanceMutex == nullptr )
  {
    return 1;
  }

  if( GetLastError() == ERROR_ALREADY_EXISTS )
  {
    HWND hExisting = FindWindow( CLASS_NAME, nullptr );
    if( hExisting )
    {
      if( !IsWindowVisible( hExisting ) )
      {
        ShowWindow( hExisting, SW_SHOW );
      }
      ShowWindow( hExisting, SW_RESTORE );
      SetForegroundWindow( hExisting );
      SetWindowPos( hExisting, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );

      if( !sscPath.empty() )
      {
        COPYDATASTRUCT cds = {};
        cds.dwData = SIMONSAYS_COPYDATA_IMPORT_SSC;
        cds.cbData = static_cast<DWORD>( ( sscPath.size() + 1 ) * sizeof( wchar_t ) );
        cds.lpData = (PVOID) sscPath.c_str();
        SendMessage( hExisting, WM_COPYDATA, 0, (LPARAM) &cds );
      }
    }
  }
  else
  {
    HRESULT hr = CoInitialize( nullptr );
    if( FAILED( hr ) )
    {
      CloseHandle( hInstanceMutex );
      return 1;
    }
    else
    {
      MainWindow mainWindow;

      if( mainWindow.Create( hInstance, nCmdShow ) )
      {
        if( !sscPath.empty() )
        {
          if( !sscPath.empty() )
          {
            COPYDATASTRUCT cds = {};
            cds.dwData = SIMONSAYS_COPYDATA_IMPORT_SSC;
            cds.cbData = static_cast<DWORD>( ( sscPath.size() + 1 ) * sizeof( wchar_t ) );
            cds.lpData = (PVOID) sscPath.c_str();
            SendMessage( mainWindow.GetHwnd(), WM_COPYDATA, 0, (LPARAM) &cds);
          }
        }
        mainWindow.RunMessageLoop();
      }
    }
    CoUninitialize();
  }

  CloseHandle( hInstanceMutex );
  return 0;
}