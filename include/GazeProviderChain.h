/*
   GazeProviderChain.h: ordered set of tracking sources queried by the dwell engine.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef GazeProviderChain_h
#define GazeProviderChain_h

#include "IGazeProvider.h"

// Process-wide registry of tracking sources. SSButton asks the chain for a
// point rather than talking to providers directly, so adding a backend (HID
// gaze, the Tobii Stream Engine provider, ...) needs no button changes.
//
// Two direct-gaze slots exist besides the cursor: the HID reader and the Tobii
// Stream Engine. "Hid" in this API means *direct gaze* (either slot): callers
// like the detector, the gaze router, and the calibration probes only care
// that gaze arrives without the cursor moving, not which backend supplied it.
// Callers honoring a specific dwell mode use GetCursorSample / GetHidSample;
// callers wanting "best available" use GetTrackingSample (direct gaze if live,
// else cursor).
class GazeProviderChain
{
public:
  static GazeProviderChain & Instance();

  void StartAll(); // creates/starts the cursor provider (and installed backends)
  void StopAll();  // stops every provider; safe to call repeatedly

  // Installs the HID provider (ownership transferred). Pass nullptr to remove.
  void SetHidProvider( std::unique_ptr<IGazeProvider> hid );

  // Installs the Tobii Stream Engine provider (ownership transferred). Pass
  // nullptr to remove. Serves Tobii devices whose HID interface is ACL-locked.
  void SetTobiiProvider( std::unique_ptr<IGazeProvider> tobii );

  bool HidLive() const; // some direct-gaze provider (HID or Tobii) is producing data

  bool GetCursorSample( GazeSample * out ) const;   // cursor source (may be null pre-Start)
  bool GetHidSample( GazeSample * out ) const;      // direct-gaze source (false when none live)
  bool GetTrackingSample( GazeSample * out ) const; // direct gaze if live, otherwise cursor

private:
  GazeProviderChain() = default;
  GazeProviderChain( const GazeProviderChain & ) = delete;
  GazeProviderChain & operator=( const GazeProviderChain & ) = delete;

  std::unique_ptr<IGazeProvider> m_cursor; // always present after StartAll
  std::unique_ptr<IGazeProvider> m_hid;    // raw HID reader (e.g. Irisbond Hiru)
  std::unique_ptr<IGazeProvider> m_tobii;  // Tobii Stream Engine (4C / PCEye5 / ET5)
  bool m_started = false;
};

#endif // GazeProviderChain_h
