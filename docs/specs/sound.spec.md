# SimonSays — Sound Markers & Audio Playback — Specification

| | |
|---|---|
| **Spec ID** | SND-SPEC |
| **Status** | Active — reverse-engineered from shipping source (2026-07-10) |
| **Version** | 1.0 (2026-07-10) |
| **REQ prefix** | `SND-F##` (functional), `SND-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`src/PlaybackEngine.cpp`](../../src/PlaybackEngine.cpp), [`include/PlaybackEngine.h`](../../include/PlaybackEngine.h); markers in [`include/stdafx.h`](../../include/stdafx.h) |
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
conventions. This file is the source of truth for **inline sound markers**,
**audio-file playback** (`.wav`/`.mid`/`.midi`/`.mp3`), the **sound-file lookup
order** and **fallback sound**, **audio ducking**, and the **threaded playback
engine** that plays mixed speech + sound. Reference format:
[`docs/specs/dwell.spec.md`](dwell.spec.md).

This spec was authored **from the existing code**, so per the working agreement
the code is authoritative for current behavior (rule 7): if a fact here drifts
from [`src/PlaybackEngine.cpp`](../../src/PlaybackEngine.cpp), fix the spec and
flag it.

> Scope reminder: the `♫` marker grammar, audio-file resolution/playback,
> stop/cancel, ducking, and the worker-thread playback model. The `PlaybackEngine`
> is **shared** with TTS: SAPI **speech** (voice selection, volume/rate, voice
> test, warm-up) is owned by [`tts.spec.md`](tts.spec.md). The `::` **authoring**
> of a phrase's audio file is in [`categories-phrases.spec.md`](categories-phrases.spec.md);
> the Settings **toggles** for ducking/stop-previous are in
> [`settings.spec.md`](settings.spec.md); `.ssz` **bundling** of assets is in
> [`import-export.spec.md`](import-export.spec.md).

---

## 1. Overview

`PlaybackEngine` plays a phrase that mixes **spoken text** and **inline sound
files**. Text is submitted as a single string; the engine parses it into ordered
**segments** — plain text (spoken via SAPI) and `♫`-delimited sound references
(played via `PlaySound`/MCI) — and plays them in order on a background worker
thread, so the UI stays responsive. Playback can be stopped instantly, and the
app can optionally raise its own volume and/or duck other apps while speaking.

## 2. Background & context

- A phrase can embed short effects (a chime, applause) between spoken words, so
  the engine interleaves speech and audio in one ordered stream.
- Playback runs off the UI thread because SAPI `Speak` and MCI play are
  synchronous and can be long; a two-queue worker keeps the UI free and lets a
  Stop interrupt mid-phrase.
- `.wav`/`.mid`/`.midi` play through `PlaySound` (low latency) or MCI; `.mp3`
  needs MCI (`mpegvideo`), whose codec is **pre-warmed** at startup so the first
  mp3 on the worker thread plays without a stall.

## 3. Goals & non-goals

**Goals**
- Interleaved speech + inline audio from one text string.
- Robust file resolution with a guaranteed fallback sound.
- Instant, reliable stop/cancel of speech and audio.
- Optional audio ducking that restores cleanly.
- Non-blocking, thread-safe playback.

**Non-goals**
- SAPI **voice** selection / volume / rate / test / warm-up (→ [`tts.spec.md`](tts.spec.md)).
- Authoring the `::` audio suffix (→ [`categories-phrases.spec.md`](categories-phrases.spec.md)).
- The ducking/stop-previous **toggles' UI** (→ [`settings.spec.md`](settings.spec.md)).
- Bundling assets into `.ssz` (→ [`import-export.spec.md`](import-export.spec.md)).

## 4. Glossary

| Term | Meaning |
|---|---|
| **Sound marker** | A `♫`-delimited audio reference inside spoken text (`SOUND_NOTE_DELIMITER`). |
| **Segment** | A `PlaybackSegment` — `Speech`, `SoundWav`, or `SoundMp3` — the unit of playback. |
| **Fallback sound** | The built-in sound used when a referenced file cannot be resolved. |
| **Ducking** | Temporarily reducing/muting other apps and/or raising own/system volume while speaking. |
| **Sound folders** | The ordered directories searched for a relative sound file. |

## 5. Personas & scenarios

- **Phrase with an effect:** `Well done ♫applause.wav♫!` → speaks "Well done",
  plays `applause.wav`, speaks "!".
- **Missing file:** a referenced file that isn't found plays the built-in
  fallback sound instead of failing.
- **Interrupt:** pressing Stop (or triggering a new phrase with "stop previous")
  halts the current speech/sound at once.
- **Focus mode:** with ducking on, other apps quieten (or mute) and the app's
  volume rises while speaking, then everything is restored.

## 6. Requirements (EARS)

Status tags per [`docs/spec.md`](../spec.md) §2.3. All requirements below are
implemented in the current source and tagged **[Done]** accordingly.

### 6.1 Marker parsing

- **SND-F01 [Done]** `ParseText` SHALL split the input on `♫` (`SOUND_NOTE_DELIMITER`)
  into ordered segments — text outside delimiters is a **Speech** segment, text
  between a matched pair is a **sound** reference — trimming each and skipping
  empties.
- **SND-F02 [Done]** WHEN a `♫` has no closing delimiter THE SYSTEM SHALL treat
  the remainder as a Speech segment (no partial sound).

### 6.2 Audio-file resolution

- **SND-F10 [Done]** FOR a **relative** sound filename (no `:`) THE SYSTEM SHALL
  search the sound folders in order — **`%LocalAppData%\SimonSays` → working
  directory (if different) → executable directory** — using the first match; an
  **absolute** path SHALL be used if it exists.
- **SND-F11 [Done]** WHEN the file cannot be resolved THE SYSTEM SHALL substitute
  the built-in **fallback sound**.
- **SND-F12 [Done]** THE SYSTEM SHALL classify by extension: `.wav`/`.mid`/`.midi`
  → **SoundWav**, `.mp3` → **SoundMp3**; a reference with any other extension SHALL
  be dropped (no segment).

### 6.3 Playback

- **SND-F20 [Done]** THE worker thread SHALL play the parsed segments **in order**:
  Speech via SAPI (→ [`tts.spec.md`](tts.spec.md)); `SoundWav` via `PlaySound`
  (`SND_FILENAME`; synchronous, or asynchronous with a measured-duration wait when
  interruptibility is required) or MCI; `SoundMp3` via MCI (`mpegvideo`).
- **SND-F21 [Done]** THE SYSTEM SHALL **pre-warm** the MCI mp3 codec at startup
  (opening a fallback mp3) so the first worker-thread mp3 plays without a stall,
  and SHALL choose `PlaySound`-vs-MCI for `.wav` based on MCI availability
  (`m_usePlaySoundForWav`).

### 6.4 Stop / interrupt

- **SND-F30 [Done]** `Stop()` and `QueueText(text, stopPrevious=true)` SHALL halt
  ongoing speech **and** sound **immediately** — SAPI purge (issued on the worker
  thread), `PlaySound(NULL, NULL, SND_PURGE)`, and MCI stop — without waiting for
  the current segment to finish.

### 6.5 Audio ducking

- **SND-F40 [Done]** WHILE playing, IF "increase volume when playing" is on THE
  SYSTEM SHALL raise the app's (and/or system) volume, and IF "reduce other audio
  when playing" is on it SHALL reduce/mute other applications' audio (Windows Core
  Audio); on finish THE SYSTEM SHALL restore all saved volumes/mute states.

### 6.6 Notifications & threading

- **SND-F50 [Done]** THE SYSTEM SHALL post `WM_PLAYBACK_STARTED` and
  `WM_PLAYBACK_FINISHED` to the owner window around a playback run.
- **SND-F60 [Done]** Playback SHALL run on a background worker with a two-queue
  model (incoming raw text → parsed segment queue); voice/ducking settings SHALL
  be updated from the main thread and applied on the worker (guarded by mutexes/
  atomics), with correct COM apartment init on the worker.

### 6.7 Non-functional

- **SND-N01 [Done]** Playback SHALL NOT block the UI thread.
- **SND-N02 [Done]** A resolvable **fallback sound** SHALL always be available so a
  missing file never silences a triggered effect.
- **SND-N03 [Done]** Cross-thread state SHALL be synchronized (mutex-guarded
  queues/settings, atomic flags), and SAPI purge SHALL be issued only from the
  worker thread to preserve COM apartment correctness.

## 7. Architecture & components

### 7.1 Files

| File | Responsibility |
|---|---|
| [`include/PlaybackEngine.h`](../../include/PlaybackEngine.h) | `PlaybackEngine`, `PlaybackSegment`, `SegmentType`, duck-factor constants. |
| [`src/PlaybackEngine.cpp`](../../src/PlaybackEngine.cpp) | Worker thread, `ParseText`, `PlaySegment`, file resolution, ducking, MCI/PlaySound. |
| [`include/stdafx.h`](../../include/stdafx.h) | `SOUND_NOTE_DELIMITER`; `WM_PLAYBACK_STARTED/FINISHED`. |
| [`src/MainWindow.cpp`](../../src/MainWindow.cpp) | Owns the engine; `QueueText` on Play/phrase-select; passes voice/ducking settings. |

### 7.2 Structure

- **Two queues:** an incoming text queue (main → worker, CV-signalled) and a
  parsed segment queue; the worker parses, then plays segment-by-segment.
- **Playback back-ends:** SAPI `ISpVoice` (speech), `PlaySound` (wav/mid), MCI
  (`mpegvideo` for mp3, `waveaudio` fallback for wav) via a message/hidden window.
- **Ducking:** Core Audio (`IAudioSessionManager2` / `IAudioEndpointVolume`) with
  saved state for restore.

### 7.3 Layering

`PlaybackEngine` depends on SAPI, WinMM (`PlaySound`/MCI), and Core Audio; it is
owned by `MainWindow`. Speech specifics are shared with
[`tts.spec.md`](tts.spec.md); this spec owns the sound/mixing/engine side.

## 8. Detailed design

### 8.1 Parse → play

`ParseText` walks the string, alternating Speech and sound segments on `♫`.
For a sound reference: resolve the path (§6.2), fall back if missing, classify by
extension, and (for wav via `PlaySound`) measure duration with `GetWavDuration`
for timed interruption. The worker dequeues and calls `PlaySegment`, checking
`m_stopRequested` between and during segments.

### 8.2 wav vs mp3 back-ends

`.wav` prefers `PlaySound` (low latency); when the MCI `waveaudio` path is
available (`m_usePlaySoundForWav == false`) wav goes through MCI so it can be
stopped reliably. `.mp3` always uses MCI `mpegvideo`; the codec is pre-opened at
construction as a warm-up. The fallback sound is `PlaySound`-playable, or an mp3
fallback when the MCI-wav path is chosen (since `PlaySound` cannot be stopped
reliably).

### 8.3 Ducking lifecycle

On start (if enabled): save current levels, raise own/system volume, reduce/mute
other sessions. On finish/stop: restore saved app/system volumes and unmute.

## 9. Data model & persistence

No persistent state of its own. The **toggles** that drive ducking / stop-previous
are `Settings` fields (stored per [`persistence.spec.md`](persistence.spec.md),
edited per [`settings.spec.md`](settings.spec.md)); voice/volume/rate are shared
with [`tts.spec.md`](tts.spec.md).

## 10. Key interfaces

```cpp
PlaybackEngine( HWND owner, const std::wstring & voiceKey, int volume, int rate );
void QueueText( const std::wstring & text, bool stopPrevious = false );
void Stop();
bool IsPlaying() const;
void SetVoiceSettings( const std::wstring & voiceKey, int volume, int rate ); // → tts
void SetAudioDuckingSettings( bool increaseVolume, bool reduceOtherAudio );
// Owner receives: WM_PLAYBACK_STARTED / WM_PLAYBACK_FINISHED
```

## 11. UI specification

N/A — no UI of its own. Triggered by the main window's Play button / phrase
selection; the ducking and stop-previous toggles live in
[`settings.spec.md`](settings.spec.md); playback state may reflect on the Play
button (owned by the main-window/TTS surface).

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| Sound marker | `♫` | `stdafx.h` `SOUND_NOTE_DELIMITER` |
| Supported audio | `.wav`, `.mid`, `.midi`, `.mp3` | `PlaybackEngine.cpp` `ParseText` |
| Sound folders | `%LocalAppData%\SimonSays`, working dir, exe dir | `PlaybackEngine.cpp` ctor |
| Fallback sound | `FALLBACK_WAV_FILE` (mp3 fallback when MCI-wav path) | `PlaybackEngine.cpp` |
| Duck factor (default / aggressive) | 0.25 / 0.16 | `PlaybackEngine.h` `*_AUDIO_DUCK_FACTOR` |
| Playback messages | `WM_PLAYBACK_STARTED` / `_FINISHED` | `stdafx.h` |

## 13. Diagnostics

N/A — no diagnostic dump. Failures degrade gracefully (fallback sound, skipped
segment, benign MCI/PlaySound errors).

## 14. Edge cases & error handling

- **Unmatched `♫`** → remainder spoken (SND-F02).
- **Missing / unresolved file** → fallback sound (SND-F11).
- **Unsupported extension** → the reference is dropped.
- **Stop mid-segment** → SAPI purge / `SND_PURGE` / MCI stop; the worker checks
  `m_stopRequested` and abandons the queue.
- **MCI mp3 codec cold** → mitigated by the startup warm-up (SND-F21).
- **Abnormal termination while ducking** → other apps may stay muted / system
  volume raised until adjusted (documented in the ChangeLog for v0.5).

## 15. Acceptance criteria (testable)

Reverse-engineered from shipping behavior; **[Pass]** reflects the code path.

- **AC-1 (SND-F01/F02) [Pass]** `A ♫x.wav♫ B` plays speech-sound-speech in order;
  an unmatched `♫` speaks the rest.
- **AC-2 (SND-F10/F11/F12) [Pass]** A relative file resolves via the folder order;
  a missing file plays the fallback; `.wav/.mid/.midi` and `.mp3` play, other
  extensions are ignored.
- **AC-3 (SND-F20/F21) [Pass]** Mixed segments play in order; the first mp3 plays
  without a stall (warm-up).
- **AC-4 (SND-F30) [Pass]** Stop / stop-previous halts speech and sound at once.
- **AC-5 (SND-F40) [Pass]** With ducking on, other apps quieten/mute and own/system
  volume rises while playing, then all restore on finish.
- **AC-6 (SND-F50/F60) [Pass]** `WM_PLAYBACK_STARTED/FINISHED` bracket a run; the UI
  stays responsive during long playback.

Build gate: Debug **and** Release x64 compile clean (no code change in this
authoring pass).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| `♫` marker parsing | ✅ Done | alternating speech/sound; unmatched → speech |
| File resolution + fallback | ✅ Done | AppData/working/exe; built-in fallback |
| wav/mid playback | ✅ Done | `PlaySound` or MCI `waveaudio` |
| mp3 playback + warm-up | ✅ Done | MCI `mpegvideo`; startup pre-open |
| Stop / interrupt | ✅ Done | SAPI purge / `SND_PURGE` / MCI stop |
| Audio ducking | ✅ Done | Core Audio; save/restore |
| Worker threading + notifications | ✅ Done | two-queue; `WM_PLAYBACK_*` |

## 17. Known limitations

- **`PlaySound` cannot be stopped reliably**, so an mp3 fallback is used when the
  MCI-wav path is unavailable.
- Ducking **mutes** other apps (rather than lowering) and raises system volume in
  some paths; abnormal termination can leave them changed (see v0.5 ChangeLog).
- `.mid`/`.midi` play but are **not** bundled into `.ssz` (see
  [`import-export.spec.md`](import-export.spec.md) §17).
- Some uncommon mp3 codecs may not play via MCI on all Windows installs.

## 18. Future work

- Unify the playable and bundleable audio sets (add `.mid`/`.midi` to `.ssz`).
- Volume *reduction* (rather than full mute) for other apps as the default duck.

## 19. Open questions

1. Should the default duck reduce rather than mute other apps everywhere?
2. Should unresolved-file fallback be configurable (silent vs fallback sound)?

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of SND-SPEC v1.0.*
