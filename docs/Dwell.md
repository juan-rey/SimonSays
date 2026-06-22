# Prompt for Claude Code — Gaze Dwell-Click System for SimonSays

## Context

SimonSays is a Win32 C++ desktop accessibility application for users with ALS
and speech impairments. It targets eye-gaze users who run external eye-control
software alongside it: Irisbond EasyClick, Tobii Dynavox TD Control, Windows
Eye Control, and OptiKey. SimonSays must add **dwell-click** activation to its
custom buttons so users can trigger them by looking, without blocking the eye
tracker from the external tool.

We have already designed (as reference implementations to adapt):

- `SSButton` — custom button class; dwell currently fires from `WM_MOUSEMOVE`.
- `SSDwellConfig` — global singleton: dwell time, tolerance, cooldown, color.
- `SSGazeReader` — direct HID eye-tracker reader (Usage Page 0x12), non-exclusive.
- `SSDwellModeDetector` + `MouseMotionClassifier` — intelligent mode selection.

This prompt asks you to integrate all of it into a coherent, configurable
system with three layers:

1. **Tracking source abstraction** — a provider chain so the dwell can be fed
   by HID gaze, by the cursor, or by future backends, without touching
   `SSButton`.
2. **Intelligent mode detection** — automatically choose the right dwell mode
   from fused signals, with user calibration as ground truth.
3. **Configuration & persistence** — a settings dialog and registry storage so
   the chosen behavior survives restarts.

## Why this design

The external tools differ in whether they move the Windows cursor:

- Cursor-moving modes (EasyClick Beginner / Pro Mouse-Movement, OptiKey
  Magnetic Cursor, WEC cursor modes) → cursor-based dwell works.
- Two-step / no-cursor modes (WEC Precise, TD Control gaze selection) → need to
  read gaze directly from HID.
- Tools that already issue clicks (TD Control dwell, WEC left-click) → our dwell
  must stay out of the way to avoid double activation.

HID standard support is device-dependent: Tobii Dynavox PCEye / I-Series and
Irisbond Hiru (HID mode) expose it; Tobii 4C only with Core SW on Win10; Tobii 5
gaming and 5L do not. So the system must degrade gracefully and never assume a
mode — it detects, or lets the user force one.

## Architecture to implement

### Layer 1 — Tracking source providers

Define an interface and a chain that `SSButton` queries for the current
tracking point, in screen coordinates plus validity.

```cpp
struct GazeSample {
    POINT screenPoint;   // virtual-desktop pixels
    DWORD timestamp;     // GetTickCount when produced
    bool  valid;
};

class IGazeProvider {
public:
    virtual ~IGazeProvider() = default;
    virtual bool        Start()  = 0;
    virtual void        Stop()   = 0;
    virtual bool        IsLive() const = 0;            // producing valid data now
    virtual bool        GetSample(GazeSample* out) const = 0;
    virtual const wchar_t* Name() const = 0;
};
```

Implement initially:

- `HidGazeProvider` — wraps the `SSGazeReader` HID logic. `IsLive()` true while
  fresh valid samples arrive (respecting a max-age, e.g. 200 ms).
- `CursorGazeProvider` — `GetSample` returns `GetCursorPos`; always `IsLive`.

Provide a `GazeProviderChain` singleton holding an ordered list. It exposes:

```cpp
class GazeProviderChain {
public:
    static GazeProviderChain& Instance();
    void StartAll();
    void StopAll();
    // Returns the sample from the first provider that IsLive(), per the mode.
    bool GetTrackingSample(GazeSample* out) const;
    bool HidLive() const;     // convenience for the detector
};
```

`SSButton::GetTrackingPoint()` (client-space) asks the chain, honoring the
active `SSDwellMode`:

- `MouseDwell` → cursor provider.
- `HidDwell` → HID provider; if not live, fall back to cursor.
- `ExternalClick` / `Off` → dwell disabled; `SSButton` does nothing.

Leave room for a future `TobiiStreamsGazeProvider` (commercial SDK, for 4C/
Experience and 5L) — do **not** implement it, just make the chain accept it.

### Layer 2 — Passive detection + intelligent mode detector

Implement the passive detection helpers (separate file, e.g. `SSGazeDetect`):

- `bool IsHidEyeTrackerPresent()` — HID enumeration filtered on Usage Page
  0x0012 / Usage 0x0001, opening handles with access 0 (no read), just to query
  `HidP_GetCaps`. Do not parse reports.
- `bool DetectActiveEyeTrackingTool(EyeTrackingTool* out)` — `Toolhelp32`
  process snapshot matched against a known-tools table (easyclick.exe,
  hirusystray.exe, tdcontrol.exe, optikey.exe, tobii*.exe, etc.), case-insensitive.
- `bool IsWindowsEyeControlEnabled()` — read the Eye Control enable value under
  `HKCU\Software\Microsoft\...` (verify the exact key on a machine with Eye
  Control on; treat a missing key as "not enabled").
- `bool IsClickingTool(const EyeTrackingTool&)` — maps known tools/modes that
  issue their own clicks → used to recommend `ExternalClick`.

Integrate the already-designed `SSDwellModeDetector` and
`MouseMotionClassifier`:

- A 60 Hz timer calls `SSDwellModeDetector::Tick()` (samples `GetCursorPos`).
- A 1–3 s timer (and `WM_DEVICECHANGE`) refreshes passive signals via
  `UpdatePassive(...)` and `SetHidLive(GazeProviderChain::Instance().HidLive())`.
- `Decide()` returns `{ SSDwellMode, confidence, reason }`.
- Apply **hysteresis**: only switch the active mode after a decision repeats for
  N consecutive evaluations (default 3) to prevent flapping.

### Calibration (ground truth)

Implement the two-probe-button calibration as part of the settings flow:

- A small modal/inline panel with two large `SSButton`s:
  - "Activate me by LOOKING at it"  (gaze probe)
  - "Activate me with your MOUSE/switch" (manual probe)
- On gaze-probe activation, infer the outcome:
  - HID provider had a valid sample inside the button → `ConfirmedHidGaze`.
  - else cursor inside + `MouseMotionClassifier` gaze-likelihood > 0.6 →
    `ConfirmedCursorIsGaze`.
  - else → `ConfirmedMouseIsHand`.
- Feed via `SSDwellModeDetector::ReportCalibration(outcome)`. Calibration is
  authoritative while fresh (10 min, configurable) and may be persisted as the
  last known good mode (see registry below).

### Layer 3 — Configuration, persistence, UI

#### `SSDwellConfig` additions

```cpp
enum class SSDwellModeSelection { Auto, ForceOff, ForceMouseDwell, ForceHidDwell };

void                 SetModeSelection(SSDwellModeSelection);
SSDwellModeSelection GetModeSelection() const;

void        ApplyDetectedMode(SSDwellMode);   // Auto path: mode -> enabled+source
SSDwellMode GetActiveMode() const;            // resolved mode currently in effect

bool        LoadFromRegistry();               // called at startup
bool        SaveToRegistry() const;           // called on settings apply / exit
```

When `GetModeSelection() == Auto`, `ApplyDetectedMode` follows the detector;
otherwise the forced mode wins and the detector output is ignored (but may still
be shown in the UI for information).

#### Registry layout

Store under `HKEY_CURRENT_USER\Software\SimonSays\Dwell`. Use `DWORD` for all
numerics and `COLORREF` packed in a `DWORD`. Provide sane defaults when a value
is missing (first run). Never require admin; HKCU only.

| Value name              | Type  | Meaning                                   | Default        |
|-------------------------|-------|-------------------------------------------|----------------|
| `ModeSelection`         | DWORD | 0 Auto · 1 Off · 2 Mouse · 3 HID          | 0 (Auto)       |
| `DwellTimeMs`           | DWORD | Fixation time before firing               | 800            |
| `ToleranceRadiusPx`     | DWORD | Jitter tolerance radius                   | 35             |
| `CooldownMs`            | DWORD | Post-click re-arm block                   | 300            |
| `ProgressColor`         | DWORD | COLORREF of the progress indicator        | RGB(0,120,215) |
| `HysteresisCount`       | DWORD | Consecutive decisions before mode switch  | 3              |
| `CalibrationValidMs`    | DWORD | How long calibration stays authoritative  | 600000         |
| `LastDetectedMode`      | DWORD | Cache of last resolved mode (info/restore)| 0              |
| `ShowProgressRing`      | DWORD | 0 bar · 1 ring indicator                  | 0              |

`LoadFromRegistry` runs after creating the singleton and before any `SSButton`
is created. `SaveToRegistry` runs on dialog "OK/Apply" and on app shutdown.

#### Configuration dialog

Add a Win32 dialog (resource-based `.rc` or created programmatically — match how
SimonSays already builds its windows). Controls:

- **Dwell mode** group (radio buttons): Automatic · Mouse cursor · HID eye
  tracker · Off. Maps to `SSDwellModeSelection`.
- **Dwell time** — slider + numeric readout, range 300–2000 ms.
- **Tolerance radius** — slider + readout, range 15–80 px.
- **Cooldown** — slider + readout, range 0–1000 ms.
- **Progress indicator** — color button (opens `ChooseColor`) + a "bar vs ring"
  choice.
- **Run calibration…** button — launches the two-probe panel; on completion,
  shows the inferred outcome.
- **Status area** (read-only, live-updating ~1 s): current resolved mode, the
  detector's `reason` string and confidence, and a short list of detected
  signals (HID device present? which tool process? Windows Eye Control on?).
- Standard **OK / Cancel / Apply**. OK/Apply call `SSDwellConfig::SaveToRegistry`.

Localization: SimonSays ships multilingual (Spanish primary). Route all dialog
strings through the project's existing string/resource mechanism — do not
hard-code English in the UI even though code identifiers and comments are
English. Ask if you can't find the existing localization mechanism.

## Files to create

- `IGazeProvider.h`, `GazeProviderChain.h/.cpp`
- `HidGazeProvider.h/.cpp` (wraps SSGazeReader), `CursorGazeProvider.h/.cpp`
- `SSGazeReader.h/.cpp` (HID reading — from the Option B design)
- `SSGazeDetect.h/.cpp` (passive detection helpers)
- `SSDwellModeDetector.h/.cpp` (detector + MouseMotionClassifier)
- `SSDwellSettingsDialog.h/.cpp` (+ `.rc` entries if resource-based)
- `docs/Dwell.md` — architecture overview: providers, detector signals,
  registry keys, calibration flow, fallback behavior, how to test.

## Files to modify

- `SSButton.h/.cpp` — add `GetTrackingPoint`; switch dwell evaluation to a
  timer reading the provider chain; keep `WM_MOUSEMOVE`/`WM_MOUSELEAVE` as the
  cursor-mode fast path; keep `WM_LBUTTONDOWN` cancelling dwell (anti double-click).
- `SSDwellConfig.h/.cpp` — mode selection, registry load/save, `ApplyDetectedMode`,
  `GetActiveMode`, ring-vs-bar flag.
- Main entry point — `LoadFromRegistry()` early; `GazeProviderChain::StartAll()`
  after settings load; create the 60 Hz and ~1–3 s timers; forward
  `WM_DEVICECHANGE` to detection; `StopAll()` + `SaveToRegistry()` on shutdown;
  add a menu/command to open the settings dialog.

## Acceptance criteria

1. **Forced modes work exactly as labeled.** Mouse / HID / Off behave
   deterministically; Auto follows the detector with hysteresis.
2. **Concurrent operation.** With each external tool running, dwell works in the
   appropriate mode AND the tool keeps functioning. The HID device opens
   non-exclusively (never `ERROR_SHARING_VIOLATION`).
3. **Graceful degradation.** No eye tracker, no tools → behaves like a plain
   app; Auto resolves to Off and buttons don't auto-fire for a mouse user.
4. **Two-step coverage.** With a device that exposes HID but a tool in a
   no-cursor mode, Auto resolves to HidDwell and dwell works on the buttons.
5. **Calibration authority.** Completing the gaze probe pins the correct mode
   immediately, overriding kinematic guesses, until it expires.
6. **Persistence.** All settings round-trip through the registry; first run uses
   documented defaults; no admin rights needed.
7. **Hot-plug.** Connect/disconnect of an eye tracker updates detection and
   (in Auto) the resolved mode within a couple of seconds without a restart.
8. **DPI / multi-monitor.** Gaze/cursor coordinates hit-test correctly on
   secondary monitors and under per-monitor DPI scaling.
9. **No regressions / no flapping.** Existing cursor-only behavior is unchanged
   when forced to Mouse; Auto does not oscillate between modes on noisy input.
10. **Clean shutdown.** Timers killed, threads joined, HID handle closed,
    settings saved; no leaks under Application Verifier.

## Constraints and conventions

- Win32 + C++, matching the standard already configured in the project (check
  `.vcxproj` / CMake; do not bump it).
- No third-party dependencies. Windows SDK only: link `hid.lib`, `setupapi.lib`,
  `comctl32.lib` (and `comdlg32.lib` for `ChooseColor`).
- All identifiers and code comments in English. All user-visible strings via the
  existing localization mechanism (Spanish primary).
- `std::atomic` for flags; `std::mutex` only around compound state. No exceptions
  across thread boundaries — translate to status/bool returns and log.
- Read the existing `SSButton`/`SSDwellConfig` first and match brace, indent,
  and naming style before writing any new code.

## Suggested implementation order

1. Read existing files; confirm C++ standard, the main entry point name, the
   message-loop structure, and the localization/resource mechanism.
2. Introduce `IGazeProvider` + `CursorGazeProvider` + `GazeProviderChain`.
   Refactor `SSButton` to timer-driven dwell via `GetTrackingPoint`. Verify the
   cursor-only experience is unchanged. (No HID yet.)
3. Add `SSDwellConfig` mode selection + registry load/save + the settings dialog
   wired to forced modes only. Verify persistence.
4. Add passive detection (`SSGazeDetect`) and surface it in the dialog status
   area (still forced modes).
5. Implement `SSGazeReader` + `HidGazeProvider`; add HID to the chain.
6. Wire `SSDwellModeDetector` + `MouseMotionClassifier`; enable Auto mode with
   hysteresis; show reason/confidence in the dialog.
7. Implement the two-probe calibration panel and `ReportCalibration`.
8. DPI/multi-monitor pass; hot-plug via `WM_DEVICECHANGE`; profiling; Verifier.

## Testing notes

- Use a debug-only `InjectFakeSample` on the HID provider and a synthetic cursor
  feeder to exercise the detector and `SSButton` before touching hardware.
- The `MouseMotionClassifier` thresholds (`kTeleportPx`, jitter bands, scoring
  weights) are unvalidated estimates. Plan a calibration experiment: record
  `GetCursorPos` at 60 Hz with each real device in each mode, inspect the
  per-feature distributions, and tune. Surface the thresholds as constants in
  one place to make this easy.
- Verify non-exclusive HID access by running an independent HID dump tool
  alongside SimonSays and confirming both see the stream.
- Microsoft's `GazeHid` reference repo documents the Eye Tracker usage page if
  questions arise about report formats.

## What to ask me before starting

If anything is ambiguous against the real codebase — C++ standard, entry-point
name, message-loop and timer setup, how settings/resources/localization are
handled, or whether the dialog should be resource-based or code-built — stop and
ask. Before writing code, summarize your understanding, list open questions, and
propose a concrete plan for approval. Do not guess on architectural points that
are costly to reverse.
