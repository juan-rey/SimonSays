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

## 1. Product overview

*(Drawn only from [`README.md`](../README.md) and [`HELP.md`](../HELP.md).)*

**SimonSays — Simply Speak** is an accessibility-focused, Windows tray/taskbar
text-to-speech (TTS) assistant for people living with ALS and other speech
impairments. The user types (or selects a saved phrase) and SimonSays speaks it
aloud immediately using installed Windows (SAPI) voices.

**Key differentiator:** SimonSays lives in the empty space on the left of the
Windows taskbar and floats above whatever the user is doing — so the user can
stay in their video call, document, or browser and still speak, **without app
switching or context loss**. It is lightweight and always ready in the tray.

**Headline capabilities (as documented):**

- Instant type-and-speak via installed SAPI voices, with adjustable
  volume/rate and a voice test.
- Customizable **categories** of frequently used **phrases**, a couple of clicks
  away, with optional emoji or `.ico` icons.
- Inline **sound markers** (`♫`) to play short `.wav` / `.mid` / `.midi` / `.mp3`
  effects alongside spoken output.
- **Import/export** of categories and phrases, bundling icons and audio.
- **Localization** across the supported languages (built-in phrase sets + UI
  labels).
- Experimental **Gaze / Dwell-click** eye-tracking activation.
- Playback behaviors: stop-previous, raise own volume, and duck other apps
  while speaking.

**Target users:** people with ALS, dysarthria, laryngectomy, or other
conditions that make speaking difficult; and the caregivers/therapists who set
the app up for them.

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
header metadata table → How to use this spec → Overview → Background & context →
Goals & non-goals → Glossary → Personas & scenarios → Requirements (EARS) →
Architecture → Detailed design → Data model & persistence → Key interfaces →
UI specification → Configuration constants → Diagnostics → Edge cases →
Acceptance criteria → Status matrix → Known limitations → Future work → Open
questions → Build & run → Appendices. Sections that don't apply to a module may
be marked *N/A* rather than removed.

### 2.7 Build & run (shared)

```
MSBuild SimonSays.vcxproj /p:Configuration={Debug|Release} /p:Platform=x64
```

Verify against `x64\Release\SimonSays.exe`. Register new files in
`SimonSays.vcxproj(.filters)`. (See [`AGENT.md`](../AGENT.md) §5.)

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

> TODO: extend as sub-specs introduce shared terms. Module-specific terms stay
> in the module's own glossary.

---

## 5. Sub-spec index

| Prefix | Module | Spec file | Status | One-line scope |
|---|---|---|---|---|
| `REQ-` (DWELL) | Gaze / Dwell-click | [`docs/specs/dwell.spec.md`](specs/dwell.spec.md) | **Active** | Hands-free button activation by gaze/cursor dwell (cursor + HID), mode detection, calibration, settings. |
| `TTS-` | Text-to-Speech / SAPI voice playback | [`docs/specs/tts.spec.md`](specs/tts.spec.md) | Draft | Speaking typed/selected text via installed SAPI voices; voice selection, volume/rate, voice test, fallback voice. |
| `SND-` | Sound markers & audio playback | [`docs/specs/sound.spec.md`](specs/sound.spec.md) | Draft | Inline `♫` sound markers; `.wav/.mid/.midi/.mp3` playback; sound/icon file lookup order; built-in fallback sound. |
| `CAT-` | Categories & Phrases management | [`docs/specs/categories-phrases.spec.md`](specs/categories-phrases.spec.md) | Draft | Category/phrase browse + CRUD; `##` icon prefix (emoji/`.ico`); `::` inline-audio suffix; immediate-speak option. |
| `PORT-` | Import / Export (bundles) | [`docs/specs/import-export.spec.md`](specs/import-export.spec.md) | Draft | Import/export of categories & phrases bundling icon and audio files; overwrite prompts; per-language result messages. |
| `SET-` | Settings (general + window behavior) | [`docs/specs/settings.spec.md`](specs/settings.spec.md) | Draft | Settings window: default text, touch keyboard, UI language label, voice/volume/rate, category-window behavior, playback behavior; tray menu & window move/resize. |
| `LOC-` | Localization / multi-language support | [`docs/specs/localization.spec.md`](specs/localization.spec.md) | Draft | UI language labels, built-in per-language phrase defaults, localized help files, English fallback for missing strings. |
| `REG-` | Settings / registry persistence | [`docs/specs/persistence.spec.md`](specs/persistence.spec.md) | Draft | HKCU registry storage of settings and default phrase sets; first-run defaults; load/save lifecycle; non-overwrite of existing phrases. |

> **Do not invent modules.** Add a new row only when the module is evidenced in
> `README.md` / `HELP.md` (or an existing spec). If you think another module
> exists, raise it as an open question (§6) first.

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
