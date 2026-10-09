# SimonSays — Sound Markers & Audio Playback — Specification

| | |
|---|---|
| **Spec ID** | SND-SPEC |
| **Status** | Active — reverse-engineered from shipping source (2026-07-10); board resource subfolder added 2026-07-12; default resource folder added 2026-07-28; board subfolder nesting noted 2026-07-29; wav → direct waveOut, mp3 → Media Foundation, with MF/MCI/PlaySound runtime fallbacks 2026-10-09 |
| **Version** | 1.4 (2026-10-09) |
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
(streamed to `waveOut`, directly or via Media Foundation, falling back to `PlaySound`/MCI) — and plays them in order on a background worker
thread, so the UI stays responsive. Playback can be stopped instantly, and the
app can optionally raise its own volume and/or duck other apps while speaking.

## 2. Background & context

- A phrase can embed short effects (a chime, applause) between spoken words, so
  the engine interleaves speech and audio in one ordered stream.
- Playback runs off the UI thread because SAPI `Speak` and sound playback are
  synchronous and can be long; a two-queue worker keeps the UI free and lets a
  Stop interrupt mid-phrase.
- Both formats end up streamed to `waveOut`, which knows exactly when playback
  ends and stops reliably. `.wav` streams its own data chunk (the wave mapper
  converts compressed formats through ACM — every bundled wav is **MS-ADPCM**,
  which Media Foundation can't decode); `.mp3` is decoded with **Media
  Foundation**. MCI
  `mpegvideo` (DirectShow) used to be the mp3 path, but on Windows 11 Insider
  10.0.26300 its MPEG audio decoder fail-fasts (`0xC0000602`) on the **second**
  graph opened in a process (reproduced standalone, single-threaded, 2026-10-04),
  so it is now only a fallback.
- Media Foundation is absent on Windows **N** editions without the Media Feature
  Pack, so `mfplat.dll`/`mfreadwrite.dll` are **delay-loaded** and mp3 falls
  back to MCI at runtime (wav doesn't need Media Foundation).

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
| **Default resource folder** | `%LocalAppData%\SimonSays\resources` — the shared folder for boards with no per-board resource subfolder (see [`import-export.spec.md`](import-export.spec.md) PORT-F33). |

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
  search the sound folders in order — **the active board's resource subfolder
  (when one is defined; see [`board-style.spec.md`](board-style.spec.md)
  STY-F58) → the default resource folder `%LocalAppData%\SimonSays\resources`
  → `%LocalAppData%\SimonSays` (app-data root; permanent read fallback for
  sounds placed there by earlier versions) → working directory (if different)
  → executable directory** — each folder skipped if a case-insensitive
  duplicate of one already checked; using the first match; an **absolute**
  path SHALL be used if it exists. *(Amended 2026-07-12: board subfolder
  prepended; it is pushed to the engine via `SetBoardResourceFolder` whenever
  the board style is (re)applied and is guarded for cross-thread reads.
  Amended 2026-07-28: default resource folder inserted per
  [`import-export.spec.md`](import-export.spec.md) PORT-F33 — new resources for
  boards with no per-board subfolder now install there instead of loose in the
  app-data root, which remains searched indefinitely for back-compat. Amended
  2026-07-29: the board resource subfolder itself now nests under the default
  resource folder (`resources\<name>`, not a root sibling — see
  [`board-style.spec.md`](board-style.spec.md) STY-F58); this search order is
  unaffected since the subfolder is still checked first regardless of where it
  sits.)*
- **SND-F11 [Done]** WHEN the file cannot be resolved THE SYSTEM SHALL substitute
  the built-in **fallback sound**.
- **SND-F12 [Done]** THE SYSTEM SHALL classify by extension: `.wav`/`.mid`/`.midi`
  → **SoundWav**, `.mp3` → **SoundMp3**; a reference with any other extension SHALL
  be dropped (no segment).

### 6.3 Playback

- **SND-F20 [Done]** THE worker thread SHALL play the parsed segments **in order**:
  Speech via SAPI (→ [`tts.spec.md`](tts.spec.md)); `SoundWav` by streaming the
  RIFF data chunk in its own format to `waveOut` (`PlayWavWithWaveOut`);
  `SoundMp3` via **Media Foundation** (Source Reader → PCM). Both stream through
  `StreamToWaveOut`, a ring of `WAVEOUT_BUFFER_COUNT` × `WAVEOUT_BUFFER_MS`
  buffers, so memory stays flat. *(Amended 2026-10-09: previously
  `PlaySound`/MCI `waveaudio` for wav and MCI `mpegvideo` for mp3.)*
- **SND-F21 [Done]** WHEN Media Foundation is available (both DLLs loadable from
  System32 and `MFStartup` succeeds; `m_useMediaFoundation`) THE SYSTEM SHALL
  **warm it up** on the worker thread by decoding the start of the fallback mp3,
  so the first sound plays without the ~200 ms DLL-load stall.
- **SND-F22 [Done]** WHEN Media Foundation is unavailable, or `USE_MCI_FOR_MP3` is
  defined at build time, THE SYSTEM SHALL play mp3 via MCI `mpegvideo`
  (pre-opening the fallback mp3 on the main thread as a codec warm-up).
  (No per-file MCI fallback when Media Foundation can't decode one mp3: on
  affected builds a second MCI mp3 open crashes the process.)
- **SND-F23 [Done]** WHEN `waveOut` can't open a wav file's own format (or the
  file isn't a parsable RIFF/WAVE) THE SYSTEM SHALL try Media Foundation (if
  available), and WHEN that fails too SHALL play it via `PlaySound`
  (asynchronous with a `GetWavDuration`-timed wait, or synchronous when the
  duration is unknown).

### 6.4 Stop / interrupt

- **SND-F30 [Done]** `Stop()` and `QueueText(text, stopPrevious=true)` SHALL halt
  ongoing speech **and** sound **immediately** — SAPI purge (issued on the worker
  thread), `waveOutReset` (Media Foundation path), `PlaySound(NULL, NULL,
  SND_PURGE)`, and MCI stop — within `INTERRUPT_CHECK_INTERVAL_MS`, without
  waiting for the current segment to finish.

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
  atomics), with correct COM apartment init on the worker. The worker is an STA,
  so every wait SHALL pump its message queue (`WaitWithMessagePump` /
  `MsgWaitForMultipleObjectsEx`), including the idle wait for new text.

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
| [`src/PlaybackEngine.cpp`](../../src/PlaybackEngine.cpp) | Worker thread, `ParseText`, `PlaySegment`, file resolution, ducking, Media Foundation / MCI / PlaySound back-ends. |
| [`SimonSays.vcxproj`](../../SimonSays.vcxproj) | `DelayLoadDLLs` for `mfplat.dll` / `mfreadwrite.dll` (all configurations). |
| [`include/stdafx.h`](../../include/stdafx.h) | `SOUND_NOTE_DELIMITER`; `WM_PLAYBACK_STARTED/FINISHED`. |
| [`src/MainWindow.cpp`](../../src/MainWindow.cpp) | Owns the engine; `QueueText` on Play/phrase-select; passes voice/ducking settings. |

### 7.2 Structure

- **Two queues:** an incoming text queue (main → worker, CV-signalled) and a
  parsed segment queue; the worker parses, then plays segment-by-segment.
- **Playback back-ends:** SAPI `ISpVoice` (speech); `waveOut` fed directly from
  the RIFF data (wav) or by a Media Foundation Source Reader (mp3); fallbacks:
  Media Foundation then `PlaySound` (wav, and `.mid`/`.midi` — see §17), MCI
  `mpegvideo` (mp3).
- **Ducking:** Core Audio (`IAudioSessionManager2` / `IAudioEndpointVolume`) with
  saved state for restore.

### 7.3 Layering

`PlaybackEngine` depends on SAPI, Media Foundation (`mfplat`/`mfreadwrite`,
delay-loaded), WinMM (`waveOut`/`PlaySound`/MCI), and Core Audio; it is
owned by `MainWindow`. Speech specifics are shared with
[`tts.spec.md`](tts.spec.md); this spec owns the sound/mixing/engine side.

## 8. Detailed design

### 8.1 Parse → play

`ParseText` walks the string, alternating Speech and sound segments on `♫`.
For a sound reference: resolve the path (§6.2), fall back if missing, classify by
extension. The worker dequeues and calls `PlaySegment`, checking
`m_stopRequested` between and during segments.

### 8.2 Back-end selection

The constructor sets `m_useMediaFoundation` from `IsMediaFoundationAvailable()`
(both DLLs loadable from System32; forced false by `USE_MCI_FOR_MP3`); the
worker clears it if `MFStartup` fails. Because the MF DLLs are delay-loaded, no
MF API may be called unless this flag is set.

| | Media Foundation available | Media Foundation unavailable |
|---|---|---|
| `.wav` | `PlayWavWithWaveOut` → `PlaySoundWithMediaFoundation` → `PlayWavWithPlaySound` | `PlayWavWithWaveOut` → `PlayWavWithPlaySound` (duration measured with `GetWavDuration` at play time) |
| `.mp3` | `PlaySoundWithMediaFoundation` | `PlayMp3WithMci` (fallback mp3 pre-opened on the main thread as a warm-up) |
| Fallback sound | `fallback.wav` (waveOut, stoppable) | `fallback.wav` (waveOut, stoppable) |

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
| Sound folders | board resource subfolder (dynamic), default resource folder (`%LocalAppData%\SimonSays\resources`), `%LocalAppData%\SimonSays` (legacy fallback), working dir, exe dir | `PlaybackEngine.cpp` ctor + `SetBoardResourceFolder`; `GetDefaultResourceFolder` (`utils.cpp`) |
| Default resource folder name | `resources` | `stdafx.h` `DEFAULT_RESOURCE_FOLDER_NAME` |
| Fallback sound | `FALLBACK_WAV_FILE` (`FALLBACK_MP3_FILE` is only the MF warm-up / MCI pre-open file) | `PlaybackEngine.cpp` |
| waveOut streaming buffers | 3 × 250 ms | `PlaybackEngine.cpp` `WAVEOUT_BUFFER_COUNT` / `WAVEOUT_BUFFER_MS` |
| Stop / interrupt check interval | 100 ms | `PlaybackEngine.cpp` `INTERRUPT_CHECK_INTERVAL_MS` |
| Force MCI for mp3 (build-time) | `USE_MCI_FOR_MP3` (commented out) | `PlaybackEngine.h` |
| Duck factor (default / aggressive) | 0.25 / 0.16 | `PlaybackEngine.h` `*_AUDIO_DUCK_FACTOR` |
| Playback messages | `WM_PLAYBACK_STARTED` / `_FINISHED` | `stdafx.h` |

## 13. Diagnostics

N/A — no diagnostic dump. Failures degrade gracefully (fallback sound, skipped
segment, benign MCI/PlaySound errors).

## 14. Edge cases & error handling

- **Unmatched `♫`** → remainder spoken (SND-F02).
- **Missing / unresolved file** → fallback sound (SND-F11).
- **Unsupported extension** → the reference is dropped.
- **Stop mid-segment** → SAPI purge / `waveOutReset` / `SND_PURGE` / MCI stop;
  the worker checks `m_stopRequested` and abandons the queue.
- **Media Foundation / MCI codec cold** → mitigated by the startup warm-ups
  (SND-F21 / SND-F22).
- **Media Foundation missing (Windows N)** → app still starts (delay-load); wav
  is unaffected, mp3 uses MCI (SND-F22).
- **wav format waveOut can't open / unparsable wav** → Media Foundation, then
  `PlaySound` (SND-F23); an mp3 Media Foundation can't decode is skipped
  silently.
- **Abnormal termination while ducking** → other apps may stay muted / system
  volume raised until adjusted (documented in the ChangeLog for v0.5).

## 15. Acceptance criteria (testable)

Reverse-engineered from shipping behavior; **[Pass]** reflects the code path.

- **AC-1 (SND-F01/F02) [Pass]** `A ♫x.wav♫ B` plays speech-sound-speech in order;
  an unmatched `♫` speaks the rest.
- **AC-2 (SND-F10/F11/F12) [Pass]** A relative file resolves via the folder order;
  a missing file plays the fallback; `.wav/.mid/.midi` and `.mp3` play, other
  extensions are ignored. A file placed only in the default resource folder
  resolves, and a file placed only in the legacy app-data root (pre-upgrade
  install) still resolves too. *(Verified 2026-07-28 via a standalone harness
  against the real `utils.cpp`/`PlaybackEngine.cpp` folder-building logic.)*
- **AC-3 (SND-F20/F21) [Pass]** Mixed segments play in order; the first mp3 plays
  without a stall (warm-up). Repeated mp3 plays in one session don't crash
  (the MCI regression that motivated the Media Foundation path; user-verified
  in-app 2026-10-09).
- **AC-4 (SND-F30) [Pass]** Stop / stop-previous halts speech and sound at once.
- **AC-7 (SND-F22) [Pass]** With MF unavailable (forced via `USE_MCI_FOR_MP3`)
  the app starts, pre-opens MCI (DirectShow loaded) and our code never loads
  `mfreadwrite`; `dumpbin /imports` lists `mfplat.dll`/`mfreadwrite.dll` only as
  delay-load imports. *(Verified 2026-10-09; no real Windows N machine tested.)*
- **AC-8 (SND-F20/F23) [Pass]** All 15 bundled wavs (MS-ADPCM, mono/stereo,
  22–48 kHz) play through `PlayWavWithWaveOut` for their full length; a wav is
  stopped ~130 ms after Stop; a non-RIFF `.wav` fails both `waveOut` and Media
  Foundation, so `PlaySegment` uses `PlaySound`. *(Verified 2026-10-09 in a
  standalone harness built from the extracted engine code; wav playback
  user-verified in-app the same day.)*
- **AC-5 (SND-F40) [Pass]** With ducking on, other apps quieten/mute and own/system
  volume rises while playing, then all restore on finish.
- **AC-6 (SND-F50/F60) [Pass]** `WM_PLAYBACK_STARTED/FINISHED` bracket a run; the UI
  stays responsive during long playback.

Build gate: Debug **and** Release Win32 compile clean (no code change in this
authoring pass).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| `♫` marker parsing | ✅ Done | alternating speech/sound; unmatched → speech |
| File resolution + fallback | ✅ Done | board subfolder / default resource folder / AppData root (legacy) / working / exe; built-in fallback |
| wav playback | ✅ Done | direct `waveOut` (ACM via wave mapper); Media Foundation then `PlaySound` fallbacks |
| mp3 playback + warm-up | ✅ Done | Media Foundation + `waveOut`, warmed up on the worker; MCI `mpegvideo` fallback with startup pre-open |
| mid/midi playback | ⚠️ Not working | routed to `PlaySound`, which only plays waveform audio (see §17) |
| Stop / interrupt | ✅ Done | SAPI purge / `waveOutReset` / `SND_PURGE` / MCI stop |
| Audio ducking | ✅ Done | Core Audio; save/restore |
| Worker threading + notifications | ✅ Done | two-queue; `WM_PLAYBACK_*` |

## 17. Known limitations

- **`PlaySound` cannot be stopped reliably**; it is only the last-resort wav
  path.
- **`.mid`/`.midi` are silent** *(flagged 2026-10-09 — earlier versions of this
  spec claimed they play)*: they are classified as `SoundWav`, but `waveOut`,
  Media Foundation and `PlaySound` (like the old MCI `waveaudio` path) only
  handle waveform audio. Playing MIDI would need MCI `sequencer`.
- **MCI `mpegvideo` crashes on the second open** on some Windows builds
  (Insider 10.0.26300); it is only used when Media Foundation is unavailable.
- Ducking **mutes** other apps (rather than lowering) and raises system volume in
  some paths; abnormal termination can leave them changed (see v0.5 ChangeLog).
- `.mid`/`.midi` play but are **not** bundled into `.ssz` (see
  [`import-export.spec.md`](import-export.spec.md) §17).
- Some uncommon mp3 encodings may not decode on all Windows installs.

## 18. Future work

- Make `.mid`/`.midi` actually play (MCI `sequencer`), then unify the playable
  and bundleable audio sets (add them to `.ssz`).
- Volume *reduction* (rather than full mute) for other apps as the default duck.

## 19. Open questions

1. Should the default duck reduce rather than mute other apps everywhere?
2. Should unresolved-file fallback be configurable (silent vs fallback sound)?

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of SND-SPEC v1.4.*
