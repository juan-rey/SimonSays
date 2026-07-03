# SimonSays — Gaze Dwell-Click System — Specification

| | |
|---|---|
| **Spec ID** | DWELL-SPEC |
| **Status** | Active — Phases A–D implemented; HID gaze verified on Irisbond Hiru |
| **Version** | 1.1 (2026-07-03) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |

---

## 0. How to use this spec (for AI agents / contributors)

This document is the **source of truth** for the gaze dwell-click feature. It is
prescriptive: requirements use EARS phrasing ("WHEN … THE SYSTEM SHALL …") and
are numbered for traceability (`REQ-*`). Each requirement carries an
**implementation status** tag:

- **[Done]** — implemented and verified.
- **[Done*]** — implemented; correctness depends on a device-specific assumption
  validated on one device only (see notes).
- **[Partial]** — implemented with a hardcoded/simplified form of the full intent.
- **[Planned]** — specified but not implemented.

Working agreement:
1. Treat `REQ-*` as the contract. When you change behavior, update the matching
   requirement and the **Status matrix (§16)** in the same change.
2. Do **not** regress a **[Done]** requirement to make a **[Planned]** one work.
3. Verify against **Acceptance criteria (§15)** before claiming completion.
4. Honor the **dependency/layering rules (§7)** — `SSButton` must not depend on
   concrete providers or the detector.
5. Match existing code style (brace, indent, naming) and the localization,
   registry, and dialog conventions in **§7.4 / §9 / §11**.
6. If a fact here conflicts with the code, the code wins — fix the spec and flag
   it. Confirm device-specific facts (§Appendix A) against real hardware.

---

## 1. Overview

SimonSays is a Win32 C++ tray/taskbar text-to-speech utility for users with ALS
and speech impairments, many of whom drive the computer with an **eye tracker**.
This feature adds **dwell-click**: activating a custom `SSButton` by holding the
tracking point (mouse cursor, or raw HID gaze) still on it for a configurable
time, with on-screen progress feedback — without blocking the external eye-control
tool and without requiring any tracker SDK for the common cursor-driven case.

The system is structured in three layers so trackers/backends can be added
without changing button code:

1. **Tracking source providers** — abstract where the "where the user is looking"
   point comes from (cursor, HID gaze, future SDKs).
2. **Intelligent mode detection** — choose the right dwell mode automatically from
   fused signals, with user calibration as ground truth.
3. **Configuration, persistence, UI** — a settings window and registry storage.

---

## 2. Background & context

External eye-control tools differ in how they expose gaze:

- **Cursor-moving modes** (Irisbond EasyClick, Tobii Dynavox TD Control cursor
  modes, Windows Eye Control cursor modes, OptiKey Mouse-Movement/Magnetic) move
  the Windows cursor → **cursor dwell works**.
- **Two-step / no-cursor modes** (WEC Precise, TD Control gaze selection) do not
  move the cursor → need to read **gaze directly from HID** (usage page 0x12).
- **Tools that already click** (TD Control dwell, WEC left-click) → our dwell must
  stay out of the way to avoid double activation.

HID eye-tracker support is device- and OS-dependent (e.g., **Tobii 4C does not
expose standard HID on Windows 11**; Irisbond Hiru does in HID mode). The system
must therefore degrade gracefully, never assume a mode, and either detect or let
the user force one.

---

## 3. Goals & non-goals

**Goals**
- Hands-free button activation by gaze for cursor-moving AND HID-only trackers.
- Never block the external tool (non-exclusive device access).
- Automatic mode selection with user calibration as ground truth; manual override.
- Persistent, localized, registry-backed configuration.
- Graceful degradation to a normal mouse app when no tracker is present.

**Non-goals**
- Replacing the external eye-control tool's own calibration/cursor control.
- Implementing a vendor SDK provider now (Tobii Stream Engine, etc.) — see §18.
- Gaze typing / scanning / non-button UI dwell.

---

## 4. Glossary

| Term | Meaning |
|---|---|
| **Dwell-click** | Activating a control by holding the tracking point on it for `DwellTime`. |
| **Tracking point** | The current "where the user points/looks" screen point, from the active provider. |
| **Provider** | An `IGazeProvider` that yields a `GazeSample` (cursor, HID, …). |
| **Provider chain** | `GazeProviderChain` singleton selecting among providers per mode. |
| **Active mode** | The `SSDwellMode` currently in effect (resolved from the user selection + detector). |
| **Mode selection** | The user's choice (`SSDwellModeSelection`): Auto / Off / Mouse / HID. |
| **Gaze router** | The poll that arms the button under the gaze point in HID mode (cursor doesn't move). |
| **Probe** | A calibration button in the dwell window ("LOOK at me" / "use your MOUSE"). |
| **Tolerance radius** | Pixels the tracking point may jitter without resetting the dwell timer. |
| **Cooldown** | Dead time after any activation during which a button won't re-arm. |

---

## 5. Personas & scenarios

- **P1 — Cursor-gaze user**: tracker moves the Windows cursor. Expects Mouse dwell.
- **P2 — HID-gaze user**: tracker streams HID gaze, cursor frozen (e.g., Hiru HID
  mode, two-step modes). Expects HID dwell.
- **P3 — Mouse/switch user**: ordinary cursor by hand. Expects dwell **off** so
  buttons don't auto-fire.
- **P4 — Clicking-tool user**: tool issues its own clicks. Expects our dwell to
  stay out of the way (ExternalClick).

---

## 6. Requirements (EARS)

### 6.1 Tracking sources (Layer 1)

- **REQ-F01 [Done]** THE SYSTEM SHALL define `IGazeProvider` yielding `GazeSample
  { POINT screenPoint (virtual-desktop px); DWORD timestamp; bool valid; }` with
  `Start/Stop/IsLive/GetSample/Name`.
- **REQ-F02 [Done]** THE SYSTEM SHALL provide a `CursorGazeProvider` that returns
  `GetCursorPos` and is always live.
- **REQ-F03 [Done]** THE SYSTEM SHALL provide a process-wide `GazeProviderChain`
  with `StartAll/StopAll`, `SetHidProvider`, `HidLive`, and accessors
  `GetCursorSample`, `GetHidSample`, `GetTrackingSample` (HID if live, else cursor).
- **REQ-F04 [Done]** WHEN a button needs the tracking point, THE SYSTEM SHALL
  obtain it through the chain per the active mode (MouseDwell→cursor; HidDwell→HID
  with cursor fallback; Off/ExternalClick→none).
- **REQ-F05 [Done]** THE chain SHALL accept additional providers in future without
  changes to `SSButton` (e.g., a Tobii Stream Engine provider — see §18).

### 6.2 Dwell behavior on `SSButton`

- **REQ-F10 [Done]** WHILE dwell is active for a button (mode is Mouse/HID, the
  button is enabled and dwell-enabled, and not in cooldown), THE SYSTEM SHALL start
  a fixation when the tracking point enters the button.
- **REQ-F11 [Done]** WHILE a fixation is in progress, THE SYSTEM SHALL advance a
  progress value 0→1 over `DwellTime` and render a bottom-edge progress bar in
  `ProgressColor`.
- **REQ-F12 [Done]** IF the tracking point moves beyond `ToleranceRadius` from the
  fixation anchor, THEN THE SYSTEM SHALL re-anchor and restart the fixation clock.
- **REQ-F13 [Done]** WHEN the fixation reaches `DwellTime`, THE SYSTEM SHALL fire
  `BN_CLICKED` to the parent (identical to a real click), set the activation source
  to `Dwell`, and enter `Cooldown`.
- **REQ-F14 [Done]** WHILE within `Cooldown` after any activation, THE SYSTEM SHALL
  NOT start a new fixation on that button.
- **REQ-F15 [Done]** IF the tracking point leaves the button (or `WM_MOUSELEAVE` in
  cursor mode), THEN THE SYSTEM SHALL cancel the in-progress fixation and clear the
  progress bar.
- **REQ-F16 [Done]** WHEN a real `WM_LBUTTONDOWN` arrives, THE SYSTEM SHALL cancel
  any in-progress dwell and start a cooldown (anti double-activation).
- **REQ-F17 [Done]** THE SYSTEM SHALL allow per-button opt-out via
  `SetDwellEnabled(false)` (e.g., destructive actions), gated under the global mode.
- **REQ-F18 [Done]** THE button SHALL expose its last activation source
  (`None/Mouse/Keyboard/Dwell`) for calibration inference.
- **REQ-F19 [Done]** In HID mode (cursor frozen, no `WM_MOUSEMOVE`), THE SYSTEM
  SHALL arm the button under the gaze point via an external **gaze router**
  (`SSButton::RouteGaze`) driven by a poll; the button's own timer then advances
  and self-cancels. Mouse mode continues to arm via `WM_MOUSEMOVE`.

### 6.3 Mode selection & automatic detection (Layer 2)

- **REQ-F20 [Done]** THE SYSTEM SHALL support four user selections
  (`SSDwellModeSelection`): `Auto`, `ForceOff`, `ForceMouseDwell`, `ForceHidDwell`,
  resolving to an active `SSDwellMode` (`Off`, `MouseDwell`, `HidDwell`,
  `ExternalClick`) via `SSDwellConfig::GetActiveMode()`.
- **REQ-F21 [Done]** WHEN selection is a Force* value, THE active mode SHALL be that
  mode deterministically.
- **REQ-F22 [Done]** WHEN selection is `Auto`, THE active mode SHALL follow the
  detector's most recent applied decision (`detectedMode`), defaulting to `Off`.
- **REQ-F23 [Done]** THE SYSTEM SHALL run `SSDwellModeDetector` fusing, in priority
  order: (1) fresh calibration (authoritative ≤10 min), (2) a known clicking tool →
  `ExternalClick`, (3) the no-presence gate (REQ-F27) → `Off`, (4) live HID,
  (5) cursor kinematics (`MouseMotionClassifier`), (6) passive presence;
  producing `{mode, confidence, reason}`.
- **REQ-F24 [Done]** WHILE selection is `Auto`, THE SYSTEM SHALL refresh passive
  signals + HID-liveness (~3 s), and sample the cursor (~30 Hz) **only while
  eye-tracking presence is detected** (REQ-F27) to feed the detector.
- **REQ-F25 [Done*]** THE SYSTEM SHALL apply a new Auto decision only after it
  repeats for `HysteresisCount` consecutive evaluations (anti-flapping). *Count is
  hardcoded `3` (`DWELL_HYSTERESIS_COUNT`); not yet registry-configurable (§18).*
- **REQ-F26 [Done]** WHEN the Auto-resolved mode changes, THE SYSTEM SHALL persist
  it (`dwellDetectedMode`) so Auto restores it next launch.
- **REQ-F27 [Done]** WHILE selection is `Auto` AND no eye-tracking presence is
  detected (no HID eye tracker, no known eye-control tool, no Windows Eye
  Control, no live HID stream), THE SYSTEM SHALL resolve the detected mode to
  `Off` (never enabling dwell from cursor kinematics alone) AND suspend the
  ~30 Hz cursor-kinematics sampling until presence appears, to avoid background
  cost when no tracker can be used. Fresh calibration (REQ-F44) still takes
  priority over this gate while valid.

### 6.4 Passive detection

- **REQ-F30 [Done]** THE SYSTEM SHALL detect HID eye-tracker presence by enumerating
  HID interfaces (usage page `0x12`, usage `0x01`), opening with access `0`
  (query-only, never blocking the tool).
- **REQ-F31 [Done]** THE SYSTEM SHALL detect a running eye-control tool via a
  Toolhelp32 process scan against a known-tools table (EasyClick, Hiru, TD Control,
  OptiKey, `tobii*`), case-insensitive, flagging tools known to issue their own
  clicks.
- **REQ-F32 [Done*]** THE SYSTEM SHALL report whether Windows Eye Control is enabled
  by reading its CloudStore blob (see §Appendix B). *Verified ON/OFF on one machine.*
- **REQ-F33 [Done]** THE SYSTEM SHALL aggregate the above into `EyeTrackingStatus`
  via `DetectEyeTracking()`.

### 6.5 Calibration (ground truth)

- **REQ-F40 [Done]** THE dwell window SHALL present two probe buttons: a gaze probe
  ("activate by LOOKING", dwell-enabled) and a manual probe ("activate with
  MOUSE/switch", dwell-disabled).
- **REQ-F41 [Done]** WHEN the gaze probe is activated by **dwell** AND a valid HID
  sample was inside it, THE SYSTEM SHALL report `ConfirmedHidGaze`; ELSE (gaze probe
  by dwell) `ConfirmedCursorIsGaze`.
- **REQ-F42 [Done]** WHEN the manual probe is activated, THE SYSTEM SHALL report
  `ConfirmedMouseIsHand`.
- **REQ-F43 [Done]** WHEN the gaze probe is activated by a physical **click** (not
  dwell), THE SYSTEM SHALL NOT record calibration and SHALL prompt the user to look.
- **REQ-F44 [Done]** Calibration SHALL be authoritative in the detector while fresh
  (≤ `CalibrationValidMs`, hardcoded 10 min).

### 6.6 HID gaze reader (Layer 1 backend)

- **REQ-F50 [Done]** THE SYSTEM SHALL read gaze from a HID eye tracker on a
  background thread, opening **non-exclusively** (`GENERIC_READ`,
  `FILE_SHARE_READ|WRITE`, `OVERLAPPED`).
- **REQ-F51 [Done]** THE reader SHALL self-heal: if no device is present or one is
  removed, it idles and retries (`kRetryMs` = 2000 ms); `Wake()` triggers an
  immediate re-attempt (used on device change).
- **REQ-F52 [Done]** `HasLiveGaze()` SHALL be true only while a valid sample arrived
  within `kMaxAgeMs` (200 ms); otherwise the chain falls back to cursor.
- **REQ-F53 [Done*]** THE reader SHALL parse the gaze point from the input report
  and map it to virtual-desktop pixels (see §Appendix A). *Field layout/units are
  confirmed for the Irisbond Hiru only; other devices may differ (§17, §18).*
- **REQ-F54 [Done]** THE reader SHALL skip the device "no gaze" sentinel (both axes
  zero) so it does not park the pointer at the origin.
- **REQ-F55 [Done]** ON shutdown THE reader SHALL signal stop, cancel I/O, join the
  thread, and close the handle (no leaks).
- **REQ-F56 [Done]** THE reader SHALL expose `InjectFakeSample(POINT)` and a
  compile-time `SSGAZE_DEBUG_FEED_CURSOR` (default 0) for hardware-free testing.

### 6.7 Configuration, persistence, UI (Layer 3)

- **REQ-F60 [Done]** `SSDwellConfig` SHALL be a thread-safe process-wide singleton
  holding dwell time, tolerance, cooldown, progress color, mode selection, and the
  detected mode, with live getters/setters.
- **REQ-F61 [Done]** Dwell settings SHALL persist in the existing `Settings` struct
  via `RegistryManager` (HKCU, `REG_SZ`), with documented defaults on first run.
- **REQ-F62 [Done]** A separate **Gaze / Dwell-click window** (`IDD_DWELL_DIALOG`)
  SHALL provide: mode radios (Auto/Mouse/HID/Off), dwell-time/tolerance/cooldown
  sliders with numeric readouts, a progress-color picker (`ChooseColor`), a live
  "Detected" panel, two calibration probes, a Reset button (REQ-F67), and
  OK/Cancel.
- **REQ-F63 [Done]** THE HID radio SHALL be enabled only when a HID eye tracker is
  present; the "Detected" HID line SHALL distinguish *none / present (not streaming)
  / streaming gaze*.
- **REQ-F64 [Done]** WHILE the dwell window is open, THE SYSTEM SHALL force a mode
  that exercises the probes — HID if HID is live, else Mouse — apply tuning live,
  and restore prior config on Cancel / commit on OK.
- **REQ-F65 [Done]** THE window SHALL open from a tray menu entry and the **F3**
  accelerator (`ID_DWELL_OPEN`). (F3 was swapped from add-after-selection, now F7.)
- **REQ-F66 [Done]** All user-visible strings SHALL route through
  `GetLocalizedString`; English + Spanish authored, other languages fall back to
  English.
- **REQ-F67 [Done]** THE dwell window SHALL provide a **Reset** button at the
  bottom-left that restores every dwell setting to its first-run default
  (mode selection Auto, time 800 ms, tolerance 35 px, cooldown 300 ms, progress
  color = Windows accent color, detected mode Off), updating the dialog controls
  and applying the tuning live to the probes; the reset is committed only on OK
  and discarded on Cancel. Defaults are single-sourced (`DWELL_DEFAULT_*` in
  `stdafx.h`, shared with `RegistryManager`).

### 6.8 Lifecycle, hot-plug, DPI

- **REQ-F70 [Done]** AT startup THE app SHALL install the HID provider, `StartAll`
  the chain, load settings into `SSDwellConfig`, and start the detector/gaze timers.
- **REQ-F71 [Done]** ON `WM_DEVICECHANGE` (DBT_DEVNODES_CHANGED, debounced ~800 ms)
  THE SYSTEM SHALL re-probe HID (`Wake`) and refresh detection within ~1–2 s.
- **REQ-F72 [Done]** Gaze/cursor coordinates SHALL hit-test correctly across
  monitors. *The process is DPI-unaware, so cursor, window, and gaze share one
  virtualized coordinate space; HID gaze maps to the primary monitor.*
- **REQ-F73 [Done]** ON shutdown THE SYSTEM SHALL kill timers, `StopAll` the chain
  (joining the HID thread), and save settings.

### 6.9 Diagnostics

- **REQ-F80 [Done]** WHEN env var `SIMONSAYS_HID_DUMP` is set, THE reader SHALL
  write a diagnostic report to `%LocalAppData%\SimonSays\hid_dump.txt` containing:
  app/OS/display/DPI; passive detection; every HID eye-tracker device (VID/PID/
  version, manufacturer/product strings, report lengths); HID capabilities and
  value caps; a decoded gaze capture (raw + mapped px); and a min/max gaze-range
  summary. It SHALL run once at reader-thread start and never in normal use.

### 6.10 Non-functional

- **REQ-N01 [Done]** Win32 + C++ at the project's standard (MSVC default ≈ C++14,
  toolset v145); SHALL NOT bump the standard.
- **REQ-N02 [Done]** Windows SDK only; no third-party deps. Links `hid.lib`,
  `setupapi.lib`, `comdlg32.lib` (and existing `d2d1`/`dwrite`/`msimg32`/`comctl32`).
- **REQ-N03 [Done]** `std::atomic` for flags; `std::mutex` for compound state; no
  exceptions across thread boundaries (status/bool returns).
- **REQ-N04 [Done]** Concurrent operation: the HID device opens non-exclusively;
  the external tool keeps working (never `ERROR_SHARING_VIOLATION`).
- **REQ-N05 [Done]** No regression to existing cursor-only behavior when forced to
  Mouse; Auto SHALL NOT oscillate (hysteresis).
- **REQ-N06 [Done]** Identifiers and comments in English; UI localized.
- **REQ-N07 [Planned]** Clean under Application Verifier (timers killed, thread
  joined, handle closed, no leaks). *Designed for; not yet run under Verifier.*

---

## 7. Architecture & components

### 7.1 Layer diagram

```
            ┌─────────────────────────── UI / app ───────────────────────────┐
            │ MainWindow (timers, tray, WM_DEVICECHANGE)   SSDwellWindow      │
            └───────────────┬───────────────────────────────────┬────────────┘
                            │ owns timers, wiring                │ settings + probes
        ┌───────────────────▼─────────────┐         ┌────────────▼───────────┐
        │ SSDwellModeDetector (+Classifier)│◀────────│ SSGazeDetect (passive) │
        └───────────────────┬─────────────┘  signals └────────────────────────┘
                            │ decision
        ┌───────────────────▼─────────────┐
        │ SSDwellConfig (mode + tuning)    │
        └───────────────────┬─────────────┘ active mode
        ┌───────────────────▼─────────────┐         ┌────────────────────────┐
        │ SSButton (dwell + RouteGaze)     │────────▶│ GazeProviderChain      │
        └──────────────────────────────────┘ get pt  └───────┬─────────┬──────┘
                                                              │         │
                                            CursorGazeProvider│         │HidGazeProvider→SSGazeReader(HID thread)
```

### 7.2 New files

| File | Responsibility |
|---|---|
| `include/IGazeProvider.h` | `GazeSample`, `IGazeProvider` interface |
| `include/CursorGazeProvider.h` / `src/CursorGazeProvider.cpp` | Cursor provider |
| `include/GazeProviderChain.h` / `src/GazeProviderChain.cpp` | Provider chain singleton |
| `include/HidGazeProvider.h` / `src/HidGazeProvider.cpp` | HID provider (wraps reader) |
| `include/SSGazeReader.h` / `src/SSGazeReader.cpp` | HID reader thread + diagnostics |
| `include/SSGazeDetect.h` / `src/SSGazeDetect.cpp` | Passive detection |
| `include/SSDwellModeDetector.h` | Detector + `MouseMotionClassifier` (header-only) |
| `include/SSDwellWindow.h` / `src/SSDwellWindow.cpp` | Dwell settings window + probes |

### 7.3 Modified files

`SSButton.{h,cpp}` (config refactor + dwell + `RouteGaze`), `MainWindow.{h,cpp}`
(timers, wiring, hot-plug), `CategoryWindow.cpp` (F-key handler), `RegistryManager.cpp`
(dwell persistence), `stdafx.h` (Settings fields + string IDs), `localized_strings.h`
(EN/ES strings), `resources/SimonSays.rc` + `resource.h` (dialog, IDs, accelerators),
`SimonSays.vcxproj(.filters)`.

### 7.4 Dependency / layering rules

- `SSButton` depends on `SSDwellConfig` and `GazeProviderChain` only — **never** on
  concrete providers or the detector.
- The detector depends on the classifier + `SSGazeDetect` signals (pushed in), not
  on the chain directly (HID-liveness is pushed via `SetHidLive`).
- `RegistryManager` stays decoupled from `SSDwellConfig`; `MainWindow` bridges
  `Settings` ↔ `SSDwellConfig` (`ApplyDwellSettingsToConfig`).
- `SSDwellMode` / `SSDwellModeSelection` are defined once in `SSButton.h` and reused
  everywhere (the detector reference's duplicate enums were removed).

---

## 8. Detailed design

### 8.1 Dwell state machine (per `SSButton`)

States: **Idle → Dwelling → (Fire) → Cooldown → Idle**.

- Arming (enter Dwelling): cursor mode via `WM_MOUSEMOVE`→`OnDwellMouseMove`; HID
  mode via the gaze router→`OnDwellMouseMove`. Guards: `IsDwellActive()` (active
  mode is Mouse/HID), `m_dwellEnabled`, button enabled, `now ≥ cooldownUntil`.
- `StartDwell`: set anchor = point, start tick = now, progress = 0, `SetTimer`
  (`SSBUTTON_DWELL_TIMER_ID` 0xD3E11, 33 ms ≈ 30 fps).
- `UpdateDwell` (each tick): re-read the tracking point via `GetTrackingPoint`
  (provider per active mode → `ScreenToClient`). If outside client rect → cancel.
  If moved > tolerance → re-anchor + restart clock. When `elapsed ≥ DwellTime`:
  kill timer, set cooldown = now + `CooldownMs`, set activation = `Dwell`,
  `FireClick`.
- Cancel: kill timer, clear progress, repaint.

### 8.2 Gaze router (HID activation)

`SSButton::RouteGaze(screenPt)` (static): `WindowFromPoint` → if class is
`SSBUTTON_CLASS` → get instance via `GWLP_USERDATA` → `ScreenToClient` → if the
button is not already dwelling, `OnDwellMouseMove`. The button's own timer then
advances and self-cancels.

`MainWindow` runs an **always-on** `TIMER_DWELL_GAZE` (id 5, 50 ms). When
`GetActiveMode()==HidDwell` it reads `GetTrackingSample` and calls `RouteGaze`. It
is always-on (not gated by `SyncDwellTimers`) so it also fires during the modal
dwell window (whose message loop dispatches the main window's `WM_TIMER`), arming
the probes. Mouse mode does **not** use the router (unchanged `WM_MOUSEMOVE` path).

### 8.3 Provider selection per mode

`GetTrackingPoint`: MouseDwell → `GetCursorSample`; HidDwell → `GetTrackingSample`
(HID if live, else cursor fallback); Off/ExternalClick → false. `GetTrackingSample`
prefers a live HID sample, otherwise the cursor.

### 8.4 Detector decision (`Decide()`)

Priority: (1) fresh calibration → mapped mode at high confidence; (2)
`toolKnownToClick` → `ExternalClick`; (3) no presence at all (no HID device, no
known tool, no Windows Eye Control, no live HID) → `Off` (REQ-F27); (4) `hidLive`
→ `MouseDwell` if cursor also gaze-like else `HidDwell`; (5) cursor kinematics
(presence guaranteed here) → frozen→`Off`, gaze-like→`MouseDwell`, else
`MouseDwell` (ambiguous); (6) not enough data → `MouseDwell` (warming).
`MainWindow` mirrors the gate: `SyncDwellSampleTimer` runs `TIMER_DWELL_SAMPLE`
only while the last passive scan (or live HID) found presence
(`m_dwellPresence`), and `UpdateDwellPassiveSignals` toggles it on transitions;
`SyncDwellTimers` refreshes the passive scan immediately when Auto starts so the
sampler decision doesn't wait for the first ~3 s tick. `MouseMotionClassifier`
scores teleports/jitter/bimodal velocity over a 120-sample window (≥45 to judge);
thresholds are collected as constants and are **unvalidated estimates**.

### 8.5 Calibration inference (dwell window probes)

Gaze probe fired by `Dwell`: HID sample inside the probe rect → `ConfirmedHidGaze`
(detected=HID); else `ConfirmedCursorIsGaze` (detected=Mouse). Manual probe →
`ConfirmedMouseIsHand` (detected=Off). Each calls `ReportCalibration` and reflects
the result in the radios. Gaze probe fired by physical click → hint to look.

### 8.6 HID report parsing — see §Appendix A (device-specific).

### 8.7 Windows Eye Control detection — see §Appendix B (CloudStore blob).

---

## 9. Data model & persistence

### 9.1 Enums (defined in `SSButton.h`)

```cpp
enum class SSDwellMode          { Off=0, MouseDwell=1, HidDwell=2, ExternalClick=3 };
enum class SSDwellModeSelection { Auto=0, ForceOff=1, ForceMouseDwell=2, ForceHidDwell=3 };
enum class SSButtonActivation   { None, Mouse, Keyboard, Dwell };
enum class CalibrationOutcome   { None, ConfirmedHidGaze, ConfirmedCursorIsGaze, ConfirmedMouseIsHand };
```

### 9.2 `Settings` fields (in `stdafx.h`) — stored enum-free as ints

| Field | Meaning | Maps to |
|---|---|---|
| `int dwellModeSelection` | 0 Auto · 1 Off · 2 Mouse · 3 HID | `SSDwellModeSelection` |
| `int dwellTimeMs` | Fixation time (ms) | `SSDwellConfig` |
| `int dwellToleranceRadius` | Jitter radius (px) | |
| `int dwellCooldownMs` | Post-activation dead time (ms) | |
| `COLORREF dwellProgressColor` | Progress bar color | |
| `int dwellDetectedMode` | Auto restore: 0 Off · 1 Mouse · 2 HID · 3 ExternalClick | `SSDwellMode` |

### 9.3 Registry (HKCU, under the existing SimonSays settings key, all `REG_SZ`)

| Value name | Default |
|---|---|
| `Dwell Mode Selection` | `0` (Auto) |
| `Dwell Time Ms` | `800` |
| `Dwell Tolerance Radius` | `35` |
| `Dwell Cooldown Ms` | `300` |
| `Dwell Progress Color` | Windows accent color (`GetAccentColor()`) |
| `Dwell Detected Mode` | `0` (Off) |

Load happens in `RegistryManager::LoadSettingsFromRegistry`; `MainWindow` pushes
values into `SSDwellConfig` via `ApplyDwellSettingsToConfig`. Save rides the
existing settings save (dwell-window OK and app shutdown). Numeric parse uses
`wcstol/wcstoul` (no throw on malformed values).

Numeric defaults are single-sourced as `DWELL_DEFAULT_*` in `stdafx.h` (shared
by `RegistryManager` and the dwell window's Reset button). The progress color is
special: `Dwell Progress Color` is only written when it differs from the current
accent color, and the value is **deleted** when it matches, so an untouched (or
reset) color keeps following the Windows accent color across theme changes.

---

## 10. Key interfaces

```cpp
// GazeProviderChain (singleton)
static GazeProviderChain& Instance();
void StartAll(); void StopAll();
void SetHidProvider(std::unique_ptr<IGazeProvider>);
bool HidLive() const;
bool GetCursorSample(GazeSample*) const;
bool GetHidSample(GazeSample*) const;
bool GetTrackingSample(GazeSample*) const;   // HID if live, else cursor

// SSDwellConfig (singleton, thread-safe)
void SetDwellTimeMs(UINT); UINT GetDwellTimeMs() const;        // + tolerance, cooldown
void SetProgressColor(COLORREF); COLORREF GetProgressColor() const;
void SetModeSelection(SSDwellModeSelection); SSDwellModeSelection GetModeSelection() const;
void ApplyDetectedMode(SSDwellMode); SSDwellMode GetDetectedMode() const;
SSDwellMode GetActiveMode() const; bool IsDwellActive() const;  // Mouse||Hid

// SSButton (dwell surface)
void SetDwellEnabled(bool); bool IsDwellEnabled() const;
SSButtonActivation GetLastActivation() const;
static bool RouteGaze(POINT screenPt);        // arm the button under the gaze point

// SSDwellModeDetector (singleton, header-only)
void Tick(); void UpdatePassive(const PassiveSignals&); void SetHidLive(bool);
void ReportCalibration(CalibrationOutcome); Decision Decide() const; void ResetMotion();

// SSGazeDetect (free functions)
bool IsHidEyeTrackerPresent();
bool DetectActiveEyeTrackingTool(EyeTrackingTool*);
bool IsWindowsEyeControlEnabled();
bool IsClickingTool(const EyeTrackingTool&);
EyeTrackingStatus DetectEyeTracking();

// SSGazeReader (singleton)
static SSGazeReader& Instance();
bool Start(); void Stop(); bool HasLiveGaze() const; bool GetLatest(GazeSample*) const;
void InjectFakeSample(POINT); void Wake();

// SSDwellWindow
bool ShowDwellSettingsDialog(HWND owner, HINSTANCE, Settings&);  // true on OK
```

---

## 11. UI specification

- **Dialog** `IDD_DWELL_DIALOG` (resource-based, populated programmatically), opened
  modally via `DialogBoxParam`. Controls (`resource.h` IDs `IDC_DWELL_*` 6001–6028):
  intro text; **Activation mode** group with auto-radios Auto/Mouse/HID/Off; three
  sliders (`msctls_trackbar32`) + numeric edits for **dwell time (300–2000 ms)**,
  **tolerance (15–80 px)**, **cooldown (0–1000 ms)**; **progress color** button
  (`ChooseColor`); **Detect** group with two probe placeholders + a status line;
  **Detected** group with three signal lines (HID / eye-control app / Windows Eye
  Control), refreshed on a ~2 s timer; a bottom-left **Reset** button
  (`IDC_DWELL_RESET`, REQ-F67) restoring first-run defaults; **OK / Cancel**.
- **Probes** are `SSButton`s created over the placeholder rects at `WM_INITDIALOG`
  (look probe dwell-enabled; mouse probe dwell-disabled), using the dialog font.
- Reuse helpers `ConfigureSlider`, `SyncSliderToEdit`, `SyncEditToSlider`; RTL via
  `updateRtlExStyle` + `ApplyRtlStylesCallback`.
- **Entry points**: tray menu item `ID_TRAY_DWELL` ("Gaze / Dwell-click…") and
  accelerator **F3** `ID_DWELL_OPEN`.

### 11.1 Accelerators (current, after the F3/F7 swap)

`F1` Help · `F2` Settings · **`F3` Gaze/Dwell window** · `F4` Edit · `F5/F6` move ·
**`F7` Add-after-selection** · `F8` Delete · `F9` Import · `F10` Export. The table is
`IDR_MAINACCEL`; `CategoryWindow` also handles these keys directly (shadowed by the
accelerator in the normal message loop).

---

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| Dwell timer interval | 33 ms | `SSButton.cpp` `SSBUTTON_DWELL_INTERVAL` |
| Progress bar height | 4 px | `SSButton.cpp` |
| HID sample max age | 200 ms | `SSGazeReader.cpp` `kMaxAgeMs` |
| HID open retry | 2000 ms | `SSGazeReader.cpp` `kRetryMs` |
| Detector cursor sample | 33 ms (only while presence detected, REQ-F27) | `MainWindow.cpp` `TIMER_DWELL_SAMPLE` |
| Detector passive refresh | 3000 ms | `TIMER_DWELL_DETECT` |
| Gaze router poll | 50 ms | `TIMER_DWELL_GAZE` |
| Device-change debounce | 800 ms | `TIMER_DWELL_DEVCHANGE` |
| Hysteresis count | 3 | `DWELL_HYSTERESIS_COUNT` |
| Calibration valid | 10 min | `SSDwellModeDetector.h` `kCalibrationValidMs` |
| Classifier window / min | 120 / 45 samples | `SSDwellModeDetector.h` |
| Classifier thresholds | `kStillPx 1.5`, `kTeleportPx 150`, `kFixationSpeed 300`, `kJitterLow 0.8`, `kJitterHigh 2.0` | `SSDwellModeDetector.h` (**unvalidated**) |
| Slider ranges | time 300–2000, tol 15–80, cooldown 0–1000 | `SSDwellWindow.cpp` |
| First-run defaults | mode Auto, 800 ms, 35 px, 300 ms, detected Off | `stdafx.h` `DWELL_DEFAULT_*` (color: accent via `GetAccentColor()`) |

---

## 13. Diagnostics & device bring-up

To bring up a **new** HID eye tracker on any machine:

1. With the tracker's HID/streaming mode ON, set env `SIMONSAYS_HID_DUMP=1` and run
   the Release exe; slowly sweep gaze to all four screen corners in the first ~4 s.
2. Read `%LocalAppData%\SimonSays\hid_dump.txt`. Use the **value caps** + **per-report
   decoded usages** + **mapped px / min-max summary** to identify the gaze X/Y
   usages, units, origin, and axis assignment (compare against the display geometry
   the report prints).
3. Encode findings in `SSGazeReader.cpp` (`kGazeUsageX/Y` + `SelectGazeFields` +
   `MapGazeUmToPx`). Validate live via the dwell window LOOK probe.

Hardware-free pipeline testing: set `SSGAZE_DEBUG_FEED_CURSOR 1` (feeds the cursor
as synthetic HID), or call `SSGazeReader::InjectFakeSample`.

### 13.1 Application Verifier procedure (REQ-N07 / AC-10) — pending manual run

Requires an **elevated** prompt (Verifier writes HKLM Image-File-Execution-Options
keyed by the exe name). Validates the shutdown-critical concerns: HID thread join,
device-handle close, timer cleanup, and heap/handle leaks at exit.

```cmd
:: 1) Enable standard checks for the image (admin):
appverif -enable Heaps Handles Locks Memory Exceptions Leak -for SimonSays.exe

:: 2) Run the Debug build (clearer stacks): x64\Debug\SimonSays.exe
::    (taskbar must be at the bottom or the app exits early with a message box)

:: 3) Exercise dwell paths: open the Gaze/Dwell window (F3), run the LOOK probe,
::    adjust sliders + color, OK; set HID mode and dwell-activate buttons; unplug
::    and replug the tracker (hot-plug); open/close the dwell window a few times.

:: 4) Exit CLEANLY via tray icon -> Exit (NOT Task Manager / kill). This is the
::    real teardown: ID_TRAY_EXIT -> DestroyWindow -> WM_DESTROY ->
::    GazeProviderChain::StopAll() (joins the HID thread, closes the handle) +
::    KillTimer(all) + PostQuitMessage; force-kill would skip these checks.

:: 5) Review: attach WinDbg/cdb for live stops, or open appverif.exe (GUI) to see
::    logged stops per image, or export:
appverif -export log -for SimonSays.exe -with To=%USERPROFILE%\Desktop\appverif_simonsays.xml

:: 6) ALWAYS disable when done (else the app keeps running under Verifier):
appverif -disable * -for SimonSays.exe
```

> **Note (learned 2026-06-22):** Verifier is IFEO-based and keyed by exe name, so
> enabling it once (admin) instruments the app **however it is later launched** —
> you do NOT need to launch the app from the elevated prompt. In fact, launching it
> from an elevated automation context made it hit its bottom-taskbar guard and not
> reach steady state. Correct flow: enable elevated (step 1) → **launch the app
> normally** (double-click, taskbar at bottom) and exercise it (steps 2–4) → then
> export + `-disable` from the elevated prompt (steps 5–6). `appverif -disable`
> leaves an empty `…\Image File Execution Options\SimonSays.exe` IFEO key — harmless
> (no `GlobalFlag`/`VerifierDlls` ⇒ not instrumented); delete it manually if desired.

---

## 14. Edge cases & error handling

- HID device absent/removed → reader idles & retries; `IsLive` false → chain uses
  cursor; no UI error.
- Provisional HID mapping wrong → gaze lands off-target → button never arms → falls
  back to cursor; **no crash, no regression**.
- `(0,0)` HID sentinel (tracking lost / blink) → skipped.
- Malformed registry values → benign numeric fallback (`wcstol`), defaults preserved.
- Eye Control CloudStore key absent → reported as not enabled.
- Modal dialogs (Edit/Settings) running their own loops → the main accelerator/gaze
  timer is not active there; the dwell window relies on the always-on main-window
  `WM_TIMER` being dispatched by the modal loop (verified).
- Cooldown prevents gaze + physical double-activation.

---

## 15. Acceptance criteria (testable)

- **AC-1 (REQ-F20/21) [Pass]** Forced Mouse / HID / Off behave deterministically.
- **AC-2 (REQ-N04) [Pass]** With an external tool running, dwell works in the right
  mode AND the tool keeps functioning; HID opens non-exclusively.
- **AC-3 (REQ-F22/F24/F27, P3) [Pass]** No tracker/tools → Auto resolves to Off
  deterministically (kinematics can no longer enable dwell without presence); a
  mouse user sees no auto-firing.
- **AC-4 (REQ-F19/F50–F53, P2) [Pass — Hiru]** HID-only tracker (cursor frozen) →
  HID dwell activates buttons. *Verified on Irisbond Hiru.*
- **AC-5 (REQ-F40–F44) [Pass]** Completing the gaze probe pins the correct mode and
  the detector trusts it while fresh.
- **AC-6 (REQ-F61) [Pass]** All settings round-trip through the registry; first run
  uses documented defaults; HKCU only, no admin.
- **AC-7 (REQ-F71) [Pass]** Connect/disconnect updates detection and (Auto) the mode
  within ~1–2 s without restart.
- **AC-8 (REQ-F72) [Pass]** Hit-testing correct on secondary monitors (DPI-unaware
  shared coordinate space).
- **AC-9 (REQ-N05) [Pass]** Cursor-only behavior unchanged when forced to Mouse;
  Auto does not oscillate.
- **AC-10 (REQ-N07) [Pending — inconclusive attempt 2026-06-22]** Clean shutdown
  under Application Verifier. An elevated automated pass enabled Verifier (Heaps,
  Handles, Locks, Memory, Exceptions, Leak), launched the app, force-exited it, and
  disabled Verifier (machine left clean). **No Verifier stop fired during startup**
  (weak positive), but the app did not reach steady state when launched from the
  elevated automation (its bottom-taskbar guard blocked before the window / HID
  thread / timers came up), so the **clean-shutdown teardown was not validated**.
  Re-run via §13.1 by launching the app **normally** (see note there).
- **AC-11 (REQ-F27) [Pending]** With no tracker/tool present and selection Auto,
  `TIMER_DWELL_SAMPLE` is not running (no 30 Hz background sampling); plugging a
  tracker in (or starting a known tool) starts it within ~3 s (or ~1–2 s via
  device-change), and removing it stops it again.
- **AC-12 (REQ-F67) [Pending]** Clicking Reset in the dwell window sets the
  radios to Auto, sliders/edits to 800/35/300, and the progress color to the
  Windows accent color; OK persists the defaults (color value removed from the
  registry), Cancel restores the pre-dialog settings.

Build gate: Debug **and** Release x64 compile clean (only the pre-existing
`CategoryWindow.cpp` C4267 warnings).

---

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Provider chain + cursor provider | ✅ Done | |
| `SSButton` dwell + progress + cooldown | ✅ Done | Verified cursor + HID |
| Gaze router (HID activation) | ✅ Done | Verified on Hiru |
| `SSDwellConfig` + registry persistence | ✅ Done | |
| Dwell window + probes + signals | ✅ Done | |
| Passive detection (`SSGazeDetect`) | ✅ Done | Eye Control verified ON/OFF |
| Detector + classifier + Auto + hysteresis | ✅ Done | Classifier thresholds unvalidated; hysteresis hardcoded 3 |
| Auto no-presence gate + sampler suspension (REQ-F27) | ✅ Done | Manual AC-11 check pending |
| Dwell window Reset button (REQ-F67) | ✅ Done | Manual AC-12 check pending |
| HID reader (`SSGazeReader`) | ✅ Done | Non-exclusive, self-healing, clean shutdown |
| HID gaze parse + µm→px mapping | ⚠️ Done\* | Confirmed Irisbond Hiru only |
| Calibration → `ReportCalibration` | ✅ Done | |
| Hot-plug (`WM_DEVICECHANGE`) | ✅ Done | |
| Diagnostics dump | ✅ Done | Full environment report |
| Application Verifier pass | ⚠️ Attempted (inconclusive) | Elevated pass ran + machine left clean, but app didn't reach steady state under automated launch; re-run by launching normally (§13.1 / AC-10) |
| Tobii Stream Engine provider | ⛔ Planned | For 4C/5L etc. (no standard HID) |
| Registry-config hysteresis / calib-validity / ring indicator | ⛔ Planned | Hardcoded today |
| In-app Help text for F3/F7 | ⛔ Planned | `HELP_CONTENT_ID` still lists F3=add |

---

## 17. Known limitations & provisional items

- **HID parse is device-specific.** Gaze usages (`0x21`=X, `0x22`=Y), micrometer
  units, and top-left origin are confirmed for the **Irisbond Hiru** only. Other HID
  trackers may use different usages/units/origin; bring them up via §13 and adjust
  `SSGazeReader.cpp`. Wrong mapping degrades safely to cursor.
- **Tobii 4C** does not expose standard HID on Windows 11 → no HID dwell; use Mouse
  dwell (if the Tobii software drives the cursor). A future SDK provider is needed.
- **Classifier thresholds** are estimates; Auto's kinematic branch needs tuning with
  recorded per-device data.
- **Windows Eye Control** detection relies on an undocumented CloudStore byte marker
  (§Appendix B) verified on one machine; could change across Windows builds.
- **HID gaze** maps to the **primary** monitor only.
- **Unknown cursor-moving trackers get no Auto dwell.** Since REQ-F27, a tracker
  whose software is not in the known-tools table (Appendix C) and that exposes
  neither HID nor Windows Eye Control resolves to `Off` in Auto. Such users can
  force Mouse dwell, or run the LOOK probe — calibration overrides the gate, but
  only while fresh (≤10 min). Extend the known-tools table for such devices.

---

## 18. Future work / out of scope

- `TobiiStreamsGazeProvider` (or other vendor SDK provider) plugged via
  `GazeProviderChain::SetHidProvider`-style extension — for trackers without HID.
- Validate/extend HID parsing for more devices; per-device profiles.
- Make `HysteresisCount`, `CalibrationValidMs`, and a **bar-vs-ring** progress
  indicator registry-configurable + add UI (a `ShowProgressRing` option).
- Multi-monitor HID gaze mapping.
- Update in-app Help (`HELP_CONTENT_ID`, all languages) for the F3/F7 change.
- Run and clear Application Verifier (REQ-N07 / AC-10).
- Tune classifier thresholds from a calibration experiment (record `GetCursorPos`
  at 60 Hz per device/mode).

---

## 19. Open questions

1. **HID coordinate origin across devices** — is top-left-of-primary universal, or
   do some devices use center/normalized? (Hiru = top-left µm.)
2. **Auto vs. explicit** — should first run default `Auto` (current) or prompt the
   user to run calibration once?
3. **Re-fire policy** — holding gaze on a button re-fires every `DwellTime+Cooldown`
   (matches cursor dwell). Confirm this is desired for all button types.
4. **ExternalClick UX** — when a clicking tool is detected, should the UI surface a
   hint, or silently disable our dwell? (Currently the detector returns
   `ExternalClick`; no explicit user messaging.)

---

## 20. Build & run

- Build: `MSBuild SimonSays.vcxproj /p:Configuration={Debug|Release} /p:Platform=x64`.
- Verify features against the **`x64\Release\SimonSays.exe`** build (not the
  installed Program Files app).
- New files are registered in `SimonSays.vcxproj` + `.filters`
  (`SSDwellModeDetector.h` is header-only — `ClInclude`, no `.cpp`).

---

## Appendix A — Irisbond Hiru HID gaze report (confirmed)

- Application: usage page `0x12`, usage `0x01` (Eye Tracker).
- Input report **ID `0x10`**, length **65 bytes**. Relevant value usages
  (32-bit signed, logical range ±2,000,000, **micrometers** on the screen plane):
  - `0x20` — 8-byte timestamp/status (report bytes 1–8).
  - **`0x21` — gaze X**, report byte offset **9**.
  - **`0x22` — gaze Y**, report byte offset **13**.
  - `0x23` — Z (always 0 here), ignored.
- Mapping to pixels: `px = clamp(value_µm / screen_µm) * screen_px`, where
  `screen_µm = GetDeviceCaps(HORZSIZE/VERTSIZE)*1000` and `screen_px =
  GetSystemMetrics(SM_CXSCREEN/SM_CYSCREEN)`; origin top-left. Verified by a corner
  sweep: `0x21` spanned 0..260000 µm (= 260 mm screen width); `0x22` stayed within
  the 173 mm height.
- `(0,0)` on both axes is the "no valid gaze" sentinel → skipped.
- Constants live in `SSGazeReader.cpp`: `kGazeUsageX=0x21`, `kGazeUsageY=0x22`. To
  re-validate or port to another device, swap/adjust these per a §13 dump.

## Appendix B — Windows Eye Control state (CloudStore)

Stored per-user as a binary "CB" blob at:
`HKCU\Software\Microsoft\Windows\CurrentVersion\CloudStore\Store\DefaultAccount\Current\<id>$windows.data.accessibility.eyecontrol.syncedsettings\windows.data.accessibility.eyecontrol.syncedsettings`,
value `Data`. The live value is under the **GUID-suffixed** sibling key
(`default$…` is empty). Detection: the blob contains the byte sequence
`43 42 01 00 02` when Eye Control is **ON**, and `43 42 01 00 22` when **OFF**
(verified ON/OFF on one machine). Missing key ⇒ not enabled.

## Appendix C — Known eye-control tools table (`SSGazeDetect.cpp`)

| Process (match) | Display name | Clicks? | Match |
|---|---|---|---|
| `easyclick.exe` | Irisbond EasyClick | no | exact |
| `hirusystray.exe`, `hiru.exe` | Irisbond Hiru | no | exact |
| `tdcontrol.exe` | Tobii Dynavox TD Control | yes (dwell) | exact |
| `optikey.exe`, `optikeypro.exe` | OptiKey | no | exact |
| `tobii*` | Tobii Eye Tracking | no | prefix |

---

*End of DWELL-SPEC v1.0.*
