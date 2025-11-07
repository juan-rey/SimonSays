#include "MainWindow.h"
#include <windows.h>
#include <iostream>

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow )
{
  UNREFERENCED_PARAMETER( hPrevInstance );
  UNREFERENCED_PARAMETER( pCmdLine );
  // Inicializar COM
  HRESULT hr = CoInitialize( nullptr );
  if( FAILED( hr ) )
  {
    std::cerr << "Error al inicializar COM" << std::endl;
    return 1;
  }

  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof( INITCOMMONCONTROLSEX );
  icex.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx( &icex );

  MainWindow mainWindow;

  if( !mainWindow.Create( hInstance, nCmdShow ) )
  {
    return 0;
  }

  mainWindow.RunMessageLoop();

  // Finalizar COM
  CoUninitialize();
  return 0;
}