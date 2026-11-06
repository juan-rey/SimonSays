🗣️ SimonSays – Simply Speak (Text-to-Speech Utility)
===========================================

SimonSays is a Windows text-to-speech assistant. It began as a tool for people with ALS and others who can no longer speak, and turned out to work just as well as a general AAC (augmentative and alternative communication) tool for anyone who finds speaking difficult or impossible — for example because of dysarthria, a laryngectomy, apraxia of speech, a stroke or a brain injury.

It lives in the empty space on the Windows taskbar, so it never covers your other windows: you can stay in your browser, documents or video call and still speak whenever you need to, by typing or with a couple of clicks on a dozen categories of ready-made, customizable phrases — with the keyboard, the mouse, or hands-free with an eye tracker.

<img width="712" height="585" alt="imagen" src="https://github.com/user-attachments/assets/071c6b63-659a-49a0-86cd-6b3a2199c63e" />

Typical uses:
- Communicating while working intensively in other Windows applications, without losing focus.
- Boards prepared by speech therapists for people with speech disabilities.
- Online meetings: through a virtual audio cable, people in Teams, Zoom or Google Meet hear your phrases as if they came from your microphone.

Short WAV/MP3 sounds can be mixed into what is spoken — applause, a doorbell, an "ahem" to ask for a turn.

Eye tracking is supported via Gaze / Dwell-click: rest your gaze on a button to activate it. It works with any eye-control software that moves the mouse cursor, and reads gaze directly from the Irisbond Hiru (HID mode) and Tobii trackers — verified with the Tobii Eye Tracker 4C, Tobii Eye Tracker 5 and Tobii Dynavox PCEye 5.

Everyday changes to categories and phrases are made in the app itself. For bigger board work — AAC pictogram boards (with ARASAAC pictogram search), custom icons and styled, color-coded boards, especially Fitzgerald Key boards — [SimonSays Web](https://github.com/juan-rey/simonsays-web) is the recommended editor; its boards import straight into SimonSays.

SimonSays is not meant to replace full communication suites such as Mind Express, TD Communicator, Grid 3 or OptiKey, nor the professional assessment and support that usually come with them. It does a narrower job — quick speech from the taskbar — and can run alongside them.

SimonSays is open source and free as in freedom, not only as in price (OSL-3.0), and it will stay that way. It has no ads and collects no data.

SimonSays is getting close to version 1.0 and needs people to try it in daily life, with their own voices, boards and eye trackers. If something fails, feels awkward or is missing, please tell us — see [Feedback and support](#feedback-and-support).

## Supported languages

SimonSays includes built-in default phrase sets and UI language labels for the following languages:

Arabic, Basque, Catalan, Chinese (Simplified), English, French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese (European), Portuguese (Brazil), Russian, Spanish, Valencian

> Note: available voices depend on what is installed in Windows. If a matching voice is not available, you can still use the phrase sets, but playback may fall back to another installed voice.

## What's new in v0.7

0.7 is the largest release so far and the last big step before 1.0. SimonSays can now be used hands-free, works with the taskbar on any edge of the screen, ships far more default phrases and ready-made boards, and can speak straight into video calls. Eye-tracking support in particular needs testing and feedback before 1.0 — please tell us how it works for you via the **Feedback** option.

Highlights:

- **Gaze / Dwell-click**: hands-free activation by resting your gaze (or the cursor) on a button — with any eye-control software that moves the cursor, or reading gaze directly from the Irisbond Hiru (HID mode) and the Tobii Eye Tracker 4C, Eye Tracker 5 and Tobii Dynavox PCEye 5. The button you look at lights up, a short lock-on delay stops stray glances, and a key fires only once until you look away.
- **Dialogs usable by gaze**: questions and messages now appear in SimonSays' own dialogs, with larger text and `Yes` / `No` in your language; their buttons — and those of Settings and the editing dialogs — can be dwelled, with a short delay so a question that pops up under your gaze cannot answer itself.
- **The taskbar can be anywhere**: the bar works with the Windows taskbar at the bottom, top, left or right, small or large, on scaled displays, and follows it when it moves. Clicking the text box opens a roomier three-line input box.
- **More default phrases, ordered by need**: much larger phrase sets in all 18 languages, a new `Care & Comfort` category, a phrase for meeting someone new, eight new conversational sounds, and the most urgent phrases placed fewest selections away. Brazilian Portuguese is now a language of its own.
- **Online meetings**: the new `Sound output` setting sends SimonSays to a virtual audio cable, so people in Teams, Zoom or Google Meet hear your phrases as if they came from your microphone.
- **Ready-made boards one click away**: the 📂 button opens your boards folder, with Yes / No, Pain & body, Spelling, Questions, Daily care, Games, a Meeting template, a Hearing check, Alert tones, Morse code, Fitzgerald Key example boards and your language's default phrases. Importing a whole board saves the previous one as `backup.ssz` first.
- **Easier editing**: a right-click menu in the Categories window, pasting many phrases at once (`Ctrl + V`), dragging in `.txt`, `.wav`/`.mp3` and board files, zoom (`F11` / `F12`) and auto-fit.
- **Board & category styles**: color, resize and restyle the Categories window and its buttons with a CSS-like `property:value;` syntax to build color-coded boards such as Fitzgerald Key. Styles travel with exported `.ssc`/`.ssz` files. Small touches can be made in the app; whole pictogram or Fitzgerald Key boards are easiest in [SimonSays Web](https://github.com/juan-rey/simonsays-web).
- **PNG and JPG icons, bundled resources**: icons accept `.png`/`.jpg` besides `.ico` and emoji, and `.ssz` boards carry their sounds and icons in one file; resources live under one shared `resources` folder, split per board (see [App data folders](#app-data-folders)).
- **Help in your browser**: `F1` opens the help as a web page in your language — searchable, printable and easy to share.
- **More reliable**: fixes for a crash when playing MP3 sounds, long categories or pasted text being lost, and changes failing to save without warning (SimonSays now tells you).

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
- Portuguese (European): `docs/help/HELP_pt.md`
- Portuguese (Brazil): `docs/help/HELP_pt_BR.md`
- Russian: `docs/help/HELP_ru.md`
- Spanish: `docs/help/HELP_es.md`
- Valencian: `docs/help/HELP_val.md`

## Usage — mixing speech and sounds

- Use the `SOUND_NOTE_DELIMITER` character (♫) to insert inline sound markers. Example:

  `Hello ♫notification.wav♫, please wait.`

  - Text outside markers is spoken securely using Windows TTS.
  - Text between markers is treated as an audio filename and played inline.

- Supported audio formats: `.wav`, `.mp3`. (`.mid`/`.midi` names are accepted but do not play.)

## Registry defaults and installation

- Default phrase sets are created on first run and stored under `HKCU\\SOFTWARE\\SimonSays\\Phrases\\<Language>`.

### Updating default phrases on an existing install

If you already have phrases stored for a language, SimonSays will not overwrite them automatically. To re-install the built-in defaults for a language (for example, Korean):

1. Close SimonSays.
2. Delete the language key under `HKCU\\SOFTWARE\\SimonSays\\Phrases\\<Language>` (e.g. `...\\Phrases\\Korean`).
3. Restart SimonSays (or run `InstallDefaultPhrases()`), and the defaults will be recreated.

## App data folders

Besides the registry (categories, phrases, and settings), SimonSays keeps a small set of files under `%LocalAppData%\SimonSays\`, created automatically on first run:

- `resources\` — shared home for imported/custom icon and sound files; a board whose style sets a `title` or `resource-folder` gets its own subfolder here, keeping its assets separate from other boards.
- `boards\` — default folder suggested by the Export dialog (`F10`); exporting elsewhere still works.
- `debug\` — diagnostic dumps (e.g. `hid_dump.txt`) used when troubleshooting eye-tracking hardware with support.

## Troubleshooting

- Voice initialization fails: verify SAPI is installed, voices are available, and the selected voice matches the UI language if desired.
- Sound files not playing: confirm path and supported extension; check file permissions and ensure the filename is between paired delimiters.

## Web / WordPress

Want to build or edit a board (`.ssc`/`.ssz`) in a browser, no install required? The **SimonSays Board Editor** WordPress plugin lives in a companion repository, [juan-rey/simonsays-web](https://github.com/juan-rey/simonsays-web): a stateless, client-side editor — board files are opened and saved locally in the browser, nothing is uploaded to a server — usable as a Gutenberg block or shortcode.

It's a separate repository because it's licensed GPL-2.0-or-later (required for WordPress plugins), while this desktop app is OSL-3.0 (see [License](#license)). The `.ssc`/`.ssz` format and board-style syntax it implements are documented here as the canonical, CC BY 4.0–licensed references — [`docs/guides/ssc-ssz-format-reference.md`](docs/guides/ssc-ssz-format-reference.md) and [`docs/guides/board-style-reference.md`](docs/guides/board-style-reference.md) — which `simonsays-web` vendors copies of.

## Contributing

See `CONTRIBUTING.md` for coding standards and commit guidelines. When filing issues include OS version, locale, voice selection, and any sound filenames used.

## License

Licensed under the Open Software License (OSL) v3.0. See `LICENSE` for full terms.
