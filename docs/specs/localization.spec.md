# SimonSays — Localization / Multi-Language Support — Specification

| | |
|---|---|
| **Spec ID** | LOC-SPEC |
| **Status** | Draft (skeleton — no requirements authored yet) |
| **Version** | 0.1 |
| **REQ prefix** | `LOC-F##` (functional), `LOC-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
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
conventions. This file is the source of truth for **UI localization, built-in
per-language phrase defaults, localized help, and string fallback**. Reference
format: [`docs/specs/dwell.spec.md`](dwell.spec.md) (note its §6.7 string-routing /
English-fallback convention).

> Scope reminder: language label selection mechanics, localized UI strings,
> per-language default phrase sets, localized help files, and English fallback.
> Installing/storing the default phrase sets is in [`persistence.spec.md`](persistence.spec.md);
> the Settings control that picks the language is in [`settings.spec.md`](settings.spec.md).

**Source material to mine (do not invent beyond it):**
- [`README.md`](../../README.md) — "Supported languages" (the language list),
  "Help files" (per-language `docs/help/HELP_*.md`), note on voice availability.
- [`HELP.md`](../../HELP.md) — "Settings (F2)" → Language labels list;
  "Troubleshooting" → UI language not applied / ensure localization strings exist.

> Supported languages per the docs: Arabic, Basque, Catalan, Chinese
> (Simplified), English, French, Galician, German, Hebrew, Hindi, Italian,
> Japanese, Korean, Portuguese, Russian, Spanish, Valencian. (Confirm the
> authoritative list against the code when filling in.)

---

## 1. Overview

> TODO. SimonSays ships built-in phrase sets and UI labels for multiple
> languages; users pick a UI language in Settings; missing strings fall back to
> English.

## 2. Background & context

> TODO. Voice availability is independent of UI language (depends on installed
> Windows voices) — cross-reference [`tts.spec.md`](tts.spec.md).

## 3. Goals & non-goals

**Goals** — TODO.
**Non-goals** — TODO.

## 4. Glossary

| Term | Meaning |
|---|---|
| UI language label | TODO |
| Default phrase set | TODO (per-language built-in phrases) |
| String fallback | TODO (missing → English) |
| RTL | TODO (right-to-left languages, e.g. Arabic/Hebrew) |

## 5. Personas & scenarios

> TODO. e.g. switching UI language; first-run default phrases in the chosen
> language.

## 6. Requirements (EARS)

> TODO — author from actual code/behavior. No requirements invented here.

### 6.1 UI string localization (functional)

- **LOC-F01 [Planned]** TODO (all user-visible strings route through the
  localization layer; English authored as fallback). *(reserve LOC-F02 …)*

### 6.2 Language selection (functional)

- **LOC-F1# [Planned]** TODO (choose UI language in Settings; apply across UI).

### 6.3 Built-in default phrase sets (functional)

- **LOC-F2# [Planned]** TODO (per-language default phrases) — installation
  mechanics cross-reference [`persistence.spec.md`](persistence.spec.md).

### 6.4 Localized help (functional)

- **LOC-F3# [Planned]** TODO (per-language `docs/help/HELP_*.md`).

### 6.5 Non-functional

- **LOC-N01 [Planned]** TODO (e.g. identifiers/comments in English; UI localized).

## 7. Architecture & components

> TODO (e.g. `localized_strings.h` / `GetLocalizedString` per dwell.spec
> references — confirm against code).

## 8. Detailed design

> TODO.

## 9. Data model & persistence

> TODO. Selected language persists in settings; default phrases stored per
> language — cross-reference [`persistence.spec.md`](persistence.spec.md).

## 10. Key interfaces

> TODO.

## 11. UI specification

> TODO. Language label picker (in Settings → [`settings.spec.md`](settings.spec.md));
> RTL handling for applicable languages.

## 12. Configuration & tuning constants

> TODO (e.g. the supported-language enumeration).

## 13. Diagnostics

> TODO / N/A.

## 14. Edge cases & error handling

> TODO. e.g. missing localized string → English fallback; UI language not
> applied → reopen Settings (per HELP).

## 15. Acceptance criteria

> TODO.
- AC-1 (LOC-F0?) [Pending] TODO.

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| UI string localization + fallback | ⛔ TODO | |
| Language selection | ⛔ TODO | |
| Built-in default phrase sets | ⛔ TODO | |
| Localized help files | ⛔ TODO | |
| RTL support | ⛔ TODO | |

## 17. Known limitations

> TODO. e.g. some languages may fall back to English for newer strings (see
> dwell.spec: other languages fall back to English).

## 18. Future work

> TODO.

## 19. Open questions

> TODO. e.g. authoritative supported-language list vs. README/HELP wording.

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of LOC-SPEC (skeleton).*
