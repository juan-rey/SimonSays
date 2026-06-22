/*
   HidGazeProvider.h: IGazeProvider backed by the HID eye-tracker reader.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef HidGazeProvider_h
#define HidGazeProvider_h

#include "IGazeProvider.h"

// Tracking source that reads gaze straight from a HID eye-tracker via
// SSGazeReader. Used for two-step / no-cursor tool modes. IsLive() is true only
// while fresh samples are arriving, so the chain falls back to the cursor when
// the device is absent, removed, or producing nothing.
class HidGazeProvider : public IGazeProvider
{
public:
  bool                Start() override;
  void                Stop() override;
  bool                IsLive() const override;
  bool                GetSample( GazeSample * out ) const override;
  const wchar_t *     Name() const override { return L"HID gaze"; }
};

#endif // HidGazeProvider_h
