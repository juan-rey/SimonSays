# SimonSays — Master Specification & Index

| | |
|---|---|
| **Spec ID** | SIMONSAYS-SPEC |
| **Status** | Active (index) — module sub-specs at varying maturity (see §5) |
| **Version** | 0.1 (skeleton) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Agent guide** | [`AGENT.md`](../AGENT.md) |

> This is the **entry point** for Spec-Driven Development in this repository. It
> describes the product at a high level, defines the conventions every sub-spec
> must follow, and indexes the per-module sub-specs in [`docs/specs/`](specs/).
>
> For *how to work* with these specs as an AI agent or contributor, read
> [`AGENT.md`](../AGENT.md) first.

---

## Contents
- [1. Product overview](#1-product-overview)
- [2. Shared conventions (all sub-specs MUST follow)](#2-shared-conventions-all-sub-specs-must-follow)
- [3. Cross-module rules](#3-cross-module-rules)
- [4. Glossary (shared)](#4-glossary-shared)
- [5. Sub-spec index](#5-sub-spec-index)
- [6. Open questions (project-level)](#6-open-questions-project-level)

---

## 1. Product overview

*(Drawn only from [`README.md`](../README.md) and [`HELP.md`](../HELP.md).)*

**SimonSays — Simply Speak** is a Windows taskbar text-to-speech (TTS)
assistant. It began as a tool for people with ALS and others who can no longer
speak, and serves as a general AAC tool for anyone who finds speaking difficult
or impossible. The user types (or selects a saved phrase) and SimonSays speaks
it aloud immediately using installed Windows (SAPI) voices.

**Key differentiator:** SimonSays lives in the empty space on the Windows
taskbar — at any edge of the screen — and never covers the user's other
windows, so the user can stay in their video call, document, or browser and
still speak, **without app switching or context loss**, by keyboard, mouse, or
eye tracker. It is lightweight and always ready.

**Headline capabilities (as documented):**

- Instant type-and-speak via installed SAPI voices, with adjustable
  volume/rate and a voice test.
- Customizable **categories** of frequently used **phrases**, a couple of clicks
  away, with optional emoji or image-file icons (`.ico`/`.png`/`.jpg`); large,
  need-ordered default phrase sets and ready-made boards (Yes / No, Pain & body,
  Daily care, Fitzgerald Key examples…) in the boards folder.
- Inline **sound markers** (`♫`) to play short `.wav` / `.mp3` effects alongside
  spoken output (`.mid`/`.midi` names are accepted but do not play).
- **Import/export** of categories and phrases (`.ssc`/`.ssz`), bundling icons and
  audio.
- **Board & category styles** for color-coded boards such as Fitzgerald Key.
- **Localization** across the supported languages (built-in phrase sets + UI
  labels).
- **Gaze / Dwell-click** eye-tracking activation (cursor-based and direct gaze);
  supported, still needing field testing.
- A `Sound output` device choice, e.g. a virtual audio cable for online
  meetings.
- Playback behaviors: stop-previous, raise own volume, and duck other apps
  while speaking.

**Positioning:** SimonSays complements, and does not replace, full AAC suites
(Mind Express, TD Communicator, Grid 3, OptiKey). Everyday board edits happen in
the app; pictogram, custom-icon and styled boards are best built in the separate
SimonSays Web editor. It is free software (OSL-3.0) and is approaching 1.0.

**Target users:** people with ALS, dysarthria, laryngectomy, apraxia of speech,
stroke, brain injury or other conditions that make speaking difficult; and the
caregivers/therapists who set the app up for them.

> TODO: keep this section anchored strictly to README/HELP. Do not add product
> claims that aren't documented there.

---

## 2. Shared conventions (all sub-specs MUST follow)

Every sub-spec under [`docs/specs/`](specs/) follows the structure and
conventions established by the reference spec
[`docs/specs/dwell.spec.md`](specs/dwell.spec.md).

### 2.1 EARS requirement phrasing

Requirements use **EARS** (Easy Approach to Requirements Syntax) phrasing, e.g.:

- *Ubiquitous:* "THE SYSTEM SHALL …"
- *Event-driven:* "WHEN <trigger> THE SYSTEM SHALL …"
- *State-driven:* "WHILE <state> THE SYSTEM SHALL …"
- *Conditional:* "IF <condition> THEN THE SYSTEM SHALL …"

### 2.2 Requirement IDs & avoiding collisions

- Each requirement has a stable, traceable ID.
- **Per-module prefix scheme:** new sub-specs use a short module prefix plus a
  functional/non-functional letter and a two-digit number:
  - Functional: `<PREFIX>-F##` (e.g. `TTS-F01`)
  - Non-functional: `<PREFIX>-N##` (e.g. `TTS-N01`)
- IDs are **unique within their prefix**; prefixes are unique across modules, so
  IDs never collide project-wide.
- **Grandfathered:** [`docs/specs/dwell.spec.md`](specs/dwell.spec.md) uses bare `REQ-F*` /
  `REQ-N*` IDs scoped to that file. These stay as-is (do not renumber). New
  modules use the prefixed scheme above.
- Never reuse or renumber an existing ID. Retire one by marking it (e.g.
  *[Withdrawn]*) rather than deleting the number.

**Assigned prefixes:** see the index in §5.

### 2.3 Status tags

Per requirement and in the status matrix:

- **[Done]** — implemented and verified.
- **[Done*]** — implemented; depends on an environment-specific assumption
  validated in a limited setting.
- **[Partial]** — implemented in a hardcoded/simplified form.
- **[Planned]** — specified but not implemented.

### 2.4 Acceptance criteria format

Testable criteria, each referencing the requirement(s) it covers and carrying a
pass/pending marker, e.g.:

```
- AC-1 (TTS-F01/F02) [Pass]  <observable, testable statement>
- AC-2 (TTS-F05)     [Pending] <…>
```

### 2.5 Status matrix format

A table summarizing implementation state per functional area:

| Area | Status | Notes |
|---|---|---|
| <area> | ✅ Done / ⚠️ Done* / ⛔ Planned | <note> |

### 2.6 Section structure

Sub-specs mirror the section ordering of [`docs/specs/dwell.spec.md`](specs/dwell.spec.md):
header metadata table → **Table of contents** (§2.8) → How to use this spec →
Overview → Background & context → Goals & non-goals → Glossary → Personas &
scenarios → Requirements (EARS) → Architecture → Detailed design → Data model &
persistence → Key interfaces → UI specification → Configuration constants →
Diagnostics → Edge cases → Acceptance criteria → Status matrix → Known
limitations → Future work → Open questions → Build & run → Appendices. Sections
that don't apply to a module may be marked *N/A* rather than removed.

### 2.7 Build & run (shared)

**Win32 (32-bit) is the default platform**; `x64` remains supported.

```
MSBuild SimonSays.vcxproj /p:Configuration={Debug|Release} /p:Platform=Win32
```

Verify against `Release\SimonSays.exe`. Register new files in
`SimonSays.vcxproj(.filters)`. (See [`AGENT.md`](../AGENT.md) §5.)

### 2.8 Table of contents

Every spec file — this master `spec.md` and each sub-spec under
[`docs/specs/`](specs/) — MUST include a **Table of contents**: a `## Contents`
section placed immediately after the header metadata block (before the first
content section), listing the file's top-level (`##`) sections as
intra-document links. [`AGENT.md`](../AGENT.md) carries one too. Use
GitHub-style anchors (lowercase; punctuation removed; spaces → hyphens — so
`## 6. Requirements (EARS)` links to `#6-requirements-ears`). Keep the TOC in
sync whenever a top-level section is added, removed, or renamed.

---

## 3. Cross-module rules

> TODO: capture project-wide constraints that span modules as they are
> established (e.g. localization routing, registry/persistence ownership,
> dialog/RTL conventions). Until documented here, defer to the conventions shown
> in [`docs/specs/dwell.spec.md`](specs/dwell.spec.md) §7.4 / §9 / §11.

- All user-visible strings route through the localization layer (see the
  Localization sub-spec). English is the fallback language.
- Settings persistence is registry-backed under HKCU (see the Persistence
  sub-spec). Modules describe their own stored values; the Persistence spec owns
  the storage mechanism.

---

## 4. Glossary (shared)

| Term | Meaning |
|---|---|
| **AAC** | Augmentative and Alternative Communication. |
| **SAPI** | Microsoft Speech API — provides the installed Windows voices. |
| **Phrase** | A saved unit of text (optionally with icon and inline audio) the user can speak. |
| **Category** | A named group of phrases (optionally with an icon). |
| **Sound marker** | Inline audio reference delimited by `♫` within spoken text. |
| **Dwell-click** | Activating a control by holding the gaze/cursor on it (see [`docs/specs/dwell.spec.md`](specs/dwell.spec.md)). |
| **SSButton** | The app's custom owner-drawn Win32 button control every clickable surface is built from (see [`docs/specs/ssbutton.spec.md`](specs/ssbutton.spec.md)). |
| **Board style** | The category-window style cascade (`$$board` global layer + per-category styles) that colors/sizes the board, e.g. for Fitzgerald Key boards (see [`docs/specs/board-style.spec.md`](specs/board-style.spec.md)). |

> TODO: extend as sub-specs introduce shared terms. Module-specific terms stay
> in the module's own glossary.

---

## 5. Sub-spec index

| Prefix | Module | Spec file | Status | One-line scope |
|---|---|---|---|---|
| `REQ-` (DWELL) | Gaze / Dwell-click | [`docs/specs/dwell.spec.md`](specs/dwell.spec.md) | **Active** | Hands-free button activation by gaze/cursor dwell (cursor + HID), mode detection, calibration, settings. |
| `BTN-` | SSButton custom button control | [`docs/specs/ssbutton.spec.md`](specs/ssbutton.spec.md) | **Active** | Owner-drawn Win32 button: rendering, colors/theming, `.ico`/`.png`/`.jpg`/color-emoji icons, mouse/keyboard/focus activation, RTL & dialog integration; hosts the dwell-click surface (behavior owned by DWELL). |
| `TTS-` | Text-to-Speech / SAPI voice playback | [`docs/specs/tts.spec.md`](specs/tts.spec.md) | **Active** | Speaking typed/selected text via installed SAPI voices; voice selection, volume/rate, voice test, fallback voice. |
| `SND-` | Sound markers & audio playback | [`docs/specs/sound.spec.md`](specs/sound.spec.md) | **Active** | Inline `♫` sound markers; `.wav/.mp3` playback (`.mid/.midi` accepted, not played); sound/icon file lookup order; built-in fallback sound. |
| `CAT-` | Categories & Phrases management | [`docs/specs/categories-phrases.spec.md`](specs/categories-phrases.spec.md) | **Active** | Category/phrase browse + CRUD; `##` icon prefix (emoji/`.ico`/`.png`/`.jpg`); `::` inline-audio suffix; immediate-speak option; bulk entry from pasted text / dropped `.txt` files; ordering criteria for the built-in default sets. |
| `PORT-` | Import / Export (bundles) | [`docs/specs/import-export.spec.md`](specs/import-export.spec.md) | **Active** | Import/export of categories & phrases bundling icon and audio files; overwrite prompts; per-language result messages. |
| `SET-` | Settings (general + window behavior) | [`docs/specs/settings.spec.md`](specs/settings.spec.md) | **Active** | Settings window: default text, touch keyboard, UI language label, voice/volume/rate, category-window behavior, playback behavior; tray menu & window move/resize. |
| `LOC-` | Localization / multi-language support | [`docs/specs/localization.spec.md`](specs/localization.spec.md) | **Active** | UI language labels, built-in per-language phrase defaults, localized help files, English fallback for missing strings. |
| `REG-` | Settings / registry persistence | [`docs/specs/persistence.spec.md`](specs/persistence.spec.md) | **Active** | HKCU registry storage of settings and default phrase sets; first-run defaults; load/save lifecycle; non-overwrite of existing phrases. |
| `STY-` | Board & category styles | [`docs/specs/board-style.spec.md`](specs/board-style.spec.md) | **Active** | Category-window style cascade: `$$board` global layer + per-category `::` styles (`property:value;` syntax); Fitzgerald Key communication-board support; travels with import/export. |
| `TBR-` | Taskbar integration | [`docs/specs/taskbar.spec.md`](specs/taskbar.spec.md) | **Active** | Bar placement and layout on a taskbar at any edge (Regular/Thin tiers, vertical stack, dynamic width, live relayout), category window placement, and Z-order recovery; taskbar kept shown while running (auto-hide off, restored on exit, crash-safe); show-once Widgets message. |

> **Do not invent modules.** Add a new row only when the module is evidenced in
> `README.md` / `HELP.md` (or an existing spec). If you think another module
> exists, raise it as an open question (§6) first.

### Reference guides

Practical, integrator-facing companions to two sub-specs. They are licensed
**CC BY 4.0** (the app is OSL-3.0) and vendored into the `simonsays-web`
repository under `docs/format/`. The spec and the code stay authoritative; each
guide **must** be updated with any change to its area, and the vendored copies
refreshed.

| Guide | Covers | Companion spec | Sync rule |
|---|---|---|---|
| [`docs/guides/board-style-reference.md`](guides/board-style-reference.md) | Style-list syntax, value forms, cascade, property reference, storage, resource folder, Fitzgerald Key example | [`board-style.spec.md`](specs/board-style.spec.md) | STY-N02 |
| [`docs/guides/ssc-ssz-format-reference.md`](guides/ssc-ssz-format-reference.md) | `.ssc` text format, `.ssz` bundle, resource bundling, security limits, import/export behavior, `.txt` and sound-file input | [`import-export.spec.md`](specs/import-export.spec.md) | PORT-N04 |

---

## 6. Open questions (project-level)

> TODO. Examples to resolve as the specs mature:

1. Should tray-icon menu and window move/resize behavior remain folded into the
   Settings sub-spec (`SET-`), or graduate to their own sub-spec later?
2. Is there a distinct "main window / input box / Play" interaction worth its own
   sub-spec, or is it adequately covered across `TTS-`/`SET-`?
3. Ownership boundary between `CAT-` (in-memory model) and `REG-`/`PORT-`
   (persistence and bundling) — confirm where each requirement belongs.

---

*End of SIMONSAYS-SPEC (skeleton).*
