# SimonSays — Import / Export (Bundles) — Specification

| | |
|---|---|
| **Spec ID** | PORT-SPEC |
| **Status** | Draft (skeleton — no requirements authored yet) |
| **Version** | 0.1 |
| **REQ prefix** | `PORT-F##` (functional), `PORT-N##` (non-functional) |
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
conventions. This file is the source of truth for **importing and exporting
categories/phrases**, bundling their icon and audio files. Reference format:
[`docs/specs/dwell.spec.md`](dwell.spec.md).

> Scope reminder: the import/export flows (F9/F10), overwrite prompts, all-vs-
> selected export, per-language result messages, and bundling of icons + audio.
> The category/phrase model is in [`categories-phrases.spec.md`](categories-phrases.spec.md);
> registry storage is in [`persistence.spec.md`](persistence.spec.md); audio/icon
> lookup is in [`sound.spec.md`](sound.spec.md). Board/category styles ride
> inside the exported data and have their own export-scope and import-prompt
> rules — see [`board-style.spec.md`](board-style.spec.md) §6.6.

**Source material to mine (do not invent beyond it):**
- [`HELP.md`](../../HELP.md) — "Importing and exporting categories" (export all
  or selected; import with overwrite prompt; per-language success/failure
  messages; F9 import / F10 export).
- [`README.md`](../../README.md) — "What's new in v0.7" → "Bundle sounds and
  icons: import and export sound files and icon images along with categories and
  phrases"; shortcuts F9/F10.

> Note: the bundle container format itself is not described in README/HELP. Do
> not document the on-disk format here from memory — derive it from the code
> when filling this in, and raise specifics as open questions (§19).

---

## 1. Overview

> TODO. Users can export categories (all or selected) and import them back,
> carrying along the referenced icon and audio files.

## 2. Background & context

> TODO. Capture only documented behavior.

## 3. Goals & non-goals

**Goals** — TODO.
**Non-goals** — TODO.

## 4. Glossary

| Term | Meaning |
|---|---|
| Bundle | TODO (export package of categories/phrases + assets) |
| Export scope | TODO (all categories vs selected one) |

## 5. Personas & scenarios

> TODO. e.g. moving phrase sets between machines; sharing a category with assets.

## 6. Requirements (EARS)

> TODO — author from actual code/behavior. No requirements invented here.

### 6.1 Export (functional)

- **PORT-F01 [Planned]** TODO (choose all vs selected; F10; result message).
  *(reserve PORT-F02 …)*

### 6.2 Import (functional)

- **PORT-F1# [Planned]** TODO (choose file; overwrite prompt if category exists;
  F9; result message).

### 6.3 Asset bundling (functional)

- **PORT-F2# [Planned]** TODO (icons + audio bundled with categories/phrases).

### 6.4 Non-functional

- **PORT-N01 [Planned]** TODO.

## 7. Architecture & components

> TODO.

## 8. Detailed design

> TODO. Bundle/container format — derive from code; do not assume.

## 9. Data model & persistence

> TODO. Imported data lands in the registry-backed store — cross-reference
> [`persistence.spec.md`](persistence.spec.md).

## 10. Key interfaces

> TODO.

## 11. UI specification

> TODO. Export scope prompt; import file picker; overwrite confirmation;
> per-language result messages. Entry points: F9 / F10.

## 12. Configuration & tuning constants

> TODO (e.g. bundle file extension — confirm from code).

## 13. Diagnostics

> TODO / N/A.

## 14. Edge cases & error handling

> TODO. e.g. file permission / disk errors (retry per HELP); existing category
> overwrite; missing referenced assets.

## 15. Acceptance criteria

> TODO.
- AC-1 (PORT-F0?) [Pending] TODO.

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Export (all / selected) | ⛔ TODO | |
| Import (+ overwrite prompt) | ⛔ TODO | |
| Asset bundling (icons + audio) | ⛔ TODO | |
| Per-language result messages | ⛔ TODO | |

## 17. Known limitations

> TODO.

## 18. Future work

> TODO.

## 19. Open questions

> TODO. e.g. exact bundle/container format and extension (not in README/HELP);
> behavior when a bundled asset filename collides with an existing file.

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of PORT-SPEC (skeleton).*
