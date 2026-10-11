# SimonSays — Taskbar Integration — Specification

| | |
|---|---|
| **Spec ID** | TBR-SPEC |
| **Status** | Active — created 2026-10-06. TBR-F01–F04 reverse-engineered from shipping source; TBR-F10–F13 (taskbar kept shown while running, show-once Widgets hint) added the same day; AC-3–AC-7 verified manually 2026-10-07. Phase 1 of the taskbar-geometry work (any edge, Regular/Thin tiers, vertical stack, dynamic width, edge-aware category window, live relayout — TBR-F20–F23, F30, F40, N03; TBR-F01/F02 amended) added 2026-10-11; its ACs verified manually 2026-10-11 |
| **Version** | 1.1 (2026-10-11) |
| **REQ prefix** | `TBR-F##` (functional), `TBR-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/MainWindow.cpp`](../../src/MainWindow.cpp) (`Create`, `ComputeBarRect`, `UpdateTaskbarControlsPosition`, `UpdateTaskbarFonts`, `UpdateCategoryButtonText`, `RelayoutIfTaskbarChanged`, `EnsureTaskbarShown`, `RestoreTaskbarAutoHide`, `ShowWidgetsHintOnce`, Z-order timers, `LowLevelMouseProc`), [`src/CategoryWindow.cpp`](../../src/CategoryWindow.cpp) (`PositionNextToBar`, `PlaceNextToBar`, `AutoResizeWindow`), taskbar helpers in [`src/utils.cpp`](../../src/utils.cpp) (`GetTaskbarGeometry`, `TaskbarWindowsChanged`), `\LastRun` flags in [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp) |
| **Master spec** | [`docs/spec.md`](../spec.md) |

---

## Contents
- [0. How to use this spec](#0-how-to-use-this-spec)
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
- [12. Configuration & tuning constants](#12-configuration--tuning-constants)
- [13. Diagnostics](#13-diagnostics)
- [14. Edge cases & error handling](#14-edge-cases--error-handling)
- [15. Acceptance criteria](#15-acceptance-criteria)
- [16. Implementation status matrix](#16-implementation-status-matrix)
- [17. Known limitations](#17-known-limitations)
- [18. Future work](#18-future-work)
- [19. Open questions](#19-open-questions)
- [20. Build & run](#20-build--run)

---

## 0. How to use this spec

Conventions (EARS, IDs, status tags, AC format, section order) follow
[`docs/spec.md`](../spec.md) §2. This spec owns how the main window (the
SimonSays bar: input box, Play, Categories and quick access buttons) sits on the
Windows taskbar, how its controls are laid out for the taskbar's edge and size,
where the category window opens relative to it, and the Windows taskbar settings
SimonSays reads or changes. What the controls do is owned by
[`settings.spec.md`](settings.spec.md) (SET-F50/F60) and
[`tts.spec.md`](tts.spec.md); the category window's own sizing by
[`categories-phrases.spec.md`](categories-phrases.spec.md) (CAT-F41/F43);
`\LastRun` storage by [`persistence.spec.md`](persistence.spec.md).

## 1. Overview

SimonSays draws its bar as a topmost popup window laid over the taskbar. To stay
usable it must (a) find a free spot on the taskbar, whichever screen edge the
taskbar is on and however thick it is, (b) stay above the taskbar when Windows
reorders windows, (c) follow the taskbar when it moves or resizes, and (d) keep
the taskbar itself on screen. It never writes taskbar settings it cannot cleanly
restore.

## 2. Background & context

- The bar's position was only ever described in `HELP.md` (bottom taskbar
  only). This spec was created 2026-10-06 when the taskbar auto-hide and
  Widgets handling were added.
- **Any edge (2026-10-11):** current Windows builds let the taskbar sit at the
  bottom, top, left or right, and offer smaller taskbar buttons (a thinner
  taskbar). Until 2026-10-11 SimonSays refused to start unless the taskbar was
  at the bottom, and its bar had a fixed 400 × 46 px size placed from the
  desktop rectangle, so on a thinner bottom taskbar it stuck out above it.
- **Auto-hide:** when the taskbar auto-hides, the bar floats over other
  windows' content at the screen edge. Auto-hide is a documented app-bar state
  (`SHAppBarMessage` `ABM_GETSTATE`/`ABM_SETSTATE`), saved by Windows itself
  across reboots.
- **Widgets (Windows 11):** the Widgets button sits at the left end of a
  centered taskbar (where the bar goes when the Start button isn't at the start)
  and next to the notification area on a left-aligned one (where the bar goes
  otherwise), so it can collide with the bar either way. Its only switch is the
  undocumented `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced\TaskbarDa`
  value, which current Windows 11 builds protect against writes from ordinary
  apps. SimonSays therefore only reads it.
- **DPI:** the process declares no DPI awareness, so all coordinates here are
  96-dpi logical pixels and Windows bitmap-scales the windows (see §17).

## 3. Goals & non-goals

**Goals:** keep the bar on a visible taskbar for the whole session, on any
screen edge and at any taskbar thickness; follow the taskbar when it is moved or
resized; open the category window next to the taskbar; leave the user's taskbar
settings exactly as they were once SimonSays exits — including after a crash;
never write undocumented or protected settings.

**Non-goals:** multi-monitor placement (the taskbar on the primary monitor is
used); measuring the taskbar's app icons (UI Automation — rejected 2026-10-11,
the Start button and notification area positions are enough, §8.2); reserving
screen space for the bar (app-bar registration — §18); changing the Widgets
setting.

## 4. Glossary

| Term | Meaning |
|---|---|
| **Bar** | The SimonSays main window laid over the taskbar. |
| **Edge** | The screen edge the taskbar is on (`ABE_BOTTOM`/`TOP`/`LEFT`/`RIGHT`). |
| **Thickness** | The taskbar's height on a horizontal edge, its width on a vertical one. |
| **Regular / Thin tier** | Layouts of the horizontal bar for a taskbar at least / under `MW_THIN_TASKBAR_THICKNESS` thick (TBR-F21). |
| **Start near** | The Start button lies within `MW_START_NEAR_EDGE` px of the taskbar's start (left end, or top end on a side taskbar): left/top-aligned taskbar icons. Otherwise the icons are centered. |
| **Room** | The space along the taskbar the bar may use (TBR-F20). |
| **Auto-hide** | Windows' "Automatically hide the taskbar" (`ABS_AUTOHIDE`). |
| **Restore-pending flag** | `\LastRun` value `Taskbar AutoHide Restore Pending` = `"1"`: SimonSays turned auto-hide off and still owes the restore. |
| **Widgets** | The Windows 11 taskbar Widgets button (`TaskbarDa`). |

## 5. Personas & scenarios

- **User with auto-hide on** (a caregiver set it for screen space): starting
  SimonSays shows the taskbar so the bar is usable; quitting brings auto-hide
  back without anyone touching Settings.
- **PC that loses power mid-session:** the next SimonSays session ends with
  auto-hide restored, even though the first session never exited.
- **Windows 11 with Widgets on:** a one-time message explains the overlap and
  can open taskbar settings; it never nags again.
- **User with the taskbar on the left** (more vertical room on a wide screen):
  the bar is a narrow stack on the taskbar and the category window opens to its
  right.
- **User who switches to smaller taskbar buttons mid-session:** within a few
  seconds the bar becomes the Thin layout without a restart.

## 6. Requirements (EARS)

### 6.1 Placement & Z-order

- **TBR-F01 [Done]** THE SYSTEM SHALL support the taskbar on any edge —
  bottom, top, left or right — read with `ABM_GETTASKBARPOS`
  (`GetTaskbarGeometry`). IF the taskbar cannot be found or read THEN THE
  SYSTEM SHALL lay the bar out as for a 48 px bottom taskbar on the primary
  monitor and still start. *(Amended 2026-10-11: SimonSays used to show
  `ERROR_TASKBAR_POSITION_ID` and refuse to start on a non-bottom taskbar; the
  string has been removed from every table.)*
- **TBR-F02 [Done]** THE SYSTEM SHALL create the bar as a topmost layered popup
  placed from the taskbar's own rectangle (`ComputeBarRect`), centered across the
  taskbar's thickness: WHEN the Start button is near the start of the taskbar
  (`MW_START_NEAR_EDGE`, measured along the taskbar) THE bar SHALL end
  `TRAY_MARGIN` before the notification area (or `FALLBACK_TRAY_WIDTH` +
  `TRAY_MARGIN` before the taskbar's end when the area is not found); OTHERWISE
  (centered icons, or no Start button found) THE bar SHALL start at the
  taskbar's start (left end, or top end on a side taskbar). *(Amended
  2026-10-11: generalized from bottom-only x placement to all four edges; the
  vertical case follows the same rule turned 90°.)*
- **TBR-F03 [Done]** THE SYSTEM SHALL re-assert the bar's topmost Z-order every
  `SLOW_TIMER_CHECK_ZORDER_INTERVAL` ms, and immediately WHEN a low-level mouse
  hook sees a click on the taskbar; WHILE the taskbar covers the bar THE SYSTEM
  SHALL check every `FAST_TIMER_CHECK_ZORDER_INTERVAL` ms until it is on top
  again, including the Start-menu close workaround
  (`SimulateNonActivatingClickOnWindow`). The probe point is the middle of the
  `Categories` button, recomputed on every layout.
- **TBR-F04 [Done]** WHEN the bar is destroyed THE SYSTEM SHALL stop the Z-order
  timers and remove the mouse hook.

### 6.2 Keeping the taskbar shown

- **TBR-F10 [Done]** WHEN SimonSays starts AND the taskbar auto-hides, THE
  SYSTEM SHALL first set the restore-pending flag and THEN turn auto-hide off
  (`ABM_SETSTATE`), silently — no setting and no prompt. IF the flag cannot be
  written THEN THE SYSTEM SHALL leave auto-hide unchanged.
- **TBR-F11 [Done]** WHEN SimonSays exits — window destroyed, `MainWindow`
  destroyed (also after a failed start), or `WM_ENDSESSION` at log-off /
  shutdown — AND the restore-pending flag is set, THE SYSTEM SHALL turn auto-hide
  back on and, only once the taskbar reports auto-hide on again, clear the flag.
  The restore is idempotent (the first caller clears the flag).
- **TBR-F12 [Done]** IF the restore-pending flag is already set at startup (the
  previous session ended without restoring), THEN THE SYSTEM SHALL keep it — it
  is never cleared or overwritten except by a successful restore — so the next
  exit restores the user's original choice.

### 6.3 Widgets

- **TBR-F13 [Done]** WHEN SimonSays starts on Windows 11 (build ≥ 22000) AND
  Widgets is shown (`TaskbarDa` ≠ 0, missing = shown) AND the `\LastRun` value
  `Widgets Hint Shown` is not `"1"`, THE SYSTEM SHALL set that value and show a
  localized Yes/No message (`TASKBAR_WIDGETS_HINT_*`); on Yes it SHALL open
  `ms-settings:taskbar`. THE SYSTEM SHALL NOT write `TaskbarDa`. The message is
  shown once per user, whatever the answer.

### 6.4 Bar size and layout

- **TBR-F20 [Done]** ON a horizontal taskbar THE bar SHALL be as wide as the
  room allows, between a minimum (the fixed controls + `MW_MIN_EDIT_WIDTH`) and
  the preferred `MW_DEFAULT_WINDOW_WIDTH`; THE input box SHALL take the width
  left over by the fixed controls. Room: WHEN the Start button is near, from
  `TRAY_MARGIN` past the Start button's end to the bar's end (TBR-F02);
  OTHERWISE from the taskbar's start to `TRAY_MARGIN` before the Start button
  (the start of the centered icon group); with no Start button found, the
  preferred width.
- **TBR-F21 [Done]** ON a horizontal taskbar THE bar SHALL be the taskbar's
  thickness − 2 tall and use one of two tiers:
  - **Regular** (thickness ≥ `MW_THIN_TASKBAR_THICKNESS`): buttons and input box
    thickness − 12 tall (36 px on a 48 px taskbar), the menu font, the two quick
    access buttons stacked (SET-F50).
  - **Thin** (thinner): buttons and input box thickness − 8 tall, the menu font
    at 11/12 size, the two quick access buttons side by side.
- **TBR-F22 [Done]** ON a vertical taskbar THE bar SHALL be the taskbar's
  thickness − 4 wide and stack, top to bottom with `MW_VERTICAL_MARGIN` gaps,
  the `Categories` button, the quick access buttons side by side (when shown),
  a two-line input box (`MW_VERTICAL_EDIT_HEIGHT`, text from the top, wrapping)
  and `Play`, using the 11/12 menu font. THE `Categories` button SHALL show
  its TBR-F23 arrow and ≡ (so it is not mistaken for Play's triangle) — arrow
  first on a left taskbar, ≡ first on a right one —
  and `Play` a ▶ symbol (■ while speaking), drawn as
  plain text characters in a symbol font about 1.6× the menu font
  (`m_hSymbolFont`), in every language; the localized labels stay as the
  buttons' window text and accessible names (BTN-F47). *(Amended 2026-10-11:
  the labels did not fit a ~34 px button — `Play>` was clipped — and the 🗂
  fallback for `Categories` drew mojibake because it was written as a raw
  character in a BOM-less source file. Emoji-drawn symbols were tried next and
  rejected by the developer: text glyphs look better and switch faster. The
  symbols are Unicode escapes or come from `stdafx.h`, which has a BOM. The ≡
  prefix was added at the developer's request the same day.)*
- **TBR-F23 [Done]** THE `Categories` button's arrow SHALL point toward the
  category window while it is hidden and back toward the taskbar while it is
  shown: ▲/▼ on a bottom taskbar, ▼/▲ on a top one, ▶/◀ on a left one, ◀/▶ on a
  right one. The arrow is kept when the UI language changes. *(Fix 2026-10-11:
  a language change used to drop the arrow from the label.)*

### 6.5 Category window placement

- **TBR-F30 [Done]** THE category window SHALL open next to the taskbar, on the
  side away from its edge, and inside the work area of the taskbar's monitor:
  above a bottom taskbar and below a top one (left edge 4 px left of the bar's),
  right of a left taskbar and left of a right one (top edge level with the
  bar's). Its default width is `m_default_window_width`; a remembered size
  larger than the work area is reduced to fit. WHEN the bar's position changes
  at runtime — the taskbar's edge, rectangle or alignment, or the room —
  THE SYSTEM SHALL move the window along, keeping its size (also when hidden or
  minimized), UNLESS the user dragged it this session (moved without a size
  change, seen at `WM_EXITSIZEMOVE`); a change of taskbar edge SHALL re-place
  it even then. A change of the bar's width alone does not move it. Auto-fit
  keeps the edge facing the taskbar in place (CAT-F43). *(Amended 2026-10-11:
  the window only followed edge/rectangle changes, so switching the taskbar
  between centered and left-aligned moved the bar but left the window behind.)*

### 6.6 Following the taskbar

- **TBR-F40 [Done]** THE SYSTEM SHALL lay the bar out again WHEN the taskbar's
  edge, rectangle, Start button, notification area or work area changes —
  checked on every `SLOW_TIMER_CHECK_ZORDER` tick (and on the taskbar clicks
  that trigger it), on `WM_SETTINGCHANGE` and `WM_DISPLAYCHANGE`, and on
  `TaskbarCreated` (Explorer restarted, which also refreshes the mouse hook's
  taskbar handle). Controls SHALL be moved, never recreated, so the typed text
  and focus survive. A change of the quick access setting re-lays the bar out
  too (its minimum width changes).

### 6.7 Non-functional

- **TBR-N01 [Done]** Taskbar settings SHALL be changed only through documented
  APIs, and only when the original value is recorded first (TBR-F10).
- **TBR-N02 [Done]** The auto-hide and Widgets checks SHALL add no polling:
  they are read once at startup; restore runs once at exit.
- **TBR-N03 [Done]** Following the taskbar (TBR-F40) SHALL add no timer, thread
  or dependency. The periodic check compares window rectangles only
  (`TaskbarWindowsChanged`, which sends Explorer no message) and calls
  `ABM_GETTASKBARPOS` only once one of them has changed, so a hung Explorer
  cannot stall the bar every 5 s.

## 7. Architecture & components

| Piece | Where | Role |
|---|---|---|
| `TaskbarGeometry`, `GetTaskbarGeometry`, `SameTaskbarGeometry`, `TaskbarWindowsChanged` | `src/utils.cpp` | edge, rects (taskbar, Start, notification area, work area) and cheap change detection (TBR-F01/F02/F40/N03) |
| `IsTaskbarAutoHide`, `SetTaskbarAutoHide` | `src/utils.cpp` | app-bar state read/write; `Set` verifies by reading back |
| `IsTaskbarWidgetsShown` | `src/utils.cpp` | read-only Windows 11 build + `TaskbarDa` check |
| `MainWindow::ComputeBarRect` / `UpdateTaskbarControlsPosition` / `UpdateTaskbarFonts` / `UpdateCategoryButtonText` / `RelayoutIfTaskbarChanged` | `src/MainWindow.cpp` | TBR-F02, F20–F23, F40 |
| `MainWindow::EnsureTaskbarShown` / `RestoreTaskbarAutoHide` / `ShowWidgetsHintOnce` | `src/MainWindow.cpp` | TBR-F10–F13 |
| `CategoryWindow::PositionNextToBar` / `PlaceNextToBar`, `AutoResizeWindow` anchor | `src/CategoryWindow.cpp` | TBR-F30, CAT-F43 |
| `RegistryManager::Get/SetTaskbarAutoHideRestorePending`, `GetWidgetsHintShown`/`SetWidgetsHintShown` | `src/RegistryManager.cpp` | `\LastRun` flags |
| Z-order timers + `LowLevelMouseProc` | `src/MainWindow.cpp` | TBR-F03/F04 |

Layering: utils helpers know nothing about the registry flags or the bar;
`MainWindow` owns the geometry (`GetTaskbar()`), and `CategoryWindow` reads it
from there.

## 8. Detailed design

### 8.1 Startup and relayout

Startup order in `MainWindow::Create`: `EnsureTaskbarShown()` runs first (before
placement, so the geometry is read from a shown taskbar), then
`GetTaskbarGeometry` + `ComputeBarRect` (TBR-F01/F02), window creation, controls
created at placeholder positions and laid out by `UpdateTaskbarControlsPosition`,
the category window placed by `PositionNextToBar`, … , help, shipped-board sync
(import-export PORT-F41), and `ShowWidgetsHintOnce()` last (needs the bar as
owner).

`RelayoutIfTaskbarChanged( force )`: unless forced, return when
`TaskbarWindowsChanged` reports nothing; re-read the geometry, return when it is
the same; otherwise store it, move/resize the bar, lay the controls out (fonts
recreated only when the tier changes), and — when the edge or the taskbar
rectangle changed or the bar moved — move the category window
(`PlaceNextToBar`, forced when the edge changed).

### 8.2 Why no UI Automation

The room on a centered taskbar is measured up to the Start button, which is the
first icon of the centered group, so it follows the group as apps open and close
(within one slow tick). On a left-aligned taskbar the bar sits before the
notification area and keeps its preferred width; running-app icons can reach it,
as they always could. Measuring the icons themselves would need UI Automation (a
slow COM call per check) — rejected 2026-10-11.

### 8.3 Auto-hide restore points

Restore points: `WM_DESTROY`, `WM_ENDSESSION` (`wParam` = TRUE) and
`~MainWindow`. `WM_ENDSESSION` matters because Windows can end the process after
it returns without sending `WM_DESTROY`; the destructor covers a `Create()` that
failed after the auto-hide change.

## 9. Data model & persistence

`HKCU\SOFTWARE\SimonSays\LastRun` (`REG_SZ`, see
[`persistence.spec.md`](persistence.spec.md) §9.2):

| Value | Meaning |
|---|---|
| `Taskbar AutoHide Restore Pending` | `"1"` while SimonSays owes an auto-hide restore; deleted after a successful restore. Non-volatile on purpose: Windows keeps the auto-hide change across reboots, so a volatile key would lose the restore after a crash + reboot. |
| `Widgets Hint Shown` | `"1"` once the Widgets message has been shown. |

Missing or any value other than `"1"` reads as unset. The layout persists
nothing: it is recomputed from the taskbar at every start.

## 10. Key interfaces

```cpp
struct TaskbarGeometry { bool found; UINT edge; RECT taskbar, window, start, tray, work;
                         bool IsVertical() const; int Thickness() const; };
TaskbarGeometry GetTaskbarGeometry();                 // sends ABM_GETTASKBARPOS
bool SameTaskbarGeometry( const TaskbarGeometry & a, const TaskbarGeometry & b );
bool TaskbarWindowsChanged( const TaskbarGeometry & last ); // no message to Explorer
const TaskbarGeometry & MainWindow::GetTaskbar() const;
void CategoryWindow::PlaceNextToBar();
bool IsTaskbarAutoHide();
bool SetTaskbarAutoHide( bool autoHide ); // true when the read-back state matches
bool IsTaskbarWidgetsShown();
static bool RegistryManager::GetTaskbarAutoHideRestorePending();
static bool RegistryManager::SetTaskbarAutoHideRestorePending( bool pending ); // false deletes the value
static bool RegistryManager::GetWidgetsHintShown();
static bool RegistryManager::SetWidgetsHintShown();
```

## 11. UI specification

- **Horizontal, Regular:** `[▲ Categories] [📂/⚙️ stacked] [input box] [Play]`,
  one row, today's look.
- **Horizontal, Thin:** the same row, shorter, 11/12 font, 📂 ⚙️ side by side.
- **Vertical:** ▶≡/◀≡ on a left taskbar, ≡◀/≡▶ on a right one (Categories), 📂 ⚙️, a two-line input box, ▶/■ (Play),
  stacked.
- The arrow on `Categories` follows TBR-F23.
- Widgets message (TBR-F13): title `TASKBAR_WIDGETS_HINT_TITLE_ID`, text
  `TASKBAR_WIDGETS_HINT_MESSAGE_ID`, Yes/No, information icon, localized in all
  18 languages, RTL-aware via `ShowLocalizedMessageBox`.
- Auto-hide handling and relayout have no UI.

## 12. Configuration & tuning constants

| Constant | Value | Where |
|---|---|---|
| `MW_DEFAULT_WINDOW_WIDTH` (preferred horizontal width) | 400 | `include/MainWindow.h` |
| `MW_MIN_EDIT_WIDTH` | 80 | `include/MainWindow.h` |
| `MW_THIN_TASKBAR_THICKNESS` | 44 | `include/MainWindow.h` |
| `MW_START_NEAR_EDGE` | 99 | `include/MainWindow.h` |
| `MW_VERTICAL_MARGIN` / `MW_VERTICAL_EDIT_HEIGHT` | 4 / 40 | `include/MainWindow.h` |
| `MW_DEFAULT_BUTTON_HEIGHT` (vertical buttons) / `MW_DEFAULT_ICON_BUTTON_SIZE` | 36 / 20 | `include/MainWindow.h` |
| `TRAY_MARGIN` / `FALLBACK_TRAY_WIDTH` | 24 / 300 | `include/MainWindow.h` |
| Fallback taskbar thickness | 48 | `GetTaskbarGeometry` |
| `SLOW_TIMER_CHECK_ZORDER_INTERVAL` / `FAST_TIMER_CHECK_ZORDER_INTERVAL` | 5000 / 400 ms | `src/MainWindow.cpp` |
| Windows 11 minimum build | 22000 | `IsTaskbarWidgetsShown` |

## 13. Diagnostics

`OutputDebugString`:
- `[Taskbar] found=… edge=… thickness=… taskbar=(…) start=(…) tray=(…) bar=(x,y wxh)`
  at startup and on every relayout — use it to confirm the Thin threshold and
  the Start/notification-area rectangles on a given Windows build.
- `[Taskbar] Could not turn auto-hide off.` and
  `[Taskbar] Could not restore auto-hide; kept pending.`
- Z-order traces as before.

## 14. Edge cases & error handling

- **Taskbar not found / not readable** → laid out as a 48 px bottom taskbar
  (TBR-F01); the next successful read relays it out.
- **Start button not found** → treated as centered: the bar starts at the
  taskbar's start with the preferred width.
- **Notification area not found** → `FALLBACK_TRAY_WIDTH` before the end.
- **Room smaller than the minimum** → the bar keeps its minimum width and may
  overlap taskbar icons.
- **Taskbar moved while the category window is minimized** → its restored
  position is moved (`SetWindowPlacement`).
- **Explorer restarts** → `TaskbarCreated` forces a relayout and refreshes the
  mouse hook's taskbar handle; the tray icon is not re-added (§17).
- **Flag write fails** → auto-hide is left as the user had it (TBR-F10).
- **Explorer not running / restarting at exit** → the restore fails, the flag is
  kept, and the next clean exit restores (TBR-F11/F12).
- **Crash, kill or power loss** → auto-hide stays off (Windows keeps it) with the
  flag set; the next session's exit restores it.
- **User turns auto-hide on by hand while SimonSays runs** → nothing reacts
  during the session; the exit restore sets it on, which it already is.
- **User turns auto-hide off by hand between a crash and the next run** → the
  stale flag still makes the next exit turn it back on (accepted; rare).
- **Windows 10** → no Widgets message; "News and interests" is not handled. Its
  default 40 px taskbar uses the Thin tier.

## 15. Acceptance criteria

- **AC-1 (TBR-F01/F02) [Pass]** Bottom taskbar: bar left of the notification
  area on a left-aligned taskbar, at the taskbar's left end on a centered one;
  starting with the taskbar on any edge no longer shows an error. *(Re-verified after
  the 2026-10-11 rework.)*
- **AC-2 (TBR-F03/F04) [Pass]** Shipping behavior: the bar returns above the
  taskbar after taskbar clicks and Start-menu use. Re-check on a top/side
  taskbar together with AC-10/AC-11.
- **AC-3 (TBR-F10/F11) [Pass]** Auto-hide on → start SimonSays → the taskbar
  stays shown → quit → auto-hide is on again and `Taskbar AutoHide Restore
  Pending` is gone.
- **AC-4 (TBR-F12) [Pass]** Auto-hide on → start → kill the process in Task
  Manager → auto-hide is still off and the flag is set → start → quit →
  auto-hide is on and the flag is gone.
- **AC-5 (TBR-F10) [Pass]** Auto-hide off → start/quit → auto-hide unchanged
  and no flag written.
- **AC-6 (TBR-F11) [Pass]** Auto-hide on → start → sign out of Windows → sign
  in → auto-hide is on.
- **AC-7 (TBR-F13) [Pass]** Windows 11 with Widgets on and no
  `Widgets Hint Shown` → the message appears once; Yes opens taskbar settings;
  next start shows nothing (also after No); Widgets off → no message.
- **AC-8 (TBR-F21) [Pass]** Bottom taskbar at the default size: the bar looks
  as before (46 px tall, 36 px buttons, 📂/⚙️ stacked).
- **AC-9 (TBR-F21, SET-F50) [Pass]** Smaller taskbar buttons on: the bar fits
  inside the taskbar, the font is smaller and 📂 ⚙️ sit side by side; the debug
  trace shows the thickness (record it here).
- **AC-10 (TBR-F02/F21/F23/F30) [Pass]** Top taskbar: the bar sits on it, the
  arrow points down while the category window is hidden, and the window opens
  below the taskbar.
- **AC-11 (TBR-F02/F22/F23/F30) [Pass]** Left and right taskbars: the
  stacked bar sits just above the notification area when Start is at the top
  (at the very top when Start is centered); `Categories` shows ≡ and its arrow
  and `Play` shows ▶, switching to ■ while speaking and back after, in every
  language; a screen reader still announces the full labels; the window opens
  beside the taskbar; typing and `Enter` speak. *(Verified manually by the
  developer 2026-10-11, including the ≡ sign on left and right taskbars.)*
- **AC-12 (TBR-F30, CAT-F43) [Pass]** On each edge, double-clicking the
  category window's frame auto-fits it without crossing the taskbar or leaving
  the screen.
- **AC-13 (TBR-F20) [Pass]** Centered bottom taskbar: opening and closing
  many apps widens/narrows the bar (and its input box) within ~5 s, never below
  the minimum.
- **AC-14 (TBR-F40) [Pass]** Moving the taskbar to another edge (and turning
  smaller buttons on/off) while SimonSays runs relays the bar out within ~5 s,
  keeping the typed text; the category window follows.
- **AC-15 (TBR-F30) [Pass]** With the category window open, then hidden,
  then minimized, switching the taskbar between centered and left-aligned moves
  it along with the bar each time. After dragging it by hand, switching the
  alignment leaves it where it was; moving the taskbar to another edge
  re-places it next to the bar. *(Verified manually by the developer on
  `Release\SimonSays.exe`, 2026-10-11.)*

AC-3–AC-7 verified manually by the developer on `Release\SimonSays.exe`,
2026-10-07; AC-1 and AC-8–AC-15 on 2026-10-11 (AC-11 re-verified after the
vertical button symbols changed) (the AC-9 thickness was not
recorded, so the 44 px Thin threshold is confirmed only as working on the
developer's setup).

Build gate: Debug **and** Release Win32 compile clean, apart from the
pre-existing C4244 warnings in `CategoryWindow.cpp` / `utils.cpp`
(2026-10-11); `x64` Release also compiles (plus its usual C4267 warnings).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Any-edge support + placement | ✅ Done | TBR-F01/F02; AC-1/AC-10/AC-11 verified 2026-10-11 |
| Dynamic width | ✅ Done | TBR-F20; AC-13 verified 2026-10-11 |
| Regular / Thin tiers | ✅ Done | TBR-F21; AC-8/AC-9 verified 2026-10-11 |
| Vertical stack | ✅ Done | TBR-F22; text symbols (≡ + arrow, ▶/■); AC-11 verified 2026-10-11 |
| Edge-aware arrow | ✅ Done | TBR-F23; AC-10/AC-11 |
| Category window placement | ✅ Done | TBR-F30, CAT-F43; follows alignment changes unless dragged; AC-12/AC-15 verified 2026-10-11 |
| Live relayout | ✅ Done | TBR-F40/N03; AC-14 verified 2026-10-11 |
| Z-order recovery | ✅ Done | TBR-F03/F04; timers + mouse hook |
| Keep taskbar shown (auto-hide off, restore on exit, crash-safe flag) | ✅ Done | TBR-F10–F12; AC-3–AC-6 verified manually 2026-10-07 |
| Show-once Widgets message | ✅ Done | TBR-F13; 18 languages; AC-7 verified manually 2026-10-07 |

## 17. Known limitations

- The taskbar on the primary monitor only.
- The bar is not an app bar: maximized windows can extend under it.
- Widgets can still overlap the bar if the user answers No; the bar is not
  moved out of its way.
- The Widgets check reads an undocumented value; if Windows renames it, the
  message simply stops appearing.
- On a left-aligned taskbar, running-app icons can reach the bar (§8.2).
- On a narrow vertical taskbar the input box's scrollbar takes most of its
  width; the flyout input box (§18) is meant to solve it.
- The touch keyboard still opens above and to the right of the bar, which does
  not suit a top or side taskbar (TODO).
- After an Explorer restart the tray icon is not re-added (pre-existing).
- No DPI awareness: windows are bitmap-scaled at scales above 100%.

## 18. Future work

- **Phase 2 — flyout input box:** on every edge the bar keeps a small preview
  and focusing it opens a wider multi-line input next to the taskbar (amends
  SET-F60/F61).
- **Phase 3 — portrait-first auto-fit:** rows before columns, about 25–35 % of
  the screen width, and a remembered size per taskbar orientation (amends
  CAT-F43).
- Register the bar as its own app bar (`ABM_NEW`) so Windows reserves its space.
- Place the bar clear of the Widgets button (needs its size — UI Automation).

## 19. Open questions

1. ~~Should the bar follow a taskbar moved to another edge or monitor at
   runtime?~~ Resolved 2026-10-11: it follows edge and size changes (TBR-F40);
   other monitors remain a non-goal.

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7.
