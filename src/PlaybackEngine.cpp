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
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "delayimp.lib") // mfplat.dll and mfreadwrite.dll are delay-loaded (see SimonSays.vcxproj)

#define INTERRUPT_CHECK_INTERVAL_MS 100
#define FALLBACK_MP3_FILE L"fallback.mp3"
#define FALLBACK_WAV_FILE L"fallback.wav"

// The worker thread is an STA: COM objects living in it (the DirectShow graph
// behind MCI "mpegvideo", SAPI) receive cross-thread calls as window messages.
// Blocking without dispatching them makes those calls time out (RPC 0x71A) and
// the MPEG audio decoder fail-fasts (0xC0000602), so every wait must pump.
static void PumpMessages()
{
  MSG msg;
  while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
  {
    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }
}

static void WaitWithMessagePump( DWORD ms )
{
  ULONGLONG deadline = GetTickCount64() + ms;
  for( ;; )
  {
    PumpMessages();
    ULONGLONG now = GetTickCount64();
    if( now >= deadline ) break;
    MsgWaitForMultipleObjectsEx( 0, NULL, (DWORD) ( deadline - now ), QS_ALLINPUT, MWMO_INPUTAVAILABLE );
  }
}

// mfplat.dll / mfreadwrite.dll are delay-loaded; calling into them when they
// are missing (Windows N without the Media Feature Pack) would raise an
// exception, so check they can be loaded before using any MF API.
static bool IsMediaFoundationAvailable()
{
  const wchar_t * dlls[] = { L"mfplat.dll", L"mfreadwrite.dll" };
  for( const wchar_t * dll : dlls )
  {
    HMODULE module = LoadLibraryExW( dll, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32 );
    if( !module ) return false;
    FreeLibrary( module );
  }
  return true;
}

bool IsMciPlaying( std::wstring alias )
{
  TCHAR buffer[128] = { 0 };
  // mciCommand: "status mi_alias mode"
  std::wstring mciCommand = L"status " + alias + L" mode";

  if( mciSendString( mciCommand.c_str(), buffer, 128, NULL ) != 0 )
    return false;

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
  // Shared default resource folder first, then the app-data root (permanent
  // read fallback for sounds placed there by earlier versions) — see
  // sound.spec.md SND-F10.
  std::wstring defaultResourceFolder = GetDefaultResourceFolder();
  std::wstring appDataRoot = GetAppDataCustomFolder( APP_NAME );
  if( !defaultResourceFolder.empty() )
    m_soundFileFolders.push_back( defaultResourceFolder );
  if( _wcsicmp( appDataRoot.c_str(), defaultResourceFolder.c_str() ) != 0 )
    m_soundFileFolders.push_back( appDataRoot );
  if( GetWorkingDirectory() != GetExecutableDirectory() ) // avoid duplicates if both are the same
    m_soundFileFolders.push_back( GetWorkingDirectory() );
  m_soundFileFolders.push_back( GetExecutableDirectory() );

  m_fallbackSoundFilePath = FALLBACK_WAV_FILE;
  ExpandSoundFilePath( m_fallbackSoundFilePath );

#ifdef USE_MCI_FOR_MP3
  m_useMediaFoundation = false;
#else
  m_useMediaFoundation = IsMediaFoundationAvailable();
#endif

  // The wav fallback always plays (and stops reliably) through waveOut. Media
  // Foundation is started and warmed up on the worker thread; without it mp3
  // goes through MCI, which needs this warm-up instead.
  if( !m_useMediaFoundation )
  {
    std::wstring mp3File = FALLBACK_MP3_FILE;
    ExpandSoundFilePath( mp3File );
    // ugly workaround to ensure MCI can play mp3 files on the worker thread by pre-opening an mp3 file on the main thread and keeping it open (so the necessary codecs are loaded in memory)
    std::wstring openCmd = L"open \"" + mp3File + L"\" type mpegvideo alias workaround";
    mciSendString( openCmd.c_str(), NULL, 0, m_hwndMCI );
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
  // The worker thread destroys m_hwndMCI during its shutdown, so by the time
  // we get here it's already invalid. The "workaround" alias was opened with
  // m_hwndMCI as the notify window — pass NULL here so we don't hand MCI a
  // destroyed HWND. mciSendString with NULL notify is OK.
  mciSendString( L"close workaround", NULL, 0, NULL ); // no-op when Media Foundation was used
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
  std::lock_guard<std::mutex> lk( m_settingsMutex );
  if( ( m_voiceKey != voiceKey ) || ( m_volume != volume ) || ( m_rate != rate ) )
  {
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
  if( m_pVoice && std::this_thread::get_id() == m_workerThread.get_id() )
    m_pVoice->Speak( nullptr, SPF_PURGEBEFORESPEAK, nullptr );
  // wav files fall back to PlaySound per file even when Media Foundation is used; a no-op when nothing is playing
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
  CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );

  HRESULT hr = CoCreateInstance( CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **) &m_pVoice );
  if( FAILED( hr ) || !m_pVoice )
    m_pVoice = nullptr;

  ApplyVoiceSettings();

  if( m_useMediaFoundation )
  {
    m_mfStarted = SUCCEEDED( MFStartup( MF_VERSION ) );
    if( m_mfStarted )
    {
      std::wstring mp3File = FALLBACK_MP3_FILE;
      ExpandSoundFilePath( mp3File );
      WarmUpMediaFoundation( mp3File );
    }
    else
    {
      m_useMediaFoundation = false; // fall back to MCI for mp3 (without the constructor's pre-open workaround) and PlaySound for wav
    }
  }

  IncreaseAppVolume( true );
  if( m_savedAppVolume >= 0.7f )
  {
    m_useComputerVolume = true; // if we can't increase app volume much, we'll have to increase computer volume instead, but we check this in advance to avoid unnecessarily increasing and restoring computer volume on every playback which could cause issues with some audio drivers
    m_appVolumeBoost = 1.0f;
    IncreaseAppVolume(); // set app volume to 1.0f 
    m_savedAppVolume = -1.0f; // since we won't be using app volume boost, we can reset saved app volume to avoid restoring it later which would set the volume to an undesired level (since we won't be boosting app volume, we don't want to restore it either, we just want to leave it as is)
  }
  else
  {
    m_appVolumeBoost = 1.0f;
    if( m_savedAppVolume >= 0.0f && m_savedAppVolume < 0.3f )
      m_appVolumeBoost = 0.8f;
  }
  RestoreAppVolume();

  if( m_reduceOtherAudioWhenPlaying )
  {
    if( m_useComputerVolume && m_increaseVolumeWhenPlaying )
      m_otherAppsVolumeFactor = AGGRESSIVE_AUDIO_DUCK_FACTOR;
    else
      m_otherAppsVolumeFactor = DEFAULT_AUDIO_DUCK_FACTOR;
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
  bool incomingEmpty;
  // Main loop: wait for text to play, then process and play it
  while( !m_shutdown )
  {

    {
      std::lock_guard<std::mutex> peek( m_incomingMutex );
      incomingEmpty = m_incomingQueue.empty();
    }

    if( !m_shutdown && incomingEmpty && tmpQueue.empty() )
    {
      std::unique_lock<std::mutex> lk( m_incomingMutex );
      // Wait until there's text to play or shutdown is requested, pumping the STA message queue meanwhile (see WaitWithMessagePump)
      while( !m_incomingCV.wait_for( lk, std::chrono::milliseconds( INTERRUPT_CHECK_INTERVAL_MS ), [&] { return !m_incomingQueue.empty() || m_shutdown; } ) )
      {
        lk.unlock();
        PumpMessages();
        lk.lock();
      }
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
      {
        if( m_useComputerVolume )
        {
          IncreaseComputerVolume(); // this method is safe with no running conditions, RestoreComputerVolume will only restore the volume if it was increased by this method, so it's safe to call it multiple times without checking if we already increased computer volume or not
        }
        else
        {
          IncreaseAppVolume(); // this method is safe with no running conditions, RestoreAppVolume will only restore the volume if it was increased by this method, so it's safe to call it multiple times without checking if we already increased app volume or not
        }
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

  if( m_pVoice && !m_warmUpNeeded )// Aholab voices (need to be warmed up before first use) may crash on release
  {
    // Synchronous purge — blocks until SAPI's queue is drained. Even so, SAPI
    // may still have an internal callback in flight; a short WaitUntilDone
    // closes that window before Release() decrements the refcount to zero.
    // Without the wait, Release races with SAPI's internal threads and crashes.
    m_pVoice->Speak( nullptr, SPF_PURGEBEFORESPEAK, nullptr );
    m_pVoice->WaitUntilDone( 2000 ); // grace period; normally returns immediately
    m_pVoice->Release();
    m_pVoice = nullptr;
  }

  if( m_mfStarted )
    MFShutdown();

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

void PlaybackEngine::SetBoardResourceFolder( const std::wstring & folder )
{
  std::lock_guard<std::mutex> lk( m_settingsMutex );
  m_boardResourceFolder = folder;
}

void PlaybackEngine::ExpandSoundFilePath( std::wstring & filename )
{
  // The active board's resource subfolder wins over the shared folders.
  std::wstring boardFolder;
  {
    std::lock_guard<std::mutex> lk( m_settingsMutex );
    boardFolder = m_boardResourceFolder;
  }
  if( !boardFolder.empty() )
  {
    std::wstring fullPath = boardFolder + L"\\" + filename;
    if( FileExists( fullPath ) )
    {
      filename = fullPath;
      return;
    }
  }

  for( const auto & folder : m_soundFileFolders )
  {
    std::wstring fullPath = folder + L"\\" + filename;
    if( FileExists( fullPath ) )
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
      m_otherAppsVolumeFactor = AGGRESSIVE_AUDIO_DUCK_FACTOR;
    else
      m_otherAppsVolumeFactor = DEFAULT_AUDIO_DUCK_FACTOR;
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

void PlaybackEngine::IncreaseComputerVolume()
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
  pEndpointVolume->SetMasterVolumeLevelScalar( min( max( ( 2 * m_savedComputerVolume ) - ( m_savedComputerVolume * m_savedComputerVolume ) + 0.05f, 0.3f ), 1.0f ), nullptr );

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
        if( !FileExists( filename ) )
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
          segments.push_back( { SegmentType::SoundWav, filename } );
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
            hr = m_pVoice->WaitUntilDone( 0 );
            if( hr == S_OK ) break;
            WaitWithMessagePump( INTERRUPT_CHECK_INTERVAL_MS );
          }
          // If we were interrupted, purge the async speech from the worker
          // thread (same COM apartment as m_pVoice was created in). Without
          // this, SAPI keeps speaking even after Stop() was called from main.
          if( ( m_stopRequested || m_shutdown ) && hr != S_OK )
            m_pVoice->Speak( nullptr, SPF_PURGEBEFORESPEAK, nullptr );
        }
      }
      break;
    }
    case SegmentType::SoundWav:
    {
      // waveOut (directly, or after Media Foundation decoding for formats it can't open) knows exactly when playback finishes and stops reliably; PlaySound is the last resort
      if( !PlayWavWithWaveOut( segment.content ) &&
        !( m_useMediaFoundation && PlaySoundWithMediaFoundation( segment.content ) ) )
        PlayWavWithPlaySound( segment.content );
      break;
    }
    case SegmentType::SoundMp3:
    {
      if( m_useMediaFoundation )
        PlaySoundWithMediaFoundation( segment.content );
      else
        PlayMp3WithMci( segment.content );
      break;
    }
  }
}

void PlaybackEngine::PlayWavWithPlaySound( const std::wstring & path )
{
  // PlaySound doesn't provide a way to know when playback finishes, so we have to rely on the duration for timing and interruption
  DWORD durationMs = GetWavDuration( path.c_str() );
  if( durationMs == 0 )
  {
    // If we don't have a valid duration, we have to play synchronously since we won't know when it finishes
    PlaySound( path.c_str(), NULL, SND_FILENAME | SND_SYNC );
  }
  else
  {
    int steps = CEILING_DIV( durationMs, INTERRUPT_CHECK_INTERVAL_MS );
    PlaySound( path.c_str(), NULL, SND_FILENAME | SND_ASYNC );

    while( !m_stopRequested && !m_shutdown && steps > 0 )
    {
      WaitWithMessagePump( INTERRUPT_CHECK_INTERVAL_MS );
      steps--;
    }

    if( m_stopRequested )
      PlaySound( NULL, NULL, SND_PURGE );
  }
}

void PlaybackEngine::PlayMp3WithMci( const std::wstring & path )
{
  std::wstring openCmd = L"open \"" + path + L"\" type mpegvideo alias soundfile";
  if( mciSendString( openCmd.c_str(), NULL, 0, m_hwndMCI ) == 0 )
  {
    mciSendString( L"play soundfile", NULL, 0, m_hwndMCI );
    while( !m_stopRequested && !m_shutdown )
    {
      WaitWithMessagePump( INTERRUPT_CHECK_INTERVAL_MS );
      if( IsMciPlaying( L"soundfile" ) == false ) break;
    }
    if( m_stopRequested )
      mciSendString( L"stop soundfile", NULL, 0, m_hwndMCI );
    mciSendString( L"close soundfile", NULL, 0, m_hwndMCI );
    PumpMessages(); // drain graph-teardown messages before the next open
  }
}

#define WAVEOUT_BUFFER_COUNT 3
#define WAVEOUT_BUFFER_MS 250

// Streams audio to waveOut through a small ring of buffers, so memory stays
// flat regardless of the file length. readData fills up to the given number of
// bytes (always a multiple of nBlockAlign) and returns how many it wrote, 0 at
// the end. The wave mapper converts compressed formats (e.g. MS-ADPCM) through
// ACM. Returns false only if the output device could not be opened for pwfx.
bool PlaybackEngine::StreamToWaveOut( const WAVEFORMATEX * pwfx, const std::function<DWORD( BYTE *, DWORD )> & readData )
{
  if( pwfx->nBlockAlign == 0 ) return false;

  HANDLE hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
  HWAVEOUT hwo = NULL;
  if( !hEvent || waveOutOpen( &hwo, WAVE_MAPPER, pwfx, (DWORD_PTR) hEvent, 0, CALLBACK_EVENT ) != MMSYSERR_NOERROR )
  {
    if( hEvent ) CloseHandle( hEvent );
    return false;
  }

  DWORD bufferBytes = (DWORD) ( ( (ULONGLONG) pwfx->nAvgBytesPerSec * WAVEOUT_BUFFER_MS ) / 1000 );
  bufferBytes -= bufferBytes % pwfx->nBlockAlign;
  if( bufferBytes == 0 ) bufferBytes = pwfx->nBlockAlign;

  std::vector<BYTE> buffers[WAVEOUT_BUFFER_COUNT];
  WAVEHDR headers[WAVEOUT_BUFFER_COUNT] = {};
  bool queued[WAVEOUT_BUFFER_COUNT] = {};
  int queuedCount = 0;

  auto queueBuffer = [&]( int i )
  {
    buffers[i].resize( bufferBytes );
    DWORD bytes = readData( buffers[i].data(), bufferBytes );
    if( bytes == 0 ) return;
    headers[i] = {};
    headers[i].lpData = (LPSTR) buffers[i].data();
    headers[i].dwBufferLength = bytes;
    if( waveOutPrepareHeader( hwo, &headers[i], sizeof( WAVEHDR ) ) != MMSYSERR_NOERROR ) return;
    if( waveOutWrite( hwo, &headers[i], sizeof( WAVEHDR ) ) != MMSYSERR_NOERROR )
    {
      waveOutUnprepareHeader( hwo, &headers[i], sizeof( WAVEHDR ) );
      return;
    }
    queued[i] = true;
    queuedCount++;
  };

  for( int i = 0; i < WAVEOUT_BUFFER_COUNT; i++ )
    queueBuffer( i );

  while( queuedCount > 0 )
  {
    if( m_stopRequested || m_shutdown )
    {
      waveOutReset( hwo ); // marks every queued buffer as done
      break;
    }

    // Wake on buffer completion, a message for this STA, or the interrupt check interval
    MsgWaitForMultipleObjectsEx( 1, &hEvent, INTERRUPT_CHECK_INTERVAL_MS, QS_ALLINPUT, MWMO_INPUTAVAILABLE );
    PumpMessages();

    for( int i = 0; i < WAVEOUT_BUFFER_COUNT; i++ )
    {
      if( queued[i] && ( headers[i].dwFlags & WHDR_DONE ) )
      {
        waveOutUnprepareHeader( hwo, &headers[i], sizeof( WAVEHDR ) );
        queued[i] = false;
        queuedCount--;
        if( !m_stopRequested && !m_shutdown )
          queueBuffer( i );
      }
    }
  }

  for( int i = 0; i < WAVEOUT_BUFFER_COUNT; i++ )
  {
    if( queued[i] )
      waveOutUnprepareHeader( hwo, &headers[i], sizeof( WAVEHDR ) );
  }

  waveOutClose( hwo );
  CloseHandle( hEvent );
  return true;
}

// Plays a RIFF/WAVE file by streaming its data chunk, in its own format, to
// waveOut. Needs no Media Foundation, so it also works on Windows N. Returns
// false if the file can't be parsed or waveOut can't play its format.
bool PlaybackEngine::PlayWavWithWaveOut( const std::wstring & path )
{
  HMMIO hmmio = mmioOpenW( (LPWSTR) path.c_str(), NULL, MMIO_READ | MMIO_ALLOCBUF );
  if( !hmmio ) return false;

  bool played = false;
  MMCKINFO ckRiff = { 0 };
  ckRiff.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );
  MMCKINFO ckFmt = { 0 };
  ckFmt.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
  if( mmioDescend( hmmio, &ckRiff, NULL, MMIO_FINDRIFF ) == MMSYSERR_NOERROR &&
    mmioDescend( hmmio, &ckFmt, &ckRiff, MMIO_FINDCHUNK ) == MMSYSERR_NOERROR &&
    ckFmt.cksize >= sizeof( PCMWAVEFORMAT ) && ckFmt.cksize <= 0xFFFF )
  {
    // Keep the whole fmt chunk: compressed formats carry extra bytes after
    // WAVEFORMATEX. A bare PCMWAVEFORMAT chunk gets cbSize = 0 from the zero fill.
    std::vector<BYTE> format( max( (size_t) ckFmt.cksize, sizeof( WAVEFORMATEX ) ), 0 );
    MMCKINFO ckData = { 0 };
    ckData.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
    if( mmioRead( hmmio, (HPSTR) format.data(), (LONG) ckFmt.cksize ) == (LONG) ckFmt.cksize &&
      mmioAscend( hmmio, &ckFmt, 0 ) == MMSYSERR_NOERROR &&
      mmioDescend( hmmio, &ckData, &ckRiff, MMIO_FINDCHUNK ) == MMSYSERR_NOERROR )
    {
      DWORD remaining = ckData.cksize;
      played = StreamToWaveOut( (const WAVEFORMATEX *) format.data(), [&]( BYTE * buffer, DWORD capacity ) -> DWORD
        {
          LONG bytes = mmioRead( hmmio, (HPSTR) buffer, (LONG) min( capacity, remaining ) );
          if( bytes <= 0 ) return 0;
          remaining -= (DWORD) bytes;
          return (DWORD) bytes;
        } );
    }
  }

  mmioClose( hmmio, 0 );
  return played;
}

// Opens an audio file with a Media Foundation source reader configured to
// output PCM. On success returns the reader and the PCM format (free *ppwfx
// with CoTaskMemFree).
static bool OpenPcmSourceReader( const std::wstring & path, IMFSourceReader ** ppReader, WAVEFORMATEX ** ppwfx )
{
  *ppReader = nullptr;
  *ppwfx = nullptr;

  IMFSourceReader * reader = nullptr;
  if( FAILED( MFCreateSourceReaderFromURL( path.c_str(), nullptr, &reader ) ) )
    return false;

  reader->SetStreamSelection( (DWORD) MF_SOURCE_READER_ALL_STREAMS, FALSE );
  reader->SetStreamSelection( (DWORD) MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE );

  IMFMediaType * partialType = nullptr;
  HRESULT hr = MFCreateMediaType( &partialType );
  if( SUCCEEDED( hr ) )
  {
    partialType->SetGUID( MF_MT_MAJOR_TYPE, MFMediaType_Audio );
    partialType->SetGUID( MF_MT_SUBTYPE, MFAudioFormat_PCM );
    hr = reader->SetCurrentMediaType( (DWORD) MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, partialType );
    partialType->Release();
  }

  IMFMediaType * pcmType = nullptr;
  if( SUCCEEDED( hr ) )
    hr = reader->GetCurrentMediaType( (DWORD) MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pcmType );

  UINT32 cbFormat = 0;
  if( SUCCEEDED( hr ) )
  {
    hr = MFCreateWaveFormatExFromMFMediaType( pcmType, ppwfx, &cbFormat );
    pcmType->Release();
  }

  if( FAILED( hr ) || !*ppwfx || ( *ppwfx )->nBlockAlign == 0 )
  {
    if( *ppwfx ) { CoTaskMemFree( *ppwfx ); *ppwfx = nullptr; }
    reader->Release();
    return false;
  }

  *ppReader = reader;
  return true;
}

// The first decode in a process loads the Media Foundation DLLs (~200 ms);
// do it once at startup so the first real sound plays without that delay.
void PlaybackEngine::WarmUpMediaFoundation( const std::wstring & mp3File )
{
  IMFSourceReader * reader = nullptr;
  WAVEFORMATEX * pwfx = nullptr;
  if( !OpenPcmSourceReader( mp3File, &reader, &pwfx ) )
    return;

  DWORD flags = 0;
  IMFSample * sample = nullptr;
  reader->ReadSample( (DWORD) MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample );
  if( sample ) sample->Release();

  CoTaskMemFree( pwfx );
  reader->Release();
}

// Decodes the file (mp3, or a wav format waveOut can't open directly) to PCM
// with Media Foundation and streams it to waveOut. Returns false if the file
// could not be decoded or the output device could not be opened.
bool PlaybackEngine::PlaySoundWithMediaFoundation( const std::wstring & path )
{
  if( !m_mfStarted ) return false;

  IMFSourceReader * reader = nullptr;
  WAVEFORMATEX * pwfx = nullptr;
  if( !OpenPcmSourceReader( path, &reader, &pwfx ) )
    return false;

  // Decoded samples rarely match the buffer size, so keep what didn't fit
  std::vector<BYTE> pending;
  size_t pendingOffset = 0;
  bool endOfStream = false;

  bool played = StreamToWaveOut( pwfx, [&]( BYTE * buffer, DWORD capacity ) -> DWORD
  {
    DWORD filled = 0;
    while( filled < capacity )
    {
      if( pendingOffset < pending.size() )
      {
        size_t n = min( pending.size() - pendingOffset, (size_t) ( capacity - filled ) );
        memcpy( buffer + filled, pending.data() + pendingOffset, n );
        filled += (DWORD) n;
        pendingOffset += n;
        continue;
      }
      if( endOfStream ) break;

      DWORD flags = 0;
      IMFSample * sample = nullptr;
      HRESULT hr = reader->ReadSample( (DWORD) MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample );
      if( FAILED( hr ) || ( flags & ( MF_SOURCE_READERF_ENDOFSTREAM | MF_SOURCE_READERF_ERROR ) ) )
        endOfStream = true;

      pending.clear();
      pendingOffset = 0;
      if( sample )
      {
        IMFMediaBuffer * mediaBuffer = nullptr;
        if( SUCCEEDED( sample->ConvertToContiguousBuffer( &mediaBuffer ) ) )
        {
          BYTE * data = nullptr;
          DWORD length = 0;
          if( SUCCEEDED( mediaBuffer->Lock( &data, nullptr, &length ) ) )
          {
            pending.assign( data, data + length );
            mediaBuffer->Unlock();
          }
          mediaBuffer->Release();
        }
        sample->Release();
      }
    }
    return filled;
  } );

  CoTaskMemFree( pwfx );
  reader->Release();
  return played;
}
