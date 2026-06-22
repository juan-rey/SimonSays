/*
   CursorGazeProvider.cpp: IGazeProvider backed by the Windows mouse cursor.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "CursorGazeProvider.h"

bool CursorGazeProvider::GetSample( GazeSample * out ) const
{
  if( !out ) return false;
  POINT pt;
  if( !GetCursorPos( &pt ) )
  {
    out->valid = false;
    return false;
  }
  out->screenPoint = pt;
  out->timestamp = GetTickCount();
  out->valid = true;
  return true;
}
