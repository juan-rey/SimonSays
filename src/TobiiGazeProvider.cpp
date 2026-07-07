/*
   TobiiGazeProvider.cpp: IGazeProvider backed by the Tobii Stream Engine reader.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "TobiiGazeProvider.h"
#include "SSTobiiGaze.h"

bool TobiiGazeProvider::Start()
{
  return SSTobiiGaze::Instance().Start();
}

void TobiiGazeProvider::Stop()
{
  SSTobiiGaze::Instance().Stop();
}

bool TobiiGazeProvider::IsLive() const
{
  return SSTobiiGaze::Instance().HasLiveGaze();
}

bool TobiiGazeProvider::GetSample( GazeSample * out ) const
{
  return SSTobiiGaze::Instance().GetLatest( out );
}
