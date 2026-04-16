🗣️ SimonSays – Simply Speak (Text-to-Speech Utility)
===========================================

SimonSays is an accessibility-focused speech assistant built for people living with ALS and other speech impairments.

Integrated directly into your taskbar, it lets you type what you want to say and have it spoken out loud instantly—no app switching or context loss.

Lightweight and always ready in your taskbar, SimonSays helps you communicate quickly, independently, and on your own terms—whether you’re typing short replies or longer messages.

embed sound markers so short WAV/MIDI/MP3 effects play inline with spoken output.

Eye-tracking is planned for future releases.

## Supported languages

SimonSays includes built-in default phrase sets and UI language labels for the following languages:

- Arabic
- Basque
- Catalan
- Chinese (Simplified)
- English
- French
- Galician
- German
- Hebrew
- Hindi
- Italian
- Japanese
- Korean
- Portuguese
- Russian
- Spanish
- Valencian

> Note: available voices depend on what is installed in Windows. If a matching voice is not available, you can still use the phrase sets, but playback may fall back to another installed voice.

## What's new in v0.5

Highlights:

- **Threaded PlaybackEngine**: speech and sound playback now runs on a background worker thread — the UI stays responsive during long phrases or sequences with many embedded sounds.
- **MP3 support**: `.mp3` files can now be embedded inline alongside `.wav`, `.mid`, and `.midi` using the `♫` delimiter.
- **Immediate stop/cancel**: a global Stop action halts ongoing speech and inline sound playback instantly, without waiting for the current segment to finish.
- **Audio ducking**: when enabled, SimonSays mutes other applications and raises the system volume while speaking, restoring both when playback finishes.
- **Stop previous playback**: optional setting to automatically stop any ongoing playback when a new phrase is triggered.
- **Touch keyboard**: optional on-screen keyboard that appears when the input box receives focus, positioned near the main window for easy use with touch or eye-tracking setups.

See the `ChangeLog` for full details.

## Quick start

1. Download precompiled binaries, or build the solution in Visual Studio (recommended) or your preferred Windows C++ toolchain.
2. Launch SimonSays — the app lives in the taskbar. Open the categories window, choose a phrase or type your message, then press Play; adjust voice and language in Settings (F2) if needed.

### Keyboard shortcuts
- `F1`: Help
- `F2`: Settings
- `F3`: Add after current selection (category/phrase)
- `F4`: Edit current selection
- `F5` / `F6`: Move to previous / next selection
- `F8`: Delete current selection
- `F9`: Import phrases
- `F10`: Export phrases

## Help files

- English: `HELP.md`
- Arabic: `docs/help/HELP_ar.md`
- Basque: `docs/help/HELP_eu.md`
- Catalan: `docs/help/HELP_ca.md`
- Chinese (Simplified): `docs/help/HELP_zh_CN.md`
- French: `docs/help/HELP_fr.md`
- Galician: `docs/help/HELP_gl.md`
- German: `docs/help/HELP_de.md`
- Hebrew: `docs/help/HELP_he.md`
- Hindi: `docs/help/HELP_hi.md`
- Italian: `docs/help/HELP_it.md`
- Japanese: `docs/help/HELP_ja.md`
- Korean: `docs/help/HELP_ko.md`
- Portuguese: `docs/help/HELP_pt.md`
- Russian: `docs/help/HELP_ru.md`
- Spanish: `docs/help/HELP_es.md`
- Valencian: `docs/help/HELP_val.md`

## Usage — mixing speech and sounds

- Use the `SOUND_NOTE_DELIMITER` character (♫) to insert inline sound markers. Example:

  Hello ♫notification.wav♫, please wait.

  - Text outside markers is spoken with SAPI.
  - Text between markers is treated as an audio filename and played with `PlaySound`.

- Supported audio formats: `.wav`, `.mid`, `.midi`, `.mp3`.

## Registry defaults and installation

- Default phrase sets are created on first run and stored under `HKCU\\SOFTWARE\\SimonSays\\Phrases\\<Language>`.

### Updating default phrases on an existing install

If you already have phrases stored for a language, SimonSays will not overwrite them automatically. To re-install the built-in defaults for a language (for example, Korean):

1. Close SimonSays.
2. Delete the language key under `HKCU\\SOFTWARE\\SimonSays\\Phrases\\<Language>` (e.g. `...\\Phrases\\Korean`).
3. Restart SimonSays (or run `InstallDefaultPhrases()`), and the defaults will be recreated.

## Troubleshooting

- Voice initialization fails: verify SAPI is installed, voices are available, and the selected voice matches the UI language if desired.
- Sound files not playing: confirm path and supported extension; check file permissions and ensure the filename is between paired delimiters.

## Contributing

See `CONTRIBUTING.md` for coding standards and commit guidelines. When filing issues include OS version, locale, voice selection, and any sound filenames used.

## License

Licensed under the Open Software License (OSL) v3.0. See `LICENSE` for full terms.
