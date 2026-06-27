# SimonSays — Text-to-Speech / SAPI Voice Playback — Specification

| | |
|---|---|
| **Spec ID** | TTS-SPEC |
| **Status** | Draft (skeleton — no requirements authored yet) |
| **Version** | 0.1 |
| **REQ prefix** | `TTS-F##` (functional), `TTS-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Master spec** | [`docs/spec.md`](../spec.md) |

---

## 0. How to use this spec

See [`AGENT.md`](../../AGENT.md) and [`docs/spec.md`](../spec.md) §2 for the shared
working agreement, EARS phrasing, ID scheme, status tags, and acceptance-criteria
/ status-matrix formats. This file is the source of truth for **speaking text via
installed Windows (SAPI) voices**. Reference format: [`docs/specs/dwell.spec.md`](dwell.spec.md).

> Scope reminder: text-to-speech *output* and voice configuration only. Inline
> sound markers are specified in [`sound.spec.md`](sound.spec.md); the Settings
> window UI is in [`settings.spec.md`](settings.spec.md); persistence in
> [`persistence.spec.md`](persistence.spec.md).

**Source material to mine when filling this in (do not invent beyond it):**
- [`README.md`](../../README.md) — "Usage — mixing speech and sounds" (text spoken
  via Windows TTS), "Troubleshooting" (voice initialization / SAPI).
- [`HELP.md`](../../HELP.md) — "Launching and basic usage", "Settings (F2)" →
  Voice / Volume / Rate / Test Voice, "Troubleshooting" (voice init).

---

## 1. Overview

> TODO. One paragraph: SimonSays speaks typed or selected text aloud using
> installed SAPI voices, immediately and without app switching.

## 2. Background & context

> TODO. SAPI availability depends on voices installed in Windows; playback may
> fall back to another installed voice if the preferred one is unavailable
> (per README). Capture only what README/HELP state.

## 3. Goals & non-goals

**Goals** — TODO.
**Non-goals** — TODO (e.g. inline sound playback → see [`sound.spec.md`](sound.spec.md)).

## 4. Glossary

| Term | Meaning |
|---|---|
| SAPI | TODO |
| Voice | TODO |
| Rate / Volume | TODO |

## 5. Personas & scenarios

> TODO. e.g. user types a message and presses Play/Enter; user auditions a voice.

## 6. Requirements (EARS)

> TODO — author real requirements from the actual code/behavior. Reserve IDs in
> this module's range. No requirements invented in this skeleton.

### 6.1 Speaking text (functional)

- **TTS-F01 [Planned]** TODO — WHEN … THE SYSTEM SHALL …
- *(reserve TTS-F02 … as needed)*

### 6.2 Voice / volume / rate configuration (functional)

- **TTS-F1# [Planned]** TODO.

### 6.3 Non-functional

- **TTS-N01 [Planned]** TODO (e.g. SAPI/Windows-SDK only; no third-party deps).

## 7. Architecture & components

> TODO. Identify the owning module(s) in code before documenting (out of scope
> for this skeleton).

## 8. Detailed design

> TODO.

## 9. Data model & persistence

> TODO. Voice/volume/rate are stored settings — cross-reference
> [`persistence.spec.md`](persistence.spec.md); do not duplicate the registry
> table, link to it.

## 10. Key interfaces

> TODO.

## 11. UI specification

> TODO. Voice/Volume/Rate/Test-Voice controls live in the Settings window — see
> [`settings.spec.md`](settings.spec.md); reference, don't duplicate.

## 12. Configuration & tuning constants

> TODO (e.g. volume range 10–100, rate −10..10 per HELP — confirm against code).

## 13. Diagnostics

> TODO / N/A.

## 14. Edge cases & error handling

> TODO. e.g. voice initialization fails (SAPI missing / no voice); preferred
> voice unavailable → fallback (per README/HELP).

## 15. Acceptance criteria

> TODO.
- AC-1 (TTS-F0?) [Pending] TODO.

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Speak typed/selected text | ⛔ TODO | |
| Voice selection | ⛔ TODO | |
| Volume / rate | ⛔ TODO | |
| Test voice | ⛔ TODO | |
| Fallback voice | ⛔ TODO | |

## 17. Known limitations

> TODO. e.g. available voices depend on what Windows has installed.

## 18. Future work

> TODO.

## 19. Open questions

> TODO.

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of TTS-SPEC (skeleton).*
