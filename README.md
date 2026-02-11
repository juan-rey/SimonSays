🗣️ SimonSays – Simply Speak (Text-to-Speech Utility)
===========================================

SimonSays is an accessibility-focused speech assistant built for people living with ALS and other speech impairments.

Integrated directly into your taskbar, it lets you type what you want to say and have it spoken out loud instantly—no app switching or context loss.

Lightweight and always ready in your taskbar, SimonSays helps you communicate quickly, independently, and on your own terms—whether you’re typing short replies or longer messages.

Beyond basic text-to-speech (TTS), SimonSays includes quick-access phrase libraries and a “speech + sounds” workflow: embed sound markers so short WAV/MIDI effects play inline with spoken output.

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

## What's new in v0.3

This release expands language packs, refines the UI, and adds configuration options via F2 (voice selection, language labels, and window behavior). See the ChangeLog for full details.

## Quick start

1. Download precompiled binaries, or build the solution in Visual Studio (recommended) or your preferred Windows C++ toolchain.
2. Launch SimonSays — the app lives in the taskbar. Open the categories window, choose a phrase or type your message, then press Play; adjust voice and language in Settings (F2) if needed.

## Usage — mixing speech and sounds

- Use the `SOUND_NOTE_DELIMITER` character (♫) to insert inline sound markers. Example:

  Hello ♫notification.wav♫, please wait.

  - Text outside markers is spoken with SAPI.
  - Text between markers is treated as an audio filename and played with `PlaySound`.

- Supported audio formats: `.wav`, `.mid`, `.midi`. Provide absolute or relative paths accessible to the running process; pair delimiters to avoid treating trailing text as audio.

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
