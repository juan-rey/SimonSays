/*
   CursorGazeProvider.h: IGazeProvider backed by the Windows mouse cursor.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef CursorGazeProvider_h
#define CursorGazeProvider_h

#include "IGazeProvider.h"

// Tracking source that follows the Windows mouse cursor. Works with every eye
// tracker / head tracker / switch driver that moves the system cursor
// (Irisbond EasyClick, Tobii Dynavox TD Control cursor modes, Windows Eye
// Control cursor modes, OptiKey Mouse-Movement / Magnetic-Cursor, ...). It
// opens no device and is therefore always available and always "live".
class CursorGazeProvider: public IGazeProvider
{
public:
  bool                Start() override { return true; }
  void                Stop() override {}
  bool                IsLive() const override { return true; }
  bool                GetSample( GazeSample * out ) const override;
  const wchar_t * Name() const override { return L"Cursor"; }
};

#endif // CursorGazeProvider_h
