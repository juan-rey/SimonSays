/*
   TobiiGazeProvider.h: IGazeProvider backed by the Tobii Stream Engine reader.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef TobiiGazeProvider_h
#define TobiiGazeProvider_h

#include "IGazeProvider.h"

// Tracking source that reads gaze from Tobii trackers via SSTobiiGaze (the
// runtime-loaded Stream Engine). Serves devices whose HID interface is
// ACL-protected (4C, PCEye5, Eye Tracker 5). IsLive() is true only while fresh
// samples arrive, so the chain falls back to the HID reader or the cursor when
// the engine, DLL, or device is unavailable.
class TobiiGazeProvider: public IGazeProvider
{
public:
  bool                Start() override;
  void                Stop() override;
  bool                IsLive() const override;
  bool                GetSample( GazeSample * out ) const override;
  const wchar_t * Name() const override { return L"Tobii Stream Engine"; }
};

#endif // TobiiGazeProvider_h
