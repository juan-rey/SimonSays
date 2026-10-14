/*
   PlaybackEngine.h: PlaybackEngine class header file.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef PlaybackEngine_h
#define PlaybackEngine_h

#include "stdafx.h"
#include <sapi.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

// Sound playback backends. WAV files stream their own data straight to waveOut
// (the wave mapper converts e.g. MS-ADPCM), then fall back to Media Foundation
// decoding and finally PlaySound. MP3 files are decoded with Media Foundation
// and streamed to waveOut; when Media Foundation is unavailable (e.g. Windows N
// editions without the Media Feature Pack; mfplat/mfreadwrite are delay-loaded
// so the app still starts there) MP3 falls back to MCI "mpegvideo"
// (DirectShow). Define USE_MCI_FOR_MP3 to always use MCI for MP3.
// Note: on some Windows builds the DirectShow MPEG audio decoder fail-fasts
// (0xC0000602) on the second graph created in a process, which is why MCI is
// no longer the default.
//#define USE_MCI_FOR_MP3

#define DEFAULT_AUDIO_DUCK_FACTOR 0.25f
#define AGGRESSIVE_AUDIO_DUCK_FACTOR 0.16f

enum class SegmentType
{
  Speech,
  SoundWav,
  SoundMp3
};

struct IMMDeviceEnumerator;
struct IMMDevice;

// An active audio output (render) endpoint, for the Settings "Sound output" list.
struct AudioOutputDevice
{
  std::wstring id;   // Core Audio endpoint ID (stable across plug/unplug)
  std::wstring name; // friendly name, e.g. "CABLE Input (VB-Audio Virtual Cable)"
  bool isVirtualCable = false;
};

struct PlaybackSegment
{
  SegmentType type;
  std::wstring content;
};

class PlaybackEngine
{
public:
  PlaybackEngine( HWND hwndOwner, const std::wstring & voiceKey, int volume, int rate );
  ~PlaybackEngine();

  void QueueText( const std::wstring & text, bool stopPrevious = false );
  void Stop();
  bool IsPlaying() const;

  void SetVoiceSettings( const std::wstring & voiceKey, int volume, int rate );
  void SetAudioDuckingSettings( bool increaseVolume, bool reduceOtherAudio );
  // Board resource subfolder (title-derived, may be empty): searched before
  // m_soundFileFolders when expanding relative sound file names.
  void SetBoardResourceFolder( const std::wstring & folder );
  // Output device for speech and sounds (Core Audio endpoint ID; empty = Windows default).
  // Applied at the start of each playback run (sound.spec.md SND-F70..F72).
  void SetOutputDevice( const std::wstring & endpointId );

  // Active output devices: virtual audio cables first, then the rest in Windows' order.
  static std::vector<AudioOutputDevice> EnumerateOutputDevices();
  // Maps an endpoint ID to its current waveOut device number; false if no such device is present.
  static bool FindWaveOutDeviceId( const std::wstring & endpointId, UINT & waveOutId );

private:
  void WorkerThread();
  static LRESULT CALLBACK HiddenWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
  std::vector<PlaybackSegment> ParseText( const std::wstring & text );
  void PlaySegment( const PlaybackSegment & segment );
  void InterruptCurrentPlayback();
  void ApplyVoiceSettings();
  void ExpandSoundFilePath( std::wstring & filename );
  void ReduceOtherAppsVolume();
  void RestoreOtherAppsVolume();
  void IncreaseAppVolume( bool testVolumeOnly = false );
  void RestoreAppVolume();
  void MuteOtherApps();
  void UnmuteOtherApps();
  void IncreaseComputerVolume();
  void RestoreComputerVolume();
  void WarmUpMediaFoundation( const std::wstring & mp3File );
  bool StreamToWaveOut( const WAVEFORMATEX * pwfx, const std::function<DWORD( BYTE *, DWORD )> & readData );
  bool PlayWavWithWaveOut( const std::wstring & path );
  bool PlaySoundWithMediaFoundation( const std::wstring & path ); // mp3, and wav formats waveOut can't open
  void PlayMp3WithMci( const std::wstring & path );
  void PlayWavWithPlaySound( const std::wstring & path );
  bool m_useMediaFoundation = false; // decided in the constructor, cleared on the worker if MFStartup fails
  bool m_mfStarted = false;

  // Output device routing. m_outputDeviceId is set from the main thread
  // (guarded by m_settingsMutex); the rest is worker-thread only.
  void ResolveOutputDevice();
  HRESULT GetPlaybackEndpoint( IMMDeviceEnumerator * pEnumerator, IMMDevice ** ppDevice );
  std::wstring m_outputDeviceId;
  std::wstring m_activeEndpointId;              // resolved endpoint; empty = Windows default
  UINT m_waveOutDeviceId = (UINT) -1;           // WAVE_MAPPER
  UINT m_sapiWaveOutDeviceId = (UINT) -2;       // device SAPI output is set to; -2 = not set yet
  bool m_outputDeviceMissingReported = false;

  HWND m_hwndOwner;
  HWND m_hwndMCI = nullptr;
  bool m_useHiddenWindowForMCI = false; // Set to true if using a hidden window for MCI notifications, false if using a message-only window
  ISpVoice * m_pVoice = nullptr;

  std::thread m_workerThread;
  std::atomic<bool> m_shutdown{ false };
  std::atomic<bool> m_stopRequested{ false };
  std::atomic<bool> m_isPlaying{ false };

  // Incoming queue: raw text strings submitted by the caller
  std::mutex m_incomingMutex;
  std::condition_variable m_incomingCV;
  std::queue<std::wstring> m_incomingQueue;

  // Playing queue: preprocessed segments ready to play
  std::mutex m_playingMutex;
  std::queue<PlaybackSegment> m_playingQueue;

  // Voice settings (updated from main thread, applied on worker thread)
  std::mutex m_settingsMutex;
  std::wstring m_voiceKey;
  int m_volume = 100;
  int m_rate = 0;
  bool m_warmUpNeeded = false;
  std::atomic<bool> m_settingsChanged{ false };

  // Folders to search for sound files (for relative paths in the text).
  // m_soundFileFolders is built once in the constructor and then immutable;
  // the board folder changes at runtime and is guarded by m_settingsMutex
  // (updated from the main thread, read on the worker thread).
  std::vector<std::wstring> m_soundFileFolders;
  std::wstring m_boardResourceFolder;
  std::wstring m_fallbackSoundFilePath;

  // Audio ducking settings (controlled from main thread, read on worker thread)
  std::atomic<bool> m_increaseVolumeWhenPlaying{ false };
  std::atomic<bool> m_reduceOtherAudioWhenPlaying{ false };

  // Saved volume state for restore
  float m_savedAppVolume = -1.0f;
  std::vector<std::pair<DWORD, float>> m_savedOtherVolumes;
  std::vector<std::pair<std::wstring, float>> m_savedEndpointVolumes; // endpoint ID -> device volume before the boost
  std::vector<DWORD> m_mutedPids;
  float m_appVolumeBoost = 1.0f;
  float m_otherAppsVolumeFactor = DEFAULT_AUDIO_DUCK_FACTOR;
  bool m_useComputerVolume = false;
};

#endif // PlaybackEngine_h