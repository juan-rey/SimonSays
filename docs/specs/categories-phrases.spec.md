# SimonSays — Categories & Phrases Management — Specification

| | |
|---|---|
| **Spec ID** | CAT-SPEC |
| **Status** | Active — reverse-engineered from shipping source (2026-07-10); PNG/JPG file icons added 2026-07-11 |
| **Version** | 1.1 (2026-07-11) |
| **REQ prefix** | `CAT-F##` (functional), `CAT-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/CategoryWindow.cpp`](../../src/CategoryWindow.cpp), [`include/CategoryWindow.h`](../../include/CategoryWindow.h), [`src/utils.cpp`](../../src/utils.cpp) (serialization), [`include/stdafx.h`](../../include/stdafx.h) (model) |
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
conventions. This file is the source of truth for the **Categories window and
category/phrase management**, including the `##` icon prefix and `::` inline-audio
suffix authoring conventions. Reference format: [`docs/specs/dwell.spec.md`](dwell.spec.md).

This spec was authored **from the existing code**, so per the working agreement
the code is authoritative for current behavior (rule 7): if a fact here drifts
from [`src/CategoryWindow.cpp`](../../src/CategoryWindow.cpp), fix the spec and
flag it.

> Scope reminder: browsing, creating, editing, deleting, and ordering categories
> and phrases; icon specification (`##`); audio-file suffix (`::`); the
> immediate-speak-on-select option; the Categories window shell (zoom, resize,
> hide). Locating/playing the resulting audio and icon files is in
> [`sound.spec.md`](sound.spec.md). Import/export of categories is in
> [`import-export.spec.md`](import-export.spec.md). Registry storage of phrases
> is in [`persistence.spec.md`](persistence.spec.md). The Settings toggles that
> affect behavior are in [`settings.spec.md`](settings.spec.md). Styling of the
> window and its buttons — including the reserved `$$board` category name and
> the `::` **style** suffix on category names — is in
> [`board-style.spec.md`](board-style.spec.md). The button control is
> [`ssbutton.spec.md`](ssbutton.spec.md); text-to-speech is [`tts.spec.md`](tts.spec.md).

---

## 1. Overview

The **Categories window** is a floating grid, opened from the main window's
`Categories` button, that gives a couple-of-clicks path to frequently used
phrases. It shows two grids of [`SSButton`](ssbutton.spec.md)s: **categories**
across the top and, below a labelled separator strip, the **phrases** of the
currently selected category. Selecting a category swaps the phrase grid;
selecting a phrase loads its text into the main input box (and optionally speaks
it immediately). Categories and phrases are user-editable in place (add / edit /
move / delete) via F-key shortcuts, and each may carry an icon (emoji or an
image file — `.ico`/`.png`/`.jpg`); phrases may additionally carry an inline
audio file.

## 2. Background & context

- The window is a frameless popup that lives above the taskbar and is dismissed
  as soon as focus leaves it (optional), so a caregiver/user can pick a phrase
  and return to their task without app-switching.
- Categories and phrases are held in memory as `std::vector<Category>` (each
  `Category` owning a `std::vector<Phrase>`) and serialized to the per-language
  registry after every edit; the on-disk/registry formats are shared with the
  `.ssc`/`.ssz` import-export carriers.
- Authoring conventions are terse text markers so the whole category/phrase —
  icon, text, and audio — can be typed in a single edit field: `##` separates an
  icon from the name/text, `::` appends an audio file to a phrase, and (per
  [`board-style.spec.md`](board-style.spec.md)) `::` appends a **style** to a
  category. Supported file-based icon formats: `.ico`, `.png`, `.jpg`/`.jpeg`
  (rendering owned by [`ssbutton.spec.md`](ssbutton.spec.md)).

## 3. Goals & non-goals

**Goals**
- Fast browse-and-speak: category → phrase in two activations, keyboard- and
  dwell-navigable.
- In-place CRUD and reordering of categories and phrases with immediate
  persistence.
- Single-field authoring of icon + text + audio via `##` / `::` markers.
- A window shell that reflows to width, zooms, remembers its size, and hides on
  focus loss.

**Non-goals**
- Icon/emoji **rendering** and audio **playback/lookup** (owned by
  [`ssbutton.spec.md`](ssbutton.spec.md) and [`sound.spec.md`](sound.spec.md)).
- Registry storage mechanism (owned by [`persistence.spec.md`](persistence.spec.md)).
- Import/export and bundling (owned by [`import-export.spec.md`](import-export.spec.md)).
- Board/category **styling** and the `$$board` / category `::`-style conventions
  (owned by [`board-style.spec.md`](board-style.spec.md)).

## 4. Glossary

| Term | Meaning |
|---|---|
| **Category** | A named group of phrases with an optional icon (`Category{ name, icon, style, phrases }`). |
| **Phrase** | A saved unit `Phrase{ text, icon, audioFile }` the user can speak, optionally with an icon and inline audio. |
| **`##` icon prefix** | `<icon>##<name-or-text>` — separates an icon (emoji or an `.ico`/`.png`/`.jpg` path) from the following name/text. |
| **`::` audio suffix** | On a **phrase**, `<text>::<audio file>` — appends an inline audio file (see [`sound.spec.md`](sound.spec.md)). On a **category** the same `::` appends a style (see [`board-style.spec.md`](board-style.spec.md)). |
| **Sound marker** | The `♫` (`SOUND_NOTE_DELIMITER`) wrapper shown on a phrase button / loaded into the input box for an audio phrase. |
| **Selected category / phrase** | The category whose phrases are shown (`m_selectedCategoryIndex`) and the last phrase acted on (`m_selectedPhraseIndex`); `m_categorySelectedLast` records which of the two was touched last. |

## 5. Personas & scenarios

- **Browse & speak:** open Categories, click a category, click a phrase → the
  phrase text loads in the input box and (if the immediate-speak setting is on)
  is spoken at once.
- **Add a phrase with a sound:** select a phrase, press `F7`, type
  `👍##Well done!::applause.wav` → a new phrase with a thumbs-up icon that shows
  `♫Well done!♫` and plays `applause.wav`.
- **Rename & re-icon a category:** select a category, press `F4`, edit to
  `🍎##Food` → the button relabels and shows the apple emoji.
- **Reorder:** press `F5`/`F6` to move the selected category or phrase earlier/
  later; the change is saved immediately.

## 6. Requirements (EARS)

Status tags per [`docs/spec.md`](../spec.md) §2.3. All requirements below are
implemented in the current source and tagged **[Done]** accordingly.

### 6.1 Browsing & selection

- **CAT-F01 [Done]** THE SYSTEM SHALL render categories as a top grid and, below
  a labelled separator strip, the phrases of the selected category as a bottom
  grid, both reflowing to the window width (columns computed from button width +
  margin) and mirrored when the UI language is RTL.
- **CAT-F02 [Done]** WHEN a category button is activated THE SYSTEM SHALL make it
  the selected category, rebuild the phrase grid from its phrases, mark it
  selected (bold font + `BS_FLAT`), and move focus to it.
- **CAT-F03 [Done]** WHEN a phrase button is activated THE SYSTEM SHALL load the
  phrase into the main input box — the plain `text` for a text phrase, or the
  `♫<audioFile>♫` marker for an audio phrase — and, IF the immediate-speak
  setting (`speakDirectlyWhenClickingPhrase`) is enabled, THEN speak it at once
  (see [`settings.spec.md`](settings.spec.md) / [`tts.spec.md`](tts.spec.md) /
  [`sound.spec.md`](sound.spec.md)).
- **CAT-F04 [Done]** THE SYSTEM SHALL restore the previously selected category on
  open and persist the selected-category index on close (via `RegistryManager`;
  see [`persistence.spec.md`](persistence.spec.md)); an out-of-range saved index
  falls back to the first category.

### 6.2 Category management

- **CAT-F10 [Done]** WHEN `F7` is pressed with a category context THE SYSTEM SHALL
  open the Add dialog and insert the new category **after** the selection (or at
  the end), seeded with one default phrase (`NEW_PHRASE_DEFAULT_TEXT_ID`), then
  select it.
- **CAT-F11 [Done]** WHEN `F4` is pressed with a category context THE SYSTEM SHALL
  open the Edit dialog pre-filled with `<icon>##<name>::<style>` and, on OK,
  apply the parsed name/icon/style to the category and its button.
- **CAT-F12 [Done]** WHEN `F8` is pressed with a category context THE SYSTEM SHALL
  prompt for confirmation and, on Yes, delete the category **and all its
  phrases**, then select the first remaining category (if any).
- **CAT-F13 [Done]** WHEN `F5` / `F6` is pressed with a category context THE
  SYSTEM SHALL move the selected category to the previous / next position
  (swapping neighbours; direction mirrored under RTL), keeping it selected.
- **CAT-F14 [Done]** IF an add/edit would create a **duplicate** category name,
  or a name beginning with the reserved `$$` prefix (see
  [`board-style.spec.md`](board-style.spec.md)), THEN THE SYSTEM SHALL reject it
  with a localized name-conflict message and leave the model unchanged.

### 6.3 Phrase management

- **CAT-F20 [Done]** WHEN `F7` is pressed with a phrase context THE SYSTEM SHALL
  open the Add dialog and insert the new phrase **after** the selected phrase (or
  at the end of the category), then select it.
- **CAT-F21 [Done]** WHEN `F4` is pressed with a phrase context THE SYSTEM SHALL
  open the Edit dialog pre-filled with `<icon>##<text>::<audio file>` and, on OK,
  apply the parsed icon/text/audio to the phrase and its button.
- **CAT-F22 [Done]** WHEN `F8` is pressed with a phrase context THE SYSTEM SHALL
  prompt for confirmation and, on Yes, delete the selected phrase.
- **CAT-F23 [Done]** WHEN `F5` / `F6` is pressed with a phrase context THE SYSTEM
  SHALL move the selected phrase to the previous / next position within its
  category (direction mirrored under RTL).
- **CAT-F24 [Done]** WHICH of category vs phrase the F-keys act on is decided by
  the last-touched selection (`m_categorySelectedLast`): selecting a category
  targets category ops; selecting/adding a phrase targets phrase ops.

### 6.4 Icon & audio authoring conventions

- **CAT-F30 [Done]** THE `##` prefix SHALL be parsed by first-occurrence split:
  the part before `##` is the icon, the part after is the name/text; with no
  `##`, the whole string is the name/text and there is no icon.
- **CAT-F31 [Done]** WHEN resolving an icon string THE SYSTEM SHALL treat a value
  **containing a `.`** as a file path — rendering it only IF it ends
  (case-insensitive) in a supported icon extension (`.ico`/`.png`/`.jpg`/`.jpeg`,
  shared predicate `HasSupportedIconExt` in `utils`; resolved against the icon
  search folders, see [`sound.spec.md`](sound.spec.md)), otherwise showing **no
  icon** — and a value with **no `.`** as an emoji. *(Amended 2026-07-11: was a
  "contains `.ico`" substring check; now an ends-with check over the supported
  formats — a non-matching dotted value still shows no icon.)*
- **CAT-F32 [Done]** ON a **phrase**, a `::` suffix SHALL set the phrase's
  `audioFile`; the phrase button label SHALL then show `♫<text>♫` and selecting
  it SHALL load `♫<audioFile>♫` into the input box (CAT-F03). The `##` prefix and
  `::` suffix are optional and independent — any combination of icon / text /
  audio is valid.
- **CAT-F33 [Done]** ON a **category**, a `::` suffix SHALL be interpreted as a
  **style** (not audio) and routed to [`board-style.spec.md`](board-style.spec.md);
  category serialization keeps the style out of the stored value **name**.

### 6.5 Display & window behaviour

- **CAT-F40 [Done]** THE SYSTEM SHALL zoom the grids via `Ctrl +`/`Ctrl -`/
  `Ctrl 0` and `F12`/`F11`, clamped to `[MIN_ZOOM_FACTOR, MAX_ZOOM_FACTOR]`; when
  "remember size" is on, the zoom is persisted (clamped to the saved range) and
  restored (see [`persistence.spec.md`](persistence.spec.md) /
  [`settings.spec.md`](settings.spec.md)).
- **CAT-F41 [Done]** THE window SHALL be a resizable frameless popup with a
  minimum size (one category button + margins), draggable from its client area
  (`WM_NCHITTEST` → `HTCAPTION`), reflowing its grids on resize; its size is
  remembered when the setting is on.
- **CAT-F42 [Done]** THE SYSTEM SHALL hide the window (not destroy it) on `Esc`,
  on `WM_CLOSE`, and — WHEN the minimize-on-focus-loss setting is on — when it is
  deactivated to another process's window, notifying the main window each time.

### 6.6 Non-functional

- **CAT-N01 [Done]** Every clickable surface SHALL be an
  [`SSButton`](ssbutton.spec.md); category buttons carry command ids `1000+i`,
  phrase buttons `2000+i`, routed in `WM_COMMAND`.
- **CAT-N02 [Done]** All user-visible strings SHALL be localized via
  `GetLocalizedString` and F-keys SHALL reach the window even when a button has
  focus (SSButton forwards `VK_F1..VK_F24` to the parent — [`ssbutton.spec.md`](ssbutton.spec.md) BTN-F71).
- **CAT-N03 [Done]** Every model edit SHALL be persisted immediately via
  `RegistryManager::SaveCategoriesToRegistry(..., clearExisting=true, boardStyle)`
  (mechanism owned by [`persistence.spec.md`](persistence.spec.md)).

## 7. Architecture & components

### 7.1 Files

| File | Responsibility |
|---|---|
| [`include/CategoryWindow.h`](../../include/CategoryWindow.h) | `CategoryWindow` class, layout metrics, member state. |
| [`src/CategoryWindow.cpp`](../../src/CategoryWindow.cpp) | Window proc, grid build/layout, selection, CRUD + reorder, edit dialog, zoom, icon application, import/export entry points. |
| [`src/utils.cpp`](../../src/utils.cpp) | `Serialize`/`Deserialize` of categories & phrases, `PhraseToButtonText`, `SerializeCategoryWithStyle` / `ParseCategoryData`. |
| [`include/stdafx.h`](../../include/stdafx.h) | `Category` / `Phrase` structs and the separator constants. |
| [`src/MainWindow.cpp`](../../src/MainWindow.cpp) | Hosts the window, receives `SetEditControlText`, auto-speaks on select. |

### 7.2 Structure

- **`CategoryWindow`** owns the popup `HWND`, the two `std::vector<SSButton>`
  grids, the in-memory `std::vector<Category>`, selection indices, per-role fonts,
  and the zoom factor. It is created by and calls back into `MainWindow`.
- **Layout** is centralized in `LayoutCalcs()` (columns, margins, separator/strip
  geometry) and applied by `RefreshLayout()` / `CreateCategoryButtons()` /
  `CreatePhraseButtons()`.
- **Serialization helpers** in `utils.cpp` are shared with the registry and the
  `.ssc`/`.ssz` carriers, keeping the marker grammar in one place.

### 7.3 Dependency / layering

- Depends on `SSButton`, `RegistryManager`, `utils`, `BoardStyle`, and localization.
- Delegates: playback/lookup → `sound`; storage → `persistence`; bundling →
  `import-export`; styling → `board-style`.

## 8. Detailed design

### 8.1 Marker grammar (single source in `stdafx.h`)

| Marker | Const | Meaning |
|---|---|---|
| `##` | `ICON_SEPARATOR` | icon ⇢ name/text separator (first occurrence) |
| `::` | `AUDIO_FILE_SEPARATOR` | phrase → audio suffix; category → style suffix |
| `\|` | `CATEGORY_PHRASE_SEPARATOR` | joins phrases within a category's stored data |
| `♫` | `SOUND_NOTE_DELIMITER` | wraps an audio reference on a button / in the input box |

`DeserializePhrase` splits `<icon>##<text>::<audio>`: `##` first (icon vs rest),
then `::` (text vs audioFile). `SerializePhrase` reverses it. `PhraseToButtonText`
returns `text` for a text phrase and `♫<text>♫` for an audio phrase.
`DeserializeCategory` splits `<icon>##<name>::<style>`; `SerializeCategory` (the
value **name**) omits the style, while `SerializeCategoryWithStyle` (the edit
field) includes it.

### 8.2 Selection & F-key routing

`m_categorySelectedLast` records whether a category or a phrase was last touched;
the F-key handlers branch on it so the same F4/F5/F6/F7/F8 serve both contexts.
Edit/Add/Move/Delete each re-serialize and call `SaveCategoriesToRegistry`, and
temporarily disable minimize-on-focus-loss while a modal dialog/prompt is open.

### 8.3 Icon application

`SetSSButtonIcon` (file-scope helper) applies an icon string to a button: a `.`
means a file (rendered only for a supported extension — `HasSupportedIconExt`:
`.ico`/`.png`/`.jpg`/`.jpeg` — resolved against the icon folders; else
`NoIcon()`); no `.` means an emoji. Icons are applied **after** button creation
(`UpdateButtonIcons` / `UpdatePhraseButtonIcons`) to avoid a paint delay while
the grid is being built.

## 9. Data model & persistence

- `Phrase{ std::wstring text, icon, audioFile }` and
  `Category{ std::wstring name, icon, style; std::vector<Phrase> phrases }`
  ([`stdafx.h`](../../include/stdafx.h)). `Category::style` is owned by
  [`board-style.spec.md`](board-style.spec.md).
- Categories/phrases persist per language in the registry; the storage rules,
  value layout, first-run defaults, and the non-overwrite policy are owned by
  [`persistence.spec.md`](persistence.spec.md). This module edits the in-memory
  model and requests a save after each change; it does not own the storage.

## 10. Key interfaces

```cpp
// CategoryWindow (host-facing subset)
void UpdateCategories( const std::vector<Category> & categories,
                       std::wstring language, int selectedCategory = -1,
                       const std::wstring & boardStyle = L"" );
void Show(); void Hide(); bool IsVisible();
void EditLastSelection(); void AddAfterSelection();
void MoveSelection( int delta );        // -1 previous / +1 next
void DeleteLastSelection();
void ImportCategories( std::wstring filePath = L"" );   // → import-export
void ExportCategories();                                // → import-export
void ZoomIn(); void ZoomOut(); void ResetZoom();

// utils.cpp — marker (de)serialization
std::wstring SerializeCategory( const Category & );            // "<icon>##<name>" (value name)
std::wstring SerializeCategoryWithStyle( const Category & );   // + "::<style>" (edit field)
Category     DeserializeCategory( const std::wstring & );
std::wstring SerializePhrase( const Phrase & );               // "<icon>##<text>::<audio>"
Phrase       DeserializePhrase( const std::wstring & );
std::wstring PhraseToButtonText( const Phrase & );            // text or ♫text♫
```

## 11. UI specification

A frameless, layered, top-most popup (`WS_POPUP | WS_THICKFRAME`,
`WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW`), DWM-rounded, tinted to the
taskbar/board background, positioned above the main window. Top→bottom: the
**category grid**, a **separator strip** with a centered hint/caption
(shortcuts hint by default; a board `caption` overrides it — see
[`board-style.spec.md`](board-style.spec.md)), then the **phrase grid** of the
selected category. Add/Edit share one dialog (`IDD_EDIT_DIALOG`) whose title/
labels switch between category and phrase; Delete/overwrite use localized
Yes/No message boxes. The window is shown only after the grid is populated (to
avoid an empty-window flash).

### 11.1 Accelerators (handled in `CategoryWindow::WindowProc`)

| Key | Action |
|---|---|
| `F4` | Edit selection (CAT-F11 / F21) |
| `F5` / `F6` | Move selection previous / next (CAT-F13 / F23) |
| `F7` | Add after selection (CAT-F10 / F20) |
| `F8` | Delete selection (CAT-F12 / F22) |
| `F9` / `F10` | Import / Export → [`import-export.spec.md`](import-export.spec.md) |
| `F11` / `F12` | Zoom out / in |
| `Ctrl +` / `Ctrl -` / `Ctrl 0` | Zoom in / out / reset |
| `Esc` | Hide the window |

(The main window owns `F1`/`F2`/`F3` via `IDR_MAINACCEL`; F-keys reach this
window from focused buttons through SSButton's forwarding, BTN-F71.)

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| Category/phrase button width · height · margin | 120 · 40 · 10 px | [`CategoryWindow.h`](../../include/CategoryWindow.h) |
| Default window height | 494 px | `CategoryWindow.h` `m_default_window_height` |
| Zoom min · max · step | 0.5 · 2.0 · 0.1 | `CategoryWindow.cpp` |
| Saved-zoom clamp | 0.8 – 1.5 | `CategoryWindow.cpp` |
| Layered-window alpha | 239 | `CategoryWindow.cpp` `SetLayeredWindowAttributes` |
| Command id ranges | categories `1000+i`, phrases `2000+i` | `CategoryWindow.cpp` |
| Edit dialog text buffer | 1024 wchar | `CategoryWindow.cpp` `EditDialogProc` |
| Icon search folders | AppData\SimonSays, working dir, exe dir | `CategoryWindow.cpp` ctor (→ [`sound.spec.md`](sound.spec.md)) |
| Markers | `##` `::` `\|` `♫` | [`stdafx.h`](../../include/stdafx.h) |

## 13. Diagnostics

N/A — no diagnostic dump of its own (SSZ import notes go to `OutputDebugString`;
see [`import-export.spec.md`](import-export.spec.md)).

## 14. Edge cases & error handling

- Empty selection / out-of-range indices: ops guard on `m_selectedCategoryIndex`
  / `m_selectedPhraseIndex` and no-op when invalid.
- Duplicate or `$$`-prefixed category name on add/edit → rejected (CAT-F14).
- Deleting the last category leaves the phrase grid empty until a category is
  re-selected.
- Icon file that is missing or not a supported format (`.ico`/`.png`/`.jpg`) →
  no icon shown (CAT-F31); a corrupt image file also degrades to no icon
  ([`ssbutton.spec.md`](ssbutton.spec.md) BTN-F14).
- Category name containing `::` is split at the first `::` as a style suffix — a
  pre-existing ambiguity noted in [`board-style.spec.md`](board-style.spec.md) §14.
- Zoom is reset to 1.0 on `UpdateCategories` when categories are (re)loaded and
  the prior zoom ≠ 1.0, to avoid an unpleasant scale after a data change.
- Very large phrase sets are bounded by the registry value buffer — see the
  limitation in [`persistence.spec.md`](persistence.spec.md).

## 15. Acceptance criteria (testable)

Reverse-engineered from shipping behavior; **[Pass]** reflects the code path.

- **AC-1 (CAT-F01/F02) [Pass]** Categories render on top, phrases below; clicking
  a category swaps the phrase grid and marks the category selected.
- **AC-2 (CAT-F03) [Pass]** Clicking a text phrase loads its text into the input
  box; an audio phrase loads `♫<file>♫`; with immediate-speak on, it speaks at once.
- **AC-3 (CAT-F10/F12/F14) [Pass]** F7 adds a category (seeded with a default
  phrase) after the selection; F8 deletes it with its phrases after confirmation;
  a duplicate / `$$` name is rejected.
- **AC-4 (CAT-F20/F21/F22) [Pass]** F7/F4/F8 add/edit/delete a phrase in the
  selected category, with the edit field showing `<icon>##<text>::<audio>`.
- **AC-5 (CAT-F13/F23) [Pass]** F5/F6 reorder the selected category or phrase and
  the change persists; direction mirrors under RTL.
- **AC-6 (CAT-F30–F32) [Pass]** `👍##Hi::a.wav` yields icon 👍, button label
  `♫Hi♫`, audio `a.wav`; an unsupported dotted icon (e.g. `.gif`) shows nothing;
  an emoji icon (no dot) renders.
- **AC-9 (CAT-F31) [Pending]** `smile.png##Greetings` on a category (and a
  `.png`/`.jpg` icon on a phrase) renders the image on the button, resolved via
  the icon search folders — same lookup as `.ico`. *(Manual check on
  `x64\Release`.)*
- **AC-7 (CAT-F33) [Pass]** `X##Name::background:#FFD966;` on a category sets its
  **style** (not audio) and the stored value name stays style-free.
- **AC-8 (CAT-F40/F41/F42) [Pass]** Ctrl +/–/0 and F11/F12 zoom within clamps;
  the window reflows on resize, drags from its body, and hides on Esc / focus loss.

Build gate: Debug **and** Release x64 compile clean (no code change in this
authoring pass).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Browse categories/phrases | ✅ Done | Two grids, width-reflow, RTL mirror |
| Category CRUD + reorder | ✅ Done | F7/F4/F8/F5/F6; dup + `$$` rejection |
| Phrase CRUD + reorder | ✅ Done | F7/F4/F8/F5/F6 |
| `##` icon parsing | ✅ Done | emoji vs file (`.ico`/`.png`/`.jpg`); folder lookup |
| `::` inline-audio parsing | ✅ Done | phrase audio; `♫text♫` label |
| Category `::` style suffix | ✅ Done | routed to board-style |
| Immediate-speak-on-select | ✅ Done | via `SetEditControlText` |
| Zoom / resize / hide shell | ✅ Done | clamps, remember-size, focus-loss hide |

## 17. Known limitations

- File-based icons are limited to `.ico`/`.png`/`.jpg`/`.jpeg`; other dotted
  values show no icon (`.svg` is not supported — [`ssbutton.spec.md`](ssbutton.spec.md) §17/§18).
- Category names starting with `$$`, or containing `::`, are reserved/ambiguous
  (see [`board-style.spec.md`](board-style.spec.md)).
- Per-category serialized data is bounded by the registry value buffer
  ([`persistence.spec.md`](persistence.spec.md)).
- Add/Edit use a single-line text field (no dedicated icon/audio pickers).

## 18. Future work

- A richer Add/Edit dialog with separate icon/audio pickers.
- Drag-and-drop reordering as an alternative to F5/F6.
- Board-style authoring UI (tracked in [`board-style.spec.md`](board-style.spec.md) §18).

## 19. Open questions

1. Should deleting the last category auto-create a placeholder, or is the empty
   phrase grid acceptable (current behavior)?
2. Should the category-name `::` ambiguity be disambiguated (e.g. an escape) or
   left documented?

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of CAT-SPEC v1.1.*
