/*
   IGazeProvider.h: tracking-source provider interface for dwell-click.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef IGazeProvider_h
#define IGazeProvider_h

#include "stdafx.h"

// A single tracking point fed to the dwell engine. Screen coordinates are
// virtual-desktop pixels (multi-monitor aware); validity reports whether the
// provider currently has a meaningful point to offer.
struct GazeSample
{
  POINT screenPoint = { 0, 0 }; // virtual-desktop pixels
  DWORD timestamp   = 0;        // GetTickCount when produced
  bool  valid       = false;
};

// A source of tracking points. Implemented initially by CursorGazeProvider
// (the Windows mouse cursor) and, in a later phase, by HidGazeProvider (raw
// HID eye-tracker) and others. SSButton never talks to providers directly; it
// goes through GazeProviderChain so new backends require no button changes.
class IGazeProvider
{
public:
  virtual ~IGazeProvider() = default;

  virtual bool                Start() = 0;             // begin producing samples
  virtual void                Stop() = 0;              // stop and release resources
  virtual bool                IsLive() const = 0;      // producing valid data right now
  virtual bool                GetSample( GazeSample * out ) const = 0; // latest point
  virtual const wchar_t *     Name() const = 0;        // for status UI / logging
};

#endif // IGazeProvider_h
