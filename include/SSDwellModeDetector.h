/*
   SSDwellModeDetector.h: intelligent selection of the dwell operating mode.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.

   Adapted from the reference design in docs/ssdwell-mode-detector.txt. The
   SSDwellMode / SSDwellModeSelection enums are reused from SSButton.h rather
   than redefined here. Header-only: the singletons use Meyers statics.
*/
#ifndef SSDwellModeDetector_h
#define SSDwellModeDetector_h

#include "SSButton.h" // SSDwellMode, SSDwellModeSelection
#include <array>
#include <cmath>
#include <cstdint>

// ============================================================================
//  MouseMotionClassifier
//  Samples the cursor at a fixed rate and classifies the likely driver of its
//  motion as hand vs. gaze, from kinematic features over a sliding window.
//  DORMANT since spec v1.6: no decision consumes the verdict anymore (live
//  direct gaze wins unconditionally) and nothing samples the cursor. Kept for
//  potential diagnostics / future tuning work.
// ============================================================================
class MouseMotionClassifier
{
public:
  struct Verdict
  {
    float gazeLikelihood; // 0..1 — how "gaze-like" the motion looks
    bool  cursorFrozen;   // true if the cursor barely moved at all
    bool  enoughData;     // false until the window has filled
  };

  void Reset()
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    m_count = 0;
    m_head = 0;
  }

  // Call at a fixed cadence (e.g. 30-60 Hz) with the current cursor position
  // and a millisecond timestamp (GetTickCount).
  void AddSample( POINT cursor, DWORD timestampMs )
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    Sample & s = m_ring[m_head];
    s.pt = cursor;
    s.t = timestampMs;
    m_head = ( m_head + 1 ) % kWindow;
    if( m_count < kWindow ) ++m_count;
  }

  Verdict Classify() const
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    Verdict v{ 0.0f, false, false };
    if( m_count < kMinSamples ) return v;
    v.enoughData = true;

    const int n = m_count;
    auto at = [&]( int i ) -> const Sample &
      {
        int idx = ( m_head - m_count + i + kWindow ) % kWindow;
        return m_ring[idx];
      };

    int    teleports = 0;    // implausibly fast single-frame jumps
    int    movingFrames = 0; // frames with any motion
    int    stillFrames = 0;  // frames essentially static
    double sumSpeed = 0.0;
    double peakSpeed = 0.0;
    double jitterAccum = 0.0; // residual motion during slow phases
    int    jitterFrames = 0;

    for( int i = 1; i < n; ++i )
    {
      const Sample & a = at( i - 1 );
      const Sample & b = at( i );
      const double dt = std::max<DWORD>( 1, b.t - a.t ) / 1000.0; // seconds
      const double dx = double( b.pt.x - a.pt.x );
      const double dy = double( b.pt.y - a.pt.y );
      const double dist = std::sqrt( dx * dx + dy * dy );
      const double speed = dist / dt; // px/s

      sumSpeed += speed;
      peakSpeed = std::max<double>( peakSpeed, speed );

      if( dist < kStillPx ) ++stillFrames;
      else                  ++movingFrames;

      // A jump beyond kTeleportPx within a single ~16 ms frame is far too fast
      // for a hand: it is a saccade or a two-step teleport.
      if( dist > kTeleportPx && dt < 0.05 ) ++teleports;

      // During slow phases, accumulate residual jitter. Hands holding still
      // produce ~0; gaze fixation always shimmers.
      if( speed < kFixationSpeed )
      {
        jitterAccum += dist;
        ++jitterFrames;
      }
    }

    if( movingFrames <= n / 20 ) v.cursorFrozen = true;

    const double avgSpeed = sumSpeed / std::max<int>( 1, n - 1 );
    const double fixJitter = jitterFrames ? jitterAccum / jitterFrames : 0.0;
    const double teleRatio = double( teleports ) / std::max<int>( 1, n - 1 );

    // --- Heuristic scoring ----------------------------------------------------
    // Each cue contributes evidence toward "gaze-driven". Tuned for typical
    // 30-120 Hz trackers; thresholds are collected below for easy retuning.
    float score = 0.0f;

    if( teleRatio > 0.02 )       score += 0.40f;
    else if( teleRatio > 0.005 ) score += 0.20f;

    if( fixJitter > kJitterHigh )     score += 0.30f;
    else if( fixJitter > kJitterLow ) score += 0.15f;

    if( peakSpeed > 4000.0 && avgSpeed < 400.0 )      score += 0.30f;
    else if( peakSpeed > 2500.0 && avgSpeed < 600.0 ) score += 0.15f;

    v.gazeLikelihood = std::min<float>( 1.0f, score );
    return v;
  }

private:
  struct Sample { POINT pt; DWORD t; };

  // NOTE: these thresholds are unvalidated estimates (see Dwell.md testing
  // notes). They live here, in one place, so a calibration experiment can tune
  // them without touching the scoring logic.
  static constexpr int    kWindow = 120;        // ~2-4 s of samples
  static constexpr int    kMinSamples = 45;     // need ~0.75-1.5 s before judging
  static constexpr double kStillPx = 1.5;       // below this = "still" frame
  static constexpr double kTeleportPx = 150.0;  // single-frame jump = saccade
  static constexpr double kFixationSpeed = 300.0; // px/s threshold for fixation
  static constexpr double kJitterLow = 0.8;     // px residual per slow frame
  static constexpr double kJitterHigh = 2.0;

  mutable std::mutex          m_mutex;
  std::array<Sample, kWindow> m_ring{};
  int                         m_head = 0;
  int                         m_count = 0;
};

// ============================================================================
//  Calibration outcome — produced by the two-probe ground-truth mechanism
//  (wired up in a later phase). Authoritative while fresh.
// ============================================================================
enum class CalibrationOutcome
{
  None,
  ConfirmedHidGaze,      // gaze probe hit, HID stream valid -> HidDwell
  ConfirmedCursorIsGaze, // gaze probe hit via cursor -> MouseDwell
  ConfirmedMouseIsHand,  // mouse probe hit with hand-like motion -> mouse user
};

// ============================================================================
//  SSDwellModeDetector — fuses calibration > clicking tool > direct-gaze
//  liveness > passive presence into a recommended SSDwellMode + confidence.
//  (Cursor kinematics no longer participate — see MouseMotionClassifier note.)
// ============================================================================
class SSDwellModeDetector
{
public:
  static SSDwellModeDetector & Instance()
  {
    static SSDwellModeDetector inst;
    return inst;
  }

  // Drive the classifier. Call from a fixed-rate timer. DORMANT: no caller
  // remains since the kinematic branch left Decide() (spec v1.6).
  void Tick()
  {
    POINT pt;
    if( GetCursorPos( &pt ) )
      m_motion.AddSample( pt, GetTickCount() );
  }

  void ReportCalibration( CalibrationOutcome outcome )
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    m_calibration = outcome;
    m_calibrationTick = GetTickCount();
  }

  struct PassiveSignals
  {
    bool hidDevicePresent = false;
    bool externalToolRunning = false;
    bool windowsEyeControl = false;
    bool toolKnownToClick = false; // tool issues its own clicks
    bool AnyPresence() const { return hidDevicePresent || externalToolRunning || windowsEyeControl; }
  };

  void UpdatePassive( const PassiveSignals & s )
  {
    std::lock_guard<std::mutex> lock( m_mutex );
    m_passive = s;
  }

  void SetHidLive( bool live ) { m_hidLive.store( live ); }

  struct Decision
  {
    SSDwellMode    mode;
    float          confidence; // 0..1
    const wchar_t * reason;    // for status UI / logging
  };

  Decision Decide() const
  {
    std::lock_guard<std::mutex> lock( m_mutex );

    // 1) Calibration is authoritative while it is fresh.
    const DWORD now = GetTickCount();
    const bool calibFresh =
      ( m_calibration != CalibrationOutcome::None ) &&
      ( now - m_calibrationTick < kCalibrationValidMs );

    if( calibFresh )
    {
      switch( m_calibration )
      {
        case CalibrationOutcome::ConfirmedHidGaze:
          return { SSDwellMode::HidDwell, 1.0f, L"Calibrated: HID gaze" };
        case CalibrationOutcome::ConfirmedCursorIsGaze:
          return { SSDwellMode::MouseDwell, 1.0f, L"Calibrated: gaze-driven cursor" };
        case CalibrationOutcome::ConfirmedMouseIsHand:
          return { SSDwellMode::MouseDwell, 0.9f, L"Calibrated: manual mouse" };
        default: break;
      }
    }

    // 2) A tool that already clicks on our buttons -> stay out of the way.
    if( m_passive.toolKnownToClick )
      return { SSDwellMode::ExternalClick, 0.8f, L"External tool issues clicks" };

    // 3) Nothing eye-tracking-related is around (no HID device, no known tool,
    //    no Windows Eye Control, no live HID stream) -> dwell stays off. This
    //    keeps Auto from ever enabling dwell for a plain mouse user from
    //    kinematics alone, and lets the caller suspend cursor sampling.
    if( !m_hidLive.load() && !m_passive.AnyPresence() )
      return { SSDwellMode::Off, 0.9f, L"No eye tracker or eye-control software" };

    // 4) Live direct gaze (HID reader or Tobii engine) -> dwell follows the
    //    gaze itself, unconditionally. Even when a tool also drives the cursor
    //    it tracks the same gaze; the old kinematic "prefer Mouse dwell"
    //    refinement misfired in the field (PCEye5, 2026-07-06): burst cursor
    //    warps from TD Control / WEC / hand-mouse use scored "gaze-like" and
    //    steered Auto onto a mostly-parked cursor.
    if( m_hidLive.load() )
      return { SSDwellMode::HidDwell, 0.9f, L"Direct gaze stream live" };

    // 5) Presence without a HID stream (device, known tool, or Windows Eye
    //    Control) -> enable Mouse dwell. Presence alone is enough; cursor
    //    kinematics no longer veto (the tracker's software drives the cursor
    //    in the common case, and Mouse dwell is harmless if it doesn't).
    return { SSDwellMode::MouseDwell, 0.6f, L"Eye tracker or eye-control software present" };
  }

  void ResetMotion() { m_motion.Reset(); }

private:
  SSDwellModeDetector() = default;
  SSDwellModeDetector( const SSDwellModeDetector & ) = delete;
  SSDwellModeDetector & operator=( const SSDwellModeDetector & ) = delete;

  static constexpr DWORD kCalibrationValidMs = 10 * 60 * 1000; // 10 minutes

  mutable std::mutex    m_mutex;
  MouseMotionClassifier m_motion;
  PassiveSignals        m_passive;
  CalibrationOutcome    m_calibration = CalibrationOutcome::None;
  DWORD                 m_calibrationTick = 0;
  std::atomic<bool>     m_hidLive{ false };
};

#endif // SSDwellModeDetector_h
