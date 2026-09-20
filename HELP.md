# SimonSays Help Guide

## Overview
SimonSays is a Windows text-to-speech (TTS) assistant created by Juan Rey Saura for people who find speaking difficult or impossible — for example because of ALS, dysarthria, a laryngectomy, apraxia of speech, a stroke or a brain injury. It is optimized for fast speech, phrase libraries, and inline sound playback. It supports multiple languages, customizable voices, and features designed for efficient communication.

SimonSays uses the empty space to the left of the Windows taskbar, providing quick access to a dozen categories of frequently used (and customizable) phrases with just a couple of clicks. Because it never covers your other windows, you can stay in your browser, documents or video call and still speak whenever you need to — with the keyboard, the mouse, or hands-free with an eye tracker.

Use Cases:
- Intensive use of multiple Windows applications without losing focus on communication.
- Boards created by speech therapists for people with speech disabilities.
- Online meetings using Virtual CABLE (see [Using SimonSays in online meetings](#using-simonsays-in-online-meetings)).

SimonSays is free and open source, with no ads and no data collection.

## Contents
- [Getting started](#getting-started)
- [Categories window](#categories-window)
- [Default phrases](#default-phrases)
- [Shortcuts](#shortcuts)
- [Settings (F2)](#settings-f2)
- [Gaze / Dwell-click (F3)](#gaze--dwell-click-f3)
- [Using SimonSays in online meetings](#using-simonsays-in-online-meetings)
- [Tray icon menu](#tray-icon-menu)
- Editing boards (advanced):
  - [Editing categories and phrases](#editing-categories-and-phrases)
  - [Mixing speech and sounds](#mixing-speech-and-sounds)
  - [Customizing the look (board & category styles)](#customizing-the-look-board--category-styles)
  - [Importing and exporting categories](#importing-and-exporting-categories)
  - [Undoing changes](#undoing-changes)
  - [Where SimonSays keeps your files](#where-simonsays-keeps-your-files)
- [Troubleshooting](#troubleshooting)
- [Feedback, support and privacy](#feedback-support-and-privacy)

## Getting started
1. Start SimonSays. It appears in the empty space on the left of the Windows taskbar and adds an icon to the system tray. The taskbar must be at the bottom of the screen.
2. To speak, you can either:
   - Type custom text in the input box and press the `Play>` button (or `Enter`).
   - Click one or more saved phrases in the Categories window.
3. While playing, the button shows `>` to indicate active playback. You can press it again to stop instantly.

Next to the `Categories` button, two small **quick access buttons** give one-click access to the two things you would otherwise reach through a shortcut or the tray menu:

- 📂 opens a board saved on your computer — the same import as `F9`, but starting in your boards folder (`%LocalAppData%\SimonSays\boards\`), where exported and downloaded boards are kept (see [Importing and exporting categories](#importing-and-exporting-categories)).
- ⚙️ opens Settings, the same as `F2`.

Both can be hidden with `Show quick access buttons` in Settings (`F2`), which leaves more room for the input box.

If SimonSays is hidden, click its tray icon to show it again.

## Categories window
- Open via the `Categories` button. Resize it or enable "Remember category window size" in Settings to persist the size.
- Select a category from the top list and phrases from the bottom list.
- Single-click a phrase to auto-speak if `Speak phrase immediately when selecting` is enabled; otherwise the phrase will load into the main input box without speaking.
- Double-click an empty area or the edge of the window to resize it so that all its categories and phrases fit.
- Drag an empty area of the window to move it, and press `Esc` to hide it.
- Zoom in and out with `F12` and `F11` (see [Shortcuts](#shortcuts)).
- The strip between the categories and the phrases shows a reminder of the main shortcuts (or the board's caption, if it has one).

## Default phrases
SimonSays comes with a ready-made set of categories and phrases in each of its 18 languages, ordered so that the phrases needed most often take the fewest selections:

- 👋🏻 Frequent Greetings
- 🗣️ Conversation Phrases
- 🚨 Emergency Phrases
- 🤲 Care & Comfort
- 🏥 Health Phrases
- 🏠 Family & Home
- 👥 Social Phrases
- ⏰ Time & Date
- 💼 Business Phrases
- ✈️ Travel Phrases
- 🔊 Sounds

Each language has its own categories and phrases: when you change the language in Settings (`F2`), the Categories window switches to that language's board, and changes you make in one language do not affect the others.

Phrases are spoken with the voices installed in Windows. If no voice is installed for your language, the phrases still appear but may be spoken with another voice (see [Troubleshooting](#troubleshooting)).

You can change the default phrases or get them back later — see [Editing categories and phrases](#editing-categories-and-phrases) and [Restoring the default phrases](#restoring-the-default-phrases).

## Shortcuts
- `F1`: Open Help
- `F2`: Open Settings
- `F3`: Dwell settings
- `Enter`: Speak the text in the input box
- `Esc`: Hide the Categories window
- `F11` / `Ctrl -`: Zoom out (Categories window)
- `F12` / `Ctrl +`: Zoom in (Categories window)
- `Ctrl + 0`: Reset zoom

Shortcuts for editing categories, phrases and boards are listed in [Editing categories and phrases](#editing-categories-and-phrases).

## Settings (F2)
Open Settings (`F2`) to control:
- **Default text**: toggle `Insert default text automatically` and customize the default phrase.
  - `Show Touch Keyboard` — opens the on-screen touch keyboard when the input box receives focus; it is positioned near the main window.
- `Show quick access buttons` — shows the 📂 (open a board) and ⚙️ (Settings) buttons next to the `Categories` button in the main window (see [Getting started](#getting-started)). Turn it off to hide both and give the input box more room.
- **UI language**: choose the interface language (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Portuguese (Brazil), Russian, Valencian). Each language also has its own categories and phrases (see [Default phrases](#default-phrases)).
- **Voice**: pick an installed SAPI voice matching your language preference.
- **Volume/Rate**: adjust `Volume (10-100)` and `Rate (-10 to 10)`.
- `Test Voice`: audition the selected voice.
- **Category window**:
  - `Minimize category window automatically` after selecting.
  - `Remember category window size`.
  - `Auto-resize category window to fit content` — resizes the Categories window to fit its categories and phrases whenever they are loaded, for example at start-up, after changing the language, or after an import.
- **Playback behavior**:
  - `Speak phrase immediately when selecting` (single-click auto-speak).
  - `Stop Previous Playback` — automatically stops any ongoing playback before starting a new phrase.
  - `Increase SimonSays volume when playing` — raises the system master volume while speaking and restores the previous level when playback finishes.
  - `Temporarily reduce other audio when playing` — reduces the volume of other applications while speaking and restores them when playback finishes.
- `OK` saves changes; `Cancel` discards.

By default, `Speak phrase immediately when selecting`, `Minimize category window automatically`, `Auto-resize category window to fit content`, `Increase SimonSays volume when playing` and `Temporarily reduce other audio when playing` are on, and `Stop Previous Playback` is off.

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

## Using SimonSays in online meetings
SimonSays can speak straight into a video call (Teams, Zoom, Google Meet…), so the other participants hear your phrases as if they came from your microphone. You need a virtual audio cable, such as the free VB-Audio Virtual Cable:

1. Install the virtual audio cable and restart Windows if asked.
2. Send SimonSays' sound to the cable: in Windows open **Settings → System → Sound → Volume mixer** (Windows 11) or **Settings → System → Sound → App volume and device preferences** (Windows 10) and set the output device of SimonSays to `CABLE Input`. SimonSays only appears in that list after it has played something, so press `Play>` once first.
3. In your meeting app, choose `CABLE Output` as the microphone.

To hear SimonSays yourself as well, open the classic Sound control panel, select `CABLE Output` on the **Recording** tab, open **Properties → Listen**, tick **Listen to this device** and choose your speakers or headset.

> Note: this relies on Windows' own per-app sound settings; SimonSays does not yet let you choose an output device itself.

## Tray icon menu
- `Show` / `Hide` main UI
- `Settings`
- `Gaze / Dwell-click`
- `Web` opens the SimonSays web page
- `About` shows version and credits
- `Feedback` opens a [feedback form](https://forms.gle/KMbpEDYmwnFJUhgy8) in the default browser
- `Exit` quits SimonSays

You can move the tray pop-up window and other dialogs by dragging their title bars; resize the Categories window by dragging its edges, and enable `Remember category window size` in Settings to persist the size.

## Editing categories and phrases
This section and the ones after it are for caregivers, speech therapists and advanced users who change the categories, phrases, sounds and look of a board. Every change is saved immediately (see [Undoing changes](#undoing-changes)).

Editing shortcuts (Categories window):
- `F4`: Edit current selection
- `F5` / `F6`: Move to previous / next selection in lists
- `F7`: Add after current selection (category/phrase context)
- `F8`: Delete current selection
- `F9`: Import categories
- `F10`: Export categories
- `Ctrl + F4`: Edit the board style (see [Customizing the look](#customizing-the-look-board--category-styles))
- `Ctrl + F8`: Delete all categories and their phrases (asks twice for confirmation)

> **Tip:** Export your board (`F10`) before using `Ctrl + F8` or `Ctrl + F9`, so that you can get it back.

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
- **Icon file** (`.ico`, `.png`, `.jpg`): enter the file name or full path followed by `##` and the name. A file name without a folder is searched for as described in [Sound and icon file lookup order](#sound-and-icon-file-lookup-order).
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

- Example: `🔔##Attention::alert.mp3`

The `##` prefix and `::` suffix are both optional and independent; you can use any combination:
- Icon only prefix: `🔔##Attention`
- Audio only suffix: `Attention::alert.mp3`
- Both: `🔔##Attention::alert.mp3`
- Neither: `Attention`

## Mixing speech and sounds
- Use the delimiter `♫` to embed audio filenames in text:
  - Example: `Hello ♫doorbell.mp3♫, please wait.`
  - Text outside delimiters is spoken; text inside is treated as a sound file and played inline.
- Supported audio: `.wav`, `.mid`, `.midi`, `.mp3`.
- Use absolute or relative paths accessible to the process. Ensure delimiters are paired to avoid trailing text being treated as audio.
- To type `♫`, copy it from this help, or select a phrase that has a sound: its `♫…♫` marker is placed in the input box, where you can add text around it.

### Sounds included with SimonSays
These sounds are installed with SimonSays and can be used by name in any text or phrase — for example `♫laugh.mp3♫` in the input box, or `Well done!::applause.mp3` for a phrase button labelled "Well done!" that plays applause:

`alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `laugh.mp3`, `phone_ring.mp3`, `scream.mp3`, `shots.mp3`

### Sound and icon file lookup order
When a sound or icon filename does not contain a drive letter or full path, SimonSays searches for it in the following order, using the first match found:

1. **The current board's resource subfolder** — if the board's style sets a `title` or `resource-folder`, its own subfolder inside the resources folder (see [Customizing the look](#customizing-the-look-board--category-styles)).
2. **Resources folder** — `%LocalAppData%\SimonSays\resources\` (the default home for board icons and sounds; imported `.ssz` bundles install their files here).
3. **SimonSays app-data folder** — `%LocalAppData%\SimonSays\` (kept as a fallback for files placed here by earlier versions, before the resources folder existed).
4. **Working directory** — the current working directory of the process (only searched if it differs from the executable folder).
5. **Executable folder** — the folder containing `SimonSays.exe`.

If the file is not found in any of these locations, the built-in fallback sound is used instead (an icon just shows nothing).

> **Tip:** We recommend placing custom sound and icon files in `%LocalAppData%\SimonSays\resources\`; they will survive application updates and are user-specific. A board with a `title` or `resource-folder` style gets its own subfolder there automatically, keeping its files separate from other boards.

## Customizing the look (board & category styles)
You can color, resize, and restyle the Categories window and its buttons — for example to build color-coded communication boards such as Fitzgerald Key. Styles are optional: without them the window keeps its default look.

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

- Example: `🧍##People::background:#FFD966;` colors the People button yellow. The category's color **also applies to the phrases inside it**, so one line colors a whole word group — the core of an Fitzgerald Key board.
- A category style can set `background` and `text-color` for the category button, and any phrase property (below) prefixed with `phrase-`, e.g. `phrase-corner-radius:8;`.

### Styling the whole board (`$$board`)
The overall look — window background, separators, and defaults for every category and phrase button — is a **board style**. It is stored under the reserved category name `$$board` (you cannot use `$$` names for your own categories).

- **Edit it** by pressing `Ctrl + F4` in the Categories window. The editor shows one `property:value;` per line. When you confirm with `OK`, the new style is applied and SimonSays asks "Do you want to revert the changes?": choose `OK` to go back to the previous style, or `Cancel` to keep the new one.
- **Get one** by importing a styled categories file (`.ssc` / `.ssz`). When you import a file that contains a board style, SimonSays asks before replacing your current one, and shows the board's `title` and `credits` (if any) in the import message.

### Style properties
- **Whole window**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (label shown between the separators), `title` and `credits` (shown when the board is imported).
- **All category buttons** (`category-` prefix) and **all phrase buttons** (`phrase-` prefix): `background`, `text-color`, `width`, `height`, `corner-radius`, `border-width`, `margin`, `icon-position` (`left`/`right`/`top`/`bottom`/`center`), `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.

Setting `icon-position` to `center` places the icon in the middle of the button and hides the text (an icon-only key); use `top` instead if you want the icon above with the name below. If you do want text over a centered icon, add an explicit `text-layout` (for example `text-layout:bottom`) and it will be shown at that position.

`text-layout` accepts a horizontal keyword (`left`/`center`/`right`) and/or a vertical one (`top`/`middle`/`bottom`), or `smart`. The default is smart: the text is centered in the space left over by the icon (icon on top → caption centered below; centered in the whole button when there is no icon).

### Example: an Fitzgerald Key color-coded board
To color six word groups, edit each category (`F4`) and give it one background color. These are the exact strings you type in the Edit dialog:

```
🧍##People::background:#FFD966;
🏃##Verbs::background:#93C47D;
📏##Describe::background:#6FABDC;
🍎##Nouns::background:#F6B26B;
🕒##Misc::background:#FFFFFF;
👋##Social::background:#B4A7D6;
```

Each color also colors the phrases inside that category, so the whole board becomes color-coded. Board-wide touches — such as putting every category's emoji on top with its name below, and coloring the separators — come from a board style (`Ctrl + F4`) like:

```
category-icon-position:top;separator-color:#B4A7D6;
```

Styles travel with your categories: exporting (`F10`) includes them, and importing (`F9`) brings them along, so a styled board can be shared as a single file.

## Importing and exporting categories
Boards are shared as `.ssc` files (categories and phrases) or `.ssz` files (the same, plus the sound and icon files they use, in a single file).

- **Export** (`F10`): choose to export all categories or only the selected one when prompted; the Save dialog opens in `%LocalAppData%\SimonSays\boards\` by default, though you can still save anywhere. SimonSays saves an `.ssz` automatically when your categories or phrases use sound or icon files it can find (`.mid`/`.midi` sounds are not included). Exporting all categories includes the board style; exporting one category includes only that category's style. Success/failure messages appear in the selected UI language.
- **Import** (`F9`): choose a `.ssc` or `.ssz` file.
  - A file with **several categories or a board style** is a whole board: it **replaces all the categories and phrases of the current language**. Your previous board is saved first as `backup.ssz` (see [Undoing changes](#undoing-changes)).
  - A file with **a single category** is added to your board; if a category with the same name exists, you will be asked to overwrite it.
  - Sound and icon files inside an `.ssz` are installed in the resources folder, and the Categories window resizes to fit the imported board. Success/failure messages appear in the selected UI language.

## Undoing changes
SimonSays saves every change straight away and has no general Undo command. These are the ways to go back:

- **Undo an import**: before an import replaces your board, SimonSays saves the previous board as `backup.ssz` in `%LocalAppData%\SimonSays\boards\`. To undo the import, press `F9` and choose `backup.ssz` (answer `Yes` if you are asked to replace the board style). Only the most recently replaced board is kept: each new import of a whole board overwrites the backup.
- **Undo a board style edit**: after you edit the board style with `Ctrl + F4`, SimonSays asks "Do you want to revert the changes?" — choose `OK` to go back to the previous style.
- **Undo a category or phrase edit**: changes made with `F4`, `F7` or `F8` cannot be undone automatically. Before making big changes to a board you care about, export it (`F10`); importing that file later (`F9`) takes you back to that point.

### Restoring the default phrases
SimonSays never overwrites your own categories and phrases, so new default phrases from an update only appear in languages you have not used yet. To reinstall the built-in categories and phrases for one language — this **replaces everything in that language**:

1. Export your board (`F10`) if you want to keep a copy.
2. Exit SimonSays from the tray menu (`Exit`).
3. Open the Registry Editor (`regedit`), go to `HKEY_CURRENT_USER\SOFTWARE\SimonSays\Phrases\` and delete the key named after the language (for example `English`).
4. Start SimonSays again: the default categories and phrases for that language are created again.

## Where SimonSays keeps your files
- **Categories, phrases and settings** are stored in the Windows registry under `HKEY_CURRENT_USER\SOFTWARE\SimonSays` (each language's board under `Phrases\<Language>`).
- `%LocalAppData%\SimonSays\resources\` — your sound and icon files, and those installed from `.ssz` boards; a board whose style sets a `title` or `resource-folder` gets its own subfolder.
- `%LocalAppData%\SimonSays\boards\` — the default folder for exported boards, and the automatic `backup.ssz`.
- `%LocalAppData%\SimonSays\debug\` — diagnostic files (such as `hid_dump.txt`) that support may ask for.

To open one of these folders, paste its path (for example `%LocalAppData%\SimonSays`) into the File Explorer address bar.

## Troubleshooting
- **Voice initialization fails**: verify SAPI is installed and a compatible voice is present.
- **No voice for my language, or phrases spoken with the wrong accent**: SimonSays uses the voices installed in Windows. Add a voice for your language in Windows Settings (**Time & language → Speech → Manage voices**), then choose it in Settings (`F2`). Some newer Windows "natural" voices may not be available to other apps.
- **No sound playback**: confirm file path/extension and paired delimiters `♫`.
- **An error says SimonSays only supports a bottom taskbar**: move the Windows taskbar to the bottom of the screen and start SimonSays again.
- **The Categories window is too small or too large**: double-click an empty area or the edge of the window to fit it to its content, or zoom with `F11` / `F12`.
- **My categories or phrases disappeared**: check the language in Settings (`F2`) — each language has its own board. If an import replaced your board, see [Undoing changes](#undoing-changes).
- **Import/export errors**: check file permissions and disk location; retry.
- **UI language or voice changes not applied**: reopen Settings and confirm the selection. If changes still do not apply, restart SimonSays and try again.
- **Reporting eye-tracking hardware issues**: if support asks for diagnostics, the gaze/eye-tracker dump (`hid_dump.txt`) is written to `%LocalAppData%\SimonSays\debug\`.

## Feedback, support and privacy
- The `About` dialog (tray menu) lists version, description, and copyright.
- Share suggestions, problems, or how SimonSays works with your eye tracker through the tray menu `Feedback` option, which opens a [feedback form](https://forms.gle/KMbpEDYmwnFJUhgy8).
- For bug reports and technical support, visit the [SimonSays GitHub repository](https://github.com/juan-rey/SimonSays).
- SimonSays is free and open source under the Open Software License 3.0 (OSL-3.0). It shows no ads and does not collect your data.
