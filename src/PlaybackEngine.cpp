/*
   PlaybackEngine.cpp: PlaybackEngine class source file.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "PlaybackEngine.h"
#include "utils.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <sapi.h>
#pragma warning(disable:4996)
#include <sphelper.h>
#pragma warning(default: 4996)
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <endpointvolume.h>

#define INTERRUPT_CHECK_INTERVAL_MS 100
#define FALLBACK_MP3_FILE L"fallback.mp3"
#define FALLBACK_WAV_FILE L"fallback.wav"

bool IsMciPlaying( std::wstring alias )
{
  TCHAR buffer[128];
  // mciCommand: "status mi_alias mode"
  std::wstring mciCommand = L"status " + alias + L" mode";

  mciSendString( mciCommand.c_str(), buffer, 128, NULL );

  // let's check if the status is "playing"         
  std::wstring mciStatus( buffer );

  return ( mciStatus == L"playing" );
}

// Returns duration in milliseconds, or 0 on error.
DWORD GetWavDuration( const wchar_t * path )
{
  // Open the RIFF file
  HMMIO hmmio = mmioOpenW( (LPWSTR) path, NULL, MMIO_READ | MMIO_ALLOCBUF );
  if( !hmmio ) return 0;

  // Descend into the RIFF/WAVE chunk
  MMCKINFO ckRiff = { 0 };
  ckRiff.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );
  if( mmioDescend( hmmio, &ckRiff, NULL, MMIO_FINDRIFF ) != MMSYSERR_NOERROR )
  {
    mmioClose( hmmio, 0 );
    return 0;
  }

  // Find the 'fmt ' sub-chunk
  MMCKINFO ckFmt = { 0 };
  ckFmt.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
  if( mmioDescend( hmmio, &ckFmt, &ckRiff, MMIO_FINDCHUNK ) != MMSYSERR_NOERROR )
  {
    mmioClose( hmmio, 0 );
    return 0;
  }

  WAVEFORMATEX wfx = { 0 };
  LONG bytesRead = mmioRead( hmmio, (HPSTR) &wfx, sizeof( WAVEFORMATEX ) );
  if( bytesRead < (LONG) offsetof( WAVEFORMATEX, cbSize ) )
  {
    mmioClose( hmmio, 0 );
    return 0;
  }

  mmioAscend( hmmio, &ckFmt, 0 );

  // Find the 'data' sub-chunk
  MMCKINFO ckData = { 0 };
  ckData.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
  if( mmioDescend( hmmio, &ckData, &ckRiff, MMIO_FINDCHUNK ) != MMSYSERR_NOERROR )
  {
    mmioClose( hmmio, 0 );
    return 0;
  }

  // Calculate duration
  // Use 64-bit arithmetic to avoid overflow on large files
  DWORD durationMs = 0;
  if( wfx.nAvgBytesPerSec > 0 )
    durationMs = (DWORD) ( ( (ULONGLONG) ckData.cksize * 1000ULL )
      / wfx.nAvgBytesPerSec );

  mmioClose( hmmio, 0 );
  return durationMs;
}

PlaybackEngine::PlaybackEngine( HWND hwndOwner, const std::wstring & voiceKey, int volume, int rate )
  : m_hwndOwner( hwndOwner ), m_voiceKey( voiceKey ), m_volume( volume ), m_rate( rate ),
  m_warmUpNeeded( voiceKey.find( L"Aholab" ) != std::wstring::npos )
{
  m_soundFileFolders.push_back( GetAppDataCustomFolder( APP_NAME ) );
  if( GetWorkingDirectory() != GetExecutableDirectory() ) // avoid duplicates if both are the same
    m_soundFileFolders.push_back( GetWorkingDirectory() );
  m_soundFileFolders.push_back( GetExecutableDirectory() );

  m_fallbackSoundFilePath = FALLBACK_WAV_FILE;
  ExpandSoundFilePath( m_fallbackSoundFilePath );

  // ugly workaround to ensure MCI can play mp3 files on the worker thread by pre-opening an mp3 file on the main thread and keeping it open (so the necessary codecs are loaded in memory)
  std::wstring mp3File = FALLBACK_MP3_FILE;
  ExpandSoundFilePath( mp3File );
  std::wstring openCmd = L"open \"" + mp3File + L"\" type mpegvideo alias workaround";
  if( mciSendString( openCmd.c_str(), NULL, 0, m_hwndMCI ) == 0 )
  {

    std::wstring wavFile = FALLBACK_WAV_FILE;
    ExpandSoundFilePath( wavFile );
    openCmd = L"open \"" + wavFile + L"\" type waveaudio alias wavfile";
    if( mciSendString( openCmd.c_str(), NULL, 0, NULL ) == 0 )
    {
      m_usePlaySoundForWav = false;
    }
    else
    {
      // PlaySound can not be stopped reliably, so we use an mp3 file as fallback since MCI can play and stop it
      m_fallbackSoundFilePath = mp3File;
    }
  }

  m_workerThread = std::thread( &PlaybackEngine::WorkerThread, this );
}

PlaybackEngine::~PlaybackEngine()
{
  m_shutdown = true;
  m_stopRequested = true;
  InterruptCurrentPlayback();
  m_incomingCV.notify_one();
  if( m_workerThread.joinable() )
    m_workerThread.join();
  mciSendString( L"close workaround", NULL, 0, m_hwndMCI );
  mciSendString( L"close wavfile", NULL, 0, m_hwndMCI );
}

void PlaybackEngine::QueueText( const std::wstring & text, bool stopPrevious )
{
  if( stopPrevious )
    Stop();

  {
    std::lock_guard<std::mutex> lk( m_incomingMutex );
    m_incomingQueue.push( text );
  }

  m_incomingCV.notify_one();
}

void PlaybackEngine::Stop()
{
  m_stopRequested = true;
  {
    std::lock_guard<std::mutex> lk( m_incomingMutex );
    std::queue<std::wstring>().swap( m_incomingQueue );
  }
  {
    std::lock_guard<std::mutex> lk( m_playingMutex );
    std::queue<PlaybackSegment>().swap( m_playingQueue );
  }
  InterruptCurrentPlayback();
}

bool PlaybackEngine::IsPlaying() const
{
  return m_isPlaying;
}

void PlaybackEngine::SetVoiceSettings( const std::wstring & voiceKey, int volume, int rate )
{
  if( ( m_voiceKey != voiceKey ) || ( m_volume != volume ) || ( m_rate != rate ) )
  {
    std::lock_guard<std::mutex> lk( m_settingsMutex );
    m_voiceKey = voiceKey;
    m_volume = volume;
    m_rate = rate;
    if( voiceKey.find( L"Aholab" ) != std::wstring::npos )
      m_warmUpNeeded = true;
    else
      m_warmUpNeeded = false;
    m_settingsChanged = true;
  }
}

void PlaybackEngine::InterruptCurrentPlayback()
{
  if( m_pVoice )
    m_pVoice->Speak( nullptr, SPF_PURGEBEFORESPEAK, nullptr );
  if( m_usePlaySoundForWav )
    PlaySound( NULL, NULL, SND_PURGE );
}

void PlaybackEngine::ApplyVoiceSettings()
{
  if( !m_pVoice ) return;

  std::wstring voiceKey;
  int volume, rate;
  bool warmUp;

  {
    std::lock_guard<std::mutex> lk( m_settingsMutex );
    voiceKey = m_voiceKey;
    volume = m_volume;
    rate = m_rate;
    warmUp = m_warmUpNeeded;
    m_warmUpNeeded = false;
  }

  if( voiceKey.empty() )
  {
    LPWSTR pszTokenId = NULL;
    if( SUCCEEDED( SpGetDefaultTokenIdFromCategoryId( SPCAT_VOICES, &pszTokenId ) ) )
    {
      voiceKey = pszTokenId;
      CoTaskMemFree( pszTokenId );
    }
  }

  if( !voiceKey.empty() )
  {
    ISpObjectToken * token = nullptr;
    if( SUCCEEDED( SpGetTokenFromId( voiceKey.c_str(), &token, FALSE ) ) )
    {
      m_pVoice->SetVoice( token );
      token->Release();
    }
  }

  m_pVoice->SetVolume( (USHORT) volume );
  m_pVoice->SetRate( (long) rate );

  if( warmUp )
  {
    m_pVoice->Speak( L" ", SPF_ASYNC | SPF_IS_NOT_XML, nullptr );
  }
  m_settingsChanged = false;
}

void PlaybackEngine::WorkerThread()
{
  CoInitializeEx( NULL, COINIT_MULTITHREADED );

  HRESULT hr = CoCreateInstance( CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **) &m_pVoice );
  if( FAILED( hr ) || !m_pVoice )
    m_pVoice = nullptr;

  ApplyVoiceSettings();

  float savedAppVolume = -1.0f;
  IncreaseAppVolume( true );
  savedAppVolume = m_savedAppVolume;
  RestoreAppVolume();
  if( savedAppVolume == 1.0f )
    m_useComputerVolume = true; // if we can't increase app volume, we'll have to increase computer volume instead, but we check this in advance to avoid unnecessarily increasing and restoring computer volume on every playback which could cause issues with some audio drivers

  float savedComputerVolume = -1.0f;
  IncreaseComputerVolume( true );
  savedComputerVolume = m_savedComputerVolume;
  RestoreComputerVolume();
  if( savedComputerVolume > COMPUTER_VOLUME_BOOST )
    m_computerVolumeBoost = 1.0f; // if computer volume is already high, don't apply boost to avoid making it too loud

  if( m_reduceOtherAudioWhenPlaying )
  {
    if( m_useComputerVolume && m_increaseVolumeWhenPlaying )
      m_otherAppsVolumeFactor = OTHER_APPS_VOLUME_FACTOR / 2;
    else
      m_otherAppsVolumeFactor = OTHER_APPS_VOLUME_FACTOR;
  }

  if( m_useHiddenWindowForMCI )
  {
    // for some reason MCI won't work properly on this thread until we create a message queue by calling PeekMessage or similar, even if we don't actually use it to receive messages (we receive MCI notifications via callback, not messages)
    MSG msg;
    PeekMessage( &msg, NULL, WM_USER, WM_USER, PM_NOREMOVE );

    // Create a hidden message-only window to receive MCI notifications (required for MCI_NOTIFY_SUCCESSFUL to work)
    WNDCLASS wc = {};
    wc.lpfnWndProc = PlaybackEngine::HiddenWndProc;
    wc.hInstance = GetModuleHandle( NULL );
    wc.lpszClassName = L"MCIWindow";
    RegisterClass( &wc );

    m_hwndMCI = CreateWindow(
      L"MCIWindow", L"", 0,
      0, 0, 0, 0,
      HWND_MESSAGE,   // message-only window
      NULL, GetModuleHandle( NULL ), this
    );
  }

  std::queue<std::wstring> tmpQueue;
  // Main loop: wait for text to play, then process and play it
  while( !m_shutdown )
  {

    if( !m_shutdown && m_incomingQueue.empty() && tmpQueue.empty() )
    {
      std::unique_lock<std::mutex> lk( m_incomingMutex );
      m_incomingCV.wait( lk, [&] { return !m_incomingQueue.empty() || m_shutdown; } ); // Wait until there's text to play or shutdown is requested
      if( m_shutdown ) break;
      tmpQueue.push( m_incomingQueue.front() );
      m_incomingQueue.pop();
      m_stopRequested = false; // tmpQueue was empty, reset stop requested flag to allow playback to proceed, if we didn't reset it here, the worker thread would consume the new text but then immediately stop playback because stop was requested while there was no text in the queue
    }
    else
    {
      std::unique_lock<std::mutex> lk( m_incomingMutex );
      while( !m_shutdown && !m_incomingQueue.empty() )
      {
        tmpQueue.push( m_incomingQueue.front() );
        m_incomingQueue.pop();
      }
    }

    while( !m_shutdown && !m_stopRequested && !tmpQueue.empty() )
    {
      // Parse text into segments and populate playing queue
      std::vector<PlaybackSegment> segments = ParseText( tmpQueue.front() );
      tmpQueue.pop();
      {
        std::lock_guard<std::mutex> lk( m_playingMutex );
        for( auto & seg : segments )
          m_playingQueue.push( std::move( seg ) );
      }
    }

    if( !m_shutdown && m_settingsChanged )
      ApplyVoiceSettings();

    if( !m_shutdown && !m_playingQueue.empty() )
    {
      m_isPlaying = true;
      PostMessage( m_hwndOwner, WM_PLAYBACK_STARTED, 0, 0 );

      if( m_reduceOtherAudioWhenPlaying )
        ReduceOtherAppsVolume(); // this method is safe with no running conditions, RestoreOtherAppsVolume will only restore volumes that were reduced by this method, so it's safe to call it multiple times without checking if we already reduced other apps volume or not
      //MuteOtherApps(); // Muting other apps causes issues with some games that pause when they detect their audio is muted, so instead of muting we just reduce their volume to a very low level, this way they can keep playing without disturbing the TTS audio

      if( m_increaseVolumeWhenPlaying )
        if( m_useComputerVolume )
        {
          IncreaseComputerVolume(); // this method is safe with no running conditions, RestoreComputerVolume will only restore the volume if it was increased by this method, so it's safe to call it multiple times without checking if we already increased computer volume or not
        }
        else
        {
          IncreaseAppVolume(); // this method is safe with no running conditions, RestoreAppVolume will only restore the volume if it was increased by this method, so it's safe to call it multiple times without checking if we already increased app volume or not
        }

      // Play segments from the playing queue
      while( !m_stopRequested && !m_shutdown )
      {
        PlaybackSegment seg;
        {
          std::lock_guard<std::mutex> lk( m_playingMutex );
          if( m_playingQueue.empty() ) break;
          seg = m_playingQueue.front();
          m_playingQueue.pop();
        }
        PlaySegment( seg );
      }

      RestoreComputerVolume();
      RestoreAppVolume(); // safe to call even if we didn't increase app volume, it will only restore the volume if it was increased by IncreaseAppVolume  otherwise it does nothing

      RestoreOtherAppsVolume(); // safe to call even if we didn't reduce other apps volume, it will only restore volumes that were reduced by ReduceOtherAppsVolume otherwise it does nothing
      //UnmuteOtherApps();

      m_isPlaying = false;
      PostMessage( m_hwndOwner, WM_PLAYBACK_FINISHED, 0, 0 );
    }

    if( !m_shutdown && m_stopRequested )
    {
      while( !m_shutdown && !tmpQueue.empty() )
      {
        tmpQueue.pop();
      }
      m_stopRequested = false;
    }

  }

  if( m_useHiddenWindowForMCI && m_hwndMCI )
    DestroyWindow( m_hwndMCI );

  if( m_pVoice )
  {
    try
    {
      m_pVoice->Speak( nullptr, SPF_PURGEBEFORESPEAK, nullptr );
      m_pVoice->Release();
      m_pVoice = nullptr;
    }
    catch( const std::exception & )
    {
    }
  }

  CoUninitialize();
}

LRESULT CALLBACK PlaybackEngine::HiddenWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  PlaybackEngine * pThis = nullptr;

  if( uMsg == WM_CREATE )
  {
    CREATESTRUCT * pCreate = (CREATESTRUCT *) lParam;
    pThis = (PlaybackEngine *) pCreate->lpCreateParams;
    SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
  }
  else
  {
    pThis = (PlaybackEngine *) GetWindowLongPtr( hwnd, GWLP_USERDATA );
  }

  if( pThis )
  {
    switch( uMsg )
    {
      case MM_MCINOTIFY:
      {
        if( LOWORD( wParam ) == MCI_NOTIFY_SUCCESSFUL )
        {
          // Playback finished
        }
        break;
      }
    }
  }

  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

void PlaybackEngine::ExpandSoundFilePath( std::wstring & filename )
{
  for( const auto & folder : m_soundFileFolders )
  {
    std::wstring fullPath = folder + L"\\" + filename;
    if( GetFileAttributes( fullPath.c_str() ) != INVALID_FILE_ATTRIBUTES )
    {
      filename = fullPath;
      break;
    }
  }
}

void PlaybackEngine::SetAudioDuckingSettings( bool increaseVolume, bool reduceOtherAudio )
{
  m_increaseVolumeWhenPlaying = increaseVolume;
  m_reduceOtherAudioWhenPlaying = reduceOtherAudio;
  if( reduceOtherAudio )
  {
    if( m_useComputerVolume && increaseVolume )
      m_otherAppsVolumeFactor = OTHER_APPS_VOLUME_FACTOR / 2;
    else
      m_otherAppsVolumeFactor = OTHER_APPS_VOLUME_FACTOR;
  }
}

void PlaybackEngine::ReduceOtherAppsVolume()
{
  m_savedOtherVolumes.clear();

  IMMDeviceEnumerator * pEnumerator = nullptr;
  HRESULT hr = CoCreateInstance( __uuidof( MMDeviceEnumerator ), nullptr, CLSCTX_ALL,
    __uuidof( IMMDeviceEnumerator ), (void **) &pEnumerator );
  if( FAILED( hr ) || !pEnumerator ) return;

  IMMDevice * pDevice = nullptr;
  hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &pDevice );
  if( FAILED( hr ) || !pDevice ) { pEnumerator->Release(); return; }

  IAudioSessionManager2 * pSessionManager = nullptr;
  hr = pDevice->Activate( __uuidof( IAudioSessionManager2 ), CLSCTX_ALL, nullptr, (void **) &pSessionManager );
  if( FAILED( hr ) || !pSessionManager ) { pDevice->Release(); pEnumerator->Release(); return; }

  IAudioSessionEnumerator * pSessionEnum = nullptr;
  hr = pSessionManager->GetSessionEnumerator( &pSessionEnum );
  if( FAILED( hr ) || !pSessionEnum ) { pSessionManager->Release(); pDevice->Release(); pEnumerator->Release(); return; }

  DWORD myPid = GetCurrentProcessId();
  int count = 0;
  pSessionEnum->GetCount( &count );

  for( int i = 0; i < count; i++ )
  {
    IAudioSessionControl * pControl = nullptr;
    if( FAILED( pSessionEnum->GetSession( i, &pControl ) ) || !pControl ) continue;

    IAudioSessionControl2 * pControl2 = nullptr;
    hr = pControl->QueryInterface( __uuidof( IAudioSessionControl2 ), (void **) &pControl2 );
    pControl->Release();
    if( FAILED( hr ) || !pControl2 ) continue;

    DWORD pid = 0;
    pControl2->GetProcessId( &pid );

    if( pid != myPid && pid != 0 )
    {
      ISimpleAudioVolume * pVolume = nullptr;
      hr = pControl2->QueryInterface( __uuidof( ISimpleAudioVolume ), (void **) &pVolume );
      if( SUCCEEDED( hr ) && pVolume )
      {
        float currentVolume = 1.0f;
        pVolume->GetMasterVolume( &currentVolume );
        m_savedOtherVolumes.push_back( { pid, currentVolume } );
        pVolume->SetMasterVolume( currentVolume * m_otherAppsVolumeFactor, nullptr );
        pVolume->Release();
      }
    }

    pControl2->Release();
  }

  pSessionEnum->Release();
  pSessionManager->Release();
  pDevice->Release();
  pEnumerator->Release();
}

void PlaybackEngine::RestoreOtherAppsVolume()
{
  if( m_savedOtherVolumes.empty() ) return;

  IMMDeviceEnumerator * pEnumerator = nullptr;
  HRESULT hr = CoCreateInstance( __uuidof( MMDeviceEnumerator ), nullptr, CLSCTX_ALL,
    __uuidof( IMMDeviceEnumerator ), (void **) &pEnumerator );
  if( FAILED( hr ) || !pEnumerator ) { m_savedOtherVolumes.clear(); return; }

  IMMDevice * pDevice = nullptr;
  hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &pDevice );
  if( FAILED( hr ) || !pDevice ) { pEnumerator->Release(); m_savedOtherVolumes.clear(); return; }

  IAudioSessionManager2 * pSessionManager = nullptr;
  hr = pDevice->Activate( __uuidof( IAudioSessionManager2 ), CLSCTX_ALL, nullptr, (void **) &pSessionManager );
  if( FAILED( hr ) || !pSessionManager ) { pDevice->Release(); pEnumerator->Release(); m_savedOtherVolumes.clear(); return; }

  IAudioSessionEnumerator * pSessionEnum = nullptr;
  hr = pSessionManager->GetSessionEnumerator( &pSessionEnum );
  if( FAILED( hr ) || !pSessionEnum ) { pSessionManager->Release(); pDevice->Release(); pEnumerator->Release(); m_savedOtherVolumes.clear(); return; }

  int count = 0;
  pSessionEnum->GetCount( &count );

  for( int i = 0; i < count; i++ )
  {
    IAudioSessionControl * pControl = nullptr;
    if( FAILED( pSessionEnum->GetSession( i, &pControl ) ) || !pControl ) continue;

    IAudioSessionControl2 * pControl2 = nullptr;
    hr = pControl->QueryInterface( __uuidof( IAudioSessionControl2 ), (void **) &pControl2 );
    pControl->Release();
    if( FAILED( hr ) || !pControl2 ) continue;

    DWORD pid = 0;
    pControl2->GetProcessId( &pid );

    for( const auto & saved : m_savedOtherVolumes )
    {
      if( saved.first == pid )
      {
        ISimpleAudioVolume * pVolume = nullptr;
        hr = pControl2->QueryInterface( __uuidof( ISimpleAudioVolume ), (void **) &pVolume );
        if( SUCCEEDED( hr ) && pVolume )
        {
          pVolume->SetMasterVolume( saved.second, nullptr );
          pVolume->Release();
        }
        break;
      }
    }

    pControl2->Release();
  }

  pSessionEnum->Release();
  pSessionManager->Release();
  pDevice->Release();
  pEnumerator->Release();
  m_savedOtherVolumes.clear();
}

void PlaybackEngine::IncreaseAppVolume( bool testVolumeOnly )
{
  IMMDeviceEnumerator * pEnumerator = nullptr;
  HRESULT hr = CoCreateInstance( __uuidof( MMDeviceEnumerator ), nullptr, CLSCTX_ALL,
    __uuidof( IMMDeviceEnumerator ), (void **) &pEnumerator );
  if( FAILED( hr ) || !pEnumerator ) return;

  IMMDevice * pDevice = nullptr;
  hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &pDevice );
  if( FAILED( hr ) || !pDevice ) { pEnumerator->Release(); return; }

  IAudioSessionManager2 * pSessionManager = nullptr;
  hr = pDevice->Activate( __uuidof( IAudioSessionManager2 ), CLSCTX_ALL, nullptr, (void **) &pSessionManager );
  if( FAILED( hr ) || !pSessionManager ) { pDevice->Release(); pEnumerator->Release(); return; }

  IAudioSessionEnumerator * pSessionEnum = nullptr;
  hr = pSessionManager->GetSessionEnumerator( &pSessionEnum );
  if( FAILED( hr ) || !pSessionEnum ) { pSessionManager->Release(); pDevice->Release(); pEnumerator->Release(); return; }

  DWORD myPid = GetCurrentProcessId();
  int count = 0;
  pSessionEnum->GetCount( &count );

  for( int i = 0; i < count; i++ )
  {
    IAudioSessionControl * pControl = nullptr;
    if( FAILED( pSessionEnum->GetSession( i, &pControl ) ) || !pControl ) continue;

    IAudioSessionControl2 * pControl2 = nullptr;
    hr = pControl->QueryInterface( __uuidof( IAudioSessionControl2 ), (void **) &pControl2 );
    pControl->Release();
    if( FAILED( hr ) || !pControl2 ) continue;

    DWORD pid = 0;
    pControl2->GetProcessId( &pid );

    if( pid == myPid )
    {
      ISimpleAudioVolume * pVolume = nullptr;
      hr = pControl2->QueryInterface( __uuidof( ISimpleAudioVolume ), (void **) &pVolume );
      if( SUCCEEDED( hr ) && pVolume )
      {
        float currentVolume = 1.0f;
        pVolume->GetMasterVolume( &currentVolume );
        m_savedAppVolume = currentVolume;
        if( !testVolumeOnly )
        {
          pVolume->SetMasterVolume( m_appVolumeBoost, nullptr );
        }
        pVolume->Release();
      }
      pControl2->Release();
      break;
    }

    pControl2->Release();
  }

  pSessionEnum->Release();
  pSessionManager->Release();
  pDevice->Release();
  pEnumerator->Release();

  if( m_savedAppVolume < 0.0f )
    m_savedAppVolume = 1.0f; // if we failed to get the current volume, assume it's 100% so we can restore to that later
}

void PlaybackEngine::RestoreAppVolume()
{
  if( m_savedAppVolume < 0.0f ) return;

  IMMDeviceEnumerator * pEnumerator = nullptr;
  HRESULT hr = CoCreateInstance( __uuidof( MMDeviceEnumerator ), nullptr, CLSCTX_ALL,
    __uuidof( IMMDeviceEnumerator ), (void **) &pEnumerator );
  if( FAILED( hr ) || !pEnumerator ) { m_savedAppVolume = -1.0f; return; }

  IMMDevice * pDevice = nullptr;
  hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &pDevice );
  if( FAILED( hr ) || !pDevice ) { pEnumerator->Release(); m_savedAppVolume = -1.0f; return; }

  IAudioSessionManager2 * pSessionManager = nullptr;
  hr = pDevice->Activate( __uuidof( IAudioSessionManager2 ), CLSCTX_ALL, nullptr, (void **) &pSessionManager );
  if( FAILED( hr ) || !pSessionManager ) { pDevice->Release(); pEnumerator->Release(); m_savedAppVolume = -1.0f; return; }

  IAudioSessionEnumerator * pSessionEnum = nullptr;
  hr = pSessionManager->GetSessionEnumerator( &pSessionEnum );
  if( FAILED( hr ) || !pSessionEnum ) { pSessionManager->Release(); pDevice->Release(); pEnumerator->Release(); m_savedAppVolume = -1.0f; return; }

  DWORD myPid = GetCurrentProcessId();
  int count = 0;
  pSessionEnum->GetCount( &count );

  for( int i = 0; i < count; i++ )
  {
    IAudioSessionControl * pControl = nullptr;
    if( FAILED( pSessionEnum->GetSession( i, &pControl ) ) || !pControl ) continue;

    IAudioSessionControl2 * pControl2 = nullptr;
    hr = pControl->QueryInterface( __uuidof( IAudioSessionControl2 ), (void **) &pControl2 );
    pControl->Release();
    if( FAILED( hr ) || !pControl2 ) continue;

    DWORD pid = 0;
    pControl2->GetProcessId( &pid );

    if( pid == myPid )
    {
      ISimpleAudioVolume * pVolume = nullptr;
      hr = pControl2->QueryInterface( __uuidof( ISimpleAudioVolume ), (void **) &pVolume );
      if( SUCCEEDED( hr ) && pVolume )
      {
        pVolume->SetMasterVolume( m_savedAppVolume, nullptr );
        pVolume->Release();
      }
      pControl2->Release();
      break;
    }

    pControl2->Release();
  }

  pSessionEnum->Release();
  pSessionManager->Release();
  pDevice->Release();
  pEnumerator->Release();
  m_savedAppVolume = -1.0f;
}

void PlaybackEngine::MuteOtherApps()
{
  m_mutedPids.clear();

  IMMDeviceEnumerator * pEnumerator = nullptr;
  HRESULT hr = CoCreateInstance( __uuidof( MMDeviceEnumerator ), nullptr, CLSCTX_ALL,
    __uuidof( IMMDeviceEnumerator ), (void **) &pEnumerator );
  if( FAILED( hr ) || !pEnumerator ) return;

  IMMDevice * pDevice = nullptr;
  hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &pDevice );
  if( FAILED( hr ) || !pDevice ) { pEnumerator->Release(); return; }

  IAudioSessionManager2 * pSessionManager = nullptr;
  hr = pDevice->Activate( __uuidof( IAudioSessionManager2 ), CLSCTX_ALL, nullptr, (void **) &pSessionManager );
  if( FAILED( hr ) || !pSessionManager ) { pDevice->Release(); pEnumerator->Release(); return; }

  IAudioSessionEnumerator * pSessionEnum = nullptr;
  hr = pSessionManager->GetSessionEnumerator( &pSessionEnum );
  if( FAILED( hr ) || !pSessionEnum ) { pSessionManager->Release(); pDevice->Release(); pEnumerator->Release(); return; }

  DWORD myPid = GetCurrentProcessId();
  int count = 0;
  pSessionEnum->GetCount( &count );

  for( int i = 0; i < count; i++ )
  {
    IAudioSessionControl * pControl = nullptr;
    if( FAILED( pSessionEnum->GetSession( i, &pControl ) ) || !pControl ) continue;

    IAudioSessionControl2 * pControl2 = nullptr;
    hr = pControl->QueryInterface( __uuidof( IAudioSessionControl2 ), (void **) &pControl2 );
    pControl->Release();
    if( FAILED( hr ) || !pControl2 ) continue;

    DWORD pid = 0;
    pControl2->GetProcessId( &pid );

    if( pid != myPid && pid != 0 )
    {
      ISimpleAudioVolume * pVolume = nullptr;
      hr = pControl2->QueryInterface( __uuidof( ISimpleAudioVolume ), (void **) &pVolume );
      if( SUCCEEDED( hr ) && pVolume )
      {
        BOOL alreadyMuted = FALSE;
        pVolume->GetMute( &alreadyMuted );
        if( !alreadyMuted )
        {
          pVolume->SetMute( TRUE, nullptr );
          m_mutedPids.push_back( pid );
        }
        pVolume->Release();
      }
    }

    pControl2->Release();
  }

  pSessionEnum->Release();
  pSessionManager->Release();
  pDevice->Release();
  pEnumerator->Release();
}

void PlaybackEngine::UnmuteOtherApps()
{
  if( m_mutedPids.empty() ) return;

  IMMDeviceEnumerator * pEnumerator = nullptr;
  HRESULT hr = CoCreateInstance( __uuidof( MMDeviceEnumerator ), nullptr, CLSCTX_ALL,
    __uuidof( IMMDeviceEnumerator ), (void **) &pEnumerator );
  if( FAILED( hr ) || !pEnumerator ) { m_mutedPids.clear(); return; }

  IMMDevice * pDevice = nullptr;
  hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &pDevice );
  if( FAILED( hr ) || !pDevice ) { pEnumerator->Release(); m_mutedPids.clear(); return; }

  IAudioSessionManager2 * pSessionManager = nullptr;
  hr = pDevice->Activate( __uuidof( IAudioSessionManager2 ), CLSCTX_ALL, nullptr, (void **) &pSessionManager );
  if( FAILED( hr ) || !pSessionManager ) { pDevice->Release(); pEnumerator->Release(); m_mutedPids.clear(); return; }

  IAudioSessionEnumerator * pSessionEnum = nullptr;
  hr = pSessionManager->GetSessionEnumerator( &pSessionEnum );
  if( FAILED( hr ) || !pSessionEnum ) { pSessionManager->Release(); pDevice->Release(); pEnumerator->Release(); m_mutedPids.clear(); return; }

  int count = 0;
  pSessionEnum->GetCount( &count );

  for( int i = 0; i < count; i++ )
  {
    IAudioSessionControl * pControl = nullptr;
    if( FAILED( pSessionEnum->GetSession( i, &pControl ) ) || !pControl ) continue;

    IAudioSessionControl2 * pControl2 = nullptr;
    hr = pControl->QueryInterface( __uuidof( IAudioSessionControl2 ), (void **) &pControl2 );
    pControl->Release();
    if( FAILED( hr ) || !pControl2 ) continue;

    DWORD pid = 0;
    pControl2->GetProcessId( &pid );

    for( DWORD mutedPid : m_mutedPids )
    {
      if( mutedPid == pid )
      {
        ISimpleAudioVolume * pVolume = nullptr;
        hr = pControl2->QueryInterface( __uuidof( ISimpleAudioVolume ), (void **) &pVolume );
        if( SUCCEEDED( hr ) && pVolume )
        {
          pVolume->SetMute( FALSE, nullptr );
          pVolume->Release();
        }
        break;
      }
    }

    pControl2->Release();
  }

  pSessionEnum->Release();
  pSessionManager->Release();
  pDevice->Release();
  pEnumerator->Release();
  m_mutedPids.clear();
}

void PlaybackEngine::IncreaseComputerVolume( bool testVolumeOnly )
{
  IMMDeviceEnumerator * pEnumerator = nullptr;
  HRESULT hr = CoCreateInstance( __uuidof( MMDeviceEnumerator ), nullptr, CLSCTX_ALL,
    __uuidof( IMMDeviceEnumerator ), (void **) &pEnumerator );
  if( FAILED( hr ) || !pEnumerator ) return;

  IMMDevice * pDevice = nullptr;
  hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &pDevice );
  if( FAILED( hr ) || !pDevice ) { pEnumerator->Release(); return; }

  IAudioEndpointVolume * pEndpointVolume = nullptr;
  hr = pDevice->Activate( __uuidof( IAudioEndpointVolume ), CLSCTX_ALL, nullptr, (void **) &pEndpointVolume );
  if( FAILED( hr ) || !pEndpointVolume ) { pDevice->Release(); pEnumerator->Release(); return; }

  pEndpointVolume->GetMasterVolumeLevelScalar( &m_savedComputerVolume );
  if( !testVolumeOnly )
  {
    pEndpointVolume->SetMasterVolumeLevelScalar( m_computerVolumeBoost, nullptr );
  }

  pEndpointVolume->Release();
  pDevice->Release();
  pEnumerator->Release();
}

void PlaybackEngine::RestoreComputerVolume()
{
  if( m_savedComputerVolume < 0.0f ) return;

  IMMDeviceEnumerator * pEnumerator = nullptr;
  HRESULT hr = CoCreateInstance( __uuidof( MMDeviceEnumerator ), nullptr, CLSCTX_ALL,
    __uuidof( IMMDeviceEnumerator ), (void **) &pEnumerator );
  if( FAILED( hr ) || !pEnumerator ) { m_savedComputerVolume = -1.0f; return; }

  IMMDevice * pDevice = nullptr;
  hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eMultimedia, &pDevice );
  if( FAILED( hr ) || !pDevice ) { pEnumerator->Release(); m_savedComputerVolume = -1.0f; return; }

  IAudioEndpointVolume * pEndpointVolume = nullptr;
  hr = pDevice->Activate( __uuidof( IAudioEndpointVolume ), CLSCTX_ALL, nullptr, (void **) &pEndpointVolume );
  if( FAILED( hr ) || !pEndpointVolume ) { pDevice->Release(); pEnumerator->Release(); m_savedComputerVolume = -1.0f; return; }

  pEndpointVolume->SetMasterVolumeLevelScalar( m_savedComputerVolume, nullptr );

  pEndpointVolume->Release();
  pDevice->Release();
  pEnumerator->Release();
  m_savedComputerVolume = -1.0f;
}

std::vector<PlaybackSegment> PlaybackEngine::ParseText( const std::wstring & text )
{
  std::vector<PlaybackSegment> segments;
  std::wstring delim = SOUND_NOTE_DELIMITER;

  size_t pos = 0;
  while( pos < text.length() )
  {
    size_t start = text.find( delim, pos );
    if( start == std::wstring::npos )
    {
      // remaining speech
      std::wstring segment = text.substr( pos );
      trim( segment );
      if( !segment.empty() )
        segments.push_back( { SegmentType::Speech, segment } );
      break;
    }

    if( start > pos )
    {
      std::wstring segment = text.substr( pos, start - pos );
      trim( segment );
      if( !segment.empty() )
        segments.push_back( { SegmentType::Speech, segment } );
    }

    size_t end = text.find( delim, start + delim.length() );
    if( end == std::wstring::npos )
    {
      // unmatched delimiter - treat rest as speech
      std::wstring segment = text.substr( start );
      trim( segment );
      if( !segment.empty() )
        segments.push_back( { SegmentType::Speech, segment } );
      break;
    }

    // extract filename between delimiters
    std::wstring filename = text.substr( start + delim.length(), end - ( start + delim.length() ) );
    trim( filename );
    if( !filename.empty() )
    {
      // Check if the filename is relative and search in the sound file folders
      if( filename.find( L":" ) == std::wstring::npos && !m_soundFileFolders.empty() )
      {
        ExpandSoundFilePath( filename );

        if( filename.find( L":" ) == std::wstring::npos )
        {
          // File doesn't exist
          filename = m_fallbackSoundFilePath;
        }
      }
      else
      {
        if( GetFileAttributes( filename.c_str() ) == INVALID_FILE_ATTRIBUTES )
        {
          // File doesn't exist
          filename = m_fallbackSoundFilePath;
        }
      }

      if( filename.find_last_of( L"." ) != std::wstring::npos )
      {
        std::wstring ext = filename.substr( filename.find_last_of( L"." ) + 1 );
        for( auto & c : ext ) c = towlower( c );
        if( ext == L"wav" || ext == L"mid" || ext == L"midi" )
        {
          segments.push_back( { SegmentType::SoundWav, filename, ( m_usePlaySoundForWav ? GetWavDuration( filename.c_str() ) : 0 ) } );
        }
        else if( ext == L"mp3" )
        {
          segments.push_back( { SegmentType::SoundMp3, filename } );
        }
      }
    }

    pos = end + delim.length();
  }

  return segments;
}

void PlaybackEngine::PlaySegment( const PlaybackSegment & segment )
{
  if( m_stopRequested ) return;

  switch( segment.type )
  {
    case SegmentType::Speech:
    {
      if( m_pVoice )
      {
        HRESULT hr = m_pVoice->Speak( segment.content.c_str(), SPF_ASYNC | SPF_IS_NOT_XML, nullptr );
        if( SUCCEEDED( hr ) )
        {
          while( !m_stopRequested && !m_shutdown )
          {
            hr = m_pVoice->WaitUntilDone( INTERRUPT_CHECK_INTERVAL_MS );
            if( hr == S_OK ) break;
          }
        }
      }
      break;
    }
    case SegmentType::SoundWav:
    {
      if( m_usePlaySoundForWav )
      {
        // Let's use PlaySound for wav files since it's simpler and has lower latency, but it doesn't provide a way to know when playback finishes, so we have to rely on the duration for timing and interruption
        if( segment.durationMs == 0 )
        {
          // If we don't have a valid duration, we have to play synchronously since we won't know when it finishes
          PlaySound( segment.content.c_str(), NULL, SND_FILENAME | SND_SYNC );
        }
        else
        {
          // PlaySound doesn't provide a way to know when playback finishes, so we use the duration to wait and allow interruption
          int steps = CEILING_DIV( segment.durationMs, INTERRUPT_CHECK_INTERVAL_MS );
          PlaySound( segment.content.c_str(), NULL, SND_FILENAME | SND_ASYNC );

          while( !m_stopRequested && !m_shutdown && steps > 0 )
          {
            std::this_thread::sleep_for( std::chrono::milliseconds( INTERRUPT_CHECK_INTERVAL_MS ) );
            steps--;
          }

          if( m_stopRequested )
            PlaySound( NULL, NULL, SND_PURGE );
        }
      }
      else
      {
        // Use MCI to play wav files so we can know exactly when playback finishes and allow interruption, but it has higher latency and sometimes fails to play certain wav files (probably due to unsupported formats or codecs)
        std::wstring openCmd = L"open \"" + segment.content + L"\" type waveaudio alias soundfile";
        if( mciSendString( openCmd.c_str(), NULL, 0, m_hwndMCI ) == 0 )
        {
          mciSendString( L"play soundfile", NULL, 0, m_hwndMCI );
          while( !m_stopRequested && !m_shutdown )
          {
            std::this_thread::sleep_for( std::chrono::milliseconds( INTERRUPT_CHECK_INTERVAL_MS ) );
            if( IsMciPlaying( L"soundfile" ) == false ) break;
          }
          if( m_stopRequested )
            mciSendString( L"stop soundfile", NULL, 0, m_hwndMCI );
          mciSendString( L"close soundfile", NULL, 0, m_hwndMCI );
        }
      }
      break;
    }
    case SegmentType::SoundMp3:
    {
      std::wstring openCmd = L"open \"" + segment.content + L"\" type mpegvideo alias soundfile";
      if( mciSendString( openCmd.c_str(), NULL, 0, m_hwndMCI ) == 0 )
      {
        mciSendString( L"play soundfile", NULL, 0, m_hwndMCI );
        while( !m_stopRequested && !m_shutdown )
        {
          std::this_thread::sleep_for( std::chrono::milliseconds( INTERRUPT_CHECK_INTERVAL_MS ) );
          if( IsMciPlaying( L"soundfile" ) == false ) break;
        }
        if( m_stopRequested )
          mciSendString( L"stop soundfile", NULL, 0, m_hwndMCI );
        mciSendString( L"close soundfile", NULL, 0, m_hwndMCI );
      }
      break;
    }
  }
}
