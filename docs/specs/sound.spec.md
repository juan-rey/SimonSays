# SimonSays — Sound Markers & Audio Playback — Specification

| | |
|---|---|
| **Spec ID** | SND-SPEC |
| **Status** | Draft (skeleton — no requirements authored yet) |
| **Version** | 0.1 |
| **REQ prefix** | `SND-F##` (functional), `SND-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Master spec** | [`docs/spec.md`](../spec.md) |

---

## 0. How to use this spec

See [`AGENT.md`](../../AGENT.md) and [`docs/spec.md`](../spec.md) §2 for shared
conventions. This file is the source of truth for **inline sound markers and
audio file playback**. Reference format: [`docs/specs/dwell.spec.md`](dwell.spec.md).

> Scope reminder: parsing `♫`-delimited markers, playing audio files, and the
> sound/icon file **lookup order**. Spoken text itself is in
> [`tts.spec.md`](tts.spec.md). The `::` per-phrase audio suffix authoring is in
> [`categories-phrases.spec.md`](categories-phrases.spec.md) (this spec owns how
> the resulting file is located and played).

**Source material to mine (do not invent beyond it):**
- [`README.md`](../../README.md) — "Usage — mixing speech and sounds"
  (`SOUND_NOTE_DELIMITER` ♫, supported formats `.wav/.mid/.midi/.mp3`),
  "Troubleshooting" (sound files not playing).
- [`HELP.md`](../../HELP.md) — "Mixing speech and sounds", "Sound and icon file
  lookup order" (AppData → working dir → exe folder → built-in fallback).

---

## 1. Overview

> TODO. Inline sound markers let short audio effects play alongside spoken text;
> text outside `♫` is spoken, text inside is treated as an audio filename.

## 2. Background & context

> TODO. Capture only documented behavior (delimiter `♫`, paired delimiters,
> supported extensions, lookup order, built-in fallback sound).

## 3. Goals & non-goals

**Goals** — TODO.
**Non-goals** — TODO.

## 4. Glossary

| Term | Meaning |
|---|---|
| Sound marker / `♫` (`SOUND_NOTE_DELIMITER`) | TODO |
| Inline audio | TODO |
| Fallback sound | TODO |

## 5. Personas & scenarios

> TODO. e.g. a phrase mixing speech and a notification chime.

## 6. Requirements (EARS)

> TODO — author from actual code/behavior. No requirements invented here.

### 6.1 Marker parsing (functional)

- **SND-F01 [Planned]** TODO — WHEN text contains paired `♫` markers THE SYSTEM
  SHALL … *(reserve SND-F02 …)*

### 6.2 Audio playback & formats (functional)

- **SND-F1# [Planned]** TODO (supported: `.wav`, `.mid`, `.midi`, `.mp3`).

### 6.3 File lookup order (functional)

- **SND-F2# [Planned]** TODO — search order: `%APPDATA%\SimonSays\` → working
  directory (if different from exe folder) → executable folder → built-in
  fallback. *(Confirm against code before finalizing.)*

### 6.4 Non-functional

- **SND-N01 [Planned]** TODO.

## 7. Architecture & components

> TODO.

## 8. Detailed design

> TODO.

## 9. Data model & persistence

> TODO / N/A (audio files are external assets; see lookup order).

## 10. Key interfaces

> TODO.

## 11. UI specification

> TODO / N/A.

## 12. Configuration & tuning constants

> TODO (e.g. the `♫` delimiter character; supported extension list).

## 13. Diagnostics

> TODO / N/A.

## 14. Edge cases & error handling

> TODO. e.g. unpaired delimiters cause trailing text to be treated as audio;
> unsupported extension; file not found → built-in fallback.

## 15. Acceptance criteria

> TODO.
- AC-1 (SND-F0?) [Pending] TODO.

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| `♫` marker parsing | ⛔ TODO | |
| Audio playback (`.wav/.mid/.midi/.mp3`) | ⛔ TODO | |
| Sound/icon lookup order | ⛔ TODO | |
| Built-in fallback sound | ⛔ TODO | |

## 17. Known limitations

> TODO.

## 18. Future work

> TODO.

## 19. Open questions

> TODO.

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of SND-SPEC (skeleton).*
