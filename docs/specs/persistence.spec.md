# SimonSays — Settings / Registry Persistence — Specification

| | |
|---|---|
| **Spec ID** | REG-SPEC |
| **Status** | Active — reverse-engineered from shipping source (2026-07-10) |
| **Version** | 1.0 (2026-07-10) |
| **REQ prefix** | `REG-F##` (functional), `REG-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp), [`include/RegistryManager.h`](../../include/RegistryManager.h), `Settings` in [`include/stdafx.h`](../../include/stdafx.h) |
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
conventions. This file is the **cross-cutting** source of truth for **how**
SimonSays stores settings, phrases, and session state in the Windows registry
(HKCU), including first-run defaults, the non-overwrite policy, and the
load/save lifecycle. Reference format: [`docs/specs/dwell.spec.md`](dwell.spec.md).

This spec was authored **from the existing code**, so per the working agreement
the code is authoritative for current behavior (rule 7): if a fact here drifts
from [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp), fix the spec and
flag it.

> Scope reminder: this spec owns the **storage mechanism** and the concrete
> key/value layout. **What** each value *means* is owned by the module that
> uses it — settings fields → [`settings.spec.md`](settings.spec.md); voice/
> volume/rate → [`tts.spec.md`](tts.spec.md); dwell values →
> [`dwell.spec.md`](dwell.spec.md) §9; category/phrase model →
> [`categories-phrases.spec.md`](categories-phrases.spec.md); the `$$board` /
> `$$` style tokens inside phrase data → [`board-style.spec.md`](board-style.spec.md).
> Import/export carriers (`.ssc`/`.ssz`) are in
> [`import-export.spec.md`](import-export.spec.md).

---

## 1. Overview

All persistent state lives per-user under **`HKCU\SOFTWARE\SimonSays`** as
`REG_SZ` values, in three subkeys:

- **`\Settings`** — the app's `Settings` (language, voice/volume/rate, playback
  and category-window toggles, and the gaze dwell values).
- **`\Phrases\<Language>`** — the categories & phrases for each language, one
  value per category.
- **`\LastRun`** — session/run state: the category window size, selected
  category, zoom factor, and run/version counters.

Defaults are created on first run (when a key is absent) and are **not**
overwritten thereafter. `RegistryManager` is the sole owner of registry access;
no other module reads or writes the registry directly.

## 2. Background & context

- **HKCU only** — everything is per-user and needs no elevation; a
  system-wide/`HKLM` install would require admin and is out of scope.
- **All strings** — every value is stored as `REG_SZ`; booleans are `"1"`/`"0"`,
  numbers are decimal strings, colors are decimal `DWORD` strings, the window
  size is `"<w>x<h>"`.
- **Single-sourced** — value names and default values are `#define`d once in
  `RegistryManager.cpp` (dwell numeric defaults come from `DWELL_DEFAULT_*` in
  [`stdafx.h`](../../include/stdafx.h) so the Dwell window's Reset uses the same
  numbers).
- **Non-overwrite** — updating the app never clobbers a user's existing phrases;
  defaults are (re)installed only when a key is missing (delete a language key to
  force a re-install).

## 3. Goals & non-goals

**Goals**
- One owner (`RegistryManager`) for all registry I/O, with a stable key layout.
- First-run defaults for settings and per-language phrase sets.
- Preserve user data across app updates (non-overwrite).
- Benign fallback to defaults for missing keys.

**Non-goals**
- The **meaning** of individual values (owned by the consuming module specs).
- The `.ssc`/`.ssz` file formats (owned by [`import-export.spec.md`](import-export.spec.md)).
- Roaming/sync or cross-user storage.

## 4. Glossary

| Term | Meaning |
|---|---|
| **HKCU** | `HKEY_CURRENT_USER` — the per-user registry hive (no admin needed). |
| **Settings struct** | In-memory [`Settings`](../../include/stdafx.h) mirrored to `\Settings` values. |
| **Default phrase set** | Per-language categories/phrases from `DEFAULT_FREQUENT_PHRASES_CATEGORIES_ALL_LANGUAGES`, installed on first run. |
| **LastRun** | The `\LastRun` subkey holding non-Settings session/run state. |

## 5. Personas & scenarios

- **First run:** no keys exist → default settings and the system-language phrase
  set are created; the app opens ready to use.
- **App update:** a new version keeps all existing phrases and settings; only the
  `\LastRun` version counters change.
- **Re-install one language's phrases:** delete `HKCU\SOFTWARE\SimonSays\Phrases\<Language>`
  → next load recreates that language's defaults.
- **Settings round-trip:** change voice/volume in Settings → OK writes `\Settings`
  → next launch loads them.

## 6. Requirements (EARS)

Status tags per [`docs/spec.md`](../spec.md) §2.3. All requirements below are
implemented in the current source and tagged **[Done]** accordingly.

### 6.1 Storage location & format

- **REG-F01 [Done]** THE SYSTEM SHALL store all persistent state under
  `HKCU\SOFTWARE\SimonSays` in three subkeys — `\Settings`, `\Phrases\<Language>`,
  and `\LastRun` — as `REG_SZ` values (booleans `"1"`/`"0"`, numbers as decimal
  strings, window size as `"<w>x<h>"`).
- **REG-F02 [Done]** THE SYSTEM SHALL route **all** registry access through
  `RegistryManager`; value names and default values SHALL be single-sourced as
  `#define`s (dwell numeric defaults from `DWELL_DEFAULT_*`).

### 6.2 First-run defaults

- **REG-F10 [Done]** WHEN `\Settings` is absent on settings load THE SYSTEM SHALL
  create it via `InstallDefaultSettings()` with the documented default values,
  then read it back.
- **REG-F11 [Done]** WHEN `\Phrases\<Language>` is absent on category load THE
  SYSTEM SHALL create that language's defaults via `InstallDefaultPhrases(<lang>)`
  from `DEFAULT_FREQUENT_PHRASES_CATEGORIES_ALL_LANGUAGES`, then read them back.
- **REG-F12 [Done]** WHEN the language is unspecified THE SYSTEM SHALL resolve it
  to `GetSystemLanguage()` for both settings and phrase paths.

### 6.3 Non-overwrite of existing data

- **REG-F20 [Done]** THE SYSTEM SHALL install defaults **only when the target key
  is missing**, never overwriting existing phrases or settings on update; to
  re-install a language's defaults the user deletes that language key.

### 6.4 Load / save lifecycle

- **REG-F30 [Done]** `LoadSettingsFromRegistry` SHALL first seed the `Settings`
  struct with defaults, then enumerate `\Settings` and override present values
  (booleans by `== "1"`; volume/rate clamped; dwell values via `wcstol`),
  returning the merged struct.
- **REG-F31 [Done]** `SaveSettingsToRegistry` SHALL clamp volume/rate and write
  every settings value to `\Settings` (creating the key if needed).
- **REG-F32 [Done]** `LoadCategoriesFromRegistry` / `SaveCategoriesToRegistry`
  SHALL read/write `\Phrases\<Language>` (one value per category); saving with
  `clearExisting` SHALL delete and rewrite the language key, and SHALL
  extract/re-emit the `$$board` style value (owned by
  [`board-style.spec.md`](board-style.spec.md)).
- **REG-F33 [Done]** THE SYSTEM SHALL persist session state under `\LastRun`:
  `Category Window Size` (`"<w>x<h>"`), `Selected Category`, and `Zoom Factor`,
  each with its own save/load entry point.
- **REG-F34 [Done]** `SaveRunInfoToRegistry(version)` SHALL increment `Total Runs`
  every run; WHEN the passed version differs from the stored `Version` it SHALL
  set `Version` to the new value and reset `Version Runs` to 1, ELSE increment
  `Version Runs`.

### 6.5 Robustness

- **REG-F40 [Done]** Corrupt/malformed numeric registry values SHALL fall back to
  a benign default instead of throwing. Dwell values are parsed with `wcstol`,
  which returns `0` on malformed input. `Voice Volume`, `Voice Rate`,
  `Category Window Size`, `Selected Category`, `Zoom Factor`, and the run counters
  (`Version Runs`, `Total Runs`) are parsed with `std::stoi`/`std::stof` guarded by
  `try/catch`: on `std::invalid_argument`/`std::out_of_range` each keeps its default
  (the seeded settings default, the getter's initial value, or — for the window
  size — a `false` "no remembered size" result), so a hand-corrupted value cannot
  crash the app on load.

### 6.6 Non-functional

- **REG-N01 [Done]** Storage SHALL be **HKCU-only** and require no elevation.
- **REG-N02 [Done]** All values SHALL be `REG_SZ`.
- **REG-N03 [Done]** Registry access SHALL be confined to `RegistryManager`
  (other modules call its API; they do not touch the registry).

## 7. Architecture & components

### 7.1 Files

| File | Responsibility |
|---|---|
| [`include/RegistryManager.h`](../../include/RegistryManager.h) | `RegistryManager` static API + clamp macros. |
| [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp) | Value-name/default `#define`s, key-path helpers, all load/save/install functions. |
| [`include/stdafx.h`](../../include/stdafx.h) | `Settings` struct; `DWELL_DEFAULT_*`; `APP_NAME`. |
| [`include/default_phrases.h`](../../include/default_phrases.h) | `DEFAULT_FREQUENT_PHRASES_CATEGORIES_ALL_LANGUAGES` (first-run phrase sets). |

### 7.2 Key-path helpers

- `GetRegistryPath()` → `SOFTWARE\SimonSays`
- `GetSettingsRegistryPath()` → `…\Settings`
- `GetPhrasesRegistryPath()` → `…\Phrases`
- `GetLanguageSpecificPath(lang)` → `…\Phrases\<lang>`
- `GetLastRunRegistryPath()` → `…\LastRun`

### 7.3 Layering

`RegistryManager` depends on `utils` (serialization, `GetSystemLanguage`,
`GetAccentColor`), `default_phrases.h`, and SAPI helpers (for voice enumeration).
Consuming modules (MainWindow, CategoryWindow, Settings/Dwell dialogs) call its
static API; none access the registry directly (REG-N03).

## 8. Detailed design

### 8.1 Registry layout

```
HKCU\SOFTWARE\SimonSays
├─ Settings        (app Settings — REG_SZ values, §9.1)
├─ Phrases
│   └─ <Language>  (one REG_SZ value per category: name "<icon>##<name>",
│                   data "$$<style>|<phrase>|<phrase>…")
└─ LastRun         (session/run state, §9.2)
```

### 8.2 Buffers

Enumeration uses fixed buffers: `REG_KEY_NAME_BUFFER_SIZE` = 256 wchar,
`REG_KEY_DATA_BUFFER_SIZE` = 1024 wchar. A category's serialized data (style
token + all phrases) must fit in the data buffer (see §17).

### 8.3 Load/save flow

- **Settings load:** seed defaults → open `\Settings` (install if missing) →
  enumerate values → override by name. **Settings save:** clamp → create key →
  write all values.
- **Categories:** load opens `\Phrases\<lang>` (install if missing), enumerates,
  parses each value, extracts `$$board`, and drops empty/`$$`-named entries; save
  (with `clearExisting`) deletes the language key and rewrites all categories plus
  the `$$board` value. *(Model/token details in
  [`categories-phrases.spec.md`](categories-phrases.spec.md) /
  [`board-style.spec.md`](board-style.spec.md).)*
- **LastRun:** each datum (size, selected category, zoom, run/version counters)
  has its own tiny save/load helper writing to `\LastRun`.

## 9. Data model & persistence

### 9.1 `\Settings` values (all `REG_SZ`)

| Value name | Type | Default |
|---|---|---|
| `Language` | string | `GetSystemLanguage()` |
| `Use Default Text` | bool | `0` |
| `Default Text` | string | `""` |
| `Voice Key` | string | `""` |
| `Voice Volume` | int (clamped) | max volume |
| `Voice Rate` | int (clamped) | `0` |
| `Speak Directly When Clicking Phrase` | bool | `1` |
| `Remember Category Window Size` | bool | `1` |
| `Minimize Category Window Automatically` | bool | `1` |
| `Increase Volume When Playing` | bool | `1` |
| `Reduce Other Audio When Playing` | bool | `1` |
| `Stop Previous Playback` | bool | `0` |
| `Show Touch Keyboard` | bool | `0` |
| `Dwell Mode Selection` | int | `DWELL_DEFAULT_MODE_SELECTION` (0 = Auto) |
| `Dwell Time Ms` | int | `DWELL_DEFAULT_TIME_MS` (800) |
| `Dwell Tolerance Radius` | int | `DWELL_DEFAULT_TOLERANCE_PX` (35) |
| `Dwell Cooldown Ms` | int | `DWELL_DEFAULT_COOLDOWN_MS` (300) |
| `Dwell Progress Color` | DWORD | `GetAccentColor()` |
| `Dwell Detected Mode` | int | `DWELL_DEFAULT_DETECTED_MODE` (0 = Off) |

Value semantics: settings → [`settings.spec.md`](settings.spec.md); voice/volume/
rate → [`tts.spec.md`](tts.spec.md); dwell → [`dwell.spec.md`](dwell.spec.md).

### 9.2 `\LastRun` values (all `REG_SZ`)

| Value name | Format | Meaning |
|---|---|---|
| `Category Window Size` | `"<w>x<h>"` | remembered window size |
| `Selected Category` | int | last selected category index (−1 if unset) |
| `Zoom Factor` | float | category-window zoom |
| `Version` | string | last-run product version |
| `Version Runs` | int | runs of the current version (reset to 1 on change) |
| `Total Runs` | int | total application runs |

### 9.3 `\Phrases\<Language>`

One value per category — see [`categories-phrases.spec.md`](categories-phrases.spec.md)
§8 (marker grammar) and [`board-style.spec.md`](board-style.spec.md) §8.3 (`$$board`
value and `$$` style tokens). This spec owns only the key location and `REG_SZ`
storage rule.

## 10. Key interfaces

```cpp
// RegistryManager (static). Language "" resolves to GetSystemLanguage().
static Settings LoadSettingsFromRegistry();
static bool     SaveSettingsToRegistry( const Settings & );

static std::vector<Category> LoadCategoriesFromRegistry( std::wstring language = L"",
                                                         std::wstring * outBoardStyle = nullptr );
static bool SaveCategoriesToRegistry( const std::vector<Category> &, std::wstring language = L"",
                                      bool clearExisting = false, const std::wstring & boardStyle = L"" );

static bool  SaveCategoryWindowSizeToRegistry( int w, int h );
static bool  LoadCategoryWindowSizeFromRegistry( int & w, int & h );
static bool  SaveSelectedCategoryToRegistry( int category );
static int   GetSelectedCategoryFromRegistry();          // −1 if unset
static bool  SaveZoomFactorToRegistry( float );
static float GetZoomFactorFromRegistry();                // 1.0 if unset

static bool        SaveRunInfoToRegistry( std::wstring version );
static int         GetVersionRunCount();
static int         GetApplicationRunCount();
static std::wstring GetLastRunVersionFromRegistry();
static std::vector<LanguageInfo> GetPhrasesLanguagesInRegistry();
static std::vector<VoiceInfo>    PopulateAvaibleVoicesFromRegistry( std::wstring languageFilter = L"" );
```

## 11. UI specification

N/A — no UI of its own. Persistence is triggered by app lifecycle and by other
windows (Settings/Dwell OK → save; startup → load; CategoryWindow close → save
size/zoom/selected category).

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| Root key | `HKCU\SOFTWARE\SimonSays` | `RegistryManager.cpp` `GetRegistryPath` |
| Subkeys | `Settings`, `Phrases\<lang>`, `LastRun` | `RegistryManager.cpp` path helpers |
| Name buffer | 256 wchar | `REG_KEY_NAME_BUFFER_SIZE` |
| Data buffer | 1024 wchar | `REG_KEY_DATA_BUFFER_SIZE` |
| Value type | `REG_SZ` | throughout |
| Dwell numeric defaults | 0 / 800 / 35 / 300 / (Off) | `stdafx.h` `DWELL_DEFAULT_*` |

## 13. Diagnostics

N/A — no diagnostic dump. (Registry failures are handled by returning a benign
result; there is no logging.)

## 14. Edge cases & error handling

- **Missing key** → install defaults (settings / phrases) or return a benign
  fallback (`\LastRun` getters return −1 / 1.0 / `""`).
- **Malformed dwell value** → `wcstol` returns 0 (no throw).
- **Malformed numeric setting / `\LastRun` value** → the `std::stoi`/`std::stof`
  parse is wrapped in `try/catch`; on non-numeric or out-of-range input the value
  falls back to its default (`Voice Volume`/`Voice Rate` keep the seeded settings
  default; `Selected Category` → −1; `Zoom Factor` → 1.0; run counters → 0;
  `Category Window Size` → `false`). No uncaught exception reaches the caller.
- **Oversized category data** → truncated to the 1024-wchar data buffer on read.
- **`RegCreateKeyEx`/`RegSetValueEx` failure** → the save function returns
  `false`; callers proceed (best-effort).

## 15. Acceptance criteria (testable)

Reverse-engineered from shipping behavior; **[Pass]** reflects the code path.

- **AC-1 (REG-F01/N01/N02) [Pass]** All state lives under `HKCU\SOFTWARE\SimonSays`
  in `\Settings` / `\Phrases\<lang>` / `\LastRun` as `REG_SZ`; no admin needed.
- **AC-2 (REG-F10/F11/F12) [Pass]** A clean profile gets default settings and the
  system-language phrase set on first run.
- **AC-3 (REG-F20) [Pass]** Re-running the app preserves edited phrases; deleting
  a language key recreates that language's defaults.
- **AC-4 (REG-F30/F31) [Pass]** A settings change round-trips (save → relaunch →
  load), with volume/rate clamped.
- **AC-5 (REG-F32) [Pass]** Categories (and the `$$board` style) round-trip
  through `\Phrases\<lang>`; a `clearExisting` save rewrites the key.
- **AC-6 (REG-F33/F34) [Pass]** Window size / selected category / zoom persist in
  `\LastRun`; `Total Runs` increments and `Version Runs` resets on a version change.
- **AC-7 (REG-F40) [Pass]** A corrupt numeric value loads as its default rather
  than crashing: a corrupt dwell value loads as 0, and a corrupt `Voice Volume`,
  `Voice Rate`, `Category Window Size`, `Selected Category`, `Zoom Factor`, or run
  counter falls back to its default (§14) with no uncaught exception on load.

Build gate: Debug **and** Release x64 compile clean (no code change in this
authoring pass).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| HKCU key layout (`Settings`/`Phrases`/`LastRun`) | ✅ Done | all `REG_SZ` |
| First-run default settings | ✅ Done | `InstallDefaultSettings` |
| First-run default phrase sets | ✅ Done | `InstallDefaultPhrases` per language |
| Non-overwrite on update | ✅ Done | install only when key missing |
| Settings load/save (+ clamps) | ✅ Done | seed defaults → override |
| Categories load/save (+ `$$board`) | ✅ Done | `clearExisting` rewrite |
| `\LastRun` session/run state | ✅ Done | size, selected, zoom, run/version |
| Malformed-value robustness | ✅ Done | dwell via `wcstol`; `std::stoi`/`stof` guarded by `try/catch` → default fallback (REG-F40) |

## 17. Known limitations

- **1024-wchar data buffer** caps a single category's serialized phrase data.
- **HKCU-only** — no system-wide or roaming storage.
- **No schema/versioning** of the value layout beyond the `\LastRun\Version`
  string.

## 18. Future work

- Dynamic-size reads (two-call `RegQueryValueEx`) to remove the 1024-wchar cap.

## 19. Open questions

1. ~~Should the numeric-parse hardening clamp to defaults silently, or surface a
   one-time "settings reset" notice?~~ **Resolved (REG-F40):** the fallback is
   **silent** — a corrupt value loads as its default with no user-facing notice.

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of REG-SPEC v1.0.*
