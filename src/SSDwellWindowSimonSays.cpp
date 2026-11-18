/*
   SSDwellWindowSimonSays.cpp: SimonSays' entry point to the shared dwell window.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSDwellWindow.h"
#include "utils.h"

// SimonSays' host for the shared window (dwell.spec.md REQ-F109): its string
// tables in the settings' language, its RTL rule, the Windows accent for
// Reset, and both optional buttons shown - the window behaves exactly as it
// did before the host-neutral entry point existed.
namespace
{
  class SimonSaysDwellHost : public SSDwellWindowHost
  {
  public:
    explicit SimonSaysDwellHost( const std::wstring & language ) : m_language( language ) {}
    // An empty language makes GetLocalizedString resolve the system language.
    std::wstring Text( int stringId ) override { return GetLocalizedString( stringId, m_language ); }
    bool IsRtl() override { return IsLanguageRTL( m_language ); }
    COLORREF DefaultProgressColor() override { return GetAccentColor(); }

  private:
    std::wstring m_language;
  };
}

bool ShowDwellSettingsDialog( HWND owner, HINSTANCE hInstance, Settings & settings )
{
  SSDwellValues values;
  values.modeSelection = settings.dwellModeSelection;
  values.detectedMode = settings.dwellDetectedMode;
  values.timeMs = settings.dwellTimeMs;
  values.toleranceRadius = settings.dwellToleranceRadius;
  values.cooldownMs = settings.dwellCooldownMs;
  values.lockOnMs = settings.dwellLockOnMs;
  values.progressColor = settings.dwellProgressColor;
  values.rearmOnLeave = settings.dwellRearmOnLeave;

  SimonSaysDwellHost host( settings.language );
  if( !ShowDwellSettingsDialog( owner, hInstance, values, host ) )
    return false;

  settings.dwellModeSelection = values.modeSelection;
  settings.dwellDetectedMode = values.detectedMode;
  settings.dwellTimeMs = values.timeMs;
  settings.dwellToleranceRadius = values.toleranceRadius;
  settings.dwellCooldownMs = values.cooldownMs;
  settings.dwellLockOnMs = values.lockOnMs;
  settings.dwellProgressColor = values.progressColor;
  settings.dwellRearmOnLeave = values.rearmOnLeave;
  return true;
}
