/*
   GazeProviderChain.cpp: ordered set of tracking sources queried by the dwell engine.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "GazeProviderChain.h"
#include "CursorGazeProvider.h"

GazeProviderChain & GazeProviderChain::Instance()
{
  static GazeProviderChain s_chain;
  return s_chain;
}

void GazeProviderChain::StartAll()
{
  if( m_started ) return;
  if( !m_cursor )
    m_cursor = std::make_unique<CursorGazeProvider>();
  m_cursor->Start();
  if( m_hid )
    m_hid->Start();
  m_started = true;
}

void GazeProviderChain::StopAll()
{
  if( m_hid )    m_hid->Stop();
  if( m_cursor ) m_cursor->Stop();
  m_started = false;
}

void GazeProviderChain::SetHidProvider( std::unique_ptr<IGazeProvider> hid )
{
  if( m_hid ) m_hid->Stop();
  m_hid = std::move( hid );
  if( m_hid && m_started ) m_hid->Start();
}

bool GazeProviderChain::HidLive() const
{
  return m_hid && m_hid->IsLive();
}

bool GazeProviderChain::GetCursorSample( GazeSample * out ) const
{
  if( !m_cursor ) return false;
  return m_cursor->GetSample( out );
}

bool GazeProviderChain::GetHidSample( GazeSample * out ) const
{
  if( !m_hid || !m_hid->IsLive() ) return false;
  return m_hid->GetSample( out );
}

bool GazeProviderChain::GetTrackingSample( GazeSample * out ) const
{
  // Prefer a live HID stream; fall back to the cursor otherwise.
  if( m_hid && m_hid->IsLive() && m_hid->GetSample( out ) && out->valid )
    return true;
  return GetCursorSample( out );
}
