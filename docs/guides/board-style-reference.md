# SimonSays — Board & Category Style Reference

**Audience:** advanced users customizing boards by hand, and integrators
generating styled boards programmatically.

| | |
|---|---|
| **Applies to** | SimonSays – Simply Speak (board/category style cascade) |
| **Authoritative spec** | [`docs/specs/board-style.spec.md`](../specs/board-style.spec.md) (`STY-` requirements) |
| **Authoritative code** | [`src/BoardStyle.cpp`](../../src/BoardStyle.cpp), [`include/BoardStyle.h`](../../include/BoardStyle.h) |
| **Related** | [`ssc-ssz-format-reference.md`](ssc-ssz-format-reference.md) (how styles are stored/shared) |

> This is a **practical reference**, not the contract. The spec (`STY-F*`/`STY-N*`
> requirements) and the code are authoritative; where this guide and the code
> disagree, the code wins and this guide is the bug. This guide **must** be kept
> in sync with any style change (board-style.spec.md **STY-N02**).

---

## Contents
- [1. What a style is](#1-what-a-style-is)
- [2. Syntax](#2-syntax)
- [3. Value forms](#3-value-forms)
- [4. The three-layer cascade](#4-the-three-layer-cascade)
- [5. Property reference](#5-property-reference)
- [6. Where styles live (storage & portability)](#6-where-styles-live-storage--portability)
- [7. Board resource folder](#7-board-resource-folder)
- [8. Constraints & gotchas](#8-constraints--gotchas)
- [9. Worked example — an SPC board](#9-worked-example--an-spc-board)
- [10. Quick reference card](#10-quick-reference-card)

---

## 1. What a style is

A **style list** is a string of `property:value;` pairs that customizes the look
of the category window (the pop-up board of category and phrase buttons). Styling
is CSS-like: a small set of typed properties resolved through a cascade, with
unset properties falling back to the layer above and, ultimately, to the app's
built-in defaults.

There are two authoring surfaces:

- **Board style** — one global layer per language, stored under the reserved
  category name `$$board`. It sets window-level properties plus defaults for
  *all* category buttons and *all* phrase buttons.
- **Category style** — a per-category layer: the category's own button colors,
  which also flow to that category's phrase buttons, plus any phrase-button
  property.

There is **no style-editing UI** in the current version beyond a raw text editor
for the board list. Styles are authored by importing a styled `.ssc`/`.ssz`, by
editing the registry value, or (per category) via the `::` suffix in the F4 edit
dialog. This guide is what you need to write those strings by hand or generate
them from a tool.

## 2. Syntax

A style list is a sequence of pairs:

```
property:value;property:value;property:value
```

Parsing rules (from `ForEachPair` / `ApplyProp` in `BoardStyle.cpp`):

- **Pair separator:** `;`. The trailing `;` is **optional**.
- **Name/value separator:** the **first** `:` in the pair. (Values may therefore
  contain `:` — useful for nothing in practice, but it means a stray colon in a
  free-text value does not break parsing.)
- **Whitespace** around the name and around the value is trimmed
  (` \t\r\n`). Interior whitespace in a value is preserved.
- **Property names are case-insensitive** (lower-cased before matching). Values
  are **not** lower-cased except where a value is itself a keyword
  (`text-weight`, `icon-position`, `text-layout`); colors and free text keep
  their case.
- **Skip-on-error (STY-F03).** A pair with an unknown property, a malformed
  value, or a value out of range is **silently ignored** — only that pair;
  parsing continues. Malformed input never throws or aborts the whole list
  (STY-N01). The affected property simply resolves from the layer above.
- **Last-wins (STY-F04).** If the same property appears twice in one layer's
  lists, the **last** occurrence applies.

A pair with no `:` or an empty name is skipped. An empty value is rejected for
every property (so `background:;` does nothing).

## 3. Value forms

| Form | Syntax | Notes |
|---|---|---|
| **Color** | `#RRGGBB` | Exactly 7 chars, `#` + 6 hex digits (case-insensitive). No 3-digit shorthand, no named colors, no alpha. |
| **Size (absolute)** | non-negative integer | **Pixels.** Up to 6 digits. |
| **Size (relative)** | `NN%` | Percentage of the built-in default for **that** property (the only relative form). Whitespace before `%` is tolerated. |
| **Weight** | `normal` \| `bold` \| `100`–`900` | `normal` = 400, `bold` = 700; numeric is a `LOGFONT` weight, clamped to the 100–900 range (out of range → ignored). |
| **Icon position** | `left` \| `right` \| `top` \| `bottom` \| `center` | `center` centers the icon and hides the label (unless a non-smart `text-layout` is also set). |
| **Text layout** | one/two align keywords, **or** `smart`/`auto` | Horizontal `left\|center\|right` and/or vertical `top\|middle\|bottom`, space-separated, order-free. `smart` (alias `auto`) clears explicit alignment so the label uses SSButton's centered-in-leftover-space default. |
| **Free text** | any non-empty string | `font-name`, `caption`, `title`, `credits`, `resource-folder`. Stored verbatim. **Must not contain `\|` or `;`** (see §8). |

**Sizes resolve** as `built-in default × percentage (if any, else absolute) ×
zoom factor` (STY-F45). A resolved size of `0` is honored only where meaningful
(`margin`, `border-width`, `corner-radius`, `icon-size`); for layout dimensions
(`width`, `height`, `font-size`) a resolved `0` or negative is ignored and the
layer above applies. Absolute `font-size` is **pixels** (`lfHeight = -px`).

## 4. The three-layer cascade

Every property resolves through:

```
built-in default  →  board style ($$board)  →  category style
```

Each layer overrides only the properties it explicitly sets; everything else
falls through. Resolution happens when buttons are (re)created and on category
selection — there is no per-paint cost.

Two cascade rules worth internalizing:

- **Own-color flow-through (STY-F12).** At the **category** layer, `background`
  and `text-color` apply to the category's *own* button **and** become the base
  for that category's *phrase* buttons — unless `phrase-background` /
  `phrase-text-color` override them at the same layer. This is what makes an SPC
  class a single property: `::background:#FFD966;` colors the key **and** its
  phrases.
- **`text-layout` resolves atomically.** An inherited `smart` is reset by an
  explicit axis; an explicit axis overrides just that axis and drops the
  inherited smart flag. `smart` at a lower layer resets an inherited explicit
  alignment.

Hover/pressed/disabled shades are **not** styleable: SSButton derives them from
the resolved base `background` (STY-F11). You set base colors only. This is why
white keys (`#FFFFFF`) still get a visible bevel.

## 5. Property reference

### 5.1 Board style (`$$board`)

**Window properties** — unprefixed, board layer only:

| Property | Value | Effect |
|---|---|---|
| `background` | color | Window background (instance brush + erase handler + DWM caption color). |
| `separator-color` | color | Color of the two separator lines around the strip. |
| `text-color` | color | Strip/hint text color. Unset + styled background → auto black/white by luminance (STY-F41). |
| `font-name` | free text | Strip/hint font family. |
| `font-size` | size (px/%) | Strip/hint font size (px absolute). |
| `text-weight` | weight | Strip/hint font weight. |
| `caption` | free text | Replaces the localized shortcuts hint shown between the separators. Does **not** touch the title bar. |
| `title` | free text | Board metadata; shown in the import confirmation box; also derives the resource folder (§7). Not rendered persistently. |
| `credits` | free text | Board metadata; shown in the import confirmation box. Not rendered persistently. |
| `resource-folder` | free text | Overrides `title` as the resource-folder name (§7). Not rendered anywhere. |

**Group properties** — prefixed `category-` (all category buttons) and `phrase-`
(all phrase buttons):

| Property (add `category-`/`phrase-`) | Value | Effect |
|---|---|---|
| `background` | color | Button base background. |
| `text-color` | color | Button label color. |
| `width` | size | Button width (px/%). |
| `height` | size | Button height (px/%). |
| `corner-radius` | size | Rounded-corner radius (0 = square). |
| `border-width` | size | Border thickness (SSButton clamps to `[0,2]`). |
| `margin` | size | Gap around buttons in the grid. |
| `icon-position` | keyword | `left\|right\|top\|bottom\|center`. |
| `icon-size` | size | Icon box size (0 = auto). |
| `font-name` | free text | Label font family. |
| `font-size` | size | Label font size (px). |
| `text-weight` | weight | Label weight. |
| `text-layout` | layout | Label alignment / `smart`. |

Example board line:

```
$$board=$$background:#FBFAFD;separator-color:#B4A7D6;category-icon-position:top;phrase-corner-radius:8;caption:SPC Board;title:SPC Adult Board;credits:by SimonSays;
```

### 5.2 Category style

The per-category layer (the `::<style>` suffix, or a `$$` token in the
category's stored data — see §6):

| Property | Value | Effect |
|---|---|---|
| `background` | color | The category's own button background **and** the base for its phrase buttons (STY-F12). |
| `text-color` | color | Own button text **and** phrase base text. |
| `phrase-*` | (full `phrase-` set above) | Overrides for that category's phrase buttons. |

The category's **own** button width/height/margin are **not** styleable (all
category buttons share one grid); such pairs are skipped.

Example (F4 edit dialog form): `🧍##Personas::background:#FFD966;`

## 6. Where styles live (storage & portability)

Styles travel **with the categories**, per language, through three carriers, and
round-trip **verbatim** — including properties this version does not recognize,
so a board authored in a newer version survives editing in an older one
(STY-F33). Never store a style in a name token.

| Carrier | Board style | Category style |
|---|---|---|
| **Registry** (per-language key) | value named `$$board`, data = style-list token(s) joined by `\|`, each with an optional leading `$$` (stripped) | a `$$<style list>` **style token** placed in the category's phrase data (conventionally first) |
| **`.ssc`** | line `$$board=$$<style list>` | `<icon>##<name>=$$<style>\|phrase\|phrase…` |
| **`.ssz`** | same `.ssc`, wrapped in the Zip bundle | same |

Reserved tokens (constants in [`include/stdafx.h`](../../include/stdafx.h)):

| Token | Constant | Meaning |
|---|---|---|
| `$$board` | `BOARD_STYLE_CATEGORY_NAME` | Reserved category name carrying the board layer. Rejected as a user category name; extracted at load, re-emitted at save; never reaches the button grid. |
| `$$` | `STYLE_TOKEN_PREFIX` | Prefix marking a style token in phrase-position data (distinguishes it from a real phrase). |
| `::` | `AUDIO_FILE_SEPARATOR` | The category style suffix in the F4 dialog (`<icon>##<name>::<style>`) — reuses the audio separator. |
| `\|` | `CATEGORY_PHRASE_SEPARATOR` | Separates phrases (and style tokens) in category data. |

For the exact byte-level `.ssc`/`.ssz` layout, see
[`ssc-ssz-format-reference.md`](ssc-ssz-format-reference.md).

**Export scope (STY-F52):** exporting **all** categories includes `$$board`;
exporting a **single** selected category includes only that category's own style
and **not** `$$board`.

**Import (STY-F53):** an incoming board style is applied silently when none
exists locally, prompts before replacing a different existing one, and does
nothing when identical. `title`/`credits` in the import box are read from the
**incoming** file, so they appear even if you decline to replace.

## 7. Board resource folder

A board that carries a `title` (or an explicit `resource-folder`) keeps its icon
and audio files in a dedicated subfolder of `%LocalAppData%\SimonSays\`, searched
**first** when the app resolves a relative icon/sound name (STY-F58).

- **Name source:** `resource-folder` if set, otherwise `title`. When
  `resource-folder` is set but sanitizes to empty (e.g. a reserved device name),
  there is **no** fallback to `title` — the override was deliberate.
- **Sanitization:** spaces → `_`; the characters `\ / : * ? " < > |` and control
  characters are stripped; leading/trailing dots trimmed; reserved DOS device
  names (`CON`, `NUL`, `COM1`…, even with an "extension") rejected; capped at
  `BOARD_RESOURCE_FOLDER_MAX_NAME` (64) chars. A name that sanitizes to empty
  means "no subfolder" — resources live in the app-data root, as before.
- **Rename:** changing the title/`resource-folder` moves the old subfolder to the
  new name — a plain rename when the target is absent, otherwise a per-file merge
  that never overwrites existing files (STY-F59).

So `title:My Board;` → `%LocalAppData%\SimonSays\My_Board\`. Integrators bundling
a `.ssz` don't need to think about this: the bundle's `resources/` is flat and
the app installs into the right subfolder on import.

## 8. Constraints & gotchas

- **No `|` in any value.** `|` is the carrier separator; it truncates the token
  at parse time. Font names and `caption`/`title`/`credits`/`resource-folder`
  text with a `|` are unsupported.
- **No `;` in free text.** `;` ends the pair, so a free-text value cannot contain
  one.
- **No `=` needed inside a value** — but note the `.ssc` line itself is split at
  the **first** `=`, so put styles after the `=` (they live in phrase-position
  data anyway).
- **Colors are strict** `#RRGGBB` only. `#fff`, `red`, `rgb(...)`, and 8-digit
  alpha are all rejected (silently → property unset).
- **Percent is of the built-in default**, not of the parent layer's value. There
  is no "relative to inherited" form.
- **`$$`-prefixed category names are reserved.** Creating/renaming a category to
  `$$board` — or any name starting with `$$` — is rejected.
- **Category names containing `::`** are split at the first `::` in the F4 dialog
  (a pre-existing ambiguity with the audio suffix).
- **Older app versions** show a styled file's `$$board` as an ordinary category
  and `$$` tokens as ordinary phrases — harmless, user-deletable.
- **Per language.** Switching UI language switches (or drops) the board style;
  two languages whose boards share a sanitized title share one resource folder.

## 9. Worked example — an SPC board

A complete SPC/Fitzgerald adult board (`.ssc`, UTF-8), one color per class
flowing to each class's phrase keys via STY-F12:

```
SIMONSAYS_CATEGORIES_V1
$$board=$$background:#FBFAFD;separator-color:#B4A7D6;category-icon-position:top;phrase-corner-radius:8;caption:SPC Board;title:SPC Adult Board;credits:by SimonSays;
🧍##Personas=$$background:#FFD966;|Yo|Tú|Familia|Cuidador|Médico
🏃##Acciones=$$background:#93C47D;|Quiero|Necesito|Comer|Beber|Ir|Sentir
📏##Descriptivos=$$background:#6FABDC;|Mucho|Poco|Bueno|Malo|Frío|Caliente
🍎##Sustantivos=$$background:#F6B26B;|Agua|Comida|Baño|Teléfono|Medicación|Dolor
🕒##Miscelánea=$$background:#FFFFFF;|Hoy|Mañana|Aquí|Allí|Números|Colores
👋##Social=$$background:#B4A7D6;|Hola|Gracias|Por favor|Sí|No|Adiós
```

The board line sets the shared look (soft background, purple separators, category
emoji on top with the name centered below via smart layout, rounded phrase keys),
the strip `caption`, and the `title`/`credits` shown on import. Each category sets
one `background`; the text-only phrase keys have no icon, so smart layout centers
them.

## 10. Quick reference card

```
LIST      := PAIR (';' PAIR)* ';'?
PAIR      := NAME ':' VALUE          # split at first ':'; name lower-cased & trimmed; value trimmed
CASCADE   := built-in -> $$board -> per-category   # per-property fallthrough
SCOPES    := window(unprefixed) | category-* | phrase-*    # board layer
           | own(background,text-color) | phrase-*          # category layer

COLOR     := '#' HEX{6}
SIZE      := INT | INT '%'           # px, or % of built-in default; effective = default*% (or abs) * zoom
WEIGHT    := 'normal' | 'bold' | 100..900
ICONPOS   := left|right|top|bottom|center
LAYOUT    := (H? V?) | 'smart' | 'auto'   ; H=left|center|right  V=top|middle|bottom
TEXT      := verbatim, no '|' and no ';'

STORE     := $$board (board) ; $$<style> token in phrase data (category) ; ::<style> in F4 dialog
FOLDER    := %LocalAppData%\SimonSays\<sanitize(resource-folder || title)>
ERRORS    := unknown/malformed pair -> skipped ; duplicate -> last wins ; never throws
```

---

*Reference guide — see [`docs/specs/board-style.spec.md`](../specs/board-style.spec.md)
for the authoritative `STY-` requirements and acceptance criteria.*
