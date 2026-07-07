# SimonSays Help Guide

## Overview
SimonSays is a Windows tray-based text-to-speech (TTS) assistant created by Juan Rey Saura. It is optimized for fast speech, phrase libraries, and inline sound playback. It supports multiple languages, customizable voices, and features designed for efficient communication.

SimonSays uses the empty space to the left of the Windows taskbar, providing quick access to a dozen categories of frequently used (and customizable) phrases with just a couple of clicks.

## Shortcuts
- `F1`: Open Help
- `F2`: Open Settings
- `F3`: Dwell settings
- `F4`: Edit current selection
- `F5` / `F6`: Move to previous / next selection in lists
- `F7`: Add after current selection (category/phrase context)
- `F8`: Delete current selection
- `F9`: Import categories
- `F10`: Export categories
- `F11` / `Ctrl +`: Zoom out (Categories window)
- `F12` / `Ctrl +`: Zoom in (Categories window)
- `Ctrl + 0`: Reset zoom
- `Play` button: Start playback

## Launching and basic usage
1. Start SimonSays. It appears in the Windows system tray.
2. To speak, you can either:
   - Type custom text in the input box and press the `Play>` button (or `Enter`).
   - Click one or more saved phrases in the Categories window.
3. While playing, the button shows `>` to indicate active playback. You can press it again to stop instantly.

## Categories window (phrases)
- Open via the `Categories` button. Resize it or enable "Remember category window size" in Settings to persist the size.
- Select a category from the top list and phrases from the bottom list.
- Single-click a phrase to auto-speak if `Speak phrase immediately when selecting` is enabled; otherwise the phrase will load into the main input box without speaking.

### Managing categories
- Rename: select category → (shortcut: `F4`) `Edit` → change name → `OK`.
- Move selection: `F5`/`F6` to move to previous/next item when browsing lists.
- Add: select category → (shortcut: `F7`) `Add category` dialog → enter name → `OK`.
- Delete: select category → (shortcut: `F8`) `Delete`; confirm deletion of the category and its phrases.

#### Setting or changing a category icon
Each category can display an emoji or a custom `.ico` file on its button. Specify the icon in the Add/Edit dialog using the `##` separator prefix:

```
<icon>##<category name>
```

- **Emoji**: enter the emoji character(s) followed by `##` and the name.
  - Example: `🙂##Greetings`
- **Icon file** (`.ico`): enter the file path (absolute or relative to the SimonSays folder) followed by `##` and the name.
  - Example: `C:\icons\smile.ico##Greetings`
- **No icon**: just enter the category name without any `##` prefix.
  - Example: `Greetings`

> Note: Only `.ico` files are supported for file-based icons. Other file extensions are ignored and no icon is shown.

### Managing phrases
- Edit: select phrase → (shortcut: `F4`) `Edit` → update text → `OK`.
- Move selection: `F5`/`F6` to move to previous/next phrase.
- Add: select phrase → (shortcut: `F7`) `Add phrase` → enter text → `OK`.
- Delete: select phrase → (shortcut: `F8`) `Delete`; confirm.

#### Setting or changing a phrase icon
Phrases follow the same icon prefix convention as categories, with the optional `##` separator appearing before the phrase text:

```
<icon>##<phrase text>
```

- **Emoji**: `👍##Sounds good!`
- **Icon file**: `C:\icons\check.ico##Sounds good!`
- **No icon**: `Sounds good!`

Phrases can include an inline audio file (see [Mixing speech and sounds](#mixing-speech-and-sounds)). To combine icon, text, and audio use the format:

```
<icon>##<phrase text>::<audio file>
```

- Example: `🔔##Attention::notification.wav`

- Icon only prefix: `🔔##Attention`
- Audio only suffix: `Attention::notification.wav`
- Both: `🔔##Attention::notification.wav`
- Neither: `Attention`
The `##` prefix and `::` suffix are both optional and independent; you can use any combination:
- Icon only prefix: `🔔##Attention`
- Audio only suffix: `Attention::notification.wav`
- Both: `🔔##Attention::notification.wav`
- Neither: `Attention`
- Icon only prefix: `🔔##Attention`
- Audio only suffix: `Attention::notification.wav`
- Both: `🔔##Attention::notification.wav`
- Neither: `Attention`

## Settings (F2)
Open Settings (`F2`) to control:
- **Default text**: toggle `Insert default text automatically` and customize the default phrase.
  - `Show touch keyboard` — opens the on-screen touch keyboard when the input box receives focus; it is positioned near the main window.
- **UI language**: choose the interface language (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voice**: pick an installed SAPI voice matching your language preference.
- **Volume/Rate**: adjust `Volume (10-100)` and `Rate (-10 to 10)`.
- `Test Voice`: audition the selected voice.
- **Category window**:
  - `Minimize category window automatically` after selecting.
  - `Remember category window size`.
- **Playback behavior**:
  - `Speak phrase immediately when selecting` (single-click auto-speak).
  - `Stop previous playback` — automatically stops any ongoing playback before starting a new phrase.
  - `Increase SimonSays volume when playing` — raises the system master volume while speaking and restores the previous level when playback finishes.
  - `Temporarily reduce other audio when playing` — reduces the volume of other applications while speaking and restores them when playback finishes.
- `OK` saves changes; `Cancel` discards.

## Gaze / Dwell-click (F3)
Activate phrases and buttons hands-free by holding your gaze (or the mouse cursor) on them for a set duration. This feature is new — it needs testing and feedback: please share how it works with your tracker through the tray menu `Feedback` option.

Supported setups:
- **Any eye-control software that moves the mouse cursor** (Irisbond EasyClick, Tobii Dynavox Computer Control, Windows Eye Control cursor modes, OptiKey): dwell-click follows the cursor.
- **Direct gaze reading (no cursor movement needed)**: Irisbond Hiru (HID mode) and Tobii trackers through their own software — verified with the Tobii Eye Tracker 4C and Tobii Dynavox PCEye 5. The Tobii Eye Tracker 5 is expected to work but has not been tested yet.

Open the Gaze / Dwell-click settings (`F3`) to configure:
- **Dwell mode**: choose one of the following options:
  - `Automatic` — detects device presence and enables dwell-click using the mouse cursor or gaze detection as appropriate.
  - `Mouse cursor` — always enable dwell-click using the mouse cursor.
  - `HID eye tracker` — always enable dwell-click using gaze detection.
  - `Off` — always disable dwell-click.
  - **Dwell time**: set the duration (in milliseconds) required to trigger a click when gazing at a UI element. A progress indicator is shown while the dwell is in progress.
  - **Tolerance radius**: adjust the gaze tolerance (in pixels) to allow for slight eye movement while still registering a dwell-click.
  - **Cooldown**: time in milliseconds after activation during which dwell-click cannot trigger (prevents accidental repeated clicks).
  - **Progress color**: choose the color of the dwell progress indicator (defaults to the Windows accent color).
  - **Detect how you use the tracker**: two buttons to help the app detect whether you are moving the mouse cursor with an eye tracker or simply clicking on buttons.
  - **Detected**: shows the detected device type and further information about related software.
  - A `Reset` button (bottom left) restores all dwell settings to their defaults.
  - An `OK` button saves changes; `Cancel` discards.

## Tray icon menu
- `Show` / `Hide` main UI
- `Settings`
- `Gaze / Dwell-click`
- `About` shows version and credits
- `Web` opens the project page if configured
- `Feedback` opens a [feedback form](https://forms.gle/KMbpEDYmwnFJUhgy8) in the default browser
- `Exit` quits SimonSays

You can move the tray pop-up window and other dialogs by dragging their title bars; resize the Categories window by dragging its edges, and enable `Remember category window size` in Settings to persist the size.

## Mixing speech and sounds
- Use the delimiter `♫` (`SOUND_NOTE_DELIMITER`) to embed audio filenames in text:
  - Example: `Hello ♫notification.wav♫, please wait.`
  - Text outside delimiters is spoken; text inside is treated as a sound file and played inline.
- Supported audio: `.wav`, `.mid`, `.midi`, `.mp3`.
- Use absolute or relative paths accessible to the process. Ensure delimiters are paired to avoid trailing text being treated as audio.

### Sound and icon file lookup order
When a sound or icon filename does not contain a drive letter or full path, SimonSays searches for it in the following order, using the first match found:

1. **AppData folder** — `%APPDATA%\SimonSays\` (user-specific; place files here to keep them separate from the application install).
2. **Working directory** — the current working directory of the process (only searched if it differs from the executable folder).
3. **Executable folder** — the folder containing `SimonSays.exe`.

If the file is not found in any of these locations, the built-in fallback sound is used instead.

> **Tip:** We recommend placing custom sound and icon files in `%APPDATA%\SimonSays\`; they will survive application updates and are user-specific.

## Importing and exporting categories
- Export: choose to export all categories or only the selected one when prompted. Success/failure messages appear in the selected UI language (shortcut: `F10`).
- Import: choose a file; if a category exists, you will be asked to overwrite. Success/failure messages appear in the selected UI language (shortcut: `F9`).

## Troubleshooting
- **Voice initialization fails**: verify SAPI is installed and a compatible voice is present.
- **No sound playback**: confirm file path/extension and paired delimiters `♫`.
- **Import/export errors**: check file permissions and disk location; retry.
- **UI language or voice changes not applied**: reopen Settings and confirm the selection. If changes still do not apply, restart SimonSays and try again.

## About
- `About` dialog lists version, description, and copyright.
