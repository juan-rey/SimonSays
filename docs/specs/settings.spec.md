# SimonSays — Settings (General + Window Behavior) — Specification

| | |
|---|---|
| **Spec ID** | SET-SPEC |
| **Status** | Draft (skeleton — no requirements authored yet) |
| **Version** | 0.1 |
| **REQ prefix** | `SET-F##` (functional), `SET-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Master spec** | [`docs/spec.md`](../spec.md) |

---

## 0. How to use this spec

See [`AGENT.md`](../../AGENT.md) and [`docs/spec.md`](../spec.md) §2 for shared
conventions. This file is the source of truth for the **Settings window (F2)**,
the **tray icon menu**, and **window move/resize behavior**. Reference format:
[`docs/specs/dwell.spec.md`](dwell.spec.md).

> Scope reminder: the Settings dialog and its options, the tray menu entries, and
> dragging/resizing windows. Voice/TTS specifics are in [`tts.spec.md`](tts.spec.md);
> UI-language label selection mechanics are in [`localization.spec.md`](localization.spec.md);
> where settings are stored is in [`persistence.spec.md`](persistence.spec.md);
> the separate Gaze/Dwell settings window is in [`docs/specs/dwell.spec.md`](dwell.spec.md).

**Source material to mine (do not invent beyond it):**
- [`HELP.md`](../../HELP.md) — "Settings (F2)" (default text, touch keyboard,
  language labels, voice, volume/rate, test voice, category-window behavior,
  playback behavior: speak-on-select, stop-previous, increase own volume, duck
  others), "Tray icon menu", window move/resize note.
- [`README.md`](../../README.md) — shortcuts F1/F2/F3, voice/language in Settings.

---

## 1. Overview

> TODO. Central Settings window controlling default text, touch keyboard, UI
> language, voice and playback behavior, and category-window behavior; plus the
> tray menu and movable/resizable windows.

## 2. Background & context

> TODO.

## 3. Goals & non-goals

**Goals** — TODO.
**Non-goals** — TODO.

## 4. Glossary

| Term | Meaning |
|---|---|
| Default text | TODO |
| Touch keyboard | TODO |
| Audio ducking | TODO ("Temporarily reduce other audio when playing") |
| Tray menu | TODO |

## 5. Personas & scenarios

> TODO.

## 6. Requirements (EARS)

> TODO — author from actual code/behavior. No requirements invented here.

### 6.1 Default text & input (functional)

- **SET-F01 [Planned]** TODO (insert default text automatically; show touch
  keyboard on input focus). *(reserve SET-F02 …)*

### 6.2 Category-window behavior (functional)

- **SET-F1# [Planned]** TODO (minimize automatically after selecting; remember
  window size).

### 6.3 Playback behavior (functional)

- **SET-F2# [Planned]** TODO (speak-on-select; stop previous playback; increase
  SimonSays volume when playing; duck other audio when playing).

### 6.4 Tray menu (functional)

- **SET-F3# [Planned]** TODO (Show/Hide, Settings, Gaze/Dwell-click, About, Web,
  Feedback, Exit).

### 6.5 Window move/resize (functional)

- **SET-F4# [Planned]** TODO (drag title bars to move; resize Categories window
  edges; persistence of size gated by the "remember size" setting).

### 6.6 Non-functional

- **SET-N01 [Planned]** TODO.

## 7. Architecture & components

> TODO.

## 8. Detailed design

> TODO.

## 9. Data model & persistence

> TODO. All Settings fields persist via the registry — cross-reference
> [`persistence.spec.md`](persistence.spec.md); link, don't duplicate the table.

## 10. Key interfaces

> TODO.

## 11. UI specification

> TODO. Settings dialog layout & controls; OK saves / Cancel discards; tray menu
> items. Entry point: **F2** (Gaze/Dwell window is **F3** → separate spec).

## 12. Configuration & tuning constants

> TODO (e.g. volume 10–100, rate −10..10 — but those are owned by
> [`tts.spec.md`](tts.spec.md); link rather than duplicate).

## 13. Diagnostics

> TODO / N/A.

## 14. Edge cases & error handling

> TODO. e.g. restoring previous volume after playback (own-volume / ducking);
> UI language not applied → reopen Settings (per HELP troubleshooting).

## 15. Acceptance criteria

> TODO.
- AC-1 (SET-F0?) [Pending] TODO.

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Default text / touch keyboard | ⛔ TODO | |
| Category-window behavior | ⛔ TODO | |
| Playback behavior toggles | ⛔ TODO | |
| Tray menu | ⛔ TODO | |
| Window move/resize | ⛔ TODO | |

## 17. Known limitations

> TODO.

## 18. Future work

> TODO. e.g. whether tray-menu + window behavior graduate to their own sub-spec
> (open question in [`docs/spec.md`](../spec.md) §6).

## 19. Open questions

> TODO.

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of SET-SPEC (skeleton).*
