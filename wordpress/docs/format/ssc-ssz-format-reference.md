> **Vendored copy** - do not edit here. Canonical source:
> https://github.com/juan-rey/SimonSays/blob/main/docs/guides/ssc-ssz-format-reference.md
> Vendored 2026-07-17 (format as of SimonSays desktop 0.7 development).
> License: CC BY 4.0 (see the metadata table below). Refresh this copy from
> the canonical file whenever the format changes.

# SimonSays — `.ssc` / `.ssz` Format Reference

**Audience:** integrators reading, generating, or validating SimonSays board
files outside the app (tooling, conversion, board libraries).

| | |
|---|---|
| **Applies to** | SimonSays – Simply Speak (import/export formats) |
| **Authoritative spec** | [`docs/specs/import-export.spec.md`](https://github.com/juan-rey/SimonSays/blob/main/docs/specs/import-export.spec.md) (`PORT-` requirements) |
| **Authoritative code** | [`src/utils.cpp`](https://github.com/juan-rey/SimonSays/blob/main/src/utils.cpp), `SSZ_*` in [`include/stdafx.h`](https://github.com/juan-rey/SimonSays/blob/main/include/stdafx.h) |
| **Related** | [`board-style.spec.md`](https://github.com/juan-rey/SimonSays/blob/main/docs/specs/board-style.spec.md) / [`board-style-reference.md`](board-style-reference.md) (`$$board` / `$$` style tokens) |
| **License (this guide only)** | **CC BY 4.0** — you may copy this reference into integration projects with attribution ("SimonSays .ssc/.ssz format reference, © Juan Rey Saura, CC BY 4.0"). The SimonSays application itself is licensed separately (OSL-3.0). |
| **Downstream consumers** | `simonsays-web` repository — vendored copy under `docs/format/`. Refresh vendored copies whenever this guide changes (PORT-N04). |

> This is a **practical reference**, not the contract. The spec (`PORT-F*`/`PORT-N*`)
> and the code are authoritative; where this guide and the code disagree, the
> code wins and this guide is the bug. This guide **must** be kept in sync with
> any format change (import-export.spec.md **PORT-N04**).

---

## Contents
- [1. Two formats at a glance](#1-two-formats-at-a-glance)
- [2. `.ssc` — the text format](#2-ssc--the-text-format)
- [3. Line grammar](#3-line-grammar)
- [4. `.ssz` — the bundle format](#4-ssz--the-bundle-format)
- [5. Resources: bundling & reconciliation](#5-resources-bundling--reconciliation)
- [6. Security model (reading untrusted bundles)](#6-security-model-reading-untrusted-bundles)
- [7. Import & export behavior](#7-import--export-behavior)
- [8. Constants](#8-constants)
- [9. Worked examples](#9-worked-examples)
- [10. Generating a valid file — checklist](#10-generating-a-valid-file--checklist)

---

## 1. Two formats at a glance

| | `.ssc` | `.ssz` |
|---|---|---|
| **What** | Plain UTF-8 text: categories, phrases, and the board style. | A **Zip bundle**: one `.ssc` plus the referenced icon/audio files. |
| **Use** | Portable text; no assets. | Self-contained board — assets travel with it. |
| **Contains** | Header + one line per category (+ optional `$$board` line). | `categories.ssc` at the root + `resources/<file>` assets. |
| **Zip library** | — | Bundled single-file **miniz** (`src/miniz.c`); no external tools. |

Export **auto-picks** the format: `.ssz` when any exported category/phrase
references an icon/audio file that exists on disk, else `.ssc` (the user can
still force an extension). Import accepts either, detected by sniffing the Zip
local-header magic `50 4B 03 04` (`PK\x03\x04`), else the `.ssz` extension, else
the plain `.ssc` reader.

## 2. `.ssc` — the text format

Byte layout (written by `SerializeCategoriesToUtf8`, read by
`ParseCategoriesFromUtf8`):

```
EF BB BF                                   UTF-8 BOM
SIMONSAYS_CATEGORIES_V1\n                   header (exact)
$$board=$$<style list>\n                   optional; board style (any position, conventionally first)
<icon>##<name>=<$$style?|phrase|phrase…>\n  one line per category
…
```

Reader rules:

- The file **must** begin with the header line `SIMONSAYS_CATEGORIES_V1` (after
  an optional BOM and after trimming a trailing `\r`). A wrong/short header →
  the whole import fails.
- **Encoding is UTF-8.** The BOM is optional on read but always written.
- **Line endings:** `\n` or `\r\n` (a trailing `\r` is trimmed per line).
- **Blank lines are skipped.** A line **without `=` is skipped** (not an error).
- Each category line is split at the **first `=`**. Everything left of it is the
  category (icon + name); everything right is the category's phrase data.

## 3. Line grammar

The same marker grammar is single-sourced with the registry serialization
(`SerializePhrase` / `DeserializePhrase` in `utils.cpp`). Markers:

| Marker | Constant | Role |
|---|---|---|
| `##` | `ICON_SEPARATOR` | Separates an icon token from the following name/text: `<icon>##<name>`. |
| `::` | `AUDIO_FILE_SEPARATOR` | Separates a phrase's text from its audio filename: `<text>::<file>`. |
| `\|` | `CATEGORY_PHRASE_SEPARATOR` | Separates phrases (and style tokens) within a category's data. |
| `$$` | `STYLE_TOKEN_PREFIX` | Marks a **style token** (not a phrase) in phrase-position data. |
| `♫` | `SOUND_NOTE_DELIMITER` | *In-text* sound marker used at speak time; not part of the stored phrase record. |

### Category (left of `=`)

```
<icon>##<name>
```

- `icon` (optional): an emoji, or a bare icon filename ending `.ico`/`.png`/
  `.jpg`/`.jpeg`. With no `##`, the whole left side is the name.
- `name`: the category name. **Reserved:** `$$board` is the board-style line, not
  a category; names starting with `$$` are rejected by the app.

### Category data (right of `=`)

A `|`-separated list. Each token is either:

- a **style token** `$$<style list>` — extracted into the category's style
  (multiple concatenated in order), removed before the UI sees it; or
- a **phrase** `<icon>##<text>::<audioFile>`, where:
  - `icon` (optional, before `##`): emoji or icon filename.
  - `text`: the spoken text. May itself contain inline `♫<file>♫` sound markers
    (resolved at speak time, not here).
  - `audioFile` (optional, after `::`): a bare `.wav`/`.mp3` filename played when
    the phrase is used.

Empty tokens are skipped.

### `$$board` line

```
$$board=$$<style list>
```

Data is one or more style-list tokens joined by `|`; a leading `$$` on each token
is accepted and stripped. See the
[board style reference](board-style-reference.md) for the style-list grammar.

## 4. `.ssz` — the bundle format

A `.ssz` is a plain Zip archive with a fixed, flat layout:

```
categories.ssc                 exactly one, at the archive root (the §2 byte stream)
resources/<basename>           each bundled asset (flat; no nested dirs)
```

- **Exactly one** `categories.ssc` at the root — zero or more than one is
  rejected.
- Assets live directly under `resources/` with **no** further subdirectories.
- Bundled asset types: `.ico`, `.png`, `.jpg`, `.jpeg` (icons) and `.wav`, `.mp3`
  (audio). Anything else under `resources/` is rejected on read.
- Entry filenames are stored as UTF-8. The bundle layout is **flat** regardless
  of where the app installs the files locally (a board's `title` may route them
  into a subfolder on import — that is a local concern, not part of the format;
  see the board style reference §7).
- No compression method is mandated; miniz reads stored/deflated entries.

## 5. Resources: bundling & reconciliation

**On export (`PORT-F31`):** the app collects the referenced icon/audio files that
exist on disk — searching the active board's resource subfolder first, then the
app-data root (optionally working/exe dirs) — copies them under `resources/`, and
**rewrites each reference to its bare basename** in the embedded `.ssc` so it
resolves after import. A referenced file that does not exist is not bundled.

**On import (`PORT-F32`)** each asset reference is reconciled to one of:

| Case | Outcome |
|---|---|
| **Bundled** in the archive | keep, using the extracted basename |
| **Not bundled but resolvable locally** (board subfolder → app-data root → working → exe) | keep (e.g. resources shipped with the app) |
| **Neither** | strip the reference (the phrase/category still imports, just without that asset) |

**Install (`PORT-F33`)** uses a **two-phase commit**: entries are extracted to a
unique temp directory, and only after *every* entry is written successfully are
they copied into the target folder — chosen **after** the board-style adoption
decision (the board's resource subfolder if its style defines one, else the
app-data root). A failed commit installs nothing and the whole import is reported
as failed.

## 6. Security model (reading untrusted bundles)

A `.ssz` may come from an untrusted sender, so the reader is hardened
(`ImportCategoriesFromSsz`). An integrator generating files should stay well
inside these limits; a validator can mirror them:

- **Entry-name safety:** reject absolute paths, `\` (backslash), `:`, control
  characters, and any `..` traversal. `resources/` entries must be a single flat
  name with an allowed extension. Symlink entries (Unix mode bits) are rejected.
- **Structural:** exactly one root `categories.ssc`; any unexpected entry name →
  reject.
- **Zip-bomb limits:** at most `SSZ_MAX_ENTRIES` entries; each entry ≤
  `SSZ_MAX_ENTRY_UNCOMPRESSED`; total uncompressed ≤ `SSZ_MAX_TOTAL_UNCOMPRESSED`;
  per-entry compression ratio ≤ `SSZ_MAX_COMPRESSION_RATIO`.
- Failures are **non-destructive** and surface a short diagnostic via
  `OutputDebugString` as `[SSZ import] <detail>` (no user-facing detail beyond the
  localized success/failure message).

## 7. Import & export behavior

- **Export (`F10`):** all categories or just the selected one (Yes/No prompt when
  a category is selected). Suggested filename = date + user (or category) name +
  native language name, with `&` and spaces sanitized. Export-all carries
  `$$board`; export-selected carries only that category's own style.
- **Import (`F9`):** open-file dialog (`*.ssc;*.ssz`); each already-existing
  category prompts (Yes/No) before overwriting; a localized success/failure
  message follows; a carried board style applies/prompts per STY-F53.
- **File association / command line (`PORT-F13`):** launching the app with a
  `.ssc`/`.ssz` path imports it — forwarded to an already-running instance via
  `WM_COPYDATA` (`dwData == SIMONSAYS_COPYDATA_IMPORT_SSC`, `0x53534331` =
  "SSC1"), or imported by the fresh instance once its window is up.

## 8. Constants

Single-sourced in [`include/stdafx.h`](https://github.com/juan-rey/SimonSays/blob/main/include/stdafx.h):

| Constant | Value |
|---|---|
| `.ssc` header | `SIMONSAYS_CATEGORIES_V1` |
| Bundle `.ssc` entry name (`SSZ_SSC_ENTRY_NAME`) | `categories.ssc` |
| Bundle resource prefix (`SSZ_RESOURCE_PREFIX`) | `resources/` |
| Max entries (`SSZ_MAX_ENTRIES`) | 5000 |
| Max total uncompressed (`SSZ_MAX_TOTAL_UNCOMPRESSED`) | 256 MB |
| Max per-entry uncompressed (`SSZ_MAX_ENTRY_UNCOMPRESSED`) | 64 MB |
| Max per-entry ratio (`SSZ_MAX_COMPRESSION_RATIO`) | 100 |
| Bundled extensions | `.ico` `.png` `.jpg` `.jpeg` `.wav` `.mp3` |
| Markers | `##` `::` `\|` `$$` `♫` |
| COPYDATA import id (`SIMONSAYS_COPYDATA_IMPORT_SSC`) | `0x53534331` |
| Board resource folder name cap (`BOARD_RESOURCE_FOLDER_MAX_NAME`) | 64 |

## 9. Worked examples

### 9.1 Minimal `.ssc`

```
SIMONSAYS_CATEGORIES_V1
Greetings=Hello|Goodbye|Thank you
```

One category, three plain phrases, no icons, no style.

### 9.2 `.ssc` with icons, audio, and style

```
SIMONSAYS_CATEGORIES_V1
$$board=$$background:#FBFAFD;caption:Demo;title:Demo Board;
🙂##Greetings=$$background:#93C47D;|👋##Hello|Goodbye::bye.mp3|🔔##Attention::chime.wav
```

- `$$board` line: board background + strip caption + title.
- `Greetings` category: emoji icon `🙂`, own `background` (flows to its phrases),
  then three phrases — an emoji-iconned `Hello`, a `Goodbye` that plays
  `bye.mp3`, and an emoji-iconned `Attention` that plays `chime.wav`.

### 9.3 `.ssz` of 9.2

```
board.ssz  (Zip)
├── categories.ssc          # the bytes above, with bye.mp3 / chime.wav references as bare basenames
└── resources/
    ├── bye.mp3
    └── chime.wav
```

(The emoji icons need no files; only the `.mp3`/`.wav` are carried.)

## 10. Generating a valid file — checklist

For a `.ssc`:

- [ ] Write the UTF-8 BOM (optional) then `SIMONSAYS_CATEGORIES_V1\n` exactly.
- [ ] One category per line; split point is the **first** `=`.
- [ ] Escape nothing — instead **avoid** the reserved markers inside values:
      no `|`, `##`, `::`, `=` inside a name/text where it would be misparsed; no
      `|`/`;` inside style free-text.
- [ ] Put styles as `$$<style>` tokens in phrase-position data (or the `$$board`
      line); never in the name.
- [ ] Reference assets by **bare filename** (`bye.mp3`), not a path.

For a `.ssz`, additionally:

- [ ] Exactly one `categories.ssc` at the archive root.
- [ ] Every asset flat under `resources/` with an allowed extension.
- [ ] Bundle only files the `.ssc` actually references (bare basenames match).
- [ ] Stay within the size/entry/ratio limits (§6); no `..`, absolute paths,
      backslashes, or symlinks in entry names.

---

*Reference guide — see [`docs/specs/import-export.spec.md`](https://github.com/juan-rey/SimonSays/blob/main/docs/specs/import-export.spec.md)
for the authoritative `PORT-` requirements and acceptance criteria, and the
[board style reference](board-style-reference.md) for the style-list grammar.*
