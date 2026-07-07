🗣️ SimonSays – Simply Speak (Text-to-Speech Utility)
===========================================

SimonSays is an accessibility-focused speech assistant built for people living with ALS and other speech impairments.

Integrated directly into your taskbar, it lets you type what you want to say and have it spoken out loud instantly—no app switching or context loss.

<img width="712" height="585" alt="imagen" src="https://github.com/user-attachments/assets/071c6b63-659a-49a0-86cd-6b3a2199c63e" />

Lightweight and always ready in your taskbar, SimonSays helps you communicate quickly, independently, and on your own terms—whether you’re typing short replies or longer messages.

You can also embed sound markers so short WAV/MIDI/MP3 effects play inline with spoken output.

Eye tracking is supported via the Gaze / Dwell-click feature, allowing you to select phrases or buttons by looking at them for a set duration. It works with any eye-control software that moves the mouse cursor, and can also read gaze directly (no cursor movement needed) on the Irisbond Hiru (HID mode) and on Tobii trackers through their installed software — verified with the Tobii Eye Tracker 4C and Tobii Dynavox PCEye 5; the Tobii Eye Tracker 5 is expected to work but has not been tested yet. This feature is new and needs testing and feedback — please tell us how it works with your tracker via the **Feedback** option.

## Supported languages

SimonSays includes built-in default phrase sets and UI language labels for the following languages:

Arabic, Basque, Catalan, Chinese (Simplified), English, French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Spanish, Valencian

> Note: available voices depend on what is installed in Windows. If a matching voice is not available, you can still use the phrase sets, but playback may fall back to another installed voice.

## What's new in v0.7

Highlights:

- **Gaze / Dwell-click**: hands-free activation by eye tracking — cursor-based dwell with any eye-control software, plus direct gaze on the Irisbond Hiru and Tobii 4C / Tobii Dynavox PCEye 5. New: needs testing and feedback.
- **Bundle sounds and icons**: import and export sound files and icon images along with categories and phrases.
- **Zooming**: zoom in/out on the categories window for better visibility.

See the `ChangeLog` for full details.

## Feedback and support

Your feedback is invaluable to us! If you have suggestions, encounter issues, or want to contribute, please reach out. A feedback form is available directly from the tray menu under **Feedback**, or you can access it online here: [Feedback Form](https://forms.gle/KMbpEDYmwnFJUhgy8).

For bug reports or technical support, please visit our [GitHub repository](https://github.com/juan-rey/SimonSays).

## Quick start

1. Download precompiled binaries from the [releases page](https://github.com/juan-rey/SimonSays/releases) or from Microsoft Store [page](https://aka.ms/AA11hvzs), or build the solution in Visual Studio (recommended) or your preferred Windows C++ toolchain.
2. Launch SimonSays — the app lives in the taskbar. Open the categories window, choose a phrase or type your message, then press Play; adjust voice and language in Settings (F2) if needed.

### Keyboard shortcuts
- `F1`: Help
- `F2`: Settings
- `F3`: Gaze / Dwell click settings
- `F4`: Edit current selection
- `F5` / `F6`: Move to previous / next selection
- `F7`: Add after current selection (category/phrase)
- `F8`: Delete current selection
- `F9`: Import phrases
- `F10`: Export phrases
- `F11` / `Ctrl -`: Zoom out
- `F12` / `Ctrl +`: Zoom in
- `Ctrl 0`: Reset zoom

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

  `Hello ♫notification.wav♫, please wait.`

  - Text outside markers is spoken securely using Windows TTS.
  - Text between markers is treated as an audio filename and played inline.

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
