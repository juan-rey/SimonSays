#include "MainWindow.h"
#include <windows.h>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow )
{
  UNREFERENCED_PARAMETER( hPrevInstance );
  UNREFERENCED_PARAMETER( pCmdLine );

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
        mainWindow.RunMessageLoop();
      }
    }
    CoUninitialize();
  }

  CloseHandle( hInstanceMutex );
  return 0;
}