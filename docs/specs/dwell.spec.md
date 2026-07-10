# SimonSays — Gaze Dwell-Click System — Specification

| | |
|---|---|
| **Spec ID** | DWELL-SPEC |
| **Status** | Active — Phases A–D + Stream Engine provider implemented; HID gaze verified on Irisbond Hiru; Tobii direct gaze verified on 4C + PCEye5 |
| **Version** | 1.6 (2026-07-06) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |

---

## Contents
- [0. How to use this spec (for AI agents / contributors)](#0-how-to-use-this-spec-for-ai-agents--contributors)
- [1. Overview](#1-overview)
- [2. Background & context](#2-background--context)
- [3. Goals & non-goals](#3-goals--non-goals)
- [4. Glossary](#4-glossary)
- [5. Personas & scenarios](#5-personas--scenarios)
- [6. Requirements (EARS)](#6-requirements-ears)
- [7. Architecture & components](#7-architecture--components)
- [8. Detailed design](#8-detailed-design)
- [9. Data model & persistence](#9-data-model--persistence)
- [10. Key interfaces](#10-key-interfaces)
- [11. UI specification](#11-ui-specification)
- [12. Configuration & tuning constants (single source of each)](#12-configuration--tuning-constants-single-source-of-each)
- [13. Diagnostics & device bring-up](#13-diagnostics--device-bring-up)
- [14. Edge cases & error handling](#14-edge-cases--error-handling)
- [15. Acceptance criteria (testable)](#15-acceptance-criteria-testable)
- [16. Implementation status matrix](#16-implementation-status-matrix)
- [17. Known limitations & provisional items](#17-known-limitations--provisional-items)
- [18. Future work / out of scope](#18-future-work--out-of-scope)
- [19. Open questions](#19-open-questions)
- [20. Build & run](#20-build--run)
- [Appendix A — Irisbond Hiru HID gaze report (confirmed)](#appendix-a--irisbond-hiru-hid-gaze-report-confirmed)
- [Appendix B — Windows Eye Control state (CloudStore)](#appendix-b--windows-eye-control-state-cloudstore)
- [Appendix C — Known eye-control tools table (`SSGazeDetect.cpp`)](#appendix-c--known-eye-control-tools-table-ssgazedetectcpp)

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

HID eye-tracker support is device- and OS-dependent. The Irisbond Hiru exposes
an openable standard HID interface in HID mode. Tobii hardware (4C, PCEye5, …)
exposes a virtual "Tobii Eye Tracker HID" device (`HID\VID_2104&UP:0012_U:0001`)
that is **ACL-protected for the Windows gaze platform**: user-mode opens fail
with `ERROR_ACCESS_DENIED` even query-only, so it is unusable and undetectable
through the HID path (AC-16 verdict, 2026-07-05). The system must therefore
degrade gracefully, never assume a mode, and either detect or let the user
force one.

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
- **P3 — Mouse/switch user**: ordinary cursor by hand, **no tracker attached**.
  Expects dwell **off** so buttons don't auto-fire. (With a tracker present,
  presence wins — see REQ-F28 and §17.)
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
  with `StartAll/StopAll`, `SetHidProvider`, `SetTobiiProvider`, `HidLive`, and
  accessors `GetCursorSample`, `GetHidSample`, `GetTrackingSample`. Since v1.5
  "Hid" in this API means **direct gaze** (raw HID reader OR Tobii Stream
  Engine): `HidLive` is true when either backend streams, `GetHidSample`
  returns the freshest direct-gaze sample (HID preferred), and
  `GetTrackingSample` returns direct gaze if live, else cursor.
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
  `ExternalClick`, (3) the no-presence gate (REQ-F27) → `Off`, (4) live direct
  gaze (HID reader or Tobii engine) → `HidDwell` **unconditionally**, (5) any
  other presence → `MouseDwell` (REQ-F28); producing `{mode, confidence,
  reason}`. Cursor kinematics take no part in any decision. *(The v1.5
  "MouseDwell when the cursor is itself gaze-like" refinement was removed after
  the 2026-07-06 PCEye5 field test: burst cursor warps from TD Control / WEC /
  hand-mouse use scored "gaze-like" and steered Auto onto a parked cursor.)*
- **REQ-F24 [Done]** WHILE selection is `Auto`, THE SYSTEM SHALL refresh passive
  signals + direct-gaze liveness (~3 s) to feed the detector. Cursor sampling
  is retired: no timer feeds `MouseMotionClassifier` (dormant since v1.6).
- **REQ-F25 [Done*]** THE SYSTEM SHALL apply a new Auto decision only after it
  repeats for `HysteresisCount` consecutive evaluations (anti-flapping). *Count is
  hardcoded `3` (`DWELL_HYSTERESIS_COUNT`); not yet registry-configurable (§18).*
- **REQ-F26 [Done]** WHEN the Auto-resolved mode changes, THE SYSTEM SHALL persist
  it (`dwellDetectedMode`) so Auto restores it next launch.
- **REQ-F27 [Done]** WHILE selection is `Auto` AND no eye-tracking presence is
  detected (no HID eye tracker, no known eye-control tool, no Windows Eye
  Control, no live HID stream), THE SYSTEM SHALL resolve the detected mode to
  `Off` — dwell is never enabled from cursor kinematics alone. Fresh calibration
  (REQ-F44) still takes priority over this gate while valid. (Sampling policy:
  see REQ-F24.)
- **REQ-F28 [Done]** WHILE selection is `Auto` AND eye-tracking presence is
  detected (a HID eye tracker, a known eye-control tool, or Windows Eye Control)
  AND no live HID stream AND no known clicking tool, THE SYSTEM SHALL resolve
  the detected mode to `MouseDwell`: presence alone enables dwell. *Consequence:
  a hand-mouse user on a machine with a tracker merely plugged in gets dwell
  auto-firing until they run the MOUSE probe (calibration) or force Off — see
  §17.*

### 6.4 Passive detection

- **REQ-F30 [Done]** THE SYSTEM SHALL detect HID eye-tracker presence by enumerating
  HID interfaces (usage page `0x12`, usage `0x01`), opening with access `0`
  (query-only, never blocking the tool). *Trackers whose HID interface denies
  user-mode opens (the Tobii virtual "Eye Tracker HID", `err=5`) are invisible
  to this check by design of their driver — they are covered by the process
  table (REQ-F31) and Windows Eye Control signal (REQ-F32) instead.*
- **REQ-F31 [Done]** THE SYSTEM SHALL detect a running eye-control tool via a
  Toolhelp32 process scan against a known-tools table (EasyClick, Hiru, TD Control,
  OptiKey, `tobii*`, `tdx.*`, `platform_runtime_is*`), case-insensitive, flagging
  tools known to issue their own clicks (see §Appendix C).
- **REQ-F32 [Done]** THE SYSTEM SHALL report Windows Eye Control as enabled when
  its app process (`microsoft.ecapp.exe`) is running, OR when the CloudStore
  blob carries the "enabled" marker (see §Appendix B). *Both signals verified
  ON/OFF via dumps from three machines (original + PCEye5 + 4C, 2026-07-04/05).*
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
  that exercises the probes — HID if direct gaze is live, else Mouse —
  **re-evaluated on the ~2 s signals timer** (the Tobii engine / HID reader may
  connect or drop after the window opened), apply tuning live, and restore
  prior config on Cancel / commit on OK. *(One-shot forcing at open left the
  LOOK probe stuck in Mouse mode on the PCEye5 when the engine connected late —
  fixed 2026-07-06.)*
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

- **REQ-F80 [Done]** THE reader SHALL write a diagnostic report to
  `%LocalAppData%\SimonSays\hid_dump.txt` containing: app/OS/display/DPI;
  passive detection; a hex dump of every CloudStore `*eyecontrol*` blob; **all**
  HID top-level collections — each with its registry-side identity (device
  description, instance ID, hardware IDs — readable even when the device blocks
  handle queries), a query-only probe logging the exact failing step +
  `GetLastError` per metadata query (attributes / preparsed / caps / strings),
  and, when blocked, a retry with the shared `GENERIC_READ` open `SSGazeReader`
  uses; every HID eye-tracker device (usage 0x12/0x01 detail);
  the unique image names of all running processes; HID capabilities and value
  caps; a decoded gaze capture (raw + mapped px); and a min/max gaze-range
  summary. It SHALL run once at reader-thread start — unconditionally while the
  compile-time switch `SSGAZE_ENABLE_HID_DUMP` is `1` (current default, while
  device detection is being broadened), or only when env var
  `SIMONSAYS_HID_DUMP` is set once the switch is returned to `0`.

### 6.10 Tobii Stream Engine provider (Layer 1 backend)

- **REQ-F90 [Done]** THE SYSTEM SHALL read gaze from Tobii trackers whose HID
  interface is ACL-locked (AC-16) via the legacy Stream Engine C API, loading
  the **user-installed** `tobii_stream_engine.dll` at runtime with
  `LoadLibrary` — never bundling or redistributing it — resolving all exports
  dynamically, and requesting the Interactive field of use on engine ≥ 4
  (`tobii_device_create` arity resolved per `tobii_get_api_version`). Candidate
  DLL paths derive from the directories of running `tobii*` / `tdx.*` /
  `platform_runtime_is*` processes.
- **REQ-F91 [Done]** THE reader (`SSTobiiGaze`, singleton) SHALL stream on a
  background thread (poll `tobii_device_process_callbacks` ~100 Hz), accept
  only `TOBII_VALIDITY_VALID` points, map normalized 0..1 coordinates to
  primary-monitor pixels (same mapping and limitation as the HID reader),
  expose `HasLiveGaze` (200 ms freshness) / `GetLatest` / `Wake`, self-heal
  (missing DLL/engine/device → idle + retry 2 s; connection errors → reconnect,
  then rebuild after ~2 s), and shut down cleanly (signal, join, engine
  teardown, `FreeLibrary`).
- **REQ-F92 [Done]** THE provider (`TobiiGazeProvider`) SHALL plug into
  `GazeProviderChain` as a second direct-gaze slot (`SetTobiiProvider`) with
  **no `SSButton` changes** (REQ-F05): detector, gaze router, calibration
  probes, and the dwell window consume it through the existing
  `HidLive`/`GetHidSample`/`GetTrackingSample` surface (REQ-F03).
- **REQ-F93 [Done]** THE provider SHALL NOT block or degrade the vendor's own
  software (REQ-N04): device-create/subscribe failures are retried, never
  fatal, and no exclusive resources are taken by SimonSays itself.
  *Coexistence field-verified 2026-07-06/07 on both machines: gaze dwell ran
  alongside TD Computer Control, Windows Eye Control, and bare services with
  the vendor stack fully functional (Talon's kill-the-services guidance did not
  apply here).*

### 6.11 Non-functional

- **REQ-N01 [Done]** Win32 + C++ at the project's standard (MSVC default ≈ C++14,
  toolset v145); SHALL NOT bump the standard.
- **REQ-N02 [Done]** Windows SDK only; no third-party deps. Links `hid.lib`,
  `setupapi.lib`, `comdlg32.lib` (and existing `d2d1`/`dwrite`/`msimg32`/`comctl32`).
  *Runtime `LoadLibrary` of a vendor DLL already installed by the user's own
  tracker software (REQ-F90) is permitted — nothing third-party is linked,
  bundled, or required.*
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
                                                              │         │TobiiGazeProvider→SSTobiiGaze(Stream Engine thread)
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
| `include/SSTobiiGaze.h` / `src/SSTobiiGaze.cpp` | Tobii Stream Engine reader thread (runtime-loaded DLL) |
| `include/TobiiGazeProvider.h` / `src/TobiiGazeProvider.cpp` | Tobii provider (wraps SSTobiiGaze) |
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
known tool, no Windows Eye Control, no live direct gaze) → `Off` (REQ-F27);
(4) live direct gaze → `HidDwell` unconditionally; (5) any other presence →
`MouseDwell` (REQ-F28 — presence alone enables dwell). `SyncDwellTimers`
refreshes the passive scan immediately when Auto starts so the first decision
doesn't run on ~3 s-stale signals. `MouseMotionClassifier` (teleports/jitter/
bimodal velocity over a 120-sample window) is **dormant** since v1.6: nothing
samples the cursor and no decision consumes its verdict; it is retained for
potential diagnostics.

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
// GazeProviderChain (singleton) — "Hid" here means direct gaze (HID or Tobii)
static GazeProviderChain& Instance();
void StartAll(); void StopAll();
void SetHidProvider(std::unique_ptr<IGazeProvider>);
void SetTobiiProvider(std::unique_ptr<IGazeProvider>);
bool HidLive() const;                        // either direct-gaze backend live
bool GetCursorSample(GazeSample*) const;
bool GetHidSample(GazeSample*) const;        // freshest direct-gaze sample
bool GetTrackingSample(GazeSample*) const;   // direct gaze if live, else cursor

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

// SSTobiiGaze (singleton) — runtime-loaded Tobii Stream Engine
static SSTobiiGaze& Instance();
bool Start(); void Stop(); bool HasLiveGaze() const; bool GetLatest(GazeSample*) const;
void Wake();

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
| Tobii sample max age / setup retry | 200 ms / 2000 ms | `SSTobiiGaze.cpp` `kMaxAgeMs` / `kRetryMs` |
| Tobii callback pump / error pump / errors→rebuild | 10 ms / 100 ms / 20 | `SSTobiiGaze.cpp` |
| ~~Detector cursor sample~~ | retired in v1.6 (timer id 3 freed) | — |
| Diagnostic dump switch | `SSGAZE_ENABLE_HID_DUMP` = 1 (dump always on; set 0 for env-var opt-in) | `SSGazeReader.cpp` |
| Detector passive refresh | 3000 ms | `TIMER_DWELL_DETECT` |
| Gaze router poll | 50 ms | `TIMER_DWELL_GAZE` |
| Device-change debounce | 800 ms | `TIMER_DWELL_DEVCHANGE` |
| Hysteresis count | 3 | `DWELL_HYSTERESIS_COUNT` |
| Calibration valid | 10 min | `SSDwellModeDetector.h` `kCalibrationValidMs` |
| Classifier window / min | 120 / 45 samples | `SSDwellModeDetector.h` (**dormant** since v1.6) |
| Classifier thresholds | `kStillPx 1.5`, `kTeleportPx 150`, `kFixationSpeed 300`, `kJitterLow 0.8`, `kJitterHigh 2.0` | `SSDwellModeDetector.h` (**dormant**, unvalidated) |
| Slider ranges | time 300–2000, tol 15–80, cooldown 0–1000 | `SSDwellWindow.cpp` |
| First-run defaults | mode Auto, 800 ms, 35 px, 300 ms, detected Off | `stdafx.h` `DWELL_DEFAULT_*` (color: accent via `GetAccentColor()`) |

---

## 13. Diagnostics & device bring-up

To bring up a **new** HID eye tracker on any machine:

1. With the tracker's HID/streaming mode ON, run the Release exe (the dump is
   currently always-on via `SSGAZE_ENABLE_HID_DUMP 1`; once that is set back to
   `0`, set env `SIMONSAYS_HID_DUMP=1` instead); slowly sweep gaze to all four
   screen corners in the first ~4 s.
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
- **AC-11 (REQ-F24/F27) [Withdrawn]** *(Tested sampler gating; obsolete since
  v1.6 — the cursor sampler no longer exists at all, which satisfies the
  underlying performance intent by construction.)*
- **AC-13 (REQ-F28) [Pending]** With any detected eye-tracking device, known
  tool, or Windows Eye Control enabled — and no live HID / clicking tool — Auto
  resolves `MouseDwell` within ~9 s (3-tick hysteresis) and buttons
  dwell-activate under the cursor. *Detection now recognizes the PCEye5 and 4C
  stacks (their dumps showed `tobii*` matching); awaiting the live re-test.*
- **AC-14 (REQ-F80) [Pass]** On a machine with a Tobii tracker, launching the
  app produces `hid_dump.txt` listing every HID collection (with VID/PID and
  usage page), the running-process names, and the Eye Control blob bytes.
  *Five dumps delivered from the PCEye5 and 4C machines (2026-07-04/05) drove
  the REQ-F31/F32 broadening.*
- **AC-15 (REQ-F31/F32) [Pass]** On the PCEye5 and 4C machines, the detection
  signals the "Detected" panel displays track the real state. *Verified via the
  2026-07-05 dumps (same code path as the panel): PCEye5 reports "Tobii Dynavox"
  (the new `tdx.*` entry matched) and WEC yes; 4C reports "Tobii Eye Tracking"
  and WEC yes. Note the displayed tool name can vary between matching entries
  ("Tobii Dynavox" vs "Tobii Eye Tracking") with process enumeration order —
  cosmetic.*
- **AC-16 (REQ-F80) [Pass]** On the PCEye5 machine, the dump identifies the
  "Tobii Eye Tracker HID" device node by name/hardware ID and reports the exact
  error blocking each query under both access-0 and `GENERIC_READ` opens.
  *Verdict (dumps from both machines, 2026-07-05): the device exists — 4C
  `HID\TOBII\…`, PCEye5 `HID\TOBIIHIDDRIVER\…`, both `HID\VID_2104&UP:0012_U:0001`
  — but every open fails with `ERROR_ACCESS_DENIED` (5), query-only included, so
  direct HID dwell on Tobii hardware is impossible without the vendor SDK or the
  Windows gaze API (§18). The anonymous `VID=0x0000` entry from the v2 dumps was
  a HID sensor hub (metadata `err=1168`), not the Tobii device, which v2 had
  skipped silently on its failed open.*
- **AC-12 (REQ-F67) [Pending]** Clicking Reset in the dwell window sets the
  radios to Auto, sliders/edits to 800/35/300, and the progress color to the
  Windows accent color; OK persists the defaults (color value removed from the
  registry), Cancel restores the pre-dialog settings.

- **AC-17 (REQ-F90–F93, REQ-N04) [Pass]** On the 4C and PCEye5 machines with
  the vendor software running normally: Auto resolves direct-gaze dwell
  (`HidLive` true via the Stream Engine), the LOOK probe activates by gaze with
  the cursor untouched, buttons dwell-activate — AND the vendor stack (TD
  Computer Control / Tobii Experience / WEC) keeps working throughout
  (coexistence, the mandatory part). If the engine cannot connect (device
  busy), SimonSays silently stays on Mouse dwell with no errors.
  *[Pass] confirmed 2026-07-07: after the v1.6 fixes, Auto direct-gaze dwell
  works on the 4C and PCEye5 with TD Control / WEC / no software, with the
  vendor stack coexisting throughout. (The 2026-07-06 pre-fix test had passed
  streaming + coexistence in forced HID only.)*
- **AC-18 (REQ-F64) [Pass]** Opening the dwell window *before* the gaze
  stream is up: within ~4 s of the stream starting (2 s signals timer + engine
  connect) the forced mode flips to HID and the LOOK probe becomes armable by
  gaze; if the stream drops, the probes fall back to Mouse forcing.
  *Confirmed with AC-17 in the 2026-07-07 re-test.*

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
| Dwell window + probes + signals | ✅ Done | Probe forcing re-evaluated every ~2 s since v1.6 (late-connecting engine; AC-18 Pass 2026-07-07) |
| Passive detection (`SSGazeDetect`) | ✅ Done | Eye Control verified ON/OFF on three machines (blob + `microsoft.ecapp.exe`) |
| Detector + Auto + hysteresis | ✅ Done | Hysteresis hardcoded 3; classifier dormant since v1.6 (direct gaze → HidDwell unconditionally) |
| Auto no-presence gate (REQ-F27) | ✅ Done | Manual AC-11 check pending |
| Auto presence → MouseDwell (REQ-F28) | ✅ Done | Field re-test pending (AC-13); detection now recognizes the PCEye5/4C stacks |
| Cursor sampler retired (REQ-F24) | ✅ Done | Removed in v1.6; AC-11 withdrawn (intent satisfied by construction) |
| Dwell window Reset button (REQ-F67) | ✅ Done | Manual AC-12 check pending |
| Diagnostics dump extension + default-on switch (REQ-F80) | ✅ Done | AC-14 Pass — PCEye5 + 4C dumps delivered 2026-07-04/05; v3 adds per-device registry identity + failure codes + GENERIC_READ retry (AC-16 pending) |
| Tobii virtual "Eye Tracker HID" device readable? | ⛔ Blocked | AC-16 verdict 2026-07-05: device exists on both machines (`HID\VID_2104&UP:0012_U:0001`) but all user-mode opens fail with ERROR_ACCESS_DENIED — reserved for the Windows gaze platform. Direct HID dwell on Tobii needs the vendor SDK or the Windows gaze API (§18) |
| Detection of Tobii PCEye5 / 4C + robust WEC signal | ✅ Done | `tdx.*` + `platform_runtime_is*` table entries; WEC via `microsoft.ecapp.exe` OR blob marker (3 machines). Live panel re-test pending (AC-15); ET5 expected via `platform_runtime_is*`/`tobii*`, unverified |
| HID reader (`SSGazeReader`) | ✅ Done | Non-exclusive, self-healing, clean shutdown |
| HID gaze parse + µm→px mapping | ⚠️ Done\* | Confirmed Irisbond Hiru only |
| Calibration → `ReportCalibration` | ✅ Done | |
| Hot-plug (`WM_DEVICECHANGE`) | ✅ Done | |
| Diagnostics dump | ✅ Done | Full environment report |
| Application Verifier pass | ⚠️ Attempted (inconclusive) | Elevated pass ran + machine left clean, but app didn't reach steady state under automated launch; re-run by launching normally (§13.1 / AC-10) |
| Tobii Stream Engine provider (REQ-F90–F93) | ✅ Done | **Field-verified 2026-07-07 on 4C + PCEye5**: Auto direct-gaze dwell + vendor coexistence with TD Control / WEC / no software (AC-17 Pass) |
| WinRT gaze API provider (`Windows.Devices.Input.Preview`) | ⛔ Parked | Needs MSIX package identity (`gazeInput`) + CoreWindow (`GetForCurrentView`, no HWND interop); see §18 |
| Registry-config hysteresis / calib-validity / ring indicator | ⛔ Planned | Hardcoded today |
| In-app Help text for F3/F7 + dwell section | ✅ Done | 2026-07-07: all 17 help sources (`HELP.md` + `docs/help/*`) updated (F3=dwell, F7=add, zoom keys, translated Gaze/Dwell section incl. verified-tracker list) and `HELP_CONTENT_ID` regenerated via `scripts/sync_help_content.ps1` |

---

## 17. Known limitations & provisional items

- **HID parse is device-specific.** Gaze usages (`0x21`=X, `0x22`=Y), micrometer
  units, and top-left origin are confirmed for the **Irisbond Hiru** only. Other HID
  trackers may use different usages/units/origin; bring them up via §13 and adjust
  `SSGazeReader.cpp`. Wrong mapping degrades safely to cursor.
- **Tobii hardware (4C, PCEye5, …) allows no HID dwell** — their virtual "Tobii
  Eye Tracker HID" device is a genuine usage-page-0x12 tracker but denies all
  user-mode opens (`ERROR_ACCESS_DENIED`, verified on both machines 2026-07-05);
  it is reserved for the Windows gaze platform. Direct gaze on these devices
  goes through the Stream Engine provider instead (REQ-F90–F93, field
  verification pending — AC-17), with Mouse dwell as the fallback.
- **Tobii Eye Tracker 5 has no accessibility path from Tobii.** Officially not
  compatible with Windows Eye Control (gaming-only device; Tobii points
  assistive users to the Tobii Dynavox PCEye5 + Computer Control), and its
  software provides no OS cursor control. OptiKey's native support today covers
  only mouse-cursor-driving trackers (Tobii dropped). ET5 users need a
  third-party gaze-mouse tool that rides the legacy Stream Engine (e.g. Talon,
  Precision Gaze Mouse) to drive the cursor — then SimonSays Mouse dwell works,
  and `tobii*` presence already enables Auto. Note: ET5 machines are typically
  gamer machines with a hand mouse, so the REQ-F28 presence trade-off surfaces
  most often here (escape: MOUSE probe or force Off).
- **Classifier thresholds** are estimates; Auto's kinematic branch needs tuning with
  recorded per-device data.
- **Windows Eye Control** detection relies on an undocumented CloudStore byte marker
  (§Appendix B) verified on one machine; could change across Windows builds.
- **HID gaze** maps to the **primary** monitor only (the Stream Engine
  provider shares this: normalized coordinates map to the primary monitor).
- **The dwell window's HID radio stays gated on physical HID presence**
  (`IsHidEyeTrackerPresent`), so on Tobii machines Auto reaches direct-gaze
  dwell via the Stream Engine but the manual "HID eye tracker" selection stays
  disabled; the Detected panel's HID line does reflect the engine stream
  ("streaming gaze") since it reads `HidLive()`.
- **Unknown cursor-moving trackers get no Auto dwell.** Since REQ-F27, a tracker
  whose software is not in the known-tools table (Appendix C) and that exposes
  neither HID nor Windows Eye Control resolves to `Off` in Auto. Such users can
  force Mouse dwell, or run the LOOK probe — calibration overrides the gate, but
  only while fresh (≤10 min). Extend the known-tools table for such devices.
  **Field test 2026-07-04, resolved via the AC-14/AC-16 dumps (2026-07-05):**
  Tobii PCEye5 and 4C *do* expose a standard HID eye-tracker collection (the
  virtual "Tobii Eye Tracker HID", `HID\VID_2104&UP:0012_U:0001`) — but it
  denies all user-mode opens (`ERROR_ACCESS_DENIED`, even query-only), so the
  HID path can neither detect nor read it. The dumps also showed the `tobii*`
  process match and the WEC blob marker actually working on both machines; the
  earlier "undetected" impression came from the pre-REQ-F28 logic where presence
  didn't enable dwell. v1.3 added `tdx.*` and `platform_runtime_is*` (device
  service) table entries plus the `microsoft.ecapp.exe` WEC signal for
  robustness. Eye Tracker 5 remains unverified (no dump available); expected
  covered by `platform_runtime_is*` / `tobii*`.
- **Presence enables dwell for hand-mouse users too (REQ-F28).** With a tracker
  plugged in (or WEC enabled) but the user driving the cursor by hand, Auto now
  resolves `MouseDwell` and buttons will dwell-fire under a resting cursor. The
  escape hatches are the MOUSE probe (calibration, ≤10 min) or forcing Off.
  Accepted trade-off per the product owner (2026-07-04): a detected tracker
  means an eye-tracking user is far more likely than not.

---

## 18. Future work / out of scope

- **Tobii Stream Engine provider — IMPLEMENTED 2026-07-06** as `SSTobiiGaze` +
  `TobiiGazeProvider` (REQ-F90–F93) after `tobii_stream_engine.dll` was found
  installed on both test machines (including inside TD Computer Control on the
  PCEye5 machine). Field verification pending (AC-17). Background from the
  research below. Tobii's
  *current* offering (Tobii Streams SDK) is closed for us: it supports only the
  5L/Nexus, dev licenses cost €1,495–2,990/yr, and standard terms exclude
  commercial use. But the **legacy Stream Engine C API**
  (`tobii_stream_engine.dll`) remains pragmatically viable: the runtime ships
  with the user's installed Tobii software (Tobii Service was running on both
  test machines, PCEye5 included), so a provider could `LoadLibrary` the
  user-installed DLL at runtime — no SDK redistribution, no build-time
  dependency (REQ-N02 intact), self-declared prototypes from the still-public
  tutorial docs. The old license permits **Interactive Use** (gaze as input, no
  storage/transfer) — exactly dwell-click. Proven in the wild by current tools
  (Talon, Precision Gaze Mouse, LSL's TobiiStreamEngine app) incl. on the Eye
  Tracker 5. Risks: SDK deprecated/unsupported (NuGet unlisted, last release
  2018), Tobii's "ET5 is not for development" stance under the *new* licensing,
  PCEye5 compatibility unverified (plausible — Tobii Service runs there).
  **Coexistence caveat:** Talon's setup docs instruct users to *kill the Tobii
  services* (its engine integrates directly with the hardware), so the legacy
  engine may contend with the vendor stack for the device instead of sharing
  it. That would violate REQ-N04 ("never block the external tool") — any
  feasibility spike MUST verify the provider coexists with the running vendor
  software (mandatory AC); if coexistence fails on Dynavox hardware, this route
  is effectively ET5/4C-only (where no vendor cursor control exists anyway).
  Plugs in via `GazeProviderChain` as designed. Device expectations: proven in
  the wild on ET5 + 4C; very likely on EyeX / Tobii-integrated laptops;
  unknown on PCEye5/Dynavox and 5L/Nexus.
- **Windows gaze platform API (WinRT `Windows.Devices.Input.Preview`) —
  investigated 2026-07-05, parked.** Two blockers for an unpackaged Win32 app:
  (1) the `gazeInput` device capability requires package identity (MSIX) and
  drives the consent/privacy gating — it is what satisfies the ACL on the
  protected Tobii HID device (AC-16); (2) gaze events come only from
  `GazeInputSourcePreview.GetForCurrentView()`, a CoreWindow/view-bound API
  with no HWND interop; the MS gaze library and every MS sample are UWP-only.
  Only workaround: a separate MSIX-packaged helper process with a CoreWindow
  piping gaze to SimonSays — heavy and fragile, and it only serves
  WEC-compatible trackers (not the ET5). Revisit only if the app ever adopts
  MSIX packaging.
- Validate/extend HID parsing for more devices; per-device profiles.
- Make `HysteresisCount`, `CalibrationValidMs`, and a **bar-vs-ring** progress
  indicator registry-configurable + add UI (a `ShowProgressRing` option).
- Multi-monitor HID gaze mapping.
- ~~Update in-app Help (`HELP_CONTENT_ID`, all languages) for the F3/F7 change.~~
  Done 2026-07-07 (all languages, incl. the new Gaze/Dwell help section).
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
5. **Does Tobii Dynavox Computer Control / EyeAssist issue its own clicks?**
   The `tdx.*` entries are marked non-clicking for now (product owner unsure,
   2026-07-05). If its dwell mode clicks like old `tdcontrol.exe`, the running
   app (likely `tdx.computercontrol.exe`) should be flagged `clicks=true` so
   Auto resolves `ExternalClick`; watch for double activations in field tests.

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

Primary signal (since v1.3): the Eye Control app process
**`microsoft.ecapp.exe`** — observed running exactly while Eye Control is ON in
dumps from the PCEye5 and 4C machines (2026-07-04/05).

Fallback: the persisted toggle, stored per-user as a binary "CB" blob at:
`HKCU\Software\Microsoft\Windows\CurrentVersion\CloudStore\Store\DefaultAccount\Current\<id>$windows.data.accessibility.eyecontrol.syncedsettings\windows.data.accessibility.eyecontrol.syncedsettings`,
value `Data`. The live value may sit under the **GUID-suffixed** key with
`default$…` empty (original machine, 4C machine) **or** directly under
`default$…` (PCEye5 machine) — both are scanned. Detection: the blob contains
the byte sequence `43 42 01 00 02` when Eye Control is **ON** (the inner CB
header is followed by an `02 01` "enabled" field). Observed OFF variants after
the inner header: `22` (original machine), `00` (PCEye5 machine), `C2 09` (4C
machine) — none matches the ON marker. Verified ON/OFF via dumps on all three
machines. Missing key ⇒ not enabled.

## Appendix C — Known eye-control tools table (`SSGazeDetect.cpp`)

| Process (match) | Display name | Clicks? | Match |
|---|---|---|---|
| `easyclick.exe` | Irisbond EasyClick | no | exact |
| `hirusystray.exe`, `hiru.exe` | Irisbond Hiru | no | exact |
| `tdcontrol.exe` | Tobii Dynavox TD Control | yes (dwell) | exact |
| `optikey.exe`, `optikeypro.exe` | OptiKey | no | exact |
| `tobii*` | Tobii Eye Tracking | no | prefix |
| `tdx.*` | Tobii Dynavox | no (unconfirmed — see §19) | prefix |
| `platform_runtime_is*` | Tobii eye tracker service | no | prefix |

Observed in the 2026-07-04/05 dumps: the PCEye5 stack runs
`tobiidynavox.eyeassist.*` + `tobiidynavox.eyetrackingsettings.exe` (caught by
`tobii*`), `tdx.switcher.exe` + `tdx.computercontrol.updater.exe` (caught by
`tdx.*`), and the device service `platform_runtime_is5largepceye5_service.exe`
(caught by `platform_runtime_is*` — IS-series platform runtime, expected to
also cover the Eye Tracker 5). The 4C stack runs `tobii.eyex.*`,
`tobii.service.exe`, `tobiivirtualdevice.exe` (all caught by `tobii*`).
`microsoft.ecapp.exe` is deliberately **not** in this table — it feeds the
Windows Eye Control signal instead (Appendix B).

---

*End of DWELL-SPEC v1.0.*
