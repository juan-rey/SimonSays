# SimonSays — Gaze Dwell-Click System — Specification

| | |
|---|---|
| **Spec ID** | DWELL-SPEC |
| **Status** | Active — Phases A–D + Stream Engine provider implemented; HID gaze verified on Irisbond Hiru; Tobii direct gaze verified on 4C + PCEye5; REQ-F80 dump location moved under a debug\ subfolder 2026-07-29; Stream Engine licensing basis recorded and pinned by REQ-N08; **the Auto detector's timer was dead until 2026-09-21 (see REQ-F24) — every Auto decision path is implemented but has never executed in the field**; 2026-10-18: a switched-off Tobii tracker resolves Auto to Off (REQ-F29/F94, AC-20 Pass); 2026-10-19: "move away to re-arm" after dwell activations (REQ-F95, AC-21 Pass); 2026-10-22: 64-bit gaze helper for the Eye Tracker 5's 64-bit-only engine (REQ-F96–F101, §6.11) — gaze dwell, tracking off/on and vendor coexistence verified on the ET5 (AC-23–AC-26 Pass), 4C regression pending (AC-27); licensing basis re-checked against Tobii EULA v3.0 (§18); diagnostic report no longer written at startup — on demand from the dwell window's button (REQ-F80 amended, REQ-F102); 2026-10-23: Tobii background services alone no longer turn on cursor dwell in Auto (REQ-F28/F31 amended, AC-35–AC-37 Pass); 2026-10-24: gaze hover highlight (REQ-F103, AC-39/40 Pass); lock-on delay before the progress starts (REQ-F104, partly verified); 2026-10-25: "HID eye tracker" option available for Tobii trackers and gaze-only (REQ-F63 amended, §8.3); release builds always include the helper and the installer verifies it (REQ-N10/N11, AC-47–AC-49); 2026-11-04: Auto presence-only → cursor dwell verified with the Irisbond Hiru in mouse mode (AC-13 Pass), so every Auto outcome has now run in the field; **per-button dwell behaviours for other hosts added 2026-11-16 (REQ-F106 to REQ-F108): repeat while held, second stage, own dwell time; no SimonSays button uses them**; **2026-11-17: the dwell window can be hosted by another application (REQ-F109): host-neutral entry point, dialog helpers in `SSDialogUtils`, template in `SSDwellDialog.rc`; SimonSays unchanged (AC-55 Pass, 2026-11-18)**; **2026-11-18: the 64-bit helper is named `SSGazeHelper64.exe` (was `SimonSaysGaze64.exe`), REQ-F97; version 0.7.1**; **2026-11-22: the mode choices can be chosen by dwell, except Off (REQ-F110)**; **2026-11-24: a dwell goes on anywhere on the button and survives a short time off it (*LeaveGrace*, 150 ms, registry only); a fire ends every other dwell; the tolerance radius is only the move-away distance (REQ-F12, REQ-F15, REQ-F111, from SimonSays Gaze's SPK-12)**; **2026-11-25: within the tolerance radius around a button a dwell goes on as if on it, unless the point is on another button; time away from the button pauses the dwell instead of running on (REQ-F12, REQ-F15); default tolerance radius 25 px (was 35)** |
| **Version** | 1.25 (2026-11-25) |
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
- **[Done*]** — implemented; correctness depends on an environment-specific
  assumption validated in a limited setting — a device-specific assumption
  checked on one device only, or a path that has not yet been exercised at
  all (see notes). Matches [`docs/spec.md`](../spec.md) §2.3.
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
| **Tolerance radius** | Pixels the tracking point must move from where a dwell fired before dwell can start again (REQ-F95), and, since 2026-11-25, the margin around a button within which a dwell in progress goes on as if the point were on it (REQ-F15). Since 2026-11-24 it no longer restarts a dwell in progress (REQ-F12). |
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
  obtain it through the chain per the active mode (MouseDwell→cursor; HidDwell→direct
  gaze only, never the cursor; Off/ExternalClick→none). *(The cursor fallback
  for a forced HID mode was removed 2026-10-25, §8.3.)*
- **REQ-F05 [Done]** THE chain SHALL accept additional providers in future without
  changes to `SSButton` (e.g., a Tobii Stream Engine provider — see §18).

### 6.2 Dwell behavior on `SSButton`

- **REQ-F10 [Done]** WHILE dwell is active for a button (mode is Mouse/HID, the
  button is enabled and dwell-enabled, and not in cooldown), THE SYSTEM SHALL start
  a fixation when the tracking point enters the button.
- **REQ-F11 [Done]** WHILE a fixation is in progress, THE SYSTEM SHALL advance a
  progress value 0→1 over `DwellTime` and render a bottom-edge progress bar in
  `ProgressColor`.
- **REQ-F12 [Done]** WHILE a fixation is in progress, THE SYSTEM SHALL count
  every tracking point on the button alike: moving within the button never
  re-anchors or restarts the fixation clock. *(Since 2026-11-24, from
  SimonSays Gaze's SPK-12 (its `docs/spikes.md`), Eye Tracker 5, developer
  decision: measured from its first point with a 35 px tolerance, a dwell
  restarted on jitter and drift and fired 19 of 45 targets within 4 s;
  counted anywhere on the button, 35, about half a second sooner, with
  10 unwanted fires against 9. Until then: IF the point moved beyond
  `ToleranceRadius` from the fixation anchor THEN the clock restarted.)*
- **REQ-F13 [Done]** WHEN the fixation reaches `DwellTime`, THE SYSTEM SHALL fire
  `BN_CLICKED` to the parent (identical to a real click), set the activation source
  to `Dwell`, and enter `Cooldown`.
- **REQ-F14 [Done]** WHILE within `Cooldown` after any activation, THE SYSTEM SHALL
  NOT start a new fixation on that button. (After a **dwell** activation the
  re-arm block of REQ-F95 follows the cooldown.)
- **REQ-F15 [Done]** WHILE a fixation is in progress, THE SYSTEM SHALL
  count a tracking point within `ToleranceRadius` of the button's edge (the
  *margin*) as on the button, for progress and for firing, UNLESS it lies on
  another dwell-enabled, enabled SSButton of the process. WHILE the point is
  off the button and beyond the margin (or after `WM_MOUSELEAVE` in cursor
  mode), THE SYSTEM SHALL pause the fixation: no firing, the progress bar
  as it was, and the fixation clock stopped; WHEN the point returns to the
  button or its margin, THE SYSTEM SHALL resume it from where it stopped,
  the time away not counted. IF the point stays off longer than
  `LeaveGrace` (`SSDwellConfig::Get/SetLeaveGraceMs`, 0–500 ms, default
  `DWELL_DEFAULT_LEAVE_GRACE_MS` = 150; 0 = at once) THEN THE SYSTEM SHALL
  cancel the fixation and clear the progress bar. With no tracking point at
  all, the same pause applies and the blink gap (`SSBUTTON_DWELL_GAP_MS`,
  500 ms) replaces LeaveGrace. A button held after firing (REQ-F106,
  REQ-F107) ends its hold at once when the point leaves the button itself,
  margin or not, as before. *(Since 2026-11-24,
  SPK-12; gaze and cursor dwell alike. Until then leaving cancelled at once.
  A first version kept the fixation for the whole blink gap off the button;
  tried by gaze in SimonSays, neighbouring buttons fired with the one looked
  at (developer, 2026-11-24), so the time off is shorter and configurable,
  and REQ-F111 was added. `LeaveGrace` is a registry value only for now,
  §9.3. The margin was added 2026-11-25 at the developer's request, so the
  tolerance radius and LeaveGrace act together; the exception for another
  button keeps a neighbour looked at from extending this dwell. Its first
  version only kept the fixation in the margin, bar frozen, while the clock
  ran on, as it had off the button since 2026-11-24; tried by gaze the bar
  stuttered, stalled, and the button fired as the point came back with the
  bar part-way (developer, 2026-11-25). So the margin counts fully and time
  away pauses the clock. Tried by gaze, smooth and with no neighbour fired,
  the developer set the default `ToleranceRadius` to 25 px, from 35
  (`DWELL_DEFAULT_TOLERANCE_PX`, 2026-11-25); a value already stored is
  kept, so it reaches new installs and Reset only.)*
- **REQ-F111 [Done]** *(numbered out of range; belongs to this section.)*
  WHEN a button fires by dwell (REQ-F13), or fires again while held
  (REQ-F106, REQ-F107), THE SYSTEM SHALL cancel every other fixation in
  progress in the process, so that a press never brings a neighbour's
  dwell with it; with REQ-F95 nothing then starts until the point moves
  away. *(Added 2026-11-24, after the trial by gaze of REQ-F15.)*
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
- **REQ-F95 [Done]** *(numbered out of range because F10–F19 is full; it
  belongs to this section.)* WHILE "Move away to re-arm" is enabled
  (`SSDwellConfig::GetRearmOnLeave`, default on, dwell window checkbox), AFTER a
  **dwell** activation (REQ-F13), THE SYSTEM SHALL NOT start a fixation on
  **any** `SSButton` until a tracking point is observed farther than
  `ToleranceRadius` from the activation point, OR (HidDwell) no tracking point
  exists for longer than `SSBUTTON_DWELL_GAP_MS` (500 ms: looking away or
  closing the eyes re-arms). A button that always asks for it (a checkbox,
  [`ssbutton.spec.md`](ssbutton.spec.md) BTN-F83, BTN-F86) sets the block
  even while the option is off, so one look changes it once
  *(2026-11-22)*. Physical clicks (REQ-F16) neither set nor are
  affected by this block. The block is process-wide and tied to the point, not
  the button, so keys rebuilt under a resting pointer (e.g. after a category
  change) are blocked too. Points are observed in `OnDwellMouseMove` and by
  `SSButton::NoteTrackingPoint`, which the main window's 50 ms
  `TIMER_DWELL_GAZE` calls in Mouse and HID dwell, so moving over the
  background also counts. For a button that repeats or has a second stage
  (REQ-F106, REQ-F107) the block is set when the hold ends, at the last
  point seen on the button, not at the first activation. *Motivation
  (2026-10-19 measurement, Release build,
  mouse dwell with a ±3 px jittering pointer): a key re-fired every
  1.08–1.25 s, and a ~4 s phrase was re-fired three times before it could
  finish. Verified by AC-21 (2026-10-19).*
- **REQ-F103 [Done]** *(numbered out of range; belongs to this section.)*
  WHILE the active mode is `HidDwell`, THE SYSTEM SHALL show the **hover
  color** on the `SSButton` under the current gaze point, exactly as mouse
  hover does (the cursor does not move in gaze dwell, so `WM_MOUSEMOVE` hover
  never shows there). Only one button is highlighted at a time; moving the gaze
  moves it; a gap up to `SSBUTTON_DWELL_GAP_MS` (500 ms, a blink) keeps it, a
  longer one clears it; leaving `HidDwell` clears it. Disabled buttons keep
  their disabled color; dwell-opted-out buttons (`SetDwellEnabled(false)`) are
  still highlighted. Driven by the main window's `TIMER_DWELL_GAZE`
  (`SSButton::UpdateGazeHover` / `ClearGazeHover`, §8.2); repaints only on a
  change. Button side: [`ssbutton.spec.md`](ssbutton.spec.md) BTN-F66. *(Added
  2026-10-24; verified by AC-39/AC-40 the same day.)*
- **REQ-F104 [Done*]** *(numbered out of range; belongs to this section.)*
  THE SYSTEM SHALL apply a **lock-on delay** (`SSDwellConfig::Get/SetLockOnMs`,
  0–500 ms, default `DWELL_DEFAULT_LOCKON_MS` = 250, 150 until 2026-11-07): after a fixation starts,
  no progress is shown for the lock-on time; then the progress fills over
  `DwellTime` and the button fires when it is full (time to fire = lock-on +
  dwell time). The lock-on runs from the start of the fixation; leaving the
  button for longer than the blink gap cancels it, as for REQ-F15 (before
  2026-11-24 moving past `ToleranceRadius` also restarted it). It applies to
  gaze and cursor dwell alike; 0 behaves exactly as before. Gaze hover
  (REQ-F103) shows during the lock-on. Existing installs without the registry
  value get the default. *Motivation: scanning a board brushes over many
  buttons; without a lock-on each showed a bar that reset at once (2026-10-24,
  developer request; OptiKey calls the equivalent "lock-on time").*
  *(Added 2026-10-24; AC-41/AC-42 Pass; AC-43 Pass, gaze part confirmed 2026-11-04.)*
- **REQ-F105 [Done]** *(numbered out of range; dwell window UI.)* THE dwell
  window's five push buttons (Progress color..., Save diagnostic report...,
  Reset, OK, Cancel) SHALL be dwell-able `SSButton`s via
  `SSButton::ConvertDialogPushButtons` ([`ssbutton.spec.md`](ssbutton.spec.md)
  BTN-F75), armed 750 ms after the window opens. Because the dwell timing is
  edited live here, a short dwell time also applies to these buttons at once;
  the arming delay and REQ-F95 re-arm keep the window from pressing a button
  on its own as it opens. The two probes are unchanged. Verified by BTN
  AC-18/AC-19. *(Added 2026-10-31.)*
- **REQ-F106 [Done*]** *(numbered out of range; belongs to this section.)*
  WHERE a button is set to **repeat while held**
  (`SSButton::SetDwellRepeat( RepeatWhileHeld, intervalMs )`), AFTER a dwell
  activation (REQ-F13) and WHILE the tracking point stays on that button,
  THE SYSTEM SHALL activate it again every `intervalMs`, showing the
  progress of each interval on the usual bar. Each interval counts from
  when the one before was due, not from the timer tick that fired it, so
  the repeats keep `intervalMs` on average; a button a whole interval
  behind starts again from that moment, with no burst to catch up
  *(2026-11-21: counted from the tick, a 0.15 s repeat ran near 0.2 s)*. Between repeats there is no
  cooldown, no lock-on, no tolerance test and no re-arm block: being on the
  button is all that counts. The hold ends when the point leaves the button,
  when no point exists for longer than `SSBUTTON_DWELL_GAP_MS`, or when dwell
  stops for the button (disabled, opted out, mode without dwell, a physical
  click); the cooldown and the block of REQ-F95 apply from then. The
  keyboard activates once, as on any button; a mouse press held on the
  button repeats as well ([`ssbutton.spec.md`](ssbutton.spec.md) BTN-F67,
  added 2026-11-21), while a short click activates once. The default for
  every button is to fire once, which is the behavior before this
  requirement. *(Added 2026-11-16 for a second host, SimonSays Gaze, whose
  keyboard needs Backspace and the arrows to repeat; no SimonSays button is
  set to repeat. Verified in cursor dwell by AC-52.)*
- **REQ-F107 [Done*]** *(numbered out of range; belongs to this section.)*
  WHERE a button is set to **second stage**
  (`SSButton::SetDwellRepeat( SecondStage, furtherMs )`), AFTER a dwell
  activation and WHILE the tracking point stays on that button for a further
  `furtherMs`, THE SYSTEM SHALL send the parent one `WM_COMMAND` with the
  notification code `SSBN_DWELLSECONDSTAGE` (the first stage sent
  `BN_CLICKED` as usual), and SHALL show the second stage's progress along
  the **top** edge of the button, so it can be told from the first. The
  rules of REQ-F106 for the time in between and for how the hold ends apply;
  after the second-stage notification the hold is over. A hold that ends
  early sends nothing more. A mouse press held on the button reaches the
  second stage too ([`ssbutton.spec.md`](ssbutton.spec.md) BTN-F67, added
  2026-11-21). *(Added 2026-11-16 for SimonSays Gaze, whose
  keyboard opens a letter's variants this way. Verified in cursor dwell by
  AC-53.)*
- **REQ-F108 [Done*]** *(numbered out of range; belongs to this section.)*
  WHERE a button has a dwell time of its own
  (`SSButton::SetDwellTimeMs( ms )`, `ms` > 0), THE SYSTEM SHALL use it in
  place of `DwellTime` for that button's fixation; the lock-on, the
  tolerance and the cooldown stay the process-wide ones. 0, the default,
  uses `DwellTime`. *(Added 2026-11-16 for SimonSays Gaze, where keys and
  other buttons need different dwell times. Verified in cursor dwell by
  AC-54.)*

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
  `ExternalClick`, (3) a streaming direct-gaze device (HID reader or Tobii
  engine, REQ-F94) → `HidDwell` **unconditionally**, (4) a direct-gaze device
  open but silent (REQ-F29) → `Off`, (5) the no-presence gate (REQ-F27) →
  `Off`, (6) vendor background services only → `Off` (REQ-F28, 2026-10-23),
  (7) anything that can drive the cursor → `MouseDwell` (REQ-F28); producing
  `{mode, confidence, reason}`. Cursor kinematics take no part in any decision.
  *(2026-10-18: "streaming" replaced "live" in step (3) and step (4) was added,
  so that a switched-off tracker no longer reaches `MouseDwell` and a glance
  away no longer counts as "not live".)* *(The v1.5
  "MouseDwell when the cursor is itself gaze-like" refinement was removed after
  the 2026-07-06 PCEye5 field test: burst cursor warps from TD Control / WEC /
  hand-mouse use scored "gaze-like" and steered Auto onto a parked cursor.)*
- **REQ-F24 [Done]** WHILE selection is `Auto`, THE SYSTEM SHALL refresh passive
  signals + direct-gaze liveness (~3 s) to feed the detector. Cursor sampling
  is retired: no timer feeds `MouseMotionClassifier` (dormant since v1.6).
  THE timer id backing this refresh SHALL be unique among the main window's
  timers (§12).

  > *Defect and resolution (2026-09-21, [`AGENT.md`](../../AGENT.md) §4).*
  > `TIMER_DWELL_DETECT` and `FAST_TIMER_CHECK_ZORDER` were **both defined as
  > `2`** in `src/MainWindow.cpp`, and the `WM_TIMER` handler dispatched with
  > an `if / else if` chain that tested the z-order ids first. Three
  > consequences, none of them visible at compile time:
  >
  > 1. The dwell branch was **unreachable**, so `EvaluateDwellAutoMode()` had
  >    no reachable caller at all: the periodic `Decide()` and the REQ-F25
  >    hysteresis never ran. Auto only ever showed the `dwellDetectedMode`
  >    restored from the registry (REQ-F26).
  > 2. `SetTimer` replaces an existing id on the same window, so starting Auto
  >    retimed the 400 ms fast z-order guard to the 3000 ms detector cadence.
  > 3. Selecting any non-`Auto` mode made `SyncDwellTimers` call
  >    `KillTimer(hwnd, TIMER_DWELL_DETECT)`, which killed the **fast z-order
  >    timer** while `s_fastZOrderCheckTimerId` stayed non-zero — so the
  >    `if( !s_fastZOrderCheckTimerId )` restart guard never fired again and
  >    the taskbar Z-order recovery stayed off for the rest of the session.
  >
  > Fixed by renumbering `TIMER_DWELL_DETECT` to `3` (free since the v1.6
  > sampler retirement) and converting the handler to a `switch`, so a future
  > collision is a compile error (MSVC C2196) instead of a silent dead branch.
  >
  > **Verification status (2026-09-21):** with the timer live, Auto was
  > re-tested on a Tobii Eye Tracker 4C and resolves direct-gaze dwell
  > correctly — the REQ-F23 priority-(4) branch (live direct gaze → `HidDwell`)
  > and the REQ-F24 refresh itself are now exercised for real (AC-17).
  > The other two Auto outcomes are still **unverified**, because a streaming
  > 4C cannot reach them: priority (5) presence-without-live-gaze →
  > `MouseDwell` (REQ-F28 / AC-13) needs a tracker that is present but not
  > streaming, and the no-presence gate → `Off` (REQ-F27 / AC-3) needs a
  > machine with no tracker or tool at all.
  > *Both verified since: AC-3 Pass 2026-10-26 (no tracker or tool → `Off`)
  > and AC-13 Pass 2026-11-04 (Irisbond Hiru in mouse mode → `MouseDwell`).*
- **REQ-F25 [Done*]** THE SYSTEM SHALL apply a new Auto decision only after it
  repeats for `HysteresisCount` consecutive evaluations (anti-flapping). *Count is
  hardcoded `3` (`DWELL_HYSTERESIS_COUNT`); not yet registry-configurable (§18).*
- **REQ-F26 [Done]** WHEN the Auto-resolved mode changes, THE SYSTEM SHALL persist
  it (`dwellDetectedMode`) so Auto restores it next launch.
- **REQ-F27 [Done]** WHILE selection is `Auto` AND no eye-tracking presence is
  detected (no HID eye tracker, no known eye-control tool, no Windows Eye
  Control, no streaming or open direct-gaze device), THE SYSTEM SHALL resolve the detected mode to
  `Off` — dwell is never enabled from cursor kinematics alone. Fresh calibration
  (REQ-F44) still takes priority over this gate while valid. (Sampling policy:
  see REQ-F24.)
- **REQ-F28 [Done]** WHILE selection is `Auto` AND **cursor presence** is
  detected (a HID eye tracker, a running **cursor tool** per REQ-F31, or
  Windows Eye Control) AND no direct-gaze device is open (streaming or not) AND
  no known clicking tool, THE SYSTEM SHALL resolve the detected mode to
  `MouseDwell`: presence alone enables dwell **where the tracker cannot be read
  directly** (e.g. Irisbond in mouse mode, OptiKey, Windows Eye Control on a
  non-Tobii device). WHEN the only presence is **vendor background services**
  (REQ-F31), THE SYSTEM SHALL resolve `Off` (reason "Tobii software running,
  tracker not readable"). *Consequence: a hand-mouse user on a machine with a
  cursor tool still gets dwell auto-firing until they run the MOUSE probe
  (calibration) or force Off — see §17. On Tobii machines whose Stream Engine
  opens the device, REQ-F29 decides instead.* *(Amended 2026-10-23: Tobii
  Service / Experience / Core and the platform runtime run all the time once
  installed, even with no tracker in use, and never move the cursor; counting
  them as presence turned on cursor dwell for every Tobii owner. Verified by
  AC-37.)*
- **REQ-F29 [Done]** WHILE selection is `Auto` AND a direct-gaze device is
  open (`GazeProviderChain::DirectDeviceOpen`: the Stream Engine created and
  subscribed the device) AND it is not streaming (REQ-F94), THE SYSTEM SHALL
  resolve the detected mode to `Off`, overriding presence from processes and
  Windows Eye Control: the tracker is switched off, so nobody is driving the PC
  with it. The dwell window's HID line SHALL then read "connected (tracking
  off)" (`DWELL_SIGNAL_HID_OFF_ID`). *Basis — 4C probe, engine 2.2.3.3008,
  2026-10-18: with tracking off the device is listed (`tobii-ttp://IS404-…`)
  and opens with no error, but delivers zero gaze callbacks in 10 s; with
  tracking on it delivers ~90 callbacks/s whether the user looks at the screen
  (~90 % valid) or away (validity 0). Only the HID reader's "device open" is
  not reported (default `false`), so Hiru keeps REQ-F28. Verified in the app
  by AC-20 (2026-10-18).*

### 6.4 Passive detection

- **REQ-F30 [Done]** THE SYSTEM SHALL detect HID eye-tracker presence by enumerating
  HID interfaces (usage page `0x12`, usage `0x01`), opening with access `0`
  (query-only, never blocking the tool). *Trackers whose HID interface denies
  user-mode opens (the Tobii virtual "Eye Tracker HID", `err=5`) are invisible
  to this check by design of their driver — they are covered by the process
  table (REQ-F31) and Windows Eye Control signal (REQ-F32) instead.*
- **REQ-F31 [Done]** THE SYSTEM SHALL detect a running eye-control tool via a
  Toolhelp32 process scan against a known-tools table (EasyClick, Hiru, TD Control,
  OptiKey, `tobiidynavox*`, `tobii*`, `tdx.*`, `platform_runtime_is*`),
  case-insensitive, flagging tools known to issue their own clicks and giving
  each row a **role**: *cursor tool* (can move the Windows cursor: EasyClick,
  Hiru, TD Control, OptiKey, `tobiidynavox*` EyeAssist, `tdx.*` Computer
  Control) or *vendor service* (other `tobii*`, `platform_runtime_is*`). THE
  scan SHALL check **every** running process — each takes the first matching
  row, specific prefixes first — and report whether any cursor tool and any
  clicking tool runs; the displayed name prefers clicking > cursor > service
  (see §Appendix C). *(Amended 2026-10-23: the scan used to stop at the first
  match, so an always-running Tobii service enumerated first hid a cursor or
  clicking tool running alongside it.)*
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
- **REQ-F62 [Done]** A separate **Gaze / Dwell-click window** (`IDD_DWELL_DIALOG`,
  in `resources/SSDwellDialog.rc` since 2026-11-17, REQ-F109)
  SHALL provide: mode radios (Auto/Mouse/HID/Off), dwell-time/tolerance/cooldown
  sliders with numeric readouts, a progress-color picker (`ChooseColor`), the
  "Move away before dwell can activate again" checkbox (REQ-F95, applied live,
  restored on Cancel), a live
  "Detected" panel, two calibration probes, a Reset button (REQ-F67), and
  OK/Cancel.
- **REQ-F63 [Done]** THE HID radio ("HID eye tracker") SHALL be enabled
  whenever gaze can be read directly — a HID eye tracker is present, OR a
  direct-gaze device is open (`DirectDeviceOpen`: a Tobii tracker through the
  Stream Engine, in-process or via the 64-bit helper, even with tracking off),
  OR gaze is streaming — AND always while it is the selected option. The same
  rule SHALL apply at open, on the ~2 s signals refresh and after the LOOK
  probe. A saved HID selection SHALL be kept even when no tracker is readable;
  it uses gaze only (§8.3), so nothing dwells until gaze arrives. The
  "Detected" HID line SHALL distinguish *none / present (not streaming) /
  connected (tracking off) / streaming gaze*. *(Amended 2026-10-25: the radio
  used to follow raw HID presence only, so on Tobii machines the LOOK probe
  enabled it and the 2 s refresh disabled it again; and a saved HID choice was
  reset to Auto at open — developer report. AC-44–AC-46 Pass.)*
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
  English. *(In SimonSays. Since 2026-11-17 the window asks its host for each
  string by its id (REQ-F109); SimonSays' host answers with
  `GetLocalizedString`, another host with its own texts.)*
- **REQ-F67 [Done]** THE dwell window SHALL provide a **Reset** button at the
  bottom-left that restores every dwell setting to its first-run default
  (mode selection Auto, time 500 ms, tolerance 25 px (35 until 2026-11-25), cooldown 300 ms, progress
  color = Windows accent color, detected mode Off, move away to re-arm on),
  updating the dialog controls
  and applying the tuning live to the probes; the reset is committed only on OK
  and discarded on Cancel. Defaults are single-sourced (`DWELL_DEFAULT_*` in
  `stdafx.h`, shared with `RegistryManager`).
- **REQ-F109 [Partial]** *(numbered out of range; belongs to this section.)*
  THE dwell window SHALL be usable by an application other than SimonSays,
  through an entry point that takes only the dwell values (`SSDwellValues`)
  and a host (`SSDwellWindowHost`) that supplies the window's strings by
  their ids, whether the UI language is right-to-left, the progress colour
  Reset restores, and whether the progress-colour button and the Save
  diagnostic report button are shown (both by default). The window SHALL
  depend on none of `utils.cpp`, the string tables or `Settings`; the slider
  and right-to-left helpers it uses live in `SSDialogUtils` and its template
  in `resources/SSDwellDialog.rc`. SimonSays' entry point
  (`ShowDwellSettingsDialog( HWND, HINSTANCE, Settings & )`) SHALL keep its
  behaviour exactly, as a host showing both buttons with SimonSays' strings,
  RTL rule and the Windows accent. *(Added 2026-11-17 for SimonSays Gaze,
  which hides both buttons: its accent colour is its own setting and it
  writes nothing derived from gaze. AC-55, AC-56.)*
- **REQ-F110 [Done]** *(numbered out of range; dwell window UI.)* THE dwell
  window's mode choices SHALL be radio buttons that dwell can choose
  ([`ssbutton.spec.md`](ssbutton.spec.md) BTN-F89 to BTN-F93), except
  **Off**: WHEN the tracking point rests on Off, THE SYSTEM SHALL NOT choose
  it; the mouse, Space and the arrow keys still can. *(Added 2026-11-22:
  Off chosen by gaze and confirmed with OK would leave a gaze user no way
  back by gaze. AC-58.)*

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
  `%LocalAppData%\SimonSays\debug\hid_dump.txt` (`GetDebugFolder`, created if
  missing) containing: app/OS/display/DPI;
  passive detection; a hex dump of every CloudStore `*eyecontrol*` blob; **all**
  HID top-level collections — each with its registry-side identity (device
  description, instance ID, hardware IDs — readable even when the device blocks
  handle queries), a query-only probe logging the exact failing step +
  `GetLastError` per metadata query (attributes / preparsed / caps / strings),
  and, when blocked, a retry with the shared `GENERIC_READ` open `SSGazeReader`
  uses; every HID eye-tracker device (usage 0x12/0x01 detail);
  the unique image names of all running processes; HID capabilities and value
  caps; a decoded gaze capture (raw + mapped px); and a min/max gaze-range
  summary; and the Stream Engine status section (REQ-F101). It SHALL be
  written **only on request**: from the dwell window's button (REQ-F102), or
  once at reader-thread start when env var `SIMONSAYS_HID_DUMP` is set. The
  compile-time switch `SSGAZE_ENABLE_HID_DUMP` (default `0` since 2026-10-22;
  `1` forces the startup dump) remains for test builds. Each dump overwrites
  the previous file; a startup dump and a button dump never overlap. *(Amended
  2026-07-29: moved from directly in the app-data root into its own `debug\`
  subfolder, shared with other diagnostic output — see
  [`import-export.spec.md`](import-export.spec.md) PORT-F40. Write-only: no
  search order applies, and dumps from earlier versions are left in the root,
  unread — no migration.)* *(Amended 2026-10-22: no longer written at every
  startup; the switch was `1` while device detection was being broadened.)*
- **REQ-F102 [Done]** THE dwell window SHALL provide a **Save diagnostic
  report…** button (`IDC_DWELL_DUMP_BUTTON`, right column below the Detected
  group). WHEN pressed, THE SYSTEM SHALL write the REQ-F80 report on a
  background thread (`SSGazeReader::StartDiagnosticDump`), disable the button
  and show "Writing diagnostic report…" in the status line, keeping the dialog,
  probes and gaze dwell responsive. WHEN done, it SHALL re-enable the button
  and show "Diagnostic report saved." and open Explorer with `hid_dump.txt`
  selected, or show "Could not save the diagnostic report." on failure. A press
  while a dump runs SHALL do nothing. `SSGazeReader::Stop` SHALL join the dump
  thread, so exiting during a dump stays clean (REQ-F73); a result arriving
  after the dialog closed is dropped. Strings: `DWELL_DUMP_BUTTON_ID`,
  `DWELL_STATUS_DUMP_{RUNNING,SAVED,FAILED}_ID` — authored in all 18
  languages (the Arabic, Chinese, Hebrew, Hindi, Japanese, Korean and Russian
  wording awaits native review). *(Added 2026-10-22. Since 2026-11-17 a host
  other than SimonSays may hide the button, REQ-F109; SimonSays shows it.)*

### 6.10 Tobii Stream Engine provider (Layer 1 backend)

- **REQ-F90 [Done]** THE SYSTEM SHALL read gaze from Tobii trackers whose HID
  interface is ACL-locked (AC-16) via the legacy Stream Engine C API, loading
  the **user-installed** `tobii_stream_engine.dll` at runtime with
  `LoadLibrary` — never bundling or redistributing it — resolving all exports
  dynamically, and requesting the Interactive field of use on engine ≥ 4
  (`tobii_device_create` arity resolved per `tobii_get_api_version`). Candidate
  DLL paths derive from the directories of running `tobii*` / `tdx.*` /
  `platform_runtime_is*` processes. The engine must match the process
  architecture; an app that finds only an engine of the other architecture
  reaches it through the helper built for that architecture (REQ-F96–F100). *(Amended 2026-10-22: the engine
  code moved to `SSStreamEngine.{h,cpp}`, shared by the app and the helper.)*
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
- **REQ-F94 [Done]** THE Tobii reader SHALL report *tracking* (`IsTracking`: a
  gaze callback of **any** validity within the last 1000 ms while the device is
  open) separately from *live* (`HasLiveGaze`: a valid sample within 200 ms),
  and *device open* (`IsDeviceOpen`) from device-create + subscribe success
  until teardown. Only callback arrival times are kept for this, never
  coordinates (REQ-N08). `IGazeProvider::IsTracking` defaults to `IsLive` and
  `IsDeviceOpen` to `false` for providers that cannot tell (cursor, HID reader:
  whether the Hiru keeps sending reports when the user looks away is
  unverified). The chain exposes `DirectTracking` / `DirectDeviceOpen`.
  *Verified by AC-20 (2026-10-18).*

### 6.11 Cross-architecture Stream Engine host (Layer 1 backend)

*Added 2026-10-22. Motivation: on the Eye Tracker 5 test machine (Windows
build 26300, current Tobii Experience) the only installed engine is a 64-bit
`tobii_stream_engine.dll` (4.25, in `C:\Program Files\Tobii\Tobii EyeX`). The
default 32-bit build fails to load it with `ERROR_BAD_EXE_FORMAT` (193), so the
ET5 was not detected at all; the 4C / PCEye5 stacks ship 32-bit engines, so no
single build covers both.*

- **REQ-F96 [Done]** WHEN the in-process engine search finds no usable copy
  AND at least one copy failed with `ERROR_BAD_EXE_FORMAT` AND the helper of
  the other architecture exists next to the app's executable
  (`SSTobiiGaze::HelperUsable`), THE SYSTEM SHALL switch the Tobii reader to
  the helper route for the rest of the session. For a 32-bit app the helper
  is `SSGazeHelper64.exe` and the app must be running on 64-bit Windows; for
  an x64 app it is `SSGazeHelper32.exe`. Otherwise it SHALL behave as before:
  matching-architecture engines load in-process (4C, PCEye5 with the 32-bit
  build), and machines without Tobii software start no helper. *(Amended
  2026-11-14: the reverse case — x64 app, 32-bit engine — was out of scope
  and the x64 build never used the helper; it now uses the 32-bit helper.
  Wanted by SimonSays Gaze, a native x64 host of this code. The 32-bit app
  behaves as before. The x64 direction was field-checked on a 4C,
  2026-11-15: AC-50.)*
- **REQ-F97 [Done]** THE helper SHALL be a separate project
  (`SimonSaysGaze64.vcxproj`), built as x64 (`SSGazeHelper64.exe`) and as
  Win32 (`SSGazeHelper32.exe`) from the same sources *(Win32 added
  2026-11-14; it was x64-only. The x64 build was `SimonSaysGaze64.exe`, after
  the project, until 2026-11-18: renamed so it is not taken for SimonSays
  Gaze, a separate program; the project keeps its name. Upgrading removes
  the old file, since the installer removes the previous version; AC-57)*, with no window, console, tray icon or log file,
  using the Windows SDK only (REQ-N02), statically linked CRT. It SHALL run the
  same engine code as the app (`SSStreamEngine.{h,cpp}`: DLL discovery, device
  open for Interactive Use, subscription, pump, reconnect), so the two routes
  cannot drift.
- **REQ-F98 [Done]** THE helper SHALL publish one record
  (`SSGazeHelperRecord`, `SSGazeHelperProtocol.h`, 17 × 4-byte fields, same
  layout in 32/64-bit) in a pagefile-backed section **without a name**, created
  by the app and passed to the helper **only** by handle inheritance (explicit
  `PROC_THREAD_ATTRIBUTE_HANDLE_LIST`; inheritance flags set just for the
  launch). The record holds a format version, a heartbeat tick, engine status,
  the device-open flag, the last-callback tick (REQ-F94) and one valid gaze
  point in normalized coordinates with its tick, overwritten on every update
  and read through a sequence counter. The app maps coordinates itself (same
  mapping and primary-monitor limitation as REQ-F91). A heartbeat older than
  1000 ms SHALL read as device not open / not tracking, so REQ-F29 / F94 / §8.3
  behave as with the in-process route. *Rationale (Tobii EULA v3.0 §1.3): an
  unnamed, non-inheritable-by-default section cannot be opened by any other
  process, so SimonSays never makes gaze available to software that could
  store or transfer it.*
- **REQ-F99 [Done]** THE app SHALL launch the helper `CREATE_NO_WINDOW`,
  suspended, assign it to a kill-on-close Job object, and pass it the section,
  an unnamed stop event, an unnamed wake event and a `SYNCHRONIZE` handle to
  itself. THE helper SHALL exit when the stop event is set OR the app process
  ends. `SSTobiiGaze::Stop` SHALL set the stop event, wait up to 2 s, then
  close the job (ending anything left). An unexpected helper exit SHALL be
  retried every 2 s; after 3 exits in a row within 60 s of launch each, every
  30 s. `Wake()` SHALL be forwarded to the helper's wake event.
- **REQ-F100 [Done]** THE helper SHALL keep REQ-F93 / REQ-N04: device-create
  / subscribe failures are retried, never fatal, and nothing is taken
  exclusively. *Coexistence with Tobii Experience on the ET5 verified by
  AC-26 (2026-10-22).*
- **REQ-F101 [Done]** THE REQ-F80 dump SHALL include a "Tobii Stream Engine"
  section listing each candidate DLL path with its load result (usable /
  other architecture / not found / error code), the engine version of a usable
  in-process copy, the helper path and whether it is usable, the expected
  route, and the reader's route / helper state at dump time. It SHALL contain
  **nothing derived from the tracker's sensors** — no tracking state, no
  validity, no coordinates, no counts — and no device URL (it carries the
  device serial number), because the dump is sent to the developer, i.e.
  transferred to another computing device (Tobii EULA v3.0 §1.1, REQ-N08).

### 6.12 Non-functional

- **REQ-N01 [Done]** Win32 + C++ at the project's standard (MSVC default ≈ C++14,
  toolset v145); SHALL NOT bump the standard.
- **REQ-N02 [Done]** Windows SDK only; no third-party deps. Links `hid.lib`,
  `setupapi.lib`, `comdlg32.lib` (and existing `d2d1`/`dwrite`/`msimg32`/`comctl32`).
  *Runtime `LoadLibrary` of a vendor DLL already installed by the user's own
  tracker software (REQ-F90) introduces no build-time dependency: nothing
  third-party is linked, bundled, or required, and the repository builds from
  the Windows SDK alone. This is a dependency-hygiene property; the licensing
  basis for the provider is recorded separately in §18 and REQ-N08.*
- **REQ-N03 [Done]** `std::atomic` for flags; `std::mutex` for compound state; no
  exceptions across thread boundaries (status/bool returns).
- **REQ-N04 [Done]** Concurrent operation: the HID device opens non-exclusively;
  the external tool keeps working (never `ERROR_SHARING_VIOLATION`).
- **REQ-N05 [Done]** No regression to existing cursor-only behavior when forced to
  Mouse; Auto SHALL NOT oscillate (hysteresis).
- **REQ-N06 [Done]** Identifiers and comments in English; UI localized.
- **REQ-N07 [Planned]** Clean under Application Verifier (timers killed, thread
  joined, handle closed, no leaks). *Designed for; not yet run under Verifier.*
- **REQ-N08 [Done]** THE SYSTEM SHALL NOT persist to disk, transmit over any
  network, or otherwise store gaze data obtained through the Stream Engine
  provider (REQ-F90–F93, F96–F100), in raw or mapped form. Such samples SHALL
  exist only in `SSTobiiGaze::m_latest` (in-memory, mutex-guarded, overwritten
  on each callback), in the transient dwell state of the consuming `SSButton`,
  and — on the helper route only — in the helper's single
  `SSGazeHelperRecord` inside the unnamed, handle-inherited section of
  REQ-F98 (same machine, in memory, overwritten on every update, openable by
  no other process). *(Amended 2026-10-22.)*
  *The Tobii end-user licence (v3.0, Tobii Tech AB) §1.1 grants Interactive
  Use — Data as user input for interactive experiences in games or other
  software — excluding the right to store or transfer Data to another
  computing device or network. SimonSays therefore keeps Stream Engine gaze in
  memory only, by design and by requirement. Passing it between two processes
  on the same computer is neither storage nor a transfer to another device;
  §1.3 is met by making the record unreachable for any other process.*
  **Scope note:** REQ-F80's diagnostic dump records a decoded gaze capture and
  a min/max range summary. It is a HID-reader facility covering HID devices
  brought up per §13, and it SHALL NOT be extended to the Stream Engine
  provider. Keep this prohibition commented in `SSTobiiGaze.cpp`.
- **REQ-N09 [Done]** THE dwell window SHALL use 10 pt Segoe UI, like Settings
  ([`settings.spec.md`](settings.spec.md) SET-N05), and SHALL fit a 1280×720
  work area (the app is DPI-unaware, so the logical size counts); hence the
  two-column layout of §11 (about 870 × 510 px at 96 DPI; 884 × 581 px since
  the lock-on slider was added, 2026-10-24, AC-43). Verified by AC-22.
  *(Added 2026-10-21.)*
- **REQ-N10 [Done]** Every build route SHALL produce the x64 gaze helper
  (`SSGazeHelper64.exe`) before it is needed: (1) in `SimonSays.sln` the
  installer project depends on `SimonSays` and `SimonSaysGaze64`, and the
  solution's Win32 configurations map the helper to x64, so building the
  solution or the installer builds both first; (2) a command-line build of
  `SimonSays.vcxproj` for Win32 runs the `BuildGazeHelper` after-build target,
  which builds the helper as x64 in the same configuration (skipped inside
  Visual Studio and solution builds). Verified by AC-47 / AC-48. *(Added
  2026-10-25.)* The x64 app gets the 32-bit helper the same two ways: the
  solution's x64 configurations map the helper to Win32, and a command-line
  x64 build runs `BuildGazeHelper32`; the output is
  `x64\$(Configuration)\SSGazeHelper32.exe`. *(Added 2026-11-14; AC-51.)*
- **REQ-N11 [Done]** THE installer build SHALL fail rather than ship without
  the helper: a pre-build event (`SimonSaysInstaller\CheckInstallerInputs.ps1`)
  fails when `Release\SimonSays.exe` is not x86 or `Release\SSGazeHelper64.exe`
  is missing or not x64; a post-build event (`VerifyMsiContents.ps1`, after
  `FixMsiUnicodeNames.ps1`) opens the MSI read-only and fails unless both exes
  are in its File table and install to the same directory. Visual Studio's
  "File 'SSGazeHelper64.exe' targeting 'x64' is not compatible with the
  project's target platform 'x86'" warning is expected and harmless: the file
  is packaged and runs as 64-bit. Verified by AC-48 / AC-49. *(Added
  2026-10-25.)*

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
                                                              │         │   └─(64-bit engine only)→SSGazeHelper64.exe (unnamed shared record)
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
| `include/SSStreamEngine.h` / `src/SSStreamEngine.cpp` | Shared Stream Engine session (DLL discovery, device, pump) — compiled into the app and the helper (REQ-F97) |
| `include/SSGazeHelperProtocol.h` | Helper ↔ app shared record + sequence-counter read/write (REQ-F98) |
| `SimonSaysGaze64/SimonSaysGaze64.vcxproj` / `SimonSaysGaze64/main.cpp` | Stream Engine host for the other architecture: x64 `SSGazeHelper64.exe`, Win32 `SSGazeHelper32.exe` (REQ-F96–F100) |
| `include/SSDwellModeDetector.h` | Detector + `MouseMotionClassifier` (header-only) |
| `include/SSDwellWindow.h` / `src/SSDwellWindow.cpp` | Dwell settings window + probes; the host-neutral entry point and `SSDwellWindowHost` (REQ-F109) |
| `src/SSDwellWindowSimonSays.cpp` | SimonSays' entry point (`Settings`) and its host: strings, RTL, accent colour (REQ-F109) |
| `include/SSDialogUtils.h` / `src/SSDialogUtils.cpp` | Slider and right-to-left dialog helpers, moved unchanged from `utils.cpp` so another host can compile the window (REQ-F109); `utils.h` includes the header |
| `resources/SSDwellDialog.rc` | The window's dialog template, included by `SimonSays.rc` through TEXTINCLUDE 3 (REQ-F109) |

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
  on the chain directly (direct-gaze tracking / device-open state is pushed via
  `SetDirectGaze`).
- `RegistryManager` stays decoupled from `SSDwellConfig`; `MainWindow` bridges
  `Settings` ↔ `SSDwellConfig` (`ApplyDwellSettingsToConfig`).
- `SSDwellMode` / `SSDwellModeSelection` are defined once in `SSButton.h` and reused
  everywhere (the detector reference's duplicate enums were removed).
- `SSDwellWindow.cpp` depends on `SSDialogUtils`, never on `utils.cpp`, the
  string tables or `Settings`; those stay in `SSDwellWindowSimonSays.cpp`
  (REQ-F109).

---

## 8. Detailed design

### 8.1 Dwell state machine (per `SSButton`)

States: **Idle → Dwelling → (Fire) → Cooldown → Re-arm wait → Idle** (the
re-arm wait only after a dwell fire with REQ-F95 on; it is process-wide).

- Arming (enter Dwelling): cursor mode via `WM_MOUSEMOVE`→`OnDwellMouseMove`; HID
  mode via the gaze router→`OnDwellMouseMove`. Guards: `IsDwellActive()` (active
  mode is Mouse/HID), `m_dwellEnabled`, button enabled, `now ≥ cooldownUntil`,
  and no pending re-arm block (`RearmBlocks`, REQ-F95).
- `StartDwell`: set anchor = point, start tick = now, progress = 0, `SetTimer`
  (`SSBUTTON_DWELL_TIMER_ID` 0xD3E11, 33 ms ≈ 30 fps).
- `UpdateDwell` (each tick): re-read the tracking point via `GetTrackingPoint`
  (provider per active mode → `ScreenToClient`). If outside client rect → cancel.
  If moved > tolerance → re-anchor + restart clock. When `elapsed ≥ DwellTime`:
  kill timer, set cooldown = now + `CooldownMs`, set activation = `Dwell`,
  `FireClick`.
- Cancel: kill timer, clear progress, repaint.

### 8.2 Gaze router (HID activation)

`SSButton::RouteGaze(screenPt)` (static): `WindowFromPoint` → if the window
belongs to this process ([`ssbutton.spec.md`](ssbutton.spec.md) BTN-N06) and
its class is `SSBUTTON_CLASS` → get instance via `GWLP_USERDATA` → `ScreenToClient` → if the
button is not already dwelling, `OnDwellMouseMove`. The button's own timer then
advances and self-cancels.

`MainWindow` runs an **always-on** `TIMER_DWELL_GAZE` (id 5, 50 ms). When
`GetActiveMode()==HidDwell` it reads `GetTrackingSample` and calls `RouteGaze`,
then `UpdateGazeHover` to move the gaze-hover highlight (REQ-F103); in any
other mode it calls `ClearGazeHover`. It
is always-on (not gated by `SyncDwellTimers`) so it also fires during the modal
dwell window (whose message loop dispatches the main window's `WM_TIMER`), arming
the probes. Mouse mode does **not** use the router (unchanged `WM_MOUSEMOVE` path).

### 8.3 Provider selection per mode

`GetTrackingPoint`: MouseDwell → `GetCursorSample`; HidDwell →
`GetHidDwellSample( out )` (direct gaze only); Off/ExternalClick → false. The
gaze router (`TIMER_DWELL_GAZE`) uses the same call. HidDwell **never** falls
back to the cursor — whether `Auto` resolved it or the user forced it — so a
blink, a glance away, or a tracker switched off or unplugged while the mode is
`HidDwell` (e.g. restored at launch, REQ-F26) never becomes dwell under a
resting cursor. *(2026-10-18: no fallback for Auto; until 2026-10-25 a forced
HID mode still fell back to the cursor. That was removed when the HID option
became available to Tobii trackers (REQ-F63): an ET5 / 4C with tracking off
would have turned the whole app into cursor dwell, surprising a carer using
the mouse. "Mouse cursor" is the choice for cursor dwell.)* Two rules make this
hold inside `SSButton` (added after AC-20(c) failed on 2026-10-18: a gaze dwell
kept counting with no point, and `WM_MOUSEMOVE` still started dwells): (a) in
HidDwell, `WM_MOUSEMOVE` does not start or re-anchor a dwell — only the gaze
router does; (b) `UpdateDwell` never fires without a current tracking point:
a gap up to `SSBUTTON_DWELL_GAP_MS` (500 ms, a blink) keeps the dwell, a
longer one cancels it. MouseDwell always has a point, so it is unaffected.
`GetTrackingSample` prefers a live HID sample, otherwise the cursor.

### 8.4 Detector decision (`Decide()`)

Priority: (1) fresh calibration → mapped mode at high confidence
(`ConfirmedHidGaze` → HidDwell, `ConfirmedCursorIsGaze` → MouseDwell,
`ConfirmedMouseIsHand` → Off — the last returned MouseDwell until 2026-10-18,
contradicting §8.5); (2) `toolKnownToClick` → `ExternalClick`; (3) a streaming
direct-gaze device (`DirectTracking`, REQ-F94) → `HidDwell` unconditionally;
(4) a direct-gaze device open but silent (`DirectDeviceOpen`) → `Off`
(REQ-F29); (5) no presence at all (no HID device, no known tool, no Windows Eye
Control) → `Off` (REQ-F27); (6) vendor background services only
(`!CursorPresence()`) → `Off` (REQ-F28, 2026-10-23); (7) cursor presence →
`MouseDwell` (REQ-F28 — presence alone enables dwell). Both direct-gaze flags reach the detector via
`SetDirectGaze`, refreshed with the passive signals. `SyncDwellTimers`
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
| `int dwellToleranceRadius` | Move-away radius (px), REQ-F95 | |
| `int dwellCooldownMs` | Post-activation dead time (ms) | |
| `COLORREF dwellProgressColor` | Progress bar color | |
| `int dwellDetectedMode` | Auto restore: 0 Off · 1 Mouse · 2 HID · 3 ExternalClick | `SSDwellMode` |
| `bool dwellRearmOnLeave` | Move away to re-arm after a dwell activation (REQ-F95) | |
| `int dwellLockOnMs` | Lock-on delay before the progress starts (REQ-F104) | `SSDwellConfig` |
| `int dwellLeaveGraceMs` | Time off a button a fixation survives (REQ-F15); no UI yet | `SSDwellConfig` |

### 9.3 Registry (HKCU, under the existing SimonSays settings key, all `REG_SZ`)

| Value name | Default |
|---|---|
| `Dwell Mode Selection` | `0` (Auto) |
| `Dwell Time Ms` | `500` (800 until 2026-10-24; saved values are kept) |
| `Dwell Tolerance Radius` | `25` (`35` until 2026-11-25; a stored value is kept) |
| `Dwell Cooldown Ms` | `300` |
| `Dwell Progress Color` | Windows accent color (`GetAccentColor()`) |
| `Dwell Detected Mode` | `0` (Off) |
| `Dwell Rearm On Leave` | `1` (on; `"1"`/`"0"`) |
| `Dwell Lock On Ms` | `150` |
| `Dwell Leave Grace Ms` | `150` (0–500; anything else gives the default). No control in the dwell window yet: edited in the registry while it is tried by gaze (2026-11-24) |

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
bool DirectTracking() const;                 // either backend streaming, valid or not (REQ-F94)
bool DirectDeviceOpen() const;               // either backend has a device open (REQ-F29)
bool GetCursorSample(GazeSample*) const;
bool GetHidSample(GazeSample*) const;        // freshest direct-gaze sample
bool GetTrackingSample(GazeSample*) const;   // direct gaze if live, else cursor
bool GetHidDwellSample(GazeSample*) const;   // HidDwell's source: direct gaze only (§8.3)

// IGazeProvider (besides Start/Stop/IsLive/GetSample/Name)
virtual bool IsTracking() const;             // default: IsLive()
virtual bool IsDeviceOpen() const;           // default: false

// SSDwellConfig (singleton, thread-safe)
void SetDwellTimeMs(UINT); UINT GetDwellTimeMs() const;        // + tolerance, cooldown
void SetProgressColor(COLORREF); COLORREF GetProgressColor() const;
void SetModeSelection(SSDwellModeSelection); SSDwellModeSelection GetModeSelection() const;
void ApplyDetectedMode(SSDwellMode); SSDwellMode GetDetectedMode() const;
SSDwellMode GetActiveMode() const; bool IsDwellActive() const;  // Mouse||Hid

// SSButton (dwell surface)
void SetDwellEnabled(bool); bool IsDwellEnabled() const;
void SetDwellRepeat(SSButtonDwellRepeat, UINT timeMs);   // Once | RepeatWhileHeld | SecondStage (REQ-F106/F107)
SSButtonDwellRepeat GetDwellRepeat() const;
void SetDwellTimeMs(UINT); UINT GetDwellTimeMs() const;  // this button's own dwell time; 0 = process-wide (REQ-F108)
// WM_COMMAND notification of a completed second stage: SSBN_DWELLSECONDSTAGE
SSButtonActivation GetLastActivation() const;
static bool RouteGaze(POINT screenPt);        // arm the button under the gaze point

// SSDwellModeDetector (singleton, header-only)
void Tick(); void UpdatePassive(const PassiveSignals&); void SetDirectGaze(bool tracking, bool deviceOpen);
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
bool IsTracking() const; bool IsDeviceOpen() const;
Status GetStatus() const;                     // REQ-F101: route, helper state, engine version — no sensor data
static std::wstring HelperPath(); static bool HelperUsable(); // REQ-F96

// SSStreamEngine (free functions, shared with the helper)
SSStreamEngineRunResult SSStreamEngineRun(ISSStreamEngineSink&, const HANDLE* stops, DWORD n,
                                          HANDLE wake, bool returnOnOtherArchitecture);
SSStreamEngineLoadReport SSStreamEngineScanCandidates();  // REQ-F101 diagnostic scan

// SSDwellWindow
bool ShowDwellSettingsDialog(HWND owner, HINSTANCE, Settings&);  // true on OK (SimonSays)
bool ShowDwellSettingsDialog(HWND owner, HINSTANCE, SSDwellValues&, SSDwellWindowHost&);  // REQ-F109
// SSDwellWindowHost: Text(stringId), IsRtl(), DefaultProgressColor(),
//                    ShowProgressColor() = true, ShowDiagnosticReport() = true
```

---

## 11. UI specification

- **Dialog** `IDD_DWELL_DIALOG` (resource-based, in `resources/SSDwellDialog.rc`
  since 2026-11-17, populated programmatically), opened
  modally via `DialogBoxParam`; 10 pt Segoe UI in two 236-DLU columns
  (REQ-N09, 496 × 255 DLU): intro, mode, sliders, color and the re-arm checkbox
  on the left; the **Detect** and **Detected** groups on the right; Reset /
  OK / Cancel along the bottom. *(Layout amended 2026-10-21; 30 DLU taller
  since 2026-10-24 for the lock-on slider.)* Controls (`resource.h` IDs `IDC_DWELL_*` 6001–6033):
  intro text; **Activation mode** group with auto-radios Auto/Mouse/HID/Off; four
  sliders (`msctls_trackbar32`) + numeric edits, in this order: **lock-on delay
  (0–500 ms, REQ-F104)**, **dwell time (300–2000 ms)**, **tolerance (15–80 px)**,
  **cooldown (0–1000 ms)** *(lock-on moved first 2026-10-29: it is the first
  thing that happens on a button)*; **progress color** button
  (`ChooseColor`); **Move away before dwell can activate again** checkbox
  (`IDC_DWELL_REARM_CHECK`, REQ-F95) on its own row below it; **Detect** group
  with two probe placeholders + a status line;
  **Detected** group with three signal lines (HID / eye-control app / Windows Eye
  Control), refreshed on a ~2 s timer; a **Save diagnostic report…** button
  (`IDC_DWELL_DUMP_BUTTON`, 253,163 140×14 DLU, REQ-F102) below the Detected
  group; a bottom-left **Reset** button
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
| Tracking-point gap a dwell survives (blink) | 500 ms | `SSButton.cpp` `SSBUTTON_DWELL_GAP_MS` (§8.3) |
| Time off a button a dwell survives (*LeaveGrace*) | 150 ms default, 0–500 | `stdafx.h` `DWELL_DEFAULT_LEAVE_GRACE_MS`; registry `Dwell Leave Grace Ms` |
| Progress bar height | 4 px | `SSButton.cpp` |
| HID sample max age | 200 ms | `SSGazeReader.cpp` `kMaxAgeMs` |
| HID open retry | 2000 ms | `SSGazeReader.cpp` `kRetryMs` |
| Tobii sample max age | 200 ms | `SSTobiiGaze.cpp` `kMaxAgeMs` |
| Tobii setup retry / callback pump / error pump / errors→rebuild | 2000 ms / 10 ms / 100 ms / 20 | `SSStreamEngine.cpp` |
| Helper record poll / heartbeat max age / graceful stop wait | 10 ms / 1000 ms / 2000 ms | `SSTobiiGaze.cpp` `kPollMs` / `kHeartbeatMaxAgeMs` / `kHelperStopWaitMs` |
| Helper relaunch / back-off after 3 quick exits / "stable run" | 2000 ms / 30000 ms / 60000 ms | `SSTobiiGaze.cpp` `kRetryMs` / `kBackoffMs` / `kStableRunMs` |
| ~~Detector cursor sample~~ | retired in v1.6 (timer id 3 freed) | — |
| Diagnostic dump switch | `SSGAZE_ENABLE_HID_DUMP` = 0 (dump on request: button or `SIMONSAYS_HID_DUMP`; 1 forces it at every startup) | `SSGazeReader.cpp` |
| Detector passive refresh | 3000 ms | `TIMER_DWELL_DETECT` (id `3`; must stay unique among the main window's timer ids — see the REQ-F24 note) |
| Gaze router poll | 50 ms | `TIMER_DWELL_GAZE` |
| Device-change debounce | 800 ms | `TIMER_DWELL_DEVCHANGE` |
| Hysteresis count | 3 | `DWELL_HYSTERESIS_COUNT` |
| Calibration valid | 10 min | `SSDwellModeDetector.h` `kCalibrationValidMs` |
| Classifier window / min | 120 / 45 samples | `SSDwellModeDetector.h` (**dormant** since v1.6) |
| Classifier thresholds | `kStillPx 1.5`, `kTeleportPx 150`, `kFixationSpeed 300`, `kJitterLow 0.8`, `kJitterHigh 2.0` | `SSDwellModeDetector.h` (**dormant**, unvalidated) |
| Slider ranges | time 300–2000, tol 15–80, cooldown 0–1000, lock-on 0–500 | `SSDwellWindow.cpp` |
| First-run defaults | mode Auto, 500 ms, 25 px (35 until 2026-11-25), 300 ms, detected Off, re-arm on leave on, lock-on 250 ms (150 until 2026-11-07) | `stdafx.h` `DWELL_DEFAULT_*` (color: accent via `GetAccentColor()`) |

---

## 13. Diagnostics & device bring-up

To bring up a **new** HID eye tracker on any machine:

1. With the tracker's HID/streaming mode ON, run the Release exe, open the
   Gaze / Dwell-click window (`F3`) and press **Save diagnostic report…**
   (REQ-F102); slowly sweep gaze to all four screen corners for the next ~4 s.
   (Alternatively launch with env `SIMONSAYS_HID_DUMP=1` and sweep right after
   startup.)
2. Read `%LocalAppData%\SimonSays\debug\hid_dump.txt`. Use the **value caps** + **per-report
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

:: 2) Run the Debug build (clearer stacks): Debug\SimonSays.exe
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
  mouse user sees no auto-firing. *Was marked [Pass] before 2026-09-21, but the
  REQ-F24 timer was dead then, so the observable outcome (no auto-firing) held
  because no decision was ever applied — not because the gate resolved `Off`.
  Re-run 2026-10-26 by the developer on another computer with no tracker or
  eye-control software: passed.*
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
- **AC-13 (REQ-F28) [Pass]** With any detected eye-tracking device, known
  tool, or Windows Eye Control enabled — and no open direct-gaze device, no
  clicking tool — Auto resolves `MouseDwell` within ~9 s (3-tick hysteresis)
  and buttons dwell-activate under the cursor. *Since 2026-10-18 a Tobii
  machine whose engine opens the device is covered by AC-20 instead; test this
  one with Irisbond in mouse mode (Hiru running, no HID stream) or OptiKey. This
  criterion could not have passed before 2026-09-21: the evaluation timer never
  fired (REQ-F24 note), so no tick of that ~9 s ever elapsed.*
  *Result 2026-11-04: Pass on the Irisbond machine — Hiru software in mouse
  mode, saved mode Automatic; Auto settled on cursor dwell and phrases
  dwell-activated under the gaze-driven cursor (run by the developer).*
- **AC-14 (REQ-F80) [Pass]** On a machine with a Tobii tracker, requesting a
  dump (since 2026-10-22: the REQ-F102 button or `SIMONSAYS_HID_DUMP`; before
  that, every launch) produces `debug\hid_dump.txt` listing every HID collection (with VID/PID
  and usage page), the running-process names, and the Eye Control blob bytes.
  *Five dumps delivered from the PCEye5 and 4C machines (2026-07-04/05) drove
  the REQ-F31/F32 broadening (from the pre-2026-07-29 root location — the
  `debug\` move itself is compile-verified only).*
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
- **AC-12 (REQ-F67) [Pass]** Clicking Reset in the dwell window sets the
  radios to Auto, sliders/edits to 500/35/300 (800 before 2026-10-24) and
  lock-on 150 (250 since 2026-11-07), and the progress color to the
  Windows accent color; OK persists the defaults (color value removed from the
  registry), Cancel restores the pre-dialog settings. *(Scripted on
  `Release\SimonSays.exe` from seeded non-default values, 2026-11-02; Reset,
  Cancel and OK also work by mouse dwell, ssbutton.spec.md AC-19.)*

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
  streaming + coexistence in forced HID only.) Re-confirmed on the 4C
  2026-09-21, this time with the REQ-F24 evaluation timer actually running —
  the earlier passes had been reaching `HidDwell` without the periodic
  `Decide()` ever being applied.*
- **AC-18 (REQ-F64) [Pass]** Opening the dwell window *before* the gaze
  stream is up: within ~4 s of the stream starting (2 s signals timer + engine
  connect) the forced mode flips to HID and the LOOK probe becomes armable by
  gaze; if the stream drops, the probes fall back to Mouse forcing.
  *Confirmed with AC-17 in the 2026-07-07 re-test.*
- **AC-19 (REQ-N08) [Pass — by construction; re-check on every change to
  `SSTobiiGaze.cpp`]** No gaze sample originating from the Stream Engine
  provider reaches disk or network. Verify by inspection: `OnEngineGazePoint`
  calls only `StoreSample`; `StoreSample` writes only `m_latest` under
  `m_mutex`; no `SSTobiiGaze` path calls file or socket APIs; the REQ-F80 dump
  is written by `SSGazeReader` and reads from `SSTobiiGaze` only
  `GetStatus()` (route, helper state, engine version — REQ-F101), never a
  sample. Confirm on a
  Tobii machine that a full session leaves `%LocalAppData%\SimonSays\debug\`
  free of any Tobii-sourced gaze values. *Re-checked 2026-10-18 for REQ-F94:
  the callback now also calls `OnEngineCallback`, which stores only a tick
  count and a flag — no coordinates.* *Re-checked 2026-10-22 after the engine
  code moved to `SSStreamEngine.cpp` (the callback forwards only to the sink)
  and for the helper route — see AC-28.*
- **AC-20 (REQ-F29/F94, §8.3) [Pass]** On the 4C with the dwell selection
  on Auto: (a) tracking switched off in the Tobii software → within ~9 s Auto
  resolves `Off`, no button dwell-fires under a resting cursor, and the dwell
  window's HID line reads "connected (tracking off)"; (b) tracking on → Auto
  resolves `HidDwell` and buttons dwell by gaze; (c) looking away for ~10 s
  with tracking on → mode stays `HidDwell`, and nothing dwell-fires under the
  cursor meanwhile — including a key you looked at just before looking away;
  a blink during a gaze dwell still lets it complete; (d) launch with tracking off after a session that ended in
  `HidDwell` → no dwell under the cursor at any point. *2026-10-18 run: (a),
  (b), (d) passed; (c) failed — a key fired after looking away (see §8.3
  rules (a)/(b), fixed the same day); (c) passed on re-test the same day.*
  *Re-run 2026-10-24 on the 4C after the engine code moved to
  `SSStreamEngine.cpp` (§6.11): (a)–(d) all passed (developer).*
- **AC-21 (REQ-F95) [Pass]** Release build, mouse dwell, scripted pointer
  (progress-bar sampling as in the 2026-10-19 measurement): (a) option on, a
  pointer jittering ±3 px on a speaking key fires it **once** in 12 s; (b)
  moving ≥ 35 px away and back fires it again; (c) option off → repeats every
  ~1.1–1.25 s as before; (d) repeated physical clicks on the same key each
  activate it. Gaze side (HidDwell on the 4C): holding gaze fires once, looking
  away > 0.5 s or at another key and back fires again — checked by hand.
  *2026-10-19 scripted run (Release, ~5 s phrase "No puedo hablar…"): (b) and
  (c) passed (one fire before and one after moving away; 10 fires every
  1.08–1.25 s with the option off). (a) passed in 4 of 5 runs (exactly one
  fire in 12 s). The first run fired a second time at 7.3 s; cursor logging
  was added afterwards and never saw the pointer leave the key in the four
  later runs, so that one is unexplained (a real mouse movement during the
  test is the likely cause). (d) is by inspection only: the re-arm state is
  read solely in `OnDwellMouseMove` before `StartDwell`, and the click path
  (`WM_LBUTTONDOWN`/`UP` → `FireClick`) never touches it. Gaze side and dwell use in the app confirmed by the developer on the 4C the same day.*
- **AC-22 (REQ-N09) [Pass]** The dwell window opens at 10 pt in two columns
  (§11) and fits a 1280×720 work area; no label is clipped in English, German,
  Russian, Basque, or the mirrored Arabic layout; Tab order runs down the left
  column, then the right, then Reset / OK / Cancel; both probes are placed
  over their areas and still dwell and detect. *(Verified manually on
  `Release\SimonSays.exe`, 2026-10-21.)* *Correction 2026-10-22: in the
  right-to-left languages (Arabic, Hebrew) both probes were in fact placed one
  probe-width too far left, covering the Activation mode group and hiding its
  "Mouse cursor" / "Off" options — `CreateProbe` converted only the
  placeholder's top-left corner with `ScreenToClient`, which in a mirrored
  dialog yields the far edge. Fixed by mapping the whole rect with
  `MapWindowPoints`; re-verified by screen capture in Arabic (probes exactly
  over their placeholders, all four mode options visible) and Spanish
  (placement unchanged).*
- **AC-23 (REQ-F96–F99, ET5 machine) [Pass]** `Release\SimonSays.exe`
  (32-bit), Auto, tracking on in Tobii Experience: the helper starts within
  ~4 s, the dwell window's HID line reads "streaming gaze", the LOOK probe
  activates by gaze with the cursor untouched, and buttons dwell by gaze.
  *2026-10-22: helper start verified by script (683 ms after launch, parented
  to the app, command line carries only handle values); then by the developer
  on the ET5: HID line "streaming gaze", LOOK probe fired by gaze.*
- **AC-24 (REQ-F29 through the helper, ET5) [Pass]** Tracking switched off
  in Tobii Experience → within ~9 s Auto resolves `Off` and the HID line reads
  "connected (tracking off)"; tracking back on → `HidDwell`. *2026-10-22, by
  the developer on the ET5: off → "connected (tracking off)"; back on →
  "streaming gaze" again within a couple of seconds.*
- **AC-25 (REQ-F99) [Pass]** (a) tray Exit → helper gone within 2 s; (b)
  force-kill SimonSays → helper gone within 1 s; (c) force-kill the helper →
  the app relaunches it, no crash or error dialog; (d) gaze resumes after (c)
  within ~4 s. *2026-10-22 run on the ET5 machine (scripted): (a) app exited
  173 ms after the Exit command, helper 13 ms later; (b) helper gone 91 ms
  after the kill; (c) new helper 2.1 s after the kill. (d) is part of AC-23.*
- **AC-26 (REQ-F100, ET5) [Pass]** Tobii Experience (including its
  calibration and on/off toggle) keeps working the whole time the helper runs.
  *2026-10-22, by the developer on the ET5: everything worked normally
  throughout the AC-23 / AC-24 session.*
- **AC-27 (REQ-F96 regression) [Pass — core part]** On the 4C machine
  the engine still loads in-process with no helper process, and AC-17 / AC-20
  still pass; on a machine without Tobii software no helper starts. *2026-10-23,
  Tobii Eye Tracker 4C on the same PC as the ET5 tests, after the developer
  reinstalled Tobii Core (see §17): the 32-bit engine 2.2.3.3008 from
  `Program Files (x86)\Tobii\Tobii EyeX` loaded in-process, the device opened
  (create result 0), no helper process ran although the helper was usable, and
  the developer confirmed gaze dwell works (AC-17). Not re-run: AC-20 (tracking
  off) on the 4C, and the no-Tobii machine.*
- **AC-28 (REQ-N08, REQ-F98) [Pass — by inspection; re-check on every change
  to `SSStreamEngine.cpp`, `SSTobiiGaze.cpp`, `SimonSaysGaze64/main.cpp`]**
  The helper writes gaze only into `SSGazeHelperRecord`; neither the helper
  nor `SSStreamEngine.cpp` calls file, registry or socket APIs; the section,
  stop and wake objects are created with a null name; their handles are
  inheritable only during `LaunchHelper` and only through the explicit handle
  list. After a session, `%LocalAppData%\SimonSays\debug\` holds no
  Tobii-sourced gaze values (AC-19 re-checked for the extracted code).
- **AC-29 (REQ-F101) [Pass]** On the ET5 machine the dump's "Tobii Stream
  Engine" section lists `C:\Program Files\Tobii\Tobii EyeX\tobii_stream_engine.dll`
  as "other architecture (err=193)", the helper as usable, "Expected route:
  64-bit helper", and no tracking state, coordinates, counts or device URL.
  *(2026-10-22.)* *Fix 2026-10-23: the bare-name candidate
  (`tobii_stream_engine.dll`) can resolve to a copy the engine thread already
  loaded, so the section now prints "(resolved to <actual path>)"; and
  candidates after the first usable one had been labelled "missing exports"
  without being checked — their exports are now resolved before reporting.*
- **AC-30 (REQ-F80) [Pass]** With `hid_dump.txt` deleted, launching
  `Release\SimonSays.exe` writes no dump within 10 s; launching with
  `SIMONSAYS_HID_DUMP=1` writes it. *2026-10-22 (scripted, ET5 machine): no
  file after 10 s; with the env var, file after 152 ms.*
- **AC-31 (REQ-F102) [Pass]** Pressing the button disables it and shows the
  "writing" status, then re-enables it, shows "saved", opens Explorer with
  `hid_dump.txt` selected, and the file holds every section including the
  Stream Engine one. The dialog stays responsive throughout. *2026-10-22
  (scripted, Spanish UI): button text "Guardar informe de diagnóstico...",
  disabled + "Escribiendo…" right after the click, re-enabled + "Informe de
  diagnóstico guardado." after 139 ms, all sections present, an Explorer
  window on `debug` opened. Responsiveness during a long HID capture
  (Irisbond) not exercised — no HID tracker on this machine.*
- **AC-32 (REQ-F102, REQ-F73) [Pass]** Pressing the button, cancelling the
  dialog and exiting from the tray at once: no crash, app and helper exit.
  *2026-10-22 (scripted): app exited after 248 ms, helper gone. On this
  machine the dump is short (no HID capture), so the join was brief.*
- **AC-33 (REQ-F102, REQ-N09) [Pass]** The new button's label is not
  clipped in English, German, Basque or Russian, the Arabic layout mirrors it
  into the right place, and Tab order reaches it after the signals and before
  Reset / OK / Cancel. *2026-10-22: dialog captured in Spanish, German,
  Basque, Russian (English fallback) and Arabic; every translation measured
  in 10 pt Segoe UI fits the 140-DLU button (widest: French, 121 DLU). Tab
  order holds by resource order. The developer's check found clipping in the
  dialog — it was the probe defect recorded under AC-22, not the new button.*
- **AC-34 (REQ-F101 / N08) [Pass]** A button dump has the same status-only
  Stream Engine section as AC-29, with "helper running=yes" while the helper
  runs. *(2026-10-22.)*
- **AC-35 (REQ-F28, ET5 / 4C machine) [Pass]** With Tobii software running
  but the tracker not readable, Auto resolves `Off` within ~9 s, nothing
  dwells under the cursor, and the dwell window still names the Tobii software.
  *2026-10-23, AC-37 step A (scripted): starting from the restored
  `MouseDwell`, Auto switched to `Off` after 9.8 s with only the Tobii services
  running and no tracker streaming. The "Detected" line is unchanged by
  construction.*
- **AC-36 (REQ-F28 regression) [Pass]** On the ET5 / 4C with the tracker
  streaming, Auto resolves `HidDwell`; with tracking switched off, `Off`.
  *Confirmed by the developer at the tracker, 2026-10-24.*
- **AC-37 (REQ-F28, REQ-F31) [Pass]** With the Tobii services running and no
  tracker streaming, Auto resolves `Off`; adding a cursor tool resolves
  `MouseDwell`; a clicking tool resolves `ExternalClick` even with Tobii
  services enumerated alongside it; removing them returns to `Off`.
  *2026-10-23 (scripted, Release, selection temporarily Auto and restored
  afterwards; stand-ins were copies of `ping.exe` named `optikey.exe` and
  `tdcontrol.exe`; mode read from the persisted `Dwell Detected Mode`):
  Off after 9.8 s → MouseDwell after 8.8 s → ExternalClick after 7.2 s → Off
  after 8.7 s.*
- **AC-39 (REQ-F103) [Pass]** ET5 or 4C, gaze dwell: looking at a phrase
  gives it the hover color before and during its dwell; moving the gaze moves
  the highlight; a blink keeps it; looking away ~1 s clears it; switching
  tracking off clears it. *Confirmed by the developer, 2026-10-24.*
- **AC-40 (REQ-F103 regression) [Pass]** Mouse dwell and plain mouse use
  look exactly as before; with the mode Off no button highlights by gaze.
  *Confirmed by the developer, 2026-10-24.*
- **AC-41 (REQ-F104) [Pass]** Cursor dwell on the LOOK probe, tracking
  off: with lock-on 150 the bar appears ~150 ms after the cursor lands and the
  probe fires at ~950 ms (dwell 800); with 0 the bar starts at once and it
  fires at ~800 ms. *2026-10-24 (scripted, Release, HID line "connected
  (tracking off)", 3 runs each; times include the 33 ms dwell tick and ~50 ms
  of measurement overhead): lock-on 150 → bar seen at 229–263 ms, fired at
  946–987 ms; lock-on 0 → bar seen at 104–140 ms, fired at 842–856 ms. The
  bar is sampled 12 px into it, so it shows a few ticks after it starts. An
  earlier run with the 4C streaming was invalid (the window drove the probe
  by gaze).*
- **AC-42 (REQ-F104) [Pass]** The lock-on value is restored from the
  registry, Reset sets 150, Cancel restores the previous value, and Reset + OK
  saves 150. *2026-10-24 (scripted): saved 300 → slider 300; Reset → 150;
  Cancel and reopen → 300; Reset + OK → registry 150.*
- **AC-44 (§11) [Pass]** The lock-on delay row shows above Dwell time
  (then Tolerance, Cooldown); Tab goes from the mode radios to the lock-on
  slider, then Dwell time; arrow keys still move only within the four mode
  radios. *(Verified manually on `Release\SimonSays.exe`, 2026-10-29.)*
- **AC-43 (REQ-F104, REQ-N09) [Pass]** The dwell window
  fits 1280 × 720 and the lock-on label is not clipped in English, German,
  Basque, Russian and Arabic; scanning across phrases by gaze shows no bar
  flicker and holding the gaze still fires. *2026-10-24: captured in all five
  languages, 884 × 581 px, no clipping. Gaze part confirmed by the developer
  at the tracker, 2026-11-04.*
- **AC-47 (REQ-N10) [Pass]** With `Release\SSGazeHelper64.exe` deleted,
  `MSBuild SimonSays.vcxproj /p:Configuration=Release /p:Platform=Win32`
  rebuilds it, x64. *(2026-10-25, as `SimonSaysGaze64.exe`. 2026-11-18, after
  the rename: the Win32 builds wrote `Debug\` and `Release\SSGazeHelper64.exe`,
  x64.)*
- **AC-48 (REQ-N10/N11) [Pass]** With the helper deleted again,
  `devenv SimonSays.sln /Build "Release|Win32" /Project SimonSaysInstaller`
  builds the helper first, the pre-build check reports both exes, and the
  post-build check finds `SimonSays.exe` and `SSGazeHelper64.exe` in the MSI,
  both in `TARGETDIR`. *(2026-10-25: 2 projects built, 1 up to date, 0 failed;
  219 files in the MSI; one expected x64-in-x86 warning. To run again after
  the 2026-11-18 rename: the MSI must hold `SSGazeHelper64.exe`, version
  0.7.1, and not `SimonSaysGaze64.exe`. The developer, 2026-11-18: the MSI
  builds and its checks pass.)*
- **AC-49 (REQ-N11) [Pass]** The pre-build check passes on the real
  `Release\` folder and fails (exit 1, clear message) when the helper is
  missing or is an x86 file. *(2026-10-25, scripted on a scratch copy.
  2026-11-18, after the rename: passed on `Release\` with `SSGazeHelper64.exe`
  (x64), and failed with exit 1 and a clear message with it moved away.)*
- **AC-50 (REQ-F96/F97, x64 app with a 32-bit engine) [Pass — core part]** On a
  machine whose only Stream Engine is 32-bit (4C with Tobii Core / EyeX),
  `x64\Release\SimonSays.exe` in gaze dwell starts `SSGazeHelper32.exe`,
  buttons dwell by gaze, the helper is gone within 2 s of Exit, and the Tobii
  software keeps working. The diagnostic report names the 32-bit helper as
  the route. *(2026-11-15, not yet in SimonSays itself: an x64 test host
  built from these sources — `SSTobiiGaze`, the provider chain and one
  `SSButton`, with `SSGazeHelper32.exe` from this project beside it — took
  the helper route on a 4C with Tobii EyeX, engine 2.0.1.2323: device open
  in 954 of 958 timer ticks, 10 dwell activations in 45 s, helper gone when
  the host ended, Tobii processes unchanged. A 32-bit host on the same
  sources stayed in-process and started no helper. Then in SimonSays
  itself, same day, same machine: the developer ran
  `x64\Release\SimonSays.exe` built in Visual Studio with
  `SSGazeHelper32.exe` beside it and confirmed that its buttons dwell by
  gaze; after Exit no helper process was left, and the Tobii processes were
  the same as before. Not checked: the 2 s figure, the diagnostic report's
  route line, and the tracker line of the dwell window.)*
- **AC-51 (REQ-N10, x64) [Pending]** With `x64\Release\SSGazeHelper32.exe`
  deleted, `MSBuild SimonSays.vcxproj /p:Configuration=Release /p:Platform=x64`
  rebuilds it as a 32-bit executable; the Win32 build still produces
  `Release\SimonSaysGaze64.exe` as x64 (AC-47). *(2026-11-15, solution route
  only: the developer built Debug and Release, Win32 and x64, in Visual
  Studio, clean; the x64 configurations built `SSGazeHelper32.exe` (32-bit)
  into `x64\Debug\` and `x64\Release\` before the app, and the Win32
  configurations still built `SimonSaysGaze64.exe` as x64. The command-line
  route was not run: the machine at hand has Build Tools without ATL.)*
- **AC-52 (REQ-F106) [Pass — cursor dwell]** A button set to repeat every
  300 ms, under a resting pointer, activates once and then again about
  every 300 ms; it stops when the pointer leaves and does not start again
  while the pointer stays away; a physical click on it activates it exactly
  once although the pointer stays. *(2026-11-16, throwaway harness compiling
  `SSButton.cpp` with three more shared files, x64, forced cursor dwell,
  scripted pointer, defaults 500 / 250 / 300 ms: 6 activations in 2.65 s of
  rest (first at about 0.75 s, then five repeats), unchanged 1.5 s after
  leaving; a click gave exactly one more, reported as a mouse activation,
  with the pointer left on the button for 2 s. Not run in gaze dwell, and
  not in SimonSays, which has no such button.)*
- **AC-53 (REQ-F107) [Pass — cursor dwell]** A button set to a second stage of
  1000 ms sends `BN_CLICKED` once and, only if the pointer stays a further
  second, one `SSBN_DWELLSECONDSTAGE`; nothing more follows while the
  pointer stays. *(2026-11-16, same harness: a 1.4 s rest gave one click
  and no second-stage notification; after leaving and returning, a 2.5 s
  rest gave one click and one notification; 2.5 s more gave nothing. The
  top-edge progress was not looked at. Not run in gaze dwell.)*
- **AC-54 (REQ-F108) [Pass — cursor dwell]** A button with its own dwell time
  of 1500 ms does not fire within 1 s of rest and fires once within 2.2 s,
  while the process-wide dwell time is 500 ms. *(2026-11-16, same harness:
  0, then 1.)*
- **AC-44 (REQ-F63) [Pass]** With a Tobii tracker connected (no raw HID
  tracker), the "HID eye tracker" option stays enabled the whole time the
  dwell window is open. *2026-10-25 (scripted, Release, 4C streaming): enabled
  in all 21 samples, every 500 ms for 10 s.*
- **AC-45 (REQ-F63) [Pass]** Selecting "HID eye tracker" and pressing OK
  saves it; reopening the window shows it still selected and enabled, and gaze
  dwell works. With no tracker and no Tobii software, the option is disabled
  unless it is already selected. *Confirmed by the developer, 2026-10-25.*
- **AC-46 (§8.3, REQ-F04) [Pass]** With "HID eye tracker" selected and
  tracking switched off (or the tracker unplugged), nothing dwells under a
  resting cursor; mouse clicks work normally; switching tracking back on
  resumes gaze dwell. *Confirmed by the developer, 2026-10-25.*
- **AC-55 (REQ-F109, SimonSays unchanged) [Pass]** SimonSays' dwell window
  opens from the tray and with F3 and looks and behaves as before in
  English, in Spanish and in a right-to-left language: the four sliders and
  their edit boxes, the colour picker, the re-arm checkbox, both probes,
  Reset (accent colour), Save diagnostic report, OK (values saved) and
  Cancel (restored). *(2026-11-17: SimonSays builds Debug and Release Win32
  and Release x64 with only the noted pre-existing warnings. The developer,
  2026-11-18: the window behaves as before.)*
- **AC-56 (REQ-F109, another host) [Pending]** Another application compiles
  `SSDwellWindow.cpp` and `SSDialogUtils.cpp` without `utils.cpp`, includes
  `SSDwellDialog.rc`, and opens the window with its own strings and with the
  progress-colour and report buttons hidden. *(To be run by SimonSays
  Gaze.)*
- **AC-57 (REQ-F96/F97, rename) [Pass — helper part]** On the Eye Tracker 5 machine the
  32-bit SimonSays 0.7.1 starts `SSGazeHelper64.exe` (seen in Task Manager),
  gaze dwell works through it, and it closes with SimonSays; installing 0.7.1
  over 0.7 leaves no `SimonSaysGaze64.exe` in the install folder. *(2026-11-18:
  the helper builds under its new name, x64. The developer, the same day: the
  MSI installs and gaze dwell works on the Eye Tracker 5 through the renamed
  helper. Upgrading over an installed 0.7 was not reported.)*
- **AC-58 (REQ-F110) [Pass — cursor dwell]** In the dwell window, resting on
  Automatic or Mouse cursor chooses it; resting on Off chooses nothing; Off
  can still be clicked. *(2026-11-22, `check_radio.ps1`, SimonSays Debug
  Win32 with cursor dwell stored; the window forces HID dwell while the
  tracker streams gaze (REQ-F64), so the pointer rests count only with no
  one in front of it, BTN AC-34. By gaze: BTN AC-35, Pass, the developer,
  the same day.)*
- **AC-59 (REQ-F12, REQ-F15) [Pass — cursor dwell]** In cursor dwell, a pointer resting
  on a button and then moving within it fires the button once, after the
  lock-on plus the dwell time from where it first rested; a pointer that
  leaves the button for 200 ms and comes back keeps the dwell and fires it
  on the button; one that stays off for 700 ms cancels it and clears the
  bar *(first version, 500 ms off allowed; see AC-60 for the current
  values)*; a pointer leaving a button held in its second stage ends the hold at
  once. By gaze, looking at buttons and keys fires them without restarts
  from small eye movements. *(2026-11-24, `check_onbutton.ps1`, SimonSays
  Debug Win32, cursor dwell stored, nobody in front of the tracker; targets
  the dwell window's Automatic and Mouse cursor choices, 193 px wide:
  resting still fired after 1047 ms; moving 40 px either side every 100 ms
  after 1016 ms; 300 ms on, 200 ms off and back after 1015 ms; 300 ms on,
  700 ms off and back after 2156 ms, a new dwell. The held stage was not
  tried: no SimonSays button uses it. By gaze: neighbouring buttons fired
  with the one looked at (developer, 2026-11-24), which led to REQ-F111
  and a shorter LeaveGrace, AC-60.)*
- **AC-60 (REQ-F15, REQ-F111) [Pass]** In cursor dwell with
  the default LeaveGrace (150 ms): moving within a button still fires it on
  time; 100 ms off and back keeps the dwell; 300 ms off cancels it. With
  `Dwell Leave Grace Ms` = 400, 300 ms off keeps it. With two dwells kept
  alive at once (the pointer alternating between two buttons), the first to
  fire stays the only one fired. By gaze, neighbouring buttons no longer
  fire with the one looked at. *(2026-11-25, `check_onbutton2.ps1`,
  SimonSays Debug Win32, cursor dwell stored, nobody in front of the
  tracker, same targets as AC-59. Default: resting fired after 1031 ms;
  moving 40 px either side every 100 ms after 1031 ms; 300 ms on, 100 ms
  off and back after 1047 ms (kept); 300 ms on, 300 ms off and back after
  1719 ms (cancelled, a new dwell). 400 ms: 300 ms off and back after
  1016 ms (kept). 500 ms, alternating every 100 ms: the first fired after
  1125 ms; resting 400 ms on the other right after did not fire it. By
  gaze: no neighbouring button fired (developer, 2026-11-25).)*
- **AC-61 (REQ-F15, margin) [Pass — cursor dwell]** In cursor dwell with
  LeaveGrace 0: with the tolerance radius at 35 px, a point 20 px outside the
  button on no other button for 300 ms keeps the dwell, and 60 px outside
  cancels it; at 80 px, 60 px outside keeps it; a point within the margin but
  on the neighbouring button cancels it, and the neighbour does not fire with
  it. By gaze, dwells near a button's edge are kept and neighbours still do
  not fire. *(2026-11-25, `check_onbutton3.ps1`, SimonSays Debug Win32,
  cursor dwell stored, nobody in front of the tracker; the dwell window's
  Automatic and Mouse cursor choices, side by side. 35 px: resting fired
  after 1016 ms; 300 ms 20 px above and back after 1046 ms (kept); 300 ms
  60 px above and back after 1719 ms (cancelled, a new dwell). 80 px:
  60 px above and back after 1032 ms (kept). 19 px: 300 ms 5 px inside
  the other choice and back after 1718 ms (cancelled). A first run picked
  spots on the other choice and off the screen (clamped back within the
  margin); the app was right, the spots were not. `check_onbutton2.ps1`
  passed again (REQ-F15 LeaveGrace, REQ-F111). By gaze: the bar stuttered
  and stalled, and buttons fired as the point came back with the bar
  part-way (developer, 2026-11-25), which led to AC-62. These results are
  from the first version of the margin; rerun with AC-62.)*
- **AC-62 (REQ-F12, REQ-F15, pause) [Pass]** In cursor dwell: with the
  default LeaveGrace (150 ms), 300 ms on the button, 100 ms off beyond the
  margin and back fires about 100 ms after the reference (the time away
  does not count), not at it; 300 ms on, then staying 20 px outside the
  button (within the 35 px margin, on no other button) fires there at about
  the reference. AC-60 and AC-61 pass again, their "kept" times now the
  reference plus any time away beyond the margin. By gaze, the bar moves on
  smoothly near a button's edge, never jumps, and a button never fires with
  its bar part-way. *(2026-11-25, SimonSays Debug Win32, cursor dwell
  stored, nobody in front of the tracker, same targets as AC-61.
  `check_onbutton2.ps1`: reference 1031 ms; moving within the button
  1031 ms; 100 ms off and back 1110 ms (paused, ~1131); 300 ms off
  cancelled, 1703 ms; LeaveGrace 400, 300 ms off and back 1297 ms (paused,
  ~1331); alternating at LeaveGrace 500 the first fired after 1907 ms
  (each paused while on the other), the second did not fire in 400 ms.
  `check_onbutton3.ps1`, LeaveGrace 0, three runs: 35 px, 20 px above and
  back 1047/1015/1032 ms; staying 20 px above fired there after
  1047/1015/1046 ms; 60 px above cancelled, 1719/1719/1734 ms; 80 px,
  60 px above and back 1297 ms in the first run (FAIL), then 1031 and
  1015/1031 ms on both choices; 19 px, on the other choice cancelled,
  1719/1750/1718 ms. The 1297 ms (the first dwell after start, about the
  reference plus the 300 ms away, as if paused without a point) did not
  recur in four more tries; unexplained. By gaze: the bar moves on
  smoothly, no neighbour fires (developer, 2026-11-25, Pass).)*
Build gate: Debug **and** Release Win32 compile clean (only the pre-existing
`CategoryWindow.cpp` / `utils.cpp` C4244 warnings; the `C4267` narrowing
warnings appear on x64 only, where `size_t` is 64-bit), **and**
`SimonSaysGaze64` Debug and Release x64 compile clean (built by the solution's
Win32 configurations, or directly with `/p:Platform=x64`). *2026-10-22: all
four clean; the x64 app build also clean.*

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
| Detector + Auto + hysteresis | ✅ Done | Driving timer was dead until 2026-09-21 (REQ-F24 note); direct-gaze branch re-verified on a 4C that day (AC-17), no-presence branch AC-3 Pass 2026-10-26, presence-only branch AC-13 Pass 2026-11-04 (Irisbond mouse mode). Hysteresis hardcoded 3; classifier dormant since v1.6 (direct gaze → HidDwell unconditionally) |
| Auto no-presence gate (REQ-F27) | ✅ Done | Never executed before 2026-09-21 (REQ-F24 note); AC-3 Pass 2026-10-26 on a machine with no tracker or tool |
| Auto presence → MouseDwell (REQ-F28) | ✅ Done | Since 2026-10-23 only cursor tools / HID tracker / WEC count; Tobii vendor services alone → Off. Scripted with stand-in processes (AC-37 Pass: services → Off, OptiKey → MouseDwell, TD Control → ExternalClick); real Irisbond Hiru in mouse mode AC-13 Pass 2026-11-04 |
| Auto silent tracker → Off (REQ-F29/F94) | ✅ Done | 2026-10-18: Stream Engine "open but no callbacks" = tracker off (4C probe); Auto HidDwell no longer falls back to the cursor (§8.3); AC-20 Pass on the 4C (2026-10-18) |
| Cursor sampler retired (REQ-F24) | ✅ Done | Removed in v1.6; AC-11 withdrawn (intent satisfied by construction) |
| Dwell window Reset button (REQ-F67) | ✅ Done | AC-12 Pass 2026-11-02 |
| 10 pt two-column dwell window (REQ-N09) | ✅ Done | 2026-10-21; AC-22 verified manually |
| HID option for Tobii + gaze-only HID dwell (REQ-F63, §8.3) | ✅ Done | 2026-10-25: option enabled for Stream Engine trackers and while selected; saved HID choice kept; forced HID no longer falls back to the cursor. AC-44–AC-46 Pass |
| Lock-on delay (REQ-F104) | ✅ Done | 2026-10-24: setting, slider, registry, 18 languages; AC-41 timing, AC-42 and AC-43 Pass (gaze scanning confirmed 2026-11-04) |
| Gaze hover highlight (REQ-F103) | ✅ Done | 2026-10-24: hover color on the button under the gaze in HidDwell; AC-39/AC-40 Pass |
| Dwell anywhere on the button (REQ-F12, REQ-F15) | ✅ Done | 2026-11-24: no tolerance while dwelling; off the button for up to LeaveGrace (150 ms, registry only) keeps the dwell; a fire ends every other dwell (REQ-F111); gaze and cursor dwell. From SimonSays Gaze's SPK-12. AC-59 and AC-60 Pass in cursor dwell (scripted), AC-60 by gaze too (developer, 2026-11-25). 2026-11-25: the tolerance radius is also a margin around the button (REQ-F15): AC-61 Pass in cursor dwell (scripted); by gaze the bar stalled and fired part-way, so the margin counts fully and time away pauses the clock: AC-62 Pass in cursor dwell (scripted) and by gaze (developer). Default tolerance radius 25 px since 2026-11-25 |
| Move away to re-arm (REQ-F95) | ✅ Done | 2026-10-19: dwell-only, process-wide block tied to the activation point; on by default, dwell window checkbox; AC-21 Pass |
| Dwell window for other hosts (REQ-F109) | ⚠️ Partial | 2026-11-17: host-neutral entry point, `SSDialogUtils`, `SSDwellDialog.rc`; SimonSays builds Debug and Release Win32 and Release x64. SimonSays' own window behaves as before (AC-55 Pass, developer, 2026-11-18); no other host yet (AC-56) |
| Per-button repeat, second stage and dwell time (REQ-F106 to REQ-F108) | ⚠️ Done* | 2026-11-16: opt-in per button, default unchanged; AC-52 to AC-54 Pass in cursor dwell on a throwaway harness; SimonSays builds Debug and Release Win32 clean and its buttons dwell as before (developer). Not run in gaze dwell; no SimonSays button uses them (added for a second host) |
| Diagnostics dump extension + switch (REQ-F80) | ✅ Done | AC-14 Pass — PCEye5 + 4C dumps delivered 2026-07-04/05; v3 adds per-device registry identity + failure codes + GENERIC_READ retry (AC-16 Pass) |
| Tobii virtual "Eye Tracker HID" device readable? | ⛔ Blocked | AC-16 verdict 2026-07-05: device exists on both machines (`HID\VID_2104&UP:0012_U:0001`) but all user-mode opens fail with ERROR_ACCESS_DENIED — reserved for the Windows gaze platform. Direct HID dwell on Tobii needs the vendor SDK or the Windows gaze API (§18) |
| Detection of Tobii PCEye5 / 4C + robust WEC signal | ✅ Done | `tdx.*` + `platform_runtime_is*` table entries; WEC via `microsoft.ecapp.exe` OR blob marker (3 machines). Live panel re-test AC-15 Pass; ET5 presence confirmed by the 2026-10-22 dump (`platform_runtime_is5leyetracker5_service.exe`, `tobii*` processes; "Tobii eye tracker service") |
| HID reader (`SSGazeReader`) | ✅ Done | Non-exclusive, self-healing, clean shutdown |
| HID gaze parse + µm→px mapping | ⚠️ Done\* | Confirmed Irisbond Hiru only |
| Calibration → `ReportCalibration` | ✅ Done | |
| Hot-plug (`WM_DEVICECHANGE`) | ✅ Done | |
| Diagnostics dump | ✅ Done | Full environment report |
| Application Verifier pass | ⚠️ Attempted (inconclusive) | Elevated pass ran + machine left clean, but app didn't reach steady state under automated launch; re-run by launching normally (§13.1 / AC-10) |
| Tobii Stream Engine provider (REQ-F90–F93) | ✅ Done | **Field-verified 2026-07-07 on 4C + PCEye5**: Auto direct-gaze dwell + vendor coexistence with TD Control / WEC / no software (AC-17 Pass) |
| 64-bit gaze helper (REQ-F96–F100) | ✅ Done | **Field-verified 2026-10-22 on the Eye Tracker 5**: helper route, gaze streaming + LOOK probe (AC-23), tracking off/on (AC-24), lifecycle (AC-25), Tobii Experience coexistence (AC-26). 4C still in-process with no helper (AC-27, 2026-10-23); AC-20 on the 4C and a no-Tobii machine not re-run. **2026-11-14:** the same project also builds the 32-bit helper for an x64 app (REQ-F96/F97 amended); AC-50 Pass (core part, 2026-11-15, 4C); AC-51 shown for the solution route only; AC-23–AC-26 not re-run after the change (no Eye Tracker 5 at hand) |
| Stream Engine dump section (REQ-F101) | ✅ Done | Status only — no sensor-derived values, no device URL (AC-29 Pass) |
| Diagnostic report on demand (REQ-F80 amended, REQ-F102) | ✅ Done | 2026-10-22: no dump at startup unless `SIMONSAYS_HID_DUMP`; dwell-window button writes it on a background thread and opens Explorer (AC-30–AC-34 Pass); same day, RTL probe placement fixed (AC-22 correction) |
| Stream Engine licensing basis (§18, REQ-N08) | ✅ Done | v1.8: Interactive Use under the user's own end-user licence; no storage or transfer (REQ-N08 / AC-19), no redistribution, no build-time dependency (REQ-N02), vendor stack unaffected (AC-17) |
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
  only mouse-cursor-driving trackers (Tobii dropped). SimonSays reads the ET5
  directly through the Stream Engine — since 2026-10-22 through the 64-bit
  helper, because current Tobii software installs only a 64-bit engine
  (REQ-F96–F100; verified on the ET5 2026-10-22, AC-23–AC-26). Other apps still need a third-party gaze-mouse
  tool (e.g. Talon, Precision Gaze Mouse) to drive the cursor.
- **Tobii EULA v3.0 §5 bars the 4C and Eye Tracker 5 from Medical Use**,
  including use in hospitals, surgeries and other healthcare environments. This
  binds the user's own licence, not SimonSays' code (§1.3 extends only to §1),
  but many SimonSays users have ALS and some will be in those settings, so the
  in-app Help states it in every language next to the verified-tracker list
  (2026-10-22; the non-Latin-script translations await native review). Tobii
  Dynavox devices (PCEye5) ship under their own terms.
- **Classifier thresholds** are estimates; Auto's kinematic branch needs tuning with
  recorded per-device data.
- **Windows Eye Control** detection relies on an undocumented CloudStore byte marker
  (§Appendix B) verified on one machine; could change across Windows builds.
- **HID gaze** maps to the **primary** monitor only (the Stream Engine
  provider shares this: normalized coordinates map to the primary monitor).
- ~~**The dwell window's HID radio stays gated on physical HID presence**~~
  *Resolved 2026-10-25 (REQ-F63 amended):* the radio is also enabled for a
  Tobii tracker opened through the Stream Engine, and while it is selected.
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
  means an eye-tracking user is far more likely than not. **Narrowed
  2026-10-18 (REQ-F29):** where the Stream Engine opens the device (Tobii 4C /
  PCEye5 / ET5), a switched-off tracker now resolves `Off`, so the trade-off
  remains only for trackers that cannot be read directly (Irisbond in mouse
  mode, OptiKey, WEC on other hardware). **Narrowed again 2026-10-23
  (REQ-F28/F31):** Tobii vendor services (Tobii Service / Experience / Core,
  platform runtime) no longer count — they run all the time once installed, so
  every Tobii owner got cursor dwell with the tracker unplugged or unused. Only
  cursor tools, a HID tracker or WEC enable it now (Dynavox `tdx.*` /
  `tobiidynavox*` still do — see Appendix C).
- **The PCEye5 is Tobii Dynavox hardware.** The Tobii Eye Tracking end-user
  licence referenced in §18 is Tobii Tech AB's, covering the 4C, the Eye
  Tracker 5, and the Tobii Experience / Core / Service stack. Dynavox devices
  ship under Dynavox's own terms. AC-17 was verified on both.
- **The Stream Engine DLL must match the process architecture.** `LoadLibrary`
  skips a copy built for a different architecture, so the default 32-bit build
  is what pairs with the 32-bit `tobii_stream_engine.dll` the Tobii Core / EyeX
  stack installs on the 4C / PCEye5 machines. An x64 build cannot load those
  copies — seen 2026-09-05, where all four copies present on the test machine
  failed with `ERROR_BAD_EXE_FORMAT` (193). The opposite holds on the Eye
  Tracker 5 machine (2026-10-22): its only engine is 64-bit, which the 32-bit
  build reaches through the helper (REQ-F96). An x64 SimonSays build on a
  32-bit-engine machine uses the 32-bit helper (since 2026-11-14; AC-50,
  checked on a 4C 2026-11-15). With both
  stacks installed (seen 2026-10-23: Tobii Experience 64-bit 4.25 plus Tobii
  Core / EyeX 32-bit 2.2.3) the 32-bit app loads the 32-bit engine in-process
  and the helper stays unused — verified with the 4C plugged in. Whether the
  ET5 also streams through the 2.2.3 engine is untested.
- **Plugging in an Eye Tracker 5 replaces the 4C's software.** On the test PC
  (2026-10-23) connecting the ET5 installed Tobii Experience automatically,
  without asking, and disabled the Tobii Core stack; the 4C only worked again
  after Tobii Core was reinstalled. This is vendor behaviour SimonSays cannot
  prevent. Symptom in the REQ-F101 dump: no usable 32-bit engine, so on a
  32-bit build the route is the 64-bit helper (or none, with the 4C, which
  the 4.25 engine may not serve). Remedy for 4C users: reinstall Tobii Core —
  the recommended version for the 4C is
  `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe`, from
  <https://gaming.tobii.com/getstarted/> (developer, 2026-10-25). The English
  Help says so under *Supported setups* since 2026-10-26 (other languages to
  follow).
- **The helper must be built with the app.** It is a separate project; a
  build of `SimonSays.vcxproj` alone, or a Visual Studio session still holding
  the solution as it was before the project was added, used to leave
  `SimonSaysGaze64.exe` out of `Release\` (seen 2026-10-23). *Resolved
  2026-10-25 by REQ-N10 / REQ-N11*; an open Visual Studio must still reload
  the solution once to pick up the new dependencies.
- **The Stream Engine field-of-use value is unverified.**
  `SSStreamEngine.cpp` passes `1` on engine ≥ 4. No official Tobii
  documentation of the enum's numeric values has been found (2026-10-22 search:
  third-party mentions only). Resolve it from Tobii's own documentation, not by
  probing the DLL (EULA v3.0 §1.4); REQ-N08 fixes what the code does
  regardless of the declared value.
- **Interface declarations come from public documentation only.** The
  Stream Engine prototypes and structs in `SSStreamEngine.cpp` are declared
  from Tobii's publicly documented API. SimonSays never disassembles,
  decompiles or probes the DLL to learn or confirm its design (EULA v3.0
  §1.4); field tests check SimonSays' behaviour (does gaze arrive, do buttons
  dwell), not Tobii's internals. *(2026-10-22: the earlier "layout
  verification" comment, which described a 2026-09-05 probe as checking the
  struct layouts, was reworded on that basis.)*

---

## 18. Future work / out of scope

- **Tobii Stream Engine provider — IMPLEMENTED 2026-07-06** as `SSTobiiGaze` +
  `TobiiGazeProvider` (REQ-F90–F93) after `tobii_stream_engine.dll` was found
  installed on both test machines (including inside TD Computer Control on the
  PCEye5 machine). Field verification pending (AC-17). Background from the
  research below. Tobii's
  *current* offering (Tobii Streams SDK) targets the 5L/Nexus under a paid
  development licence and is not the route taken here. The provider instead
  uses the **legacy Stream Engine C API** (`tobii_stream_engine.dll`) as it is
  already present on the user's machine: the runtime ships with the user's own
  installed Tobii software (Tobii Service was running on both test machines,
  PCEye5 included), so the provider `LoadLibrary`s that copy at runtime.

  **Licensing basis** (re-checked 2026-10-22 against the *Tobii Technologies
  End-User License Agreement, Version 3.0*, Tobii Tech AB, which names the
  Eye Tracker 4C and Eye Tracker 5, Tobii Experience, Tobii Core and Tobii
  Service). SimonSays does not download, accept, or redistribute any Tobii
  SDK, and has no build-time dependency on one (REQ-N02). The Tobii software
  on the machine is the user's own installation under that licence, whose
  §1.1 grants Interactive Use — Data as user input for interactive
  experiences in games or other software, excluding the right to store or
  transfer Data to another computing device or network. Dwell-click is exactly
  that, and the design holds to the limits of the agreement:

  - **§1.1** — gaze is consumed as live input and never stored or transferred,
    pinned by **REQ-N08** and checked by **AC-19** / **AC-28**. The 64-bit
    helper passes it between two processes on the same computer, in memory
    (REQ-F98) — neither storage nor a transfer to another device. Diagnostic
    dumps, which are sent to the developer, carry no sensor-derived value at
    all (REQ-F101).
  - **§1.2** — nothing is re-sold, commercialised, or bundled: SimonSays is
    free and open source under OSL 3.0, and ships no Tobii component (the
    installer adds only SimonSays' own helper executable; the engine DLL is
    always the user's installed copy).
  - **§1.3** (no software that causes a third party to contravene §1) — the
    helper's record lives in an unnamed section reachable only through a handle
    inherited by the helper, so no other program can read the gaze and store
    or forward it.
  - **§1.4** (no reverse engineering or other steps to derive design
    information) — interface declarations come from public documentation; the
    DLL is never disassembled, decompiled or probed to learn or confirm its
    design; tests check SimonSays' behaviour only (see §17).
  - **§5** (no High Risk or Medical Use) — binds the user's licence; the
    in-app Help states it next to the verified-tracker list (see §17).
  - the vendor's own software keeps working throughout (REQ-N04, REQ-F93,
    REQ-F100, AC-17) — nothing is taken exclusively and no service is
    disabled.

  SimonSays is not a medical device and is not certified for use in medically
  classified devices or environments; this is stated in the in-app Help.
  *This is an engineering reading of the agreement, not legal advice.*

  Proven in the wild by current tools (Talon, Precision Gaze Mouse) incl. on
  the Eye Tracker 5. Risks: SDK deprecated/unsupported (NuGet unlisted, last release
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
- Add the medical-use statement to the Help Gaze/Dwell section in all
  languages (SimonSays is not a medical device and is not certified for
  clinical environments), and re-run `scripts/build_help_html.ps1`.

---

## 19. Open questions

1. **HID coordinate origin across devices** — is top-left-of-primary universal, or
   do some devices use center/normalized? (Hiru = top-left µm.)
2. **Auto vs. explicit** — should first run default `Auto` (current) or prompt the
   user to run calibration once?
3. ~~**Re-fire policy**~~ — *Resolved 2026-10-19 (REQ-F95):* by default a held
   pointer or gaze fires once; move away (or look away) to repeat. The old
   behavior (re-fire every `DwellTime+Cooldown`) remains available by unticking
   the dwell window checkbox.
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

- Build: `MSBuild SimonSays.vcxproj /p:Configuration={Debug|Release} /p:Platform=Win32`
  (Win32 is the default platform; `x64` remains supported). For Win32 this
  also builds the x64 helper `SSGazeHelper64.exe` into the same folder
  (REQ-N10); it can be built alone with
  `MSBuild SimonSaysGaze64\SimonSaysGaze64.vcxproj /p:Configuration={Debug|Release} /p:Platform=x64`.
  An x64 build likewise builds the 32-bit helper `SSGazeHelper32.exe` into
  `x64\{Debug|Release}\` (the same project with `/p:Platform=Win32`).
- Installer: build `SimonSaysInstaller` in Visual Studio, or
  `devenv SimonSays.sln /Build "Release|Win32" /Project SimonSaysInstaller`
  (MSBuild cannot build `.vdproj`). It builds the app and the helper first,
  checks both before packaging, fixes the non-Latin file names, and verifies
  the MSI holds both exes in the same folder (REQ-N11). The code-page
  `TargetName` warnings and the x64-in-x86 helper warning are expected.
- Verify features against the **`Release\SimonSays.exe`** build (not the
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

| Process (match) | Display name | Clicks? | Role (REQ-F31) | Match |
|---|---|---|---|---|
| `easyclick.exe` | Irisbond EasyClick | no | cursor tool | exact |
| `hirusystray.exe`, `hiru.exe` | Irisbond Hiru | no | cursor tool | exact |
| `tdcontrol.exe` | Tobii Dynavox TD Control | yes (dwell) | cursor tool | exact |
| `optikey.exe`, `optikeypro.exe` | OptiKey | no | cursor tool | exact |
| `tobiidynavox*` | Tobii Dynavox | no | cursor tool (EyeAssist mouse emulation) | prefix, before `tobii*` |
| `tobii*` | Tobii Eye Tracking | no | vendor service | prefix |
| `tdx.*` | Tobii Dynavox | no (unconfirmed — see §19) | cursor tool (Computer Control) | prefix |
| `platform_runtime_is*` | Tobii eye tracker service | no | vendor service | prefix |

*Role column added 2026-10-23: only cursor tools (plus a HID tracker or
Windows Eye Control) can turn on `MouseDwell` in Auto (REQ-F28). Trade-off kept
on purpose: `tdx.*` also matches always-running Dynavox updater / switcher
processes, so on a PCEye5 machine whose engine cannot open the device Auto
still resolves `MouseDwell`.*

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
