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
  if( m_tobii )
    m_tobii->Start();
  m_started = true;
}

void GazeProviderChain::StopAll()
{
  if( m_tobii )  m_tobii->Stop();
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

void GazeProviderChain::SetTobiiProvider( std::unique_ptr<IGazeProvider> tobii )
{
  if( m_tobii ) m_tobii->Stop();
  m_tobii = std::move( tobii );
  if( m_tobii && m_started ) m_tobii->Start();
}

bool GazeProviderChain::HidLive() const
{
  return ( m_hid && m_hid->IsLive() ) || ( m_tobii && m_tobii->IsLive() );
}

bool GazeProviderChain::GetCursorSample( GazeSample * out ) const
{
  if( !m_cursor ) return false;
  return m_cursor->GetSample( out );
}

bool GazeProviderChain::GetHidSample( GazeSample * out ) const
{
  // Direct-gaze sample: prefer the raw HID reader, else the Tobii engine.
  if( m_hid && m_hid->IsLive() && m_hid->GetSample( out ) )
    return true;
  if( m_tobii && m_tobii->IsLive() && m_tobii->GetSample( out ) )
    return true;
  return false;
}

bool GazeProviderChain::GetTrackingSample( GazeSample * out ) const
{
  // Prefer a live direct-gaze stream; fall back to the cursor otherwise.
  if( GetHidSample( out ) && out->valid )
    return true;
  return GetCursorSample( out );
}
