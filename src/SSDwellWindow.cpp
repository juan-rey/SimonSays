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
#include "utils.h"
#include "resource.h"
#include <commdlg.h>
#pragma comment(lib, "comdlg32.lib")

// Slider ranges (see Dwell.md): dwell time 300-2000 ms, tolerance 15-80 px,
// cooldown 0-1000 ms. Surfaced here so they are easy to retune.
#define DWELL_TIME_MIN      300
#define DWELL_TIME_MAX      2000
#define DWELL_TOLERANCE_MIN 15
#define DWELL_TOLERANCE_MAX 80
#define DWELL_COOLDOWN_MIN  0
#define DWELL_COOLDOWN_MAX  1000

// Refresh cadence for the passive-detection signals readout while the window is
// open (also catches connect/disconnect without a full WM_DEVICECHANGE wiring).
#define DWELL_SIGNALS_TIMER_ID 1
#define DWELL_SIGNALS_INTERVAL 2000

namespace
{
  template <typename T> T ClampValue( T v, T lo, T hi ) { return v < lo ? lo : ( v > hi ? hi : v ); }

  // Working state for one invocation of the dialog.
  struct DwellDlgContext
  {
    Settings     working;          // edited copy; written back to the caller on OK
    std::wstring language;
    HINSTANCE    hInstance = nullptr;
    bool         accepted = false;

    SSButton     lookProbe;        // dwell-enabled "look at me" probe
    SSButton     mouseProbe;       // dwell-disabled "click me" probe

    // SSDwellConfig snapshot, restored on Cancel.
    UINT                 origTime = 0;
    UINT                 origTolerance = 0;
    UINT                 origCooldown = 0;
    COLORREF             origColor = 0;
    SSDwellModeSelection origSelection = SSDwellModeSelection::Auto;
    SSDwellMode          origDetected = SSDwellMode::Off;
  };

  // Push the working tuning values (not the mode) into the live config so the
  // probes react immediately to slider / color edits.
  void ApplyTuningToConfig( const Settings & s )
  {
    SSDwellConfig & c = SSDwellConfig::Instance();
    c.SetDwellTimeMs( (UINT) s.dwellTimeMs );
    c.SetToleranceRadius( (UINT) s.dwellToleranceRadius );
    c.SetCooldownMs( (UINT) s.dwellCooldownMs );
    c.SetProgressColor( s.dwellProgressColor );
  }

  void ReadSlidersIntoWorking( HWND hDlg, Settings & w )
  {
    w.dwellTimeMs = (int) SendDlgItemMessage( hDlg, IDC_DWELL_TIME_SLIDER, TBM_GETPOS, 0, 0 );
    w.dwellToleranceRadius = (int) SendDlgItemMessage( hDlg, IDC_DWELL_TOLERANCE_SLIDER, TBM_GETPOS, 0, 0 );
    w.dwellCooldownMs = (int) SendDlgItemMessage( hDlg, IDC_DWELL_COOLDOWN_SLIDER, TBM_GETPOS, 0, 0 );
  }

  void SetStatus( HWND hDlg, const std::wstring & language, int stringId )
  {
    SetDlgItemText( hDlg, IDC_DWELL_STATUS, GetLocalizedString( stringId, language ) );
  }

  // Reads the passive detection signals and writes them into the readout lines.
  // Informational only in this phase (no automatic mode switching yet).
  void RefreshSignals( HWND hDlg, const std::wstring & lang )
  {
    EyeTrackingStatus st = DetectEyeTracking();
    std::wstring yes = GetLocalizedString( DWELL_SIGNAL_YES_ID, lang );
    std::wstring no = GetLocalizedString( DWELL_SIGNAL_NO_ID, lang );

    // Keep the HID option in sync with hot-plug while the window is open.
    EnableWindow( GetDlgItem( hDlg, IDC_DWELL_MODE_HID ), st.hidDevicePresent );

    // Distinguish "device present" from "actually streaming gaze": a tracker
    // (e.g. Irisbond Hiru) exposes its HID interface to Windows whether or not
    // its tray tool is streaming over HID, so presence alone is misleading.
    std::wstring hidState;
    if( GazeProviderChain::Instance().HidLive() )
      hidState = GetLocalizedString( DWELL_SIGNAL_HID_LIVE_ID, lang );
    else if( st.hidDevicePresent )
      hidState = GetLocalizedString( DWELL_SIGNAL_HID_IDLE_ID, lang );
    else
      hidState = no;
    std::wstring hidLine = std::wstring( GetLocalizedString( DWELL_SIGNAL_HID_LABEL_ID, lang ) ) + L" " + hidState;
    SetDlgItemText( hDlg, IDC_DWELL_SIGNAL_HID, hidLine.c_str() );

    std::wstring toolVal = st.externalToolRunning ? st.activeToolName : GetLocalizedString( DWELL_SIGNAL_NONE_ID, lang );
    std::wstring toolLine = std::wstring( GetLocalizedString( DWELL_SIGNAL_TOOL_LABEL_ID, lang ) ) + L" " + toolVal;
    SetDlgItemText( hDlg, IDC_DWELL_SIGNAL_TOOL, toolLine.c_str() );

    std::wstring wecLine = std::wstring( GetLocalizedString( DWELL_SIGNAL_WEC_LABEL_ID, lang ) ) + L" " + ( st.windowsEyeControl ? yes : no );
    SetDlgItemText( hDlg, IDC_DWELL_SIGNAL_WEC, wecLine.c_str() );
  }

  void LocalizeDwellDialog( HWND hDlg, const std::wstring & lang )
  {
    bool isRtl = IsLanguageRTL( lang );
    updateRtlExStyle( hDlg, isRtl );
    EnumChildWindows( hDlg, ApplyRtlStylesCallback, isRtl );

    SetWindowText( hDlg, GetLocalizedString( DWELL_DIALOG_TITLE_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_INTRO, GetLocalizedString( DWELL_INTRO_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_GROUP, GetLocalizedString( DWELL_MODE_GROUP_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_AUTO, GetLocalizedString( DWELL_MODE_AUTO_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_MOUSE, GetLocalizedString( DWELL_MODE_MOUSE_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_HID, GetLocalizedString( DWELL_MODE_HID_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_MODE_OFF, GetLocalizedString( DWELL_MODE_OFF_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_TIME_LABEL, GetLocalizedString( DWELL_TIME_LABEL_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_TOLERANCE_LABEL, GetLocalizedString( DWELL_TOLERANCE_LABEL_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_COOLDOWN_LABEL, GetLocalizedString( DWELL_COOLDOWN_LABEL_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_COLOR_BUTTON, GetLocalizedString( DWELL_COLOR_BUTTON_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_DETECT_GROUP, GetLocalizedString( DWELL_DETECT_GROUP_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_SIGNALS_GROUP, GetLocalizedString( DWELL_SIGNALS_GROUP_ID, lang ) );
    SetDlgItemText( hDlg, IDC_DWELL_RESET, GetLocalizedString( DWELL_RESET_BUTTON_ID, lang ) );
    SetDlgItemText( hDlg, IDOK, GetLocalizedString( SETTINGS_OK_BUTTON_ID, lang ) );
    SetDlgItemText( hDlg, IDCANCEL, GetLocalizedString( SETTINGS_CANCEL_BUTTON_ID, lang ) );
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
    POINT tl = { rc.left, rc.top };
    ScreenToClient( hDlg, &tl );
    int w = rc.right - rc.left;
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
        SSDwellConfig & cfg = SSDwellConfig::Instance();

        // Snapshot live config so Cancel can restore it.
        ctx->origTime = cfg.GetDwellTimeMs();
        ctx->origTolerance = cfg.GetToleranceRadius();
        ctx->origCooldown = cfg.GetCooldownMs();
        ctx->origColor = cfg.GetProgressColor();
        ctx->origSelection = cfg.GetModeSelection();
        ctx->origDetected = cfg.GetDetectedMode();

        // Clamp working values into the slider ranges.
        ctx->working.dwellTimeMs = ClampValue( ctx->working.dwellTimeMs, DWELL_TIME_MIN, DWELL_TIME_MAX );
        ctx->working.dwellToleranceRadius = ClampValue( ctx->working.dwellToleranceRadius, DWELL_TOLERANCE_MIN, DWELL_TOLERANCE_MAX );
        ctx->working.dwellCooldownMs = ClampValue( ctx->working.dwellCooldownMs, DWELL_COOLDOWN_MIN, DWELL_COOLDOWN_MAX );

        ConfigureSlider( hDlg, IDC_DWELL_TIME_SLIDER, DWELL_TIME_MIN, DWELL_TIME_MAX, ctx->working.dwellTimeMs );
        SyncSliderToEdit( hDlg, IDC_DWELL_TIME_SLIDER, IDC_DWELL_TIME_EDIT, FALSE );
        ConfigureSlider( hDlg, IDC_DWELL_TOLERANCE_SLIDER, DWELL_TOLERANCE_MIN, DWELL_TOLERANCE_MAX, ctx->working.dwellToleranceRadius );
        SyncSliderToEdit( hDlg, IDC_DWELL_TOLERANCE_SLIDER, IDC_DWELL_TOLERANCE_EDIT, FALSE );
        ConfigureSlider( hDlg, IDC_DWELL_COOLDOWN_SLIDER, DWELL_COOLDOWN_MIN, DWELL_COOLDOWN_MAX, ctx->working.dwellCooldownMs );
        SyncSliderToEdit( hDlg, IDC_DWELL_COOLDOWN_SLIDER, IDC_DWELL_COOLDOWN_EDIT, FALSE );

        // HID dwell is selectable only when a HID eye-tracker is present.
        bool hidAvailable = IsHidEyeTrackerPresent();
        EnableWindow( GetDlgItem( hDlg, IDC_DWELL_MODE_HID ), hidAvailable );
        if( ctx->working.dwellModeSelection == 3 && !hidAvailable ) // HID -> normalize to Auto
          ctx->working.dwellModeSelection = 0;

        int idc = IDC_DWELL_MODE_AUTO;
        switch( ctx->working.dwellModeSelection )
        {
          case 1: idc = IDC_DWELL_MODE_OFF;   break;
          case 2: idc = IDC_DWELL_MODE_MOUSE; break;
          case 3: idc = IDC_DWELL_MODE_HID;   break;
          default: idc = IDC_DWELL_MODE_AUTO; break;
        }
        CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, idc );

        LocalizeDwellDialog( hDlg, ctx->language );
        SetStatus( hDlg, ctx->language, DWELL_STATUS_INITIAL_ID );

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
          GetLocalizedString( DWELL_PROBE_LOOK_ID, ctx->language ), dlgFont, true );
        CreateProbe( ctx->mouseProbe, hDlg, ctx->hInstance, IDC_DWELL_PROBE_MOUSE_AREA, IDC_DWELL_PROBE_MOUSE,
          GetLocalizedString( DWELL_PROBE_MOUSE_ID, ctx->language ), dlgFont, false );

        // Passive-detection readout: show now and refresh periodically so a
        // tracker/tool that starts or stops is reflected while the window is open.
        RefreshSignals( hDlg, ctx->language );
        SetTimer( hDlg, DWELL_SIGNALS_TIMER_ID, DWELL_SIGNALS_INTERVAL, nullptr );
        return TRUE;
      }

      case WM_TIMER:
      {
        if( wParam == DWELL_SIGNALS_TIMER_ID )
        {
          RefreshSignals( hDlg, ctx->language );

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
                ctx->working.dwellDetectedMode = 2; // HID
                EnableWindow( GetDlgItem( hDlg, IDC_DWELL_MODE_HID ), TRUE );
                CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, IDC_DWELL_MODE_HID );
              }
              else
              {
                SSDwellModeDetector::Instance().ReportCalibration( CalibrationOutcome::ConfirmedCursorIsGaze );
                ctx->working.dwellDetectedMode = 1; // Mouse
                CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, IDC_DWELL_MODE_MOUSE );
              }
              SetStatus( hDlg, ctx->language, DWELL_STATUS_GAZE_ID );
            }
            else
            {
              // Activated by a physical click: ask the user to actually look.
              SetStatus( hDlg, ctx->language, DWELL_STATUS_LOOK_HINT_ID );
            }
            return TRUE;
          }

          case IDC_DWELL_PROBE_MOUSE:
          {
            // The manual probe has dwell disabled, so reaching here means a real
            // click / key: the user drives input by hand -> dwell off.
            SSDwellModeDetector::Instance().ReportCalibration( CalibrationOutcome::ConfirmedMouseIsHand );
            ctx->working.dwellDetectedMode = 0; // Off
            CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, IDC_DWELL_MODE_OFF );
            SetStatus( hDlg, ctx->language, DWELL_STATUS_MOUSE_ID );
            return TRUE;
          }

          case IDC_DWELL_RESET:
          {
            // Restore every dwell setting to its first-run default (see
            // DWELL_DEFAULT_* in stdafx.h; the progress color defaults to the
            // Windows accent color). Only the working copy and the dialog
            // controls change here — OK commits, Cancel still discards.
            ctx->working.dwellModeSelection = DWELL_DEFAULT_MODE_SELECTION;
            ctx->working.dwellTimeMs = DWELL_DEFAULT_TIME_MS;
            ctx->working.dwellToleranceRadius = DWELL_DEFAULT_TOLERANCE_PX;
            ctx->working.dwellCooldownMs = DWELL_DEFAULT_COOLDOWN_MS;
            ctx->working.dwellProgressColor = GetAccentColor();
            ctx->working.dwellDetectedMode = DWELL_DEFAULT_DETECTED_MODE;

            SendDlgItemMessage( hDlg, IDC_DWELL_TIME_SLIDER, TBM_SETPOS, TRUE, ctx->working.dwellTimeMs );
            SyncSliderToEdit( hDlg, IDC_DWELL_TIME_SLIDER, IDC_DWELL_TIME_EDIT, FALSE );
            SendDlgItemMessage( hDlg, IDC_DWELL_TOLERANCE_SLIDER, TBM_SETPOS, TRUE, ctx->working.dwellToleranceRadius );
            SyncSliderToEdit( hDlg, IDC_DWELL_TOLERANCE_SLIDER, IDC_DWELL_TOLERANCE_EDIT, FALSE );
            SendDlgItemMessage( hDlg, IDC_DWELL_COOLDOWN_SLIDER, TBM_SETPOS, TRUE, ctx->working.dwellCooldownMs );
            SyncSliderToEdit( hDlg, IDC_DWELL_COOLDOWN_SLIDER, IDC_DWELL_COOLDOWN_EDIT, FALSE );
            CheckRadioButton( hDlg, IDC_DWELL_MODE_AUTO, IDC_DWELL_MODE_OFF, IDC_DWELL_MODE_AUTO );

            ApplyTuningToConfig( ctx->working );
            ctx->lookProbe.Invalidate();
            return TRUE;
          }

          case IDC_DWELL_COLOR_BUTTON:
          {
            static COLORREF s_customColors[16] = { 0 };
            CHOOSECOLOR cc = {};
            cc.lStructSize = sizeof( cc );
            cc.hwndOwner = hDlg;
            cc.lpCustColors = s_customColors;
            cc.rgbResult = ctx->working.dwellProgressColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            if( ChooseColor( &cc ) )
            {
              ctx->working.dwellProgressColor = cc.rgbResult;
              ApplyTuningToConfig( ctx->working );
              ctx->lookProbe.Invalidate();
            }
            return TRUE;
          }

          case IDOK:
          {
            ReadSlidersIntoWorking( hDlg, ctx->working );

            int sel = 0; // Auto
            if( IsDlgButtonChecked( hDlg, IDC_DWELL_MODE_OFF ) == BST_CHECKED )        sel = 1;
            else if( IsDlgButtonChecked( hDlg, IDC_DWELL_MODE_MOUSE ) == BST_CHECKED ) sel = 2;
            else if( IsDlgButtonChecked( hDlg, IDC_DWELL_MODE_HID ) == BST_CHECKED )   sel = 3;
            else                                                                       sel = 0;
            ctx->working.dwellModeSelection = sel;

            // Commit the final state to the live config.
            SSDwellConfig & cfg = SSDwellConfig::Instance();
            ApplyTuningToConfig( ctx->working );
            cfg.ApplyDetectedMode( (SSDwellMode) ctx->working.dwellDetectedMode );
            cfg.SetModeSelection( (SSDwellModeSelection) ctx->working.dwellModeSelection );

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
            cfg.SetProgressColor( ctx->origColor );
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

bool ShowDwellSettingsDialog( HWND owner, HINSTANCE hInstance, Settings & settings )
{
  DwellDlgContext ctx;
  ctx.working = settings;
  ctx.language = settings.language; // empty => GetLocalizedString resolves the system language
  ctx.hInstance = hInstance;

  DialogBoxParam( hInstance, MAKEINTRESOURCE( IDD_DWELL_DIALOG ), owner, DwellDialogProc, (LPARAM) &ctx );

  if( ctx.accepted )
  {
    settings = ctx.working;
    return true;
  }
  return false;
}
