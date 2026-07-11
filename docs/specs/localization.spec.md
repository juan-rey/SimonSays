# SimonSays — Localization / Multi-Language Support — Specification

| | |
|---|---|
| **Spec ID** | LOC-SPEC |
| **Status** | Active — reverse-engineered from shipping source (2026-07-10) |
| **Version** | 1.0 (2026-07-10) |
| **REQ prefix** | `LOC-F##` (functional), `LOC-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/utils.cpp`](../../src/utils.cpp) (`GetLocalizedString`, language helpers), `SUPPORTED_LANGUAGES` + string IDs in [`include/stdafx.h`](../../include/stdafx.h), [`include/localized_strings.h`](../../include/localized_strings.h) |
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
conventions. This file is the **cross-cutting** source of truth for **UI string
localization**: the string table + lookup with English fallback, the supported
language set, RTL handling, and the localized help. Reference format:
[`docs/specs/dwell.spec.md`](dwell.spec.md).

This spec was authored **from the existing code**, so per the working agreement
the code is authoritative for current behavior (rule 7): if a fact here drifts
from [`src/utils.cpp`](../../src/utils.cpp), fix the spec and flag it.

> Scope reminder: the `GetLocalizedString` mechanism, `SUPPORTED_LANGUAGES`,
> English fallback, RTL message-box/window handling, and the localized help
> generation. **Per-language default phrase sets** are stored/installed per
> [`persistence.spec.md`](persistence.spec.md) (model in
> [`categories-phrases.spec.md`](categories-phrases.spec.md)); **RTL mirroring**
> of controls is applied by the hosts ([`ssbutton.spec.md`](ssbutton.spec.md)
> BTN-F72, [`categories-phrases.spec.md`](categories-phrases.spec.md)); the
> **language-selection UI** is in [`settings.spec.md`](settings.spec.md); the
> **help-edit workflow** is in [`AGENT.md`](../../AGENT.md) §7 + `HELP.md`.

---

## 1. Overview

Every user-visible string is looked up by a numeric **string ID** through
`GetLocalizedString(id, language)`, which reads a compiled per-language table.
The app supports **17 languages** (`SUPPORTED_LANGUAGES`); **English** is the
base and the guaranteed fallback for any missing language or string. RTL
languages (Arabic, Hebrew) get right-to-left message boxes and mirrored windows.
The in-app **Help** is localized too, generated from the Markdown help files.

## 2. Background & context

- Strings are compiled in (per-language `std::vector<std::pair<int, const
  wchar_t*>>` tables in [`localized_strings.h`](../../include/localized_strings.h))
  rather than loaded from resource DLLs, keeping the app a single self-contained
  executable.
- The chosen UI language is a `Settings` value; storing `""` means "follow the
  system language" (see [`settings.spec.md`](settings.spec.md) /
  [`persistence.spec.md`](persistence.spec.md)).
- Help is large, so it is authored as Markdown and **generated** into the string
  table by a script rather than hand-typed into `localized_strings.h`.

## 3. Goals & non-goals

**Goals**
- One lookup API for all UI strings, with a guaranteed English fallback.
- A clear supported-language set with native names and RTL flags.
- Correct RTL rendering for message boxes and windows.
- Localized help kept in sync with the Markdown sources.

**Non-goals**
- **Storing** the language choice or default phrase sets (→
  [`persistence.spec.md`](persistence.spec.md)).
- **RTL mirroring** of individual controls (→ [`ssbutton.spec.md`](ssbutton.spec.md)).
- Runtime translation; strings are compiled in.

## 4. Glossary

| Term | Meaning |
|---|---|
| **String ID** | A numeric `#define` (e.g. `PLAY_BUTTON_TEXT_ID`) identifying a UI string. |
| **Language name** | The `EnglishName` of a supported language (the table key, e.g. `"Spanish"`). |
| **`LOCALIZED_STRINGS`** | The map of language name → its `{id, string}` table. |
| **Fallback** | English — used when a language or string is missing. |
| **`HELP_CONTENT_ID`** | The string ID whose value is the whole localized help document. |

## 5. Personas & scenarios

- **Switch UI language:** pick a language in Settings → all labels, menus, and
  dialogs re-render in that language (English for any gap).
- **RTL user:** choosing Arabic/Hebrew right-aligns message boxes and mirrors
  windows.
- **First run:** the app starts in the system language if supported, else English.
- **Read help:** F1 shows the help in the current language (English if that
  language's help hasn't been translated yet).

## 6. Requirements (EARS)

Status tags per [`docs/spec.md`](../spec.md) §2.3. All requirements below are
implemented in the current source and tagged **[Done]** accordingly.

### 6.1 String lookup & fallback

- **LOC-F01 [Done]** `GetLocalizedString(id, language)` SHALL return the string for
  `id` from the `language` table; WHEN `language` is empty it SHALL use
  `GetSystemLanguage()`.
- **LOC-F02 [Done]** IF the language is not in the table, or the id is not in that
  language's table, THEN THE SYSTEM SHALL fall back to **English**; IF English also
  lacks the id THEN it SHALL return an empty string.

### 6.2 Supported languages

- **LOC-F10 [Done]** `SUPPORTED_LANGUAGES` SHALL define the supported set (17
  languages), each with `EnglishName`, `NativeName`, `IsRTL`, `LanguageId`
  (`LANGID`), and a `VoiceTestSampleText`; **English** SHALL be the base and
  fallback language.

### 6.3 Language resolution

- **LOC-F20 [Done]** `GetSystemLanguage()` SHALL map the OS locale (normalized
  lower-case, prefix-matched) to a supported language, defaulting to English.
- **LOC-F21 [Done]** A UI language equal to the system language SHALL be stored as
  `""` (so it tracks the OS); helpers `GetLanguageNativeName`,
  `GetLangIdFromLanguageString`, and `GetLanguageStringFromLangId` SHALL convert
  between the name, native name, and `LANGID`.

### 6.4 RTL support

- **LOC-F30 [Done]** `IsLanguageRTL(language)` SHALL report the language's `IsRTL`
  flag (empty → the system language).
- **LOC-F31 [Done]** `GetMessageBoxFlagsForLanguage` SHALL add
  `MB_RTLREADING | MB_RIGHT` for RTL languages, and `ShowLocalizedMessageBox` SHALL
  present via `MessageBoxEx` with the language's `LANGID`. Control/window mirroring
  is applied by the hosts (→ [`ssbutton.spec.md`](ssbutton.spec.md) BTN-F72).

### 6.5 Per-language content

- **LOC-F40 [Done]** THE SYSTEM SHALL provide per-language **default phrase sets**
  (`default_phrases.h`) installed on first run; their storage is owned by
  [`persistence.spec.md`](persistence.spec.md) and the model by
  [`categories-phrases.spec.md`](categories-phrases.spec.md).
- **LOC-F41 [Done]** THE SYSTEM SHALL localize the in-app **Help**: `HELP_CONTENT_ID`
  carries the whole help document per language, **generated** from `HELP.md`
  (English/default) and `docs/help/HELP_<xx>.md` by
  `scripts/sync_help_content.ps1` — the generated value SHALL NOT be hand-edited.

### 6.6 Text helpers

- **LOC-F50 [Done]** `ReplaceAmpersandLocalized` SHALL replace `&` with the
  language's `AMPERSAND_REPLACEMENT_ID` string (used to make export filenames
  safe / avoid mnemonic clashes).

### 6.7 Non-functional

- **LOC-N01 [Done]** All user-visible strings SHALL be obtained via
  `GetLocalizedString`; identifiers and comments SHALL be in English.
- **LOC-N02 [Done]** English SHALL always be complete so the fallback never yields
  an empty user-facing string in practice.

## 7. Architecture & components

### 7.1 Files

| File | Responsibility |
|---|---|
| [`src/utils.cpp`](../../src/utils.cpp) | `GetLocalizedString`, `GetSystemLanguage`, `IsLanguageRTL`, message-box/ampersand/language helpers. |
| [`include/stdafx.h`](../../include/stdafx.h) | `SUPPORTED_LANGUAGES`; the string-ID `#define`s; `LanguageInfo`. |
| [`include/localized_strings.h`](../../include/localized_strings.h) | Per-language `{id, string}` tables + `LOCALIZED_STRINGS` map; generated `HELP_CONTENT_ID`. |
| [`include/default_phrases.h`](../../include/default_phrases.h) | Per-language first-run phrase sets. |
| `HELP.md`, `docs/help/HELP_<xx>.md`, `scripts/sync_help_content.ps1` | Help sources + generator. |

### 7.2 Structure

- **Lookup** is a linear scan: find the language's table in `LOCALIZED_STRINGS`,
  then the id within it; miss → recurse to English.
- **Language metadata** (`SUPPORTED_LANGUAGES`) drives the Settings language list,
  RTL decisions, `LANGID`s, and voice-test samples.
- **Help** strings are produced by the sync script from Markdown into the
  per-language tables.

### 7.3 Layering

`utils` provides the localization API used everywhere; no module hard-codes UI
strings. `RegistryManager` uses `GetSystemLanguage` / language names for the
phrase-store keys; hosts apply RTL ex-styles.

## 8. Detailed design

### 8.1 Lookup with fallback

```
GetLocalizedString(id, lang):
  if lang == "" : lang = GetSystemLanguage()
  table = LOCALIZED_STRINGS[lang]?          # linear find by EnglishName
  if table and id in table : return value
  if lang != "English" : return GetLocalizedString(id, "English")
  return ""
```

### 8.2 System-language mapping

`GetSystemLanguage` reads the OS default locale, lower-cases it, and prefix-
matches it against the supported set (so e.g. `ca-ES-valencia` resolves to
Valencian), defaulting to English when unmatched.

### 8.3 Help generation

`scripts/sync_help_content.ps1` reads each help Markdown file, splits it in two
halves, and embeds it as `HELP_CONTENT_ID` (`LR"HELP(...)HELP"` raw literals) in
the matching language table. Editing help = edit the `.md`, run the script,
rebuild (see [`AGENT.md`](../../AGENT.md) §7).

## 9. Data model & persistence

No storage of its own. The chosen UI **language** is a `Settings` value
(→ [`persistence.spec.md`](persistence.spec.md) §9.1); default **phrase sets** are
installed to `\Phrases\<Language>` on first run (LOC-F40). Language table keys are
the `EnglishName`s.

## 10. Key interfaces

```cpp
const wchar_t * GetLocalizedString( int stringId, std::wstring language );  // English fallback
std::wstring GetSystemLanguage();                        // OS locale → supported language
bool         IsLanguageRTL( const std::wstring & language );
UINT         GetMessageBoxFlagsForLanguage( const std::wstring & language, UINT baseFlags );
int          ShowLocalizedMessageBox( HWND, const wchar_t * text, const wchar_t * caption,
                                      UINT baseFlags, const std::wstring & language );
std::wstring GetLanguageNativeName( std::wstring language );
LANGID       GetLangIdFromLanguageString( std::wstring language );
std::wstring GetLanguageStringFromLangId( LANGID );
std::wstring ReplaceAmpersandLocalized( const std::wstring & str, const std::wstring & language );
```

## 11. UI specification

N/A — no window of its own. The language **list** is presented by the Settings
dialog (native names); RTL languages right-align message boxes and mirror
windows; Help renders the localized `HELP_CONTENT_ID`.

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| Supported languages | 17 (Arabic, Basque, Catalan, Chinese (Simplified), English, French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Spanish, Valencian) | `stdafx.h` `SUPPORTED_LANGUAGES` |
| Fallback language | English | `utils.cpp` `GetLocalizedString` |
| String IDs | `#define … _ID` | `stdafx.h` |
| Per-language tables + map | `*_LOCALIZED_UI_STRINGS`, `LOCALIZED_STRINGS` | `localized_strings.h` |
| Help content id | `HELP_CONTENT_ID` (generated) | `localized_strings.h` / `sync_help_content.ps1` |

## 13. Diagnostics

N/A — no diagnostics; a missing string degrades to English then empty.

## 14. Edge cases & error handling

- **Unsupported system locale** → English.
- **Missing string in a language** → English; missing in English → `""`.
- **RTL language** → RTL message boxes + mirrored windows.
- **Untranslated help** → that language's `HELP_CONTENT_ID` may still be English
  until `docs/help/HELP_<xx>.md` is translated and re-synced.
- **`&` in export filenames** → replaced via `ReplaceAmpersandLocalized`.

## 15. Acceptance criteria (testable)

Reverse-engineered from shipping behavior; **[Pass]** reflects the code path.

- **AC-1 (LOC-F01/F02) [Pass]** A known id returns the language string; an id absent
  in a language returns the English string; absent everywhere → `""`.
- **AC-2 (LOC-F10/F20/F21) [Pass]** The 17 languages are selectable; the system
  language is auto-detected; a system-equal choice stores `""`.
- **AC-3 (LOC-F30/F31) [Pass]** Arabic/Hebrew produce RTL message boxes; LTR
  languages do not.
- **AC-4 (LOC-F41) [Pass]** Help renders in the current language; editing `HELP.md`
  + running the sync updates `HELP_CONTENT_ID`.
- **AC-5 (LOC-F50) [Pass]** `&` in a suggested export filename is replaced with the
  localized token.

Build gate: Debug **and** Release x64 compile clean (no code change in this
authoring pass).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| String lookup + English fallback | ✅ Done | `GetLocalizedString` |
| Supported language set | ✅ Done | 17 in `SUPPORTED_LANGUAGES` |
| System-language detection | ✅ Done | normalized prefix match |
| RTL message boxes | ✅ Done | `MB_RTLREADING\|MB_RIGHT` + `LANGID` |
| Per-language default phrases | ✅ Done | `default_phrases.h` (→ persistence) |
| Localized help generation | ✅ Done | `sync_help_content.ps1` |

## 17. Known limitations

- Strings are **compiled in** — adding/translating requires a rebuild.
- Lookup is a linear scan per call (fine at the current string count).
- Some languages' **help** may lag the English `HELP.md` until translated (e.g.
  the board-style section — see `TODO`).
- Newly added machine-detected phrase languages (registry subkeys not in
  `SUPPORTED_LANGUAGES`) appear with neutral metadata (no RTL / native name).

## 18. Future work

- Translate outstanding help sections into the 16 non-English files.
- Consider a hashed lookup if the string count grows materially.

## 19. Open questions

1. Should untranslated per-language help fall back to English at runtime (it
   currently shows whatever the file holds, which may already be English)?

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of LOC-SPEC v1.0.*
