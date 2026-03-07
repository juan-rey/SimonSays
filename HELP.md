# SimonSays Help Guide

## Overview
SimonSays is a Windows tray-based text-to-speech (TTS) assistant optimized for quick speech, phrase libraries, and inline sound playback.

## Launching and basic usage
1. Start SimonSays; it appears in the Windows taskbar tray.
2. Left-click the tray icon or choose `Show` to open the main window.
3. Type custom text in the input box, or click a saved phrase in the Categories window.
4. Press the `Play>` button (or hotkey if configured) to speak.
5. While playing, the button shows `>` to indicate active playback.

## Categories window (phrases)
- Open via `Categories` button. Resize/remember size via Settings.
- Select a category on the top and phrases on the bottom.
- Single-click a phrase to auto-speak if `Speak phrase immediately when selecting` is enabled otherwise it will load the phrase into the main input box without speaking.

### Managing categories
- Add: select category → (shortcut: `F3`) `Add category` dialog → enter name → `OK`.
- Rename: select category → (shortcut: `F4`) `Edit` → change name → `OK`.
- Delete: select category → (shortcut: `F8`) `Delete`; confirm deletion of category and its phrases.
- Move selection: `F5`/`F6` to move to previous/next item when browsing lists.

### Managing phrases
- Add: select phrase → (shortcut: `F3`) `Add phrase` → enter text → `OK`.
- Edit: select phrase → (shortcut: `F4`) `Edit` → update text → `OK`. 
- Delete: select phrase → (shortcut: `F8`) `Delete`; confirm.
- Move selection: `F5`/`F6` to move to previous/next phrase.

## Settings (F2)
Open Settings (`F2`) to control:
- **Default text**: toggle `Insert default text automatically` and customize default phrase.
- **Language labels**: choose UI language (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voice**: pick installed SAPI voice matching your language preference.
- **Volume/Rate**: adjust `Volume (10-100)` and `Rate (-10 to 10)`.
- **Playback behavior**:
  - `Speak phrase immediately when selecting` (single-click auto-speak).
  - `Remember category window size`.
  - `Minimize category window automatically` after selecting.
  - `Increase SimonSays volume when playing`.
  - `Temporarily reduce other audio when playing`.
- `Test Voice`: audition the selected voice.
- `OK` saves changes; `Cancel` discards.

## Tray icon menu
- `Show` / `Hide` main UI.
- `Settings`.
- `About` shows version and credits.
- `Web` opens project page if configured.
- `Exit` quits SimonSays.

You can move the tray pop-up window and other dialogs by dragging their title bars; resize the Categories window by dragging its edges, and enable `Remember category window size` in Settings to persist the size.

## Mixing speech and sounds
- Use delimiter `♫` (SOUND_NOTE_DELIMITER) to embed audio filenames in text:
  - Example: `Hello ♫notification.wav♫, please wait.`
  - Text outside delimiters is spoken; text inside is treated as a sound file and played with `PlaySound`.
- Supported audio: `.wav`, `.mid`, `.midi`.
- Use absolute or relative paths accessible to the process. Pair delimiters to avoid trailing text being treated as audio.

## Importing and exporting phrases
- Export: choose to export all categories or only the selected one when prompted. Success/failure messages appear per language (shortcut: `F10`).
- Import: choose a file; if a category exists, you will be asked to overwrite. Success/failure messages appear per language (shortcut: `F9`).

## Defaults storage
- Default phrases are created per language under registry key `HKCU\SOFTWARE\SimonSays\Phrases\<Language>` on first run.
- To reinstall defaults for a language: close SimonSays → delete the language key → restart SimonSays.

## Taskbar and window behavior
- SimonSays currently supports bottom taskbar alignment.
- Category window can auto-minimize or remember size based on Settings.

## Troubleshooting
- **Voice initialization fails**: verify SAPI is installed and a compatible voice is present.
- **No sound playback**: confirm file path/extension and paired delimiters `♫`.
- **Import/export errors**: check file permissions and disk location; retry.
- **UI language not applied**: reopen Settings and confirm language selection; ensure localization strings exist for the chosen language.

## Shortcuts
- `F1`: open Help.
- `F2`: open Settings.
- `F3`: add after current selection (category/phrase context).
- `F4`: edit current selection.
- `F5` / `F6`: move to previous / next selection in lists.
- `F8`: delete current selection.
- `F9`: import phrases.
- `F10`: export phrases.
- `Play>` button: start playback (click).

## About
- `About` dialog lists version, description, and copyright.
