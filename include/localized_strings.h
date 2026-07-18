/*
   localized_strings.h: localized_strings header file.

   Copyright 2025-2026 Juan Rey Saura

   This file is part of SimonSays – Simply Speak (Text-to-Speech Utility).

   This software is a copyrighted work licensed under the Open Software License version 3.0
   Please consult the file "LICENSE" for details.
*/
#ifndef localized_strings_h
#define localized_strings_h

#include "stdafx.h"

/*
  ==========================================================================
  UI STRING LOCALIZATION -- how this file is organized  (READ BEFORE EDITING)
  ==========================================================================

  Encoding: UTF-8 WITH BOM, CRLF line endings. Preserve both when editing
  (the apply script below does).

  Layout:
    - DEFAULT_LOCALIZED_UI_STRINGS   -> English. The BASE and the guaranteed
                                        fallback; keep it COMPLETE.
    - <LANG>_LOCALIZED_UI_STRINGS    -> one { STRING_ID, L"text" } table per
                                        language (Spanish, Catalan, ...).
    - LOCALIZED_STRINGS (bottom)     -> maps each language name to its table.

  Lookup (utils.cpp GetLocalizedString): returns the language's string for an
  id; on a miss it falls back to English; if English also lacks it, "". So a
  MISSING id -- or one still left in English -- silently shows English at
  runtime. That is what "pending translation" means here: nothing breaks, it
  just isn't localized yet.

  HELP_CONTENT_ID is GENERATED from HELP.md / docs/help/HELP_<xx>.md by
  scripts/sync_help_content.ps1 -- never hand-edit it (edit the .md, re-run the
  script). The apply script below leaves it untouched.

  Conventions when translating:
    - Portuguese here is Brazilian (PT-BR): "mouse", "arquivos", "Excluir"...
    - Valencian mirrors Catalan (same forms).
    - Leave language-neutral values in English: "OK", "Web", ">", "'?", and
      words already correct in the target (e.g. "Categories" is valid Catalan,
      "Volume" valid gl/it/pt). find_pending flags these; skip them by choice.
    - File-dialog FILTER strings look like
        "<label>\0*.ssc;*.ssz\0<label>\0*.*\0"
      Translate only the <label> parts; keep the \0 segments and the *.* /
      *.ssc patterns literal. Escape sequences \n and \0 stay literal.

  To complete pending translations (tooling in scripts/):
    1. powershell -File scripts/find_pending_translations.ps1
           [-Languages GERMAN,ITALIAN] [-Mode both] [-OutTemplate pending.tsv]
       Lists MISSING / still-English ids per language (with English source) and
       optionally writes a fill-in TSV template (LANG<TAB>ID<TAB>text).
    2. Edit the TSV: translate the 3rd column. Keep \n and \0 literal; UTF-8.
    3. powershell -File scripts/apply_translations.ps1 -Tsv pending.tsv
       Inserts new entries / replaces English ones in place, preserving BOM and
       CRLF (Valencian is auto-mirrored from Catalan unless disabled).
    4. Build Debug + Release x64 (this header is included widely).
  ==========================================================================
*/

static const std::vector<std::pair<int, const wchar_t *>> DEFAULT_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Play>" },
  { DWELL_DIALOG_TITLE_ID, L"Gaze / Dwell-click" },
  { DWELL_INTRO_ID, L"Activate buttons by holding your gaze (or the cursor) on them." },
  { DWELL_MODE_GROUP_ID, L"Activation mode" },
  { DWELL_MODE_AUTO_ID, L"Automatic" },
  { DWELL_MODE_MOUSE_ID, L"Mouse cursor" },
  { DWELL_MODE_HID_ID, L"HID eye tracker" },
  { DWELL_MODE_OFF_ID, L"Off" },
  { DWELL_TIME_LABEL_ID, L"Dwell time (ms)" },
  { DWELL_TOLERANCE_LABEL_ID, L"Tolerance radius (px)" },
  { DWELL_COOLDOWN_LABEL_ID, L"Cooldown (ms)" },
  { DWELL_COLOR_BUTTON_ID, L"Progress color..." },
  { DWELL_DETECT_GROUP_ID, L"Detect how you use the tracker" },
  { DWELL_PROBE_LOOK_ID, L"Activate me by LOOKING at me" },
  { DWELL_PROBE_MOUSE_ID, L"Activate me with your MOUSE or switch" },
  { DWELL_STATUS_INITIAL_ID, L"Try the two buttons above to set the best mode automatically." },
  { DWELL_STATUS_GAZE_ID, L"Detected: gaze-driven cursor. Dwell-click enabled." },
  { DWELL_STATUS_MOUSE_ID, L"Detected: manual mouse. Dwell-click turned off." },
  { DWELL_STATUS_LOOK_HINT_ID, L"Please LOOK at the button without clicking it." },
  { DWELL_APPLY_BUTTON_ID, L"Apply" },
  { DWELL_HID_UNAVAILABLE_ID, L"HID eye tracking is not available yet." },
  { TRAYICON_DWELL_ID, L"Gaze / Dwell-click..." },
  { DWELL_SIGNALS_GROUP_ID, L"Detected" },
  { DWELL_SIGNAL_HID_LABEL_ID, L"Eye tracker (HID):" },
  { DWELL_SIGNAL_TOOL_LABEL_ID, L"Eye-control app:" },
  { DWELL_SIGNAL_WEC_LABEL_ID, L"Windows Eye Control:" },
  { DWELL_SIGNAL_YES_ID, L"yes" },
  { DWELL_SIGNAL_NO_ID, L"no" },
  { DWELL_SIGNAL_NONE_ID, L"none" },
  { DWELL_SIGNAL_HID_LIVE_ID, L"streaming gaze" },
  { DWELL_SIGNAL_HID_IDLE_ID, L"present (not streaming)" },
  { DWELL_RESET_BUTTON_ID, L"Reset" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Settings" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Default text:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insert default text automatically" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Language:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voice:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Test Voice" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Rate (-10 to 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Speak phrase immediately when selecting" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Remember category window size" },
  { SETTINGS_AUTORESIZE_CATEGORY_WINDOW_ID, L"Auto-resize category window to fit content" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimize category window automatically" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Increase SimonSays volume when playing" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Temporarily reduce other audio when playing" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Click to show" },
  { TRAYICON_SHOW_ID, L"Show" },
  { TRAYICON_HIDE_ID, L"Hide" },
  { TRAYICON_SETTINGS_ID, L"Settings" },
  { TRAYICON_ABOUT_ID, L"About" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Exit" },
  { ABOUT_TITTLE_ID, L"About SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simply Speak\n\nVersion " },
  { ABOUT_2_ID, L"\n\nA simple accessibility tool that \nlets you turn text into speech instantly.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays Error" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays currently only supports bottom taskbar" },
  { BOARD_STYLE_CHANGED_TITLE_ID, L"Board Style Changed" },
  { BOARD_STYLE_CHANGED_MESSAGE_ID, L"Do you want to revert the changes?" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Change category name" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Edit phrase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Current:" },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancel" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"A category with this name already exists. Please choose a different name." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Category Name Conflict" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Delete Category" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Delete category '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' and all its phrases?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, L"Delete all categories and their phrases?\nWarning: This action cannot be undone." },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, L"Is this an error?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, L"Delete All Categories" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Delete Phrase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Delete phrase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Add new category" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Add new phrase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Category name:" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Phrase text:" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancel" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"New default phrase" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Import Categories" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Overwrite Category '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Overwrite Category?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Replace Board Style?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"The imported file includes a board style. Replace the current one?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Categories imported successfully." },
  { IMPORT_SUCCESS_TITLE_ID, L"Import Successful" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Failed to import categories." },
  { IMPORT_FAILURE_TITLE_ID, L"Import Failed" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Export Categories" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"You can export all categories or just the selected one.\nDo you want to export only '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' category?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Export Selection" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Categories exported successfully." },
  { EXPORT_SUCCESS_TITLE_ID, L"Export Successful" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Failed to export categories." },
  { EXPORT_FAILURE_TITLE_ID, L"Export Failed" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Help - F2: Settings" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays Help Guide

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
Each category can display an emoji or a custom image file (`.ico`, `.png`, or `.jpg`) on its button. Specify the icon in the Add/Edit dialog using the `##` separator prefix:

```
<icon>##<category name>
```

- **Emoji**: enter the emoji character(s) followed by `##` and the name.
  - Example: `🙂##Greetings`
- **Icon file** (`.ico`, `.png`, `.jpg`): enter the file path (absolute or relative to the SimonSays folder) followed by `##` and the name.
  - Example: `C:\icons\smile.png##Greetings`
- **No icon**: just enter the category name without any `##` prefix.
  - Example: `Greetings`

> Note: Only `.ico`, `.png`, and `.jpg` files are supported for file-based icons. Other file extensions are ignored and no icon is shown. PNG images keep their transparency; very large images are automatically reduced when loaded.

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
- **Icon file**: `C:\icons\check.png##Sounds good!`
- **No icon**: `Sounds good!`

Phrases can include an inline audio file (see [Mixing speech and sounds](#mixing-speech-and-sounds)). To combine icon, text, and audio use the format:

```
<icon>##<phrase text>::<audio file>
```

- Example: `🔔##Attention::notification.wav`

The `##` prefix and `::` suffix are both optional and independent; you can use any combination:
- Icon only prefix: `🔔##Attention`
- Audio only suffix: `Attention::notification.wav`
- Both: `🔔##Attention::notification.wav`
- Neither: `Attention`

## Customizing the look (board & category styles)
You can color, resize, and restyle the Categories window and its buttons — for example to build color-coded communication boards such as SPC (Sistema Pictográfico de Comunicación). Styles are optional: without them the window keeps its default look.

A style is a list of `property:value;` pairs. Values can be:
- **Colors** — `#RRGGBB` hex, e.g. `#FFD966`.
- **Sizes** — a number of pixels (`96`) or a percentage of the default (`120%`).
- **Keywords** — e.g. `left`, `right`, `top`, `bottom`, `smart`, `normal`, `bold`.
- **Text** — a font name, or the caption/title/credits text.

Anything the app doesn't recognize (a misspelled property or a bad value) is simply ignored, so a style never breaks the window.

### Styling one category (Edit dialog)
Select a category and press `F4` (Edit), then add `::` and a style after the name:

```
<icon>##<category name>::<property>:<value>;<property>:<value>;
```

- Example: `🧍##People::background:#FFD966;` colors the People button yellow. The category's color **also applies to the phrases inside it**, so one line colors a whole word group — the core of an SPC board.
- A category style can set `background` and `text-color` for the category button, and any phrase property (below) prefixed with `phrase-`, e.g. `phrase-corner-radius:8;`.

### Styling the whole board (`$$board`)
The overall look — window background, separators, and defaults for every category and phrase button — is a **board style**. There is no in-app editor for it yet, so you get a board style by **importing** a styled categories file (`.ssc` / `.ssz`); advanced users can also edit it directly. It is stored under the reserved category name `$$board` (you cannot use `$$` names for your own categories).

When you import a file that contains a board style, SimonSays asks before replacing your current one, and shows the board's `title` and `credits` (if any) in the import message.

### Style properties
- **Whole window**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (label shown between the separators), `title` and `credits` (shown when the board is imported).
- **All category buttons** (`category-` prefix) and **all phrase buttons** (`phrase-` prefix): `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Setting `icon-position` to `center` places the icon in the middle of the button and hides the text (an icon-only key); use `top` instead if you want the icon above with the name below. If you do want text over a centered icon, add an explicit `text-layout` (for example `text-layout:bottom`) and it will be shown at that position.

`text-layout` accepts a horizontal keyword (`left`/`center`/`right`) and/or a vertical one (`top`/`middle`/`bottom`), or `smart`. The default is smart)HELP" LR"HELP(: the text is centered in the space left over by the icon (icon on top → caption centered below; centered in the whole button when there is no icon).

### Example: an SPC color-coded board
To color six word groups, edit each category (`F4`) and give it one background color. These are the exact strings you type in the Edit dialog:

```
🧍##People::background:#FFD966;
🏃##Verbs::background:#93C47D;
📏##Describe::background:#6FABDC;
🍎##Nouns::background:#F6B26B;
🕒##Misc::background:#FFFFFF;
👋##Social::background:#B4A7D6;
```

Each color also colors the phrases inside that category, so the whole board becomes color-coded. Board-wide touches — such as putting every category's emoji on top with its name below, and coloring the separators — come from a board style like:

```
category-icon-position:top;separator-color:#B4A7D6;
```

Styles travel with your categories: exporting (`F10`) includes them, and importing (`F9`) brings them along, so a styled board can be shared as a single file.

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
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Stop Previous Playback" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Show Touch Keyboard" },
  { TRAYICON_FEEDBACK_ID, L"Feedback" },
  { AMPERSAND_REPLACEMENT_ID, L"and" },
};

static const std::vector<std::pair<int, const wchar_t *>> SPANISH_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Reproducir>" },
  { DWELL_DIALOG_TITLE_ID, L"Mirada / Clic por fijación" },
  { DWELL_INTRO_ID, L"Activa los botones manteniendo la mirada (o el cursor) sobre ellos." },
  { DWELL_MODE_GROUP_ID, L"Modo de activación" },
  { DWELL_MODE_AUTO_ID, L"Automático" },
  { DWELL_MODE_MOUSE_ID, L"Cursor del ratón" },
  { DWELL_MODE_HID_ID, L"Seguidor ocular HID" },
  { DWELL_MODE_OFF_ID, L"Desactivado" },
  { DWELL_TIME_LABEL_ID, L"Tiempo de fijación (ms)" },
  { DWELL_TOLERANCE_LABEL_ID, L"Radio de tolerancia (px)" },
  { DWELL_COOLDOWN_LABEL_ID, L"Tiempo de espera (ms)" },
  { DWELL_COLOR_BUTTON_ID, L"Color de progreso..." },
  { DWELL_DETECT_GROUP_ID, L"Detectar cómo usas el seguidor" },
  { DWELL_PROBE_LOOK_ID, L"Actívame MIRÁNDOME" },
  { DWELL_PROBE_MOUSE_ID, L"Actívame con el RATÓN o pulsador" },
  { DWELL_STATUS_INITIAL_ID, L"Prueba los dos botones para elegir el mejor modo automáticamente." },
  { DWELL_STATUS_GAZE_ID, L"Detectado: cursor controlado por la mirada. Clic por fijación activado." },
  { DWELL_STATUS_MOUSE_ID, L"Detectado: ratón manual. Clic por fijación desactivado." },
  { DWELL_STATUS_LOOK_HINT_ID, L"Por favor, MIRA el botón sin hacer clic." },
  { DWELL_APPLY_BUTTON_ID, L"Aplicar" },
  { DWELL_HID_UNAVAILABLE_ID, L"El seguimiento ocular HID aún no está disponible." },
  { TRAYICON_DWELL_ID, L"Mirada / Clic por fijación..." },
  { DWELL_SIGNALS_GROUP_ID, L"Detectado" },
  { DWELL_SIGNAL_HID_LABEL_ID, L"Seguidor ocular (HID):" },
  { DWELL_SIGNAL_TOOL_LABEL_ID, L"App de control ocular:" },
  { DWELL_SIGNAL_WEC_LABEL_ID, L"Control ocular de Windows:" },
  { DWELL_SIGNAL_YES_ID, L"sí" },
  { DWELL_SIGNAL_NO_ID, L"no" },
  { DWELL_SIGNAL_NONE_ID, L"ninguna" },
  { DWELL_SIGNAL_HID_LIVE_ID, L"transmitiendo mirada" },
  { DWELL_SIGNAL_HID_IDLE_ID, L"presente (sin transmitir)" },
  { DWELL_RESET_BUTTON_ID, L"Restablecer" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorías" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuración" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texto predeterminado:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insertar texto predeterminado automáticamente" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voz:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Probar voz" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volumen (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocidad (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Decir la frase al seleccionarla" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Recordar tamaño de la ventana de categorías" },
  { SETTINGS_AUTORESIZE_CATEGORY_WINDOW_ID, L"Autoajustar tamaño de la ventana de categorías" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar ventana de categorías automáticamente" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumentar volumen de SimonSays al reproducir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Reducir temporalmente otros sonidos al reproducir" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Detener reproducción anterior" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Mostrar teclado táctil" },
  { SETTINGS_OK_BUTTON_ID, L"Aceptar" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancelar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Haz clic para mostrar" },
  { TRAYICON_SHOW_ID, L"Mostrar" },
  { TRAYICON_HIDE_ID, L"Ocultar" },
  { TRAYICON_SETTINGS_ID, L"Configuración" },
  { TRAYICON_ABOUT_ID, L"Acerca de" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Salir" },
  { ABOUT_TITTLE_ID, L"Acerca de SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplemente Habla\n\nVersión " },
  { ABOUT_2_ID, L"\n\nUna herramienta de accesibilidad sencilla que\nconvierte texto en voz al instante.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Error de SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays solo admite la barra de tareas inferior actualmente" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Cambiar nombre de categoría" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Editar frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"Aceptar" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Ya existe una categoría con este nombre. Por favor, elige un nombre diferente." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflicto de nombres de categoría" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Eliminar categoría" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"¿Eliminar categoría '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' y todas sus frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Eliminar frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"¿Eliminar frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Agregar nueva categoría" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Agregar nueva frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nombre de categoría :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Texto de frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"Aceptar" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nueva frase predeterminada" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categorías" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categorías de SimonSays\0*.ssc;*.ssz\0Todos los archivos\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"¿Sobrescribir la categoría '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"¿Sobrescribir categoría?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"¿Reemplazar el estilo del tablero?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"El archivo importado incluye un estilo de tablero. ¿Reemplazar el actual?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importadas correctamente." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importación exitosa" },
  { IMPORT_FAILURE_MESSAGE_ID, L"No se pudieron importar las frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Error al importar" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categorías" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categorías de SimonSays\0*.ssc;*.ssz\0Todos los archivos\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Puedes exportar todas las categorías o solo la seleccionada.\n¿Quieres exportar solo la categoría '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar selección" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportadas correctamente." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportación exitosa" },
  { EXPORT_FAILURE_MESSAGE_ID, L"No se pudieron exportar las frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Error al exportar" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Ayuda - F2: Configuración" },
  { HELP_CONTENT_ID, LR"HELP(# Guía de Ayuda de SimonSays

## Descripción general
SimonSays es un asistente de texto a voz (TTS) para Windows, creado por Juan Rey Saura, optimizado para habla rápida, bibliotecas de frases y reproducción de sonidos en línea. Soporta varios idiomas, voces configurables y funciones pensadas para una comunicación eficiente.

SimonSays usa la zona libre a la izquierda de la barra de tareas de Windows para facilitar el acceso a una decena de categorías con muchas frases frecuentes (modificables) al alcance de un par de clics.

## Atajos
- `F1`: Abrir Ayuda.
- `F2`: Abrir Configuración.
- `F3`: Configuración de Mirada / Clic por fijación.
- `F4`: Editar selección actual.
- `F5` / `F6`: Mover a la selección anterior / siguiente en listas.
- `F7`: Añadir después de la selección actual (contexto categoría/frase).
- `F8`: Eliminar selección actual.
- `F9`: Importar categorías.
- `F10`: Exportar categorías.
- `F11` / `Ctrl -`: Alejar zoom (ventana de categorías).
- `F12` / `Ctrl +`: Acercar zoom (ventana de categorías).
- `Ctrl 0`: Restablecer zoom.
- Botón `Reproducir>`: Iniciar reproducción.

## Inicio y uso básico
1. Inicia SimonSays; aparece en la bandeja de la barra de tareas de Windows.
2. Para hablar, puedes:
   - Escribir texto personalizado en la caja de entrada y pulsar el botón `Reproducir>` (o `Enter`).
   - Hacer clic en una o varias frases guardadas en la ventana de Categorías.
3. Durante la reproducción, el botón muestra `>` para indicar estado activo. Puedes pulsarlo de nuevo para detener la reproducción al instante.

## Ventana de Categorías (frases)
- Ábrela con el botón `Categorías`. Puedes redimensionarla o recordar su tamaño desde Configuración.
- Selecciona una categoría arriba y frases abajo.
- Un clic en una frase la reproduce automáticamente si `Decir la frase al seleccionarla` está activado; si no, carga la frase en la caja principal sin hablar.

### Gestionar categorías
- Renombrar: selecciona categoría → (atajo: `F4`) `Cambiar nombre de categoría` → cambia nombre → `Aceptar`.
- Mover selección: `F5`/`F6` para mover al elemento anterior/siguiente.
- Añadir: selecciona categoría → (atajo: `F7`) diálogo `Agregar nueva categoría` → escribe nombre → `Aceptar`.
- Eliminar: selecciona categoría → (atajo: `F8`) `Eliminar categoría`; confirma eliminación de categoría y frases.



#### Configurar o cambiar el icono de una categoría
Cada categoría puede mostrar un emoji o un archivo personalizado `.ico` en su botón. El icono se especifica en el diálogo Agregar/Editar usando el prefijo separador `##`:

```
<icono>##<nombre de categoría>
```

- **Emoji**: escribe el/los emoji seguidos de `##` y el nombre.
  - Ejemplo: `🙂##Saludos`
- **Archivo de icono** (`.ico`): escribe la ruta del archivo (absoluta o relativa a la carpeta de SimonSays) seguida de `##` y el nombre.
  - Ejemplo: `C:\icons\smile.ico##Saludos`
- **Sin icono**: escribe solo el nombre de la categoría sin prefijo `##`.
  - Ejemplo: `Saludos`

> Nota: solo se admiten archivos `.ico` para iconos basados en archivo. Cualquier otra extensión se ignora y no se muestra icono.

### Gestionar frases
- Editar: selecciona frase → (atajo: `F4`) `Editar frase` → actualiza texto → `Aceptar`.
- Mover selección: `F5`/`F6` para mover a la frase anterior/siguiente.
- Añadir: selecciona frase → (atajo: `F7`) `Agregar nueva frase` → escribe texto → `Aceptar`.
- Eliminar: selecciona frase → (atajo: `F8`) `Eliminar frase`; confirma.



#### Configurar o cambiar el icono de una frase
Las frases siguen la misma convención de prefijo de icono que las categorías, con el separador opcional `##` antes del texto de la frase:

```
<icono>##<texto de frase>
```

- **Emoji**: `👍##¡Suena bien!`
- **Archivo de icono**: `C:\icons\check.ico##¡Suena bien!`
- **Sin icono**: `¡Suena bien!`

Las frases también pueden incluir un archivo de audio en línea. Al combinar icono, texto y archivo de audio, el formato completo es:

```
<icono>##<texto de frase>::<archivo de audio>
```

- Ejemplo: `🔔##Atención::notification.wav`

El prefijo `##` y el sufijo `::` son opcionales e independientes; puedes usar cualquier combinación:
- Solo prefijo de icono: `🔔##Atención`
- Solo sufijo de audio: `Atención::notification.wav`
- Ambos: `🔔##Atención::notification.wav`
- Ninguno: `Atención`

## Configuración (F2)
Abre Configuración (`F2`) para controlar:
- **Texto predeterminado**: activar/desactivar `Insertar texto predeterminado automáticamente` y personalizar frase por defecto.
  - `Mostrar teclado táctil` — abre el teclado táctil en pantalla cuando la caja de entrada recibe el foco, posicionado cerca de la ventana principal.
- **Idioma de la interfaz**: elegir idioma de UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voz**: seleccionar voz SAPI instalada según preferencia.
- **Volumen/Velocidad**: ajustar `Volumen (10-100)` y `Velocidad (-10 a 10)`.
- `Probar voz`: probar la voz seleccionada.
- **Ventana de categorías**:
  - `Minimizar ventana de categorías automáticamente` tras seleccionar.
  - `Recordar tamaño de la ventana de categorías`.
- **Comportamiento de reproducción**:
  - `Decir la frase al seleccionarla` (auto-reproducción al hacer clic).
  - `Detener reproducción anterior` — detiene automáticamente cualquier reproducción en curso antes de iniciar una nueva frase.
  - `Aumentar volumen de SimonSays al reproducir` — sube el volumen maestro del sistema mientras habla, restaurándolo cuando termina la reproducción.
  - `Reducir temporalmente otros sonidos al reproducir` — redu)HELP" LR"HELP(ce el volumen de las demás aplicaciones mientras habla, restaurándolo cuando termina la reproducción.
- `Aceptar` guarda cambios; `Cancelar` descarta.

## Mirada / Clic por fijación (F3)
Activa frases y botones sin usar las manos manteniendo la mirada (o el cursor del ratón) sobre ellos durante un tiempo determinado. Esta función es nueva: necesita pruebas y comentarios; por favor, cuéntanos cómo funciona con tu seguidor ocular mediante la opción `Danos tu opinión` del menú de bandeja.

Configuraciones compatibles:
- **Cualquier software de control ocular que mueva el cursor del ratón** (Irisbond EasyClick, Tobii Dynavox Computer Control, modos de cursor del Control ocular de Windows, OptiKey, ...): el clic por fijación sigue al cursor.
- **Lectura directa de la mirada, sin necesidad de mover el cursor**: Irisbond Hiru (modo HID) y seguidores Tobii a través de su propio software instalado — verificado con Tobii Eye Tracker 4C y Tobii Dynavox PCEye 5; se espera que Tobii Eye Tracker 5 funcione, pero aún no se ha probado.

Abre la configuración de Mirada / Clic por fijación (`F3`) para ajustar:
- **Modo de activación**: elige entre `Automático`, que detecta la presencia y el comportamiento del dispositivo y decide si activar el clic por fijación mediante el cursor del ratón o mediante detección de mirada (si se detecta un dispositivo de seguimiento ocular o software relacionado, el clic por fijación se activa; si no se detecta ninguno, permanece desactivado), `Cursor del ratón` para activar siempre el clic por fijación mediante el cursor, `Seguidor ocular HID` para activarlo siempre mediante detección de mirada, o `Desactivado` para desactivarlo siempre, independientemente de los dispositivos presentes.
- **Tiempo de fijación**: duración (en milisegundos) necesaria para disparar un clic al mirar un elemento. Mientras la fijación está en curso se muestra un indicador de progreso.
- **Radio de tolerancia**: tolerancia de la mirada (en píxeles) para permitir pequeños movimientos del ojo sin cancelar el clic por fijación.
- **Tiempo de espera**: milisegundos tras una activación durante los cuales no se disparará otro clic por fijación (evita clics repetidos accidentales).
- **Color de progreso**: color del indicador de progreso de la fijación (por defecto, el color de énfasis de Windows).
- **Detectar cómo usas el seguidor**: dos botones que ayudan a la aplicación a detectar si mueves el cursor con un seguidor ocular o si simplemente haces clic en los botones.
- **Detectado**: muestra el tipo de dispositivo detectado e información adicional sobre el software relacionado.
- Un botón `Restablecer` (abajo a la izquierda) restaura todos los ajustes de fijación a sus valores predeterminados.
- El botón `Aceptar` guarda los cambios; `Cancelar` los descarta.

## Menú del icono de bandeja
- `Mostrar` / `Ocultar` interfaz principal.
- `Configuración`.
- `Mirada / Clic por fijación`.
- `Acerca de` muestra versión y créditos.
- `Web` abre la página del proyecto (si está configurada).
- `Danos tu opinión` abre el [formulario de comentarios](https://forms.gle/KMbpEDYmwnFJUhgy8) en el navegador predeterminado.
- `Salir` cierra SimonSays.

Puedes mover la ventana emergente de bandeja y otros diálogos arrastrando la barra de título; redimensiona la ventana de Categorías arrastrando los bordes y activa `Recordar tamaño de la ventana de categorías` en Configuración para conservar el tamaño.

## Mezclar voz y sonidos
- Usa el delimitador `♫` (`SOUND_NOTE_DELIMITER`) para insertar nombres de archivo de audio en el texto:
  - Ejemplo: `Hola ♫notification.wav♫, por favor espere.`
  - El texto fuera de delimitadores se habla; el texto dentro se trata como archivo de sonido y se reproduce en línea.
- Audio soportado: `.wav`, `.mid`, `.midi`, `.mp3`.
- Usa rutas absolutas o relativas accesibles al proceso. Empareja delimitadores para evitar interpretar texto final como audio.



### Orden de búsqueda de archivos de sonido e icono
Cuando un nombre de archivo de sonido o icono no contiene una letra de unidad o una ruta completa, SimonSays lo busca en este orden, usando la primera coincidencia:

1. **Carpeta AppData** — `%APPDATA%\SimonSays\` (específica del usuario; coloca aquí tus archivos para separarlos de la instalación).
2. **Directorio de trabajo** — el directorio de trabajo actual del proceso (solo se busca si es distinto de la carpeta del ejecutable).
3. **Carpeta del ejecutable** — la carpeta que contiene `SimonSays.exe`.

Si no se encuentra el archivo en estas ubicaciones, se usa el sonido de reserva integrado.

> **Consejo:** se recomienda colocar tus sonidos e iconos personalizados en `%APPDATA%\SimonSays\`; sobrevivirán a las actualizaciones y son específicos del usuario.

## Importar y exportar categorías
- Exportar: puedes exportar todas las categorías o solo la seleccionada cuando se te pregunte. Los mensajes de éxito/error aparecen en el idioma seleccionado (atajo: `F10`).
- Importar: elige archivo; si una categoría existe, se pedirá sobrescribir. Los mensajes de éxito/error aparecen en el idioma seleccionado (atajo: `F9`).

## Solución de problemas
- **Fallo de inicialización de voz**: verifica que SAPI esté instalado y que haya una voz compatible.
- **No se reproducen sonidos**: confirma ruta/extensión y delimitadores `♫` emparejados.
- **Errores de importación/exportación**: revisa permisos y ubicación de disco; reintenta.
- **Idioma UI no aplicado**: reabre Configuración y confirma idioma; asegúrate de que existan cadenas de localización para el idioma elegido.

## Acerca de
- El diálogo `Acerca de` muestra versión, descripción y copyright.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { TRAYICON_FEEDBACK_ID, L"Danos tu opinión" },
  { AMPERSAND_REPLACEMENT_ID, L"y" },
  { BOARD_STYLE_CHANGED_TITLE_ID, L"Estilo de tablero cambiado" },
  { BOARD_STYLE_CHANGED_MESSAGE_ID, L"¿Quieres revertir los cambios?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, L"¿Eliminar todas las categorías y sus frases?\nAdvertencia: esta acción no se puede deshacer." },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, L"¿Es esto un error?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, L"Eliminar todas las categorías" },
};

static const std::vector<std::pair<int, const wchar_t *>> ARABIC_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"تشغيل>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"الفئات" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"الإعدادات" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"النص الافتراضي:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"إدراج النص الافتراضي تلقائياً" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"اللغة:" },
  { SETTINGS_VOICE_LABEL_ID, L"الصوت:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"اختبار الصوت" },
  { SETTINGS_VOLUME_LABEL_ID, L"مستوى الصوت (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"السرعة (-10 إلى 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"نطق العبارة مباشرة عند التحديد" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"تذكر حجم نافذة الفئات" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"تصغير نافذة الفئات تلقائياً" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"زيادة مستوى صوت SimonSays أثناء التشغيل" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"خفض الأصوات الأخرى مؤقتاً أثناء التشغيل" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"إيقاف التشغيل السابق" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"إظهار لوحة المفاتيح اللمسية" },
  { SETTINGS_OK_BUTTON_ID, L"موافق" },
  { SETTINGS_CANCEL_BUTTON_ID, L"إلغاء" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - انقر للإظهار" },
  { TRAYICON_SHOW_ID, L"إظهار" },
  { TRAYICON_HIDE_ID, L"إخفاء" },
  { TRAYICON_SETTINGS_ID, L"الإعدادات" },
  { TRAYICON_ABOUT_ID, L"حول" },
  { TRAYICON_WEB_ID, L"الموقع" },
  { TRAYICON_EXIT_ID, L"خروج" },
  { ABOUT_TITTLE_ID, L"حول SimonSays" },
  { ABOUT_1_ID, L"SimonSays - فقط تحدث\n\nالإصدار " },
  { ABOUT_2_ID, L"\n\nأداة وصول بسيطة لتحويل النص إلى كلام فوراً.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"خطأ في SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"يدعم SimonSays حالياً شريط المهام السفلي فقط" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"تغيير اسم الفئة" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"تحرير العبارة" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"الحالي : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"موافق" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"إلغاء" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"يوجد بالفعل فئة بهذا الاسم. الرجاء اختيار اسم مختلف." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"تعارض أسماء الفئات" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"حذف الفئة" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"هل تريد حذف الفئة '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' وجميع عباراتك؟" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"حذف العبارة" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"هل تريد حذف العبارة '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'؟" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"إضافة فئة جديدة" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"إضافة عبارة جديدة" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"اسم الفئة :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"نص العبارة :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"موافق" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"إلغاء" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"عبارة افتراضية جديدة" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"استيراد الفئات" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"الكتابة فوق الفئة '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'؟" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"الكتابة فوق الفئة؟" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"استبدال نمط اللوحة؟" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"يتضمن الملف المستورد نمط لوحة. هل تريد استبدال النمط الحالي؟" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"تم استيراد العبارات بنجاح." },
  { IMPORT_SUCCESS_TITLE_ID, L"تم الاستيراد بنجاح" },
  { IMPORT_FAILURE_MESSAGE_ID, L"فشل استيراد العبارات." },
  { IMPORT_FAILURE_TITLE_ID, L"فشل الاستيراد" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"تصدير الفئات" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"يمكنك تصدير كل الفئات أو الفئة المحددة فقط.\nهل تريد تصدير الفئة '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' فقط؟" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"تصدير التحديد" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"تم تصدير العبارات بنجاح." },
  { EXPORT_SUCCESS_TITLE_ID, L"تم التصدير بنجاح" },
  { EXPORT_FAILURE_MESSAGE_ID, L"فشل تصدير العبارات." },
  { EXPORT_FAILURE_TITLE_ID, L"فشل التصدير" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: المساعدة - F2: الإعدادات" },
  { HELP_CONTENT_ID, LR"HELP(# دليل مساعدة SimonSays

## نظرة عامة
SimonSays هو مساعد تحويل النص إلى كلام (TTS) لنظام Windows، أنشأه Juan Rey Saura، ومُحسّن للتحدث السريع ومكتبات العبارات وتشغيل الأصوات داخل النص. يدعم لغات متعددة وأصوات قابلة للتخصيص وميزات سهلة الاستخدام للتواصل الفعّال.

يستخدم SimonSays المساحة الفارغة على يسار شريط مهام Windows، مما يوفر وصولاً سريعاً إلى عشرات الفئات من العبارات المستخدمة بشكل متكرر (والقابلة للتخصيص) بنقرتين فقط.

## الاختصارات
- `F1`: فتح المساعدة.
- `F2`: فتح الإعدادات.
- `F3`: إعدادات النظر / النقر بالتثبيت.
- `F4`: تعديل التحديد الحالي.
- `F5` / `F6`: نقل إلى التحديد السابق / التالي في القوائم.
- `F7`: إضافة بعد التحديد الحالي (سياق فئة/عبارة).
- `F8`: حذف التحديد الحالي.
- `F9`: استيراد الفئات.
- `F10`: تصدير الفئات.
- `F11` / `Ctrl -`: تصغير (نافذة الفئات).
- `F12` / `Ctrl +`: تكبير (نافذة الفئات).
- `Ctrl 0`: إعادة تعيين التكبير.
- زر `تشغيل>`: بدء التشغيل.

## البدء والاستخدام الأساسي
1. شغّل SimonSays؛ يظهر في منطقة الإعلام بشريط مهام Windows.
2. للتحدث، يمكنك إما:
   - كتابة نص مخصص في مربع الإدخال والضغط على زر `تشغيل>` (أو `Enter`).
   - النقر على عبارة أو أكثر محفوظة في نافذة الفئات.
3. أثناء التشغيل، يعرض الزر `>` للإشارة إلى التشغيل النشط. يمكنك الضغط عليه مرة أخرى للإيقاف فوراً.

## نافذة الفئات (العبارات)
- افتحها عبر زر `الفئات`. يمكنك تغيير حجمها أو تذكر حجمها من الإعدادات.
- حدد فئة في الأعلى وعبارات في الأسفل.
- النقر مرة واحدة على عبارة ينطقها تلقائياً إذا كان `نطق العبارة مباشرة عند التحديد` مفعّلاً؛ وإلا تُحمّل العبارة في مربع الإدخال الرئيسي دون نطق.

### إدارة الفئات
- إعادة تسمية: حدد فئة → (اختصار: `F4`) `تغيير اسم الفئة` → غيّر الاسم → `موافق`.
- نقل التحديد: `F5`/`F6` للتنقل إلى العنصر السابق/التالي.
- إضافة: حدد فئة → (اختصار: `F7`) حوار `إضافة فئة جديدة` → أدخل الاسم → `موافق`.
- حذف: حدد فئة → (اختصار: `F8`) `حذف الفئة`؛ أكّد حذف الفئة وعباراتها.



#### تعيين أو تغيير أيقونة الفئة
يمكن لكل فئة عرض رمز تعبيري أو ملف `.ico` مخصص على زرها. يتم تحديد الأيقونة في مربع حوار الإضافة/التحرير باستخدام البادئة الفاصلة `##`:

```
<icon>##<category name>
```

- **رمز تعبيري**: أدخل الرمز التعبيري متبوعاً بـ `##` ثم الاسم.
  - مثال: `🙂##التحيات`
- **ملف أيقونة** (`.ico`): أدخل مسار الملف (مطلقاً أو نسبياً إلى مجلد SimonSays) متبوعاً بـ `##` ثم الاسم.
  - مثال: `C:\icons\smile.ico##التحيات`
- **بدون أيقونة**: أدخل اسم الفئة فقط من دون البادئة `##`.
  - مثال: `التحيات`

> ملاحظة: يتم دعم ملفات `.ico` فقط للأيقونات المعتمدة على الملفات. أي امتداد آخر يتم تجاهله ولن تظهر أيقونة.

### إدارة العبارات
- تعديل: حدد عبارة → (اختصار: `F4`) `تحرير العبارة` → حدّث النص → `موافق`.
- نقل التحديد: `F5`/`F6` للتنقل إلى العبارة السابقة/التالية.
- إضافة: حدد عبارة → (اختصار: `F7`) `إضافة عبارة جديدة` → أدخل النص → `موافق`.
- حذف: حدد عبارة → (اختصار: `F8`) `حذف العبارة`؛ أكّد.



#### تعيين أو تغيير أيقونة العبارة
تتبع العبارات نفس نمط بادئة الأيقونة المستخدم مع الفئات، مع وجود الفاصل الاختياري `##` قبل نص العبارة:

```
<icon>##<phrase text>
```

- **رمز تعبيري**: `👍##ممتاز!`
- **ملف أيقونة**: `C:\icons\check.ico##ممتاز!`
- **بدون أيقونة**: `ممتاز!`

يمكن للعبارات أيضاً أن تتضمن ملفاً صوتياً مضمنًا. عند الجمع بين الأيقونة والنص والملف الصوتي، يكون التنسيق الكامل كالتالي:

```
<icon>##<phrase text>::<audio file>
```

- مثال: `🔔##انتباه::notification.wav`

البادئة `##` واللاحقة `::` اختياريتان ومستقلتان؛ ويمكنك استخدام أي تركيبة:
- بادئة أيقونة فقط: `🔔##انتباه`
- لاحقة صوت فقط: `انتباه::notification.wav`
- الاثنان معاً: `🔔##انتباه::notification.wav`
- لا شيء: `انتباه`

## الإعدادات (F2)
افتح الإعدادات (`F2`) للتحكم في:
- **النص الافتراضي**: تفعيل/تعطيل `إدراج النص الافتراضي تلقائياً` وتخصيص العبارة الافتراضية.
  - `إظهار لوحة المفاتيح اللمسية` — يفتح لوحة المفاتيح اللمسية على الشاشة عندما يتلقى مربع الإدخال التركيز، بالقرب من النافذة الرئيسية.
- **لغة الواجهة**: اختيار لغة واجهة المستخدم (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **الصوت**: اختيار صوت SAPI مثبّت حسب تفضيلك.
- **مستوى الصوت/السرعة**: ضبط `مستوى الصوت (10-100)` و`السرعة (-10 إلى 10)`.
- `اختبار الصوت`: تجربة الصوت المحدد.
- **نافذة الفئات**:
  - `تصغير نافذة الفئات تلقائياً` بعد التحديد.
  - `تذكر حجم نافذة الفئات`.
- **سلوك التشغيل**:
  - `نطق العبارة مباشرة عند التحديد` (نطق تلقائي بالنقر).
  - `إيقاف التشغيل السابق` — يوقف تلقائياً أي تشغيل جارٍ قبل بدء عبارة جديدة.
  - `زيادة مستوى صوت SimonSays أثناء التشغيل` — يرفع مستوى الصوت الرئيسي للنظام أثناء التحدث، ويستعيده)HELP" LR"HELP( عند انتهاء التشغيل.
  - `خفض الأصوات الأخرى مؤقتاً أثناء التشغيل` — يخفض صوت التطبيقات الأخرى أثناء التحدث، ويستعيده عند انتهاء التشغيل.
- `موافق` يحفظ التغييرات؛ `إلغاء` يتجاهلها.

## النظر / النقر بالتثبيت (F3)
فعّل العبارات والأزرار دون استخدام اليدين بتثبيت نظرك (أو مؤشر الفأرة) عليها لمدة محددة. هذه الميزة جديدة — تحتاج إلى اختبار وملاحظات: أخبرنا كيف تعمل مع جهاز تتبع العين لديك عبر خيار `إرسال ملاحظات` في قائمة أيقونة الإعلام.

الإعدادات المدعومة:
- **أي برنامج تحكم بالعين يحرّك مؤشر الفأرة** (Irisbond EasyClick، Tobii Dynavox Computer Control، أوضاع المؤشر في التحكم بالعين في Windows، OptiKey، ...): يتبع النقر بالتثبيت المؤشر.
- **قراءة النظر مباشرة، دون تحريك المؤشر**: Irisbond Hiru (وضع HID) وأجهزة Tobii عبر برنامجها المثبّت — تم التحقق مع Tobii Eye Tracker 4C وTobii Dynavox PCEye 5؛ من المتوقع أن يعمل Tobii Eye Tracker 5 لكنه لم يُختبر بعد.

افتح إعدادات النظر / النقر بالتثبيت (`F3`) للضبط (تظهر عناصر هذا الحوار باللغة الإنجليزية):
- **وضع التفعيل**: اختر بين `Automatic` الذي يكتشف وجود الجهاز وسلوكه ويقرر تفعيل النقر بالتثبيت عبر مؤشر الفأرة أو عبر قراءة النظر (إذا اكتُشف جهاز تتبع عين أو برنامج مرتبط، يُفعَّل النقر بالتثبيت؛ وإذا لم يُكتشف شيء، يبقى معطلاً)، أو `Mouse cursor` لتفعيل النقر بالتثبيت دائماً عبر المؤشر، أو `HID eye tracker` لتفعيله دائماً عبر قراءة النظر، أو `Off` لتعطيله دائماً بغض النظر عن الأجهزة الموجودة.
- **مدة التثبيت** (`Dwell time`): المدة (بالميلي ثانية) اللازمة لإطلاق نقرة عند النظر إلى عنصر. يظهر مؤشر تقدم أثناء التثبيت.
- **نصف قطر التسامح** (`Tolerance radius`): تسامح النظر (بالبكسل) للسماح بحركات عين طفيفة دون إلغاء النقر بالتثبيت.
- **فترة الانتظار** (`Cooldown`): الميلي ثانية بعد التفعيل التي لن تُطلق خلالها نقرة تثبيت أخرى (يمنع النقرات المتكررة غير المقصودة).
- **لون التقدم** (`Progress color`): لون مؤشر تقدم التثبيت (افتراضياً لون التمييز في Windows).
- **اكتشاف كيفية استخدامك للجهاز** (`Detect how you use the tracker`): زران يساعدان التطبيق على اكتشاف ما إذا كنت تحرّك المؤشر بجهاز تتبع العين أو تنقر على الأزرار فحسب.
- **المكتشَف** (`Detected`): يعرض نوع الجهاز المكتشَف ومعلومات إضافية عن البرامج المرتبطة.
- زر `Reset` (أسفل اليسار) يعيد جميع إعدادات التثبيت إلى قيمها الافتراضية.
- زر `موافق` يحفظ التغييرات؛ `إلغاء` يتجاهلها.

## قائمة أيقونة الإعلام
- `إظهار` / `إخفاء` الواجهة الرئيسية.
- `الإعدادات`.
- `Gaze / Dwell-click` (النظر / النقر بالتثبيت).
- `حول` يعرض الإصدار والاعتمادات.
- `الموقع` يفتح صفحة المشروع (إذا كانت مُعدّة).
- `إرسال ملاحظات` يفتح [نموذج ملاحظات](https://forms.gle/KMbpEDYmwnFJUhgy8) في المتصفح الافتراضي.
- `خروج` يغلق SimonSays.

يمكنك نقل نافذة الإعلام المنبثقة والحوارات الأخرى بسحب شريط العنوان؛ غيّر حجم نافذة الفئات بسحب حوافها، وفعّل `تذكر حجم نافذة الفئات` في الإعدادات للاحتفاظ بالحجم.

## مزج الكلام والأصوات
- استخدم المحدد `♫` (`SOUND_NOTE_DELIMITER`) لإدراج أسماء ملفات صوت في النص:
  - مثال: `مرحباً ♫notification.wav♫، يرجى الانتظار.`
  - النص خارج المحددات يُنطق؛ النص بينها يُعامل كملف صوت ويُشغّل في السياق.
- الصيغ المدعومة: `.wav`، `.mid`، `.midi`، `.mp3`.
- استخدم مسارات مطلقة أو نسبية يمكن للعملية الوصول إليها. أزوج المحددات لتجنب معاملة النص المتبقي كصوت.



### ترتيب البحث عن ملفات الصوت والأيقونات
عندما لا يحتوي اسم ملف الصوت أو الأيقونة على حرف محرك أقراص أو مسار كامل، يبحث SimonSays عنه بالترتيب التالي ويستخدم أول تطابق يجده:

1. **مجلد AppData** — `%APPDATA%\SimonSays\` (خاص بالمستخدم؛ ضع ملفاتك هنا لإبقائها منفصلة عن تثبيت التطبيق).
2. **مجلد العمل** — مجلد العمل الحالي للعملية (يتم البحث فيه فقط إذا كان مختلفاً عن مجلد الملف التنفيذي).
3. **مجلد الملف التنفيذي** — المجلد الذي يحتوي على `SimonSays.exe`.

إذا لم يتم العثور على الملف في أي من هذه المواقع، فسيتم استخدام صوت الطوارئ المدمج بدلاً من ذلك.

> **نصيحة:** يوصى بوضع ملفات الصوت والأيقونات المخصصة في `%APPDATA%\SimonSays\`؛ فهي ستبقى بعد تحديثات التطبيق وستكون خاصة بالمستخدم.

## استيراد وتصدير الفئات
- تصدير: يمكنك تصدير كل الفئات أو المحددة فقط عند السؤال. تظهر رسائل النجاح/الفشل بلغة الواجهة (اختصار: `F10`).
- استيراد: اختر ملفاً؛ إذا كانت الفئة موجودة، سيُطلب منك الكتابة فوقها. تظهر رسائل النجاح/الفشل بلغة الواجهة (اختصار: `F9`).

## استكشاف الأخطاء وإصلاحها
- **فشل تهيئة الصوت**: تحقق من تثبيت SAPI ووجود صوت متوافق.
- **لا يوجد تشغيل صوت**: تأكد من المسار/الامتداد والمحددات `♫` المزدوجة.
- **أخطاء الاستيراد/التصدير**: تحقق من الأذونات وموقع القرص؛ أعد المحاولة.
- **لغة الواجهة غير مطبقة**: أعد فتح الإعدادات وتأكد من اختيار اللغة؛ تأكد من وجود سلاسل الترجمة للغة المختارة.

## حول
- حوار `حول` يعرض الإصدار والوصف وحقوق النشر.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { TRAYICON_FEEDBACK_ID, L"إرسال ملاحظات" },
  { AMPERSAND_REPLACEMENT_ID, L"و" },
};

static const std::vector<std::pair<int, const wchar_t *>> BASQUE_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Erreproduzitu>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Kategoriak" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Ezarpenak" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Testu lehenetsia:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Txertatu testu lehenetsia automatikoki" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Hizkuntza:" },
  { SETTINGS_VOICE_LABEL_ID, L"Ahotsa:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Ahotsa probatu" },
  { SETTINGS_VOLUME_LABEL_ID, L"Bolumena (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Abiadura (-10etik 10era):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Hautatzean esaldia berehala esan" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Gogoratu kategoriaren leihoaren tamaina" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizatu kategoriaren leihoa automatikoki" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Handitu SimonSays bolumena erreproduzitzean" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Murriztu aldi baterako beste audioak erreproduzitzean" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Aurreko erreprodukzioa gelditu" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Ukipen teklatua erakutsi" },
  { SETTINGS_OK_BUTTON_ID, L"Ados" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Utzi" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Egin klik erakusteko" },
  { TRAYICON_SHOW_ID, L"Erakutsi" },
  { TRAYICON_HIDE_ID, L"Ezkutatu" },
  { TRAYICON_SETTINGS_ID, L"Ezarpenak" },
  { TRAYICON_ABOUT_ID, L"Honi buruz" },
  { TRAYICON_WEB_ID, L"Webgunea" },
  { TRAYICON_EXIT_ID, L"Irten" },
  { ABOUT_TITTLE_ID, L"SimonSays-ri buruz" },
  { ABOUT_1_ID, L"SimonSays - Hitz egin besterik ez\n\nBertsioa " },
  { ABOUT_2_ID, L"\n\nTestua berehala ahots bihurtzen duen\nirisgarritasun tresna sinplea.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays errorea" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays-ek unean behetik dagoen ataza-barra soilik onartzen du" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Kategoriaren izena aldatu" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Esaldia editatu" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Orain : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"Ados" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Utzi" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Izen honekin kategoria bat dagoeneko existitzen da. Mesedez, aukeratu izen desberdin bat." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Kategoriaren izen gatazka" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Kategoria ezabatu" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Kategoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' eta bere esaldi guztiak ezabatu?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Esaldia ezabatu" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Esaldia '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' ezabatu?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Kategoria berria gehitu" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Esaldi berria gehitu" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Kategoriaren izena :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Esaldiaren testua :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"Ados" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Utzi" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Esaldi lehenetsi berria" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Kategoriak inportatu" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Gainidatzi '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' kategoria?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Gainidatzi kategoria?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Taularen estiloa ordeztu?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"Inportatutako fitxategiak taula-estilo bat dauka. Oraingoa ordeztu nahi duzu?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Esaldiak ongi inportatu dira." },
  { IMPORT_SUCCESS_TITLE_ID, L"Inportazio arrakastatsua" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Esaldien inportazioak huts egin du." },
  { IMPORT_FAILURE_TITLE_ID, L"Inportazioak huts egin du" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Kategoriak esportatu" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Kategoria guztiak edo hautatutakoa soilik esporta ditzakezu.\nHautatutako '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' kategoria soilik esportatu nahi duzu?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Esportatu hautapena" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Esaldiak ongi esportatu dira." },
  { EXPORT_SUCCESS_TITLE_ID, L"Esportazio arrakastatsua" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Esaldien esportazioak huts egin du." },
  { EXPORT_FAILURE_TITLE_ID, L"Esportazioak huts egin du" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Laguntza - F2: Ezarpenak" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays Laguntza Gida

## Ikuspegi orokorra
SimonSays Windows-eko testutik hizketara (TTS) laguntzailea da, Juan Rey Saurak sortua, hizketa azkarra, esaldi-liburutegiak eta lerro barruko soinuen erreprodukziorako optimizatua. Hizkuntza anitz, ahots konfiguragarriak eta erabiltzaileentzako funtzio eraginkorrak onartzen ditu.

SimonSays-ek Windows ataza-barraren ezkerreko eremu librea erabiltzen du, hamaika kategoriako maiz erabiltzen diren (eta pertsonalizagarriak diren) esaldietara sarbide azkarra emanez klik pare batekin.

## Laster-teklak
- `F1`: Laguntza ireki.
- `F2`: Ezarpenak ireki.
- `F3`: Begirada / Klik finkoaren ezarpenak.
- `F4`: Uneko hautapena editatu.
- `F5` / `F6`: Aurreko / hurrengo hautapenera mugitu zerrendetan.
- `F7`: Uneko hautapenaren ondoren gehitu (kategoria/esaldi testuingurua).
- `F8`: Uneko hautapena ezabatu.
- `F9`: Kategoriak inportatu.
- `F10`: Kategoriak esportatu.
- `F11` / `Ctrl -`: Zooma txikitu (kategorien leihoa).
- `F12` / `Ctrl +`: Zooma handitu (kategorien leihoa).
- `Ctrl 0`: Zooma berrezarri.
- `Erreproduzitu>` botoia: Erreprodukzioa hasi.

## Abiaraztea eta oinarrizko erabilera
1. Abiarazi SimonSays; Windows-eko ataza-barrako erretiluan agertzen da.
2. Hitz egiteko, honako hauek egin ditzakezu:
   - Testu pertsonalizatua idatzi sarrera-koadroan eta `Erreproduzitu>` botoia sakatu (edo `Enter`).
   - Gordetako esaldi bat edo gehiago klikatu Kategorien leihoan.
3. Erreprodukzioan, botoiak `>` erakusten du erreprodukzio aktiboa adierazteko. Berriro saka dezakezu berehala gelditzeko.

## Kategorien leihoa (esaldiak)
- Ireki `Kategoriak` botoiaren bidez. Tamaina aldatu edo gogoratu Ezarpenetan.
- Hautatu kategoria bat goian eta esaldiak behean.
- Esaldi batean klik bakarrak berehala hitz egiten du `Hautatzean esaldia berehala esan` aktibatuta badago; bestela, esaldia sarrera-koadro nagusian kargatzen da hitz egin gabe.

### Kategoriak kudeatu
- Berrizendatu: hautatu kategoria → (laster-tekla: `F4`) `Kategoriaren izena aldatu` → aldatu izena → `Ados`.
- Hautapena mugitu: `F5`/`F6` aurreko/hurrengo elementura mugitzeko.
- Gehitu: hautatu kategoria → (laster-tekla: `F7`) `Kategoria berria gehitu` elkarrizketa → idatzi izena → `Ados`.
- Ezabatu: hautatu kategoria → (laster-tekla: `F8`) `Kategoria ezabatu`; berretsi kategoria eta esaldien ezabaketa.



#### Kategoriaren ikonoa ezarri edo aldatu
Kategoria bakoitzak emoji bat edo `.ico` fitxategi pertsonalizatu bat erakuts dezake bere botoian. Ikonoa Gehitu/Editatu elkarrizketa-koadroan zehazten da `##` bereizle-aurrizkia erabiliz:

```
<ikonoa>##<kategoriaren izena>
```

- **Emoji-a**: idatzi emojia, ondoren `##` eta izena.
  - Adibidea: `🙂##Agurrak`
- **Ikono-fitxategia** (`.ico`): idatzi fitxategiaren bidea (absolutua edo SimonSays karpetarekiko erlatiboa), ondoren `##` eta izena.
  - Adibidea: `C:\icons\smile.ico##Agurrak`
- **Ikonorik gabe**: idatzi kategoriaren izena soilik, `##` aurrizkirik gabe.
  - Adibidea: `Agurrak`

> Oharra: fitxategietan oinarritutako ikonoetarako `.ico` fitxategiak soilik onartzen dira. Beste edozein luzapen baztertu egingo da eta ez da ikonorik erakutsiko.

### Esaldiak kudeatu
- Editatu: hautatu esaldia → (laster-tekla: `F4`) `Esaldia editatu` → eguneratu testua → `Ados`.
- Hautapena mugitu: `F5`/`F6` aurreko/hurrengo esaldira mugitzeko.
- Gehitu: hautatu esaldia → (laster-tekla: `F7`) `Esaldi berria gehitu` → idatzi testua → `Ados`.
- Ezabatu: hautatu esaldia → (laster-tekla: `F8`) `Esaldia ezabatu`; berretsi.



#### Esaldiaren ikonoa ezarri edo aldatu
Esaldiek kategorien ikono-aurrizki arau bera jarraitzen dute, eta aukerako `##` bereizlea esaldiaren testuaren aurretik jartzen da:

```
<ikonoa>##<esaldiaren testua>
```

- **Emoji-a**: `👍##Ondo dago!`
- **Ikono-fitxategia**: `C:\icons\check.ico##Ondo dago!`
- **Ikonorik gabe**: `Ondo dago!`

Esaldiek lerro barruko audio-fitxategi bat ere izan dezakete. Ikonoa, testua eta audio-fitxategia konbinatzean, formatu osoa hau da:

```
<ikonoa>##<esaldiaren testua>::<audio-fitxategia>
```

- Adibidea: `🔔##Adi::notification.wav`

`##` aurrizkia eta `::` atzizkia aukerakoak eta independenteak dira:
- Ikono-aurrizkia soilik: `🔔##Adi`
- Audio-atzizkia soilik: `Adi::notification.wav`
- Biak: `🔔##Adi::notification.wav`
- Bat ere ez: `Adi`

## Ezarpenak (F2)
Ireki Ezarpenak (`F2`) honako hauek kontrolatzeko:
- **Testu lehenetsia**: gaitu/desgaitu `Txertatu testu lehenetsia automatikoki` eta pertsonalizatu esaldi lehenetsia.
  - `Ukipen teklatua erakutsi` — pantailako ukipen teklatua irekitzen du sarrera-koadroak fokua jasotzen duenean, leiho nagusitik gertu kokatuta.
- **Interfaze-hizkuntza**: UI hizkuntza aukeratu (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Ahotsa**: instalatutako SAPI ahotsa hautatu zure hobespenen arabera.
- **Bolumena/Abiadura**: doitu `Bolumena (10-100)` eta `Abiadura (-10etik 10era)`.
- `Ahotsa probatu`: hautatutako ahotsa probatu.
- **Kategoriaren leihoa**:
  - `Minimizatu kategoriaren leihoa automatikoki` hautatu ondoren.
  - `Gogoratu kategoriaren leihoaren tamaina`.
- **Erreprodukzio-portaera**:
  - `Hautatzean esaldia berehala esan` (klik bidezko auto-erreprodukzioa).
  - `Aurreko erreprodukzioa gelditu` — automatikoki gelditzen du abian dagoen edozein erreprodukzio esaldi berri bat hasi aurretik.
  - `Handitu SimonSays bolumena erreproduzitzean` — sistemaren bolumen nagusia igotzen du hitz egiten den bitartean, erreprodukzioa amaitzean leheneratuz.
  - `Murriztu aldi baterako beste audioak erreproduzitzean` — beste aplikazioen bolumena murrizten du hitz egiten den bitartean, erreprodukzioa amaitzean leheneratuz.
- `Ados` aldaketak gordetz)HELP" LR"HELP(en ditu; `Utzi` baztertzen ditu.

## Begirada / Klik finkoa (F3)
Aktibatu esaldiak eta botoiak eskurik gabe, begirada (edo saguaren kurtsorea) haien gainean denbora jakin batez mantenduz. Funtzio hau berria da: probak eta iritziak behar ditu; mesedez, kontaiguzu nola dabilen zure begi-jarraitzailearekin erretiluko menuko `Iritzia` aukeraren bidez.

Konfigurazio bateragarriak:
- **Saguaren kurtsorea mugitzen duen edozein begi-kontrolerako software** (Irisbond EasyClick, Tobii Dynavox Computer Control, Windows-en Begi-kontrolaren kurtsore-moduak, OptiKey, ...): klik finkoak kurtsoreari jarraitzen dio.
- **Begiradaren irakurketa zuzena, kurtsorea mugitu beharrik gabe**: Irisbond Hiru (HID modua) eta Tobii jarraitzaileak beren software instalatuaren bidez — Tobii Eye Tracker 4C eta Tobii Dynavox PCEye 5 gailuekin egiaztatua; Tobii Eye Tracker 5 dabilela espero da, baina oraindik ez da probatu.

Ireki Begirada / Klik finkoaren ezarpenak (`F3`) doitzeko (elkarrizketa honetako kontrolak ingelesez agertzen dira):
- **Aktibazio modua**: aukeratu `Automatic` (gailuaren presentzia eta portaera detektatzen ditu, eta klik finkoa saguaren kurtsorearen bidez ala begirada-detekzioaren bidez aktibatu erabakitzen du; begi-jarraitzaile gailu bat edo lotutako softwarea detektatzen bada, klik finkoa aktibatzen da; bat ere ez bada detektatzen, desaktibatuta jarraitzen du), `Mouse cursor` klik finkoa beti kurtsorearen bidez aktibatzeko, `HID eye tracker` beti begirada-detekzioaren bidez aktibatzeko, edo `Off` beti desaktibatzeko, gailuak egon ala ez.
- **Finkatze-denbora** (`Dwell time`): elementu bati begiratzean klik bat abiarazteko behar den iraupena (milisegundotan). Finkatzea abian den bitartean aurrerapen-adierazle bat erakusten da.
- **Tolerantzia-erradioa** (`Tolerance radius`): begiradaren tolerantzia (pixeletan), begiaren mugimendu txikiak onartzeko klik finkoa bertan behera utzi gabe.
- **Itxarote-denbora** (`Cooldown`): aktibazio baten ondorengo milisegundoak, zeinetan ez den beste klik finkorik abiaraziko (nahi gabeko klik errepikatuak saihesten ditu).
- **Aurrerapen-kolorea** (`Progress color`): finkatzearen aurrerapen-adierazlearen kolorea (lehenespenez, Windows-en azentu-kolorea).
- **Detektatu jarraitzailea nola erabiltzen duzun** (`Detect how you use the tracker`): bi botoi, aplikazioari detektatzen laguntzeko kurtsorea begi-jarraitzaile batekin mugitzen duzun ala botoietan klik egiten duzun besterik gabe.
- **Detektatua** (`Detected`): detektatutako gailu mota eta lotutako softwareari buruzko informazio gehigarria erakusten du.
- `Reset` botoi batek (behean ezkerrean) finkatze-ezarpen guztiak balio lehenetsietara itzultzen ditu.
- `Ados` botoiak aldaketak gordetzen ditu; `Utzi`-k baztertzen ditu.

## Erretiluko ikonoaren menua
- `Erakutsi` / `Ezkutatu` interfaze nagusia.
- `Ezarpenak`.
- `Gaze / Dwell-click` (Begirada / Klik finkoa).
- `Honi buruz` bertsioa eta kredituak erakusten ditu.
- `Webgunea` proiektuaren orria irekitzen du (konfiguratuta badago).
- `Iritzia` [iritzi inprimakia](https://forms.gle/KMbpEDYmwnFJUhgy8) irekitzen du arakatzaile lehenetsian.
- `Irten` SimonSays ixten du.

Erretiluko leiho gainerakorra eta beste elkarrizketak mugitu ditzakezu titulu-barra arrastatuz; Kategorien leihoaren tamaina aldatu ertzak arrastatuz, eta gaitu `Gogoratu kategoriaren leihoaren tamaina` Ezarpenetan tamaina gordetzeko.

## Hizketa eta soinuak nahastu
- Erabili `♫` (`SOUND_NOTE_DELIMITER`) mugatzailea testuan audio fitxategien izenak txertatzeko:
  - Adibidea: `Kaixo ♫notification.wav♫, itxaron mesedez.`
  - Mugatzaileen kanpoko testua hitz egiten da; barrualdekoa soinu-fitxategi gisa tratatzen da eta lerro barruan erreproduzitzen da.
- Onartutako audioa: `.wav`, `.mid`, `.midi`, `.mp3`.
- Erabili bide absolutuak edo erlatiboak prozesuarentzat eskuragarriak. Parekatu mugatzaileak amaierako testua audio gisa tratatzeari eusteko.



### Soinu- eta ikono-fitxategien bilaketa-ordena
Soinu- edo ikono-fitxategi izen batek ez badu unitate-letrarik edo bide osorik, SimonSays-ek honako ordenan bilatzen du eta aurkitutako lehen bat-etortzea erabiltzen du:

1. **AppData karpeta** — `%APPDATA%\SimonSays\` (erabiltzailearen berariazkoa; jarri fitxategiak hemen aplikazioaren instalaziotik aparte edukitzeko).
2. **Lan-direktorioa** — prozesuaren uneko lan-direktorioa (exekutagarriaren karpetatik desberdina bada bakarrik bilatzen da).
3. **Exekutagarriaren karpeta** — `SimonSays.exe` duen karpeta.

Fitxategia kokapen hauetan aurkitzen ez bada, integratutako ordezko soinua erabiliko da.

> **Aholkua:** gomendagarria da zure soinu- eta ikono-fitxategi pertsonalizatuak `%APPDATA%\SimonSays\` karpetan jartzea; eguneratzeen ondoren mantenduko dira eta erabiltzailearenak izango dira.

## Kategoriak inportatu eta esportatu
- Esportatu: kategoria guztiak edo hautatutakoa bakarrik esporta ditzakezu galdetzen zaizunean. Arrakasta/hutsegiteen mezuak hautatutako hizkuntzan agertzen dira (laster-tekla: `F10`).
- Inportatu: aukeratu fitxategi bat; kategoria existitzen bada, gainidaztea eskatuko zaizu. Arrakasta/hutsegiteen mezuak hautatutako hizkuntzan agertzen dira (laster-tekla: `F9`).

## Arazoen konponketa
- **Ahotsaren hasieratzeak huts egin du**: egiaztatu SAPI instalatuta dagoela eta ahots bateragarria dagoela.
- **Soinurik ez da erreproduzitzen**: berretsi fitxategiaren bidea/luzapena eta `♫` mugatzaile bikoitzak.
- **Inportazio/esportazio erroreak**: egiaztatu baimenak eta diskoaren kokalekua; saiatu berriro.
- **UI hizkuntza ez da aplikatu**: berrireki Ezarpenak eta berretsi hizkuntza-hautaketa; ziurtatu lokalizazio-kateak existitzen direla hautatutako hizkuntzarako.

## Honi buruz
- `Honi buruz` elkarrizketa-koadroak bertsioa, deskribapena eta copyright-a erakusten ditu.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { TRAYICON_FEEDBACK_ID, L"Feedback" },
  { AMPERSAND_REPLACEMENT_ID, L"eta" },
};

static const std::vector<std::pair<int, const wchar_t *>> CATALAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Reproduir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuració" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Text predeterminat:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insereix el text predeterminat automàticament" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Veu:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Prova la veu" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volum (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocitat (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Parla la frase immediatament en seleccionar-la" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Recorda la mida de la finestra de categories" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimitza automàticament la finestra de categories" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Augmenta el volum de SimonSays en reproduir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Redueix temporalment altres sons en reproduir" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Atura la reproducció anterior" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Mostra el teclat tàctil" },
  { SETTINGS_OK_BUTTON_ID, L"D'acord" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Feu clic per mostrar" },
  { TRAYICON_SHOW_ID, L"Mostra" },
  { TRAYICON_HIDE_ID, L"Amaga" },
  { TRAYICON_SETTINGS_ID, L"Configuració" },
  { TRAYICON_ABOUT_ID, L"Quant a" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Sortir" },
  { ABOUT_TITTLE_ID, L"Quant a SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplement parla\n\nVersió " },
  { ABOUT_2_ID, L"\n\nUna eina d'accessibilitat senzilla que\net permet convertir text en veu a l'instant.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Error de SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays només admet la barra de tasques inferior actualment" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Canvia el nom de la categoria" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Edita la frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"D'acord" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Ja existeix una categoria amb este nom. Si us plau, tria un nom diferent." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflicte de noms de categoria" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Eliminar categoria" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Voleu eliminar la categoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' i totes les seues frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Eliminar frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Voleu eliminar la frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Afegeix una nova categoria" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Afegeix una nova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nom de la categoria :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Text de la frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"D'acord" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nova frase predeterminada" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categories" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categories de SimonSays\0*.ssc;*.ssz\0Tots els fitxers\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sobreescriure la categoria '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sobreescriure categoria?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Substituir l'estil del tauler?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"El fitxer importat inclou un estil de tauler. Voleu substituir l'actual?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importades correctament." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importació correcta" },
  { IMPORT_FAILURE_MESSAGE_ID, L"No s'han pogut importar les frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Error d'importació" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categories" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categories de SimonSays\0*.ssc;*.ssz\0Tots els fitxers\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Pots exportar totes les categories o només la seleccionada.\nVols exportar només la categoria '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar selecció" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportades correctament." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportació correcta" },
  { EXPORT_FAILURE_MESSAGE_ID, L"No s'han pogut exportar les frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Error d'exportació" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Ajuda - F2: Configuració" },
  { HELP_CONTENT_ID, LR"HELP(# Guia d'Ajuda de SimonSays

## Visió general
SimonSays és un assistent de text a veu (TTS) per a Windows, creat per Juan Rey Saura, optimitzat per a parla ràpida, biblioteques de frases i reproducció de sons en línia. Suporta múltiples idiomes, veus personalitzables i funcions pensades per a una comunicació eficient.

SimonSays utilitza l'espai lliure a l'esquerra de la barra de tasques de Windows, proporcionant accés ràpid a una dotzena de categories de frases d'ús freqüent (i personalitzables) amb un parell de clics.

## Dreceres de teclat
- `F1`: Obrir Ajuda.
- `F2`: Obrir Configuració.
- `F3`: Configuració de Mirada / Clic per fixació.
- `F4`: Editar la selecció actual.
- `F5` / `F6`: Moure a la selecció anterior / següent en llistes.
- `F7`: Afegir després de la selecció actual (context categoria/frase).
- `F8`: Eliminar la selecció actual.
- `F9`: Importar categories.
- `F10`: Exportar categories.
- `F11` / `Ctrl -`: Allunyar el zoom (finestra de categories).
- `F12` / `Ctrl +`: Apropar el zoom (finestra de categories).
- `Ctrl 0`: Restablir el zoom.
- Botó `Reproduir>`: Iniciar reproducció.

## Inici i ús bàsic
1. Inicia SimonSays; apareix a la safata del sistema de Windows.
2. Per parlar, pots:
   - Escriure text personalitzat a la caixa d'entrada i prémer el botó `Reproduir>` (o `Enter`).
   - Fer clic en una o diverses frases desades a la finestra de Categories.
3. Durant la reproducció, el botó mostra `>` per indicar que està actiu. Pots prémer-lo de nou per aturar-lo instantàniament.

## Finestra de Categories (frases)
- Obre-la amb el botó `Categories`. Pots redimensionar-la o recordar-ne la mida des de Configuració.
- Selecciona una categoria a dalt i frases a baix.
- Un clic en una frase la reprodueix automàticament si `Parla la frase immediatament en seleccionar-la` està activat; si no, carrega la frase a la caixa d'entrada principal sense parlar.

### Gestionar categories
- Reanomenar: selecciona categoria → (drecera: `F4`) `Canvia el nom de la categoria` → canvia el nom → `D'acord`.
- Moure selecció: `F5`/`F6` per moure a l'element anterior/següent.
- Afegir: selecciona categoria → (drecera: `F7`) diàleg `Afegeix una nova categoria` → escriu el nom → `D'acord`.
- Eliminar: selecciona categoria → (drecera: `F8`) `Eliminar categoria`; confirma l'eliminació de la categoria i les seves frases.



#### Definir o canviar la icona d’una categoria
Cada categoria pot mostrar un emoji o un fitxer `.ico` personalitzat al seu botó. La icona s’especifica al diàleg Afegir/Editar utilitzant el prefix separador `##`:

```
<icona>##<nom de categoria>
```

- **Emoji**: introdueix l’emoji seguit de `##` i del nom.
  - Exemple: `🙂##Salutacions`
- **Fitxer d’icona** (`.ico`): introdueix la ruta del fitxer (absoluta o relativa a la carpeta de SimonSays) seguida de `##` i del nom.
  - Exemple: `C:\icons\smile.ico##Salutacions`
- **Sense icona**: introdueix només el nom de la categoria, sense el prefix `##`.
  - Exemple: `Salutacions`

> Nota: només s’admeten fitxers `.ico` per a icones basades en fitxer. Qualsevol altra extensió s’ignora i no es mostra cap icona.

### Gestionar frases
- Editar: selecciona frase → (drecera: `F4`) `Edita la frase` → actualitza el text → `D'acord`.
- Moure selecció: `F5`/`F6` per moure a la frase anterior/següent.
- Afegir: selecciona frase → (drecera: `F7`) `Afegeix una nova frase` → escriu el text → `D'acord`.
- Eliminar: selecciona frase → (drecera: `F8`) `Eliminar frase`; confirma.



#### Definir o canviar la icona d’una frase
Les frases segueixen la mateixa convenció de prefix d’icona que les categories, amb el separador opcional `##` abans del text de la frase:

```
<icona>##<text de la frase>
```

- **Emoji**: `👍##Sona bé!`
- **Fitxer d’icona**: `C:\icons\check.ico##Sona bé!`
- **Sense icona**: `Sona bé!`

Les frases també poden incloure un fitxer d’àudio en línia. Quan combines icona, text i fitxer d’àudio, el format complet és:

```
<icona>##<text de la frase>::<fitxer d’àudio>
```

- Exemple: `🔔##Atenció::notification.wav`

El prefix `##` i el sufix `::` són opcionals i independents; pots utilitzar qualsevol combinació:
- Només prefix d’icona: `🔔##Atenció`
- Només sufix d’àudio: `Atenció::notification.wav`
- Tots dos: `🔔##Atenció::notification.wav`
- Cap dels dos: `Atenció`

## Configuració (F2)
Obre Configuració (`F2`) per controlar:
- **Text predeterminat**: activa/desactiva `Insereix el text predeterminat automàticament` i personalitza la frase per defecte.
  - `Mostra el teclat tàctil` — obre el teclat tàctil en pantalla quan la caixa d'entrada rep el focus, posicionat a prop de la finestra principal.
- **Idioma de la interfície**: tria l'idioma de la UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Veu**: selecciona una veu SAPI instal·lada segons la teva preferència.
- **Volum/Velocitat**: ajusta `Volum (10-100)` i `Velocitat (-10 a 10)`.
- `Prova la veu`: prova la veu seleccionada.
- **Finestra de categories**:
  - `Minimitza automàticament la finestra de categories` després de seleccionar.
  - `Recorda la mida de la finestra de categories`.
- **Comportament de reproducció**:
  - `Parla la frase immediatament en seleccionar-la` (auto-reproducció amb clic).
  - `Atura la reproducció anterior` — atura automàticament qualsevol reproducció en curs abans d'iniciar una nova frase.
  - `Augmenta el volum de SimonSays en reproduir` — puja el volum mestre del sistema mentre parla, restaurant-lo quan acaba la reproducció.
  - `Redueix temporalment altres sons en reproduir` — redueix el volum de les altres aplicacions mentre parla, restaurant-lo quan acaba l)HELP" LR"HELP(a reproducció.
- `D'acord` desa els canvis; `Cancel·lar` els descarta.

## Mirada / Clic per fixació (F3)
Activa frases i botons sense fer servir les mans mantenint la mirada (o el cursor del ratolí) sobre ells durant un temps determinat. Aquesta funció és nova: necessita proves i comentaris; si us plau, explica'ns com funciona amb el teu seguidor ocular mitjançant l'opció `Comentaris` del menú de safata.

Configuracions compatibles:
- **Qualsevol programari de control ocular que mogui el cursor del ratolí** (Irisbond EasyClick, Tobii Dynavox Computer Control, modes de cursor del Control ocular de Windows, OptiKey, ...): el clic per fixació segueix el cursor.
- **Lectura directa de la mirada, sense necessitat de moure el cursor**: Irisbond Hiru (mode HID) i seguidors Tobii a través del seu propi programari instal·lat — verificat amb Tobii Eye Tracker 4C i Tobii Dynavox PCEye 5; s'espera que Tobii Eye Tracker 5 funcioni, però encara no s'ha provat.

Obre la configuració de Mirada / Clic per fixació (`F3`) per ajustar (els controls d'aquest diàleg es mostren en anglès):
- **Mode d'activació**: tria entre `Automatic`, que detecta la presència i el comportament del dispositiu i decideix si activar el clic per fixació mitjançant el cursor del ratolí o mitjançant detecció de mirada (si es detecta un dispositiu de seguiment ocular o programari relacionat, el clic per fixació s'activa; si no se'n detecta cap, resta desactivat), `Mouse cursor` per activar sempre el clic per fixació mitjançant el cursor, `HID eye tracker` per activar-lo sempre mitjançant detecció de mirada, o `Off` per desactivar-lo sempre, independentment dels dispositius presents.
- **Temps de fixació** (`Dwell time`): durada (en mil·lisegons) necessària per disparar un clic en mirar un element. Mentre la fixació és en curs es mostra un indicador de progrés.
- **Radi de tolerància** (`Tolerance radius`): tolerància de la mirada (en píxels) per permetre petits moviments de l'ull sense cancel·lar el clic per fixació.
- **Temps d'espera** (`Cooldown`): mil·lisegons després d'una activació durant els quals no es dispararà cap altre clic per fixació (evita clics repetits accidentals).
- **Color de progrés** (`Progress color`): color de l'indicador de progrés de la fixació (per defecte, el color d'èmfasi de Windows).
- **Detecta com uses el seguidor** (`Detect how you use the tracker`): dos botons que ajuden l'aplicació a detectar si mous el cursor amb un seguidor ocular o si simplement fas clic als botons.
- **Detectat** (`Detected`): mostra el tipus de dispositiu detectat i informació addicional sobre el programari relacionat.
- Un botó `Reset` (a baix a l'esquerra) restaura tots els ajustos de fixació als valors predeterminats.
- El botó `D'acord` desa els canvis; `Cancel·lar` els descarta.

## Menú de la icona de safata
- `Mostra` / `Amaga` la interfície principal.
- `Configuració`.
- `Gaze / Dwell-click` (Mirada / Clic per fixació).
- `Quant a` mostra la versió i els crèdits.
- `Web` obre la pàgina del projecte (si està configurada).
- `Comentaris` obre el [formulari de comentaris](https://forms.gle/KMbpEDYmwnFJUhgy8) al navegador per defecte.
- `Sortir` tanca SimonSays.

Pots moure la finestra emergent de la safata i altres diàlegs arrossegant la barra de títol; redimensiona la finestra de Categories arrossegant les vores, i activa `Recorda la mida de la finestra de categories` a Configuració per conservar la mida.

## Barrejar veu i sons
- Utilitza el delimitador `♫` (`SOUND_NOTE_DELIMITER`) per inserir noms de fitxers d'àudio al text:
  - Exemple: `Hola ♫notification.wav♫, si us plau, espera un moment.`
  - El text fora dels delimitadors es parla; el text entre delimitadors es tracta com un fitxer de so i es reprodueix en línia.
- Àudio admès: `.wav`, `.mid`, `.midi`, `.mp3`.
- Utilitza rutes absolutes o relatives accessibles al procés. Aparella delimitadors per evitar que el text final sigui tractat com a àudio.



### Ordre de cerca de fitxers de so i d’icona
Quan un nom de fitxer de so o d’icona no conté una lletra d’unitat o una ruta completa, SimonSays el cerca en l’ordre següent i utilitza la primera coincidència trobada:

1. **Carpeta AppData** — `%APPDATA%\SimonSays\` (específica de l’usuari; posa-hi els fitxers per mantenir-los separats de la instal·lació de l’aplicació).
2. **Directori de treball** — el directori de treball actual del procés (només es cerca si és diferent de la carpeta de l’executable).
3. **Carpeta de l’executable** — la carpeta que conté `SimonSays.exe`.

Si el fitxer no es troba en cap d’aquestes ubicacions, s’utilitza el so de reserva integrat.

> **Consell:** és recomanable posar els teus fitxers de so i icona personalitzats a `%APPDATA%\SimonSays\`; així sobreviuran a les actualitzacions i seran específics de l’usuari.

## Importar i exportar categories
- Exportar: pots exportar totes les categories o només la seleccionada quan se't demani. Els missatges d'èxit/error apareixen en l'idioma seleccionat (drecera: `F10`).
- Importar: tria un fitxer; si la categoria existeix, se't demanarà sobreescriure-la. Els missatges d'èxit/error apareixen en l'idioma seleccionat (drecera: `F9`).

## Resolució de problemes
- **Error d'inicialització de veu**: verifica que SAPI estigui instal·lat i que hi hagi una veu compatible.
- **No es reprodueixen sons**: confirma la ruta/extensió i els delimitadors `♫` aparellats.
- **Errors d'importació/exportació**: comprova els permisos i la ubicació del disc; reintenta.
- **Idioma UI no aplicat**: reobre Configuració i confirma la selecció d'idioma; assegura't que existeixin cadenes de localització per a l'idioma triat.

## Quant a
- El diàleg `Quant a` mostra la versió, la descripció i el copyright.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { TRAYICON_FEEDBACK_ID, L"Dona'ns la teua opinió" },
  { AMPERSAND_REPLACEMENT_ID, L"i" },
  { DWELL_DIALOG_TITLE_ID, L"Mirada / Clic per fixació" },
  { DWELL_INTRO_ID, L"Activa els botons mantenint-hi la mirada (o el cursor) a sobre." },
  { DWELL_MODE_GROUP_ID, L"Mode d'activació" },
  { DWELL_MODE_AUTO_ID, L"Automàtic" },
  { DWELL_MODE_MOUSE_ID, L"Cursor del ratolí" },
  { DWELL_MODE_HID_ID, L"Seguidor ocular HID" },
  { DWELL_MODE_OFF_ID, L"Desactivat" },
  { DWELL_TIME_LABEL_ID, L"Temps de fixació (ms)" },
  { DWELL_TOLERANCE_LABEL_ID, L"Radi de tolerància (px)" },
  { DWELL_COOLDOWN_LABEL_ID, L"Temps d'espera (ms)" },
  { DWELL_COLOR_BUTTON_ID, L"Color de progrés..." },
  { DWELL_DETECT_GROUP_ID, L"Detecta com fas servir el seguidor" },
  { DWELL_PROBE_LOOK_ID, L"Activa'm MIRANT-ME" },
  { DWELL_PROBE_MOUSE_ID, L"Activa'm amb el RATOLÍ o commutador" },
  { DWELL_STATUS_INITIAL_ID, L"Prova els dos botons de dalt per definir el millor mode automàticament." },
  { DWELL_STATUS_GAZE_ID, L"Detectat: cursor controlat per la mirada. Clic per fixació activat." },
  { DWELL_STATUS_MOUSE_ID, L"Detectat: ratolí manual. Clic per fixació desactivat." },
  { DWELL_STATUS_LOOK_HINT_ID, L"Si us plau, MIRA el botó sense fer-hi clic." },
  { DWELL_APPLY_BUTTON_ID, L"Aplica" },
  { DWELL_HID_UNAVAILABLE_ID, L"El seguiment ocular HID encara no està disponible." },
  { TRAYICON_DWELL_ID, L"Mirada / Clic per fixació..." },
  { DWELL_SIGNALS_GROUP_ID, L"Detectat" },
  { DWELL_SIGNAL_HID_LABEL_ID, L"Seguidor ocular (HID):" },
  { DWELL_SIGNAL_TOOL_LABEL_ID, L"App de control ocular:" },
  { DWELL_SIGNAL_WEC_LABEL_ID, L"Control ocular de Windows:" },
  { DWELL_SIGNAL_YES_ID, L"sí" },
  { DWELL_SIGNAL_NO_ID, L"no" },
  { DWELL_SIGNAL_NONE_ID, L"cap" },
  { DWELL_SIGNAL_HID_LIVE_ID, L"transmetent la mirada" },
  { DWELL_SIGNAL_HID_IDLE_ID, L"present (sense transmetre)" },
  { DWELL_RESET_BUTTON_ID, L"Restableix" },
  { BOARD_STYLE_CHANGED_TITLE_ID, L"Estil del tauler canviat" },
  { BOARD_STYLE_CHANGED_MESSAGE_ID, L"Vols revertir els canvis?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, L"Eliminar totes les categories i les seues frases?\nAvís: aquesta acció no es pot desfer." },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, L"És un error?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, L"Eliminar totes les categories" },
};

static const std::vector<std::pair<int, const wchar_t *>> CHINESE_SIMPLIFIED_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"播放>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"类别" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"设置" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"默认文本：" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"自动插入默认文本" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"语言：" },
  { SETTINGS_VOICE_LABEL_ID, L"语音：" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"测试语音" },
  { SETTINGS_VOLUME_LABEL_ID, L"音量 (10-100)：" },
  { SETTINGS_RATE_LABEL_ID, L"语速 (-10 到 10)：" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"选择短语时立即朗读" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"记住分类窗口大小" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"自动最小化分类窗口" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"播放时提高 SimonSays 音量" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"播放时临时降低其他音频" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"停止上一次播放" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"显示触摸键盘" },
  { SETTINGS_OK_BUTTON_ID, L"确定" },
  { SETTINGS_CANCEL_BUTTON_ID, L"取消" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - 点击显示" },
  { TRAYICON_SHOW_ID, L"显示" },
  { TRAYICON_HIDE_ID, L"隐藏" },
  { TRAYICON_SETTINGS_ID, L"设置" },
  { TRAYICON_ABOUT_ID, L"关于" },
  { TRAYICON_WEB_ID, L"网站" },
  { TRAYICON_EXIT_ID, L"退出" },
  { ABOUT_TITTLE_ID, L"关于 SimonSays" },
  { ABOUT_1_ID, L"SimonSays - 简单开口说\n\n版本 " },
  { ABOUT_2_ID, L"\n\n一款简单的辅助工具，可立即将文本转换为语音。\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays 错误" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays 目前仅支持底部任务栏" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"更改类别名称" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"编辑短语" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"当前 : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"确定" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"取消" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"已存在一个具有此名称的类别。请选择一个不同的名称。" },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"类别名称冲突" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"删除类别" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"删除类别 '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' 和它的所有短语吗？" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"删除短语" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"删除短语 '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'？" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"添加新类别" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"添加新短语" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"类别名称:" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"短语文本:" },
  { ADD_DIALOG_OK_BUTTON_ID, L"确定" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"取消" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"新的默认短语" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"导入类别" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"覆盖类别 '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"覆盖类别?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"替换面板样式？" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"导入的文件包含面板样式。要替换当前样式吗？" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"短语导入成功。" },
  { IMPORT_SUCCESS_TITLE_ID, L"导入成功" },
  { IMPORT_FAILURE_MESSAGE_ID, L"导入短语失败。" },
  { IMPORT_FAILURE_TITLE_ID, L"导入失败" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"导出类别" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"你可以导出所有类别或只导出选中的类别。\n你要仅导出 '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' 类别吗?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"导出选择" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"短语导出成功。" },
  { EXPORT_SUCCESS_TITLE_ID, L"导出成功" },
  { EXPORT_FAILURE_MESSAGE_ID, L"导出短语失败。" },
  { EXPORT_FAILURE_TITLE_ID, L"导出失败" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1：帮助 - F2：设置" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays 帮助指南

## 概述
SimonSays 是由 Juan Rey Saura 创建的 Windows 文本转语音（TTS）助手，专注于快速发声、短语库和内联声音播放。它支持多种语言、可自定义的语音以及高效沟通的便捷功能。

SimonSays 利用 Windows 任务栏左侧的空闲区域，只需几次点击即可快速访问十多个类别中常用的（可自定义的）短语。

## 快捷键
- `F1`：打开帮助。
- `F2`：打开设置。
- `F3`：注视 / 驻留点击设置。
- `F4`：编辑当前选择。
- `F5` / `F6`：在列表中移动到上一个 / 下一个选择。
- `F7`：在当前选择后添加（类别/短语上下文）。
- `F8`：删除当前选择。
- `F9`：导入类别。
- `F10`：导出类别。
- `F11` / `Ctrl -`：缩小（类别窗口）。
- `F12` / `Ctrl +`：放大（类别窗口）。
- `Ctrl 0`：重置缩放。
- `播放>` 按钮：开始播放。

## 启动和基本使用
1. 启动 SimonSays；它出现在 Windows 系统托盘中。
2. 要发声，你可以：
   - 在输入框中键入自定义文本，然后按 `播放>` 按钮（或 `Enter`）。
   - 在类别窗口中点击一个或多个已保存的短语。
3. 播放期间，按钮显示 `>` 表示活动播放。你可以再次按下它立即停止。

## 类别窗口（短语）
- 通过 `类别` 按钮打开。可以调整大小或在设置中记住其大小。
- 在顶部选择类别，底部选择短语。
- 单击短语时，如果启用了 `选择短语时立即朗读`，则会自动朗读；否则，将短语加载到主输入框而不朗读。

### 管理类别
- 重命名：选择类别 → （快捷键：`F4`）`更改类别名称` → 更改名称 → `确定`。
- 移动选择：`F5`/`F6` 移动到上一个/下一个项目。
- 添加：选择类别 → （快捷键：`F7`）`添加新类别` 对话框 → 输入名称 → `确定`。
- 删除：选择类别 → （快捷键：`F8`）`删除类别`；确认删除类别及其所有短语。



#### 设置或更改类别图标
每个类别都可以在其按钮上显示一个表情符号或自定义 `.ico` 文件。图标通过在添加/编辑对话框中使用 `##` 分隔前缀来指定：

```
<图标>##<类别名称>
```

- **表情符号**：输入表情符号字符，后跟 `##` 和名称。
  - 示例：`🙂##问候`
- **图标文件** (`.ico`)：输入文件路径（绝对路径或相对于 SimonSays 文件夹的路径），后跟 `##` 和名称。
  - 示例：`C:\icons\smile.ico##问候`
- **无图标**：仅输入类别名称，不加 `##` 前缀。
  - 示例：`问候`

> 注意：基于文件的图标仅支持 `.ico` 文件。其他扩展名会被忽略，且不会显示图标。

### 管理短语
- 编辑：选择短语 → （快捷键：`F4`）`编辑短语` → 更新文本 → `确定`。
- 移动选择：`F5`/`F6` 移动到上一个/下一个短语。
- 添加：选择短语 → （快捷键：`F7`）`添加新短语` → 输入文本 → `确定`。
- 删除：选择短语 → （快捷键：`F8`）`删除短语`；确认。



#### 设置或更改短语图标
短语遵循与类别相同的图标前缀约定，可选的 `##` 分隔符位于短语文本之前：

```
<图标>##<短语文本>
```

- **表情符号**：`👍##听起来不错！`
- **图标文件**：`C:\icons\check.ico##听起来不错！`
- **无图标**：`听起来不错！`

短语还可以包含内联音频文件。组合图标、文本和音频文件时，完整格式为：

```
<图标>##<短语文本>::<音频文件>
```

- 示例：`🔔##注意::notification.wav`

`##` 前缀和 `::` 后缀都是可选且彼此独立的；你可以使用任意组合：
- 仅图标前缀：`🔔##注意`
- 仅音频后缀：`注意::notification.wav`
- 两者都有：`🔔##注意::notification.wav`
- 两者都没有：`注意`

## 设置（F2）
打开设置（`F2`）以控制：
- **默认文本**：启用/禁用 `自动插入默认文本` 并自定义默认短语。
  - `显示触摸键盘` — 当输入框获得焦点时打开屏幕触摸键盘，位于主窗口附近。
- **界面语言**：选择 UI 语言（English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian）。
- **语音**：选择与你偏好匹配的已安装 SAPI 语音。
- **音量/语速**：调整 `音量 (10-100)` 和 `语速 (-10 到 10)`。
- `测试语音`：试听选定的语音。
- **类别窗口**：
  - `自动最小化分类窗口`（选择后）。
  - `记住分类窗口大小`。
- **播放行为**：
  - `选择短语时立即朗读`（单击自)HELP" LR"HELP(动朗读）。
  - `停止上一次播放` — 在开始新短语之前自动停止任何正在进行的播放。
  - `播放时提高 SimonSays 音量` — 在朗读时提高系统主音量，播放结束后恢复。
  - `播放时临时降低其他音频` — 在朗读时降低所有其他应用程序的音量，播放结束后恢复。
- `确定` 保存更改；`取消` 放弃。

## 注视 / 驻留点击（F3）
将注视（或鼠标光标）停留在短语和按钮上一段设定的时间，即可免手动激活它们。此功能是新功能——需要测试和反馈：请通过托盘菜单的 `反馈` 选项告诉我们它在你的眼动仪上的使用情况。

支持的配置：
- **任何移动鼠标光标的眼控软件**（Irisbond EasyClick、Tobii Dynavox Computer Control、Windows 眼球控制的光标模式、OptiKey 等）：驻留点击跟随光标。
- **直接读取注视，无需移动光标**：Irisbond Hiru（HID 模式）以及通过自身已安装软件工作的 Tobii 眼动仪——已在 Tobii Eye Tracker 4C 和 Tobii Dynavox PCEye 5 上验证；Tobii Eye Tracker 5 预计可用，但尚未测试。

打开注视 / 驻留点击设置（`F3`）进行配置（此对话框的控件以英文显示）：
- **激活模式**：可选择 `Automatic`（自动检测设备的存在和行为，决定通过鼠标光标还是注视检测启用驻留点击；检测到眼动设备或相关软件时启用驻留点击，未检测到则保持关闭）、`Mouse cursor`（始终通过光标启用驻留点击）、`HID eye tracker`（始终通过注视检测启用），或 `Off`（无论设备是否存在都始终禁用）。
- **驻留时间**（`Dwell time`）：注视某元素触发点击所需的时长（毫秒）。驻留进行中会显示进度指示。
- **容差半径**（`Tolerance radius`）：注视容差（像素），允许眼睛轻微移动而不取消驻留点击。
- **冷却时间**（`Cooldown`）：一次激活后的毫秒数，期间不会触发新的驻留点击（防止意外重复点击）。
- **进度颜色**（`Progress color`）：驻留进度指示的颜色（默认为 Windows 主题色）。
- **检测你如何使用眼动仪**（`Detect how you use the tracker`）：两个按钮，帮助应用检测你是用眼动仪移动光标，还是只是点击按钮。
- **已检测**（`Detected`）：显示检测到的设备类型及相关软件的更多信息。
- `Reset` 按钮（左下角）将所有驻留设置恢复为默认值。
- `确定` 按钮保存更改；`取消` 放弃更改。

## 托盘图标菜单
- `显示` / `隐藏` 主界面。
- `设置`。
- `Gaze / Dwell-click`（注视 / 驻留点击）。
- `关于` 显示版本和致谢。
- `网站` 打开项目页面（如已配置）。
- `反馈` 会在默认浏览器中打开[反馈表单](https://forms.gle/KMbpEDYmwnFJUhgy8)。
- `退出` 退出 SimonSays。

你可以通过拖动标题栏来移动托盘弹出窗口和其他对话框；通过拖动边缘来调整类别窗口的大小，并在设置中启用 `记住分类窗口大小` 以保持大小。

## 混合语音和声音
- 使用分隔符 `♫`（`SOUND_NOTE_DELIMITER`）在文本中嵌入音频文件名：
  - 示例：`你好 ♫notification.wav♫，请稍等。`
  - 分隔符外的文本会被朗读；分隔符之间的文本被视为声音文件并在行内播放。
- 支持格式：`.wav`、`.mid`、`.midi`、`.mp3`。
- 使用进程可访问的绝对或相对路径。配对分隔符以避免尾部文本被视为音频。



### 声音和图标文件查找顺序
当声音或图标文件名不包含盘符或完整路径时，SimonSays 会按以下顺序搜索，并使用第一个找到的匹配项：

1. **AppData 文件夹** — `%APPDATA%\SimonSays\`（用户专用；建议将文件放在这里，以便与程序安装目录分开）。
2. **工作目录** — 进程当前的工作目录（仅当它与可执行文件所在文件夹不同的时候才会搜索）。
3. **可执行文件文件夹** — 包含 `SimonSays.exe` 的文件夹。

如果在这些位置都找不到文件，则会改用内置的后备声音。

> **提示：** 建议将自定义声音和图标文件放在 `%APPDATA%\SimonSays\` 中；这样它们可以在应用更新后继续保留，并且是用户专用的。

## 导入和导出类别
- 导出：在提示时可以导出所有类别或仅导出选中的类别。成功/失败消息以选定语言显示（快捷键：`F10`）。
- 导入：选择文件；如果类别已存在，将询问是否覆盖。成功/失败消息以选定语言显示（快捷键：`F9`）。

## 故障排除
- **语音初始化失败**：验证 SAPI 已安装且存在兼容的语音。
- **无声音播放**：确认文件路径/扩展名和配对的 `♫` 分隔符。
- **导入/导出错误**：检查文件权限和磁盘位置；重试。
- **UI 语言未应用**：重新打开设置并确认语言选择；确保所选语言存在本地化字符串。

## 关于
- `关于` 对话框列出版本、描述和版权信息。
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"和" },
};

static const std::vector<std::pair<int, const wchar_t *>> FRENCH_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Lire>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Catégories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Paramètres" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texte par défaut :" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insérer automatiquement le texte par défaut" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Langue :" },
  { SETTINGS_VOICE_LABEL_ID, L"Voix :" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Tester la voix" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100) :" },
  { SETTINGS_RATE_LABEL_ID, L"Débit (-10 à 10) :" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Dire immédiatement la phrase lors de la sélection" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Mémoriser la taille de la fenêtre des catégories" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Réduire automatiquement la fenêtre des catégories" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Augmenter le volume de SimonSays pendant la lecture" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Réduire temporairement les autres sons pendant la lecture" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Arrêter la lecture précédente" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Afficher le clavier tactile" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Annuler" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Cliquez pour afficher" },
  { TRAYICON_SHOW_ID, L"Afficher" },
  { TRAYICON_HIDE_ID, L"Masquer" },
  { TRAYICON_SETTINGS_ID, L"Paramètres" },
  { TRAYICON_ABOUT_ID, L"À propos" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Quitter" },
  { ABOUT_TITTLE_ID, L"À propos de SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplement parler\n\nVersion " },
  { ABOUT_2_ID, L"\n\nUn outil d'accessibilité simple qui\ntransforme instantanément du texte en voix.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Erreur SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays prend actuellement uniquement en charge la barre des tâches inférieure" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Changer le nom de la catégorie" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Modifier la phrase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actuel : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Annuler" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Une catégorie avec ce nom existe déjà. Veuillez choisir un nom différent." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflit de noms de catégorie" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Supprimer la catégorie" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Supprimer la catégorie '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' et toutes ses phrases ?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Supprimer la phrase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Supprimer la phrase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' ?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Ajouter une nouvelle catégorie" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Ajouter une nouvelle phrase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nom de catégorie :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Texte de la phrase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Annuler" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nouvelle phrase par défaut" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importer des catégories" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"Catégories SimonSays\0*.ssc;*.ssz\0Tous les fichiers\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Remplacer la catégorie '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' ?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Remplacer la catégorie ?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Remplacer le style du tableau ?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"Le fichier importé contient un style de tableau. Remplacer le style actuel ?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Expressions importées avec succès." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importation réussie" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Échec de l'importation des phrases." },
  { IMPORT_FAILURE_TITLE_ID, L"Échec de l'importation" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exporter des catégories" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"Catégories SimonSays\0*.ssc;*.ssz\0Tous les fichiers\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Vous pouvez exporter toutes les catégories ou seulement celle sélectionnée.\nVoulez-vous exporter uniquement la catégorie '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' ?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exporter la sélection" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Expressions exportées avec succès." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportation réussie" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Échec de l'exportation des phrases." },
  { EXPORT_FAILURE_TITLE_ID, L"Échec de l'exportation" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1 : Aide - F2 : Paramètres" },
  { HELP_CONTENT_ID, LR"HELP(# Guide d'aide SimonSays

## Vue d'ensemble
SimonSays est un assistant Windows de synthèse vocale (TTS), créé par Juan Rey Saura, optimisé pour la parole rapide, les bibliothèques de phrases et la lecture de sons intégrés. Il prend en charge plusieurs langues, des voix personnalisables et des fonctionnalités pratiques pour une communication efficace.

SimonSays utilise l'espace libre à gauche de la barre des tâches Windows, offrant un accès rapide à une douzaine de catégories de phrases fréquemment utilisées (et personnalisables) en quelques clics.

## Raccourcis
- `F1` : Ouvrir l'aide.
- `F2` : Ouvrir les paramètres.
- `F3` : Paramètres Regard / Clic par fixation.
- `F4` : Modifier la sélection actuelle.
- `F5` / `F6` : Déplacer vers la sélection précédente / suivante dans les listes.
- `F7` : Ajouter après la sélection actuelle (contexte catégorie/phrase).
- `F8` : Supprimer la sélection actuelle.
- `F9` : Importer des catégories.
- `F10` : Exporter des catégories.
- `F11` / `Ctrl -` : Zoom arrière (fenêtre des catégories).
- `F12` / `Ctrl +` : Zoom avant (fenêtre des catégories).
- `Ctrl 0` : Réinitialiser le zoom.
- Bouton `Lire>` : Démarrer la lecture.

## Démarrage et utilisation de base
1. Lancez SimonSays ; il apparaît dans la zone de notification Windows.
2. Pour parler, vous pouvez :
   - Saisir un texte personnalisé dans la zone de saisie et appuyer sur le bouton `Lire>` (ou `Enter`).
   - Cliquer sur une ou plusieurs phrases enregistrées dans la fenêtre Catégories.
3. Pendant la lecture, le bouton affiche `>` pour indiquer la lecture active. Vous pouvez appuyer à nouveau pour arrêter instantanément.

## Fenêtre Catégories (phrases)
- Ouvrez via le bouton `Catégories`. Redimensionnement/mémorisation de la taille via Paramètres.
- Sélectionnez une catégorie en haut et les phrases en bas.
- Un clic sur une phrase lance la lecture immédiate si `Dire immédiatement la phrase lors de la sélection` est activé ; sinon la phrase est chargée dans la zone de saisie principale sans lecture.

### Gérer les catégories
- Renommer : sélectionner catégorie → (raccourci : `F4`) `Changer le nom de la catégorie` → modifier le nom → `OK`.
- Déplacer la sélection : `F5`/`F6` pour précédent/suivant.
- Ajouter : sélectionner catégorie → (raccourci : `F7`) dialogue `Ajouter une nouvelle catégorie` → saisir le nom → `OK`.
- Supprimer : sélectionner catégorie → (raccourci : `F8`) `Supprimer la catégorie` ; confirmer la suppression de la catégorie et de ses phrases.



#### Définir ou modifier l’icône d’une catégorie
Chaque catégorie peut afficher un emoji ou un fichier `.ico` personnalisé sur son bouton. L’icône se spécifie dans la boîte de dialogue Ajouter/Modifier à l’aide du préfixe séparateur `##` :

```
<icône>##<nom de catégorie>
```

- **Emoji** : saisissez le ou les emoji suivis de `##` puis du nom.
  - Exemple : `🙂##Salutations`
- **Fichier d’icône** (`.ico`) : saisissez le chemin du fichier (absolu ou relatif au dossier SimonSays) suivi de `##` puis du nom.
  - Exemple : `C:\icons\smile.ico##Salutations`
- **Sans icône** : saisissez simplement le nom de la catégorie sans préfixe `##`.
  - Exemple : `Salutations`

> Remarque : seuls les fichiers `.ico` sont pris en charge pour les icônes basées sur un fichier. Toute autre extension est ignorée et aucune icône n’est affichée.

### Gérer les phrases
- Modifier : sélectionner phrase → (raccourci : `F4`) `Modifier la phrase` → mettre à jour le texte → `OK`.
- Déplacer la sélection : `F5`/`F6` pour précédent/suivant.
- Ajouter : sélectionner phrase → (raccourci : `F7`) `Ajouter une nouvelle phrase` → saisir le texte → `OK`.
- Supprimer : sélectionner phrase → (raccourci : `F8`) `Supprimer la phrase` ; confirmer.



#### Définir ou modifier l’icône d’une phrase
Les phrases suivent la même convention de préfixe d’icône que les catégories, avec le séparateur optionnel `##` placé avant le texte de la phrase :

```
<icône>##<texte de la phrase>
```

- **Emoji** : `👍##Ça marche bien !`
- **Fichier d’icône** : `C:\icons\check.ico##Ça marche bien !`
- **Sans icône** : `Ça marche bien !`

Les phrases peuvent également inclure un fichier audio intégré. Lorsque vous combinez une icône, du texte et un fichier audio, le format complet est :

```
<icône>##<texte de la phrase>::<fichier audio>
```

- Exemple : `🔔##Attention::notification.wav`

Le préfixe `##` et le suffixe `::` sont optionnels et indépendants ; vous pouvez utiliser n’importe quelle combinaison :
- Préfixe d’icône uniquement : `🔔##Attention`
- Suffixe audio uniquement : `Attention::notification.wav`
- Les deux : `🔔##Attention::notification.wav`
- Aucun : `Attention`

## Paramètres (F2)
Ouvrez les paramètres (`F2`) pour contrôler :
- **Texte par défaut** : activer/désactiver `Insérer automatiquement le texte par défaut` et personnaliser la phrase par défaut.
  - `Afficher le clavier tactile` — ouvre le clavier tactile à l'écran lorsque la zone de saisie reçoit le focus, positionné près de la fenêtre principale.
- **Langue de l'interface** : choisir la langue de l'UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voix** : sélectionner une voix SAPI installée correspondant à votre préférence.
- **Volume/Débit** : ajuster `Volume (10-100)` et `Débit (-10 à 10)`.
- `Tester la voix` : écouter la voix sélectionnée.
- **Fenêtre des catégories** :
  - `Réduire automatiquement la fenêtre des catégories` après sélection.
  - `Mémoriser la taille de la fenêtre des catégories`.
- **Comportement de lecture** :
  - `Dire immédiatement la phrase lors de la sélection` (lecture automatique au clic).
  - `Arrêter la lecture précédente` — arrête automatiquement toute lecture en cours avant de démarrer une nouvelle phrase.
  - `Augmenter le volume de SimonSays pendant la lecture` — augmente le volume maître du système pendant la parole, le restaurant à la fin de la lecture.
  - `Réduire temporairement les autres sons pendant la lecture` — réduit le volume de toutes les autres applications pendant la parole, le resta)HELP" LR"HELP(urant à la fin de la lecture.
- `OK` enregistre les modifications ; `Annuler` les annule.

## Regard / Clic par fixation (F3)
Activez des phrases et des boutons sans les mains en maintenant votre regard (ou le curseur de la souris) dessus pendant une durée définie. Cette fonctionnalité est nouvelle — elle a besoin de tests et de retours : dites-nous comment elle fonctionne avec votre dispositif de suivi oculaire via l'option `Feedback` du menu de notification.

Configurations prises en charge :
- **Tout logiciel de contrôle oculaire qui déplace le curseur de la souris** (Irisbond EasyClick, Tobii Dynavox Computer Control, modes curseur du Contrôle visuel de Windows, OptiKey, ...) : le clic par fixation suit le curseur.
- **Lecture directe du regard, sans déplacement du curseur** : Irisbond Hiru (mode HID) et dispositifs Tobii via leur propre logiciel installé — vérifié avec le Tobii Eye Tracker 4C et le Tobii Dynavox PCEye 5 ; le Tobii Eye Tracker 5 devrait fonctionner, mais n'a pas encore été testé.

Ouvrez les paramètres Regard / Clic par fixation (`F3`) pour configurer (les contrôles de cette boîte de dialogue s'affichent en anglais) :
- **Mode d'activation** : choisissez entre `Automatic`, qui détecte la présence et le comportement du dispositif et décide d'activer le clic par fixation via le curseur de la souris ou via la détection du regard (si un dispositif de suivi oculaire ou un logiciel associé est détecté, le clic par fixation est activé ; sinon, il reste désactivé), `Mouse cursor` pour toujours activer le clic par fixation via le curseur, `HID eye tracker` pour toujours l'activer via la détection du regard, ou `Off` pour toujours le désactiver, quels que soient les dispositifs présents.
- **Temps de fixation** (`Dwell time`) : durée (en millisecondes) nécessaire pour déclencher un clic en regardant un élément. Un indicateur de progression s'affiche pendant la fixation.
- **Rayon de tolérance** (`Tolerance radius`) : tolérance du regard (en pixels) pour permettre de légers mouvements des yeux sans annuler le clic par fixation.
- **Délai de repos** (`Cooldown`) : millisecondes après une activation pendant lesquelles aucun autre clic par fixation ne sera déclenché (évite les clics répétés accidentels).
- **Couleur de progression** (`Progress color`) : couleur de l'indicateur de progression de la fixation (par défaut, la couleur d'accentuation de Windows).
- **Détecter comment vous utilisez le dispositif** (`Detect how you use the tracker`) : deux boutons qui aident l'application à détecter si vous déplacez le curseur avec un dispositif de suivi oculaire ou si vous cliquez simplement sur les boutons.
- **Détecté** (`Detected`) : affiche le type de dispositif détecté et des informations supplémentaires sur les logiciels associés.
- Un bouton `Reset` (en bas à gauche) restaure tous les paramètres de fixation à leurs valeurs par défaut.
- Le bouton `OK` enregistre les modifications ; `Annuler` les rejette.

## Menu de l'icône de notification
- `Afficher` / `Masquer` l'interface principale.
- `Paramètres`.
- `Gaze / Dwell-click` (Regard / Clic par fixation).
- `À propos` affiche la version et les crédits.
- `Web` ouvre la page du projet (si configurée).
- `Feedback` ouvre un [formulaire de commentaires](https://forms.gle/KMbpEDYmwnFJUhgy8) dans le navigateur par défaut.
- `Quitter` ferme SimonSays.

Vous pouvez déplacer la fenêtre contextuelle de notification et les autres dialogues en faisant glisser la barre de titre ; redimensionnez la fenêtre Catégories en faisant glisser ses bords, et activez `Mémoriser la taille de la fenêtre des catégories` dans les Paramètres pour conserver la taille.

## Mélange parole et sons
- Utilisez le délimiteur `♫` (`SOUND_NOTE_DELIMITER`) pour insérer des noms de fichiers audio dans le texte :
  - Exemple : `Bonjour ♫notification.wav♫, veuillez patienter.`
  - Le texte en dehors des délimiteurs est prononcé ; le texte entre les délimiteurs est traité comme un fichier audio et joué en ligne.
- Formats audio pris en charge : `.wav`, `.mid`, `.midi`, `.mp3`.
- Utilisez des chemins absolus ou relatifs accessibles au processus. Appariez les délimiteurs pour éviter que le texte final ne soit traité comme audio.



### Ordre de recherche des fichiers audio et d’icône
Lorsqu’un nom de fichier audio ou d’icône ne contient ni lettre de lecteur ni chemin complet, SimonSays le recherche dans l’ordre suivant et utilise la première correspondance trouvée :

1. **Dossier AppData** — `%APPDATA%\SimonSays\` (spécifique à l’utilisateur ; placez vos fichiers ici pour les séparer de l’installation de l’application).
2. **Répertoire de travail** — le répertoire de travail actuel du processus (uniquement s’il diffère du dossier de l’exécutable).
3. **Dossier de l’exécutable** — le dossier contenant `SimonSays.exe`.

Si le fichier n’est trouvé dans aucun de ces emplacements, le son de secours intégré est utilisé.

> **Conseil :** il est recommandé de placer vos fichiers son et icônes personnalisés dans `%APPDATA%\SimonSays\` ; ils survivront aux mises à jour et resteront propres à l’utilisateur.

## Importer et exporter des catégories
- Exporter : vous pouvez exporter toutes les catégories ou seulement celle sélectionnée lorsque cela est demandé. Les messages de succès/échec apparaissent dans la langue sélectionnée (raccourci : `F10`).
- Importer : choisir un fichier ; si la catégorie existe, on vous demandera de l'écraser. Les messages de succès/échec apparaissent dans la langue sélectionnée (raccourci : `F9`).

## Dépannage
- **Échec d'initialisation de la voix** : vérifiez que SAPI est installé et qu'une voix compatible est présente.
- **Pas de lecture de sons** : confirmez le chemin/l'extension et les délimiteurs `♫` appariés.
- **Erreurs d'importation/exportation** : vérifiez les permissions et l'emplacement du disque ; réessayez.
- **Langue de l'UI non appliquée** : rouvrez les Paramètres et confirmez la sélection de langue ; assurez-vous que les chaînes de localisation existent pour la langue choisie.

## À propos
- Le dialogue `À propos` affiche la version, la description et le copyright.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { TRAYICON_FEEDBACK_ID, L"Donnez votre avis" },
  { AMPERSAND_REPLACEMENT_ID, L"et" },
  { DWELL_DIALOG_TITLE_ID, L"Regard / Clic par fixation" },
  { DWELL_INTRO_ID, L"Activez les boutons en maintenant votre regard (ou le curseur) dessus." },
  { DWELL_MODE_GROUP_ID, L"Mode d'activation" },
  { DWELL_MODE_AUTO_ID, L"Automatique" },
  { DWELL_MODE_MOUSE_ID, L"Curseur de la souris" },
  { DWELL_MODE_HID_ID, L"Oculomètre HID" },
  { DWELL_MODE_OFF_ID, L"Désactivé" },
  { DWELL_TIME_LABEL_ID, L"Temps de fixation (ms)" },
  { DWELL_TOLERANCE_LABEL_ID, L"Rayon de tolérance (px)" },
  { DWELL_COOLDOWN_LABEL_ID, L"Délai d'attente (ms)" },
  { DWELL_COLOR_BUTTON_ID, L"Couleur de progression..." },
  { DWELL_DETECT_GROUP_ID, L"Détecter comment vous utilisez le traceur" },
  { DWELL_PROBE_LOOK_ID, L"Activez-moi en me REGARDANT" },
  { DWELL_PROBE_MOUSE_ID, L"Activez-moi avec la SOURIS ou un contacteur" },
  { DWELL_STATUS_INITIAL_ID, L"Essayez les deux boutons ci-dessus pour définir automatiquement le meilleur mode." },
  { DWELL_STATUS_GAZE_ID, L"Détecté : curseur contrôlé par le regard. Clic par fixation activé." },
  { DWELL_STATUS_MOUSE_ID, L"Détecté : souris manuelle. Clic par fixation désactivé." },
  { DWELL_STATUS_LOOK_HINT_ID, L"Veuillez REGARDER le bouton sans cliquer dessus." },
  { DWELL_APPLY_BUTTON_ID, L"Appliquer" },
  { DWELL_HID_UNAVAILABLE_ID, L"Le suivi oculaire HID n'est pas encore disponible." },
  { TRAYICON_DWELL_ID, L"Regard / Clic par fixation..." },
  { DWELL_SIGNALS_GROUP_ID, L"Détecté" },
  { DWELL_SIGNAL_HID_LABEL_ID, L"Oculomètre (HID) :" },
  { DWELL_SIGNAL_TOOL_LABEL_ID, L"Application de contrôle oculaire :" },
  { DWELL_SIGNAL_WEC_LABEL_ID, L"Contrôle oculaire Windows :" },
  { DWELL_SIGNAL_YES_ID, L"oui" },
  { DWELL_SIGNAL_NO_ID, L"non" },
  { DWELL_SIGNAL_NONE_ID, L"aucune" },
  { DWELL_SIGNAL_HID_LIVE_ID, L"flux du regard actif" },
  { DWELL_SIGNAL_HID_IDLE_ID, L"présent (pas de flux)" },
  { DWELL_RESET_BUTTON_ID, L"Réinitialiser" },
  { BOARD_STYLE_CHANGED_TITLE_ID, L"Style du tableau modifié" },
  { BOARD_STYLE_CHANGED_MESSAGE_ID, L"Voulez-vous annuler les modifications ?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, L"Supprimer toutes les catégories et leurs phrases ?\nAvertissement : cette action est irréversible." },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, L"Est-ce une erreur ?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, L"Supprimer toutes les catégories" },
};

static const std::vector<std::pair<int, const wchar_t *>> GALICIAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Reproducir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorías" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuración" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texto predeterminado:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Inserir automaticamente o texto predeterminado" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voz:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Probar voz" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocidade (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Falar a frase inmediatamente ao seleccionala" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Lembrar o tamaño da xanela de categorías" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar automáticamente a xanela de categorías" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumentar o volume de SimonSays ao reproducir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Reducir temporalmente outros sons ao reproducir" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Deter a reprodución anterior" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Mostrar teclado táctil" },
  { SETTINGS_OK_BUTTON_ID, L"Aceptar" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancelar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Fai clic para mostrar" },
  { TRAYICON_SHOW_ID, L"Mostrar" },
  { TRAYICON_HIDE_ID, L"Ocultar" },
  { TRAYICON_SETTINGS_ID, L"Configuración" },
  { TRAYICON_ABOUT_ID, L"Sobre" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Saír" },
  { ABOUT_TITTLE_ID, L"Sobre SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplemente fala\n\nVersión " },
  { ABOUT_2_ID, L"\n\nUnha ferramenta de accesibilidade sinxela que\npermite converter texto en voz ao instante.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Erro de SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays só admite actualmente a barra de tarefas inferior" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Cambiar o nome da categoría" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Editar a frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"Aceptar" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Xa existe unha categoría con este nome. Por favor, elixe un nome diferente." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflito de nomes de categoría" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Eliminar categoría" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Eliminar a categoría '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' e todas as súas frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Eliminar frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Eliminar a frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Engadir nova categoría" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Engadir nova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nome da categoría :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Texto da frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"Aceptar" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nova frase predeterminada" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categorías" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categorías de SimonSays\0*.ssc;*.ssz\0Todos os ficheiros\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sobrescribir a categoría '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sobrescribir categoría?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Substituír o estilo do taboleiro?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"O ficheiro importado inclúe un estilo de taboleiro. Substituír o actual?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importadas correctamente." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importación correcta" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Fallou a importación das frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Erro na importación" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categorías" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categorías de SimonSays\0*.ssc;*.ssz\0Todos os ficheiros\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Podes exportar todas as categorías ou só a seleccionada.\nQueres exportar só a categoría '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar selección" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportadas correctamente." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportación correcta" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Fallou a exportación das frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Erro na exportación" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Axuda - F2: Configuración" },
  { HELP_CONTENT_ID, LR"HELP(# Guía de Axuda de SimonSays

## Visión xeral
SimonSays é un asistente de texto a voz (TTS) para Windows, creado por Juan Rey Saura, optimizado para fala rápida, bibliotecas de frases e reprodución de sons en liña. Soporta múltiples idiomas, voces configurables e funcións pensadas para unha comunicación eficiente.

SimonSays utiliza o espazo libre á esquerda da barra de tarefas de Windows, proporcionando acceso rápido a unha ducia de categorías de frases de uso frecuente (e personalizables) con un par de clics.

## Atallos
- `F1`: Abrir Axuda.
- `F2`: Abrir Configuración.
- `F3`: Configuración de Mirada / Clic por fixación.
- `F4`: Editar a selección actual.
- `F5` / `F6`: Mover á selección anterior / seguinte nas listas.
- `F7`: Engadir despois da selección actual (contexto categoría/frase).
- `F8`: Eliminar a selección actual.
- `F9`: Importar categorías.
- `F10`: Exportar categorías.
- `F11` / `Ctrl -`: Afastar o zoom (xanela de categorías).
- `F12` / `Ctrl +`: Achegar o zoom (xanela de categorías).
- `Ctrl 0`: Restablecer o zoom.
- Botón `Reproducir>`: Iniciar reprodución.

## Inicio e uso básico
1. Inicia SimonSays; aparece na bandexa do sistema de Windows.
2. Para falar, podes:
   - Escribir texto personalizado na caixa de entrada e premer o botón `Reproducir>` (ou `Enter`).
   - Facer clic nunha ou varias frases gardadas na xanela de Categorías.
3. Durante a reprodución, o botón mostra `>` para indicar o estado activo. Podes premelo de novo para deter a reprodución ao instante.

## Xanela de Categorías (frases)
- Ábrela co botón `Categorías`. Podes redimensionala ou recordar o seu tamaño desde Configuración.
- Selecciona unha categoría arriba e frases abaixo.
- Un clic nunha frase reprodúcea automaticamente se `Falar a frase inmediatamente ao seleccionala` está activado; se non, carga a frase na caixa de entrada principal sen falar.

### Xestionar categorías
- Renomear: selecciona categoría → (atallo: `F4`) `Cambiar o nome da categoría` → cambia o nome → `Aceptar`.
- Mover selección: `F5`/`F6` para mover ao elemento anterior/seguinte.
- Engadir: selecciona categoría → (atallo: `F7`) diálogo `Engadir nova categoría` → escribe o nome → `Aceptar`.
- Eliminar: selecciona categoría → (atallo: `F8`) `Eliminar categoría`; confirma a eliminación da categoría e as súas frases.



#### Configurar ou cambiar a icona dunha categoría
Cada categoría pode mostrar un emoji ou un ficheiro `.ico` personalizado no seu botón. A icona especifícase no diálogo Engadir/Editar usando o prefixo separador `##`:

```
<icona>##<nome da categoría>
```

- **Emoji**: escribe o emoji seguido de `##` e do nome.
  - Exemplo: `🙂##Saúdos`
- **Ficheiro de icona** (`.ico`): escribe a ruta do ficheiro (absoluta ou relativa ao cartafol de SimonSays) seguida de `##` e do nome.
  - Exemplo: `C:\icons\smile.ico##Saúdos`
- **Sen icona**: escribe só o nome da categoría, sen o prefixo `##`.
  - Exemplo: `Saúdos`

> Nota: só se admiten ficheiros `.ico` para iconas baseadas en ficheiro. Calquera outra extensión ignórase e non se mostra ningunha icona.

### Xestionar frases
- Editar: selecciona frase → (atallo: `F4`) `Editar a frase` → actualiza o texto → `Aceptar`.
- Mover selección: `F5`/`F6` para mover á frase anterior/seguinte.
- Engadir: selecciona frase → (atallo: `F7`) `Engadir nova frase` → escribe o texto → `Aceptar`.
- Eliminar: selecciona frase → (atallo: `F8`) `Eliminar frase`; confirma.



#### Configurar ou cambiar a icona dunha frase
As frases seguen a mesma convención de prefixo de icona ca as categorías, co separador opcional `##` antes do texto da frase:

```
<icona>##<texto da frase>
```

- **Emoji**: `👍##Soa ben!`
- **Ficheiro de icona**: `C:\icons\check.ico##Soa ben!`
- **Sen icona**: `Soa ben!`

As frases tamén poden incluír un ficheiro de audio en liña. Ao combinar icona, texto e ficheiro de audio, o formato completo é:

```
<icona>##<texto da frase>::<ficheiro de audio>
```

- Exemplo: `🔔##Atención::notification.wav`

O prefixo `##` e o sufixo `::` son opcionais e independentes; podes usar calquera combinación:
- Só prefixo de icona: `🔔##Atención`
- Só sufixo de audio: `Atención::notification.wav`
- Ambos: `🔔##Atención::notification.wav`
- Ningún: `Atención`

## Configuración (F2)
Abre Configuración (`F2`) para controlar:
- **Texto predeterminado**: activar/desactivar `Inserir automaticamente o texto predeterminado` e personalizar a frase por defecto.
  - `Mostrar teclado táctil` — abre o teclado táctil en pantalla cando a caixa de entrada recibe o foco, posicionado preto da xanela principal.
- **Idioma da interface**: escoller idioma de UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voz**: seleccionar a voz SAPI instalada segundo a túa preferencia.
- **Volume/Velocidade**: axustar `Volume (10-100)` e `Velocidade (-10 a 10)`.
- `Probar voz`: probar a voz seleccionada.
- **Xanela de categorías**:
  - `Minimizar automaticamente a xanela de categorías` tras seleccionar.
  - `Lembrar o tamaño da xanela de categorías`.
- **Comportamento de reprodución**:
  - `Falar a frase inmediatamente ao seleccionala` (auto-reprodución ao facer clic).
  - `Deter a reprodución anterior` — detén automaticamente calquera reprodución en curso antes de iniciar unha nova frase.
  - `Aumentar o volume de SimonSays ao reproducir` — sobe o volume mestre do sistema mentres fala, restaurándoo cando remata a reprodución.
  - `Reducir temporalmente outros sons ao reproducir` — reduce o volume das demais aplicacións mentres fala, restaurándoo cando remata a reprodución.
- `Aceptar` garda cambios; `Cancelar` descarta.

## Mirada / Clic por fixación (F3)
Activa frases e bot)HELP" LR"HELP(óns sen usar as mans mantendo a mirada (ou o cursor do rato) sobre eles durante un tempo determinado. Esta función é nova: necesita probas e comentarios; por favor, cóntanos como funciona co teu seguidor ocular mediante a opción `Comentarios` do menú da bandexa.

Configuracións compatibles:
- **Calquera software de control ocular que mova o cursor do rato** (Irisbond EasyClick, Tobii Dynavox Computer Control, modos de cursor do Control ocular de Windows, OptiKey, ...): o clic por fixación segue o cursor.
- **Lectura directa da mirada, sen necesidade de mover o cursor**: Irisbond Hiru (modo HID) e seguidores Tobii a través do seu propio software instalado — verificado con Tobii Eye Tracker 4C e Tobii Dynavox PCEye 5; espérase que Tobii Eye Tracker 5 funcione, pero aínda non se probou.

Abre a configuración de Mirada / Clic por fixación (`F3`) para axustar (os controis deste diálogo móstranse en inglés):
- **Modo de activación**: elixe entre `Automatic`, que detecta a presenza e o comportamento do dispositivo e decide se activar o clic por fixación mediante o cursor do rato ou mediante detección da mirada (se se detecta un dispositivo de seguimento ocular ou software relacionado, o clic por fixación actívase; se non se detecta ningún, permanece desactivado), `Mouse cursor` para activar sempre o clic por fixación mediante o cursor, `HID eye tracker` para activalo sempre mediante detección da mirada, ou `Off` para desactivalo sempre, independentemente dos dispositivos presentes.
- **Tempo de fixación** (`Dwell time`): duración (en milisegundos) necesaria para disparar un clic ao mirar un elemento. Mentres a fixación está en curso móstrase un indicador de progreso.
- **Raio de tolerancia** (`Tolerance radius`): tolerancia da mirada (en píxeles) para permitir pequenos movementos do ollo sen cancelar o clic por fixación.
- **Tempo de espera** (`Cooldown`): milisegundos tras unha activación durante os cales non se disparará outro clic por fixación (evita clics repetidos accidentais).
- **Cor de progreso** (`Progress color`): cor do indicador de progreso da fixación (por defecto, a cor de énfase de Windows).
- **Detecta como usas o seguidor** (`Detect how you use the tracker`): dous botóns que axudan á aplicación a detectar se moves o cursor cun seguidor ocular ou se simplemente fas clic nos botóns.
- **Detectado** (`Detected`): mostra o tipo de dispositivo detectado e información adicional sobre o software relacionado.
- Un botón `Reset` (abaixo á esquerda) restaura todos os axustes de fixación aos seus valores predeterminados.
- O botón `Aceptar` garda os cambios; `Cancelar` descártaos.

## Menú da icona da bandexa
- `Mostrar` / `Ocultar` a interface principal.
- `Configuración`.
- `Gaze / Dwell-click` (Mirada / Clic por fixación).
- `Sobre` mostra a versión e os créditos.
- `Web` abre a páxina do proxecto (se está configurada).
- `Comentarios` abre un [formulario de comentarios](https://forms.gle/KMbpEDYmwnFJUhgy8) no navegador predeterminado.
- `Saír` pecha SimonSays.

Podes mover a xanela emerxente da bandexa e outros diálogos arrastrando a barra de título; redimensiona a xanela de Categorías arrastrando os bordes, e activa `Lembrar o tamaño da xanela de categorías` en Configuración para conservar o tamaño.

## Mesturar fala e sons
- Usa o delimitador `♫` (`SOUND_NOTE_DELIMITER`) para inserir nomes de arquivo de audio no texto:
  - Exemplo: `Ola ♫notification.wav♫, por favor espere.`
  - O texto fóra dos delimitadores fálase; o texto dentro trátase como arquivo de son e reprodúcese en liña.
- Audio soportado: `.wav`, `.mid`, `.midi`, `.mp3`.
- Usa rutas absolutas ou relativas accesibles ao proceso. Emparella delimitadores para evitar interpretar texto final como audio.



### Orde de busca de ficheiros de son e icona
Cando un nome de ficheiro de son ou icona non contén unha letra de unidade nin unha ruta completa, SimonSays búscao na seguinte orde e usa a primeira coincidencia atopada:

1. **Cartafol AppData** — `%APPDATA%\SimonSays\` (específico do usuario; coloca aquí os ficheiros para mantelos separados da instalación da aplicación).
2. **Directorio de traballo** — o directorio de traballo actual do proceso (só se busca se é diferente do cartafol do executábel).
3. **Cartafol do executábel** — o cartafol que contén `SimonSays.exe`.

Se o ficheiro non se atopa en ningunha destas localizacións, úsase o son de reserva integrado.

> **Consello:** recoméndase colocar os teus ficheiros de son e icona personalizados en `%APPDATA%\SimonSays\`; así sobrevivirán ás actualizacións e serán específicos do usuario.

## Importar e exportar categorías
- Exportar: podes exportar todas as categorías ou só a seleccionada cando se che pregunte. As mensaxes de éxito/erro aparecen no idioma seleccionado (atallo: `F10`).
- Importar: escolle arquivo; se a categoría existe, pediráselle sobrescribir. As mensaxes de éxito/erro aparecen no idioma seleccionado (atallo: `F9`).

## Almacenamento de valores por defecto
- As frases por defecto créanse por idioma na clave de rexistro `HKCU\SOFTWARE\SimonSays\Phrases\<Language>` no primeiro inicio.
- Para reinstalar valores por defecto dun idioma: pecha SimonSays → elimina a clave do idioma → reinicia SimonSays.

## Solución de problemas
- **Fallo de inicialización de voz**: verifica que SAPI estea instalado e que haxa unha voz compatible.
- **Non se reproducen sons**: confirma ruta/extensión e delimitadores `♫` emparellados.
- **Erros de importación/exportación**: revisa os permisos e a localización do disco; volve intentalo.
- **Idioma UI non aplicado**: reabre Configuración e confirma idioma; asegúrate de que existan cadeas de localización para o idioma elixido.

## Sobre
- O diálogo `Sobre` mostra versión, descrición e copyright.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"e" },
  { DWELL_DIALOG_TITLE_ID, L"Mirada / Clic por fixación" },
  { DWELL_INTRO_ID, L"Activa os botóns mantendo a mirada (ou o cursor) sobre eles." },
  { DWELL_MODE_GROUP_ID, L"Modo de activación" },
  { DWELL_MODE_AUTO_ID, L"Automático" },
  { DWELL_MODE_MOUSE_ID, L"Cursor do rato" },
  { DWELL_MODE_HID_ID, L"Seguidor ocular HID" },
  { DWELL_MODE_OFF_ID, L"Desactivado" },
  { DWELL_TIME_LABEL_ID, L"Tempo de fixación (ms)" },
  { DWELL_TOLERANCE_LABEL_ID, L"Raio de tolerancia (px)" },
  { DWELL_COOLDOWN_LABEL_ID, L"Tempo de espera (ms)" },
  { DWELL_COLOR_BUTTON_ID, L"Cor de progreso..." },
  { DWELL_DETECT_GROUP_ID, L"Detectar como usas o seguidor" },
  { DWELL_PROBE_LOOK_ID, L"Actívame MIRÁNDOME" },
  { DWELL_PROBE_MOUSE_ID, L"Actívame co RATO ou pulsador" },
  { DWELL_STATUS_INITIAL_ID, L"Proba os dous botóns de arriba para definir o mellor modo automaticamente." },
  { DWELL_STATUS_GAZE_ID, L"Detectado: cursor controlado pola mirada. Clic por fixación activado." },
  { DWELL_STATUS_MOUSE_ID, L"Detectado: rato manual. Clic por fixación desactivado." },
  { DWELL_STATUS_LOOK_HINT_ID, L"Por favor, MIRA o botón sen facer clic." },
  { DWELL_APPLY_BUTTON_ID, L"Aplicar" },
  { DWELL_HID_UNAVAILABLE_ID, L"O seguimento ocular HID aínda non está dispoñible." },
  { TRAYICON_DWELL_ID, L"Mirada / Clic por fixación..." },
  { DWELL_SIGNALS_GROUP_ID, L"Detectado" },
  { DWELL_SIGNAL_HID_LABEL_ID, L"Seguidor ocular (HID):" },
  { DWELL_SIGNAL_TOOL_LABEL_ID, L"App de control ocular:" },
  { DWELL_SIGNAL_WEC_LABEL_ID, L"Control ocular de Windows:" },
  { DWELL_SIGNAL_YES_ID, L"si" },
  { DWELL_SIGNAL_NO_ID, L"non" },
  { DWELL_SIGNAL_NONE_ID, L"ningunha" },
  { DWELL_SIGNAL_HID_LIVE_ID, L"transmitindo a mirada" },
  { DWELL_SIGNAL_HID_IDLE_ID, L"presente (sen transmitir)" },
  { DWELL_RESET_BUTTON_ID, L"Restablecer" },
  { BOARD_STYLE_CHANGED_TITLE_ID, L"Estilo do taboleiro cambiado" },
  { BOARD_STYLE_CHANGED_MESSAGE_ID, L"Queres reverter os cambios?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, L"Eliminar todas as categorías e as súas frases?\nAviso: esta acción non se pode desfacer." },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, L"É isto un erro?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, L"Eliminar todas as categorías" },
  { TRAYICON_FEEDBACK_ID, L"Dános a túa opinión" },
};

static const std::vector<std::pair<int, const wchar_t *>> GERMAN_LOCALIZED_UI_STRINGS = {
  { PLAY_BUTTON_TEXT_ID, L"Abspielen>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Kategorien" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Einstellungen" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Standardtext:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Standardtext automatisch einfügen" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Sprache:" },
  { SETTINGS_VOICE_LABEL_ID, L"Stimme:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Stimme testen" },
  { SETTINGS_VOLUME_LABEL_ID, L"Lautstärke (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Geschwindigkeit (-10 bis 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Satz bei Auswahl sofort aussprechen" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Größe des Kategorienfensters merken" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Kategorienfenster automatisch minimieren" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Lautstärke von SimonSays beim Abspielen erhöhen" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Andere Audios beim Abspielen vorübergehend reduzieren" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Vorherige Wiedergabe stoppen" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Bildschirmtastatur anzeigen" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Abbrechen" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Klicken zum Anzeigen" },
  { TRAYICON_SHOW_ID, L"Anzeigen" },
  { TRAYICON_HIDE_ID, L"Ausblenden" },
  { TRAYICON_SETTINGS_ID, L"Einstellungen" },
  { TRAYICON_ABOUT_ID, L"Über" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Beenden" },
  { ABOUT_TITTLE_ID, L"Über SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Einfach sprechen\n\nVersion " },
  { ABOUT_2_ID, L"\n\nEin einfaches Barrierefreiheits-Tool,\nmit dem Sie Text sofort in Sprache umwandeln können.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays-Fehler" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays unterstützt derzeit nur die untere Taskleiste" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Kategoriename ändern" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Satz bearbeiten" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Aktuell : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Abbrechen" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Eine Kategorie mit diesem Namen existiert bereits. Bitte wählen Sie einen anderen Namen." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Kategoriename Konflikt" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Kategorie löschen" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Kategorie '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' und alle ihre Sätze löschen?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Satz löschen" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Satz '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' löschen?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Neue Kategorie hinzufügen" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Neuen Satz hinzufügen" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Kategoriename :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Satztext :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Abbrechen" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Neuer Standardsatz" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Kategorien importieren" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays-Kategorien\0*.ssc;*.ssz\0Alle Dateien\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Kategorie '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' überschreiben?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Kategorie überschreiben?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Tafelstil ersetzen?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"Die importierte Datei enthält einen Tafelstil. Aktuellen Stil ersetzen?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Sätze erfolgreich importiert." },
  { IMPORT_SUCCESS_TITLE_ID, L"Import erfolgreich" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Sätze konnten nicht importiert werden." },
  { IMPORT_FAILURE_TITLE_ID, L"Import fehlgeschlagen" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Kategorien exportieren" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays-Kategorien\0*.ssc;*.ssz\0Alle Dateien\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Sie können alle Kategorien oder nur die ausgewählte exportieren.\nMöchten Sie nur die Kategorie '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' exportieren?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Auswahl exportieren" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Sätze erfolgreich exportiert." },
  { EXPORT_SUCCESS_TITLE_ID, L"Export erfolgreich" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Sätze konnten nicht exportiert werden." },
  { EXPORT_FAILURE_TITLE_ID, L"Export fehlgeschlagen" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Hilfe - F2: Einstellungen" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays Hilfe

## Übersicht
SimonSays ist ein Windows-Text-zu-Sprache-Assistent (TTS), erstellt von Juan Rey Saura, optimiert für schnelle Sprache, Phrasenbibliotheken und eingebettete Soundwiedergabe. Er unterstützt mehrere Sprachen, anpassbare Stimmen und benutzerfreundliche Funktionen für effiziente Kommunikation.

SimonSays nutzt den freien Platz links auf der Windows-Taskleiste und bietet schnellen Zugriff auf ein Dutzend Kategorien häufig verwendeter (und anpassbarer) Phrasen mit nur wenigen Klicks.

## Tastenkürzel
- `F1`: Hilfe öffnen.
- `F2`: Einstellungen öffnen.
- `F3`: Einstellungen für Blick- / Verweilklick.
- `F4`: Aktuelle Auswahl bearbeiten.
- `F5` / `F6`: Zur vorherigen / nächsten Auswahl in Listen.
- `F7`: Nach aktueller Auswahl hinzufügen (Kategorie-/Phrasenkontext).
- `F8`: Aktuelle Auswahl löschen.
- `F9`: Kategorien importieren.
- `F10`: Kategorien exportieren.
- `F11` / `Ctrl -`: Herauszoomen (Kategorienfenster).
- `F12` / `Ctrl +`: Hineinzoomen (Kategorienfenster).
- `Ctrl 0`: Zoom zurücksetzen.
- Schaltfläche `Abspielen>`: Wiedergabe starten.

## Start und Grundbedienung
1. Starten Sie SimonSays; es erscheint im Windows-Infobereich.
2. Zum Sprechen können Sie:
   - Benutzerdefinierten Text in das Eingabefeld eingeben und die Schaltfläche `Abspielen>` drücken (oder `Enter`).
   - Eine oder mehrere gespeicherte Phrasen im Kategorienfenster anklicken.
3. Während der Wiedergabe zeigt die Schaltfläche `>` an, dass die Wiedergabe aktiv ist. Sie können erneut drücken, um sofort zu stoppen.

## Kategorienfenster (Phrasen)
- Öffnen Sie es mit der Schaltfläche `Kategorien`. Größe ändern oder merken über Einstellungen.
- Wählen Sie eine Kategorie oben und Phrasen unten.
- Ein Klick auf eine Phrase spricht sie sofort, wenn `Satz bei Auswahl sofort aussprechen` aktiviert ist; andernfalls wird die Phrase nur ins Eingabefeld geladen.

### Kategorien verwalten
- Umbenennen: Kategorie auswählen → (Kürzel: `F4`) `Kategoriename ändern` → Namen ändern → `OK`.
- Auswahl verschieben: `F5`/`F6` für vorheriges/nächstes Element.
- Hinzufügen: Kategorie auswählen → (Kürzel: `F7`) Dialog `Neue Kategorie hinzufügen` → Namen eingeben → `OK`.
- Löschen: Kategorie auswählen → (Kürzel: `F8`) `Kategorie löschen`; Löschung von Kategorie und Phrasen bestätigen.



#### Kategorie-Symbol festlegen oder ändern
Jede Kategorie kann auf ihrer Schaltfläche ein Emoji oder eine benutzerdefinierte `.ico`-Datei anzeigen. Das Symbol wird im Dialog „Hinzufügen/Bearbeiten“ mit dem Trennpräfix `##` angegeben:

```
<symbol>##<kategoriename>
```

- **Emoji**: Emoji-Zeichen gefolgt von `##` und dem Namen eingeben.
  - Beispiel: `🙂##Grüße`
- **Symbol-Datei** (`.ico`): Dateipfad (absolut oder relativ zum SimonSays-Ordner) gefolgt von `##` und dem Namen eingeben.
  - Beispiel: `C:\icons\smile.ico##Grüße`
- **Ohne Symbol**: Nur den Kategorienamen ohne `##` eingeben.
  - Beispiel: `Grüße`

> Hinweis: Für dateibasierte Symbole werden nur `.ico`-Dateien unterstützt. Andere Dateiendungen werden ignoriert und es wird kein Symbol angezeigt.

### Phrasen verwalten
- Bearbeiten: Phrase auswählen → (Kürzel: `F4`) `Satz bearbeiten` → Text aktualisieren → `OK`.
- Auswahl verschieben: `F5`/`F6` für vorherige/nächste Phrase.
- Hinzufügen: Phrase auswählen → (Kürzel: `F7`) `Neuen Satz hinzufügen` → Text eingeben → `OK`.
- Löschen: Phrase auswählen → (Kürzel: `F8`) `Satz löschen`; bestätigen.



#### Phrasen-Symbol festlegen oder ändern
Phrasen verwenden dieselbe Symbolpräfix-Konvention wie Kategorien, mit dem optionalen Trennzeichen `##` vor dem Phrasentext:

```
<symbol>##<phrasentext>
```

- **Emoji**: `👍##Klingt gut!`
- **Symbol-Datei**: `C:\icons\check.ico##Klingt gut!`
- **Ohne Symbol**: `Klingt gut!`

Phrasen können auch eine eingebettete Audiodatei enthalten. Wenn Symbol, Text und Audiodatei kombiniert werden, lautet das vollständige Format:

```
<symbol>##<phrasentext>::<audiodatei>
```

- Beispiel: `🔔##Achtung::notification.wav`

Das Präfix `##` und das Suffix `::` sind optional und unabhängig:
- Nur Symbolpräfix: `🔔##Achtung`
- Nur Audiosuffix: `Achtung::notification.wav`
- Beides: `🔔##Achtung::notification.wav`
- Keines: `Achtung`

## Einstellungen (F2)
Öffnen Sie die Einstellungen (`F2`) um zu kontrollieren:
- **Standardtext**: `Standardtext automatisch einfügen` aktivieren/deaktivieren und Standardphrase anpassen.
  - `Bildschirmtastatur anzeigen` — öffnet die Bildschirmtastatur, wenn das Eingabefeld den Fokus erhält, in der Nähe des Hauptfensters positioniert.
- **Oberflächensprache**: UI-Sprache wählen (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Stimme**: Installierte SAPI-Stimme entsprechend Ihrer Präferenz auswählen.
- **Lautstärke/Geschwindigkeit**: `Lautstärke (10-100)` und `Geschwindigkeit (-10 bis 10)` anpassen.
- `Stimme testen`: Die ausgewählte Stimme anhören.
- **Kategorienfenster**:
  - `Kategorienfenster automatisch minimieren` nach Auswahl.
  - `Größe des Kategorienfensters merken`.
- **Wiedergabeverhalten**:
  - `Satz bei Auswahl sofort aussprechen` (Auto-Wiedergabe bei Klick).
  - `Vorherige Wiedergabe stoppen` — stoppt automatisch jede laufende Wiedergabe, bevor eine neue Phrase gestartet wird.
  - `Lautstärke von SimonSays beim Abspielen erhöhen` — erhöht die Systemlautstärke während des Sprechens und stellt sie nach Ende der Wiedergabe wieder her.
  - `Andere Audios beim Abspielen vorübergehend reduzieren` — reduziert die Lautstärke aller anderen Anwendungen während des Sprechens und stellt sie nach Ende der Wiedergabe wieder her.
- `OK` speichert Änderungen; `Abbrechen` verwirft)HELP" LR"HELP( sie.

## Blick- / Verweilklick (F3)
Aktivieren Sie Phrasen und Schaltflächen freihändig, indem Sie Ihren Blick (oder den Mauszeiger) für eine festgelegte Dauer darauf halten. Diese Funktion ist neu — sie braucht Tests und Feedback: Teilen Sie uns über die Option `Feedback` im Infobereich-Menü mit, wie sie mit Ihrem Eyetracker funktioniert.

Unterstützte Konfigurationen:
- **Jede Augensteuerungssoftware, die den Mauszeiger bewegt** (Irisbond EasyClick, Tobii Dynavox Computer Control, Cursor-Modi der Windows-Augensteuerung, OptiKey, ...): Der Verweilklick folgt dem Mauszeiger.
- **Direkte Blickerfassung, ohne Mausbewegung**: Irisbond Hiru (HID-Modus) und Tobii-Geräte über deren eigene installierte Software — verifiziert mit dem Tobii Eye Tracker 4C und dem Tobii Dynavox PCEye 5; der Tobii Eye Tracker 5 sollte funktionieren, wurde aber noch nicht getestet.

Öffnen Sie die Blick- / Verweilklick-Einstellungen (`F3`) zum Konfigurieren (die Steuerelemente dieses Dialogs werden auf Englisch angezeigt):
- **Aktivierungsmodus**: Wählen Sie zwischen `Automatic` — erkennt Vorhandensein und Verhalten des Geräts und entscheidet, ob der Verweilklick über den Mauszeiger oder über die Blickerfassung aktiviert wird (wird ein Eyetracker oder zugehörige Software erkannt, wird der Verweilklick aktiviert; wird nichts erkannt, bleibt er deaktiviert) —, `Mouse cursor`, um den Verweilklick immer über den Mauszeiger zu aktivieren, `HID eye tracker`, um ihn immer über die Blickerfassung zu aktivieren, oder `Off`, um ihn unabhängig von vorhandenen Geräten immer zu deaktivieren.
- **Verweilzeit** (`Dwell time`): Dauer (in Millisekunden), die nötig ist, um durch Ansehen eines Elements einen Klick auszulösen. Während des Verweilens wird ein Fortschrittsindikator angezeigt.
- **Toleranzradius** (`Tolerance radius`): Blicktoleranz (in Pixeln), die leichte Augenbewegungen erlaubt, ohne den Verweilklick abzubrechen.
- **Abklingzeit** (`Cooldown`): Millisekunden nach einer Aktivierung, in denen kein weiterer Verweilklick ausgelöst wird (verhindert versehentliche Wiederholungsklicks).
- **Fortschrittsfarbe** (`Progress color`): Farbe des Verweil-Fortschrittsindikators (standardmäßig die Windows-Akzentfarbe).
- **Erkennen, wie Sie den Tracker verwenden** (`Detect how you use the tracker`): Zwei Schaltflächen, die der App helfen zu erkennen, ob Sie den Mauszeiger mit einem Eyetracker bewegen oder einfach auf Schaltflächen klicken.
- **Erkannt** (`Detected`): Zeigt den erkannten Gerätetyp und weitere Informationen zur zugehörigen Software.
- Eine `Reset`-Schaltfläche (unten links) setzt alle Verweilklick-Einstellungen auf die Standardwerte zurück.
- Die Schaltfläche `OK` speichert Änderungen; `Abbrechen` verwirft sie.

## Infobereich-Menü
- `Anzeigen` / `Ausblenden` der Hauptoberfläche.
- `Einstellungen`.
- `Gaze / Dwell-click` (Blick- / Verweilklick).
- `Über` zeigt Version und Credits.
- `Web` öffnet die Projektseite (falls konfiguriert).
- `Feedback` öffnet ein [Feedback-Formular](https://forms.gle/KMbpEDYmwnFJUhgy8) im Standardbrowser.
- `Beenden` beendet SimonSays.

Sie können das Popup-Fenster des Infobereichs und andere Dialoge durch Ziehen der Titelleiste verschieben; ändern Sie die Größe des Kategorienfensters durch Ziehen der Ränder, und aktivieren Sie `Größe des Kategorienfensters merken` in den Einstellungen, um die Größe beizubehalten.

## Sprache und Sounds mischen
- Verwenden Sie den Begrenzer `♫` (`SOUND_NOTE_DELIMITER`), um Audiodateinamen in den Text einzubetten:
  - Beispiel: `Hallo ♫notification.wav♫, bitte warten.`
  - Text außerhalb der Begrenzer wird gesprochen; Text dazwischen wird als Sounddatei behandelt und inline abgespielt.
- Unterstützte Audioformate: `.wav`, `.mid`, `.midi`, `.mp3`.
- Verwenden Sie absolute oder relative Pfade, die für den Prozess zugänglich sind. Paaren Sie Begrenzer, um zu vermeiden, dass nachfolgender Text als Audio behandelt wird.



### Suchreihenfolge für Sound- und Symboldateien
Wenn ein Sound- oder Symbol-Dateiname keinen Laufwerksbuchstaben oder vollständigen Pfad enthält, sucht SimonSays in dieser Reihenfolge und verwendet den ersten Treffer:

1. **AppData-Ordner** — `%APPDATA%\SimonSays\` (benutzerspezifisch; Dateien hier ablegen, um sie von der Installation zu trennen).
2. **Arbeitsverzeichnis** — aktuelles Arbeitsverzeichnis des Prozesses (nur wenn es sich vom Ordner der EXE unterscheidet).
3. **Ausführungsordner** — der Ordner, der `SimonSays.exe` enthält.

Wird die Datei dort nicht gefunden, wird der integrierte Fallback-Sound verwendet.

> **Tipp:** Eigene Sound- und Symboldateien am besten in `%APPDATA%\SimonSays\` speichern; sie bleiben bei Updates erhalten und sind benutzerspezifisch.

## Kategorien importieren und exportieren
- Exportieren: Sie können alle Kategorien oder nur die ausgewählte exportieren. Erfolgs-/Fehlermeldungen erscheinen in der gewählten Sprache (Kürzel: `F10`).
- Importieren: Datei auswählen; falls die Kategorie existiert, werden Sie gefragt, ob Sie überschreiben möchten. Erfolgs-/Fehlermeldungen erscheinen in der gewählten Sprache (Kürzel: `F9`).

## Fehlerbehebung
- **Stimminitialisierung fehlgeschlagen**: Überprüfen Sie, ob SAPI installiert ist und eine kompatible Stimme vorhanden ist.
- **Keine Soundwiedergabe**: Bestätigen Sie Dateipfad/Erweiterung und gepaarte `♫`-Begrenzer.
- **Import-/Exportfehler**: Überprüfen Sie Berechtigungen und Speicherort; versuchen Sie es erneut.
- **UI-Sprache nicht angewandt**: Öffnen Sie die Einstellungen erneut und bestätigen Sie die Sprachauswahl; stellen Sie sicher, dass Lokalisierungszeichenfolgen für die gewählte Sprache vorhanden sind.

## Über
- Der Dialog `Über` zeigt Version, Beschreibung und Copyright an.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"und" },
  { DWELL_DIALOG_TITLE_ID, L"Blick / Verweilklick" },
  { DWELL_INTRO_ID, L"Aktivieren Sie Schaltflächen, indem Sie den Blick (oder den Mauszeiger) darauf halten." },
  { DWELL_MODE_GROUP_ID, L"Aktivierungsmodus" },
  { DWELL_MODE_AUTO_ID, L"Automatisch" },
  { DWELL_MODE_MOUSE_ID, L"Mauszeiger" },
  { DWELL_MODE_HID_ID, L"HID-Eyetracker" },
  { DWELL_MODE_OFF_ID, L"Aus" },
  { DWELL_TIME_LABEL_ID, L"Verweilzeit (ms)" },
  { DWELL_TOLERANCE_LABEL_ID, L"Toleranzradius (px)" },
  { DWELL_COOLDOWN_LABEL_ID, L"Abklingzeit (ms)" },
  { DWELL_COLOR_BUTTON_ID, L"Fortschrittsfarbe..." },
  { DWELL_DETECT_GROUP_ID, L"Erkennen, wie Sie den Tracker verwenden" },
  { DWELL_PROBE_LOOK_ID, L"Aktivieren Sie mich, indem Sie mich ANSEHEN" },
  { DWELL_PROBE_MOUSE_ID, L"Aktivieren Sie mich mit der MAUS oder einem Taster" },
  { DWELL_STATUS_INITIAL_ID, L"Probieren Sie die beiden Schaltflächen oben aus, um den besten Modus automatisch festzulegen." },
  { DWELL_STATUS_GAZE_ID, L"Erkannt: blickgesteuerter Zeiger. Verweilklick aktiviert." },
  { DWELL_STATUS_MOUSE_ID, L"Erkannt: manuelle Maus. Verweilklick deaktiviert." },
  { DWELL_STATUS_LOOK_HINT_ID, L"Bitte SCHAUEN Sie auf die Schaltfläche, ohne zu klicken." },
  { DWELL_APPLY_BUTTON_ID, L"Übernehmen" },
  { DWELL_HID_UNAVAILABLE_ID, L"HID-Eyetracking ist noch nicht verfügbar." },
  { TRAYICON_DWELL_ID, L"Blick / Verweilklick..." },
  { DWELL_SIGNALS_GROUP_ID, L"Erkannt" },
  { DWELL_SIGNAL_HID_LABEL_ID, L"Eyetracker (HID):" },
  { DWELL_SIGNAL_TOOL_LABEL_ID, L"Augensteuerungs-App:" },
  { DWELL_SIGNAL_WEC_LABEL_ID, L"Windows-Augensteuerung:" },
  { DWELL_SIGNAL_YES_ID, L"ja" },
  { DWELL_SIGNAL_NO_ID, L"nein" },
  { DWELL_SIGNAL_NONE_ID, L"keine" },
  { DWELL_SIGNAL_HID_LIVE_ID, L"Blickdaten werden gesendet" },
  { DWELL_SIGNAL_HID_IDLE_ID, L"vorhanden (keine Übertragung)" },
  { DWELL_RESET_BUTTON_ID, L"Zurücksetzen" },
  { BOARD_STYLE_CHANGED_TITLE_ID, L"Tafelstil geändert" },
  { BOARD_STYLE_CHANGED_MESSAGE_ID, L"Möchten Sie die Änderungen rückgängig machen?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, L"Alle Kategorien und ihre Sätze löschen?\nWarnung: Diese Aktion kann nicht rückgängig gemacht werden." },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, L"Ist dies ein Fehler?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, L"Alle Kategorien löschen" },
  { TRAYICON_FEEDBACK_ID, L"Feedback geben" },
};

static const std::vector<std::pair<int, const wchar_t *>> HEBREW_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"נגן>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"קטגוריות" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"הגדרות" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"טקסט ברירת מחדל:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"הכנס טקסט ברירת מחדל אוטומטית" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"שפה:" },
  { SETTINGS_VOICE_LABEL_ID, L"קול:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"בדוק קול" },
  { SETTINGS_VOLUME_LABEL_ID, L"עוצמה (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"מהירות (-10 עד 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"אמור את המשפט מיד בעת הבחירה" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"זכור את גודל חלון הקטגוריות" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"מזער את חלון הקטגוריות אוטומטית" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"הגדל את עוצמת SimonSays בזמן הנגינה" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"הנמך זמנית צלילים אחרים בזמן הנגינה" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"עצור ניגון קודם" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"הצג מקלדת מגע" },
  { SETTINGS_OK_BUTTON_ID, L"אישור" },
  { SETTINGS_CANCEL_BUTTON_ID, L"ביטול" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - לחץ כדי להציג" },
  { TRAYICON_SHOW_ID, L"הצג" },
  { TRAYICON_HIDE_ID, L"הסתר" },
  { TRAYICON_SETTINGS_ID, L"הגדרות" },
  { TRAYICON_ABOUT_ID, L"אודות" },
  { TRAYICON_WEB_ID, L"אתר" },
  { TRAYICON_EXIT_ID, L"יציאה" },
  { ABOUT_TITTLE_ID, L"אודות SimonSays" },
  { ABOUT_1_ID, L"SimonSays - פשוט לדבר\n\nגרסה " },
  { ABOUT_2_ID, L"\n\nכלי נגישות פשוט שהופך טקסט לדיבור מיד.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"שגיאת SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays תומך כרגע רק בשורת משימות תחתונה" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"שנה שם קטגוריה" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"ערוך משפט" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"נוכחי : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"אישור" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"ביטול" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"כבר קיימת קטגוריה עם שם זה. אנא בחר שם אחר." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"ניגוד שמות קטגוריות" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"הסר קטגוריה" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"האם להסיר את הקטגוריה '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' ואת כל המשפטים שלה?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"הסר משפט" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"האם להסיר את המשפט '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"הוסף קטגוריה חדשה" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"הוסף משפט חדש" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"שם קטגוריה :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"טקסט המשפט :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"אישור" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"ביטול" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"משפט ברירת מחדל חדש" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"ייבוא קטגוריות" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"לשכתב את הקטגוריה '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"לשכתב קטגוריה?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"להחליף את סגנון הלוח?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"הקובץ המיובא כולל סגנון לוח. להחליף את הסגנון הנוכחי?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"המשפטים יובאו בהצלחה." },
  { IMPORT_SUCCESS_TITLE_ID, L"ייבוא הצליח" },
  { IMPORT_FAILURE_MESSAGE_ID, L"כשל בייבוא המשפטים." },
  { IMPORT_FAILURE_TITLE_ID, L"ייבוא נכשל" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"ייצוא קטגוריות" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"תוכל לייצא את כל הקטגוריות או רק את הנבחרת.\nהאם לייצא רק את הקטגוריה '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"ייצוא בחירה" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"המשפטים יוצאו בהצלחה." },
  { EXPORT_SUCCESS_TITLE_ID, L"ייצוא הצליח" },
  { EXPORT_FAILURE_MESSAGE_ID, L"כשל בייצוא המשפטים." },
  { EXPORT_FAILURE_TITLE_ID, L"ייצוא נכשל" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: עזרה - F2: הגדרות" },
  { HELP_CONTENT_ID, LR"HELP(# מדריך עזרה של SimonSays

## סקירה
SimonSays הוא עוזר המרת טקסט לדיבור (TTS) עבור Windows, שנוצר על ידי Juan Rey Saura, ומותאם לדיבור מהיר, ספריות משפטים והשמעת צלילים בתוך הטקסט. הוא תומך במספר שפות, קולות הניתנים להתאמה אישית ותכונות ידידותיות למשתמש לתקשורת יעילה.

SimonSays משתמש בשטח הפנוי בצד שמאל של שורת המשימות של Windows, ומספק גישה מהירה לעשרות קטגוריות של משפטים בשימוש תכוף (וניתנים להתאמה) בכמה לחיצות בלבד.

## קיצורי מקלדת
- `F1`: פתיחת עזרה.
- `F2`: פתיחת הגדרות.
- `F3`: הגדרות מבט / לחיצת שהייה.
- `F4`: עריכת הבחירה הנוכחית.
- `F5` / `F6`: מעבר לבחירה הקודמת / הבאה ברשימות.
- `F7`: הוספה אחרי הבחירה הנוכחית (הקשר קטגוריה/משפט).
- `F8`: מחיקת הבחירה הנוכחית.
- `F9`: ייבוא קטגוריות.
- `F10`: ייצוא קטגוריות.
- `F11` / `Ctrl -`: הקטנת תצוגה (חלון הקטגוריות).
- `F12` / `Ctrl +`: הגדלת תצוגה (חלון הקטגוריות).
- `Ctrl 0`: איפוס תצוגה.
- כפתור `נגן>`: התחלת השמעה.

## הפעלה ושימוש בסיסי
1. הפעל את SimonSays; הוא מופיע באזור ההודעות של Windows.
2. כדי לדבר, אפשר:
   - להקליד טקסט מותאם אישית בתיבת הקלט וללחוץ על כפתור `נגן>` (או `Enter`).
   - ללחוץ על משפט שמור אחד או יותר בחלון הקטגוריות.
3. בזמן ההשמעה, הכפתור מציג `>` כדי לציין השמעה פעילה. ניתן ללחוץ עליו שוב כדי לעצור מיידית.

## חלון קטגוריות (משפטים)
- פתח דרך כפתור `קטגוריות`. שנה גודל או זכור את הגודל דרך ההגדרות.
- בחר קטגוריה למעלה ומשפטים למטה.
- לחיצה בודדת על משפט מנגנת אותו אוטומטית אם `אמור את המשפט מיד בעת הבחירה` מופעל; אחרת, המשפט נטען לתיבת הקלט הראשית ללא ניגון.

### ניהול קטגוריות
- שינוי שם: בחר קטגוריה → (קיצור: `F4`) `שנה שם קטגוריה` → שנה שם → `אישור`.
- הזזת בחירה: `F5`/`F6` לפריט הקודם/הבא.
- הוספה: בחר קטגוריה → (קיצור: `F7`) דו-שיח `הוסף קטגוריה חדשה` → הזן שם → `אישור`.
- מחיקה: בחר קטגוריה → (קיצור: `F8`) `הסר קטגוריה`; אשר מחיקת הקטגוריה וכל המשפטים שלה.



#### הגדרה או שינוי של סמל קטגוריה
כל קטגוריה יכולה להציג על הכפתור שלה אימוג'י או קובץ `.ico` מותאם אישית. את הסמל מגדירים בחלון הוספה/עריכה באמצעות הקידומת המפרידה `##`:

```
<icon>##<category name>
```

- **אימוג'י**: הזן את האימוג'י ולאחריו `##` ואז את השם.
  - דוגמה: `🙂##ברכות`
- **קובץ סמל** (`.ico`): הזן את נתיב הקובץ (מוחלט או יחסי לתיקיית SimonSays) ולאחריו `##` ואז את השם.
  - דוגמה: `C:\icons\smile.ico##ברכות`
- **ללא סמל**: הזן רק את שם הקטגוריה ללא הקידומת `##`.
  - דוגמה: `ברכות`

> הערה: עבור סמלים מבוססי קובץ נתמכים רק קבצי `.ico`. כל סיומת אחרת תתעלם ולא יוצג סמל.

### ניהול משפטים
- עריכה: בחר משפט → (קיצור: `F4`) `ערוך משפט` → עדכן טקסט → `אישור`.
- הזזת בחירה: `F5`/`F6` למשפט הקודם/הבא.
- הוספה: בחר משפט → (קיצור: `F7`) `הוסף משפט חדש` → הזן טקסט → `אישור`.
- מחיקה: בחר משפט → (קיצור: `F8`) `הסר משפט`; אשר.



#### הגדרה או שינוי של סמל משפט
משפטים משתמשים באותה שיטת קידומת סמל כמו קטגוריות, עם המפריד האופציונלי `##` לפני טקסט המשפט:

```
<icon>##<phrase text>
```

- **אימוג'י**: `👍##נשמע טוב!`
- **קובץ סמל**: `C:\icons\check.ico##נשמע טוב!`
- **ללא סמל**: `נשמע טוב!`

משפטים יכולים לכלול גם קובץ שמע משולב. כאשר משלבים סמל, טקסט וקובץ שמע, הפורמט המלא הוא:

```
<icon>##<phrase text>::<audio file>
```

- דוגמה: `🔔##שימו לב::notification.wav`

הקידומת `##` והסיומת `::` הן אופציונליות ובלתי תלויות:
- קידומת סמל בלבד: `🔔##שימו לב`
- סיומת שמע בלבד: `שימו לב::notification.wav`
- שתיהן יחד: `🔔##שימו לב::notification.wav`
- ללא שתיהן: `שימו לב`

## הגדרות (F2)
פתח הגדרות (`F2`) כדי לשלוט ב:
- **טקסט ברירת מחדל**: הפעל/השבת `הכנס טקסט ברירת מחדל אוטומטית` והתאם אישית את משפט ברירת המחדל.
  - `הצג מקלדת מגע` — פותח את מקלדת המגע על המסך כאשר תיבת הקלט מקבלת מיקוד, ממוקמת ליד החלון הראשי.
- **שפת ממשק**: בחר שפת UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **קול**: בחר קול SAPI מותקן בהתאם להעדפתך.
- **עוצמה/מהירות**: התאם `עוצמה (10-100)` ו`מהירות (-10 עד 10)`.
- `בדוק קול`: האזן לקול הנבחר.
- **חלון קטגוריות**:
  - `מזער את חלון הקטגוריות אוטומטית` לאחר בחירה.
  - `זכור את גודל חלון הקטגוריות`.
- **התנהגות השמעה**:
  - `אמור את המשפט מיד בעת הבחירה` (השמעה אוטומטית בלחיצה).
  - `עצור ניגון קודם` — עוצר אוטומטית כל השמעה פעילה לפני התחלת משפט חדש.
  - `הגדל את עוצמת SimonSays בזמן הנגינה` — מעלה את עוצמת הקול הראשית של המערכת בזמן הדיבור, ומשחזר אותה בסיו)HELP" LR"HELP(ם ההשמעה.
  - `הנמך זמנית צלילים אחרים בזמן הנגינה` — מנמיך את עוצמת הקול של כל היישומים האחרים בזמן הדיבור, ומשחזר אותה בסיום ההשמעה.
- `אישור` שומר שינויים; `ביטול` מבטל.

## מבט / לחיצת שהייה (F3)
הפעל משפטים וכפתורים ללא ידיים על ידי החזקת המבט (או סמן העכבר) עליהם למשך זמן מוגדר. תכונה זו חדשה — היא זקוקה לבדיקות ולמשוב: ספרו לנו איך היא עובדת עם מכשיר עקיבת העיניים שלכם דרך האפשרות `משוב` בתפריט אייקון ההודעות.

תצורות נתמכות:
- **כל תוכנת שליטה בעיניים שמזיזה את סמן העכבר** (Irisbond EasyClick, Tobii Dynavox Computer Control, מצבי הסמן של שליטה בעיניים של Windows, OptiKey, ...): לחיצת השהייה עוקבת אחרי הסמן.
- **קריאת מבט ישירה, ללא הזזת הסמן**: Irisbond Hiru (מצב HID) ומכשירי Tobii דרך התוכנה המותקנת שלהם — אומת עם Tobii Eye Tracker 4C ו-Tobii Dynavox PCEye 5; ‏Tobii Eye Tracker 5 צפוי לעבוד אך טרם נבדק.

פתח את הגדרות מבט / לחיצת שהייה (`F3`) כדי להגדיר (הפקדים בדו-שיח זה מוצגים באנגלית):
- **מצב הפעלה**: בחר בין `Automatic` — מזהה נוכחות והתנהגות של המכשיר ומחליט אם להפעיל לחיצת שהייה דרך סמן העכבר או דרך קריאת מבט (אם מזוהה מכשיר עקיבת עיניים או תוכנה קשורה, לחיצת השהייה מופעלת; אם לא מזוהה דבר, היא נשארת כבויה), `Mouse cursor` — להפעיל תמיד דרך הסמן, `HID eye tracker` — להפעיל תמיד דרך קריאת מבט, או `Off` — לכבות תמיד, ללא תלות במכשירים הקיימים.
- **זמן שהייה** (`Dwell time`): משך הזמן (במילישניות) הנדרש להפעלת לחיצה בעת הבטה באלמנט. במהלך השהייה מוצג מחוון התקדמות.
- **רדיוס סבילות** (`Tolerance radius`): סבילות המבט (בפיקסלים) המאפשרת תנועות עין קלות מבלי לבטל את לחיצת השהייה.
- **זמן המתנה** (`Cooldown`): מילישניות לאחר הפעלה שבמהלכן לא תופעל לחיצת שהייה נוספת (מונע לחיצות חוזרות בשוגג).
- **צבע התקדמות** (`Progress color`): צבע מחוון ההתקדמות של השהייה (ברירת המחדל היא צבע ההדגשה של Windows).
- **זיהוי אופן השימוש במכשיר** (`Detect how you use the tracker`): שני כפתורים שעוזרים לאפליקציה לזהות אם אתם מזיזים את הסמן עם מכשיר עקיבת עיניים או פשוט לוחצים על כפתורים.
- **זוהה** (`Detected`): מציג את סוג המכשיר שזוהה ומידע נוסף על תוכנות קשורות.
- כפתור `Reset` (למטה משמאל) מחזיר את כל הגדרות השהייה לערכי ברירת המחדל.
- כפתור `אישור` שומר שינויים; `ביטול` מבטל אותם.

## תפריט אייקון ההודעות
- `הצג` / `הסתר` ממשק ראשי.
- `הגדרות`.
- `Gaze / Dwell-click` (מבט / לחיצת שהייה).
- `אודות` מציג גרסה וקרדיטים.
- `אתר` פותח את דף הפרויקט (אם מוגדר).
- `משוב` פותח [טופס משוב](https://forms.gle/KMbpEDYmwnFJUhgy8) בדפדפן ברירת המחדל.
- `יציאה` סוגר את SimonSays.

ניתן להזיז את חלון ההודעות הקופץ ודיאלוגים אחרים על ידי גרירת שורת הכותרת; שנה את גודל חלון הקטגוריות על ידי גרירת הקצוות, והפעל `זכור את גודל חלון הקטגוריות` בהגדרות כדי לשמר את הגודל.

## שילוב דיבור וצלילים
- השתמש במפריד `♫` (`SOUND_NOTE_DELIMITER`) כדי להטמיע שמות קבצי שמע בטקסט:
  - דוגמה: `שלום ♫notification.wav♫, אנא המתן.`
  - טקסט מחוץ למפרידים מדובר; טקסט בין מפרידים מטופל כקובץ שמע ומושמע בתוך הזרם.
- פורמטים נתמכים: `.wav`, `.mid`, `.midi`, `.mp3`.
- השתמש בנתיבים מוחלטים או יחסיים נגישים לתהליך. התאם מפרידים בזוגות כדי למנוע טיפול בטקסט עוקב כשמע.



### סדר חיפוש קובצי צליל וסמל
כאשר שם של קובץ צליל או סמל אינו כולל אות כונן או נתיב מלא, SimonSays מחפש אותו לפי הסדר הבא ומשתמש בהתאמה הראשונה שנמצאת:

1. **תיקיית AppData** — `%APPDATA%\SimonSays\` (ספציפית למשתמש; מומלץ לשים כאן קבצים כדי להפרידם מהתקנת היישום).
2. **ספריית העבודה** — ספריית העבודה הנוכחית של התהליך (נבדקת רק אם היא שונה מתיקיית הקובץ המבצע).
3. **תיקיית הקובץ המבצע** — התיקייה שמכילה את `SimonSays.exe`.

אם הקובץ לא נמצא באף אחד מהמיקומים הללו, ייעשה שימוש בצליל הגיבוי המובנה.

> **טיפ:** מומלץ לשמור את קובצי הצליל והסמלים המותאמים אישית ב-`%APPDATA%\SimonSays\`; הם יישמרו גם לאחר עדכונים ויהיו ייחודיים למשתמש.

## ייבוא וייצוא קטגוריות
- ייצוא: ניתן לייצא את כל הקטגוריות או רק את הנבחרת כשתתבקש. הודעות הצלחה/כישלון מופיעות בשפה הנבחרת (קיצור: `F10`).
- ייבוא: בחר קובץ; אם הקטגוריה קיימת, תתבקש לשכתב. הודעות הצלחה/כישלון מופיעות בשפה הנבחרת (קיצור: `F9`).

## פתרון בעיות
- **כשל באתחול קול**: ודא ש-SAPI מותקן ושקול תואם קיים.
- **אין השמעת צלילים**: אשר נתיב/סיומת ומפרידי `♫` מזווגים.
- **שגיאות ייבוא/ייצוא**: בדוק הרשאות ומיקום דיסק; נסה שוב.
- **שפת UI לא הוחלה**: פתח מחדש הגדרות ואשר בחירת שפה; ודא שמחרוזות לוקליזציה קיימות לשפה הנבחרת.

## אודות
- דו-שיח `אודות` מציג גרסה, תיאור וזכויות יוצרים.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"ו" },
};

static const std::vector<std::pair<int, const wchar_t *>> HINDI_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"चलाएँ>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"श्रेणियाँ" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"सेटिंग्स" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"डिफ़ॉल्ट पाठ:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"डिफ़ॉल्ट पाठ स्वतः जोड़ें" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"भाषा:" },
  { SETTINGS_VOICE_LABEL_ID, L"आवाज़:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"आवाज़ परीक्षण" },
  { SETTINGS_VOLUME_LABEL_ID, L"वॉल्यूम (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"गति (-10 से 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"चयन करते ही वाक्य बोलें" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"श्रेणी विंडो का आकार याद रखें" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"श्रेणी विंडो को स्वतः छोटा करें" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"चलाते समय SimonSays की आवाज़ बढ़ाएँ" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"चलाते समय अन्य ऑडियो अस्थायी रूप से घटाएँ" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"पिछला प्लेबैक रोकें" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"टच कीबोर्ड दिखाएँ" },
  { SETTINGS_OK_BUTTON_ID, L"ठीक" },
  { SETTINGS_CANCEL_BUTTON_ID, L"रद्द करें" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - दिखाने के लिए क्लिक करें" },
  { TRAYICON_SHOW_ID, L"दिखाएँ" },
  { TRAYICON_HIDE_ID, L"छिपाएँ" },
  { TRAYICON_SETTINGS_ID, L"सेटिंग्स" },
  { TRAYICON_ABOUT_ID, L"परिचय" },
  { TRAYICON_WEB_ID, L"वेब" },
  { TRAYICON_EXIT_ID, L"बाहर निकलें" },
  { ABOUT_TITTLE_ID, L"SimonSays के बारे में" },
  { ABOUT_1_ID, L"SimonSays - बस बोलें\n\nसंस्करण " },
  { ABOUT_2_ID, L"\n\nएक सरल एक्सेसिबिलिटी टूल\nजो तुरंत पाठ को भाषण में बदलता है।\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays त्रुटि" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays वर्तमान में केवल निचली टास्कबार का समर्थन करता है" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"श्रेणी का नाम बदलें" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"वाक्य संपादित करें" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"वर्तमान : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"ठीक" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"रद्द करें" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"इस नाम की एक श्रेणी पहले से मौजूद है। कृपया एक अलग नाम चुनें।" },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"श्रेणी नाम संघर्ष" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"श्रेणी हटाएं" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"क्या आप श्रेणी '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' और इसके सभी वाक्य हटाना चाहते हैं?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"वाक्य हटाएं" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"क्या आप वाक्य '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' हटाना चाहते हैं?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"नई श्रेणी जोड़ें" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"नया वाक्य जोड़ें" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"श्रेणी का नाम :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"वाक्य का पाठ :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"ठीक" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"रद्द करें" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"नया डिफ़ॉल्ट वाक्य" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"श्रेणियाँ आयात करें" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"श्रेणी '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' अधिलेखित करें?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"श्रेणी अधिलेखित करें?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"बोर्ड शैली बदलें?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"आयात की गई फ़ाइल में बोर्ड शैली शामिल है। क्या वर्तमान शैली बदलें?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"वाक्य सफलतापूर्वक आयात किए गए।" },
  { IMPORT_SUCCESS_TITLE_ID, L"आयात सफल" },
  { IMPORT_FAILURE_MESSAGE_ID, L"वाक्य आयात करने में विफल रहा।" },
  { IMPORT_FAILURE_TITLE_ID, L"आयात असफल" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"श्रेणियाँ निर्यात करें" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"आप सभी श्रेणियाँ या केवल चयनित श्रेणी निर्यात कर सकते हैं।\nक्या आप केवल श्रेणी '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' निर्यात करना चाहते हैं?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"चयन निर्यात करें" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"वाक्य सफलतापूर्वक निर्यात किए गए।" },
  { EXPORT_SUCCESS_TITLE_ID, L"निर्यात सफल" },
  { EXPORT_FAILURE_MESSAGE_ID, L"वाक्य निर्यात करने में विफल रहा।" },
  { EXPORT_FAILURE_TITLE_ID, L"निर्यात असफल" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: सहायता - F2: सेटिंग्स" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays सहायता मार्गदर्शिका

## अवलोकन
SimonSays एक Windows टेक्स्ट-टू-स्पीच (TTS) सहायक है, जिसे Juan Rey Saura ने बनाया है, जो तेज़ बोलने, वाक्य पुस्तकालयों और इनलाइन साउंड प्लेबैक के लिए अनुकूलित है। यह कई भाषाओं, अनुकूलन योग्य आवाज़ों और कुशल संचार के लिए उपयोगकर्ता-अनुकूल सुविधाओं का समर्थन करता है।

SimonSays Windows टास्कबार के बाईं ओर खाली जगह का उपयोग करता है, जिससे बस कुछ ही क्लिक में अक्सर उपयोग किए जाने वाले (और अनुकूलन योग्य) वाक्यों की एक दर्जन श्रेणियों तक त्वरित पहुँच मिलती है।

## शॉर्टकट
- `F1`: सहायता खोलें।
- `F2`: सेटिंग्स खोलें।
- `F3`: दृष्टि / ड्वेल-क्लिक सेटिंग्स।
- `F4`: वर्तमान चयन संपादित करें।
- `F5` / `F6`: सूचियों में पिछले / अगले चयन पर जाएँ।
- `F7`: वर्तमान चयन के बाद जोड़ें (श्रेणी/वाक्य संदर्भ)।
- `F8`: वर्तमान चयन हटाएँ।
- `F9`: श्रेणियाँ आयात करें।
- `F10`: श्रेणियाँ निर्यात करें।
- `F11` / `Ctrl -`: ज़ूम आउट (श्रेणियाँ विंडो)।
- `F12` / `Ctrl +`: ज़ूम इन (श्रेणियाँ विंडो)।
- `Ctrl 0`: ज़ूम रीसेट करें।
- `चलाएँ>` बटन: प्लेबैक शुरू करें।

## प्रारंभ और मूल उपयोग
1. SimonSays शुरू करें; यह Windows सिस्टम ट्रे में दिखाई देता है।
2. बोलने के लिए, आप या तो:
   - इनपुट बॉक्स में कस्टम टेक्स्ट टाइप करें और `चलाएँ>` बटन दबाएँ (या `Enter`)।
   - श्रेणी विंडो में एक या अधिक सहेजे गए वाक्यों पर क्लिक करें।
3. प्लेबैक के दौरान, बटन `>` दिखाता है जो सक्रिय प्लेबैक को इंगित करता है। आप इसे फिर से दबाकर तुरंत रोक सकते हैं।

## श्रेणी विंडो (वाक्य)
- `श्रेणियाँ` बटन से खोलें। सेटिंग्स से आकार बदलें या याद रखें।
- ऊपर एक श्रेणी और नीचे वाक्य चुनें।
- एक वाक्य पर एकल क्लिक स्वचालित रूप से बोलता है यदि `चयन करते ही वाक्य बोलें` सक्षम है; अन्यथा, वाक्य मुख्य इनपुट बॉक्स में बिना बोले लोड होता है।

### श्रेणियाँ प्रबंधित करें
- नाम बदलें: श्रेणी चुनें → (शॉर्टकट: `F4`) `श्रेणी का नाम बदलें` → नाम बदलें → `ठीक`।
- चयन स्थानांतरित करें: `F5`/`F6` पिछले/अगले आइटम पर जाने के लिए।
- जोड़ें: श्रेणी चुनें → (शॉर्टकट: `F7`) `नई श्रेणी जोड़ें` संवाद → नाम दर्ज करें → `ठीक`।
- हटाएँ: श्रेणी चुनें → (शॉर्टकट: `F8`) `श्रेणी हटाएं`; श्रेणी और उसके सभी वाक्यों की हटाने की पुष्टि करें।



#### श्रेणी आइकन सेट या बदलें
हर श्रेणी अपने बटन पर एक इमोजी या एक कस्टम `.ico` फ़ाइल दिखा सकती है। आइकन को जोड़ें/संपादित करें संवाद में `##` विभाजक प्रीफ़िक्स का उपयोग करके निर्धारित किया जाता है:

```
<icon>##<category name>
```

- **इमोजी**: इमोजी लिखें, उसके बाद `##` और फिर नाम लिखें।  
  - उदाहरण: `🙂##अभिवादन`
- **आइकन फ़ाइल** (`.ico`): फ़ाइल पथ (पूर्ण या SimonSays फ़ोल्डर के सापेक्ष) लिखें, उसके बाद `##` और फिर नाम लिखें।  
  - उदाहरण: `C:\icons\smile.ico##अभिवादन`
- **बिना आइकन**: केवल श्रेणी का नाम लिखें, `##` प्रीफ़िक्स के बिना।  
  - उदाहरण: `अभिवादन`

> ध्यान दें: फ़ाइल-आधारित आइकन के लिए केवल `.ico` फ़ाइलें समर्थित हैं। किसी अन्य एक्सटेंशन को अनदेखा किया जाएगा और कोई आइकन नहीं दिखेगा।

### वाक्य प्रबंधित करें
- संपादित करें: वाक्य चुनें → (शॉर्टकट: `F4`) `वाक्य संपादित करें` → टेक्स्ट अपडेट करें → `ठीक`।
- चयन स्थानांतरित करें: `F5`/`F6` पिछले/अगले वाक्य पर जाने के लिए।
- जोड़ें: वाक्य चुनें → (शॉर्टकट: `F7`) `नया वाक्य जोड़ें` → टेक्स्ट दर्ज करें → `ठीक`।
- हटाएँ: वाक्य चुनें → (शॉर्टकट: `F8`) `वाक्य हटाएं`; पुष्टि करें।



#### वाक्य आइकन सेट या बदलें
वाक्य श्रेणियों जैसी ही आइकन प्रीफ़िक्स परंपरा का पालन करते हैं, जिसमें वैकल्पिक `##` विभाजक वाक्य पाठ से पहले आता है:

```
<icon>##<phrase text>
```

- **इमोजी**: `👍##अच्छा है!`
- **आइकन फ़ाइल**: `C:\icons\check.ico##अच्छा है!`
- **बिना आइकन**: `अच्छा है!`

वाक्यों में इनलाइन ऑडियो फ़ाइल भी शामिल हो सकती है। जब आइकन, पाठ और ऑडियो फ़ाइल को एक साथ जोड़ा जाता है, तो पूरा प्रारूप यह होता है:

```
<icon>##<phrase text>::<audio file>
```

- उदाहरण: `🔔##ध्यान दें::notification.wav`

`##` प्रीफ़िक्स और `::` सफ़िक्स दोनों वैकल्पिक और स्वतंत्र हैं; आप किसी भी संयोजन का उपयोग कर सकते हैं:
- केवल आइकन प्रीफ़िक्स: `🔔##ध्यान दें`
- केवल ऑडियो सफ़िक्स: `ध्यान दें::notification.wav`
- दोनों: `🔔##ध्यान दें::notification.wav`
- कोई नहीं: `ध्यान दें`

## सेटिंग्स (F2)
सेटिंग्स (`F2`) खोलें नियंत्रित करने के लिए:
- **डिफ़ॉल्ट पाठ**: `डिफ़ॉल्ट पाठ स्वतः जोड़ें` सक्षम/अक्षम करें और डिफ़ॉल्ट वाक्य अनुकूलित करें।
  - `टच कीबोर्ड दिखाएँ` — जब इनपुट बॉक्स फ़ोकस प्राप्त करता है तो ऑन-स्क्रीन टच कीबोर्ड खोलता है, मुख्य विंडो के पास स्थित।
- **इंटरफ़ेस भाषा**: UI भाषा चुनें (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian)।
- **आवाज़**: अपनी पसंद के अनुसार स्थापित SAPI आवाज़ चुनें।
- **वॉल्यूम/गति**: `वॉल्यूम (10-100)` और `गति (-10 से 10)` समायोजित करें।
- `आवाज़ परीक्षण`: चयनित आवाज़ का परीक्षण करें।
- **श्रेणी विंडो**:
  - `श्रेणी विंडो को स्वतः छोटा करें` चयन के बाद।
  - `श्रेणी विंडो का आकार याद रखें`।
- **प्लेबैक व्यवहार**:
  - `चयन करते ही वाक्य बोलें` (क्लिक पर ऑटो-प्लेबैक)।
  - `पिछला प्लेबैक रोकें` — नया वाक्य शुरू करने से पहले किसी भी चल रहे प्लेबैक को स्वचालित रूप से रोकता है।
  - `चलाते समय SimonSays की आवाज़ बढ़ाएँ` — बोलते समय सिस्टम मास्टर वॉल्यूम बढ़ाता है, प्लेबैक समाप्त होने पर पुनर्स्थापित करता है।
  - `चलाते समय अन्य ऑडियो अस्थायी रूप से घटाएँ` — बोलते समय अन्य सभी अनुप्रयोगों की आवाज़ कम करता है, प्लेबैक समाप्त होने पर पुनर्)HELP" LR"HELP(स्थापित करता है।
- `ठीक` परिवर्तन सहेजता है; `रद्द करें` छोड़ देता है।

## दृष्टि / ड्वेल-क्लिक (F3)
वाक्यों और बटनों को बिना हाथों के सक्रिय करें — उन पर अपनी दृष्टि (या माउस कर्सर) एक निर्धारित अवधि तक टिकाए रखें। यह सुविधा नई है — इसे परीक्षण और प्रतिक्रिया की आवश्यकता है: कृपया ट्रे मेन्यू के `प्रतिक्रिया` विकल्प से बताएं कि यह आपके आई-ट्रैकर के साथ कैसे काम करती है।

समर्थित कॉन्फ़िगरेशन:
- **कोई भी आई-कंट्रोल सॉफ़्टवेयर जो माउस कर्सर हिलाता है** (Irisbond EasyClick, Tobii Dynavox Computer Control, Windows आई कंट्रोल के कर्सर मोड, OptiKey, ...): ड्वेल-क्लिक कर्सर का अनुसरण करता है।
- **सीधी दृष्टि रीडिंग, कर्सर हिलाए बिना**: Irisbond Hiru (HID मोड) और Tobii ट्रैकर अपने स्वयं के इंस्टॉल किए गए सॉफ़्टवेयर के माध्यम से — Tobii Eye Tracker 4C और Tobii Dynavox PCEye 5 के साथ सत्यापित; Tobii Eye Tracker 5 के काम करने की उम्मीद है, लेकिन अभी परीक्षण नहीं हुआ है।

दृष्टि / ड्वेल-क्लिक सेटिंग्स (`F3`) खोलें कॉन्फ़िगर करने के लिए (इस संवाद के नियंत्रण अंग्रेज़ी में दिखते हैं):
- **सक्रियण मोड**: `Automatic` चुनें — यह डिवाइस की उपस्थिति और व्यवहार का पता लगाता है और तय करता है कि ड्वेल-क्लिक माउस कर्सर से सक्रिय हो या दृष्टि पहचान से (यदि कोई आई-ट्रैकिंग डिवाइस या संबंधित सॉफ़्टवेयर मिलता है, तो ड्वेल-क्लिक सक्रिय होता है; कुछ न मिलने पर बंद रहता है); `Mouse cursor` — हमेशा कर्सर से सक्रिय करें; `HID eye tracker` — हमेशा दृष्टि पहचान से सक्रिय करें; या `Off` — डिवाइस की उपस्थिति की परवाह किए बिना हमेशा बंद रखें।
- **ड्वेल समय** (`Dwell time`): किसी तत्व को देखते समय क्लिक ट्रिगर करने के लिए आवश्यक अवधि (मिलीसेकंड में)। ड्वेल के दौरान एक प्रगति संकेतक दिखता है।
- **सहनशीलता त्रिज्या** (`Tolerance radius`): दृष्टि की सहनशीलता (पिक्सेल में), ताकि आँखों की हल्की हरकतों से ड्वेल-क्लिक रद्द न हो।
- **प्रतीक्षा समय** (`Cooldown`): सक्रियण के बाद के मिलीसेकंड जिनके दौरान कोई और ड्वेल-क्लिक ट्रिगर नहीं होगा (आकस्मिक दोहराए क्लिक रोकता है)।
- **प्रगति रंग** (`Progress color`): ड्वेल प्रगति संकेतक का रंग (डिफ़ॉल्ट रूप से Windows का एक्सेंट रंग)।
- **पता लगाएँ कि आप ट्रैकर कैसे उपयोग करते हैं** (`Detect how you use the tracker`): दो बटन जो ऐप को यह पता लगाने में मदद करते हैं कि आप कर्सर आई-ट्रैकर से हिलाते हैं या बस बटनों पर क्लिक करते हैं।
- **पता चला** (`Detected`): पता चले डिवाइस प्रकार और संबंधित सॉफ़्टवेयर की अतिरिक्त जानकारी दिखाता है।
- एक `Reset` बटन (नीचे बाएँ) सभी ड्वेल सेटिंग्स को डिफ़ॉल्ट मानों पर लौटाता है।
- `ठीक` बटन परिवर्तन सहेजता है; `रद्द करें` उन्हें छोड़ देता है।

## ट्रे आइकन मेन्यू
- `दिखाएँ` / `छिपाएँ` मुख्य UI।
- `सेटिंग्स`।
- `Gaze / Dwell-click` (दृष्टि / ड्वेल-क्लिक)।
- `परिचय` संस्करण और क्रेडिट दिखाता है।
- `वेब` प्रोजेक्ट पेज खोलता है (यदि कॉन्फ़िगर किया गया हो)।
- `प्रतिक्रिया` डिफ़ॉल्ट ब्राउज़र में [प्रतिक्रिया फ़ॉर्म](https://forms.gle/KMbpEDYmwnFJUhgy8) खोलती है।
- `बाहर निकलें` SimonSays बंद करता है。

आप शीर्षक पट्टी खींचकर ट्रे पॉप-अप विंडो और अन्य संवादों को स्थानांतरित कर सकते हैं; किनारों को खींचकर श्रेणी विंडो का आकार बदलें, और आकार बनाए रखने के लिए सेटिंग्स में `श्रेणी विंडो का आकार याद रखें` सक्षम करें।

## वाक् और ध्वनियाँ मिलाना
- टेक्स्ट में ऑडियो फ़ाइल नाम एम्बेड करने के लिए सीमांकक `♫` (`SOUND_NOTE_DELIMITER`) का उपयोग करें:
  - उदाहरण: `नमस्ते ♫notification.wav♫, कृपया प्रतीक्षा करें।`
  - सीमांककों के बाहर का टेक्स्ट बोला जाता है; बीच का टेक्स्ट ध्वनि फ़ाइल के रूप में माना जाता है और इनलाइन चलाया जाता है।
- समर्थित ऑडियो: `.wav`, `.mid`, `.midi`, `.mp3`।
- प्रक्रिया के लिए सुलभ पूर्ण या सापेक्ष पथों का उपयोग करें। अनुगामी टेक्स्ट को ऑडियो के रूप में माने जाने से बचने के लिए सीमांककों को जोड़ी में रखें।



### ध्वनि और आइकन फ़ाइल खोज क्रम
जब किसी ध्वनि या आइकन फ़ाइल नाम में ड्राइव लेटर या पूरा पथ नहीं होता, तो SimonSays उसे निम्न क्रम में खोजता है और पहली मिली हुई फ़ाइल का उपयोग करता है:

1. **AppData फ़ोल्डर** — `%APPDATA%\SimonSays\` (उपयोगकर्ता-विशिष्ट; फ़ाइलों को एप्लिकेशन इंस्टॉलेशन से अलग रखने के लिए इन्हें यहाँ रखें)।
2. **वर्किंग डायरेक्टरी** — प्रक्रिया की वर्तमान वर्किंग डायरेक्टरी (केवल तब खोजी जाती है जब यह executable फ़ोल्डर से अलग हो)।
3. **Executable फ़ोल्डर** — वह फ़ोल्डर जिसमें `SimonSays.exe` मौजूद है।

यदि फ़ाइल इन स्थानों में नहीं मिलती, तो अंतर्निहित fallback ध्वनि का उपयोग किया जाता है।

> **सुझाव:** अपने कस्टम ध्वनि और आइकन फ़ाइलों को `%APPDATA%\SimonSays\` में रखना बेहतर है; वे अपडेट के बाद भी बनी रहेंगी और उपयोगकर्ता-विशिष्ट होंगी।

## श्रेणियाँ आयात और निर्यात करें
- निर्यात: पूछे जाने पर सभी श्रेणियाँ या केवल चयनित श्रेणी निर्यात कर सकते हैं। सफलता/विफलता संदेश चयनित भाषा में दिखाई देते हैं (शॉर्टकट: `F10`)।
- आयात: फ़ाइल चुनें; यदि श्रेणी मौजूद है, तो अधिलेखित करने के लिए कहा जाएगा। सफलता/विफलता संदेश चयनित भाषा में दिखाई देते हैं (शॉर्टकट: `F9`)।

## समस्या निवारण
- **आवाज़ आरंभीकरण विफल**: सत्यापित करें कि SAPI स्थापित है और एक संगत आवाज़ उपलब्ध है।
- **कोई ध्वनि प्लेबैक नहीं**: फ़ाइल पथ/एक्सटेंशन और जोड़ीदार `♫` सीमांककों की पुष्टि करें।
- **आयात/निर्यात त्रुटियाँ**: अनुमतियाँ और डिस्क स्थान जाँचें; पुनः प्रयास करें।
- **UI भाषा लागू नहीं हुई**: सेटिंग्स दोबारा खोलें और भाषा चयन की पुष्टि करें; सुनिश्चित करें कि चुनी गई भाषा के लिए स्थानीयकरण स्ट्रिंग मौजूद हैं।

## परिचय
- `परिचय` संवाद संस्करण, विवरण और कॉपीराइट दिखाता है।
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"और" },
};

static const std::vector<std::pair<int, const wchar_t *>> ITALIAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Riproduci>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorie" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Impostazioni" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Testo predefinito:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Inserisci automaticamente il testo predefinito" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Lingua:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voce:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Prova voce" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocità (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Pronuncia subito la frase quando la selezioni" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Ricorda la dimensione della finestra delle categorie" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Riduci automaticamente la finestra delle categorie" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumenta il volume di SimonSays durante la riproduzione" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Riduci temporaneamente gli altri audio durante la riproduzione" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Ferma la riproduzione precedente" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Mostra la tastiera touch" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Annulla" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Clic per mostrare" },
  { TRAYICON_SHOW_ID, L"Mostra" },
  { TRAYICON_HIDE_ID, L"Nascondi" },
  { TRAYICON_SETTINGS_ID, L"Impostazioni" },
  { TRAYICON_ABOUT_ID, L"Informazioni" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Esci" },
  { ABOUT_TITTLE_ID, L"Informazioni su SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Parla semplicemente\n\nVersione " },
  { ABOUT_2_ID, L"\n\nUno strumento di accessibilità semplice\nche trasforma il testo in voce all'istante.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Errore di SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays attualmente supporta solo la barra delle applicazioni inferiore" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Cambia nome categoria" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Modifica frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Attuale : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Annulla" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Esiste già una categoria con questo nome. Per favore, scegli un nome diverso." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflitto nomi categoria" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Elimina categoria" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Eliminare la categoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' e tutte le sue frasi?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Elimina frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Eliminare la frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Aggiungi nuova categoria" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Aggiungi nuova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nome categoria :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Testo della frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Annulla" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nuova frase predefinita" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importa categorie" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categorie SimonSays\0*.ssc;*.ssz\0Tutti i file\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sovrascrivere la categoria '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sovrascrivere la categoria?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Sostituire lo stile del tabellone?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"Il file importato include uno stile del tabellone. Sostituire quello attuale?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frasi importate con successo." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importazione riuscita" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Impossibile importare le frasi." },
  { IMPORT_FAILURE_TITLE_ID, L"Importazione non riuscita" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Esporta categorie" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categorie SimonSays\0*.ssc;*.ssz\0Tutti i file\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Puoi esportare tutte le categorie o solo quella selezionata.\nVuoi esportare solo la categoria '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Esporta selezione" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frasi esportate con successo." },
  { EXPORT_SUCCESS_TITLE_ID, L"Esportazione riuscita" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Impossibile esportare le frasi." },
  { EXPORT_FAILURE_TITLE_ID, L"Esportazione non riuscita" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Aiuto - F2: Impostazioni" },
  { HELP_CONTENT_ID, LR"HELP(# Guida di SimonSays

## Panoramica
SimonSays è un assistente Windows di sintesi vocale (TTS), creato da Juan Rey Saura, ottimizzato per comunicazione rapida, librerie di frasi e riproduzione di suoni in linea. Supporta più lingue, voci personalizzabili e funzionalità intuitive per una comunicazione efficiente.

SimonSays utilizza lo spazio libero a sinistra della barra delle applicazioni di Windows, offrendo accesso rapido a una dozzina di categorie di frasi di uso frequente (e personalizzabili) con pochi clic.

## Scorciatoie
- `F1`: Apri Aiuto.
- `F2`: Apri Impostazioni.
- `F3`: Impostazioni Sguardo / Clic per fissazione.
- `F4`: Modifica la selezione corrente.
- `F5` / `F6`: Sposta alla selezione precedente / successiva nelle liste.
- `F7`: Aggiungi dopo la selezione corrente (contesto categoria/frase).
- `F8`: Elimina la selezione corrente.
- `F9`: Importa categorie.
- `F10`: Esporta categorie.
- `F11` / `Ctrl -`: Riduci zoom (finestra delle categorie).
- `F12` / `Ctrl +`: Aumenta zoom (finestra delle categorie).
- `Ctrl 0`: Reimposta zoom.
- Pulsante `Riproduci>`: Avvia riproduzione.

## Avvio e uso base
1. Avvia SimonSays; appare nell'area di notifica di Windows.
2. Per parlare, puoi:
   - Digitare testo personalizzato nella casella di input e premere il pulsante `Riproduci>` (o `Enter`).
   - Fare clic su una o più frasi salvate nella finestra Categorie.
3. Durante la riproduzione, il pulsante mostra `>` per indicare lo stato attivo. Puoi premerlo di nuovo per fermare istantaneamente.

## Finestra Categorie (frasi)
- Aprila con il pulsante `Categorie`. Ridimensiona o ricorda la dimensione tramite Impostazioni.
- Seleziona una categoria in alto e le frasi in basso.
- Un clic su una frase la riproduce automaticamente se `Pronuncia subito la frase quando la selezioni` è attivato; altrimenti, la frase viene caricata nella casella di input principale senza essere pronunciata.

### Gestione categorie
- Rinomina: seleziona categoria → (scorciatoia: `F4`) `Cambia nome categoria` → cambia nome → `OK`.
- Sposta selezione: `F5`/`F6` per elemento precedente/successivo.
- Aggiungi: seleziona categoria → (scorciatoia: `F7`) dialogo `Aggiungi nuova categoria` → inserisci nome → `OK`.
- Elimina: seleziona categoria → (scorciatoia: `F8`) `Elimina categoria`; conferma eliminazione della categoria e delle sue frasi.



#### Impostare o cambiare l’icona di una categoria
Ogni categoria può mostrare sul proprio pulsante un’emoji o un file `.ico` personalizzato. L’icona viene specificata nella finestra Aggiungi/Modifica usando il prefisso separatore `##`:

```
<icona>##<nome categoria>
```

- **Emoji**: inserisci l’emoji seguita da `##` e dal nome.
  - Esempio: `🙂##Saluti`
- **File icona** (`.ico`): inserisci il percorso del file (assoluto oppure relativo alla cartella di SimonSays) seguito da `##` e dal nome.
  - Esempio: `C:\icons\smile.ico##Saluti`
- **Senza icona**: inserisci solo il nome della categoria senza il prefisso `##`.
  - Esempio: `Saluti`

> Nota: per le icone basate su file sono supportati solo i file `.ico`. Qualsiasi altra estensione viene ignorata e non viene mostrata alcuna icona.

### Gestione frasi
- Modifica: seleziona frase → (scorciatoia: `F4`) `Modifica frase` → aggiorna testo → `OK`.
- Sposta selezione: `F5`/`F6` per frase precedente/successiva.
- Aggiungi: seleziona frase → (scorciatoia: `F7`) `Aggiungi nuova frase` → inserisci testo → `OK`.
- Elimina: seleziona frase → (scorciatoia: `F8`) `Elimina frase`; conferma.



#### Impostare o cambiare l’icona di una frase
Le frasi seguono la stessa convenzione di prefisso delle categorie, con il separatore opzionale `##` prima del testo della frase:

```
<icona>##<testo frase>
```

- **Emoji**: `👍##Va bene!`
- **File icona**: `C:\icons\check.ico##Va bene!`
- **Senza icona**: `Va bene!`

Le frasi possono anche includere un file audio in linea. Quando combini icona, testo e file audio, il formato completo è:

```
<icona>##<testo frase>::<file audio>
```

- Esempio: `🔔##Attenzione::notification.wav`

Il prefisso `##` e il suffisso `::` sono entrambi opzionali e indipendenti:
- Solo prefisso icona: `🔔##Attenzione`
- Solo suffisso audio: `Attenzione::notification.wav`
- Entrambi: `🔔##Attenzione::notification.wav`
- Nessuno: `Attenzione`

## Impostazioni (F2)
Apri Impostazioni (`F2`) per controllare:
- **Testo predefinito**: attiva/disattiva `Inserisci automaticamente il testo predefinito` e personalizza la frase predefinita.
  - `Mostra la tastiera touch` — apre la tastiera touch su schermo quando la casella di input riceve il focus, posizionata vicino alla finestra principale.
- **Lingua dell'interfaccia**: scegli la lingua UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voce**: seleziona una voce SAPI installata in base alla tua preferenza.
- **Volume/Velocità**: regola `Volume (10-100)` e `Velocità (-10 a 10)`.
- `Prova voce`: ascolta la voce selezionata.
- **Finestra categorie**:
  - `Riduci automaticamente la finestra delle categorie` dopo la selezione.
  - `Ricorda la dimensione della finestra delle categorie`.
- **Comportamento riproduzione**:
  - `Pronuncia subito la frase quando la selezioni` (riproduzione automatica al clic).
  - `Ferma la riproduzione precedente` — ferma automaticamente qualsiasi riproduzione in corso prima di avviare una nuova frase.
  - `Aumenta il volume di SimonSays durante la riproduzione` — alza il volume master del sistema durante la riproduzione, ripristinandolo al termine.
  - `Riduci temporaneamente gli altri audio durante la riproduzione` — riduce il volume di tutte le altre applicazioni durante la riproduzione, ripristinandolo al termine.
- `OK` salva le modifiche; `Annulla` le scarta.

## Sguardo / Clic per fissazione (F3)
Attiva frasi e pu)HELP" LR"HELP(lsanti a mani libere mantenendo lo sguardo (o il cursore del mouse) su di essi per una durata impostata. Questa funzionalità è nuova — ha bisogno di test e feedback: raccontaci come funziona con il tuo dispositivo di tracciamento oculare tramite l'opzione `Feedback` del menu nell'area di notifica.

Configurazioni supportate:
- **Qualsiasi software di controllo oculare che muove il cursore del mouse** (Irisbond EasyClick, Tobii Dynavox Computer Control, modalità cursore del Controllo oculare di Windows, OptiKey, ...): il clic per fissazione segue il cursore.
- **Lettura diretta dello sguardo, senza spostamento del cursore**: Irisbond Hiru (modalità HID) e dispositivi Tobii tramite il loro software installato — verificato con Tobii Eye Tracker 4C e Tobii Dynavox PCEye 5; il Tobii Eye Tracker 5 dovrebbe funzionare, ma non è ancora stato testato.

Apri le impostazioni Sguardo / Clic per fissazione (`F3`) per configurare (i controlli di questa finestra sono visualizzati in inglese):
- **Modalità di attivazione**: scegli tra `Automatic`, che rileva la presenza e il comportamento del dispositivo e decide se attivare il clic per fissazione tramite il cursore del mouse o tramite il rilevamento dello sguardo (se viene rilevato un dispositivo di tracciamento oculare o un software correlato, il clic per fissazione viene attivato; se non ne viene rilevato nessuno, rimane disattivato), `Mouse cursor` per attivare sempre il clic per fissazione tramite il cursore, `HID eye tracker` per attivarlo sempre tramite il rilevamento dello sguardo, oppure `Off` per disattivarlo sempre, indipendentemente dai dispositivi presenti.
- **Tempo di fissazione** (`Dwell time`): durata (in millisecondi) necessaria per attivare un clic guardando un elemento. Durante la fissazione viene mostrato un indicatore di avanzamento.
- **Raggio di tolleranza** (`Tolerance radius`): tolleranza dello sguardo (in pixel) per consentire piccoli movimenti degli occhi senza annullare il clic per fissazione.
- **Tempo di attesa** (`Cooldown`): millisecondi dopo un'attivazione durante i quali non verrà attivato un altro clic per fissazione (evita clic ripetuti accidentali).
- **Colore di avanzamento** (`Progress color`): colore dell'indicatore di avanzamento della fissazione (per impostazione predefinita, il colore di evidenziazione di Windows).
- **Rileva come usi il dispositivo** (`Detect how you use the tracker`): due pulsanti che aiutano l'applicazione a rilevare se muovi il cursore con un dispositivo di tracciamento oculare o se fai semplicemente clic sui pulsanti.
- **Rilevato** (`Detected`): mostra il tipo di dispositivo rilevato e ulteriori informazioni sul software correlato.
- Un pulsante `Reset` (in basso a sinistra) ripristina tutte le impostazioni di fissazione ai valori predefiniti.
- Il pulsante `OK` salva le modifiche; `Annulla` le scarta.

## Menu dell'icona nell'area di notifica
- `Mostra` / `Nascondi` interfaccia principale.
- `Impostazioni`.
- `Gaze / Dwell-click` (Sguardo / Clic per fissazione).
- `Informazioni` mostra versione e crediti.
- `Web` apre la pagina del progetto (se configurata).
- `Feedback` apre un [modulo di feedback](https://forms.gle/KMbpEDYmwnFJUhgy8) nel browser predefinito.
- `Esci` chiude SimonSays.

Puoi spostare la finestra popup dell'area di notifica e altri dialoghi trascinando la barra del titolo; ridimensiona la finestra Categorie trascinando i bordi, e attiva `Ricorda la dimensione della finestra delle categorie` nelle Impostazioni per mantenere la dimensione.

## Mescolare voce e suoni
- Usa il delimitatore `♫` (`SOUND_NOTE_DELIMITER`) per inserire nomi di file audio nel testo:
  - Esempio: `Ciao ♫notification.wav♫, attendere prego.`
  - Il testo fuori dai delimitatori viene pronunciato; il testo tra i delimitatori viene trattato come file audio e riprodotto in linea.
- Formati audio supportati: `.wav`, `.mid`, `.midi`, `.mp3`.
- Usa percorsi assoluti o relativi accessibili al processo. Accoppia i delimitatori per evitare che il testo finale venga trattato come audio.



### Ordine di ricerca dei file audio e icona
Quando un nome di file audio o icona non contiene una lettera di unità o un percorso completo, SimonSays lo cerca nel seguente ordine, usando la prima corrispondenza trovata:

1. **Cartella AppData** — `%APPDATA%\SimonSays\` (specifica dell’utente; inserisci qui i file per tenerli separati dall’installazione dell’applicazione).
2. **Directory di lavoro** — la directory di lavoro corrente del processo (cercata solo se è diversa dalla cartella dell’eseguibile).
3. **Cartella dell’eseguibile** — la cartella che contiene `SimonSays.exe`.

Se il file non viene trovato in nessuna di queste posizioni, viene usato il suono di riserva integrato.

> **Suggerimento:** è consigliato salvare i file audio e le icone personalizzati in `%APPDATA%\SimonSays\`; resisteranno agli aggiornamenti e resteranno specifici dell’utente.

## Importare ed esportare categorie
- Esportare: puoi esportare tutte le categorie o solo quella selezionata quando richiesto. I messaggi di successo/errore appaiono nella lingua selezionata (scorciatoia: `F10`).
- Importare: scegli un file; se la categoria esiste, ti verrà chiesto di sovrascrivere. I messaggi di successo/errore appaiono nella lingua selezionata (scorciatoia: `F9`).

## Risoluzione problemi
- **Inizializzazione voce fallita**: verifica che SAPI sia installato e che sia presente una voce compatibile.
- **Nessuna riproduzione audio**: conferma percorso/estensione e delimitatori `♫` accoppiati.
- **Errori di importazione/esportazione**: controlla permessi e posizione del disco; riprova.
- **Lingua UI non applicata**: riapri Impostazioni e conferma la selezione della lingua; assicurati che esistano stringhe di localizzazione per la lingua scelta.

## Informazioni
- Il dialogo `Informazioni` mostra versione, descrizione e copyright.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"e" },
  { DWELL_DIALOG_TITLE_ID, L"Sguardo / Clic a fissazione" },
  { DWELL_INTRO_ID, L"Attiva i pulsanti mantenendo lo sguardo (o il cursore) su di essi." },
  { DWELL_MODE_GROUP_ID, L"Modalità di attivazione" },
  { DWELL_MODE_AUTO_ID, L"Automatico" },
  { DWELL_MODE_MOUSE_ID, L"Cursore del mouse" },
  { DWELL_MODE_HID_ID, L"Puntatore oculare HID" },
  { DWELL_MODE_OFF_ID, L"Disattivato" },
  { DWELL_TIME_LABEL_ID, L"Tempo di fissazione (ms)" },
  { DWELL_TOLERANCE_LABEL_ID, L"Raggio di tolleranza (px)" },
  { DWELL_COOLDOWN_LABEL_ID, L"Tempo di attesa (ms)" },
  { DWELL_COLOR_BUTTON_ID, L"Colore di avanzamento..." },
  { DWELL_DETECT_GROUP_ID, L"Rileva come usi il tracker" },
  { DWELL_PROBE_LOOK_ID, L"Attivami GUARDANDOMI" },
  { DWELL_PROBE_MOUSE_ID, L"Attivami con il MOUSE o un sensore" },
  { DWELL_STATUS_INITIAL_ID, L"Prova i due pulsanti qui sopra per impostare automaticamente la modalità migliore." },
  { DWELL_STATUS_GAZE_ID, L"Rilevato: cursore controllato dallo sguardo. Clic a fissazione attivato." },
  { DWELL_STATUS_MOUSE_ID, L"Rilevato: mouse manuale. Clic a fissazione disattivato." },
  { DWELL_STATUS_LOOK_HINT_ID, L"Guarda il pulsante senza fare clic." },
  { DWELL_APPLY_BUTTON_ID, L"Applica" },
  { DWELL_HID_UNAVAILABLE_ID, L"Il puntamento oculare HID non è ancora disponibile." },
  { TRAYICON_DWELL_ID, L"Sguardo / Clic a fissazione..." },
  { DWELL_SIGNALS_GROUP_ID, L"Rilevato" },
  { DWELL_SIGNAL_HID_LABEL_ID, L"Puntatore oculare (HID):" },
  { DWELL_SIGNAL_TOOL_LABEL_ID, L"App di controllo oculare:" },
  { DWELL_SIGNAL_WEC_LABEL_ID, L"Controllo oculare di Windows:" },
  { DWELL_SIGNAL_YES_ID, L"sì" },
  { DWELL_SIGNAL_NO_ID, L"no" },
  { DWELL_SIGNAL_NONE_ID, L"nessuna" },
  { DWELL_SIGNAL_HID_LIVE_ID, L"trasmissione dello sguardo" },
  { DWELL_SIGNAL_HID_IDLE_ID, L"presente (nessuna trasmissione)" },
  { DWELL_RESET_BUTTON_ID, L"Reimposta" },
  { BOARD_STYLE_CHANGED_TITLE_ID, L"Stile della tavola modificato" },
  { BOARD_STYLE_CHANGED_MESSAGE_ID, L"Vuoi annullare le modifiche?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, L"Eliminare tutte le categorie e le loro frasi?\nAttenzione: questa azione non può essere annullata." },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, L"Si tratta di un errore?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, L"Elimina tutte le categorie" },
  { TRAYICON_FEEDBACK_ID, L"Invia feedback" },
};

static const std::vector<std::pair<int, const wchar_t *>> JAPANESE_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"再生>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"カテゴリ" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"設定" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"既定のテキスト:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"既定のテキストを自動挿入" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"言語:" },
  { SETTINGS_VOICE_LABEL_ID, L"音声:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"音声をテスト" },
  { SETTINGS_VOLUME_LABEL_ID, L"音量 (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"速度 (-10 ～ 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"選択したらすぐにフレーズを話す" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"カテゴリ ウィンドウのサイズを記憶" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"カテゴリ ウィンドウを自動的に最小化" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"再生中に SimonSays の音量を上げる" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"再生中に他の音声を一時的に下げる" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"前の再生を停止" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"タッチ キーボードを表示" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"キャンセル" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - クリックして表示" },
  { TRAYICON_SHOW_ID, L"表示" },
  { TRAYICON_HIDE_ID, L"非表示" },
  { TRAYICON_SETTINGS_ID, L"設定" },
  { TRAYICON_ABOUT_ID, L"バージョン情報" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"終了" },
  { ABOUT_TITTLE_ID, L"SimonSays について" },
  { ABOUT_1_ID, L"SimonSays - ただ話すだけ\n\nバージョン " },
  { ABOUT_2_ID, L"\n\nテキストをすぐに音声に変換できる\nシンプルなアクセシビリティ ツールです。\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays エラー" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays は現在、下部タスクバーのみをサポートしています" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"カテゴリ名を変更" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"フレーズを編集" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"現在 : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"キャンセル" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"この名前のカテゴリはすでに存在します。別の名前を選択してください。" },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"カテゴリ名の競合" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"カテゴリを削除" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"カテゴリ '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' とそのすべてのフレーズを削除しますか？" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"フレーズを削除" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"フレーズ '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"' を削除しますか？" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"新しいカテゴリを追加" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"新しいフレーズを追加" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"カテゴリ名 :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"フレーズのテキスト :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"キャンセル" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"新しい既定のフレーズ" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"カテゴリをインポート" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"カテゴリ '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"' を上書きしますか？" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"カテゴリを上書きしますか?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"ボードスタイルを置き換えますか？" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"インポートしたファイルにはボードスタイルが含まれています。現在のスタイルを置き換えますか？" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"フレーズを正常にインポートしました。" },
  { IMPORT_SUCCESS_TITLE_ID, L"インポート成功" },
  { IMPORT_FAILURE_MESSAGE_ID, L"フレーズのインポートに失敗しました。" },
  { IMPORT_FAILURE_TITLE_ID, L"インポート失敗" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"カテゴリをエクスポート" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"すべてのカテゴリをエクスポートするか、選択したものだけをエクスポートできます。\n選択したカテゴリ『" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"』だけをエクスポートしますか？" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"選択をエクスポート" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"フレーズを正常にエクスポートしました。" },
  { EXPORT_SUCCESS_TITLE_ID, L"エクスポート成功" },
  { EXPORT_FAILURE_MESSAGE_ID, L"フレーズのエクスポートに失敗しました。" },
  { EXPORT_FAILURE_TITLE_ID, L"エクスポート失敗" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: ヘルプ - F2: 設定" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays ヘルプガイド

## 概要
SimonSays は、Juan Rey Saura が作成した Windows 用テキスト読み上げ（TTS）アシスタントです。素早い発話、フレーズライブラリ、インラインサウンド再生に最適化されています。複数言語、カスタマイズ可能な音声、効率的なコミュニケーションのためのユーザーフレンドリーな機能をサポートします。

SimonSays は Windows タスクバーの左側の空きスペースを活用し、よく使うフレーズ（カスタマイズ可能）の十数カテゴリに数クリックでアクセスできます。

## ショートカット
- `F1`: ヘルプを開く。
- `F2`: 設定を開く。
- `F3`: 視線 / 停留クリックの設定。
- `F4`: 現在の選択を編集。
- `F5` / `F6`: リスト内の前 / 次の選択に移動。
- `F7`: 現在の選択の後に追加（カテゴリ/フレーズコンテキスト）。
- `F8`: 現在の選択を削除。
- `F9`: カテゴリをインポート。
- `F10`: カテゴリをエクスポート。
- `F11` / `Ctrl -`: ズームアウト（カテゴリウィンドウ）。
- `F12` / `Ctrl +`: ズームイン（カテゴリウィンドウ）。
- `Ctrl 0`: ズームをリセット。
- `再生>` ボタン: 再生開始。

## 起動と基本操作
1. SimonSays を起動します。Windows システムトレイに表示されます。
2. 話すには、以下のいずれかができます：
   - 入力ボックスにカスタムテキストを入力し、`再生>` ボタン（または `Enter`）を押す。
   - カテゴリウィンドウで保存済みのフレーズを1つ以上クリックする。
3. 再生中、ボタンは `>` を表示してアクティブな再生を示します。もう一度押すと即座に停止できます。

## カテゴリウィンドウ（フレーズ）
- `カテゴリ` ボタンで開きます。設定からサイズ変更またはサイズの記憶ができます。
- 上部でカテゴリを、下部でフレーズを選択します。
- フレーズをシングルクリックすると、`選択したらすぐにフレーズを話す` が有効な場合は自動的に読み上げます。無効の場合は、読み上げずにメインの入力ボックスにフレーズを読み込みます。

### カテゴリの管理
- 名前変更: カテゴリを選択 → （ショートカット: `F4`）`カテゴリ名を変更` → 名前を変更 → `OK`。
- 選択移動: `F5`/`F6` で前/次の項目に移動。
- 追加: カテゴリを選択 → （ショートカット: `F7`）`新しいカテゴリを追加` ダイアログ → 名前を入力 → `OK`。
- 削除: カテゴリを選択 → （ショートカット: `F8`）`カテゴリを削除`；カテゴリとそのすべてのフレーズの削除を確認。



#### カテゴリのアイコンを設定または変更する
各カテゴリは、ボタンに絵文字またはカスタム `.ico` ファイルを表示できます。アイコンは、追加/編集ダイアログで区切りプレフィックス `##` を使って指定します。

```
<icon>##<category name>
```

- **絵文字**: 絵文字の後に `##` と名前を入力します。
  - 例: `🙂##あいさつ`
- **アイコンファイル** (`.ico`): ファイルパス（絶対パス、または SimonSays フォルダーからの相対パス）の後に `##` と名前を入力します。
  - 例: `C:\icons\smile.ico##あいさつ`
- **アイコンなし**: `##` プレフィックスを付けずにカテゴリ名だけを入力します。
  - 例: `あいさつ`

> 注: ファイルベースのアイコンとしてサポートされるのは `.ico` ファイルのみです。それ以外の拡張子は無視され、アイコンは表示されません。

### フレーズの管理
- 編集: フレーズを選択 → （ショートカット: `F4`）`フレーズを編集` → テキストを更新 → `OK`。
- 選択移動: `F5`/`F6` で前/次のフレーズに移動。
- 追加: フレーズを選択 → （ショートカット: `F7`）`新しいフレーズを追加` → テキストを入力 → `OK`。
- 削除: フレーズを選択 → （ショートカット: `F8`）`フレーズを削除`；確認。



#### フレーズのアイコンを設定または変更する
フレーズはカテゴリと同じアイコンプレフィックス規則に従い、オプションの区切り文字 `##` がフレーズテキストの前に付きます。

```
<icon>##<phrase text>
```

- **絵文字**: `👍##いいですね！`
- **アイコンファイル**: `C:\icons\check.ico##いいですね！`
- **アイコンなし**: `いいですね！`

フレーズにはインライン音声ファイルを含めることもできます。アイコン、テキスト、音声ファイルを組み合わせる場合の完全な形式は次のとおりです。

```
<icon>##<phrase text>::<audio file>
```

- 例: `🔔##注意::notification.wav`

`##` プレフィックスと `::` サフィックスはどちらも省略可能で、それぞれ独立しています。
- アイコンプレフィックスのみ: `🔔##注意`
- 音声サフィックスのみ: `注意::notification.wav`
- 両方: `🔔##注意::notification.wav`
- なし: `注意`

## 設定（F2）
設定（`F2`）を開いて制御：
- **既定のテキスト**: `既定のテキストを自動挿入` を有効/無効にし、既定のフレーズをカスタマイズ。
  - `タッチ キーボードを表示` — 入力ボックスがフォーカスを受け取ったときに画面上のタッチキーボードを開きます。メインウィンドウの近くに配置されます。
- **インターフェース言語**: UI 言語を選択（English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian）。
- **音声**: 好みに合ったインストール済みの SAPI 音声を選択。
- **音量/速度**: `音量 (10-100)` と `速度 (-10 ～ 10)` を調整。
- `音声をテスト`: 選択した音声を試聴。
- **カテゴリ ウィンドウ**:
  - `カテゴリ ウィンドウを自動的に最小化`（選択後）。
  - `カテゴリ ウィンドウのサイズを記憶`。
- **再生動作**:
  - `選択したらすぐにフレーズを話す`（クリックで自動再生）。
  - `前の再生を停止` — 新しいフレーズを開始する前に進行中の再生を自動的に停止します。
  - `再生中に Si)HELP" LR"HELP(monSays の音量を上げる` — 話している間、システムのマスター音量を上げ、再生終了後に復元します。
  - `再生中に他の音声を一時的に下げる` — 話している間、他のすべてのアプリケーションの音量を下げ、再生終了後に復元します。
- `OK` は変更を保存；`キャンセル` は破棄。

## 視線 / 停留クリック（F3）
フレーズやボタンに視線（またはマウスカーソル）を一定時間とどめることで、手を使わずに操作できます。この機能は新しく、テストとフィードバックが必要です。トレイメニューの `フィードバック` から、お使いのアイトラッカーでの動作をぜひお知らせください。

対応する構成：
- **マウスカーソルを動かすタイプの視線制御ソフトウェア**（Irisbond EasyClick、Tobii Dynavox Computer Control、Windows 視線制御のカーソルモード、OptiKey など）：停留クリックはカーソルに追従します。
- **カーソルを動かさない直接視線読み取り**：Irisbond Hiru（HID モード）、および各自のインストール済みソフトウェア経由の Tobii トラッカー — Tobii Eye Tracker 4C と Tobii Dynavox PCEye 5 で検証済み。Tobii Eye Tracker 5 は動作が見込まれますが未検証です。

視線 / 停留クリックの設定（`F3`）を開いて構成します（このダイアログのコントロールは英語で表示されます）：
- **有効化モード**: `Automatic` はデバイスの存在と動作を検出し、停留クリックをマウスカーソル経由と視線検出経由のどちらで有効にするかを決めます（アイトラッキングデバイスまたは関連ソフトウェアが検出されると停留クリックが有効になり、何も検出されない場合は無効のままです）。`Mouse cursor` は常にカーソル経由で有効化、`HID eye tracker` は常に視線検出経由で有効化、`Off` はデバイスの有無にかかわらず常に無効にします。
- **停留時間**（`Dwell time`）: 要素を見つめてクリックを発動させるのに必要な時間（ミリ秒）。停留中は進行状況インジケーターが表示されます。
- **許容半径**（`Tolerance radius`）: 視線の許容範囲（ピクセル）。目のわずかな動きがあっても停留クリックをキャンセルしません。
- **待機時間**（`Cooldown`）: 一度発動した後、次の停留クリックが発動しないミリ秒数（誤った連続クリックを防ぎます）。
- **進行色**（`Progress color`）: 停留の進行状況インジケーターの色（既定では Windows のアクセントカラー）。
- **トラッカーの使い方を検出**（`Detect how you use the tracker`）: アイトラッカーでカーソルを動かしているのか、単にボタンをクリックしているのかをアプリが検出するのを助ける 2 つのボタン。
- **検出結果**（`Detected`）: 検出されたデバイスの種類と、関連ソフトウェアに関する追加情報を表示します。
- `Reset` ボタン（左下）は、すべての停留設定を既定値に戻します。
- `OK` ボタンで変更を保存し、`キャンセル` で破棄します。

## トレイアイコンメニュー
- `表示` / `非表示` メインUI。
- `設定`。
- `Gaze / Dwell-click`（視線 / 停留クリック）。
- `バージョン情報` はバージョンとクレジットを表示。
- `Web` はプロジェクトページを開く（設定されている場合）。
- `フィードバック` は既定のブラウザで[フィードバック フォーム](https://forms.gle/KMbpEDYmwnFJUhgy8)を開きます。
- `終了` は SimonSays を終了。

トレイポップアップウィンドウや他のダイアログはタイトルバーをドラッグして移動できます。カテゴリウィンドウは端をドラッグしてサイズ変更し、設定で `カテゴリ ウィンドウのサイズを記憶` を有効にしてサイズを保持できます。

## 音声とサウンドの混合
- テキスト内にオーディオファイル名を埋め込むには、区切り文字 `♫`（`SOUND_NOTE_DELIMITER`）を使用します：
  - 例: `こんにちは ♫notification.wav♫、お待ちください。`
  - 区切り文字の外のテキストは読み上げられ、区切り文字の間のテキストはサウンドファイルとしてインラインで再生されます。
- 対応フォーマット: `.wav`、`.mid`、`.midi`、`.mp3`。
- プロセスがアクセス可能な絶対パスまたは相対パスを使用してください。末尾のテキストがオーディオとして扱われないよう、区切り文字をペアにしてください。



### サウンドファイルとアイコンファイルの検索順序
サウンドまたはアイコンのファイル名にドライブ文字や完全なパスが含まれていない場合、SimonSays は次の順序で検索し、最初に見つかったものを使用します。

1. **AppData フォルダー** — `%APPDATA%\SimonSays\`（ユーザーごとのフォルダー。アプリのインストール先と分けて管理できます）。
2. **作業ディレクトリ** — プロセスの現在の作業ディレクトリ（実行ファイルのフォルダーと異なる場合のみ検索）。
3. **実行ファイルのフォルダー** — `SimonSays.exe` があるフォルダー。

これらの場所のいずれにもファイルが見つからない場合は、内蔵の代替サウンドが使用されます。

> **ヒント:** カスタムのサウンドファイルやアイコンファイルは `%APPDATA%\SimonSays\` に置くことをおすすめします。アプリ更新後も残り、ユーザーごとに管理できます。

## カテゴリのインポートとエクスポート
- エクスポート: 要求されたときに、すべてのカテゴリまたは選択したカテゴリのみをエクスポートできます。成功/失敗メッセージは選択した言語で表示されます（ショートカット: `F10`）。
- インポート: ファイルを選択；カテゴリが存在する場合、上書きするかどうか確認されます。成功/失敗メッセージは選択した言語で表示されます（ショートカット: `F9`）。

## トラブルシューティング
- **音声初期化の失敗**: SAPI がインストールされ、互換性のある音声が存在することを確認してください。
- **サウンドが再生されない**: ファイルパス/拡張子とペアの `♫` 区切り文字を確認してください。
- **インポート/エクスポートエラー**: ファイルの権限とディスクの場所を確認し、再試行してください。
- **UI 言語が適用されない**: 設定を再度開いて言語の選択を確認し、選択した言語のローカライズ文字列が存在することを確認してください。

## バージョン情報
- `バージョン情報` ダイアログにはバージョン、説明、著作権が表示されます。
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"と" },
};

static const std::vector<std::pair<int, const wchar_t *>> KOREAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"재생>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"카테고리" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"설정" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"기본 텍스트:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"기본 텍스트 자동 삽입" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"언어:" },
  { SETTINGS_VOICE_LABEL_ID, L"음성:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"음성 테스트" },
  { SETTINGS_VOLUME_LABEL_ID, L"볼륨 (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"속도 (-10~10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"선택 시 문장을 바로 말하기" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"카테고리 창 크기 기억" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"카테고리 창 자동 최소화" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"재생 중 SimonSays 볼륨 증가" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"재생 중 다른 오디오 임시 감소" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"이전 재생 중지" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"터치 키보드 표시" },
  { SETTINGS_OK_BUTTON_ID, L"확인" },
  { SETTINGS_CANCEL_BUTTON_ID, L"취소" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - 클릭하여 표시" },
  { TRAYICON_SHOW_ID, L"표시" },
  { TRAYICON_HIDE_ID, L"숨기기" },
  { TRAYICON_SETTINGS_ID, L"설정" },
  { TRAYICON_ABOUT_ID, L"정보" },
  { TRAYICON_WEB_ID, L"웹" },
  { TRAYICON_EXIT_ID, L"종료" },
  { ABOUT_TITTLE_ID, L"SimonSays 정보" },
  { ABOUT_1_ID, L"SimonSays - 그냥 말하세요\n\n버전 " },
  { ABOUT_2_ID, L"\n\n텍스트를 즉시 음성으로 변환하는\n간단한 접근성 도구입니다.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"SimonSays 오류" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays는 현재 하단 작업 표시줄만 지원합니다" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"카테고리 이름 변경" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"문구 편집" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"현재 : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"확인" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"취소" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"이 이름의 카테고리가 이미 존재합니다. 다른 이름을 선택하세요." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"카테고리 이름 충돌" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"카테고리 삭제" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"카테고리 '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' 및 모든 문구를 삭제하시겠습니까?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"문구 삭제" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"문구 '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"새 카테고리 추가" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"새 문구 추가" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"카테고리 이름 :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"문구 텍스트 :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"확인" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"취소" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"새 기본 문구" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"카테고리 가져오기" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"카테고리 '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'를 덮어쓸까요?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"카테고리 덮어쓰기?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"보드 스타일을 바꾸시겠습니까?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"가져온 파일에 보드 스타일이 포함되어 있습니다. 현재 스타일을 바꾸시겠습니까?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"문구를 성공적으로 가져왔습니다." },
  { IMPORT_SUCCESS_TITLE_ID, L"가져오기 성공" },
  { IMPORT_FAILURE_MESSAGE_ID, L"문구를 가져오지 못했습니다." },
  { IMPORT_FAILURE_TITLE_ID, L"가져오기 실패" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"카테고리 내보내기" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"모든 카테고리를 내보내거나 선택한 것만 내보낼 수 있습니다.\n선택한 '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' 카테고리만 내보내겠습니까?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"선택 항목 내보내기" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"문구를 성공적으로 내보냈습니다." },
  { EXPORT_SUCCESS_TITLE_ID, L"내보내기 성공" },
  { EXPORT_FAILURE_MESSAGE_ID, L"문구를 내보내지 못했습니다." },
  { EXPORT_FAILURE_TITLE_ID, L"내보내기 실패" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: 도움말 - F2: 설정" },
  { HELP_CONTENT_ID, LR"HELP(# SimonSays 도움말 가이드

## 개요
SimonSays는 Juan Rey Saura가 만든 Windows용 TTS(텍스트 음성 변환) 도우미로, 빠른 발화, 문구 라이브러리, 인라인 사운드 재생에 최적화되어 있습니다. 다국어, 사용자 정의 음성, 효율적인 커뮤니케이션을 위한 편리한 기능을 지원합니다.

SimonSays는 Windows 작업 표시줄 왼쪽의 빈 공간을 활용하여, 자주 사용하는(사용자 정의 가능한) 문구의 십여 개 카테고리에 몇 번의 클릭만으로 빠르게 접근할 수 있습니다.

## 단축키
- `F1`: 도움말 열기.
- `F2`: 설정 열기.
- `F3`: 시선 / 응시 클릭 설정.
- `F4`: 현재 선택 편집.
- `F5` / `F6`: 목록에서 이전 / 다음 선택으로 이동.
- `F7`: 현재 선택 다음에 추가 (카테고리/문구 컨텍스트).
- `F8`: 현재 선택 삭제.
- `F9`: 카테고리 가져오기.
- `F10`: 카테고리 내보내기.
- `F11` / `Ctrl -`: 축소 (카테고리 창).
- `F12` / `Ctrl +`: 확대 (카테고리 창).
- `Ctrl 0`: 확대/축소 재설정.
- `재생>` 버튼: 재생 시작.

## 시작 및 기본 사용법
1. SimonSays를 실행합니다. Windows 시스템 트레이에 나타납니다.
2. 말하려면 다음 중 하나를 할 수 있습니다:
   - 입력란에 사용자 정의 텍스트를 입력하고 `재생>` 버튼(또는 `Enter`)을 누릅니다.
   - 카테고리 창에서 저장된 문구를 하나 이상 클릭합니다.
3. 재생 중 버튼은 `>`를 표시하여 활성 재생을 나타냅니다. 다시 눌러 즉시 중지할 수 있습니다.

## 카테고리 창 (문구)
- `카테고리` 버튼으로 엽니다. 설정에서 크기 변경 또는 크기 기억이 가능합니다.
- 위에서 카테고리를, 아래에서 문구를 선택합니다.
- 문구를 단일 클릭하면 `선택 시 문장을 바로 말하기`가 활성화된 경우 자동으로 읽습니다. 그렇지 않으면 읽지 않고 메인 입력란에 문구를 로드합니다.

### 카테고리 관리
- 이름 변경: 카테고리 선택 → (단축키: `F4`) `카테고리 이름 변경` → 이름 변경 → `확인`.
- 선택 이동: `F5`/`F6`으로 이전/다음 항목으로 이동.
- 추가: 카테고리 선택 → (단축키: `F7`) `새 카테고리 추가` 대화 상자 → 이름 입력 → `확인`.
- 삭제: 카테고리 선택 → (단축키: `F8`) `카테고리 삭제`; 카테고리 및 모든 문구 삭제 확인.



#### 카테고리 아이콘 설정 또는 변경
각 카테고리는 버튼에 이모지 또는 사용자 지정 `.ico` 파일을 표시할 수 있습니다. 아이콘은 추가/편집 대화 상자에서 `##` 구분 접두사를 사용해 지정합니다.

```
<icon>##<category name>
```

- **이모지**: 이모지를 입력한 뒤 `##`와 이름을 입력합니다.
  - 예: `🙂##인사`
- **아이콘 파일** (`.ico`): 파일 경로(절대 경로 또는 SimonSays 폴더 기준 상대 경로)를 입력한 뒤 `##`와 이름을 입력합니다.
  - 예: `C:\icons\smile.ico##인사`
- **아이콘 없음**: `##` 접두사 없이 카테고리 이름만 입력합니다.
  - 예: `인사`

> 참고: 파일 기반 아이콘으로는 `.ico` 파일만 지원됩니다. 다른 확장자는 무시되며 아이콘이 표시되지 않습니다.

### 문구 관리
- 편집: 문구 선택 → (단축키: `F4`) `문구 편집` → 텍스트 업데이트 → `확인`.
- 선택 이동: `F5`/`F6`으로 이전/다음 문구로 이동.
- 추가: 문구 선택 → (단축키: `F7`) `새 문구 추가` → 텍스트 입력 → `확인`.
- 삭제: 문구 선택 → (단축키: `F8`) `문구 삭제`; 확인.



#### 문구 아이콘 설정 또는 변경
문구는 카테고리와 같은 아이콘 접두사 규칙을 따르며, 선택적 `##` 구분자가 문구 텍스트 앞에 옵니다.

```
<icon>##<phrase text>
```

- **이모지**: `👍##좋아요!`
- **아이콘 파일**: `C:\icons\check.ico##좋아요!`
- **아이콘 없음**: `좋아요!`

문구에는 인라인 오디오 파일도 포함할 수 있습니다. 아이콘, 텍스트, 오디오 파일을 함께 사용할 때의 전체 형식은 다음과 같습니다.

```
<icon>##<phrase text>::<audio file>
```

- 예: `🔔##주의::notification.wav`

`##` 접두사와 `::` 접미사는 모두 선택 사항이며 서로 독립적입니다.
- 아이콘 접두사만: `🔔##주의`
- 오디오 접미사만: `주의::notification.wav`
- 둘 다: `🔔##주의::notification.wav`
- 둘 다 없음: `주의`

## 설정 (F2)
설정(`F2`)을 열어 제어:
- **기본 텍스트**: `기본 텍스트 자동 삽입` 활성화/비활성화 및 기본 문구 사용자 정의.
  - `터치 키보드 표시` — 입력란이 포커스를 받으면 화면 터치 키보드를 열며, 메인 창 근처에 위치합니다.
- **인터페이스 언어**: UI 언어 선택 (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **음성**: 선호에 맞는 설치된 SAPI 음성 선택.
- **볼륨/속도**: `볼륨 (10-100)` 및 `속도 (-10~10)` 조정.
- `음성 테스트`: 선택한 음성 미리 듣기.
- **카테고리 창**:
  - `카테고리 창 자동 최소화` (선택 후).
  - `카테고리 창 크기 기억`.
- **재생 동작**:
  - `선택 시 문장을 바로 말하기` (클릭 시 자동 재생).
  - `이전 재생 중지` — 새 문구를 시작하기 전에 진행 중인 재생을 자동으로 중지합니다.
  - `재생 중 SimonSays 볼륨 증가` — 말하는 동안 시스템 마스터 볼륨)HELP" LR"HELP(을 올리고, 재생이 끝나면 복원합니다.
  - `재생 중 다른 오디오 임시 감소` — 말하는 동안 다른 모든 애플리케이션의 볼륨을 줄이고, 재생이 끝나면 복원합니다.
- `확인`은 변경 사항 저장; `취소`는 취소.

## 시선 / 응시 클릭 (F3)
문구와 버튼에 시선(또는 마우스 커서)을 일정 시간 유지하면 손을 쓰지 않고 활성화할 수 있습니다. 이 기능은 새로운 기능으로, 테스트와 피드백이 필요합니다. 트레이 메뉴의 `피드백` 옵션을 통해 사용 중인 시선 추적 장치에서 어떻게 작동하는지 알려주세요.

지원되는 구성:
- **마우스 커서를 움직이는 모든 시선 제어 소프트웨어** (Irisbond EasyClick, Tobii Dynavox Computer Control, Windows 시선 제어의 커서 모드, OptiKey 등): 응시 클릭이 커서를 따라갑니다.
- **커서 이동 없이 시선을 직접 읽기**: Irisbond Hiru(HID 모드) 및 자체 설치 소프트웨어를 통한 Tobii 트래커 — Tobii Eye Tracker 4C와 Tobii Dynavox PCEye 5에서 검증됨. Tobii Eye Tracker 5는 작동할 것으로 예상되지만 아직 테스트되지 않았습니다.

시선 / 응시 클릭 설정(`F3`)을 열어 구성합니다(이 대화 상자의 컨트롤은 영어로 표시됩니다):
- **활성화 모드**: `Automatic`은 장치의 존재와 동작을 감지해 응시 클릭을 마우스 커서 방식으로 활성화할지 시선 감지 방식으로 활성화할지 결정합니다(시선 추적 장치나 관련 소프트웨어가 감지되면 응시 클릭이 활성화되고, 아무것도 감지되지 않으면 꺼진 상태로 유지됩니다). `Mouse cursor`는 항상 커서 방식으로, `HID eye tracker`는 항상 시선 감지 방식으로 활성화하며, `Off`는 장치 유무와 관계없이 항상 비활성화합니다.
- **응시 시간** (`Dwell time`): 요소를 바라보며 클릭을 발동시키는 데 필요한 시간(밀리초). 응시가 진행되는 동안 진행 표시기가 표시됩니다.
- **허용 반경** (`Tolerance radius`): 시선 허용 오차(픽셀)로, 눈이 약간 움직여도 응시 클릭이 취소되지 않습니다.
- **대기 시간** (`Cooldown`): 활성화 후 다른 응시 클릭이 발동되지 않는 밀리초(의도치 않은 반복 클릭 방지).
- **진행 색상** (`Progress color`): 응시 진행 표시기의 색상(기본값은 Windows 테마 색).
- **트래커 사용 방식 감지** (`Detect how you use the tracker`): 커서를 시선 추적 장치로 움직이는지, 아니면 단순히 버튼을 클릭하는지 앱이 감지하도록 돕는 두 개의 버튼.
- **감지됨** (`Detected`): 감지된 장치 유형과 관련 소프트웨어에 대한 추가 정보를 표시합니다.
- `Reset` 버튼(왼쪽 아래)은 모든 응시 설정을 기본값으로 되돌립니다.
- `확인` 버튼은 변경 사항을 저장하고, `취소`는 취소합니다.

## 트레이 아이콘 메뉴
- `표시` / `숨기기` 메인 UI.
- `설정`.
- `Gaze / Dwell-click` (시선 / 응시 클릭).
- `정보`는 버전과 크레딧 표시.
- `웹`은 프로젝트 페이지 열기 (구성된 경우).
- `피드백`은 기본 브라우저에서 [피드백 양식](https://forms.gle/KMbpEDYmwnFJUhgy8)을 엽니다.
- `종료`는 SimonSays 종료.

트레이 팝업 창과 다른 대화 상자는 제목 표시줄을 끌어 이동할 수 있습니다. 가장자리를 끌어 카테고리 창의 크기를 변경하고, 설정에서 `카테고리 창 크기 기억`을 활성화하여 크기를 유지할 수 있습니다.

## 음성과 사운드 혼합
- 텍스트에 오디오 파일명을 삽입하려면 구분자 `♫` (`SOUND_NOTE_DELIMITER`)를 사용합니다:
  - 예: `안녕하세요 ♫notification.wav♫, 잠시만 기다려 주세요.`
  - 구분자 외부의 텍스트는 읽힙니다. 구분자 사이의 텍스트는 사운드 파일로 처리되어 인라인 재생됩니다.
- 지원 형식: `.wav`, `.mid`, `.midi`, `.mp3`.
- 프로세스가 접근 가능한 절대 또는 상대 경로를 사용하세요. 뒤따르는 텍스트가 오디오로 처리되지 않도록 구분자를 쌍으로 맞추세요.



### 사운드 및 아이콘 파일 검색 순서
사운드 또는 아이콘 파일 이름에 드라이브 문자나 전체 경로가 없으면 SimonSays 는 다음 순서대로 검색하고, 처음 찾은 항목을 사용합니다.

1. **AppData 폴더** — `%APPDATA%\SimonSays\` (사용자별 폴더이며, 파일을 프로그램 설치 위치와 분리해 보관할 수 있습니다).
2. **작업 디렉터리** — 현재 프로세스의 작업 디렉터리(실행 파일 폴더와 다를 때만 검색).
3. **실행 파일 폴더** — `SimonSays.exe` 가 들어 있는 폴더.

이 위치들 어디에서도 파일을 찾지 못하면 기본 제공 대체 사운드가 사용됩니다.

> **팁:** 사용자 지정 사운드 및 아이콘 파일은 `%APPDATA%\SimonSays\` 에 두는 것이 좋습니다. 업데이트 후에도 유지되며 사용자별로 관리됩니다.

## 카테고리 가져오기 및 내보내기
- 내보내기: 요청 시 모든 카테고리 또는 선택한 카테고리만 내보낼 수 있습니다. 성공/실패 메시지는 선택한 언어로 표시됩니다 (단축키: `F10`).
- 가져오기: 파일 선택; 카테고리가 존재하면 덮어쓸지 묻습니다. 성공/실패 메시지는 선택한 언어로 표시됩니다 (단축키: `F9`).

## 문제 해결
- **음성 초기화 실패**: SAPI가 설치되어 있고 호환되는 음성이 있는지 확인하세요.
- **사운드가 재생되지 않음**: 파일 경로/확장자와 쌍을 이룬 `♫` 구분자를 확인하세요.
- **가져오기/내보내기 오류**: 파일 권한과 디스크 위치를 확인하고 다시 시도하세요.
- **UI 언어가 적용되지 않음**: 설정을 다시 열고 언어 선택을 확인하세요. 선택한 언어의 지역화 문자열이 존재하는지 확인하세요.

## 정보
- `정보` 대화 상자는 버전, 설명, 저작권을 표시합니다.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"및" },
};

static const std::vector<std::pair<int, const wchar_t *>> PORTUGUESE_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Reproduzir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categorias" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configurações" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Texto padrão:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Inserir texto padrão automaticamente" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Voz:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Testar voz" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volume (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocidade (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Falar a frase imediatamente ao selecionar" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Lembrar o tamanho da janela de categorias" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimizar automaticamente a janela de categorias" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Aumentar o volume do SimonSays ao reproduzir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Reduzir temporariamente outros áudios ao reproduzir" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Parar reprodução anterior" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Mostrar teclado tátil" },
  { SETTINGS_OK_BUTTON_ID, L"OK" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancelar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Clique para mostrar" },
  { TRAYICON_SHOW_ID, L"Mostrar" },
  { TRAYICON_HIDE_ID, L"Ocultar" },
  { TRAYICON_SETTINGS_ID, L"Configurações" },
  { TRAYICON_ABOUT_ID, L"Sobre" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Sair" },
  { ABOUT_TITTLE_ID, L"Sobre o SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplesmente fale\n\nVersão " },
  { ABOUT_2_ID, L"\n\nUma ferramenta de acessibilidade simples\nque transforma texto em voz instantaneamente.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Erro do SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"O SimonSays atualmente suporta apenas a barra de tarefas inferior" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Alterar nome da categoria" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Editar frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Atual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"OK" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Já existe uma categoria com esse nome. Por favor, escolha um nome diferente." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflito de nomes de categoria" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Excluir categoria" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Excluir a categoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' e todas as suas frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Excluir frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Excluir a frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Adicionar nova categoria" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Adicionar nova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nome da categoria :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Texto da frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"OK" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancelar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nova frase padrão" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categorias" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categorias do SimonSays\0*.ssc;*.ssz\0Todos os arquivos\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sobrescrever a categoria '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sobrescrever categoria?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Substituir o estilo do quadro?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"O arquivo importado inclui um estilo de quadro. Substituir o atual?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importadas com sucesso." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importação bem-sucedida" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Falha ao importar as frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Falha na importação" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categorias" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categorias do SimonSays\0*.ssc;*.ssz\0Todos os arquivos\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Você pode exportar todas as categorias ou apenas a selecionada.\nDeseja exportar apenas a categoria '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar seleção" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportadas com sucesso." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportação bem-sucedida" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Falha ao exportar as frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Falha na exportação" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Ajuda - F2: Configurações" },
  { HELP_CONTENT_ID, LR"HELP(# Guia de Ajuda do SimonSays

## Visão geral
SimonSays é um assistente de texto para fala (TTS) para Windows, criado por Juan Rey Saura, otimizado para fala rápida, bibliotecas de frases e reprodução de sons em linha. Suporta vários idiomas, vozes personalizáveis e funcionalidades práticas para uma comunicação eficiente.

SimonSays utiliza o espaço livre à esquerda da barra de tarefas do Windows, proporcionando acesso rápido a uma dezena de categorias de frases de uso frequente (e personalizáveis) com apenas alguns cliques.

## Atalhos
- `F1`: Abrir Ajuda.
- `F2`: Abrir Definições.
- `F3`: Definições de Olhar / Clique por fixação.
- `F4`: Editar a seleção atual.
- `F5` / `F6`: Mover para a seleção anterior / seguinte nas listas.
- `F7`: Adicionar após a seleção atual (contexto categoria/frase).
- `F8`: Eliminar a seleção atual.
- `F9`: Importar categorias.
- `F10`: Exportar categorias.
- `F11` / `Ctrl -`: Reduzir zoom (janela de categorias).
- `F12` / `Ctrl +`: Aumentar zoom (janela de categorias).
- `Ctrl 0`: Repor zoom.
- Botão `Reproduzir>`: Iniciar reprodução.

## Início e utilização básica
1. Inicie o SimonSays; aparece na área de notificação do Windows.
2. Para falar, pode:
   - Escrever texto personalizado na caixa de entrada e premir o botão `Reproduzir>` (ou `Enter`).
   - Clicar numa ou mais frases guardadas na janela de Categorias.
3. Durante a reprodução, o botão mostra `>` para indicar reprodução ativa. Pode premir novamente para parar instantaneamente.

## Janela de Categorias (frases)
- Abra com o botão `Categorias`. Redimensione ou recorde o tamanho através das Definições.
- Selecione uma categoria em cima e frases em baixo.
- Um clique numa frase reproduz automaticamente se `Falar a frase imediatamente ao selecionar` estiver ativado; caso contrário, carrega a frase na caixa de entrada principal sem falar.

### Gerir categorias
- Renomear: selecione categoria → (atalho: `F4`) `Alterar nome da categoria` → altere o nome → `OK`.
- Mover seleção: `F5`/`F6` para o item anterior/seguinte.
- Adicionar: selecione categoria → (atalho: `F7`) diálogo `Adicionar nova categoria` → introduza o nome → `OK`.
- Eliminar: selecione categoria → (atalho: `F8`) `Eliminar categoria`; confirme a eliminação da categoria e das suas frases.



#### Definir ou alterar o ícone de uma categoria
Cada categoria pode mostrar um emoji ou um ficheiro `.ico` personalizado no respetivo botão. O ícone é indicado na janela Adicionar/Editar usando o prefixo separador `##`:

```
<ícone>##<nome da categoria>
```

- **Emoji**: introduza o emoji seguido de `##` e do nome.
  - Exemplo: `🙂##Saudações`
- **Ficheiro de ícone** (`.ico`): introduza o caminho do ficheiro (absoluto ou relativo à pasta do SimonSays) seguido de `##` e do nome.
  - Exemplo: `C:\icons\smile.ico##Saudações`
- **Sem ícone**: introduza apenas o nome da categoria, sem prefixo `##`.
  - Exemplo: `Saudações`

> Nota: apenas ficheiros `.ico` são suportados para ícones baseados em ficheiro. Qualquer outra extensão é ignorada e nenhum ícone é apresentado.

### Gerir frases
- Editar: selecione frase → (atalho: `F4`) `Editar frase` → atualize o texto → `OK`.
- Mover seleção: `F5`/`F6` para a frase anterior/seguinte.
- Adicionar: selecione frase → (atalho: `F7`) `Adicionar nova frase` → introduza o texto → `OK`.
- Eliminar: selecione frase → (atalho: `F8`) `Eliminar frase`; confirme.



#### Definir ou alterar o ícone de uma frase
As frases seguem a mesma convenção de prefixo de ícone das categorias, com o separador opcional `##` antes do texto da frase:

```
<ícone>##<texto da frase>
```

- **Emoji**: `👍##Parece bem!`
- **Ficheiro de ícone**: `C:\icons\check.ico##Parece bem!`
- **Sem ícone**: `Parece bem!`

As frases também podem incluir um ficheiro de áudio em linha. Ao combinar ícone, texto e ficheiro de áudio, o formato completo é:

```
<ícone>##<texto da frase>::<ficheiro de áudio>
```

- Exemplo: `🔔##Atenção::notification.wav`

O prefixo `##` e o sufixo `::` são opcionais e independentes; pode usar qualquer combinação:
- Apenas prefixo de ícone: `🔔##Atenção`
- Apenas sufixo de áudio: `Atenção::notification.wav`
- Ambos: `🔔##Atenção::notification.wav`
- Nenhum: `Atenção`

## Definições (F2)
Abra as Definições (`F2`) para controlar:
- **Texto predefinido**: ativar/desativar `Inserir texto predefinido automaticamente` e personalizar a frase predefinida.
  - `Mostrar teclado tátil` — abre o teclado tátil no ecrã quando a caixa de entrada recebe o foco, posicionado perto da janela principal.
- **Idioma da interface**: escolher idioma da UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Voz**: selecionar voz SAPI instalada de acordo com a sua preferência.
- **Volume/Velocidade**: ajustar `Volume (10-100)` e `Velocidade (-10 a 10)`.
- `Testar voz`: ouvir a voz selecionada.
- **Janela de categorias**:
  - `Minimizar automaticamente a janela de categorias` após seleção.
  - `Lembrar o tamanho da janela de categorias`.
- **Comportamento de reprodução**:
  - `Falar a frase imediatamente ao selecionar` (reprodução automática ao clicar).
  - `Parar reprodução anterior` — para automaticamente qualquer reprodução em curso antes de iniciar uma nova frase.
  - `Aumentar o volume do SimonSays ao reproduzir` — aumenta o volume mestre do sistema enquanto fala, restaurando-o quando a reprodução termina.
  - `Reduzir temporariamente outros áudios ao reproduzir` — reduz o volume de todas as outras aplicações enquanto fala, restaurando-o quando a reprodução termina.
- `OK` guarda as alterações; `Cancelar` descarta.

## Olhar )HELP" LR"HELP(/ Clique por fixação (F3)
Ative frases e botões sem usar as mãos, mantendo o olhar (ou o cursor do rato) sobre eles durante um tempo definido. Esta funcionalidade é nova — precisa de testes e feedback: conte-nos como funciona com o seu dispositivo de seguimento ocular através da opção `Comentários` do menu da área de notificação.

Configurações suportadas:
- **Qualquer software de controlo ocular que mova o cursor do rato** (Irisbond EasyClick, Tobii Dynavox Computer Control, modos de cursor do Controlo ocular do Windows, OptiKey, ...): o clique por fixação segue o cursor.
- **Leitura direta do olhar, sem necessidade de mover o cursor**: Irisbond Hiru (modo HID) e dispositivos Tobii através do seu próprio software instalado — verificado com o Tobii Eye Tracker 4C e o Tobii Dynavox PCEye 5; espera-se que o Tobii Eye Tracker 5 funcione, mas ainda não foi testado.

Abra as definições de Olhar / Clique por fixação (`F3`) para configurar (os controlos deste diálogo são apresentados em inglês):
- **Modo de ativação**: escolha entre `Automatic`, que deteta a presença e o comportamento do dispositivo e decide se ativa o clique por fixação através do cursor do rato ou através da deteção do olhar (se for detetado um dispositivo de seguimento ocular ou software relacionado, o clique por fixação é ativado; se nenhum for detetado, permanece desativado), `Mouse cursor` para ativar sempre o clique por fixação através do cursor, `HID eye tracker` para o ativar sempre através da deteção do olhar, ou `Off` para o desativar sempre, independentemente dos dispositivos presentes.
- **Tempo de fixação** (`Dwell time`): duração (em milissegundos) necessária para acionar um clique ao olhar para um elemento. Durante a fixação é mostrado um indicador de progresso.
- **Raio de tolerância** (`Tolerance radius`): tolerância do olhar (em píxeis) para permitir pequenos movimentos dos olhos sem cancelar o clique por fixação.
- **Tempo de espera** (`Cooldown`): milissegundos após uma ativação durante os quais não será acionado outro clique por fixação (evita cliques repetidos acidentais).
- **Cor de progresso** (`Progress color`): cor do indicador de progresso da fixação (por predefinição, a cor de destaque do Windows).
- **Detetar como usa o dispositivo** (`Detect how you use the tracker`): dois botões que ajudam a aplicação a detetar se move o cursor com um dispositivo de seguimento ocular ou se simplesmente clica nos botões.
- **Detetado** (`Detected`): mostra o tipo de dispositivo detetado e informações adicionais sobre o software relacionado.
- Um botão `Reset` (em baixo à esquerda) repõe todas as definições de fixação nos valores predefinidos.
- O botão `OK` guarda as alterações; `Cancelar` descarta-as.

## Menu do ícone da área de notificação
- `Mostrar` / `Ocultar` interface principal.
- `Definições`.
- `Gaze / Dwell-click` (Olhar / Clique por fixação).
- `Sobre` mostra versão e créditos.
- `Web` abre a página do projeto (se configurada).
- `Comentários` abre o [formulário de feedback](https://forms.gle/KMbpEDYmwnFJUhgy8) no navegador padrão.
- `Sair` fecha o SimonSays.

Pode mover a janela pop-up da área de notificação e outros diálogos arrastando a barra de título; redimensione a janela de Categorias arrastando as bordas, e ative `Lembrar o tamanho da janela de categorias` nas Definições para manter o tamanho.

## Misturar fala e sons
- Use o delimitador `♫` (`SOUND_NOTE_DELIMITER`) para inserir nomes de ficheiros de áudio no texto:
  - Exemplo: `Olá ♫notification.wav♫, por favor aguarde.`
  - O texto fora dos delimitadores é falado; o texto entre delimitadores é tratado como ficheiro de som e reproduzido em linha.
- Formatos de áudio suportados: `.wav`, `.mid`, `.midi`, `.mp3`.
- Use caminhos absolutos ou relativos acessíveis ao processo. Emparelhe delimitadores para evitar que o texto final seja tratado como áudio.



### Ordem de procura de ficheiros de som e ícone
Quando um nome de ficheiro de som ou ícone não contém uma letra de unidade ou um caminho completo, o SimonSays procura-o pela seguinte ordem, usando a primeira correspondência encontrada:

1. **Pasta AppData** — `%APPDATA%\SimonSays\` (específica do utilizador; coloque aqui os ficheiros para os manter separados da instalação da aplicação).
2. **Diretório de trabalho** — o diretório de trabalho atual do processo (apenas é pesquisado se for diferente da pasta do executável).
3. **Pasta do executável** — a pasta que contém `SimonSays.exe`.

Se o ficheiro não for encontrado em nenhuma destas localizações, é usado o som de reserva incorporado.

> **Sugestão:** recomenda-se colocar os seus ficheiros de som e ícone personalizados em `%APPDATA%\SimonSays\`; sobreviverão às atualizações e serão específicos do utilizador.

## Importar e exportar categorias
- Exportar: pode exportar todas as categorias ou apenas a selecionada quando solicitado. As mensagens de sucesso/erro aparecem no idioma selecionado (atalho: `F10`).
- Importar: escolha um ficheiro; se a categoria existir, será perguntado se deseja sobrescrever. As mensagens de sucesso/erro aparecem no idioma selecionado (atalho: `F9`).

## Resolução de problemas
- **Falha na inicialização da voz**: verifique se o SAPI está instalado e se existe uma voz compatível.
- **Sem reprodução de sons**: confirme caminho/extensão e delimitadores `♫` emparelhados.
- **Erros de importação/exportação**: verifique permissões e localização do disco; tente novamente.
- **Idioma da UI não aplicado**: reabra as Definições e confirme a seleção de idioma; certifique-se de que existem cadeias de localização para o idioma escolhido.

## Sobre
- O diálogo `Sobre` mostra versão, descrição e direitos de autor.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"e" },
  { DWELL_DIALOG_TITLE_ID, L"Olhar / Clique por fixação" },
  { DWELL_INTRO_ID, L"Ative os botões mantendo o olhar (ou o cursor) sobre eles." },
  { DWELL_MODE_GROUP_ID, L"Modo de ativação" },
  { DWELL_MODE_AUTO_ID, L"Automático" },
  { DWELL_MODE_MOUSE_ID, L"Cursor do mouse" },
  { DWELL_MODE_HID_ID, L"Rastreador ocular HID" },
  { DWELL_MODE_OFF_ID, L"Desativado" },
  { DWELL_TIME_LABEL_ID, L"Tempo de fixação (ms)" },
  { DWELL_TOLERANCE_LABEL_ID, L"Raio de tolerância (px)" },
  { DWELL_COOLDOWN_LABEL_ID, L"Tempo de espera (ms)" },
  { DWELL_COLOR_BUTTON_ID, L"Cor de progresso..." },
  { DWELL_DETECT_GROUP_ID, L"Detectar como você usa o rastreador" },
  { DWELL_PROBE_LOOK_ID, L"Ative-me OLHANDO para mim" },
  { DWELL_PROBE_MOUSE_ID, L"Ative-me com o MOUSE ou acionador" },
  { DWELL_STATUS_INITIAL_ID, L"Experimente os dois botões acima para definir automaticamente o melhor modo." },
  { DWELL_STATUS_GAZE_ID, L"Detectado: cursor controlado pelo olhar. Clique por fixação ativado." },
  { DWELL_STATUS_MOUSE_ID, L"Detectado: mouse manual. Clique por fixação desativado." },
  { DWELL_STATUS_LOOK_HINT_ID, L"Por favor, OLHE para o botão sem clicar nele." },
  { DWELL_APPLY_BUTTON_ID, L"Aplicar" },
  { DWELL_HID_UNAVAILABLE_ID, L"O rastreamento ocular HID ainda não está disponível." },
  { TRAYICON_DWELL_ID, L"Olhar / Clique por fixação..." },
  { DWELL_SIGNALS_GROUP_ID, L"Detectado" },
  { DWELL_SIGNAL_HID_LABEL_ID, L"Rastreador ocular (HID):" },
  { DWELL_SIGNAL_TOOL_LABEL_ID, L"App de controle ocular:" },
  { DWELL_SIGNAL_WEC_LABEL_ID, L"Controle ocular do Windows:" },
  { DWELL_SIGNAL_YES_ID, L"sim" },
  { DWELL_SIGNAL_NO_ID, L"não" },
  { DWELL_SIGNAL_NONE_ID, L"nenhum" },
  { DWELL_SIGNAL_HID_LIVE_ID, L"transmitindo o olhar" },
  { DWELL_SIGNAL_HID_IDLE_ID, L"presente (sem transmitir)" },
  { DWELL_RESET_BUTTON_ID, L"Redefinir" },
  { BOARD_STYLE_CHANGED_TITLE_ID, L"Estilo do quadro alterado" },
  { BOARD_STYLE_CHANGED_MESSAGE_ID, L"Deseja reverter as alterações?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, L"Excluir todas as categorias e suas frases?\nAviso: esta ação não pode ser desfeita." },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, L"Isso é um erro?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, L"Excluir todas as categorias" },
  { TRAYICON_FEEDBACK_ID, L"Enviar comentários" },
};

static const std::vector<std::pair<int, const wchar_t *>> RUSSIAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Воспроизвести>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Категории" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Настройки" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Текст по умолчанию:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Автоматически вставлять текст по умолчанию" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Язык:" },
  { SETTINGS_VOICE_LABEL_ID, L"Голос:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Проверить голос" },
  { SETTINGS_VOLUME_LABEL_ID, L"Громкость (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Скорость (-10 до 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Произносить фразу сразу при выборе" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Запоминать размер окна категорий" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Автоматически сворачивать окно категорий" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Повышать громкость SimonSays при воспроизведении" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Временно снижать другие звуки при воспроизведении" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Остановить предыдущее воспроизведение" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Показать сенсорную клавиатуру" },
  { SETTINGS_OK_BUTTON_ID, L"ОК" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Отмена" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Нажмите, чтобы показать" },
  { TRAYICON_SHOW_ID, L"Показать" },
  { TRAYICON_HIDE_ID, L"Скрыть" },
  { TRAYICON_SETTINGS_ID, L"Настройки" },
  { TRAYICON_ABOUT_ID, L"О программе" },
  { TRAYICON_WEB_ID, L"Веб" },
  { TRAYICON_EXIT_ID, L"Выход" },
  { ABOUT_TITTLE_ID, L"О программе SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Просто говорите\n\nВерсия " },
  { ABOUT_2_ID, L"\n\nПростой инструмент доступности,\nкоторый мгновенно превращает текст в речь.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Ошибка SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays в настоящее время поддерживает только нижнюю панель задач" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Изменить имя категории" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Редактировать фразу" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Текущий : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"ОК" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Отмена" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Категория с таким именем уже существует. Пожалуйста, выберите другое имя." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Конфликт имен категорий" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Удалить категорию" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Удалить категорию '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' и все ее фразы?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Удалить фразу" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Удалить фразу '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Добавить новую категорию" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Добавить новую фразу" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Название категории :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Текст фразы :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"ОК" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Отмена" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Новая фраза по умолчанию" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Импорт категорий" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Перезаписать категорию '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Перезаписать категорию?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Заменить стиль доски?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"Импортированный файл содержит стиль доски. Заменить текущий стиль?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Фразы успешно импортированы." },
  { IMPORT_SUCCESS_TITLE_ID, L"Импорт выполнен" },
  { IMPORT_FAILURE_MESSAGE_ID, L"Не удалось импортировать фразы." },
  { IMPORT_FAILURE_TITLE_ID, L"Ошибка импорта" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Экспорт категорий" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"SimonSays Categories (*.ssc;*.ssz)\0*.ssc;*.ssz\0All Files\0*.*\0" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Вы можете экспортировать все категории или только выбранную.\nХотите экспортировать только категорию '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Экспорт выбора" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Фразы успешно экспортированы." },
  { EXPORT_SUCCESS_TITLE_ID, L"Экспорт выполнен" },
  { EXPORT_FAILURE_MESSAGE_ID, L"Не удалось экспортировать фразы." },
  { EXPORT_FAILURE_TITLE_ID, L"Ошибка экспорта" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Справка - F2: Настройки" },
  { HELP_CONTENT_ID, LR"HELP(# Руководство по SimonSays

## Обзор
SimonSays — это помощник для преобразования текста в речь (TTS) для Windows, созданный Juan Rey Saura, оптимизированный для быстрой речи, библиотек фраз и воспроизведения звуков в тексте. Поддерживает несколько языков, настраиваемые голоса и удобные функции для эффективного общения.

SimonSays использует свободное пространство слева от панели задач Windows, обеспечивая быстрый доступ к десяткам категорий часто используемых (и настраиваемых) фраз всего в несколько кликов.

## Горячие клавиши
- `F1`: Открыть справку.
- `F2`: Открыть настройки.
- `F3`: Настройки взгляда / клика с задержкой.
- `F4`: Редактировать текущий выбор.
- `F5` / `F6`: Перейти к предыдущему / следующему элементу в списках.
- `F7`: Добавить после текущего выбора (контекст категории/фразы).
- `F8`: Удалить текущий выбор.
- `F9`: Импорт категорий.
- `F10`: Экспорт категорий.
- `F11` / `Ctrl -`: Уменьшить масштаб (окно категорий).
- `F12` / `Ctrl +`: Увеличить масштаб (окно категорий).
- `Ctrl 0`: Сбросить масштаб.
- Кнопка `Воспроизвести>`: Начать воспроизведение.

## Запуск и основное использование
1. Запустите SimonSays; он появится в области уведомлений Windows.
2. Чтобы говорить, вы можете:
   - Ввести текст в поле ввода и нажать кнопку `Воспроизвести>` (или `Enter`).
   - Нажать на одну или несколько сохранённых фраз в окне категорий.
3. Во время воспроизведения кнопка показывает `>`, указывая на активное воспроизведение. Нажмите снова, чтобы мгновенно остановить.

## Окно категорий (фразы)
- Откройте кнопкой `Категории`. Измените размер или запомните его через настройки.
- Выберите категорию вверху и фразы внизу.
- Один клик по фразе автоматически воспроизводит её, если включено `Произносить фразу сразу при выборе`; иначе фраза загружается в основное поле ввода без произнесения.

### Управление категориями
- Переименовать: выберите категорию → (горячая клавиша: `F4`) `Изменить имя категории` → измените название → `ОК`.
- Переместить выбор: `F5`/`F6` для предыдущего/следующего элемента.
- Добавить: выберите категорию → (горячая клавиша: `F7`) диалог `Добавить новую категорию` → введите название → `ОК`.
- Удалить: выберите категорию → (горячая клавиша: `F8`) `Удалить категорию`; подтвердите удаление категории и всех её фраз.



#### Настройка или изменение значка категории
Каждая категория может отображать на своей кнопке эмодзи или пользовательский файл `.ico`. Значок задаётся в окне добавления/редактирования с помощью префикса-разделителя `##`:

```
<icon>##<category name>
```

- **Эмодзи**: введите эмодзи, затем `##` и имя.
  - Пример: `🙂##Приветствия`
- **Файл значка** (`.ico`): введите путь к файлу (абсолютный или относительный к папке SimonSays), затем `##` и имя.
  - Пример: `C:\icons\smile.ico##Приветствия`
- **Без значка**: просто введите имя категории без префикса `##`.
  - Пример: `Приветствия`

> Примечание: для файловых значков поддерживаются только файлы `.ico`. Любое другое расширение игнорируется, и значок не отображается.

### Управление фразами
- Редактировать: выберите фразу → (горячая клавиша: `F4`) `Редактировать фразу` → обновите текст → `ОК`.
- Переместить выбор: `F5`/`F6` для предыдущей/следующей фразы.
- Добавить: выберите фразу → (горячая клавиша: `F7`) `Добавить новую фразу` → введите текст → `ОК`.
- Удалить: выберите фразу → (горячая клавиша: `F8`) `Удалить фразу`; подтвердите.



#### Настройка или изменение значка фразы
Фразы используют ту же схему префикса значка, что и категории, с необязательным разделителем `##` перед текстом фразы:

```
<icon>##<phrase text>
```

- **Эмодзи**: `👍##Звучит хорошо!`
- **Файл значка**: `C:\icons\check.ico##Звучит хорошо!`
- **Без значка**: `Звучит хорошо!`

Фразы также могут включать встроенный аудиофайл. При объединении значка, текста и аудиофайла полный формат выглядит так:

```
<icon>##<phrase text>::<audio file>
```

- Пример: `🔔##Внимание::notification.wav`

Префикс `##` и суффикс `::` являются необязательными и независимыми:
- Только префикс значка: `🔔##Внимание`
- Только суффикс аудио: `Внимание::notification.wav`
- Оба: `🔔##Внимание::notification.wav`
- Ни одного: `Внимание`

## Настройки (F2)
Откройте настройки (`F2`) для управления:
- **Текст по умолчанию**: включить/выключить `Автоматически вставлять текст по умолчанию` и настроить фразу по умолчанию.
  - `Показать сенсорную клавиатуру` — открывает экранную сенсорную клавиатуру, когда поле ввода получает фокус, расположенную рядом с главным окном.
- **Язык интерфейса**: выбрать язык UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Голос**: выбрать установленный голос SAPI по вашему предпочтению.
- **Громкость/Скорость**: настроить `Громкость (10-100)` и `Скорость (-10 до 10)`.
- `Проверить голос`: прослушать выбранный голос.
- **Окно категорий**:
  - `Автоматически сворачивать окно категорий` после выбора.
  - `Запоминать размер окна категорий`.
- **Поведение воспроизведения**:
  - `Произносить фразу сразу при выборе` (автовоспроизведение по клику).
  - `Остановить предыдущее воспроизведение` — автоматически останавливает текущее воспроизведение перед началом новой фразы.
  - `Повышать громкость SimonSays при воспроизведении` — повышает системную громкость во время речи, восстанавливая её по окончании.
  - `Временно снижать другие звуки при воспроизведении` — снижает громко)HELP" LR"HELP(сть всех других приложений во время речи, восстанавливая её по окончании.
- `ОК` сохраняет изменения; `Отмена` отменяет.

## Взгляд / Клик с задержкой (F3)
Активируйте фразы и кнопки без рук, удерживая взгляд (или курсор мыши) на них в течение заданного времени. Эта функция новая — ей нужны тестирование и обратная связь: расскажите, как она работает с вашим айтрекером, через пункт `Обратная связь` в меню области уведомлений.

Поддерживаемые конфигурации:
- **Любое ПО управления взглядом, которое перемещает курсор мыши** (Irisbond EasyClick, Tobii Dynavox Computer Control, режимы курсора «Управления глазами» Windows, OptiKey, ...): клик с задержкой следует за курсором.
- **Прямое чтение взгляда, без перемещения курсора**: Irisbond Hiru (режим HID) и устройства Tobii через их собственное установленное ПО — проверено с Tobii Eye Tracker 4C и Tobii Dynavox PCEye 5; Tobii Eye Tracker 5 должен работать, но пока не тестировался.

Откройте настройки взгляда / клика с задержкой (`F3`), чтобы настроить (элементы этого диалога отображаются на английском):
- **Режим активации**: выберите `Automatic` — определяет наличие и поведение устройства и решает, включать ли клик с задержкой через курсор мыши или через чтение взгляда (если обнаружено устройство отслеживания взгляда или связанное ПО, клик с задержкой включается; если ничего не обнаружено, он остаётся выключенным), `Mouse cursor` — всегда включать клик с задержкой через курсор, `HID eye tracker` — всегда включать его через чтение взгляда, или `Off` — всегда выключать независимо от наличия устройств.
- **Время задержки** (`Dwell time`): длительность (в миллисекундах), необходимая для срабатывания клика при взгляде на элемент. Во время задержки отображается индикатор прогресса.
- **Радиус допуска** (`Tolerance radius`): допуск взгляда (в пикселях), позволяющий небольшие движения глаз без отмены клика с задержкой.
- **Время ожидания** (`Cooldown`): миллисекунды после активации, в течение которых новый клик с задержкой не сработает (предотвращает случайные повторные клики).
- **Цвет прогресса** (`Progress color`): цвет индикатора прогресса задержки (по умолчанию — акцентный цвет Windows).
- **Определить, как вы используете трекер** (`Detect how you use the tracker`): две кнопки, помогающие приложению определить, перемещаете ли вы курсор с помощью айтрекера или просто нажимаете кнопки.
- **Обнаружено** (`Detected`): показывает тип обнаруженного устройства и дополнительную информацию о связанном ПО.
- Кнопка `Reset` (внизу слева) возвращает все настройки задержки к значениям по умолчанию.
- Кнопка `ОК` сохраняет изменения; `Отмена` отменяет их.

## Меню значка в области уведомлений
- `Показать` / `Скрыть` основной интерфейс.
- `Настройки`.
- `Gaze / Dwell-click` (Взгляд / Клик с задержкой).
- `О программе` показывает версию и авторов.
- `Веб` открывает страницу проекта (если настроена).
- `Обратная связь` открывает [форму обратной связи](https://forms.gle/KMbpEDYmwnFJUhgy8) в браузере по умолчанию.
- `Выход` закрывает SimonSays.

Вы можете перемещать всплывающее окно области уведомлений и другие диалоги, перетаскивая заголовок; изменяйте размер окна категорий, перетаскивая края, и включите `Запоминать размер окна категорий` в настройках для сохранения размера.

## Смешивание речи и звуков
- Используйте разделитель `♫` (`SOUND_NOTE_DELIMITER`) для вставки имён звуковых файлов в текст:
  - Пример: `Привет ♫notification.wav♫, пожалуйста, подождите.`
  - Текст вне разделителей произносится; текст между разделителями воспроизводится как звуковой файл.
- Поддерживаемые форматы: `.wav`, `.mid`, `.midi`, `.mp3`.
- Используйте абсолютные или относительные пути, доступные процессу. Парные разделители предотвращают обработку оставшегося текста как аудио.



### Порядок поиска файлов звука и значков
Если имя файла звука или значка не содержит букву диска или полный путь, SimonSays ищет его в следующем порядке и использует первое найденное совпадение:

1. **Папка AppData** — `%APPDATA%\SimonSays\` (специфична для пользователя; размещайте файлы здесь, чтобы отделить их от установки приложения).
2. **Рабочий каталог** — текущий рабочий каталог процесса (проверяется только если он отличается от папки исполняемого файла).
3. **Папка исполняемого файла** — папка, содержащая `SimonSays.exe`.

Если файл не найден ни в одном из этих мест, используется встроенный резервный звук.

> **Совет:** рекомендуется хранить пользовательские звуки и значки в `%APPDATA%\SimonSays\`; они сохранятся после обновлений и будут привязаны к пользователю.

## Импорт и экспорт категорий
- Экспорт: можно экспортировать все категории или только выбранную. Сообщения об успехе/ошибке отображаются на выбранном языке (горячая клавиша: `F10`).
- Импорт: выберите файл; если категория существует, вам будет предложено перезаписать. Сообщения об успехе/ошибке отображаются на выбранном языке (горячая клавиша: `F9`).

## Устранение неполадок
- **Ошибка инициализации голоса**: убедитесь, что SAPI установлен и доступен совместимый голос.
- **Звуки не воспроизводятся**: проверьте путь/расширение файла и парные разделители `♫`.
- **Ошибки импорта/экспорта**: проверьте права доступа и расположение файла; повторите попытку.
- **Язык UI не применяется**: откройте настройки заново и подтвердите выбор языка; убедитесь, что существуют строки локализации для выбранного языка.

## О программе
- Диалог `О программе` показывает версию, описание и авторские права.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"и" },
};

static const std::vector<std::pair<int, const wchar_t *>> VALENCIAN_LOCALIZED_UI_STRINGS = {

  { PLAY_BUTTON_TEXT_ID, L"Reproduir>" },
  { PLAY_BUTTON_TEXT_PLAYING_ID, L">" },
  { CATEGORIES_BUTTON_TEXT_ID, L"Categories" },
  { SETTINGS_DIALOG_TITLE_TEXT_ID, L"Configuració" },
  { SETTINGS_DEFAULT_TEXT_LABEL_ID, L"Text predeterminat:" },
  { SETTINGS_USE_DEFAULT_TEXT_ID, L"Insereix el text predeterminat automàticament" },
  { SETTINGS_LANGUAGE_LABEL_ID, L"Idioma:" },
  { SETTINGS_VOICE_LABEL_ID, L"Veu:" },
  { SETTINGS_TEST_VOICE_BUTTON_ID, L"Provar veu" },
  { SETTINGS_VOLUME_LABEL_ID, L"Volum (10-100):" },
  { SETTINGS_RATE_LABEL_ID, L"Velocitat (-10 a 10):" },
  { SETTINGS_SPEAK_ON_CLICK_ID, L"Parla la frase immediatament quan la seleciones" },
  { SETTINGS_REMEMBER_CATEGORY_WINDOW_ID, L"Recorda la mida de la finestra de categories" },
  { SETTINGS_MINIMIZE_CATEGORY_WINDOW_ID, L"Minimitza automàticament la finestra de categories" },
  { SETTINGS_INCREASE_VOLUME_WHEN_PLAYING_ID, L"Augmenta el volum de SimonSays en reproduir" },
  { SETTINGS_REDUCE_OTHER_AUDIO_WHEN_PLAYING_ID, L"Redueix temporalment altres sons en reproduir" },
  { SETTINGS_STOP_PREVIOUS_PLAYBACK_ID, L"Atura la reproducció anterior" },
  { SETTINGS_SHOW_TOUCH_KEYBOARD_ID, L"Mostra el teclat tàctil" },
  { SETTINGS_OK_BUTTON_ID, L"D'acord" },
  { SETTINGS_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { TRAYICON_TOOLTIP_ID, L"SimonSays - Feu clic per mostrar" },
  { TRAYICON_SHOW_ID, L"Mostra" },
  { TRAYICON_HIDE_ID, L"Amaga" },
  { TRAYICON_SETTINGS_ID, L"Configuració" },
  { TRAYICON_ABOUT_ID, L"Quant a" },
  { TRAYICON_WEB_ID, L"Web" },
  { TRAYICON_EXIT_ID, L"Eixir" },
  { ABOUT_TITTLE_ID, L"Quant a SimonSays" },
  { ABOUT_1_ID, L"SimonSays - Simplement parla\n\nVersió " },
  { ABOUT_2_ID, L"\n\nUna eina d'accessibilitat senzilla que\net permet convertir text en veu a l'instant.\n\n(c) 2026 Juan Rey Saura" },
  { ERROR_TITTLE_ID, L"Error de SimonSays" },
  { ERROR_TASKBAR_POSITION_ID, L"SimonSays només admet actualment la barra de tasques inferior" },
  { EDIT_DIALOG_CATEGORY_TITLE_ID, L"Canviar nom de categoria" },
  { EDIT_DIALOG_PHRASE_TITLE_ID, L"Editar frase" },
  { EDIT_DIALOG_TEXT_LABEL_ID, L"Actual : " },
  { EDIT_DIALOG_OK_BUTTON_ID, L"D'acord" },
  { EDIT_DIALOG_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { CATEGORY_NAME_CONFLICT_MESSAGE_ID, L"Ja existeix una categoria amb este nom. Per favor, trieu un nom diferent." },
  { CATEGORY_NAME_CONFLICT_TITLE_ID, L"Conflicte de noms de categoria" },
  { DELETE_CATEGORY_CONFIRMATION_TITLE_ID, L"Eliminar categoria" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Voleu eliminar la categoria '" },
  { DELETE_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"' i totes les seues frases?" },
  { DELETE_PHRASE_CONFIRMATION_TITLE_ID, L"Eliminar frase" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE1_ID, L"Voleu eliminar la frase '" },
  { DELETE_PHRASE_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { ADD_DIALOG_CATEGORY_TITLE_ID, L"Afig nova categoria" },
  { ADD_DIALOG_PHRASE_TITLE_ID, L"Afig nova frase" },
  { ADD_DIALOG_CATEGORY_TEXT_LABEL_ID, L"Nom de la categoria :" },
  { ADD_DIALOG_PHRASE_TEXT_LABEL_ID, L"Text de la frase :" },
  { ADD_DIALOG_OK_BUTTON_ID, L"D'acord" },
  { ADD_DIALOG_CANCEL_BUTTON_ID, L"Cancel·lar" },
  { NEW_PHRASE_DEFAULT_TEXT_ID, L"Nova frase predeterminada" },
  { IMPORT_CATEGORIES_DIALOG_TITLE_ID, L"Importar categories" },
  { IMPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categories de SimonSays\0*.ssc;*.ssz\0Tots els fitxers\0*.*\0" },
  { EXPORT_CATEGORIES_DIALOG_TITLE_ID, L"Exportar categories" },
  { EXPORT_CATEGORIES_DIALOG_FILTER_ID, L"Categories de SimonSays\0*.ssc;*.ssz\0Tots els fitxers\0*.*\0" },
  { EXPORT_SUCCESS_MESSAGE_ID, L"Frases exportades correctament." },
  { EXPORT_SUCCESS_TITLE_ID, L"Exportació correcta" },
  { EXPORT_FAILURE_MESSAGE_ID, L"No s'han pogut exportar les frases." },
  { EXPORT_FAILURE_TITLE_ID, L"Error d'exportació" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE1_ID, L"Sobreescriure la categoria '" },
  { IMPORT_CATEGORY_OVERWRITE_MESSAGE2_ID, L"'?" },
  { IMPORT_CATEGORY_OVERWRITE_TITLE_ID, L"Sobreescriure categoria?" },
  { IMPORT_BOARD_STYLE_REPLACE_TITLE_ID, L"Substituir l'estil del tauler?" },
  { IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID, L"El fitxer importat inclou un estil de tauler. Voleu substituir l'actual?" },
  { IMPORT_SUCCESS_MESSAGE_ID, L"Frases importades correctament." },
  { IMPORT_SUCCESS_TITLE_ID, L"Importació correcta" },
  { IMPORT_FAILURE_MESSAGE_ID, L"No s'han pogut importar les frases." },
  { IMPORT_FAILURE_TITLE_ID, L"Error d'importació" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE1_ID, L"Pots exportar totes les categories o només la seleccionada.\nVols exportar només la categoria '" },
  { EXPORT_CATEGORY_CONFIRMATION_MESSAGE2_ID, L"'?" },
  { EXPORT_CATEGORY_CONFIRMATION_TITLE_ID, L"Exportar selecció" },
  { CATEGORY_SHORTCUTS_TEXT_ID, L"F1: Ajuda - F2: Configuració" },
  { HELP_CONTENT_ID, LR"HELP(# Guia d'Ajuda de SimonSays

## Visió general
SimonSays és un assistent de text a veu (TTS) per a Windows, creat per Juan Rey Saura, optimitzat per a parla ràpida, biblioteques de frases i reproducció de sons en línia. Suporta múltiples idiomes, veus personalitzables i funcions pensades per a una comunicació eficient.

SimonSays utilitza l'espai lliure a l'esquerra de la barra de tasques de Windows, proporcionant accés ràpid a una dotzena de categories de frases d'ús freqüent (i personalitzables) amb un parell de clics.

## Dreceres de teclat
- `F1`: Obrir Ajuda.
- `F2`: Obrir Configuració.
- `F3`: Configuració de Mirada / Clic per fixació.
- `F4`: Editar la selecció actual.
- `F5` / `F6`: Moure a la selecció anterior / següent en llistes.
- `F7`: Afegir després de la selecció actual (context categoria/frase).
- `F8`: Eliminar la selecció actual.
- `F9`: Importar categories.
- `F10`: Exportar categories.
- `F11` / `Ctrl -`: Allunyar el zoom (finestra de categories).
- `F12` / `Ctrl +`: Apropar el zoom (finestra de categories).
- `Ctrl 0`: Restablir el zoom.
- Botó `Reproduir>`: Iniciar reproducció.

## Inici i ús bàsic
1. Inicia SimonSays; apareix a la safata del sistema de Windows.
2. Per parlar, pots:
   - Escriure text personalitzat a la caixa d'entrada i prémer el botó `Reproduir>` (o `Enter`).
   - Fer clic en una o diverses frases desades a la finestra de Categories.
3. Durant la reproducció, el botó mostra `>` per indicar que està actiu. Pots prémer-lo de nou per aturar-lo instantàniament.

## Finestra de Categories (frases)
- Obre-la amb el botó `Categories`. Pots redimensionar-la o recordar-ne la grandària des de Configuració.
- Selecciona una categoria a dalt i frases a baix.
- Un clic en una frase la reprodueix automàticament si `Parla la frase immediatament quan la selecciones` està activat; si no, carrega la frase en la caixa d'entrada principal sense parlar.

### Gestionar categories
- Reanomenar: selecciona categoria → (drecera: `F4`) `Canvia el nom de la categoria` → canvia el nom → `D'acord`.
- Moure selecció: `F5`/`F6` per moure a l'element anterior/següent.
- Afegir: selecciona categoria → (drecera: `F7`) diàleg `Afegeix una nova categoria` → escriu el nom → `D'acord`.
- Eliminar: selecciona categoria → (drecera: `F8`) `Eliminar categoria`; confirma l'eliminació de la categoria i les seues frases.



#### Definir o canviar la icona d’una categoria
Cada categoria pot mostrar un emoji o un fitxer `.ico` personalitzat en el seu botó. La icona s’especifica en el diàleg Afegir/Editar utilitzant el prefix separador `##`:

```
<icona>##<nom de categoria>
```

- **Emoji**: introduïx l’emoji seguit de `##` i del nom.
  - Exemple: `🙂##Salutacions`
- **Fitxer d’icona** (`.ico`): introduïx la ruta del fitxer (absoluta o relativa a la carpeta de SimonSays) seguida de `##` i del nom.
  - Exemple: `C:\icons\smile.ico##Salutacions`
- **Sense icona**: introduïx només el nom de la categoria, sense el prefix `##`.
  - Exemple: `Salutacions`

> Nota: només s’admeten fitxers `.ico` per a icones basades en fitxer. Qualsevol altra extensió s’ignora i no es mostra cap icona.

### Gestionar frases
- Editar: selecciona frase → (drecera: `F4`) `Edita la frase` → actualitza el text → `D'acord`.
- Moure selecció: `F5`/`F6` per moure a la frase anterior/següent.
- Afegir: selecciona frase → (drecera: `F7`) `Afegeix una nova frase` → escriu el text → `D'acord`.
- Eliminar: selecciona frase → (drecera: `F8`) `Eliminar frase`; confirma.



#### Definir o canviar la icona d’una frase
Les frases seguixen la mateixa convenció de prefix d’icona que les categories, amb el separador opcional `##` davant del text de la frase:

```
<icona>##<text de la frase>
```

- **Emoji**: `👍##Sona bé!`
- **Fitxer d’icona**: `C:\icons\check.ico##Sona bé!`
- **Sense icona**: `Sona bé!`

Les frases també poden incloure un fitxer d’àudio en línia. Quan combines icona, text i fitxer d’àudio, el format complet és:

```
<icona>##<text de la frase>::<fitxer d’àudio>
```

- Exemple: `🔔##Atenció::notification.wav`

El prefix `##` i el sufix `::` són opcionals i independents; pots utilitzar qualsevol combinació:
- Només prefix d’icona: `🔔##Atenció`
- Només sufix d’àudio: `Atenció::notification.wav`
- Tots dos: `🔔##Atenció::notification.wav`
- Cap dels dos: `Atenció`

## Configuració (F2)
Obre Configuració (`F2`) per controlar:
- **Text predeterminat**: activa/desactiva `Insereix el text predeterminat automàticament` i personalitza la frase predeterminada.
  - `Mostra el teclat tàctil` — obri el teclat tàctil en pantalla quan la caixa d'entrada rep el focus, situat prop de la finestra principal.
- **Idioma de la interfície**: tria l'idioma de la UI (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Russian, Valencian).
- **Veu**: selecciona una veu SAPI instal·lada segons la teua preferència.
- **Volum/Velocitat**: ajusta `Volum (10-100)` i `Velocitat (-10 a 10)`.
- `Provar veu`: prova la veu seleccionada.
- **Finestra de categories**:
  - `Minimitza automàticament la finestra de categories` després de seleccionar.
  - `Recorda la grandària de la finestra de categories`.
- **Comportament de reproducció**:
  - `Parla la frase immediatament quan la selecciones` (auto-reproducció en fer clic).
  - `Atura la reproducció anterior` — atura automàticament qualsevol reproducció en curs abans d'iniciar una nova frase.
  - `Augmenta el volum de SimonSays en reproduir` — puja el volum mestre del sistema mentre parla i el restaura en acabar la reproducció.
  - `Reduïx temporalment altres sons en reproduir` — reduïx el volum de les altres aplicacions mentre parla i el restaura en acaba)HELP" LR"HELP(r la reproducció.
- `D'acord` desa els canvis; `Cancel·lar` els descarta.

## Mirada / Clic per fixació (F3)
Activa frases i botons sense fer servir les mans mantenint la mirada (o el cursor del ratolí) sobre ells durant un temps determinat. Esta funció és nova: necessita proves i comentaris; per favor, conta'ns com funciona amb el teu seguidor ocular mitjançant l'opció `Comentaris` del menú de safata.

Configuracions compatibles:
- **Qualsevol programari de control ocular que moga el cursor del ratolí** (Irisbond EasyClick, Tobii Dynavox Computer Control, modes de cursor del Control ocular de Windows, OptiKey, ...): el clic per fixació seguix el cursor.
- **Lectura directa de la mirada, sense necessitat de moure el cursor**: Irisbond Hiru (mode HID) i seguidors Tobii a través del seu propi programari instal·lat — verificat amb Tobii Eye Tracker 4C i Tobii Dynavox PCEye 5; s'espera que Tobii Eye Tracker 5 funcione, però encara no s'ha provat.

Obri la configuració de Mirada / Clic per fixació (`F3`) per ajustar (els controls d'este diàleg es mostren en anglés):
- **Mode d'activació**: tria entre `Automatic`, que detecta la presència i el comportament del dispositiu i decidix si activar el clic per fixació mitjançant el cursor del ratolí o mitjançant detecció de mirada (si es detecta un dispositiu de seguiment ocular o programari relacionat, el clic per fixació s'activa; si no se'n detecta cap, resta desactivat), `Mouse cursor` per activar sempre el clic per fixació mitjançant el cursor, `HID eye tracker` per activar-lo sempre mitjançant detecció de mirada, o `Off` per desactivar-lo sempre, independentment dels dispositius presents.
- **Temps de fixació** (`Dwell time`): durada (en mil·lisegons) necessària per disparar un clic en mirar un element. Mentre la fixació és en curs es mostra un indicador de progrés.
- **Radi de tolerància** (`Tolerance radius`): tolerància de la mirada (en píxels) per permetre xicotets moviments de l'ull sense cancel·lar el clic per fixació.
- **Temps d'espera** (`Cooldown`): mil·lisegons després d'una activació durant els quals no es dispararà cap altre clic per fixació (evita clics repetits accidentals).
- **Color de progrés** (`Progress color`): color de l'indicador de progrés de la fixació (per defecte, el color d'èmfasi de Windows).
- **Detecta com uses el seguidor** (`Detect how you use the tracker`): dos botons que ajuden l'aplicació a detectar si mous el cursor amb un seguidor ocular o si simplement fas clic als botons.
- **Detectat** (`Detected`): mostra el tipus de dispositiu detectat i informació addicional sobre el programari relacionat.
- Un botó `Reset` (a baix a l'esquerra) restaura tots els ajustos de fixació als valors predeterminats.
- El botó `D'acord` desa els canvis; `Cancel·lar` els descarta.

## Menú de la icona de safata
- `Mostra` / `Amaga` la interfície principal.
- `Configuració`.
- `Gaze / Dwell-click` (Mirada / Clic per fixació).
- `Quant a` mostra la versió i els crèdits.
- `Web` obre la pàgina del projecte (si està configurada).
- `Comentaris` obri el [formulari de comentaris](https://forms.gle/KMbpEDYmwnFJUhgy8) en el navegador predeterminat.
- `Eixir` tanca SimonSays.

Pots moure la finestra emergent de la safata i altres diàlegs arrossegant la barra de títol; redimensiona la finestra de Categories arrossegant les vores, i activa `Recorda la grandària de la finestra de categories` en Configuració per conservar la grandària.

## Barrejar veu i sons
- Utilitza el delimitador `♫` (`SOUND_NOTE_DELIMITER`) per inserir noms de fitxers d'àudio al text:
  - Exemple: `Hola ♫notification.wav♫, espera un moment, per favor.`
  - El text fora dels delimitadors es parla; el text entre delimitadors es tracta com un fitxer de so i es reprodueix en línia.
- Àudio admès: `.wav`, `.mid`, `.midi`, `.mp3`.
- Utilitza rutes absolutes o relatives accessibles al procés. Aparella delimitadors per evitar que el text final sigui tractat com a àudio.



### Orde de busca de fitxers de so i d’icona
Quan un nom de fitxer de so o d’icona no conté una lletra d’unitat o una ruta completa, SimonSays el busca en el següent orde i utilitza la primera coincidència trobada:

1. **Carpeta AppData** — `%APPDATA%\SimonSays\` (específica de l’usuari; posa els fitxers ací per a mantindre’ls separats de la instal·lació de l’aplicació).
2. **Directori de treball** — el directori de treball actual del procés (només es busca si és diferent de la carpeta de l’executable).
3. **Carpeta de l’executable** — la carpeta que conté `SimonSays.exe`.

Si el fitxer no es troba en cap d’estes ubicacions, s’utilitza el so de reserva integrat.

> **Consell:** es recomana posar els teus fitxers de so i d’icona personalitzats en `%APPDATA%\SimonSays\`; així sobreviuran a les actualitzacions i seran específics de l’usuari.

## Importar i exportar categories
- Exportar: pots exportar totes les categories o només la seleccionada quan se't demani. Els missatges d'èxit/error apareixen en l'idioma seleccionat (drecera: `F10`).
- Importar: tria un fitxer; si la categoria existeix, se't demanarà sobreescriure-la. Els missatges d'èxit/error apareixen en l'idioma seleccionat (drecera: `F9`).

## Resolució de problemes
- **Error d'inicialització de veu**: verifica que SAPI estigui instal·lat i que hi hagi una veu compatible.
- **No es reprodueixen sons**: confirma la ruta/extensió i els delimitadors `♫` aparellats.
- **Errors d'importació/exportació**: comprova els permisos i la ubicació del disc; reintenta.
- **Idioma UI no aplicat**: reobre Configuració i confirma la selecció d'idioma; assegura't que existeixin cadenes de localització per a l'idioma triat.

## Quant a
- El diàleg `Quant a` mostra la versió, la descripció i el copyright.
)HELP" },
  // DO NOT EDIT THE PREVIOUS STRING, HELP_CONTENT_ID is set by sync_help_content.ps1 script (must change encoding to UTF-8 with BOOM later)
  { AMPERSAND_REPLACEMENT_ID, L"i" },
  { DWELL_DIALOG_TITLE_ID, L"Mirada / Clic per fixació" },
  { DWELL_INTRO_ID, L"Activa els botons mantenint-hi la mirada (o el cursor) a sobre." },
  { DWELL_MODE_GROUP_ID, L"Mode d'activació" },
  { DWELL_MODE_AUTO_ID, L"Automàtic" },
  { DWELL_MODE_MOUSE_ID, L"Cursor del ratolí" },
  { DWELL_MODE_HID_ID, L"Seguidor ocular HID" },
  { DWELL_MODE_OFF_ID, L"Desactivat" },
  { DWELL_TIME_LABEL_ID, L"Temps de fixació (ms)" },
  { DWELL_TOLERANCE_LABEL_ID, L"Radi de tolerància (px)" },
  { DWELL_COOLDOWN_LABEL_ID, L"Temps d'espera (ms)" },
  { DWELL_COLOR_BUTTON_ID, L"Color de progrés..." },
  { DWELL_DETECT_GROUP_ID, L"Detecta com fas servir el seguidor" },
  { DWELL_PROBE_LOOK_ID, L"Activa'm MIRANT-ME" },
  { DWELL_PROBE_MOUSE_ID, L"Activa'm amb el RATOLÍ o commutador" },
  { DWELL_STATUS_INITIAL_ID, L"Prova els dos botons de dalt per definir el millor mode automàticament." },
  { DWELL_STATUS_GAZE_ID, L"Detectat: cursor controlat per la mirada. Clic per fixació activat." },
  { DWELL_STATUS_MOUSE_ID, L"Detectat: ratolí manual. Clic per fixació desactivat." },
  { DWELL_STATUS_LOOK_HINT_ID, L"Si us plau, MIRA el botó sense fer-hi clic." },
  { DWELL_APPLY_BUTTON_ID, L"Aplica" },
  { DWELL_HID_UNAVAILABLE_ID, L"El seguiment ocular HID encara no està disponible." },
  { TRAYICON_DWELL_ID, L"Mirada / Clic per fixació..." },
  { DWELL_SIGNALS_GROUP_ID, L"Detectat" },
  { DWELL_SIGNAL_HID_LABEL_ID, L"Seguidor ocular (HID):" },
  { DWELL_SIGNAL_TOOL_LABEL_ID, L"App de control ocular:" },
  { DWELL_SIGNAL_WEC_LABEL_ID, L"Control ocular de Windows:" },
  { DWELL_SIGNAL_YES_ID, L"sí" },
  { DWELL_SIGNAL_NO_ID, L"no" },
  { DWELL_SIGNAL_NONE_ID, L"cap" },
  { DWELL_SIGNAL_HID_LIVE_ID, L"transmetent la mirada" },
  { DWELL_SIGNAL_HID_IDLE_ID, L"present (sense transmetre)" },
  { DWELL_RESET_BUTTON_ID, L"Restableix" },
  { BOARD_STYLE_CHANGED_TITLE_ID, L"Estil del tauler canviat" },
  { BOARD_STYLE_CHANGED_MESSAGE_ID, L"Vols revertir els canvis?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE1_ID, L"Eliminar totes les categories i les seues frases?\nAvís: aquesta acció no es pot desfer." },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_MESSAGE2_ID, L"És un error?" },
  { DELETE_ALL_CATEGORIES_CONFIRMATION_TITLE_ID, L"Eliminar totes les categories" },
  { TRAYICON_FEEDBACK_ID, L"Dona'ns la teua opinió" },
};

static const  std::vector < std::pair < std::wstring, std::vector<std::pair<int, const wchar_t *>>>> LOCALIZED_STRINGS = {
  { L"English", DEFAULT_LOCALIZED_UI_STRINGS },
  { L"Spanish", SPANISH_LOCALIZED_UI_STRINGS },
  { L"Arabic", ARABIC_LOCALIZED_UI_STRINGS },
  { L"Basque", BASQUE_LOCALIZED_UI_STRINGS },
  { L"Catalan", CATALAN_LOCALIZED_UI_STRINGS },
  { L"Chinese (Simplified)", CHINESE_SIMPLIFIED_LOCALIZED_UI_STRINGS },
  { L"French", FRENCH_LOCALIZED_UI_STRINGS },
  { L"Galician", GALICIAN_LOCALIZED_UI_STRINGS },
  { L"German", GERMAN_LOCALIZED_UI_STRINGS },
  { L"Hebrew", HEBREW_LOCALIZED_UI_STRINGS },
  { L"Hindi", HINDI_LOCALIZED_UI_STRINGS },
  { L"Italian", ITALIAN_LOCALIZED_UI_STRINGS },
  { L"Japanese", JAPANESE_LOCALIZED_UI_STRINGS },
  { L"Korean", KOREAN_LOCALIZED_UI_STRINGS },
  { L"Portuguese", PORTUGUESE_LOCALIZED_UI_STRINGS },
  { L"Russian", RUSSIAN_LOCALIZED_UI_STRINGS },
  { L"Valencian", VALENCIAN_LOCALIZED_UI_STRINGS }
};

#endif // localized_strings_h
