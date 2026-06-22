/*
   HidGazeProvider.cpp: IGazeProvider backed by the HID eye-tracker reader.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "HidGazeProvider.h"
#include "SSGazeReader.h"

bool HidGazeProvider::Start()
{
  return SSGazeReader::Instance().Start();
}

void HidGazeProvider::Stop()
{
  SSGazeReader::Instance().Stop();
}

bool HidGazeProvider::IsLive() const
{
  return SSGazeReader::Instance().HasLiveGaze();
}

bool HidGazeProvider::GetSample( GazeSample * out ) const
{
  return SSGazeReader::Instance().GetLatest( out );
}
