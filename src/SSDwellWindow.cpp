/*
   SSDwellWindow.cpp: modal "Gaze / Dwell-click" settings window.

   Copyright 2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#include "SSDwellWindow.h"
#include "SSButton.h"
#include "GazeProviderChain.h"
#include "SSGazeDetect.h"
#include "SSDwellModeDetector.h"
#include "SSDialogUtils.h" // sliders and RTL (REQ-F109: not utils.h)
#include "resource.h"
#include "SSGazeReader.h" // REQ-F102 diagnostic report
#include <commdlg.h>
#include <shellapi.h>
#pragma comment(lib, "comdlg32.lib")

// Slider ranges (see Dwell.md): dwell time 300-2000 ms, tolerance 15-80 px,
// cooldown 0-1000 ms. Surfaced here so they are easy to retune.
#define DWELL_TIME_MIN      300
#define DWELL_TIME_MAX      2000
#define DWELL_TOLERANCE_MIN 15
#define DWELL_TOLERANCE_MAX 80
#define DWELL_COOLDOWN_MIN  0
#define DWELL_COOLDOWN_MAX  1000
#define DWELL_LOCKON_MIN    0   // REQ-F104 lock-on delay, ms
#define DWELL_LOCKON_MAX    500

// Refresh cadence for the passive-detection signals readout while the window is
// open (also catches connect/disconnect without a full WM_DEVICECHANGE wiring).
#define DWELL_SIGNALS_TIMER_ID 1
#define DWELL_SIGNALS_INTERVAL 2000

// Posted by SSGazeReader when the background diagnostic dump finishes
// (wParam 1 = saved, 0 = failed) — REQ-F102.
#define WM_DWELL_DUMP_DONE ( WM_APP + 0x51 )

namespace
{
  template <typename T> T ClampValue( T v, T lo, T hi ) { return v < lo ? lo : ( v > hi ? hi : v ); }

  // Working state for one invocation of the dialog.
  struct DwellDlgContext
  {
    SSDwellValues       working;     // edited copy; written back to the caller on OK
    SSDwellWindowHost * host = nullptr; // text, RTL and defaults (REQ-F109)
    HINSTANCE    hInstance = nullptr;
    bool         accepted = false;

    SSButton     lookProbe;        // dwell-enabled "look at me" probe
    SSButton     mouseProbe;       // dwell-disabled "click me" probe

    // SSDwellConfig snapshot, restored on Cancel.
    UINT                 origTime = 0;
    UINT                 origTolerance = 0;
    UINT                 origCooldown = 0;
    UINT                 origLockOn = DWELL_DEFAULT_LOCKON_MS;
    COLORREF             origColor = 0;
    bool                 origRearm = DWELL_DEFAULT_REARM_ON_LEAVE;
    SSDwellModeSelection origSelection = SSDwellModeSelection::Auto;
    SSDwellMode          origDetected = SSDwellMode::Off;
  };

  // Push the working tuning values (not the mode) into the live config so the
  // probes react immediately to slider / color edits.
  void ApplyTuningToConfig( const SSDwellValues & s )
  {
    SSDwellConfig & c = SSDwellConfig::Instance();
    c.SetDwellTimeMs( (UINT) s.timeMs );
    c.SetToleranceRadius( (UINT) s.toleranceRadius );
    c.SetCooldownMs( (UINT) s.cooldownMs );
    c.SetLockOnMs( (UINT) s.lockOnMs );
    c.SetProgressColor( s.progressColor );
    c.SetRearmOnLeave( s.rearmOnLeave );
  }

  void ReadSlidersIntoWorking( HWND hDlg, SSDwellValues & w )
  {
    w.timeMs = (int) SendDlgItemMessage( hDlg, IDC_DWELL_TIME_SLIDER, TBM_GETPOS, 0, 0 );
    w.toleranceRadius = (int) SendDlgItemMessage( hDlg, IDC_DWELL_TOLERANCE_SLIDER, TBM_GETPOS, 0, 0 );
    w.cooldownMs = (int) SendDlgItemMessage( hDlg, IDC_DWELL_COOLDOWN_SLIDER, TBM_GETPOS, 0, 0 );
    w.lockOnMs = (int) SendDlgItemMessage( hDlg, IDC_DWELL_LOCKON_SLIDER, TBM_GETPOS, 0, 0 );
  }

  void SetStatus( HWND hDlg, SSDwellWindowHost & host, int stringId )
  {
    SetDlgItemText( hDlg, IDC_DWELL_STATUS, host.Text( stringId ).c_str() );
  }

  // REQ-F63: the "HID eye tracker" option is available whenever gaze can be
  // read directly — a raw HID eye tracker, or a Tobii tracker opened through
  // the Stream Engine (in-process or via the 64-bit helper), even with
  // tracking off — and always while it is the selected option, so a checked
  // radio is never greyed out.
  void SyncHidOption( HWND hDlg, bool hidDevicePresent )
  {
    GazeProviderChain & chain = GazeProviderChain::Instance();
    bool available = hidDevicePresent || chain.DirectDeviceOpen() || chain.DirectTracking() || chain.HidLive();
    bool selected = IsDlgButtonChecked( hDlg, IDC_DWELL_MODE_HID ) == BST_CHECKED;
    EnableWindow( GetDlgItem( hDlg, IDC_DWELL_MODE_HID ), available || selected );
  }

  // Reads the passive detection signals and writes them into the readout lines.
  // Informational only in this phase (no automatic mode switching yet).
  void RefreshSignals( HWND hDlg, SSDwellWindowHost & host )
  {
    EyeTrackingStatus st = DetectEyeTracking();
    std::wstring yes = host.Text( DWELL_SIGNAL_YES_ID );
    std::wstring no = host.Text( DWELL_SIGNAL_NO_ID );

    // Keep the HID option in sync with hot-plug while the window is open.
    SyncHidOption( hDlg, st.hidDevicePresent );

    // Distinguish "device present" from "actually streaming gaze": a tracker
    // (e.g. Irisbond Hiru) exposes its HID interface to Windows whether or not
    // its tray tool is streaming over HID, so presence alone is misleading.
    std::wstring hidState;
    GazeProviderChain & chain = GazeProviderChain::Instance();
    if( chain.DirectTracking() )
      hidState = host.Text( DWELL_SIGNAL_HID_LIVE_ID );
    else if( chain.DirectDeviceOpen() ) // REQ-F29: open but silent = switched off
      hidState = host.Text( DWELL_SIGNAL_HID_OFF_ID );
    else if( st.hidDevicePresent )
      hidState = host.Text( DWELL_SIGNAL_HID_IDLE_ID );
    else
      hidState = no;
    std::wstring hidLine = host.Text( DWELL_SIGNAL_HID_LABEL_ID ) + L" " + hidState;
    SetDlgItemText( hDlg, IDC_DWELL_SIGNAL_HID, hidLine.c_str() );

    std::wstring toolVal = st.externalToolRunning ? st.activeToolName : host.Text( DWELL_SIGNAL_NONE_ID );
    std::wstring toolLine = host.Text( DWELL_SIGNAL_TOOL_LABEL_ID ) + L" " + toolVal;
    SetDlgItemText( hDlg, IDC_DWELL_SIGNAL_TOOL, toolLine.c_str() );

    std::wstring wecLine = host.Text( DWELL_SIGNAL_WEC_LABEL_ID ) + L" " + ( st.windowsEyeControl ? yes : no );
    SetDlgItemText( hDlg, IDC_DWELL_SIGNAL_WEC, wecLine.c_str() );
  }

  void LocalizeDwellDialog( HWND hDlg, SSDwellWindowHost & host )
  {
    bool isRtl = host.IsRtl();
    updateRtlExStyle( hDlg, isRtl );
    EnumChildWindows( hDlg, ApplyRtlStylesCallback, isRtl );

    SetWindowText( hDlg, host.Text( DWELL_DIALOG_TITLE_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_INTRO, host.Text( DWELL_INTRO_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_GROUP, host.Text( DWELL_MODE_GROUP_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_AUTO, host.Text( DWELL_MODE_AUTO_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_MOUSE, host.Text( DWELL_MODE_MOUSE_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_HID, host.Text( DWELL_MODE_HID_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_OFF, host.Text( DWELL_MODE_OFF_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_TIME_LABEL, host.Text( DWELL_TIME_LABEL_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_TOLERANCE_LABEL, host.Text( DWELL_TOLERANCE_LABEL_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_COOLDOWN_LABEL, host.Text( DWELL_COOLDOWN_LABEL_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_LOCKON_LABEL, host.Text( DWELL_LOCKON_LABEL_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_COLOR_BUTTON, host.Text( DWELL_COLOR_BUTTON_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_REARM_CHECK, host.Text( DWELL_REARM_CHECK_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_DETECT_GROUP, host.Text( DWELL_DETECT_GROUP_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_SIGNALS_GROUP, host.Text( DWELL_SIGNALS_GROUP_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_RESET, host.Text( DWELL_RESET_BUTTON_ID ).c_str() );
    SetDlgItemText( hDlg, IDC_DWELL_DUMP_BUTTON, host.Text( DWELL_DUMP_BUTTON_ID ).c_str() );
    SetDlgItemText( hDlg, IDOK, host.Text( SETTINGS_OK_BUTTON_ID ).c_str() );
    SetDlgItemText( hDlg, IDCANCEL, host.Text( SETTINGS_CANCEL_BUTTON_ID ).c_str() );

    // REQ-F109: a host may leave out the colour and report buttons.
    if( !host.ShowProgressColor() )
    {
      EnableWindow( GetDlgItem( hDlg, IDC_DWELL_COLOR_BUTTON ), FALSE );
      ShowWindow( GetDlgItem( hDlg, IDC_DWELL_COLOR_BUTTON ), SW_HIDE );
    }
    if( !host.ShowDiagnosticReport() )
    {
      EnableWindow( GetDlgItem( hDlg, IDC_DWELL_DUMP_BUTTON ), FALSE );
      ShowWindow( GetDlgItem( hDlg, IDC_DWELL_DUMP_BUTTON ), SW_HIDE );
    }
  }

  // Creates a probe SSButton over the placeholder static's rect and hides the
  // placeholder. The probe is parented to the dialog, so its BN_CLICKED arrives
  // in the dialog proc keyed by `probeId`.
  void CreateProbe( SSButton & btn, HWND hDlg, HINSTANCE hInstance, int areaId, int probeId,
    const std::wstring & text, HFONT font, bool dwellEnabled )
  {
    HWND area = GetDlgItem( hDlg, areaId );
    if( !area ) return;
    RECT rc;
    GetWindowRect( area, &rc );
    // Map the whole rect, not just its top-left corner: in a right-to-left
    // (mirrored) dialog ScreenToClient on one point yields the placeholder's
    // far edge, which shifted both probes one probe-width over the Activation
    // mode group in Arabic and Hebrew. MapWindowPoints adjusts mirrored rects.
    MapWindowPoints( HWND_DESKTOP, hDlg, reinterpret_cast<POINT *>( &rc ), 2 );
    POINT tl = { rc.left < rc.right ? rc.left : rc.right, rc.top };
    int w = rc.right > rc.left ? rc.right - rc.left : rc.left - rc.right;
    int h = rc.bottom - rc.top;
    ShowWindow( area, SW_HIDE );

    SSButtonConfig cfg;
    cfg.borderStyle = SSButtonBorderStyle::Rounded;
    cfg.cornerRadius = 8;
    cfg.borderWidth = 2;

    btn.Create( hDlg, hInstance, probeId, text, tl.x, tl.y, w, h,
      WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_MULTILINE | BS_CENTER | BS_VCENTER, 0, cfg );
    if( font ) btn.SetFont( font );
    btn.SetDwellEnabled( dwellEnabled );
  }

  INT_PTR CALLBACK DwellDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
  {
    DwellDlgContext * ctx = nullptr;
    if( message == WM_INITDIALOG )
    {
      ctx = reinterpret_cast<DwellDlgContext *>( lParam );
      if( !ctx ) return FALSE;
      SetWindowLongPtr( hDlg, GWLP_USERDATA, (LONG_PTR) ctx );
    }
    else
    {
      ctx = reinterpret_cast<DwellDlgContext *>( GetWindowLongPtr( hDlg, GWLP_USERDATA ) );
    }
    if( !ctx ) return FALSE;

    switch( message )
    {
      case WM_INITDIALOG:
      {
        // Dwell-able Reset / OK / Cancel / color / report buttons (BTN-F75).
        SSButton::ConvertDialogPushButtons( hDlg );

        SSDwellConfig & cfg = SSDwellConfig::Instance();

        // Snapshot live config so Cancel can restore it.
        ctx->origTime = cfg.GetDwellTimeMs();
        ctx->origTolerance = cfg.GetToleranceRadius();
        ctx->origCooldown = cfg.GetCooldownMs();
        ctx->origLockOn = cfg.GetLockOnMs();
        ctx->origColor = cfg.GetProgressColor();
        ctx->origRearm = cfg.GetRearmOnLeave();
        ctx->origSelection = cfg.GetModeSelection();
        ctx->origDetected = cfg.GetDetectedMode();

        // Clamp working values into the slider ranges.
        ctx->working.timeMs = ClampValue( ctx->working.timeMs, DWELL_TIME_MIN, DWELL_TIME_MAX );
        ctx->working.toleranceRadius = ClampValue( ctx->working.toleranceRadius, DWELL_TOLERANCE_MIN, DWELL_TOLERANCE_MAX );
        ctx->working.cooldownMs = ClampValue( ctx->working.cooldownMs, DWELL_COOLDOWN_MIN, DWELL_COOLDOWN_MAX );
        ctx->working.lockOnMs = ClampValue( ctx->working.lockOnMs, DWELL_LOCKON_MIN, DWELL_LOCKON_MAX );

        ConfigureSlider( hDlg, IDC_DWELL_TIME_SLIDER, DWELL_TIME_MIN, DWELL_TIME_MAX, ctx->working.timeMs );
        SyncSliderToEdit( hDlg, IDC_DWELL_TIME_SLIDER, IDC_DWELL_TIME_EDIT, FALSE );
        ConfigureSlider( hDlg, IDC_DWELL_TOLERANCE_SLIDER, DWELL_TOLERANCE_MIN, DWELL_TOLERANCE_MAX, ctx->working.toleranceRadius );
        SyncSliderToEdit( hDlg, IDC_DWELL_TOLERANCE_SLIDER, IDC_DWELL_TOLERANCE_EDIT, FALSE );
        ConfigureSlider( hDlg, IDC_DWELL_COOLDOWN_SLIDER, DWELL_COOLDOWN_MIN, DWELL_COOLDOWN_MAX, ctx->working.cooldownMs );
        SyncSliderToEdit( hDlg, IDC_DWELL_COOLDOWN_SLIDER, IDC_DWELL_COOLDOWN_EDIT, FALSE );
        ConfigureSlider( hDlg, IDC_DWELL_LOCKON_SLIDER, DWELL_LOCKON_MIN, DWELL_LOCKON_MAX, ctx->working.lockOnMs );
        SyncSliderToEdit( hDlg, IDC_DWELL_LOCKON_SLIDER, IDC_DWELL_LOCKON_EDIT, FALSE );
        CheckDlgButton( hDlg, IDC_DWELL_REARM_CHECK, ctx->working.rearmOnLeave ? BST_CHECKED : BST_UNCHECKED );

        // A saved "HID eye tracker" choice is kept even when no tracker is
        // readable right now (a Tobii engine may connect seconds later); it
        // uses gaze only, so nothing dwells until gaze arrives (REQ-F63).
        int idc = IDC_DWELL_MODE_AUTO;
        switch( ctx->working.modeSelection )
        {
          case 1: idc = IDC_DWELL_MODE_OFF;   break;
          case 2: idc = IDC_DWELL_MODE_MOUSE; break;
          case 3: idc = IDC_DWELL_MODE_HID;   break;
          default: idc = IDC_DWELL_MODE_AUTO; break;
        }
        CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, idc );
        SyncHidOption( hDlg, IsHidEyeTrackerPresent() );

        LocalizeDwellDialog( hDlg, *ctx->host );
        SetStatus( hDlg, *ctx->host, DWELL_STATUS_INITIAL_ID );

        // Make the probes work regardless of the saved mode for the dialog's
        // life: force HID dwell when the tracker is streaming gaze (the LOOK
        // probe is then armed by the app's gaze router, since the cursor does
        // not move), otherwise force cursor dwell.
        ApplyTuningToConfig( ctx->working );
        GazeProviderChain::Instance().StartAll(); // idempotent; ensures providers
        cfg.SetModeSelection( GazeProviderChain::Instance().HidLive()
          ? SSDwellModeSelection::ForceHidDwell
          : SSDwellModeSelection::ForceMouseDwell );

        HFONT dlgFont = (HFONT) SendMessage( hDlg, WM_GETFONT, 0, 0 );
        CreateProbe( ctx->lookProbe, hDlg, ctx->hInstance, IDC_DWELL_PROBE_LOOK_AREA, IDC_DWELL_PROBE_LOOK,
          ctx->host->Text( DWELL_PROBE_LOOK_ID ).c_str(), dlgFont, true );
        CreateProbe( ctx->mouseProbe, hDlg, ctx->hInstance, IDC_DWELL_PROBE_MOUSE_AREA, IDC_DWELL_PROBE_MOUSE,
          ctx->host->Text( DWELL_PROBE_MOUSE_ID ).c_str(), dlgFont, false );

        // Passive-detection readout: show now and refresh periodically so a
        // tracker/tool that starts or stops is reflected while the window is open.
        RefreshSignals( hDlg, *ctx->host );
        SetTimer( hDlg, DWELL_SIGNALS_TIMER_ID, DWELL_SIGNALS_INTERVAL, nullptr );

        // A dump started from an earlier opening of this window may still run.
        if( ctx->host->ShowDiagnosticReport() )
          EnableWindow( GetDlgItem( hDlg, IDC_DWELL_DUMP_BUTTON ), !SSGazeReader::Instance().IsDiagnosticDumpRunning() );
        return TRUE;
      }

      case WM_DWELL_DUMP_DONE:
      {
        // REQ-F102: report the result; on success show the file in Explorer
        // so it can be attached to an e-mail.
        EnableWindow( GetDlgItem( hDlg, IDC_DWELL_DUMP_BUTTON ), TRUE );
        if( wParam )
        {
          SetStatus( hDlg, *ctx->host, DWELL_STATUS_DUMP_SAVED_ID );
          std::wstring args = L"/select,\"" + SSGazeReader::DiagnosticDumpPath() + L"\"";
          ShellExecuteW( hDlg, L"open", L"explorer.exe", args.c_str(), nullptr, SW_SHOWNORMAL );
        }
        else
        {
          SetStatus( hDlg, *ctx->host, DWELL_STATUS_DUMP_FAILED_ID );
        }
        return TRUE;
      }

      case WM_TIMER:
      {
        if( wParam == DWELL_SIGNALS_TIMER_ID )
        {
          RefreshSignals( hDlg, *ctx->host );

          // Keep the probes in the right forced mode for the dialog's life:
          // the direct-gaze stream (HID reader / Tobii engine) may connect —
          // or drop — after WM_INITDIALOG chose the initial forcing, and a
          // LOOK probe stuck in Mouse mode can never be armed by gaze.
          SSDwellConfig & cfg = SSDwellConfig::Instance();
          SSDwellModeSelection want = GazeProviderChain::Instance().HidLive()
            ? SSDwellModeSelection::ForceHidDwell
            : SSDwellModeSelection::ForceMouseDwell;
          if( cfg.GetModeSelection() != want )
            cfg.SetModeSelection( want );
          return TRUE;
        }
        break;
      }

      case WM_HSCROLL:
      {
        HWND hCtrl = (HWND) lParam;
        if( hCtrl == GetDlgItem( hDlg, IDC_DWELL_TIME_SLIDER ) )
          SyncSliderToEdit( hDlg, IDC_DWELL_TIME_SLIDER, IDC_DWELL_TIME_EDIT, FALSE );
        else if( hCtrl == GetDlgItem( hDlg, IDC_DWELL_TOLERANCE_SLIDER ) )
          SyncSliderToEdit( hDlg, IDC_DWELL_TOLERANCE_SLIDER, IDC_DWELL_TOLERANCE_EDIT, FALSE );
        else if( hCtrl == GetDlgItem( hDlg, IDC_DWELL_COOLDOWN_SLIDER ) )
          SyncSliderToEdit( hDlg, IDC_DWELL_COOLDOWN_SLIDER, IDC_DWELL_COOLDOWN_EDIT, FALSE );
        else if( hCtrl == GetDlgItem( hDlg, IDC_DWELL_LOCKON_SLIDER ) )
          SyncSliderToEdit( hDlg, IDC_DWELL_LOCKON_SLIDER, IDC_DWELL_LOCKON_EDIT, FALSE );
        else
          break;
        ReadSlidersIntoWorking( hDlg, ctx->working );
        ApplyTuningToConfig( ctx->working );
        return TRUE;
      }

      case WM_COMMAND:
      {
        WORD controlId = LOWORD( wParam );
        WORD notifyCode = HIWORD( wParam );

        if( notifyCode == EN_CHANGE )
        {
          HWND hFocus = GetFocus();
          if( controlId == IDC_DWELL_TIME_EDIT && hFocus == GetDlgItem( hDlg, IDC_DWELL_TIME_EDIT ) )
          {
            SyncEditToSlider( hDlg, IDC_DWELL_TIME_EDIT, IDC_DWELL_TIME_SLIDER, FALSE, DWELL_TIME_MIN, DWELL_TIME_MAX );
            ReadSlidersIntoWorking( hDlg, ctx->working );
            ApplyTuningToConfig( ctx->working );
            return TRUE;
          }
          if( controlId == IDC_DWELL_TOLERANCE_EDIT && hFocus == GetDlgItem( hDlg, IDC_DWELL_TOLERANCE_EDIT ) )
          {
            SyncEditToSlider( hDlg, IDC_DWELL_TOLERANCE_EDIT, IDC_DWELL_TOLERANCE_SLIDER, FALSE, DWELL_TOLERANCE_MIN, DWELL_TOLERANCE_MAX );
            ReadSlidersIntoWorking( hDlg, ctx->working );
            ApplyTuningToConfig( ctx->working );
            return TRUE;
          }
          if( controlId == IDC_DWELL_COOLDOWN_EDIT && hFocus == GetDlgItem( hDlg, IDC_DWELL_COOLDOWN_EDIT ) )
          {
            SyncEditToSlider( hDlg, IDC_DWELL_COOLDOWN_EDIT, IDC_DWELL_COOLDOWN_SLIDER, FALSE, DWELL_COOLDOWN_MIN, DWELL_COOLDOWN_MAX );
            ReadSlidersIntoWorking( hDlg, ctx->working );
            ApplyTuningToConfig( ctx->working );
            return TRUE;
          }
          if( controlId == IDC_DWELL_LOCKON_EDIT && hFocus == GetDlgItem( hDlg, IDC_DWELL_LOCKON_EDIT ) )
          {
            SyncEditToSlider( hDlg, IDC_DWELL_LOCKON_EDIT, IDC_DWELL_LOCKON_SLIDER, FALSE, DWELL_LOCKON_MIN, DWELL_LOCKON_MAX );
            ReadSlidersIntoWorking( hDlg, ctx->working );
            ApplyTuningToConfig( ctx->working );
            return TRUE;
          }
        }

        switch( controlId )
        {
          case IDC_DWELL_PROBE_LOOK:
          {
            if( ctx->lookProbe.GetLastActivation() == SSButtonActivation::Dwell )
            {
              // Ground truth: the gaze probe fired by dwell. If the HID stream had
              // a valid sample inside the probe, gaze is read directly (HidDwell);
              // otherwise the cursor itself is gaze-driven (MouseDwell). Either
              // way, feed the detector so Auto trusts this over kinematic guesses.
              RECT pr = {};
              GazeSample gs;
              bool hidInside =
                ctx->lookProbe.GetWindowRect( pr ) &&
                GazeProviderChain::Instance().GetHidSample( &gs ) && gs.valid &&
                PtInRect( &pr, gs.screenPoint );

              if( hidInside )
              {
                SSDwellModeDetector::Instance().ReportCalibration( CalibrationOutcome::ConfirmedHidGaze );
                ctx->working.detectedMode = 2; // HID
                EnableWindow( GetDlgItem( hDlg, IDC_DWELL_MODE_HID ), TRUE );
                CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, IDC_DWELL_MODE_HID );
              }
              else
              {
                SSDwellModeDetector::Instance().ReportCalibration( CalibrationOutcome::ConfirmedCursorIsGaze );
                ctx->working.detectedMode = 1; // Mouse
                CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, IDC_DWELL_MODE_MOUSE );
              }
              SetStatus( hDlg, *ctx->host, DWELL_STATUS_GAZE_ID );
            }
            else
            {
              // Activated by a physical click: ask the user to actually look.
              SetStatus( hDlg, *ctx->host, DWELL_STATUS_LOOK_HINT_ID );
            }
            return TRUE;
          }

          case IDC_DWELL_PROBE_MOUSE:
          {
            // The manual probe has dwell disabled, so reaching here means a real
            // click / key: the user drives input by hand -> dwell off.
            SSDwellModeDetector::Instance().ReportCalibration( CalibrationOutcome::ConfirmedMouseIsHand );
            ctx->working.detectedMode = 0; // Off
            CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, IDC_DWELL_MODE_OFF );
            SetStatus( hDlg, *ctx->host, DWELL_STATUS_MOUSE_ID );
            return TRUE;
          }

          case IDC_DWELL_RESET:
          {
            // Restore every dwell setting to its first-run default (see
            // DWELL_DEFAULT_* in stdafx.h; the progress color defaults to the
            // Windows accent color). Only the working copy and the dialog
            // controls change here — OK commits, Cancel still discards.
            ctx->working.modeSelection = DWELL_DEFAULT_MODE_SELECTION;
            ctx->working.timeMs = DWELL_DEFAULT_TIME_MS;
            ctx->working.toleranceRadius = DWELL_DEFAULT_TOLERANCE_PX;
            ctx->working.cooldownMs = DWELL_DEFAULT_COOLDOWN_MS;
            ctx->working.progressColor = ctx->host->DefaultProgressColor(); // SimonSays: the Windows accent
            ctx->working.detectedMode = DWELL_DEFAULT_DETECTED_MODE;
            ctx->working.rearmOnLeave = DWELL_DEFAULT_REARM_ON_LEAVE;
            ctx->working.lockOnMs = DWELL_DEFAULT_LOCKON_MS;
            CheckDlgButton( hDlg, IDC_DWELL_REARM_CHECK, ctx->working.rearmOnLeave ? BST_CHECKED : BST_UNCHECKED );
            SendDlgItemMessage( hDlg, IDC_DWELL_LOCKON_SLIDER, TBM_SETPOS, TRUE, ctx->working.lockOnMs );
            SyncSliderToEdit( hDlg, IDC_DWELL_LOCKON_SLIDER, IDC_DWELL_LOCKON_EDIT, FALSE );

            SendDlgItemMessage( hDlg, IDC_DWELL_TIME_SLIDER, TBM_SETPOS, TRUE, ctx->working.timeMs );
            SyncSliderToEdit( hDlg, IDC_DWELL_TIME_SLIDER, IDC_DWELL_TIME_EDIT, FALSE );
            SendDlgItemMessage( hDlg, IDC_DWELL_TOLERANCE_SLIDER, TBM_SETPOS, TRUE, ctx->working.toleranceRadius );
            SyncSliderToEdit( hDlg, IDC_DWELL_TOLERANCE_SLIDER, IDC_DWELL_TOLERANCE_EDIT, FALSE );
            SendDlgItemMessage( hDlg, IDC_DWELL_COOLDOWN_SLIDER, TBM_SETPOS, TRUE, ctx->working.cooldownMs );
            SyncSliderToEdit( hDlg, IDC_DWELL_COOLDOWN_SLIDER, IDC_DWELL_COOLDOWN_EDIT, FALSE );
            CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, IDC_DWELL_MODE_AUTO );

            ApplyTuningToConfig( ctx->working );
            ctx->lookProbe.Invalidate();
            return TRUE;
          }

          case IDC_DWELL_DUMP_BUTTON:
          {
            // REQ-F102: write the diagnostic report off the UI thread so the
            // probes and gaze dwell keep working while a HID capture runs.
            if( SSGazeReader::Instance().StartDiagnosticDump( hDlg, WM_DWELL_DUMP_DONE ) )
            {
              EnableWindow( GetDlgItem( hDlg, IDC_DWELL_DUMP_BUTTON ), FALSE );
              SetStatus( hDlg, *ctx->host, DWELL_STATUS_DUMP_RUNNING_ID );
            }
            return TRUE;
          }

          case IDC_DWELL_REARM_CHECK:
          {
            // Applies live (like the sliders) so the probes reflect it; Cancel restores.
            ctx->working.rearmOnLeave = IsDlgButtonChecked( hDlg, IDC_DWELL_REARM_CHECK ) == BST_CHECKED;
            ApplyTuningToConfig( ctx->working );
            return TRUE;
          }

          case IDC_DWELL_COLOR_BUTTON:
          {
            static COLORREF s_customColors[16] = { 0 };
            CHOOSECOLOR cc = {};
            cc.lStructSize = sizeof( cc );
            cc.hwndOwner = hDlg;
            cc.lpCustColors = s_customColors;
            cc.rgbResult = ctx->working.progressColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            if( ChooseColor( &cc ) )
            {
              ctx->working.progressColor = cc.rgbResult;
              ApplyTuningToConfig( ctx->working );
              ctx->lookProbe.Invalidate();
            }
            return TRUE;
          }

          case IDOK:
          {
            ReadSlidersIntoWorking( hDlg, ctx->working );
            ctx->working.rearmOnLeave = IsDlgButtonChecked( hDlg, IDC_DWELL_REARM_CHECK ) == BST_CHECKED;

            int sel = 0; // Auto
            if( IsDlgButtonChecked( hDlg, IDC_DWELL_MODE_OFF ) == BST_CHECKED )        sel = 1;
            else if( IsDlgButtonChecked( hDlg, IDC_DWELL_MODE_MOUSE ) == BST_CHECKED ) sel = 2;
            else if( IsDlgButtonChecked( hDlg, IDC_DWELL_MODE_HID ) == BST_CHECKED )   sel = 3;
            else                                                                       sel = 0;
            ctx->working.modeSelection = sel;

            // Commit the final state to the live config.
            SSDwellConfig & cfg = SSDwellConfig::Instance();
            ApplyTuningToConfig( ctx->working );
            cfg.ApplyDetectedMode( (SSDwellMode) ctx->working.detectedMode );
            cfg.SetModeSelection( (SSDwellModeSelection) ctx->working.modeSelection );

            KillTimer( hDlg, DWELL_SIGNALS_TIMER_ID );
            ctx->accepted = true;
            EndDialog( hDlg, IDOK );
            return TRUE;
          }

          case IDCANCEL:
          {
            // Restore the live config to its pre-dialog state.
            SSDwellConfig & cfg = SSDwellConfig::Instance();
            cfg.SetDwellTimeMs( ctx->origTime );
            cfg.SetToleranceRadius( ctx->origTolerance );
            cfg.SetCooldownMs( ctx->origCooldown );
            cfg.SetLockOnMs( ctx->origLockOn );
            cfg.SetProgressColor( ctx->origColor );
            cfg.SetRearmOnLeave( ctx->origRearm );
            cfg.ApplyDetectedMode( ctx->origDetected );
            cfg.SetModeSelection( ctx->origSelection );
            KillTimer( hDlg, DWELL_SIGNALS_TIMER_ID );
            EndDialog( hDlg, IDCANCEL );
            return TRUE;
          }
        }
        break;
      }
    }
    return FALSE;
  }
} // namespace

bool ShowDwellSettingsDialog( HWND owner, HINSTANCE hInstance, SSDwellValues & values, SSDwellWindowHost & host )
{
  DwellDlgContext ctx;
  ctx.working = values;
  ctx.host = &host;
  ctx.hInstance = hInstance;

  DialogBoxParam( hInstance, MAKEINTRESOURCE( IDD_DWELL_DIALOG ), owner, DwellDialogProc, (LPARAM) &ctx );

  if( ctx.accepted )
  {
    values = ctx.working;
    return true;
  }
  return false;
}
