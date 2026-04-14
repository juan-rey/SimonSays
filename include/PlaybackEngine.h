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


#define OTHER_APPS_VOLUME_FACTOR 0.25f
#define APP_VOLUME_BOOST 1.0f
#define COMPUTER_VOLUME_BOOST 0.8f

enum class SegmentType
{
  Speech,
  SoundWav,
  SoundMp3
};

struct PlaybackSegment
{
  SegmentType type;
  std::wstring content;
  DWORD durationMs = 0; // For sound segments, store the duration in milliseconds (used for timing and interruption)
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
  void IncreaseComputerVolume( bool testVolumeOnly = false );
  void RestoreComputerVolume();

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
  bool m_usePlaySoundForWav = true;
  std::atomic<bool> m_settingsChanged{ false };

  // Folders to search for sound files (for relative paths in the text)
  std::vector<std::wstring> m_soundFileFolders;
  std::wstring m_fallbackSoundFilePath;

  // Audio ducking settings (controlled from main thread)
  bool m_increaseVolumeWhenPlaying = false;
  bool m_reduceOtherAudioWhenPlaying = false;

  // Saved volume state for restore
  float m_savedAppVolume = -1.0f;
  std::vector<std::pair<DWORD, float>> m_savedOtherVolumes;
  float m_savedComputerVolume = -1.0f;
  std::vector<DWORD> m_mutedPids;
  float m_appVolumeBoost = APP_VOLUME_BOOST;
  float m_computerVolumeBoost = COMPUTER_VOLUME_BOOST;
  float m_otherAppsVolumeFactor = OTHER_APPS_VOLUME_FACTOR;
  bool m_useComputerVolume = false;
};

#endif // PlaybackEngine_h