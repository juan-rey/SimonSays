# SimonSays — Taskbar Integration — Specification

| | |
|---|---|
| **Spec ID** | TBR-SPEC |
| **Status** | Active — created 2026-10-06. TBR-F01–F04 reverse-engineered from shipping source; TBR-F10–F13 (taskbar kept shown while running, show-once Widgets hint) added the same day; AC-3–AC-7 verified manually 2026-10-07 |
| **Version** | 1.0 (2026-10-06) |
| **REQ prefix** | `TBR-F##` (functional), `TBR-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/MainWindow.cpp`](../../src/MainWindow.cpp) (`Create`, `EnsureTaskbarShown`, `RestoreTaskbarAutoHide`, `ShowWidgetsHintOnce`, Z-order timers, `LowLevelMouseProc`), taskbar helpers in [`src/utils.cpp`](../../src/utils.cpp), `\LastRun` flags in [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp) |
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
Windows taskbar, and the Windows taskbar settings SimonSays reads or changes.
The controls inside the bar are owned by [`settings.spec.md`](settings.spec.md)
(SET-F50/F60) and [`tts.spec.md`](tts.spec.md); `\LastRun` storage by
[`persistence.spec.md`](persistence.spec.md).

## 1. Overview

SimonSays draws its bar as a topmost popup window laid over the bottom
taskbar. To stay usable it must (a) find a free spot on the taskbar, (b) stay
above the taskbar when Windows reorders windows, and (c) keep the taskbar itself
on screen. It never writes taskbar settings it cannot cleanly restore.

## 2. Background & context

- The bar's position was only ever described in `HELP.md` (bottom taskbar
  only). This spec was created 2026-10-06 when the taskbar auto-hide and
  Widgets handling were added.
- **Auto-hide:** when the taskbar auto-hides, the bar floats over other
  windows' content at the bottom of the screen. Auto-hide is a documented
  app-bar state (`SHAppBarMessage` `ABM_GETSTATE`/`ABM_SETSTATE`), saved by
  Windows itself across reboots.
- **Widgets (Windows 11):** the Widgets button sits at the left end of a
  centered taskbar (where the bar goes when the Start button isn't at the left)
  and next to the notification area on a left-aligned one (where the bar goes
  otherwise), so it can collide with the bar either way. Its only switch is the
  undocumented `HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced\TaskbarDa`
  value, which current Windows 11 builds protect against writes from ordinary
  apps. SimonSays therefore only reads it.

## 3. Goals & non-goals

**Goals:** keep the bar on a visible taskbar for the whole session; leave the
user's taskbar settings exactly as they were once SimonSays exits — including
after a crash; never write undocumented or protected settings.

**Non-goals:** supporting top/left/right taskbars; multi-monitor placement;
reserving screen space for the bar (app-bar registration — §18); changing the
Widgets setting.

## 4. Glossary

| Term | Meaning |
|---|---|
| **Bar** | The SimonSays main window laid over the taskbar. |
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

## 6. Requirements (EARS)

### 6.1 Placement & Z-order (existing behavior)

- **TBR-F01 [Done]** IF the taskbar is not at the bottom edge
  (`IsTaskbarAtBottom`, `ABM_GETTASKBARPOS`) THEN THE SYSTEM SHALL show the
  localized `ERROR_TASKBAR_POSITION_ID` error and not start.
- **TBR-F02 [Done]** THE SYSTEM SHALL create the bar as a topmost layered popup
  (`MW_DEFAULT_WINDOW_WIDTH` × `MW_DEFAULT_WINDOW_HEIGHT`) on the bottom edge of
  the primary screen: WHEN the Start button is found within the first 99 px
  (left-aligned taskbar) left of the notification area with `TRAY_MARGIN`
  (or `FALLBACK_TRAY_WIDTH` from the right edge when the area is not found),
  OTHERWISE at x = 0.
- **TBR-F03 [Done]** THE SYSTEM SHALL re-assert the bar's topmost Z-order every
  `SLOW_TIMER_CHECK_ZORDER_INTERVAL` ms, and immediately WHEN a low-level mouse
  hook sees a click on the taskbar; WHILE the taskbar covers the bar THE SYSTEM
  SHALL check every `FAST_TIMER_CHECK_ZORDER_INTERVAL` ms until it is on top
  again, including the Start-menu close workaround
  (`SimulateNonActivatingClickOnWindow`).
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

### 6.4 Non-functional

- **TBR-N01 [Done]** Taskbar settings SHALL be changed only through documented
  APIs, and only when the original value is recorded first (TBR-F10).
- **TBR-N02 [Done]** The checks SHALL add no polling: auto-hide and Widgets are
  read once at startup; restore runs once at exit.

## 7. Architecture & components

| Piece | Where | Role |
|---|---|---|
| `IsTaskbarAtBottom`, `GetStartButtonXPosition`, `GetSystemTrayXPosition` | `src/utils.cpp` | placement (TBR-F01/F02) |
| `IsTaskbarAutoHide`, `SetTaskbarAutoHide` | `src/utils.cpp` | app-bar state read/write; `Set` verifies by reading back |
| `IsTaskbarWidgetsShown` | `src/utils.cpp` | read-only Windows 11 build + `TaskbarDa` check |
| `MainWindow::EnsureTaskbarShown` / `RestoreTaskbarAutoHide` / `ShowWidgetsHintOnce` | `src/MainWindow.cpp` | TBR-F10–F13 |
| `RegistryManager::Get/SetTaskbarAutoHideRestorePending`, `GetWidgetsHintShown`/`SetWidgetsHintShown` | `src/RegistryManager.cpp` | `\LastRun` flags |
| Z-order timers + `LowLevelMouseProc` | `src/MainWindow.cpp` | TBR-F03/F04 |

Layering: utils helpers know nothing about the registry flags; only
`MainWindow` combines them.

## 8. Detailed design

Startup order in `MainWindow::Create`: `EnsureTaskbarShown()` runs first (before
placement, so the notification-area position is read from a shown taskbar), then
placement (TBR-F01/F02), window creation, … , help, shipped-board sync
(import-export PORT-F41), and `ShowWidgetsHintOnce()` last (needs the bar as
owner).

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

Missing or any value other than `"1"` reads as unset.

## 10. Key interfaces

```cpp
bool IsTaskbarAutoHide();
bool SetTaskbarAutoHide( bool autoHide ); // true when the read-back state matches
bool IsTaskbarWidgetsShown();
static bool RegistryManager::GetTaskbarAutoHideRestorePending();
static bool RegistryManager::SetTaskbarAutoHideRestorePending( bool pending ); // false deletes the value
static bool RegistryManager::GetWidgetsHintShown();
static bool RegistryManager::SetWidgetsHintShown();
```

## 11. UI specification

- Error `ERROR_TASKBAR_POSITION_ID` (TBR-F01).
- Widgets message (TBR-F13): title `TASKBAR_WIDGETS_HINT_TITLE_ID`, text
  `TASKBAR_WIDGETS_HINT_MESSAGE_ID`, Yes/No, information icon, localized in all
  18 languages, RTL-aware via `ShowLocalizedMessageBox`.
- Auto-hide handling has no UI.

## 12. Configuration & tuning constants

| Constant | Value | Where |
|---|---|---|
| `MW_DEFAULT_WINDOW_WIDTH` / `MW_DEFAULT_WINDOW_HEIGHT` | 400 / 46 | `include/MainWindow.h` |
| `TRAY_MARGIN` / `FALLBACK_TRAY_WIDTH` | 24 / 300 | `include/MainWindow.h` |
| `SLOW_TIMER_CHECK_ZORDER_INTERVAL` / `FAST_TIMER_CHECK_ZORDER_INTERVAL` | 5000 / 400 ms | `src/MainWindow.cpp` |
| Windows 11 minimum build | 22000 | `IsTaskbarWidgetsShown` |

## 13. Diagnostics

`OutputDebugString`: `[Taskbar] Could not turn auto-hide off.` and
`[Taskbar] Could not restore auto-hide; kept pending.`; Z-order traces as before.

## 14. Edge cases & error handling

- **Flag write fails** → auto-hide is left as the user had it (TBR-F10).
- **Explorer not running / restarting at exit** → the restore fails, the flag is
  kept, and the next clean exit restores (TBR-F11/F12).
- **Crash, kill or power loss** → auto-hide stays off (Windows keeps it) with the
  flag set; the next session's exit restores it.
- **User turns auto-hide on by hand while SimonSays runs** → nothing reacts
  during the session; the exit restore sets it on, which it already is.
- **User turns auto-hide off by hand between a crash and the next run** → the
  stale flag still makes the next exit turn it back on (accepted; rare).
- **Start fails on a non-bottom taskbar** → auto-hide was already turned off by
  `EnsureTaskbarShown`; `~MainWindow` restores it straight away.
- **Windows 10** → no Widgets message; "News and interests" is not handled.

## 15. Acceptance criteria

- **AC-1 (TBR-F01/F02) [Pass]** Shipping behavior: bottom taskbar only; bar left
  of the notification area on a left-aligned taskbar, at x = 0 otherwise.
- **AC-2 (TBR-F03/F04) [Pass]** Shipping behavior: the bar returns above the
  taskbar after taskbar clicks and Start-menu use.
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

AC-3–AC-7 verified manually by the developer on `Release\SimonSays.exe`,
2026-10-07.

Build gate: Debug **and** Release Win32 compile clean (2026-10-06); `x64`
Release also compiles.

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Bottom-only check + placement | ✅ Done | TBR-F01/F02; shipping behavior |
| Z-order recovery | ✅ Done | TBR-F03/F04; timers + mouse hook |
| Keep taskbar shown (auto-hide off, restore on exit, crash-safe flag) | ✅ Done | TBR-F10–F12; AC-3–AC-6 verified manually 2026-10-07 |
| Show-once Widgets message | ✅ Done | TBR-F13; 18 languages; AC-7 verified manually 2026-10-07 |

## 17. Known limitations

- Bottom taskbar on the primary monitor only.
- The bar is not an app bar: maximized windows can extend under it.
- Widgets can still overlap the bar if the user answers No; the bar is not
  moved out of its way.
- The Widgets check reads an undocumented value; if Windows renames it, the
  message simply stops appearing.

## 18. Future work

- Register the bar as its own app bar (`ABM_NEW`) so Windows reserves its space.
- Place the bar clear of the Widgets button (needs its size — UI Automation).

## 19. Open questions

1. Should the bar follow a taskbar moved to another edge or monitor at runtime?

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7.
