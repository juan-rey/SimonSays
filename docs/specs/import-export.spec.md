# SimonSays — Import / Export (Bundles) — Specification

| | |
|---|---|
| **Spec ID** | PORT-SPEC |
| **Status** | Active — reverse-engineered from shipping source (2026-07-10); PNG/JPG icons bundled since 2026-07-11 |
| **Version** | 1.1 (2026-07-11) |
| **REQ prefix** | `PORT-F##` (functional), `PORT-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/utils.cpp`](../../src/utils.cpp) (`.ssc`/`.ssz`), [`src/CategoryWindow.cpp`](../../src/CategoryWindow.cpp) (flows), [`src/main.cpp`](../../src/main.cpp) (file association), `SSZ_*` in [`include/stdafx.h`](../../include/stdafx.h) |
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
categories/phrases**, the `.ssc` (plain) and `.ssz` (bundle) file formats, and
the file-association import path. Reference format: [`docs/specs/dwell.spec.md`](dwell.spec.md).

This spec was authored **from the existing code**, so per the working agreement
the code is authoritative for current behavior (rule 7): if a fact here drifts
from [`src/utils.cpp`](../../src/utils.cpp), fix the spec and flag it.

> Scope reminder: the export/import flows (F9/F10 and file association), the
> `.ssc`/`.ssz` on-disk formats, overwrite/adoption prompts, all-vs-selected
> export, asset bundling, and per-language result messages. The category/phrase
> **model** and the `##`/`::` marker grammar are in
> [`categories-phrases.spec.md`](categories-phrases.spec.md); registry **storage**
> is in [`persistence.spec.md`](persistence.spec.md); audio/icon **lookup &
> playback** are in [`sound.spec.md`](sound.spec.md); the `$$board` / `$$` style
> tokens, the export **scope** rule, and the board-style import prompt are in
> [`board-style.spec.md`](board-style.spec.md).

---

## 1. Overview

Users can **export** their categories & phrases to a file and **import** them
back — to move a board between machines or share one. Two formats:

- **`.ssc`** — a plain UTF-8 text file: a header line plus one line per category.
- **`.ssz`** — a Zip **bundle** (via miniz) wrapping a `categories.ssc` plus the
  referenced icon/audio resource files, so a styled board with its assets travels
  as a single file.

Export offers all categories or just the selected one and auto-picks the format
(`.ssz` when there are resources to carry, else `.ssc`). Import accepts either
format — from the F9 picker or by opening a `.ssc`/`.ssz` file (file
association) — prompting before overwriting an existing category.

## 2. Background & context

- The formats reuse the same category/phrase serialization as the registry
  (shared helpers in [`utils.cpp`](../../src/utils.cpp)), so the marker grammar
  (`##`, `::`, `|`, `$$`) is single-sourced.
- Bundling exists so icons (`.ico`/`.png`/`.jpg`) and audio (`.wav`/`.mp3`)
  don't have to be copied manually; on import they are extracted into the user's
  app-data folder where the app looks them up (see [`sound.spec.md`](sound.spec.md)).
- `.ssz` import is hardened against malformed/hostile archives (zip-bomb limits,
  entry-name validation, two-phase commit) because a bundle may come from an
  untrusted sender.

## 3. Goals & non-goals

**Goals**
- Round-trip categories/phrases (and their style) losslessly through `.ssc`/`.ssz`.
- Carry referenced icon/audio assets in `.ssz` so a board is self-contained.
- Safe import of untrusted bundles (size/ratio limits, atomic asset install).
- Localized, non-destructive UX (overwrite prompt; benign on error).

**Non-goals**
- The category/phrase **model** and marker grammar (→
  [`categories-phrases.spec.md`](categories-phrases.spec.md)).
- Registry **storage** of imported data (→ [`persistence.spec.md`](persistence.spec.md)).
- Asset **lookup/playback** (→ [`sound.spec.md`](sound.spec.md)).
- Board-style token semantics, export scope, and the replace prompt (→
  [`board-style.spec.md`](board-style.spec.md)).

## 4. Glossary

| Term | Meaning |
|---|---|
| **`.ssc`** | "SimonSays Categories" — a plain UTF-8 text export (header + category lines). |
| **`.ssz`** | A Zip bundle: one `categories.ssc` at the root + `resources/<file>` assets. |
| **Bundle** | The `.ssz` archive and its contents. |
| **Export scope** | All categories vs the single selected category. |
| **Resource** | A bundled asset — only the supported icon formats (`.ico`/`.png`/`.jpg`/`.jpeg`) plus `.wav`, `.mp3` are carried. |
| **Reconciliation** | On import, deciding per asset reference: keep (bundled), keep (resolvable locally), or strip (dangling). |

## 5. Personas & scenarios

- **Therapist shares a styled board:** exports all → a `.ssz` with the six SPC
  categories, their colors, and any icons/sounds; the user imports it and gets
  the whole board.
- **Move to a new PC:** export all → `.ssc`/`.ssz`, copy the file, import on the
  new machine.
- **Open a received file:** double-click a `.ssc`/`.ssz` → it imports into the
  running SimonSays (or a fresh instance).
- **Merge one category:** export the selected category only; on import, if it
  already exists, choose whether to overwrite.

## 6. Requirements (EARS)

Status tags per [`docs/spec.md`](../spec.md) §2.3. All requirements below are
implemented in the current source and tagged **[Done]** accordingly.

### 6.1 Export

- **PORT-F01 [Done]** WHEN `F10` is pressed THE SYSTEM SHALL offer to export **all**
  categories or only the **selected** one (a Yes/No prompt when a category is
  selected; all otherwise).
- **PORT-F02 [Done]** THE SYSTEM SHALL auto-pick the format — `.ssz` WHEN any
  exported category/phrase references an icon/audio file that exists on disk,
  otherwise `.ssc` — while honoring an extension the user types explicitly in the
  save dialog.
- **PORT-F03 [Done]** WHEN exporting **all** THE SYSTEM SHALL include the board
  style (`$$board`); WHEN exporting a **selected** category it SHALL include only
  that category's own style and not `$$board` (owned by
  [`board-style.spec.md`](board-style.spec.md) STY-F52).
- **PORT-F04 [Done]** THE SYSTEM SHALL suggest a filename from the date + user
  (or category) name + native language name, with `&` and spaces sanitized, and
  report success/failure in a localized message.

### 6.2 Import

- **PORT-F10 [Done]** WHEN `F9` is pressed THE SYSTEM SHALL prompt for a file and
  accept `.ssc` or `.ssz`, choosing the reader by sniffing the Zip local-header
  magic (`PK\x03\x04`) or the `.ssz` extension, else the plain `.ssc` reader.
- **PORT-F11 [Done]** WHILE importing, FOR each category whose name already exists
  THE SYSTEM SHALL prompt (localized Yes/No) before overwriting; it SHALL count
  imported categories and, on success, save to the registry (→
  [`persistence.spec.md`](persistence.spec.md)) and show a localized success
  message; on parse failure it SHALL show a localized failure message.
- **PORT-F12 [Done]** IF the imported file carries a board style THEN THE SYSTEM
  SHALL apply/prompt per [`board-style.spec.md`](board-style.spec.md) STY-F53.
- **PORT-F13 [Done]** WHEN a `.ssc`/`.ssz` path is passed on the command line THE
  SYSTEM SHALL import it — forwarding the path to an already-running instance via
  `WM_COPYDATA` (`SIMONSAYS_COPYDATA_IMPORT_SSC`) and exiting, or importing it in
  the newly started instance after the window is up.

### 6.3 `.ssc` format

- **PORT-F20 [Done]** THE `.ssc` file SHALL be UTF-8 with a BOM, a first line
  `SIMONSAYS_CATEGORIES_V1`, then one line per category
  `<icon>##<name>=<style-token?|phrase|phrase…>` plus an optional `$$board=…`
  line; parsing SHALL skip blank lines and lines without `=`, and reject a file
  whose header does not match.

### 6.4 `.ssz` bundle

- **PORT-F30 [Done]** THE `.ssz` SHALL be a Zip archive containing exactly one
  `categories.ssc` at the root and referenced resources under `resources/`; only
  the supported icon formats (`.ico`/`.png`/`.jpg`/`.jpeg`, shared predicate
  `HasSupportedIconExt`) plus `.wav` and `.mp3` SHALL be bundled. *(Amended
  2026-07-11: icon formats broadened from `.ico`-only in step with
  [`categories-phrases.spec.md`](categories-phrases.spec.md) CAT-F31; rendering
  of imported images is hardened per [`ssbutton.spec.md`](ssbutton.spec.md)
  BTN-F13/F14 — decode cap + no-crash on corrupt files.)*
- **PORT-F31 [Done]** ON export THE SYSTEM SHALL collect existing referenced
  resources (from the app-data folder by default; optionally the working/exe
  folders) and rewrite each icon/audio reference to its bare basename so it
  resolves after import.
- **PORT-F32 [Done]** ON import THE SYSTEM SHALL reconcile each asset reference:
  **bundled** → keep (extracted basename); **not bundled but resolvable locally**
  → keep; **neither** → strip the reference.
- **PORT-F33 [Done]** THE SYSTEM SHALL install bundled resources with a
  **two-phase commit** — extract to a unique temp directory, then copy into the
  resource folder only after every entry has been written successfully.
- **PORT-F34 [Done]** THE `.ssz` reader SHALL enforce zip-bomb limits (≤
  `SSZ_MAX_ENTRIES` entries, ≤ `SSZ_MAX_ENTRY_UNCOMPRESSED` per entry, ≤
  `SSZ_MAX_TOTAL_UNCOMPRESSED` total, ≤ `SSZ_MAX_COMPRESSION_RATIO` per-entry
  ratio) and reject unexpected/duplicate entries, returning a diagnostic
  `errorDetail`.

### 6.5 Non-functional

- **PORT-N01 [Done]** Zip handling SHALL use the bundled single-file **miniz**
  (`src/miniz.c`); no external tools or admin rights are required.
- **PORT-N02 [Done]** All user-facing prompts/messages SHALL be localized.
- **PORT-N03 [Done]** Import failures SHALL be non-destructive and surface a
  detail string via `OutputDebugString` (`[SSZ import] …`).

## 7. Architecture & components

### 7.1 Files

| File | Responsibility |
|---|---|
| [`src/utils.cpp`](../../src/utils.cpp) | `.ssc` serialize/parse; `.ssz` export/import; resource collection/reconciliation; file-picker prompts; `IsZipArchive`. |
| [`include/utils.h`](../../include/utils.h) | Import/export API declarations. |
| [`src/CategoryWindow.cpp`](../../src/CategoryWindow.cpp) | `ImportCategories` / `ExportCategories` flows (prompts, format auto-pick, board-style scope). |
| [`src/main.cpp`](../../src/main.cpp) | Command-line `.ssc`/`.ssz` handling; `WM_COPYDATA` forwarding to a running instance. |
| [`src/miniz.c`](../../src/miniz.c) / [`include/miniz.h`](../../include/miniz.h) | Zip reader/writer. |
| [`include/stdafx.h`](../../include/stdafx.h) | `SSZ_*` constants; `SIMONSAYS_COPYDATA_IMPORT_SSC`. |

### 7.2 Layering

`utils` owns the formats and depends on `miniz` + the shared serialization;
`CategoryWindow` drives the UX and delegates storage to `RegistryManager`;
`main.cpp` bridges the shell/command line to the running window. Imported data
lands in the registry-backed model (→ [`persistence.spec.md`](persistence.spec.md)).

## 8. Detailed design

### 8.1 `.ssc` byte layout

```
EF BB BF                      (UTF-8 BOM)
SIMONSAYS_CATEGORIES_V1\n
$$board=$$<style>\n           (optional; board style)
<icon>##<name>=<$$style?|phrase|phrase…>\n
…
```

Written by `SerializeCategoriesToUtf8`, read by `ParseCategoriesFromUtf8`
(splits each line at the first `=`; reserved `$$…` names are handled per
[`board-style.spec.md`](board-style.spec.md)). Marker grammar is owned by
[`categories-phrases.spec.md`](categories-phrases.spec.md) §8.

### 8.2 `.ssz` layout

```
categories.ssc               (root; the .ssc byte stream above)
resources/<basename>         (each bundled .ico/.png/.jpg/.wav/.mp3)
```

Export: `CollectResources` finds existing referenced assets, refs are rewritten
to basenames, the `.ssc` is generated, then entries are added and the heap
archive is written to disk (removed on write failure). Import:
`ImportCategoriesFromSsz` reads the whole archive (bounded), validates entries,
extracts and parses `categories.ssc`, reconciles asset refs (§6.4), and installs
resources via two-phase commit.

### 8.3 File-association / single-instance

`main.cpp` parses the command line (`CommandLineToArgvW`); on a `.ssc`/`.ssz`
argument it finds an existing window (`FindWindow(CLASS_NAME)`) and sends the
path via `WM_COPYDATA` with `dwData == SIMONSAYS_COPYDATA_IMPORT_SSC` (then
exits), or lets the newly started instance import it once its window exists. The
`WM_COPYDATA` handler calls `CategoryWindow::ImportCategories(filePath)`.

## 9. Data model & persistence

Import/export operates on `std::vector<Category>` (+ a board-style string) and,
on successful import, persists via `RegistryManager::SaveCategoriesToRegistry`
(→ [`persistence.spec.md`](persistence.spec.md)). The formats mirror the registry
serialization; this spec owns the **file** carriers only.

## 10. Key interfaces

```cpp
// utils.h
bool ExportCategoriesToFile( const std::vector<Category> &, const std::wstring & filePath,
                             const std::wstring & boardStyle = L"" );
bool ImportCategoriesFromFile( const std::wstring & filePath, std::vector<Category> & out,
                               std::wstring * outBoardStyle = nullptr );
bool ExportCategoriesToSsz( const std::vector<Category> &, const std::wstring & filePath,
                            const std::wstring & resourceFolder, bool appDataOnly = true,
                            const std::wstring & boardStyle = L"" );
bool ImportCategoriesFromSsz( const std::wstring & filePath, const std::wstring & resourceFolder,
                              std::vector<Category> & out, std::wstring & errorDetail,
                              std::wstring * outBoardStyle = nullptr );
bool CategoriesHaveBundledResources( const std::vector<Category> &, const std::wstring & resourceFolder,
                                     bool appDataOnly = true );   // auto-format choice
bool IsZipArchive( const std::wstring & filePath );              // sniffs "PK\x03\x04"
std::wstring PromptExportCategoriesFilePath( HWND, const std::wstring & lang,
                                             const std::wstring & suggested = L"",
                                             const std::wstring & defaultExt = L"ssc" );
std::wstring PromptImportCategoriesFilePath( HWND, const std::wstring & lang );
```

## 11. UI specification

- **Export (`F10`):** optional "export only the selected category?" Yes/No; a
  save-file dialog (default extension per the auto-pick); a localized
  success/failure message.
- **Import (`F9` or file association):** an open-file dialog (`.ssc;*.ssz`
  filter) for the manual path; a per-existing-category overwrite Yes/No prompt; a
  board-style replace prompt when applicable (→ board-style STY-F53); a localized
  success/failure message. No window of its own.

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| `.ssc` entry name in bundle | `categories.ssc` | `stdafx.h` `SSZ_SSC_ENTRY_NAME` |
| Resource prefix in bundle | `resources/` | `stdafx.h` `SSZ_RESOURCE_PREFIX` |
| Max entries | 5000 | `stdafx.h` `SSZ_MAX_ENTRIES` |
| Max total uncompressed | 256 MB | `stdafx.h` `SSZ_MAX_TOTAL_UNCOMPRESSED` |
| Max per-entry uncompressed | 64 MB | `stdafx.h` `SSZ_MAX_ENTRY_UNCOMPRESSED` |
| Max per-entry compression ratio | 100 | `stdafx.h` `SSZ_MAX_COMPRESSION_RATIO` |
| Bundled resource extensions | `.ico`, `.png`, `.jpg`, `.jpeg`, `.wav`, `.mp3` | `utils.cpp` `HasAllowedResourceExt` (icons via `HasSupportedIconExt`) |
| `.ssc` header | `SIMONSAYS_CATEGORIES_V1` | `utils.cpp` |
| File extensions | `.ssc`, `.ssz` | `utils.cpp` |
| COPYDATA import id | `0x53534331` ("SSC1") | `stdafx.h` `SIMONSAYS_COPYDATA_IMPORT_SSC` |

## 13. Diagnostics

`.ssz` import failures set an `errorDetail` string, logged by the caller as
`[SSZ import] <detail>` via `OutputDebugString`; there is no user-facing
diagnostic beyond the localized success/failure message.

## 14. Edge cases & error handling

- **Wrong/short header** → `.ssc` parse returns false → localized failure.
- **Unreadable / oversized archive** (> total limit) → import fails with detail.
- **Zip-bomb / bad entry** (too many entries, oversized, bad ratio, unexpected
  name) → rejected with detail; nothing is installed.
- **Dangling asset reference** (neither bundled nor locally resolvable) → the
  reference is stripped, the phrase/category still imports.
- **Overwrite declined** → that category is skipped; the import continues.
- **Resource write failure** during the temp-dir phase → the two-phase commit
  aborts before copying into the resource folder (no partial install).
- **Export write failure** → the partially written file is deleted.

## 15. Acceptance criteria (testable)

Reverse-engineered from shipping behavior; **[Pass]** reflects the code path.

- **AC-1 (PORT-F01/F02/F04) [Pass]** F10 offers all-vs-selected, auto-picks
  `.ssz` when assets exist else `.ssc`, suggests a sanitized filename, and reports
  a localized result.
- **AC-2 (PORT-F03) [Pass]** Export-all carries `$$board`; export-selected carries
  only that category's style.
- **AC-3 (PORT-F10/F11) [Pass]** F9 imports `.ssc`/`.ssz`; existing categories
  prompt to overwrite; a localized success/failure message follows.
- **AC-4 (PORT-F13) [Pass]** Opening a `.ssc`/`.ssz` file imports it into a running
  instance (or a fresh one) via `WM_COPYDATA`.
- **AC-5 (PORT-F20/F30) [Pass]** A `.ssc` round-trips (BOM + header + lines); a
  `.ssz` round-trips with `categories.ssc` + `resources/` assets.
- **AC-6 (PORT-F31/F32/F33) [Pass]** Referenced `.ico`/`.png`/`.jpg`/`.wav`/`.mp3`
  assets are bundled on export and installed (two-phase) on import; dangling refs
  are stripped.
- **AC-8 (PORT-F30) [Pass]** Exporting a category whose icon is a `.png`
  produces a `.ssz` containing it under `resources/`; importing on a clean setup
  restores the icon on the button. *(Verified manually on `x64\Release`,
  2026-07-11.)*
- **AC-7 (PORT-F34) [Pass]** A malformed/oversized/zip-bomb archive is rejected
  with a detail string and installs nothing.

Build gate: Debug **and** Release x64 compile clean (no code change in this
authoring pass).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Export (all / selected) | ✅ Done | prompt + auto-format |
| Board-style export scope | ✅ Done | all→`$$board`, selected→own style |
| Import (+ overwrite prompt) | ✅ Done | localized messages |
| File-association import | ✅ Done | `WM_COPYDATA` forward / fresh instance |
| `.ssc` format | ✅ Done | BOM + header + lines |
| `.ssz` bundle + assets | ✅ Done | miniz; `.ico`/`.png`/`.jpg`/`.wav`/`.mp3` |
| Resource reconciliation | ✅ Done | bundled / local / strip |
| Two-phase asset install | ✅ Done | temp dir → copy on success |
| Zip-bomb hardening | ✅ Done | entries/size/ratio limits |

## 17. Known limitations

- **Bundled asset types are `.ico`/`.png`/`.jpg`/`.wav`/`.mp3` only.** Playable
  audio also includes `.mid`/`.midi` (see [`sound.spec.md`](sound.spec.md)), but
  those are **not** carried in a `.ssz` — a phrase referencing a `.mid` file
  imports with a dangling/local-only reference.
- A `.ssz` holds exactly one `categories.ssc` (no multi-document bundles).
- Import is per-file; there is no batch/folder import.
- Assets are installed into the app-data folder only (not per-language).

## 18. Future work

- Widen `.ssz` bundling to `.mid`/`.midi` to match the playable set.
- Optional merge (vs overwrite) strategies for colliding categories.

## 19. Open questions

1. Should a dangling asset reference warn the user, or stay silent (current)?
2. Should bundling and the playable-audio set be unified (see §17)?

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of PORT-SPEC v1.1.*
