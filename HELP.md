# SimonSays Help Guide

## Overview
SimonSays is a Windows tray-based text-to-speech (TTS) assistant, created by Juan Rey Saura, optimized for quick speech, phrase libraries, and inline sound playback. It supports multiple languages, customizable voices, and user-friendly features for efficient communication.

SimonSays utilizes the empty space on the left side of the Windows taskbar, providing quick access to a dozen categories of frequently used (and customizable) phrases with just a couple of clicks.

## Shortcuts
- `F1`: Open Help.
- `F2`: Open Settings.
- `F3`: Add after current selection (category/phrase context).
- `F4`: Edit current selection.
- `F5` / `F6`: Move to previous / next selection in lists.
- `F8`: Delete current selection.
- `F9`: Import categories.
- `F10`: Export categories.
- `Play>` button: Start playback.

## Launching and basic usage
1. Start SimonSays; it appears in the Windows system tray.
2. To speak, you can either:
   - Type custom text in the input box and press the `Play>` button (or `Enter`).
   - Click one or more saved phrases in the Categories window.
3. While playing, the button shows `>` to indicate active playback. You can press it again to stop instantly.

## Categories window (phrases)
- Open via the `Categories` button. Resize it or remember its size via Settings.
- Select a category at the top and phrases at the bottom.
- Single-click a phrase to auto-speak if `Speak phrase immediately when selecting` is enabled; otherwise, it will load the phrase into the main input box without speaking.

### Managing categories
- Add: select category → (shortcut: `F3`) `Add category` dialog → enter name → `OK`.
- Rename: select category → (shortcut: `F4`) `Edit` → change name → `OK`.
- Move selection: `F5`/`F6` to move to previous/next item when browsing lists.
- Delete: select category → (shortcut: `F8`) `Delete`; confirm deletion of category and its phrases.

#### Setting or changing a category icon
Each category can display an emoji or a custom `.ico` file on its button. The icon is specified in the Add/Edit dialog using the `##` separator prefix:

```
<icon>##<category name>
```

- **Emoji**: enter the emoji character(s) followed by `##` and the name.
  - Example: `🙂##Greetings`
- **Icon file** (`.ico`): enter the file path (absolute, or relative to the SimonSays folder) followed by `##` and the name.
  - Example: `C:\icons\smile.ico##Greetings`
- **No icon**: just enter the category name without any `##` prefix.
  - Example: `Greetings`

> Note: only `.ico` files are supported for file-based icons. Any other file extension is ignored and no icon is shown.

### Managing phrases
- Add: select phrase → (shortcut: `F3`) `Add phrase` → enter text → `OK`.
- Edit: select phrase → (shortcut: `F4`) `Edit` → update text → `OK`.
- Move selection: `F5`/`F6` to move to previous/next phrase.
- Delete: select phrase → (shortcut: `F8`) `Delete`; confirm.

#### Setting or changing a phrase icon
Phrases follow the same icon prefix convention as categories, with the optional `##` separator appearing before the phrase text:

```
<icon>##<phrase text>
```

- **Emoji**: `👍##Sounds good!`
- **Icon file**: `C:\icons\check.ico##Sounds good!`
- **No icon**: `Sounds good!`

Phrases can also include an inline audio file (see [Mixing speech and sounds](#mixing-speech-and-sounds)). When combining an icon, text, and an audio file, the full format is:

```
<icon>##<phrase text>::<audio file>
```

- Example: `🔔##Attention::notification.wav`

The `##` prefix and `::` suffix are both optional and independent; you can have any combination:
- Icon only prefix: `🔔##Attention`
- Audio only suffix: `Attention::notification.wav`
- Both: `🔔##Attention::notification.wav`
- Neither: `Attention`

## Settings (F2)
Open Settings (`F2`) to control:
- **Default text**: toggle `Insert default text automatically` and customize default phrase.
  - `Show touch keyboard` — opens the on-screen touch keyboard when the input box receives focus, positioned near the main window.
- **Language labels**: choose UI language (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voice**: pick installed SAPI voice matching your language preference.
- **Volume/Rate**: adjust `Volume (10-100)` and `Rate (-10 to 10)`.
- `Test Voice`: audition the selected voice.
- **Category window**:
  - `Minimize category window automatically` after selecting.
  - `Remember category window size`.
- **Playback behavior**:
  - `Speak phrase immediately when selecting` (single-click auto-speak).
  - `Stop previous playback` — automatically stops any ongoing playback before starting a new phrase.
  - `Increase SimonSays volume when playing` — raises the system master volume while speaking, restoring the previous level when playback finishes.
  - `Temporarily reduce other audio when playing` — reduces the volume of all other applications while speaking, restoring them when playback finishes.
- `OK` saves changes; `Cancel` discards.

## Tray icon menu
- `Show` / `Hide` main UI.
- `Settings`.
- `About` shows version and credits.
- `Web` opens project page if configured.
- `Feedback` opens a [feedback form](https://forms.gle/KMbpEDYmwnFJUhgy8) in the default browser.
- `Exit` quits SimonSays.

You can move the tray pop-up window and other dialogs by dragging their title bars; resize the Categories window by dragging its edges, and enable `Remember category window size` in Settings to persist the size.

## Mixing speech and sounds
- Use the delimiter `♫` (`SOUND_NOTE_DELIMITER`) to embed audio filenames in text:
  - Example: `Hello ♫notification.wav♫, please wait.`
  - Text outside delimiters is spoken; text inside is treated as a sound file and played inline.
- Supported audio: `.wav`, `.mid`, `.midi`, `.mp3`.
- Use absolute or relative paths accessible to the process. Pair delimiters to avoid trailing text being treated as audio.

### Sound and icon file lookup order
When a sound or icon filename does not contain a drive letter or full path, SimonSays searches for it in the following order, using the first match found:

1. **AppData folder** — `%APPDATA%\SimonSays\` (user-specific; place files here to keep them separate from the application install).
2. **Working directory** — the current working directory of the process (only searched if it differs from the executable folder).
3. **Executable folder** — the folder containing `SimonSays.exe`.

If the file is not found in any of these locations, the built-in fallback sound is used instead.

> **Tip:** placing your custom sound and icon files in `%APPDATA%\SimonSays\` is recommended — they will survive application updates and are user-specific.

## Importing and exporting categories
- Export: choose to export all categories or only the selected one when prompted. Success/failure messages appear per language (shortcut: `F10`).
- Import: choose a file; if a category exists, you will be asked to overwrite. Success/failure messages appear per language (shortcut: `F9`).

## Troubleshooting
- **Voice initialization fails**: verify SAPI is installed and a compatible voice is present.
- **No sound playback**: confirm file path/extension and paired delimiters `♫`.
- **Import/export errors**: check file permissions and disk location; retry.
- **UI language not applied**: reopen Settings and confirm language selection; ensure localization strings exist for the chosen language.

## About
- `About` dialog lists version, description, and copyright.
