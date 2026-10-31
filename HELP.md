# SimonSays Help Guide

## Overview
SimonSays is a Windows text-to-speech (TTS) assistant created by Juan Rey Saura for people who find speaking difficult or impossible — for example because of ALS, dysarthria, a laryngectomy, apraxia of speech, a stroke or a brain injury. It is optimized for fast speech, phrase libraries, and inline sound playback. It supports multiple languages, customizable voices, and features designed for efficient communication.

SimonSays uses the empty space on the Windows taskbar, providing quick access to a dozen categories of frequently used (and customizable) phrases with just a couple of clicks. Because it never covers your other windows, you can stay in your browser, documents or video call and still speak whenever you need to — with the keyboard, the mouse, or hands-free with an eye tracker.

Use Cases:
- Intensive use of multiple Windows applications without losing focus on communication.
- Boards created by speech therapists for people with speech disabilities.
- Online meetings using [Virtual CABLE](https://vb-audio.com/Cable/) (see [Using SimonSays in online meetings](#using-simonsays-in-online-meetings)).

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
1. Start SimonSays. It appears in an empty space on the Windows taskbar and adds an icon to the system tray. The taskbar can be at the bottom, top, left or right of the screen:
   - When the taskbar icons are centered, SimonSays sits at the very start of the taskbar (the left end, or the top on a side taskbar). When they are left-aligned, it sits just before the system tray.
   - On a small (thin) taskbar the bar becomes shorter, with a smaller font and the two quick access buttons side by side. On a taskbar at the left or right of the screen, the controls are stacked one under the other.
   - The Categories window opens next to the taskbar, and the arrow on the `Categories` button points to where it opens.
   - If you move or resize the taskbar while SimonSays is running, it follows within a few seconds.
2. To speak, you can either:
   - Click the text box on the bar (or dwell on it): a larger input box opens next to the taskbar. Type your text and press `Enter` to speak it — the box closes and the focus moves to `Play>`, so `Space` speaks the text again (or stops it). `Esc` closes the box without speaking and takes you back to the window you were using; your text stays in the bar, where `Play>` speaks it again.
   - Click one or more saved phrases in the Categories window.
3. While playing, the button shows `>` to indicate active playback. You can press it again to stop instantly.

Next to the `Categories` button, two small **quick access buttons** give one-click access to the two things you would otherwise reach through a shortcut or the tray menu:

- 📂 opens a board saved on your computer — the same import as `F9`, but starting in your boards folder (`%LocalAppData%\SimonSays\boards\`), where exported and downloaded boards are kept (see [Importing and exporting categories](#importing-and-exporting-categories)).
- ⚙️ opens Settings, the same as `F2`.

Both can be hidden with `Show quick access buttons and menu` in Settings (`F2`), which leaves more room for the input box. The same setting also turns off the Categories window's right-click menu.

If SimonSays is hidden, click its tray icon to show it again.

## Categories window
- Open via the `Categories` button. Resize it or enable "Remember category window size" in Settings to persist the size.
- Select a category from the top list and phrases from the bottom list.
- Single-click a phrase to auto-speak if `Speak phrase immediately when selecting` is enabled; otherwise the phrase will load into the main input box without speaking.
- Double-click an empty area or the edge of the window to resize it so that all its categories and phrases fit.
- Drag an empty area of the window to move it, and press `Esc` to hide it.
- Zoom in and out with `F12` and `F11` (see [Shortcuts](#shortcuts)).
- Right-click a button or an empty area (or press `Shift + F10` or the Menu key) for a menu with Paste, Zoom in, Zoom out, Reset zoom, Help, Settings, Gaze / Dwell-click, Hide and Exit, each showing its shortcut. The first entry shows the board's title (`Untitled board` if it has none); choose it to see the board's credits, language and how many categories and phrases it holds. The menu appears only while `Show quick access buttons and menu` is on in Settings.
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
Open Settings (`F2`) to control the following. Settings has two columns.

**Left column**
- **Default text**: toggle `Insert default text automatically` and customize the default phrase.
  - `Show Touch Keyboard` — opens the on-screen touch keyboard when the input box opens; it is placed beside the input box, on the side away from the taskbar.
- `Show quick access buttons and menu` — shows the 📂 (open a board) and ⚙️ (Settings) buttons next to the `Categories` button in the main window (see [Getting started](#getting-started)) and the right-click menu of the Categories window (see [Categories window](#categories-window)). Turn it off to hide them all and give the input box more room.
- **Category window**:
  - `Minimize category window automatically` after selecting.
  - `Remember category window size`.
  - `Auto-resize category window to fit content` — resizes the Categories window to fit its categories and phrases whenever they are loaded, for example at start-up, after changing the language, or after an import.
- `Gaze / Dwell-click...` — opens the [Gaze / Dwell-click](#gaze--dwell-click-f3) settings on top of Settings, which stays open with your changes. Changes you make there are saved when you press `OK` in that window, even if you then press `Cancel` in Settings.

**Right column**
- **UI language**: choose the interface language (English, Spanish, Arabic, Basque, Catalan, Chinese (Simplified), French, Galician, German, Hebrew, Hindi, Italian, Japanese, Korean, Portuguese, Portuguese (Brazil), Russian, Valencian). Each language also has its own categories and phrases (see [Default phrases](#default-phrases)).
- **Voice**: pick an installed SAPI voice matching your language preference.
- **Volume/Rate**: adjust `Volume (10-100)` and `Rate (-10 to 10)`.
- `Test Voice`: audition the selected voice (through the device chosen in `Sound output`).
- `Sound output` — where SimonSays plays its voice and sounds: `Windows default` (the usual choice), or any connected device. Virtual audio cables are listed first, which is what you need for [online meetings](#using-simonsays-in-online-meetings). If the chosen device is not connected, SimonSays plays through the Windows default device and tells you once; the choice is kept, shown as `(not connected)`, and is used again as soon as the device is back.
- `Listen settings...` — opens the Windows recording devices, where you can turn on **Listen to this device** for a virtual cable (see [Using SimonSays in online meetings](#using-simonsays-in-online-meetings)).
- **Playback behavior**:
  - `Speak phrase immediately when selecting` (single-click auto-speak).
  - `Stop Previous Playback` — automatically stops any ongoing playback before starting a new phrase.
  - `Increase SimonSays volume when playing` — raises the volume of the device SimonSays plays through (see `Sound output`) while speaking and restores the previous level when playback finishes.
  - `Temporarily reduce other audio when playing` — reduces the volume of other applications while speaking and restores them when playback finishes.

**Bottom**
- `Help` (bottom-left corner, or `F1` while Settings is open) opens this help in the language selected in the `Language` list; Settings stays open with your changes.
- `OK` saves changes; `Cancel` discards.

By default, `Speak phrase immediately when selecting`, `Minimize category window automatically`, `Auto-resize category window to fit content`, `Increase SimonSays volume when playing` and `Temporarily reduce other audio when playing` are on, and `Stop Previous Playback` is off.

## Gaze / Dwell-click (F3)
Activate phrases and buttons hands-free by holding your gaze (or the mouse cursor) on them for a set duration. This feature is new — it needs testing and feedback: please share how it works with your tracker through the tray menu `Feedback` option.

Supported setups:
- **Any eye-control software that moves the mouse cursor** (Irisbond EasyClick, Tobii Dynavox Computer Control, Windows Eye Control cursor modes, OptiKey): dwell-click follows the cursor.
- **Direct gaze reading (no cursor movement needed)**: Irisbond Hiru (HID mode) and Tobii trackers through their own software — verified with the Tobii Eye Tracker 4C, Tobii Eye Tracker 5 and Tobii Dynavox PCEye 5.
- **Tobii Eye Tracker 4C and Eye Tracker 5 licence**: Tobii's end-user licence for these trackers does not allow medical use, including use in hospitals and other healthcare settings. Tobii Dynavox devices, such as the PCEye 5, come with their own licence terms.
- **Tobii Eye Tracker 4C software**: SimonSays reads the 4C through Tobii Core. Plugging an Eye Tracker 5 into the same computer makes Windows install Tobii Experience, which turns Tobii Core off, so the 4C stops working. To use the 4C again, reinstall Tobii Core — the recommended version is `Tobii_Eye_Tracking_Core_v2.13.1.7294_x86.exe`, from the [Tobii getting-started page](https://gaming.tobii.com/getstarted/).

Open the Gaze / Dwell-click settings (`F3`, the tray menu, or the `Gaze / Dwell-click...` button in Settings) to configure:
- **Dwell mode**: choose one of the following options:
  - `Automatic` — detects device presence and enables dwell-click using the mouse cursor or gaze detection as appropriate. Tobii's background software alone (always running once installed) does not turn on mouse-cursor dwell; a Tobii tracker is used only while it is actually tracking.
  - `Mouse cursor` — always enable dwell-click using the mouse cursor.
  - `HID eye tracker` — always enable dwell-click using gaze detection (Irisbond Hiru in HID mode, or Tobii trackers). Only your gaze activates buttons: while no gaze arrives — tracking switched off, or the tracker unplugged — nothing dwells, and the mouse works normally.
  - `Off` — always disable dwell-click.
  - **Lock-on delay**: how long, in milliseconds, your gaze (or the cursor) must rest on a button before the progress bar starts (default 150). Glancing across buttons then shows no progress at all; the button activates after the lock-on delay plus the dwell time. Set it to 0 to start the progress immediately.
  - **Dwell time**: set the duration (in milliseconds) required to trigger a click when gazing at a UI element (default 500). A progress indicator is shown while the dwell is in progress.
  - **Tolerance radius**: adjust the gaze tolerance (in pixels) to allow for slight eye movement while still registering a dwell-click.
  - **Cooldown**: time in milliseconds after activation during which dwell-click cannot trigger (prevents accidental repeated clicks).
  - **Progress color**: choose the color of the dwell progress indicator (defaults to the Windows accent color).
  - **Move away before dwell can activate again** (on by default): after a dwell-click, no button can be activated by dwell again until your pointer or gaze moves away from that spot (farther than the tolerance radius), or you look away for a moment. Resting on a button activates it only once, so a long phrase is not repeated before it finishes. Mouse clicks are not affected.
  - **Detect how you use the tracker**: two buttons to help the app detect whether you are moving the mouse cursor with an eye tracker or simply clicking on buttons.
  - **Detected**: shows the detected device type and further information about related software.
  - `Save diagnostic report...` (below Detected): writes a report about your eye tracker and its software to `hid_dump.txt` and opens its folder, so you can send it to support.
  - A `Reset` button (bottom left) restores all dwell settings to their defaults.
  - An `OK` button saves changes; `Cancel` discards.

## Using SimonSays in online meetings
SimonSays can speak straight into a video call (Teams, Zoom, Google Meet…), so the other participants hear your phrases as if they came from your microphone. You need a virtual audio cable, such as the free [VB-Audio Virtual Cable](https://vb-audio.com/Cable/):

1. Install the virtual audio cable and restart Windows if asked.
2. Send SimonSays' sound to the cable: open Settings (`F2`) and choose `CABLE Input` in `Sound output` (virtual cables are listed first). Press `Test Voice` to check it: you should not hear it from your speakers any more, because it now goes into the cable.
3. In your meeting app, choose `CABLE Output` as the microphone.
4. To hear SimonSays yourself as well, press `Listen settings...` in Settings. It opens the Windows recording devices: select `CABLE Output`, open **Properties → Listen**, tick **Listen to this device**, choose your speakers or headset in **Playback through this device**, and press `OK`. You only need to do this once.

SimonSays cannot turn on **Listen to this device** for you: it is a setting of the cable's recording side that Windows only lets you change yourself, so `Listen settings...` takes you straight to the right window instead.

Good to know:
- Leave the Windows default playback device on your speakers or headset, not on `CABLE Input`. If **Listen to this device** plays to "Default Playback Device" while that default is the cable, the sound loops back into the cable.
- If the meeting hears your phrases but not sound effects (applause, a doorbell), the meeting app's noise suppression is removing them; lower or turn off noise suppression in the meeting app.
- If the cable is uninstalled or disconnected, SimonSays plays through the Windows default device and tells you once; it goes back to the cable as soon as it is available again.
- As an alternative to step 2, you can still route SimonSays with Windows' own per-app setting (**Settings → System → Sound → Volume mixer**), leaving `Sound output` on `Windows default`. SimonSays only appears in that list after it has played something.

## Tray icon menu
- `Show` / `Hide` main UI
- `Settings`
- `Gaze / Dwell-click`
- `Web` opens the SimonSays web page
- `About` shows version and credits
- `Feedback` opens a [feedback form](https://forms.gle/KMbpEDYmwnFJUhgy8) in the default browser
- `Exit` quits SimonSays, after asking first

You can move the tray pop-up window and other dialogs by dragging their title bars; resize the Categories window by dragging its edges, and enable `Remember category window size` in Settings to persist the size.

## Editing categories and phrases
This section and the ones after it are for caregivers, speech therapists and advanced users who change the categories, phrases, sounds and look of a board. Every change is saved immediately (see [Undoing changes](#undoing-changes)).

For a visual board editor, and for SimonSays on other platforms, see [SimonSays Web](https://github.com/juan-rey/simonsays-web) — it opens and saves `.ssc`/`.ssz` boards right in your browser, without uploading them.

Editing shortcuts (Categories window):
- `F4`: Edit current selection
- `F5` / `F6`: Move to previous / next selection in lists
- `F7`: Add after current selection (category/phrase context)
- `F8`: Delete current selection
- `F9`: Import categories
- `F10`: Export categories
- `Ctrl + F4`: Edit the board style (see [Customizing the look](#customizing-the-look-board--category-styles))
- `Ctrl + F8`: Delete all categories and their phrases (asks twice for confirmation)
- `Ctrl + V`: Add the copied text as phrases, one per paragraph (see [Managing phrases](#managing-phrases))
- Drag a `.txt`, `.ssc` or `.ssz` file onto the window: add a `.txt` as a new category, or import a board as with `F9` (see [Managing categories](#managing-categories))

> **Tip:** Export your board (`F10`) before using `Ctrl + F8` or `Ctrl + F9`, so that you can get it back.

### Managing categories
- Rename: select category → (shortcut: `F4`) `Edit` → change name → `OK`.
- Move selection: `F5`/`F6` to move to previous/next item when browsing lists.
- Add: select category → (shortcut: `F7`) `Add category` dialog → enter name → `OK`.
- Delete: select category → (shortcut: `F8`) `Delete`; confirm deletion of the category and its phrases.
- Add from a text file: drag a `.txt` file onto the Categories window → a category named after the file is created, with one phrase per paragraph (paragraphs are separated by a blank line) → check the preview → `OK`. Several files can be dropped at once; each gets its own preview. A file whose name matches an existing category is refused.
- Open a board by dragging it: drag a `.ssc` or `.ssz` file onto the Categories window → SimonSays asks whether to import it; on `Yes` it is imported just as with `F9` (see [Importing and exporting categories](#importing-and-exporting-categories)). If the file holds a whole board, it replaces your current one, and a copy of the current board is saved first as `backup.ssz`. You can drop text files and board files together; each is handled in turn. Opening a `.ssc` or `.ssz` file from File Explorer (double-click) asks the same question.

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
- Add many at once: copy text whose phrases are separated by a blank line → select a phrase (or a category) → `Ctrl + V` → check the preview → `OK`. The phrases are added after the selected phrase, or at the end of the category. Line breaks inside a paragraph become spaces, and the `##` icon and `::` audio markers work as in the `Add phrase` dialog.
- Add sounds: drag one or more `.wav` or `.mp3` files onto the Categories window → check the preview → `OK`. Each file becomes a sound phrase named after the file, added after the selected phrase (or at the end of the selected category), and the file is copied into the board's resource folder so it travels with the board when you export it. An existing file is never overwritten: if a different sound with the same name is already there, the new one is saved as `name (2).wav`.

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

`ahem.mp3`, `ahem.mp3`, `alert.mp3`, `applause.mp3`, `aww.mp3`, `beep.mp3`, `boo.mp3`, `call_bell.mp3`, `censor.mp3`, `doorbell.mp3`, `drumroll.mp3`, `evil_laugh.mp3`, `fail.mp3`, `hmmmm.mp3`, `kisses.mp3`, `laugh.mp3`, `mmm-hmm.mp3`, `nuh-uh.mp3`, `phone_ring.mp3`, `rimshot.mp3`, `scream.mp3`, `shots.mp3`, `sigh.mp3`, `sigh.mp3`

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
- **Get one** by importing a styled categories file (`.ssc` / `.ssz`). When you import a file that contains a board style, SimonSays asks before replacing your current one, and shows the board's `title` and `credits` (if any) in the import message. A file whose board style only sets `title`, `credits`, `language` or `resource-folder` changes nothing you can see, so SimonSays does not ask: a single category from it is added and your look stays; a whole board from it replaces your board, look included, as any whole board does.

### Style properties
- **Whole window**: `background`, `separator-color`, `text-color`, `font-name`, `font-size`, `text-weight`, `caption` (label shown between the separators), `title` and `credits` (shown when the board is imported), `language` (the language the board was made for, e.g. `language:Spanish`; SimonSays refuses to import the board while it is set to a different language, and tells you which language to switch to in Settings, `F2`).
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
Boards are shared as `.ssc` files (categories and phrases) or `.ssz` files (the same, plus the sound and icon files they use, in a single file). You can also create and edit them visually in your browser with [SimonSays Web](https://github.com/juan-rey/simonsays-web).

- **Export** (`F10`): choose to export all categories or only the selected one when prompted; the Save dialog opens in `%LocalAppData%\SimonSays\boards\` by default, though you can still save anywhere. SimonSays saves an `.ssz` automatically when your categories or phrases use sound or icon files it can find (`.mid`/`.midi` sounds are not included). Exporting all categories includes the board style; exporting one category includes only that category's style. Success/failure messages appear in the selected UI language.
- **Import** (`F9`): choose a `.ssc` or `.ssz` file.
  - A file with **several categories or a board style** is a whole board: it **replaces all the categories and phrases of the current language**. Your previous board is saved first as `backup.ssz` (see [Undoing changes](#undoing-changes)).
  - A file with **a single category** is added to your board; if a category with the same name exists, you will be asked to overwrite it. This is also true when the file only names its board — `title`, `credits` or `language`, with no colors, sizes or fonts: such details never replace your board or its look.
  - Sound and icon files inside an `.ssz` are installed in the resources folder, and the Categories window resizes to fit the imported board. Success/failure messages appear in the selected UI language.

## Undoing changes
SimonSays saves every change straight away and has no general Undo command. These are the ways to go back:

- **Undo an import**: before an import replaces your board, SimonSays saves the previous board as `backup.ssz` in `%LocalAppData%\SimonSays\boards\`. To undo the import, press `F9` and choose `backup.ssz`: your previous board comes back as it was, categories and look, without further questions. Only the most recently replaced board is kept: each new import of a whole board overwrites the backup.
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
- `%LocalAppData%\SimonSays\boards\` — the default folder for exported boards, the automatic `backup.ssz`, and the boards that come with SimonSays: the default phrases of each language, plus ready-made categories you can add to your board with `F9` or the 📂 button — Yes / No, Pain & body, Spelling, Games, Questions, Daily care and a Hearing check, named in your language, and `Spelling_NATO`, `Alert_tones` and `Morse_code` for any language. Each of these adds one category without replacing your board. Your language's boards are copied there at startup and when you change the language in Settings, whenever your language's default board is missing or has changed, replacing the copies in the folder — so save a board you edit under a new name.
- `%LocalAppData%\SimonSays\debug\` — diagnostic files (such as `hid_dump.txt`) that support may ask for.

To open one of these folders, paste its path (for example `%LocalAppData%\SimonSays`) into the File Explorer address bar.

## Troubleshooting
- **Voice initialization fails**: verify SAPI is installed and a compatible voice is present.
- **No voice for my language, or phrases spoken with the wrong accent**: SimonSays uses the voices installed in Windows. Add a voice for your language in Windows Settings (**Time & language → Speech → Manage voices**), then choose it in Settings (`F2`). Some newer Windows "natural" voices may not be available to other apps.
- **No sound playback**: confirm file path/extension and paired delimiters `♫`.
- **The taskbar no longer hides automatically while SimonSays is open**: this is intended. SimonSays sits on the taskbar, so it keeps the taskbar shown while it runs and turns "Automatically hide the taskbar" back on when you quit it. If SimonSays was closed unexpectedly (for example a power cut), it is turned back on the next time you quit SimonSays.
- **The Widgets button covers the SimonSays bar** (Windows 11): turn Widgets off in Windows **Settings → Personalization → Taskbar**. SimonSays offers to open that page once, the first time it notices Widgets.
- **The Categories window is too small or too large**: double-click an empty area or the edge of the window to fit it to its content, or zoom with `F11` / `F12`.
- **My categories or phrases disappeared**: check the language in Settings (`F2`) — each language has its own board. If an import replaced your board, see [Undoing changes](#undoing-changes).
- **Import/export errors**: check file permissions and disk location; retry.
- **UI language or voice changes not applied**: reopen Settings and confirm the selection. If changes still do not apply, restart SimonSays and try again.
- **Reporting eye-tracking hardware issues**: if support asks for diagnostics, open the Gaze / Dwell-click window (`F3`) and click `Save diagnostic report...`. The report (`hid_dump.txt`) is saved to `%LocalAppData%\SimonSays\debug\` and that folder opens. It is only created when you ask for it.

## Feedback, support and privacy
- The `About` dialog (tray menu) lists version, description, and copyright.
- Share suggestions, problems, or how SimonSays works with your eye tracker through the tray menu `Feedback` option, which opens a [feedback form](https://forms.gle/KMbpEDYmwnFJUhgy8).
- For bug reports and technical support, visit the [SimonSays GitHub repository](https://github.com/juan-rey/SimonSays).
- SimonSays is free and open source under the Open Software License 3.0 (OSL-3.0). It shows no ads and does not collect your data.
