# SimonSays — Settings (General + Window Behavior) — Specification

| | |
|---|---|
| **Spec ID** | SET-SPEC |
| **Status** | Active — reverse-engineered from shipping source (2026-07-10) |
| **Version** | 1.0 (2026-07-10) |
| **REQ prefix** | `SET-F##` (functional), `SET-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/MainWindow.cpp`](../../src/MainWindow.cpp) (`SettingsDialogProc`, tray menu, apply path), `Settings` in [`include/stdafx.h`](../../include/stdafx.h) |
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

See [`AGENT.md`](../../AGENT.md) and [`docs/spec.md`](../spec.md) §2 for shared
conventions. This file is the source of truth for the **Settings dialog** (its
fields, voice test, and apply/persist behavior), the **tray icon menu**, and
the touch-keyboard / window-move behaviors. Reference format:
[`docs/specs/dwell.spec.md`](dwell.spec.md).

This spec was authored **from the existing code**, so per the working agreement
the code is authoritative for current behavior (rule 7): if a fact here drifts
from [`src/MainWindow.cpp`](../../src/MainWindow.cpp), fix the spec and flag it.

> Scope reminder: the Settings window fields and their apply/cancel semantics,
> the tray menu, the touch keyboard, and main-window/dialog move. **Where** each
> value is stored is in [`persistence.spec.md`](persistence.spec.md); **voice /
> volume / rate / test** semantics are in [`tts.spec.md`](tts.spec.md); the
> playback toggles' *effects* (increase volume / duck others / stop previous) are
> in [`sound.spec.md`](sound.spec.md); the **category-window** behaviors the
> toggles affect are in [`categories-phrases.spec.md`](categories-phrases.spec.md);
> the **Dwell** settings dialog is in [`dwell.spec.md`](dwell.spec.md); UI
> language is in [`localization.spec.md`](localization.spec.md).

---

## 1. Overview

The **Settings** dialog (`F2` / tray → Settings) is a modal window that edits a
working copy of the app's `Settings`: default text, UI language, SAPI voice,
volume/rate, and a set of playback and category-window toggles, plus a **Test
Voice** preview. **OK** applies and persists; **Cancel** discards. Settings is
reached from the **tray icon** menu, which also offers Show/Hide, the Dwell
dialog, Web, About, Feedback, and Exit. This spec also covers the touch-keyboard
option and main-window/dialog dragging; the separate **Dwell** dialog is owned
by [`dwell.spec.md`](dwell.spec.md).

## 2. Background & context

- The app is a tray-resident utility with a small taskbar-docked main window; its
  configuration surface is one modal Settings dialog plus the tray menu.
- Settings edits a **copy** (`tempSettings`) so Cancel is lossless; only OK
  mutates the live `Settings`, persists them, and re-applies voice/language.
- Voice preview must not freeze the modal dialog, so it runs on a **detached
  worker thread** that owns and releases the preview voice.

## 3. Goals & non-goals

**Goals**
- One modal dialog to edit all general settings, with lossless Cancel.
- Language-aware voice selection and a non-blocking voice preview.
- Immediate apply on OK: persist, re-voice, and re-language the running app.
- A tray menu for the app's top-level actions.

**Non-goals**
- Registry **storage** of the values (→ [`persistence.spec.md`](persistence.spec.md)).
- Voice/volume/rate **playback** semantics (→ [`tts.spec.md`](tts.spec.md) /
  [`sound.spec.md`](sound.spec.md)).
- The **Dwell** settings dialog (→ [`dwell.spec.md`](dwell.spec.md)).
- The category window's own resize/zoom (→ [`categories-phrases.spec.md`](categories-phrases.spec.md)).

## 4. Glossary

| Term | Meaning |
|---|---|
| **Settings** | The [`Settings`](../../include/stdafx.h) struct mirrored to the registry. |
| **tempSettings** | The working copy the dialog edits; committed to `Settings` only on OK. |
| **Test Voice** | A non-blocking preview of the selected voice/volume/rate. |
| **Tray menu** | The `Shell_NotifyIcon` context menu of top-level actions. |

## 5. Personas & scenarios

- **Pick a voice for my language:** open Settings, choose a language → the voice
  list filters to matching voices; press Test Voice to hear one; OK applies.
- **Quieter, slower speech:** drag the volume/rate sliders (or type in the edits);
  OK persists and re-voices.
- **Tune behavior:** toggle "speak immediately on click", "stop previous
  playback", "reduce other audio", etc.; OK applies.
- **Everyday control:** right-click the tray icon → Show/Hide, Settings, Dwell,
  Web, About, Feedback, Exit.

## 6. Requirements (EARS)

Status tags per [`docs/spec.md`](../spec.md) §2.3. All requirements below are
implemented in the current source and tagged **[Done]** accordingly.

### 6.1 Settings dialog fields

- **SET-F01 [Done]** THE Settings dialog SHALL expose: default text + "use
  default text"; UI language; SAPI voice; volume and rate (each a slider bound to
  a numeric edit, clamped); and toggles for speak-immediately-on-select, remember
  category-window size, minimize category-window automatically, increase volume
  when playing, reduce other audio when playing, stop previous playback, and show
  touch keyboard.
- **SET-F02 [Done]** THE dialog SHALL edit a **copy** (`tempSettings`) seeded from
  the live settings on open; **Cancel** SHALL discard it and **OK** SHALL commit
  it (SET-F20).
- **SET-F03 [Done]** THE volume/rate slider and edit SHALL stay in sync and values
  SHALL be clamped to `[SIMONSAYS_SETTINGS_MIN…MAX_VOICE_VOLUME]` and
  `[…_VOICE_RATE]`.

### 6.2 Voice selection & test

- **SET-F10 [Done]** THE voice combo SHALL be populated from the installed SAPI
  voices filtered by the selected language; WHEN the language selection changes
  THE SYSTEM SHALL repopulate the voice combo for the new language.
- **SET-F11 [Done]** WHEN **Test Voice** is pressed THE SYSTEM SHALL preview the
  currently selected voice at the current volume/rate using the language's sample
  text (falling back to the default text, then a built-in sample), on a
  **detached worker thread** that owns and releases the preview voice so the
  dialog stays responsive.

### 6.3 Apply on OK

- **SET-F20 [Done]** WHEN **OK** is pressed THE SYSTEM SHALL read every control
  into `tempSettings`, mark the result accepted, and — after the dialog closes —
  commit it to the live `Settings` (clamping volume/rate), persist via
  `RegistryManager::SaveSettingsToRegistry` (→ [`persistence.spec.md`](persistence.spec.md)),
  and apply the voice/volume/rate to playback (`ApplyVoiceSettings`).
- **SET-F21 [Done]** IF the language changed THEN THE SYSTEM SHALL reload the
  categories for the new language, refresh the category window, and update the UI
  language of the main window and (if open) the Help window (→
  [`localization.spec.md`](localization.spec.md)).
- **SET-F22 [Done]** ON apply THE SYSTEM SHALL push the remember-size and
  minimize-on-focus-loss toggles to the category window, and IF "use default
  text" is on THEN set the input box to the default text.

### 6.4 Tray icon & menu

- **SET-F30 [Done]** THE SYSTEM SHALL show a tray icon with a localized tooltip;
  a **left-click** SHALL toggle show/hide of the main window; a **right-click**
  SHALL open a context menu.
- **SET-F31 [Done]** THE tray menu SHALL offer Show/Hide (label reflecting
  visibility), Settings, Dwell, Web, About, Feedback, and Exit; **Settings** and
  **Dwell** SHALL be disabled while a modal dialog is already open.
- **SET-F32 [Done]** Menu commands SHALL: toggle the window; open the Settings
  dialog; open the Dwell dialog (→ [`dwell.spec.md`](dwell.spec.md)); open the
  project web page / feedback form in the browser; show the About dialog
  (version + credits); and exit the app.

### 6.5 Touch keyboard & window move

- **SET-F40 [Done]** WHILE "show touch keyboard" is enabled THE SYSTEM SHALL open
  the on-screen touch keyboard near the input box when it receives focus.
- **SET-F41 [Done]** THE main window and dialogs SHALL be movable by dragging;
  the category window's own resize/remember-size is owned by
  [`categories-phrases.spec.md`](categories-phrases.spec.md).

### 6.6 Non-functional

- **SET-N01 [Done]** All dialog/menu strings SHALL be localized via
  `GetLocalizedString`.
- **SET-N02 [Done]** The dialog SHALL persist only through `RegistryManager`
  (never touching the registry directly) and SHALL clamp volume/rate on commit.
- **SET-N03 [Done]** The voice preview SHALL not block the UI thread (detached
  worker; COM initialized/uninitialized within it).

## 7. Architecture & components

### 7.1 Files

| File | Responsibility |
|---|---|
| [`src/MainWindow.cpp`](../../src/MainWindow.cpp) | `ShowSettingsDialog` + `SettingsDialogProc`; tray icon/menu; apply/persist; About/Web/Feedback; touch keyboard. |
| [`include/stdafx.h`](../../include/stdafx.h) | `Settings` struct; string IDs. |
| [`src/utils.cpp`](../../src/utils.cpp) | `ConfigureSlider` / `SyncSliderToEdit` / `SyncEditToSlider`; `ShowTouchKeyboard`. |
| [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp) | Load/save of the values (→ [`persistence.spec.md`](persistence.spec.md)). |

### 7.2 Flow

`ShowSettingsDialog` builds a `SettingsDialogContext` (a copy of `Settings`, the
language list, and the voice list), runs `DialogBoxParam(IDD_SETTINGS_DIALOG)`,
and on `accepted` commits/persists/applies. `SettingsDialogProc` handles
`WM_INITDIALOG` (seed controls), `WM_HSCROLL`/edit sync, language-combo change
(repopulate voices), Test Voice, and OK/Cancel.

## 8. Detailed design

### 8.1 Dialog lifecycle

- **Init:** localize labels; set default-text + checkboxes from `tempSettings`;
  populate language combo (current selected); populate voice combo filtered to the
  language; configure volume/rate sliders + edits.
- **Interact:** slider⇄edit sync (clamped); language change → repopulate voices;
  Test Voice → detached preview.
- **OK:** read all controls into `tempSettings`; `accepted = true`; `EndDialog`.
- **Cancel:** `EndDialog` with no commit.

### 8.2 Apply (post-dialog)

If accepted: on language change → reload categories + `UpdateUILanguage` + Help
language; push category-window toggles; `m_settings = tempSettings` (clamped);
`SaveSettingsToRegistry`; `ApplyVoiceSettings`; set default text if enabled.

### 8.3 Voice preview threading

The preview `ISpVoice` is created and configured (voice/volume/rate) on the UI
thread, then a detached `std::thread` `CoInitializeEx`s, `Speak`s +
`WaitUntilDone`, `Release`s the voice, and `CoUninitialize`s — so a synchronous
speak never freezes the modal dialog.

## 9. Data model & persistence

The dialog reads/writes the [`Settings`](../../include/stdafx.h) struct; all
fields persist as `REG_SZ` under `HKCU\SOFTWARE\SimonSays\Settings` — the value
names, defaults, and load/save lifecycle are owned by
[`persistence.spec.md`](persistence.spec.md) §9.1. The dwell fields in the same
struct are edited by the Dwell dialog ([`dwell.spec.md`](dwell.spec.md)), not
this one.

## 10. Key interfaces

```cpp
// MainWindow (settings-related)
void ShowSettingsDialog();                 // build context → DialogBoxParam → apply
static INT_PTR CALLBACK SettingsDialogProc( HWND, UINT, WPARAM, LPARAM );
void ApplyVoiceSettings();                 // push voice/volume/rate to playback (→ tts)
void SetEditControlText( const std::wstring & ); // used by "use default text" / phrase select
// Tray
void CreateTrayIcon(); void ShowTrayMenu();
// Slider/keyboard helpers (utils.h)
void ConfigureSlider( HWND, int sliderId, int min, int max, int value, int ticks = 10 );
void SyncSliderToEdit( HWND, int sliderId, int editId, BOOL isSigned );
void SyncEditToSlider( HWND, int editId, int sliderId, BOOL isSigned, int min = 0, int max = 0 );
void ShowTouchKeyboard( HWND near = NULL, SIZE rel = {0,0}, int margin = 0 );
```

## 11. UI specification

A modal dialog (`IDD_SETTINGS_DIALOG`) with: default-text edit + "use default
text" check; language combo; voice combo + **Test Voice** button; volume and
rate sliders each paired with a numeric edit; and the seven behavior checkboxes;
`OK` / `Cancel`. The **tray menu** lists Show/Hide · Settings · Dwell · Web ·
About · Feedback · Exit (Settings/Dwell greyed while a dialog is open). The
**About** dialog shows version + credits. The main window and dialogs move by
dragging their title bars / bodies.

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| Settings dialog resource | `IDD_SETTINGS_DIALOG` | `resource.h` |
| Voice volume range | 10 – 100 | `SIMONSAYS_SETTINGS_MIN/MAX_VOICE_VOLUME` |
| Voice rate range | −10 … +10 | `SIMONSAYS_SETTINGS_MIN/MAX_VOICE_RATE` |
| Default-text buffer | 1024 wchar | `MainWindow.cpp` `SettingsDialogProc` |
| Tray callback message | `WM_TRAYICON` | [`stdafx.h`](../../include/stdafx.h) |
| Control / string IDs | `IDC_SETTINGS_*` / `SETTINGS_*_ID` | `resource.h` / `stdafx.h` |

Value names/defaults for storage are in [`persistence.spec.md`](persistence.spec.md) §9.1.

## 13. Diagnostics

N/A — no diagnostic output of its own.

## 14. Edge cases & error handling

- **No voices / none for the language** → the voice combo may be empty; playback
  falls back per [`tts.spec.md`](tts.spec.md).
- **Language set to the system language** → stored as `""` (meaning "system
  default") so it tracks the OS language.
- **Cancel** → no field is committed or persisted.
- **Test Voice with COM failure** → the preview is skipped silently.
- **Modal dialog already open** → Settings/Dwell tray items are disabled to
  prevent re-entrancy.

## 15. Acceptance criteria (testable)

Reverse-engineered from shipping behavior; **[Pass]** reflects the code path.

- **AC-1 (SET-F01/F02/F03) [Pass]** All fields load from current settings; sliders
  and edits stay in sync and clamp; Cancel changes nothing.
- **AC-2 (SET-F10/F11) [Pass]** Choosing a language filters the voice list;
  Test Voice previews without freezing the dialog.
- **AC-3 (SET-F20/F21/F22) [Pass]** OK persists and re-voices; a language change
  reloads categories + UI language; toggles reach the category window; default
  text is applied when enabled.
- **AC-4 (SET-F30–F32) [Pass]** Tray left-click toggles the window; the menu opens
  and each command works; Settings/Dwell are disabled while a dialog is open.
- **AC-5 (SET-F40/F41) [Pass]** The touch keyboard appears near the input box on
  focus when enabled; windows drag.

Build gate: Debug **and** Release x64 compile clean (no code change in this
authoring pass).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Settings dialog fields | ✅ Done | copy-edit + clamp |
| Voice list (language-filtered) | ✅ Done | repopulate on language change |
| Test Voice (non-blocking) | ✅ Done | detached worker |
| Apply/persist/re-voice on OK | ✅ Done | save + `ApplyVoiceSettings` |
| Language change → reload + relabel | ✅ Done | categories + UI + Help |
| Tray icon + menu | ✅ Done | 7 actions; re-entrancy guard |
| Touch keyboard | ✅ Done | on input focus |
| Window/dialog move | ✅ Done | drag |

## 17. Known limitations

- One flat dialog (no tabs/search) for all general settings.
- The Dwell settings live in a **separate** dialog (`F3`), not this one.
- Voice list depends on installed SAPI voices; none may match a language.

## 18. Future work

- Grouped/tabbed settings as the option set grows.
- Live preview of playback toggles.

## 19. Open questions

1. Should Settings and Dwell be merged into one multi-page dialog?

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of SET-SPEC v1.0.*
