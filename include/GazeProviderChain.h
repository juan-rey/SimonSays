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
// gaze, a commercial Tobii Streams provider, ...) needs no button changes.
//
// Phase A ships only the cursor provider; the HID slot is present but empty.
// Callers honoring a specific dwell mode use GetCursorSample / GetHidSample;
// callers wanting "best available" use GetTrackingSample (HID if live, else
// cursor).
class GazeProviderChain
{
public:
  static GazeProviderChain & Instance();

  void StartAll(); // creates/starts the cursor provider (and HID once present)
  void StopAll();  // stops every provider; safe to call repeatedly

  // Installs the HID provider (ownership transferred). Pass nullptr to remove.
  // Reserved for the HID phase; unused in Phase A.
  void SetHidProvider( std::unique_ptr<IGazeProvider> hid );

  bool HidLive() const; // a HID provider exists and is currently producing data

  bool GetCursorSample( GazeSample * out ) const;   // cursor source (may be null pre-Start)
  bool GetHidSample( GazeSample * out ) const;      // HID source (false when absent)
  bool GetTrackingSample( GazeSample * out ) const; // HID if live, otherwise cursor

private:
  GazeProviderChain() = default;
  GazeProviderChain( const GazeProviderChain & ) = delete;
  GazeProviderChain & operator=( const GazeProviderChain & ) = delete;

  std::unique_ptr<IGazeProvider> m_cursor; // always present after StartAll
  std::unique_ptr<IGazeProvider> m_hid;    // null until the HID phase
  bool m_started = false;
};

#endif // GazeProviderChain_h
