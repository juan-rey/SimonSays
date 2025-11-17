#include "MainWindow.h"
#include <windows.h>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow )
{
  UNREFERENCED_PARAMETER( hPrevInstance );
  UNREFERENCED_PARAMETER( pCmdLine );

  HRESULT hr = CoInitialize( nullptr );
  if( FAILED( hr ) )
  {
    return 1;
  }
  
  MainWindow mainWindow;

  if( !mainWindow.Create( hInstance, nCmdShow ) )
  {
    return 0;
  }

  mainWindow.RunMessageLoop();

  CoUninitialize();
  return 0;
}