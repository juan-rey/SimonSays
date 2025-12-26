SimonSays – Simple Speak (Text-to-Speech Utility)

SimonSays is a lightweight Windows text-to-speech (TTS) utility focused on fast, repeatable communication. It runs from the taskbar and lets you either type free-form text or pick from saved phrase categories, then speaks it using the installed Windows SAPI voices.

In addition to basic TTS, SimonSays supports quick-access phrase libraries and a simple “speech + sounds” workflow: you can embed sound markers in your text so short WAV/MIDI effects play inline alongside spoken output.

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

This release adds expanded language packs and configuration options with F2. See the ChangeLog for full details.

## Quick start

1. Download precompiled binaries or build the solution in Visual Studio (recommended) or your preferred Windows C++ toolchain.
2. Launch SimonSays — the app runs from the taskbar. Open the categories window, choose a phrase or type your message, then press Play.

## Usage — mixing speech and sounds

- Use the `SOUND_NOTE_DELIMITER` character (♫) to insert inline sound markers. Example:

  Hello ♫notification.wav♫, please wait.

  - Text outside markers is spoken with SAPI.
  - Text between markers is treated as an audio filename and played with `PlaySound`.

- Supported audio formats: `.wav`, `.mid`, `.midi`. Provide absolute or relative paths accessible to the running process.

## Registry defaults and installation

- Default phrase sets are created and stored under `HKCU\\SOFTWARE\\SimonSays\\Phrases\\<Language>`.

### Updating default phrases on an existing install

If you already have phrases stored for a language, SimonSays will not overwrite them automatically. To re-install the built-in defaults for a language (for example, Korean):

1. Close SimonSays.
2. Delete the language key under `HKCU\\SOFTWARE\\SimonSays\\Phrases\\<Language>` (e.g. `...\\Phrases\\Korean`).
3. Restart SimonSays (or run `InstallDefaultPhrases()`), and the defaults will be recreated.

## Troubleshooting

- Voice initialization fails: verify SAPI is installed and voices are available.
- Sound files not playing: confirm path and supported extension; check file permissions.

## Contributing

See `CONTRIBUTING.md` for coding standards and commit guidelines. When filing issues include OS version, locale, and any voice settings.

## License

Licensed under the Open Software License (OSL) v3.0. See `LICENSE` for full terms.
