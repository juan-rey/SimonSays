# SimonSays — Categories & Phrases Management — Specification

| | |
|---|---|
| **Spec ID** | CAT-SPEC |
| **Status** | Draft (skeleton — no requirements authored yet) |
| **Version** | 0.1 |
| **REQ prefix** | `CAT-F##` (functional), `CAT-N##` (non-functional) |
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
conventions. This file is the source of truth for the **Categories window and
category/phrase management**, including the `##` icon prefix and `::` inline-audio
suffix authoring conventions. Reference format: [`docs/specs/dwell.spec.md`](dwell.spec.md).

> Scope reminder: browsing, creating, editing, deleting, and ordering categories
> and phrases; icon specification (`##`); audio-file suffix (`::`); the
> immediate-speak-on-select option. Locating/playing the resulting audio is in
> [`sound.spec.md`](sound.spec.md). Import/export of categories is in
> [`import-export.spec.md`](import-export.spec.md). Storage of phrases is in
> [`persistence.spec.md`](persistence.spec.md). The Settings toggles that affect
> behavior are in [`settings.spec.md`](settings.spec.md). Styling of the
> category window and its buttons — including the reserved `$$board` category
> name and the `::` style suffix on category names — is in
> [`board-style.spec.md`](board-style.spec.md).

**Source material to mine (do not invent beyond it):**
- [`HELP.md`](../../HELP.md) — "Categories window (phrases)", "Managing
  categories", "Setting or changing a category icon" (`<icon>##<name>`),
  "Managing phrases", "Setting or changing a phrase icon", phrase full format
  `<icon>##<phrase text>::<audio file>` and the optional/independent rules.
- [`README.md`](../../README.md) — keyboard shortcuts F4–F8 (edit / move / add /
  delete), "Bundle sounds and icons" note.

---

## 1. Overview

> TODO. A dozen categories of customizable phrases reachable in a couple of
> clicks; each category/phrase may carry an icon, and phrases may carry inline
> audio.

## 2. Background & context

> TODO. The `##` prefix and `::` suffix are both optional and independent (per
> HELP); only `.ico` files are supported for file-based icons.

## 3. Goals & non-goals

**Goals** — TODO.
**Non-goals** — TODO (icon rendering details / audio playback → other specs).

## 4. Glossary

| Term | Meaning |
|---|---|
| Category | TODO |
| Phrase | TODO |
| `##` icon prefix | TODO (`<icon>##<name>`) |
| `::` audio suffix | TODO (`<text>::<audio file>`) |

## 5. Personas & scenarios

> TODO. e.g. selecting a category then a phrase; renaming a category; adding a
> phrase with an emoji and a sound.

## 6. Requirements (EARS)

> TODO — author from actual code/behavior. No requirements invented here.

### 6.1 Browsing & selection (functional)

- **CAT-F01 [Planned]** TODO (select category at top, phrases at bottom;
  single-click behavior gated by the immediate-speak setting). *(reserve CAT-F02 …)*

### 6.2 Category management (functional)

- **CAT-F1# [Planned]** TODO (add / edit/rename / delete; F7 / F4 / F8;
  delete confirms category + its phrases).

### 6.3 Phrase management (functional)

- **CAT-F2# [Planned]** TODO (add / edit / delete; move previous/next F5/F6).

### 6.4 Icon & audio authoring conventions (functional)

- **CAT-F3# [Planned]** TODO (`##` emoji or `.ico` path; `::` audio suffix;
  combinations; non-`.ico` extensions ignored).

### 6.5 Non-functional

- **CAT-N01 [Planned]** TODO.

## 7. Architecture & components

> TODO.

## 8. Detailed design

> TODO. Parsing of `<icon>##<text>::<audio>` — define precedence and edge cases
> from the actual code.

## 9. Data model & persistence

> TODO. Phrases/categories persist via the registry — cross-reference
> [`persistence.spec.md`](persistence.spec.md); link, don't duplicate.

## 10. Key interfaces

> TODO.

## 11. UI specification

> TODO. Categories window; Add/Edit/Delete dialogs; resize + "remember size"
> (the setting itself is in [`settings.spec.md`](settings.spec.md)).

### 11.1 Accelerators (subset relevant here)

> TODO — confirm against `IDR_MAINACCEL` / `CategoryWindow`. Per HELP/README:
> F4 Edit · F5/F6 move · F7 Add-after-selection · F8 Delete (F9/F10 import/export
> → [`import-export.spec.md`](import-export.spec.md)).

## 12. Configuration & tuning constants

> TODO (e.g. the `##` and `::` separators).

## 13. Diagnostics

> TODO / N/A.

## 14. Edge cases & error handling

> TODO. e.g. non-`.ico` icon file ignored (no icon shown); empty name; combining
> icon + text + audio.

## 15. Acceptance criteria

> TODO.
- AC-1 (CAT-F0?) [Pending] TODO.

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Browse categories/phrases | ⛔ TODO | |
| Category CRUD | ⛔ TODO | |
| Phrase CRUD | ⛔ TODO | |
| `##` icon parsing | ⛔ TODO | |
| `::` inline-audio parsing | ⛔ TODO | |
| Immediate-speak-on-select | ⛔ TODO | |

## 17. Known limitations

> TODO. e.g. only `.ico` supported for file-based icons.

## 18. Future work

> TODO.

## 19. Open questions

> TODO.

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of CAT-SPEC (skeleton).*
