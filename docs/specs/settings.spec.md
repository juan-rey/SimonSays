# SimonSays — Settings (General + Window Behavior) — Specification

| | |
|---|---|
| **Spec ID** | SET-SPEC |
| **Status** | Active — reverse-engineered from shipping source (2026-07-10); main-window quick access buttons (SET-F50/F51/N04) added 2026-09-20; input box made a wrapping multiline box with folded paste (SET-F60/F61) 2026-09-25; quick access buttons placed side by side on thin and side taskbars (SET-F50, taskbar.spec.md TBR-F21/F22) 2026-10-11; input box moved into a flyout opened from a preview in the bar (SET-F62/F63, SET-F40/F61 amended) 2026-10-11; sound output device + Listen settings button (SET-F15/F16, SET-F01/F11 amended) 2026-10-13; Help button + F1 in the dialog (SET-F17) 2026-10-14; dialog code moved out of `MainWindow.cpp` into `SettingsDialog.cpp` 2026-10-14 (no behavior change); quick access setting renamed `Show quick access buttons and menu` and now also gates the category window's right-click menu (SET-F51 amended), Exit asks for confirmation (SET-F33) 2026-10-30 |
| **Version** | 1.7 (2026-10-30) |
| **REQ prefix** | `SET-F##` (functional), `SET-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/SettingsDialog.cpp`](../../src/SettingsDialog.cpp) (the dialog: `ShowSettingsDialog`, `SettingsDialogProc`), [`src/MainWindow.cpp`](../../src/MainWindow.cpp) (`MainWindow::ShowSettingsDialog` apply path, tray menu), `Settings` in [`include/stdafx.h`](../../include/stdafx.h) |
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
  when playing, reduce other audio when playing, stop previous playback, show
  touch keyboard, and show quick access buttons and menu (SET-F51); and the sound output
  device with its **Listen settings...** button (SET-F15/F16); and the
  **Gaze / Dwell-click...** button (SET-F18). The sound output row SHALL sit
  directly below the voice settings (volume and rate), in the layout of §11.
  *(Amended 2026-10-21.)*
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
  dialog stays responsive. The preview SHALL play through the device currently
  selected in **Sound output** (SET-F15), so the cable routing can be checked
  before pressing OK. *(Amended 2026-10-13.)*

### 6.2.1 Sound output (virtual cables and online meetings)

- **SET-F15 [Done]** THE dialog SHALL offer a **Sound output** combo listing
  `Windows default`, then every active output device with **virtual audio
  cables first** — a device whose friendly name, description or driver
  interface name contains one of `VIRTUAL_CABLE_NAME_PATTERNS` (`vb-audio`,
  `voicemeeter`, `virtual audio cable`, `virtual cable`, case-insensitive) —
  then the rest in Windows' order. WHEN the saved device is not connected THE
  combo SHALL add and select `<saved name> (not connected)`, so pressing OK
  keeps the choice. WHEN `SETTINGS_OUTPUT_DEVICE_SHORT_LIST` is defined at build
  time THE combo SHALL list only `Windows default`, the current choice and the
  virtual cables. The choice SHALL be stored as the Core Audio endpoint ID plus
  its display name (empty ID = Windows default) and applied through
  `PlaybackEngine::SetOutputDevice` ([`sound.spec.md`](sound.spec.md) SND-F70).
  WHEN the UI language changes in the dialog THE combo SHALL be refilled in that
  language, keeping the selection. The cable patterns only affect the order: an
  unlisted cable is still offered, further down.
- **SET-F16 [Done]** THE **Listen settings...** button SHALL open the classic
  Sound control panel on its **Recording** tab (`control.exe mmsys.cpl,,1`), and
  carry a localized tooltip naming the steps (select the cable's output →
  Properties → Listen → Listen to this device). SimonSays SHALL NOT change
  **Listen to this device** itself: it is a setting of another device that
  Windows offers apps no supported way to change, so the user sets it once in
  the Windows dialog (documented in the help, *Using SimonSays in online
  meetings*).
- **SET-F17 [Done]** WHEN the **Help** button in the dialog's bottom-left
  corner is pressed, or `F1` is pressed while the dialog is open (`WM_HELP`;
  the main window's accelerator does not reach the modal dialog), THE SYSTEM
  SHALL open the shipped help page for the language **currently selected in
  the dialog's Language list** — so it matches the dialog's text — falling
  back to English like `F1` ([`localization.spec.md`](localization.spec.md)
  LOC-F42), at the top of the page. The dialog SHALL stay open with its unsaved
  changes. The button label (`SETTINGS_HELP_BUTTON_ID`) is the same word each
  language already uses for Help in `CATEGORY_SHORTCUTS_TEXT_ID`. Both paths
  use `OpenHelpPage` (`src/utils.cpp`), which `ShowHelpWindow` also uses.
  *(Added 2026-10-14.)*
- **SET-F18 [Done]** WHEN the **Gaze / Dwell-click...** button is pressed THE
  SYSTEM SHALL open the dwell window ([`dwell.spec.md`](dwell.spec.md),
  `IDD_DWELL_DIALOG`) owned by the Settings dialog, which SHALL stay open with
  its unsaved changes. The dwell window's **OK** SHALL save the dwell fields
  at once, whatever Settings' OK/Cancel does later; and Settings' **OK** SHALL
  keep the live dwell fields instead of the copy taken when it opened, so
  neither the dwell window's changes nor an Auto-detected `dwellDetectedMode`
  recorded meanwhile are overwritten. Both paths go through
  `MainWindow::RunDwellWindow` / `CopyDwellSettings`, which `F3` and the tray
  entry also use. The label reuses `TRAYICON_DWELL_ID` (the tray menu's
  wording); no new localized string. *(Added 2026-10-21.)*

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
- **SET-F33 [Done]** WHEN the user chooses Exit — from the tray menu or from
  the category window's right-click menu
  ([`categories-phrases.spec.md`](categories-phrases.spec.md) CAT-F59), both
  `ID_TRAY_EXIT` — THE SYSTEM SHALL ask first (`EXIT_CONFIRM_TITLE_ID` /
  `EXIT_CONFIRM_MESSAGE_ID`, Yes/No, **No** the default) and exit only on Yes.
  A second Exit while the question is open SHALL be ignored. *(Added
  2026-10-30: the tray's Exit used to close at once; with a second Exit next
  to the phrase buttons, a slip would silence the user's voice.)*

### 6.5 Touch keyboard & window move

- **SET-F40 [Done]** WHILE "show touch keyboard" is enabled THE SYSTEM SHALL open
  the on-screen touch keyboard near the input box when it receives focus — beside
  the input flyout, on the side away from the taskbar
  ([`taskbar.spec.md`](taskbar.spec.md) TBR-F32). *(Amended 2026-10-11: it
  opened above and to the right of the bar whatever the taskbar's edge.)*
- **SET-F41 [Done]** THE main window and dialogs SHALL be movable by dragging;
  the category window's own resize/remember-size is owned by
  [`categories-phrases.spec.md`](categories-phrases.spec.md).

### 6.6 Quick access buttons (main window)

- **SET-F50 [Done]** THE main window SHALL place two icon-only **quick access
  buttons** immediately to the right of the `Categories` button, stacked
  vertically on a regular horizontal taskbar and side by side on a thin one
  (below the `Categories` button on a side taskbar — the layout is owned by
  [`taskbar.spec.md`](taskbar.spec.md) TBR-F21/F22): 📂 **open a board**, which opens the import dialog with the boards
  folder as its initial directory (→ [`import-export.spec.md`](import-export.spec.md)
  PORT-F40), and ⚙️ **Settings**, which opens the Settings dialog (the `F2`
  action). They are `SSButton`s drawn with a centered emoji, no border, and a
  fully rounded corner radius (→ [`ssbutton.spec.md`](ssbutton.spec.md)).
  *(Amended 2026-10-11: side by side where two stacked 20 px buttons do not fit
  the taskbar's height.)*
- **SET-F51 [Done]** THE Settings dialog SHALL expose a **`Show quick access
  buttons and menu`** toggle (default **on**, persisted as `Show Quick Access Buttons` →
  [`persistence.spec.md`](persistence.spec.md) §9.1). WHILE it is off THE SYSTEM
  SHALL hide both buttons and give the reclaimed width to the input box; WHEN it
  changes on OK THE SYSTEM SHALL apply the new state immediately
  (`RelayoutIfTaskbarChanged`, which also recomputes the bar's width — TBR-F20),
  without a restart. The same toggle SHALL also turn the category window's
  right-click menu on and off ([`categories-phrases.spec.md`](categories-phrases.spec.md)
  CAT-F59), pushed with the other category-window toggles (SET-F22), so a
  caregiver can hide every extra control with one switch. *(Amended
  2026-10-30: renamed from `Show quick access buttons`; the registry value
  name is unchanged, so existing settings carry over.)*

### 6.7 Main window input box

- **SET-F60 [Done]** WHEN text is pasted into the main window's input box THE
  SYSTEM SHALL fold it with `NormalizePhraseText`
  ([`categories-phrases.spec.md`](categories-phrases.spec.md) CAT-F50) before
  inserting it, so a pasted document becomes one speakable line. A `WM_PASTE`
  case in `EditSubclassProc` reads `CF_UNICODETEXT` and inserts with
  `EM_REPLACESEL(bCanUndo = TRUE)`, so `Ctrl + Z` still reverses the paste;
  any other clipboard format falls through to the control untouched.
- **SET-F61 [Done]** THE input box (in the input flyout, SET-F62) SHALL be a
  **wrapping multiline** edit:
  `ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL`, **without** `ES_AUTOHSCROLL`
  (whose absence is what enables wrapping) and without `ES_WANTRETURN`
  (`EditSubclassProc` turns `VK_RETURN` into "speak"). Each bit is
  load-bearing; all were settled by measurement rather than by reading the API
  docs, which describe none of these failures.

  > *How this was arrived at (2026-09-25).* The box was `ES_MULTILINE |
  > ES_WANTRETURN | ES_AUTOHSCROLL`, and a 90,748-character paste landed **0
  > characters** — a multiline edit without `ES_AUTOVSCROLL` refuses input once
  > a line would overflow its height (1,000 accepted, 10,000 refused with
  > nothing inserted). Raising the text limit (CAT-N06) did not help, and
  > neither did folding the newlines: length alone trips it.
  >
  > A plain **single-line** edit was tried next. It accepted everything, but
  > rendered **nothing** past ~40,000–48,000 characters (measured: 40,000
  > painted, 48,000 blank), so the text was stored and speakable but invisible —
  > the same class of silent failure this spec keeps eliminating. It also
  > cannot show a scrollbar: single-line edits ignore `WS_HSCROLL`.
  >
  > A multiline edit **hard-breaks text into ~1,000-character lines** whatever
  > `ES_AUTOHSCROLL` says (30,000 characters → `EM_GETLINECOUNT` = 30), so a
  > horizontal scrollbar only ever covers one such chunk — 3% of the text — and
  > that variant additionally needs ≥46 px of height before it renders at all,
  > because the scrollbar leaves only 15 px of client height at 36 px.
  >
  > Wrapping is what makes the problem go away: every visual line is as wide as
  > the control, so nothing approaches the width where rendering gives up, at
  > any length. `WS_VSCROLL` supplies the draggable scrollbar. No cap is needed.
  >
  > *Accepted cost:* the scrollbar takes ~17 px of the box's width. Since
  > 2026-10-11 the box lives in the input flyout (SET-F62), three lines tall and
  > at least 420 px wide, so the strip's one-line view no longer applies. A
  > 30,000-character paste still wraps to ~1,800 lines, so the flyout is not a
  > place to review a long paste — the phrase dialog, multiline with its own
  > `WS_VSCROLL`, is. `updateEditAlignment` is unaffected either way: it only
  > touches `ES_LEFT`/`ES_RIGHT`/`ES_CENTER`.
- **SET-F62 [Done]** THE main window's text SHALL be edited in an **input
  flyout** (`InputFlyout`): a topmost tool pop-up owned by the bar that holds
  the SET-F60/F61 edit control for good. THE bar SHALL show, where the input box
  was, a **preview** of the text — an `SSButton` styled like an input box
  (window colors, square 1 px border, the bold bar font, left-aligned; one line
  on a horizontal taskbar, two wrapped lines from the top on a vertical one);
  clicking, dwelling on or pressing (`Space`/`Enter`) the preview SHALL open the
  flyout with the caret at the end of the text. Every change to the text —
  typing, paste, phrase clicks, the default text — SHALL refresh the preview
  (`UpdateInputPreview`; a multiline edit sends no `EN_CHANGE` for
  `WM_SETTEXT`, so the setters call it themselves). The edit's notifications,
  `Enter` and `Esc` reach the main window through the flyout. Where the flyout
  opens is owned by [`taskbar.spec.md`](taskbar.spec.md) TBR-F31.
- **SET-F63 [Done]** WHEN `Enter` is pressed in the flyout THE SYSTEM SHALL
  speak the text, close the flyout and put the keyboard focus on the bar's
  `Play` button (so `Space` repeats or stops the phrase). `Esc` SHALL close it
  without speaking and return the focus to the window that was in front before
  the user turned to SimonSays (the last other-process foreground window seen
  at a mouse press, or the foreground window when the flyout opened without a
  click), when it still exists. *(Amended 2026-10-11 at the developer's
  request: `Enter` returned the focus to the previous window too.)* WHEN the flyout loses activation (a click
  elsewhere, a dialog) it SHALL close and leave the focus where it went; a
  relayout (TBR-F40) or hiding the bar SHALL close it too. The text always
  stays in the box and the preview.

### 6.8 Non-functional

- **SET-N01 [Done]** All dialog/menu strings SHALL be localized via
  `GetLocalizedString`.
- **SET-N02 [Done]** The dialog SHALL persist only through `RegistryManager`
  (never touching the registry directly) and SHALL clamp volume/rate on commit.
- **SET-N03 [Done]** The voice preview SHALL not block the UI thread (detached
  worker; COM initialized/uninitialized within it).
- **SET-N04 [Done]** Each quick access button SHALL carry a localized window
  text naming its action, so screen readers and UI Automation announce it even
  though a centered icon suppresses the drawn label (BTN rendering rule). The
  texts reuse `IMPORT_CATEGORIES_DIALOG_TITLE_ID` and
  `SETTINGS_DIALOG_TITLE_TEXT_ID`; no new localized string is introduced for
  them, and both are refreshed (with the RTL ex-style) when the UI language
  changes (SET-F21).
- **SET-N05 [Done]** THE Settings dialog SHALL use 10 pt Segoe UI (was 8 pt)
  for readability, and SHALL stay short enough to fit a 720-px-high work area
  (a 1366×768 laptop, or 1920×1080 at 150 % — the app is DPI-unaware, so the
  logical size is what counts), and a 1280-px-wide one; hence the two-column
  layout of §11 (485 × 234 DLU, about 850 × 520 px at 96 DPI).
  *(Added 2026-10-21.)*

## 7. Architecture & components

### 7.1 Files

| File | Responsibility |
|---|---|
| [`src/SettingsDialog.cpp`](../../src/SettingsDialog.cpp) / [`include/SettingsDialog.h`](../../include/SettingsDialog.h) | The modal dialog: `ShowSettingsDialog( owner, hInstance, Settings & )` and its file-local `SettingsDialogContext`, `SettingsDialogProc`, combo/localization helpers. Same shape as the Dwell dialog (`SSDwellWindow.cpp`). |
| [`src/MainWindow.cpp`](../../src/MainWindow.cpp) | `MainWindow::ShowSettingsDialog` (re-entry guard, calls the dialog, then apply/persist); tray icon/menu; About/Web/Feedback; touch keyboard. |
| [`include/stdafx.h`](../../include/stdafx.h) | `Settings` struct; string IDs. |
| [`src/utils.cpp`](../../src/utils.cpp) | `ConfigureSlider` / `SyncSliderToEdit` / `SyncEditToSlider`; `ShowTouchKeyboard`. |
| [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp) | Load/save of the values (→ [`persistence.spec.md`](persistence.spec.md)). |

### 7.2 Flow

`MainWindow::ShowSettingsDialog` passes a copy of `m_settings` to the free
`ShowSettingsDialog` (`SettingsDialog.cpp`), which builds a
`SettingsDialogContext` (that copy, the language list, and the voice list), runs
`DialogBoxParam(IDD_SETTINGS_DIALOG)` and returns true with the edited copy on
OK; `MainWindow` then commits/persists/applies. `SettingsDialogProc` handles
`WM_INITDIALOG` (seed controls; enumerate output devices; create the Listen
tooltip), `WM_HSCROLL`/edit sync, language-combo change (repopulate voices and
the output list), Test Voice, Listen settings, Help (and `WM_HELP` for F1), and OK/Cancel.

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
// SettingsDialog.h — the dialog; true + edited `settings` on OK, untouched on Cancel
bool ShowSettingsDialog( HWND owner, HINSTANCE hInstance, Settings & settings );
// MainWindow (settings-related)
void ShowSettingsDialog();                 // guard → ::ShowSettingsDialog → apply
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

A modal dialog (`IDD_SETTINGS_DIALOG`, 10 pt, SET-N05) in two columns: left
225 DLU (the narrowest that keeps its longest translated label on one line),
right 236 DLU (the old single column's width, for the long playback labels).
**Left:** default-text edit + "use default text", touch keyboard and quick
access checks; the three category-window checkboxes; then the
**Gaze / Dwell-click...** button (SET-F18). **Right:** language combo; voice
combo + **Test Voice** button; volume and rate sliders each paired with a
numeric edit; the **Sound output** combo with the **Listen settings...**
button beside it (SET-F15/F16); then the four playback checkboxes. **Bottom row:** **Help** in the bottom-left corner (SET-F17;
bottom-right in the mirrored Arabic/Hebrew layout); `OK` / `Cancel`. The **main window** carries, to the right of the `Categories`
button, the two quick access buttons of SET-F50 (📂 open a board, ⚙️
Settings), shown unless `Show quick access buttons and menu` is off, then the text
preview that opens the input flyout (SET-F62), then `Play`. The **tray menu** lists Show/Hide · Settings · Dwell · Web ·
About · Feedback · Exit (Settings/Dwell greyed while a dialog is open; Exit asks first, SET-F33). The
**About** dialog shows version + credits. The main window and dialogs move by
dragging their title bars / bodies.

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| Settings dialog resource | `IDD_SETTINGS_DIALOG` | `resource.h` |
| Voice volume range | 10 – 100 | `SIMONSAYS_SETTINGS_MIN/MAX_VOICE_VOLUME` |
| Voice rate range | −10 … +10 | `SIMONSAYS_SETTINGS_MIN/MAX_VOICE_RATE` |
| Default-text buffer | 1024 wchar | `SettingsDialog.cpp` `SettingsDialogProc` |
| Tray callback message | `WM_TRAYICON` | [`stdafx.h`](../../include/stdafx.h) |
| Control / string IDs | `IDC_SETTINGS_*` / `SETTINGS_*_ID` | `resource.h` / `stdafx.h` |
| Virtual cable name patterns | `vb-audio`, `voicemeeter`, `virtual audio cable`, `virtual cable` | `PlaybackEngine.cpp` `VIRTUAL_CABLE_NAME_PATTERNS` |
| Short output list (build-time) | `SETTINGS_OUTPUT_DEVICE_SHORT_LIST` (commented out) | `stdafx.h` |
| Listen settings command | `control.exe mmsys.cpl,,1` (Recording tab) | `SettingsDialog.cpp` `SettingsDialogProc` |

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
- **Saved output device not connected** → listed and selected as
  `<name> (not connected)`; OK keeps it (SET-F15). Playback meanwhile uses the
  Windows default device ([`sound.spec.md`](sound.spec.md) SND-F72).
- **Core Audio enumeration fails** → the output list holds only `Windows default`.

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
- **AC-7 (SET-F60/F61) [Pass]**
  Pasting a multi-line document into the input box keeps the whole text, folds
  its line breaks, shows it, and offers a draggable scrollbar; `Ctrl + Z`
  reverses the paste. *(Verified 2026-09-25 against the shipping `Release`
  build: style reads `0x50210044` — multiline, autovscroll, no autohscroll,
  `WS_VSCROLL`; a 30,000-character paste became 29,859 folded characters over
  1,802 wrapped lines with scrollbar range 0–1801, and a screen capture of the
  control shows the text rather than a blank box. The `WM_PASTE` fold itself was
  verified separately on a 90,748-character clipboard: 301 line breaks in, 0
  out, byte-identical to `NormalizePhraseText`, `EM_CANUNDO` true.)*
  *(Look and feel confirmed by the developer inside the input flyout,
  2026-10-11.)*
- **AC-8 (SET-F62/F63, SET-F40) [Pass]** The flyout opens from the preview
  by click, by dwell and by `Tab` + `Space`; typing, paste folding and
  `Ctrl + Z` work in it; `Enter` speaks, closes it and leaves the focus on
  `Play`; `Esc` closes without speaking and returns the focus to the app you
  were in; a click elsewhere closes it; the preview follows typing and phrase
  clicks; the touch keyboard (when enabled) opens beside the flyout. AC-7 is
  re-checked inside the flyout. *(Verified manually by the developer 2026-10-11,
  including the `Enter` → `Play` focus.)*
- **AC-6 (SET-F50/F51/N04) [Pass]** With the setting on, both quick access
  buttons show next to `Categories`; 📂 opens the import dialog in the boards
  folder and ⚙️ opens Settings. Unticking `Show quick access buttons and menu` and
  pressing OK hides both and widens the input box without a restart; the state
  survives a restart. Neither button draws a label, and both report their
  localized name to UI Automation. *(Manually verified on `Release\SimonSays.exe`,
  2026-09-20.)*
- **AC-9 (SET-F15) [Pass]** The device list puts virtual cables first:
  `CABLE Input (VB-Audio Virtual Cable)` (flagged virtual) came before
  `Speakers (Realtek…)`, although Windows lists the speakers first, and each
  maps to its waveOut device. The default, `SETTINGS_OUTPUT_DEVICE_SHORT_LIST`
  and `USE_MCI_FOR_MP3` builds compile clean. *(Verified 2026-10-13 with a
  harness built from the engine's own code.)* The combo, the
  `(not connected)` entry, relabelling on a language change and the short list
  in the running dialog were confirmed by the developer in-app, 2026-10-14.
- **AC-10 (SET-F16) [Pass]** The **Listen settings...** command opens the Sound
  control panel with tab 1 of 4 (**Recording**) selected. *(Verified
  2026-10-13 by reading the dialog's tab control; the tooltip itself was not
  checked visually.)*
- **AC-11 (SET-F17) [Pass]** The **Help** button shows in the bottom-left
  corner (bottom-right in Arabic/Hebrew) with the localized label and opens the
  help page; after switching the Language list to Spanish (without pressing
  OK), Help and `F1` both open the Spanish page and the label reads *Ayuda*;
  Settings stays open with its unsaved changes. *(Verified manually
  on `Release\SimonSays.exe`, 2026-10-14.)*
- **AC-12 (SET-F01/N05) [Pass]** The dialog opens at 10 pt in two columns
  as in §11 and fits a 1280×720 work area; Sound output + Listen settings sit
  directly below Rate; no label is clipped in English, German, Russian,
  Basque, or the mirrored Arabic layout; Tab order runs down the left column,
  then the right, then Help / OK / Cancel. *(Verified manually on
  `Release\SimonSays.exe`, 2026-10-21.)*
- **AC-13 (SET-F18) [Pass]** **Gaze / Dwell-click...** opens the dwell
  window over Settings (Settings is disabled until it closes) with the
  localized label. Change the dwell time, press its OK, then press **Cancel**
  in Settings: the new dwell time is kept (registry and the F3 window). Repeat
  with a Settings edit and Settings **OK**: both the Settings edit and the
  dwell change are kept. *(Verified manually on `Release\SimonSays.exe`,
  2026-10-21.)*
- **AC-14 (SET-F33) [Pass]** Exit from the tray menu and from the
  category window menu each ask; No (also the default, `Enter`) keeps
  SimonSays running, Yes exits; choosing Exit again while the question is
  open does not open a second one. *(Driven on `Release\SimonSays.exe`
  2026-10-30: the menu's Exit opened "¿Salir de SimonSays?" with Sí / No;
  a second `ID_TRAY_EXIT` posted while it was open left one box; `Enter`
  closed it and SimonSays kept running; `ID_TRAY_EXIT` again (the tray
  menu's command) → Sí exited, likewise in Arabic.)*

Build gate: Debug **and** Release Win32 compile clean (no code change in this
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
| Exit confirmation | ✅ Done | SET-F33; tray + category window menu; AC-14 driven in-app 2026-10-30 |
| Quick access buttons + toggle | ✅ Done | SET-F50/F51; live apply via `RelayoutIfTaskbarChanged`; side-by-side layout on thin/side taskbars verified 2026-10-11 (taskbar.spec.md AC-9/AC-11) |
| Input box: wrapping + folded paste | ✅ Done | SET-F60/F61; measured in the shipping build 2026-09-25 (style, wrap, scrollbar, visible text); look and feel confirmed by the developer inside the input flyout 2026-10-11 (AC-7) |
| Input flyout + bar preview | ✅ Done | SET-F62/F63; AC-8 verified 2026-10-11 |
| Window/dialog move | ✅ Done | drag |
| Sound output device + Listen settings | ✅ Done | SET-F15/F16; engine routing verified with a real VB-Cable signal test (sound.spec.md AC-9); dialog confirmed in-app 2026-10-14 (AC-9) |
| Settings Help button + F1 | ✅ Done | SET-F17; 18 languages; AC-11 verified manually 2026-10-14 |
| 10 pt two-column layout | ✅ Done | SET-F01/N05; AC-12 verified manually 2026-10-21 |
| Gaze / Dwell-click... button | ✅ Done | SET-F18; reuses `TRAYICON_DWELL_ID`; AC-13 verified manually 2026-10-21 |

## 17. Known limitations

- One flat dialog (no tabs/search) for all general settings.
- The Dwell settings live in a **separate** dialog (`F3`), not this one.
- Voice list depends on installed SAPI voices; none may match a language.
- SimonSays cannot turn on **Listen to this device** for a virtual cable; the
  **Listen settings...** button only opens the Windows dialog where the user
  does it (SET-F16). Windows cannot open one device's Properties directly, so
  the user still selects the cable on the Recording tab.
- Opening the flyout by HID dwell (no mouse click) may not bring it to the
  front, because Windows only lets the app that received the last input take
  the foreground; the flyout then shows without keyboard focus.
- Characters typed while the preview has keyboard focus do nothing until the
  flyout is open. The preview's accessible name is the text itself, so an
  empty box has an empty name.

## 18. Future work

- Grouped/tabbed settings as the option set grows.
- Live preview of playback toggles.

## 19. Open questions

1. Should Settings and Dwell be merged into one multi-page dialog?

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of SET-SPEC v1.0.*
