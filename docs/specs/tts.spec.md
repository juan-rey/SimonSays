# SimonSays — Text-to-Speech / SAPI Voice Playback — Specification

| | |
|---|---|
| **Spec ID** | TTS-SPEC |
| **Status** | Active — reverse-engineered from shipping source (2026-07-10) |
| **Version** | 1.0 (2026-07-10) |
| **REQ prefix** | `TTS-F##` (functional), `TTS-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/PlaybackEngine.cpp`](../../src/PlaybackEngine.cpp) (speech), [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp) (voice enumeration), [`src/MainWindow.cpp`](../../src/MainWindow.cpp) (voice test / apply) |
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
conventions. This file is the source of truth for **speaking text via installed
SAPI voices**: voice enumeration/selection, the fallback voice, volume/rate, the
voice test, and the warm-up. Reference format: [`docs/specs/dwell.spec.md`](dwell.spec.md).

This spec was authored **from the existing code**, so per the working agreement
the code is authoritative for current behavior (rule 7): if a fact here drifts
from [`src/PlaybackEngine.cpp`](../../src/PlaybackEngine.cpp), fix the spec and
flag it.

> Scope reminder: the SAPI **speech** side — enumerating/selecting voices,
> speaking text, volume/rate, voice test, fallback voice, warm-up. The
> `PlaybackEngine` is **shared** with sound: the `♫` markers, audio-file
> playback, the worker-thread/queue model, stop/cancel, ducking, and playback
> notifications are owned by [`sound.spec.md`](sound.spec.md). The voice-test
> **button** and the persistence of voice/volume/rate are in
> [`settings.spec.md`](settings.spec.md) / [`persistence.spec.md`](persistence.spec.md);
> the language used to filter voices is in [`localization.spec.md`](localization.spec.md).

---

## 1. Overview

SimonSays speaks typed or selected text aloud through an installed Windows
**SAPI** voice. The Settings dialog lets the user pick a voice (filtered to their
language) and set volume/rate, and preview it with **Test Voice**. Speech is
produced by the shared `PlaybackEngine`: text segments are spoken via `ISpVoice`
on the engine's worker thread, interleaved with any inline sounds
([`sound.spec.md`](sound.spec.md)). When no voice is chosen, the system default
voice is used.

## 2. Background & context

- SAPI voices come from two registries — classic SAPI5
  (`…\Speech\Voices`) and modern OneCore (`…\Speech_OneCore\Voices`) — so both
  are enumerated to expose the full set.
- Voices are stored/selected by their **token id** (an opaque string), not by
  display name, so a voice survives description changes.
- Some voices (e.g. Aholab) have a first-utterance latency, so a tiny warm-up
  utterance is issued after (re)configuring the voice.

## 3. Goals & non-goals

**Goals**
- Enumerate installed SAPI voices (SAPI5 + OneCore), filterable by language.
- Speak text with the selected voice at the chosen volume/rate.
- Fall back to the system default voice when none is selected.
- Preview a voice without freezing the UI.

**Non-goals**
- The `♫` markers, audio-file playback, queue/threading, stop/cancel, ducking,
  and notifications (→ [`sound.spec.md`](sound.spec.md)).
- The voice-test **button**/dialog and value **persistence**
  (→ [`settings.spec.md`](settings.spec.md) / [`persistence.spec.md`](persistence.spec.md)).

## 4. Glossary

| Term | Meaning |
|---|---|
| **SAPI** | Microsoft Speech API — provides the installed `ISpVoice` voices. |
| **Voice token id** | The opaque SAPI token id stored as the `Voice Key` setting. |
| **VoiceInfo** | `{ name, key, language }` for one enumerated voice. |
| **Fallback voice** | The system default voice used when no `Voice Key` is set. |
| **Warm-up** | A tiny async utterance to hide a voice's first-call latency. |

## 5. Personas & scenarios

- **Choose a voice:** open Settings, pick a language → the voice list shows
  matching voices; select one, press Test Voice, OK.
- **Type & speak:** type in the input box, press Play/Enter → the text is spoken.
- **Speak a saved phrase:** click a phrase → its text is spoken (if immediate-
  speak is on).
- **No voice picked:** the app speaks with the Windows default voice.

## 6. Requirements (EARS)

Status tags per [`docs/spec.md`](../spec.md) §2.3. All requirements below are
implemented in the current source and tagged **[Done]** accordingly.

### 6.1 Voice enumeration & selection

- **TTS-F01 [Done]** `PopulateAvaibleVoicesFromRegistry` SHALL enumerate installed
  voices from the SAPI5 registry (`HKLM\…\Speech\Voices`, `SPCAT_VOICES` fallback)
  **and** the OneCore registry (`HKLM\…\Speech_OneCore\Voices`), returning a
  `VoiceInfo{ name, key, language }` per voice (name from the token description,
  key from the token id, language from the token's `LANGID`).
- **TTS-F02 [Done]** WHEN a `languageFilter` is given THE SYSTEM SHALL include only
  voices whose language string contains it (case-insensitive substring); an empty
  filter returns all.
- **TTS-F03 [Done]** THE selected voice SHALL be stored/applied by its **token id**
  (`Voice Key`); voice/volume/rate are held in the engine and updated via
  `SetVoiceSettings`.

### 6.2 Fallback voice

- **TTS-F10 [Done]** IF the stored voice key is empty THEN THE SYSTEM SHALL resolve
  the system default voice via `SpGetDefaultTokenIdFromCategoryId(SPCAT_VOICES)`
  before speaking.

### 6.3 Speaking text

- **TTS-F20 [Done]** THE Speech segments of a queued phrase SHALL be spoken via
  `ISpVoice::Speak` (`SPF_ASYNC | SPF_IS_NOT_XML`), the worker waiting on
  `WaitUntilDone(INTERRUPT_CHECK_INTERVAL_MS)` in a loop so a Stop can interrupt;
  on interrupt the worker SHALL purge with `SPF_PURGEBEFORESPEAK` (same COM
  apartment the voice was created in).
- **TTS-F21 [Done]** Speaking SHALL be triggered by the main window's Play button /
  `Enter`, and by phrase selection when immediate-speak is on (→
  [`categories-phrases.spec.md`](categories-phrases.spec.md) /
  [`settings.spec.md`](settings.spec.md)), via `PlaybackEngine::QueueText`.

### 6.4 Volume & rate

- **TTS-F30 [Done]** THE SYSTEM SHALL apply `SetVolume` (0–100 range;
  `SIMONSAYS_SETTINGS_*_VOICE_VOLUME`) and `SetRate` (−10…+10;
  `…_VOICE_RATE`), clamped, on the worker thread when settings change.

### 6.5 Voice test

- **TTS-F40 [Done]** THE Settings **Test Voice** SHALL preview the selected voice
  at the current volume/rate on a **detached worker thread** (COM initialized
  within it) so the dialog stays responsive, using the language's sample text
  (then the default text, then a built-in sample). *(The button belongs to
  [`settings.spec.md`](settings.spec.md).)*

### 6.6 Warm-up & apply lifecycle

- **TTS-F50 [Done]** IF the selected voice key contains `"Aholab"` THEN, after
  applying voice settings, THE SYSTEM SHALL issue a warm-up async `Speak(" ")` to
  hide its first-utterance latency.
- **TTS-F51 [Done]** Voice/volume/rate SHALL be updated from the main thread
  (`SetVoiceSettings`, guarded) and applied on the worker thread when
  `settingsChanged` is set, so no SAPI call crosses apartments incorrectly.

### 6.7 Non-functional

- **TTS-N01 [Done]** Speech SHALL use SAPI (`ISpVoice`) on the apartment-threaded
  playback worker; the voice object's lifetime is owned by the engine.
- **TTS-N02 [Done]** THE SYSTEM SHALL degrade gracefully when no voice is
  installed/selected (default-voice fallback; no crash if `Speak` fails).

## 7. Architecture & components

### 7.1 Files

| File | Responsibility |
|---|---|
| [`src/PlaybackEngine.cpp`](../../src/PlaybackEngine.cpp) | `ApplyVoiceSettings`, the `Speech` segment (`ISpVoice::Speak` + interrupt), warm-up. |
| [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp) | `PopulateAvaibleVoicesFromRegistry` (SAPI5 + OneCore, language filter). |
| [`src/MainWindow.cpp`](../../src/MainWindow.cpp) | Voice combo population, Test Voice preview, `ApplyVoiceSettings` wiring. |
| [`include/stdafx.h`](../../include/stdafx.h) | `VoiceInfo`; volume/rate clamp bounds (via `RegistryManager.h`). |

### 7.2 Structure

Voice **enumeration** is a `RegistryManager` static that reads the SAPI token
stores. **Selection/volume/rate** live in the shared `PlaybackEngine` and are
applied on its worker thread. **Speaking** is the `Speech`-segment path of
`PlaySegment`. The **test** preview is a standalone `ISpVoice` on a detached
thread (in `SettingsDialogProc`). The mixing/queue/stop model is in
[`sound.spec.md`](sound.spec.md).

## 8. Detailed design

### 8.1 Apply voice settings (`ApplyVoiceSettings`)

Snapshot voice/volume/rate/warm-up under the settings mutex; if the key is empty,
resolve the default token; `SetVoice` (by token id), `SetVolume`, `SetRate`; if
warm-up is needed, async-speak a space; clear `settingsChanged`.

### 8.2 Speaking with interruption

`Speak(..., SPF_ASYNC)` then loop `WaitUntilDone(INTERRUPT_CHECK_INTERVAL_MS)`
until done or `m_stopRequested`/`m_shutdown`; on interruption, purge from the
worker thread (`Speak(nullptr, SPF_PURGEBEFORESPEAK)`), which is required because
the voice lives in the worker's COM apartment.

### 8.3 Enumeration & filter

Enumerate SAPI5 then OneCore token stores; for each, read description + `LANGID`,
map the `LANGID` to a language string, and include the voice when the filter is
empty or a case-insensitive substring of the language.

## 9. Data model & persistence

`Voice Key` (token id), `Voice Volume`, and `Voice Rate` are `Settings` fields
stored under `\Settings` (→ [`persistence.spec.md`](persistence.spec.md) §9.1) and
edited in [`settings.spec.md`](settings.spec.md). This spec owns how they are
**applied to SAPI**, not how they are stored.

## 10. Key interfaces

```cpp
// Voice enumeration (RegistryManager, static)
static std::vector<VoiceInfo> PopulateAvaibleVoicesFromRegistry( std::wstring languageFilter = L"" );

// Apply / speak (PlaybackEngine)
void SetVoiceSettings( const std::wstring & voiceKey, int volume, int rate );
void QueueText( const std::wstring & text, bool stopPrevious = false );  // speech + sound (→ sound)
// internal: ApplyVoiceSettings(); PlaySegment(Speech) → ISpVoice::Speak
```

## 11. UI specification

N/A — no window of its own. Voice choice, volume/rate, and Test Voice are
presented by the Settings dialog ([`settings.spec.md`](settings.spec.md)); typed
text and the Play button belong to the main window. Playback state may be
reflected on the Play button.

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| Voice volume range | 10 – 100 | `SIMONSAYS_SETTINGS_MIN/MAX_VOICE_VOLUME` |
| Voice rate range | −10 … +10 | `SIMONSAYS_SETTINGS_MIN/MAX_VOICE_RATE` |
| Interrupt poll interval | `INTERRUPT_CHECK_INTERVAL_MS` | `PlaybackEngine.cpp` |
| Warm-up voice marker | `"Aholab"` (substring of the token id) | `PlaybackEngine.cpp` |
| Voice registries | `…\Speech\Voices` (WOW6432Node) + `…\Speech_OneCore\Voices` | `RegistryManager.cpp` |
| Default voice category | `SPCAT_VOICES` | `PlaybackEngine.cpp` / `RegistryManager.cpp` |

## 13. Diagnostics

N/A — no diagnostic dump; SAPI failures are handled by benign fallback (default
voice, skipped speak).

## 14. Edge cases & error handling

- **No `Voice Key`** → system default voice (TTS-F10).
- **No voices for a language** → the filtered list may be empty; the user can pick
  from all voices or rely on the default.
- **`Speak` fails** → the segment is skipped (no crash).
- **Stop during speech** → async purge from the worker (TTS-F20).
- **Voice with first-call latency** → warm-up utterance (TTS-F50).
- **COM apartment** → all `ISpVoice` calls stay on the worker thread that created
  the voice (the settings snapshot crosses threads, not the SAPI calls).

## 15. Acceptance criteria (testable)

Reverse-engineered from shipping behavior; **[Pass]** reflects the code path.

- **AC-1 (TTS-F01/F02) [Pass]** The voice list includes SAPI5 and OneCore voices;
  a language filter narrows it by substring.
- **AC-2 (TTS-F03/F10) [Pass]** A selected voice speaks; with no selection the
  system default voice is used.
- **AC-3 (TTS-F20/F21) [Pass]** Play/Enter and immediate-speak selection speak the
  text; a Stop interrupts speech promptly.
- **AC-4 (TTS-F30) [Pass]** Volume/rate changes take effect (clamped) on the next
  utterance.
- **AC-5 (TTS-F40) [Pass]** Test Voice previews without freezing the dialog.
- **AC-6 (TTS-F50/F51) [Pass]** An Aholab voice gets a warm-up; settings applied
  from the main thread take effect on the worker.

Build gate: Debug **and** Release x64 compile clean (no code change in this
authoring pass).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Voice enumeration (SAPI5 + OneCore) | ✅ Done | language-substring filter |
| Voice selection by token id | ✅ Done | `Voice Key` |
| Fallback (default) voice | ✅ Done | `SpGetDefaultTokenIdFromCategoryId` |
| Speak + interrupt | ✅ Done | async + `WaitUntilDone`; worker purge |
| Volume / rate | ✅ Done | clamped; applied on worker |
| Voice test (non-blocking) | ✅ Done | detached preview voice |
| Warm-up (Aholab) | ✅ Done | async `Speak(" ")` |

## 17. Known limitations

- The voice list depends on installed SAPI voices; a language may have none.
- Warm-up is keyed on the `"Aholab"` substring (a targeted workaround, not a
  general latency solution).
- No SSML/markup support (text spoken as `SPF_IS_NOT_XML`).

## 18. Future work

- Per-phrase or per-category voice override.
- Optional SSML for emphasis/pauses.

## 19. Open questions

1. Should the warm-up be generalized (measure first-call latency) rather than a
   voice-name substring?

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of TTS-SPEC v1.0.*
