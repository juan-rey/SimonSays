# SimonSays — Board & Category Styles — Specification

| | |
|---|---|
| **Spec ID** | STY-SPEC |
| **Status** | Active — fully implemented & verified (manual passes 2026-07-08); all ACs Pass |
| **Version** | 0.8 (2026-07-08) |
| **REQ prefix** | `STY-F##` (functional), `STY-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Master spec** | [`docs/spec.md`](../spec.md) |

---

## 0. How to use this spec

See [`AGENT.md`](../../AGENT.md) and [`docs/spec.md`](../spec.md) §2 for the shared
working agreement, EARS phrasing, ID scheme, status tags, and acceptance-criteria
/ status-matrix formats. This file is the source of truth for the **style
cascade of the category window**: the board-level (`$$board`) and per-category
style layers, their `property:value;` syntax, persistence, and portability.
Reference format: [`docs/specs/dwell.spec.md`](dwell.spec.md).

> Scope reminder: this spec owns the **style model** (syntax, cascade,
> reserved names, storage tokens) and its **application** to the category
> window and its buttons. The category/phrase data model and authoring flows
> are in [`categories-phrases.spec.md`](categories-phrases.spec.md); the
> registry storage mechanism is in [`persistence.spec.md`](persistence.spec.md);
> bundling/portability flows are in [`import-export.spec.md`](import-export.spec.md);
> the button control's rendering capabilities are in
> [`ssbutton.spec.md`](ssbutton.spec.md) (this spec only *feeds* it a resolved
> `SSButtonConfig`).

---

## 1. Overview

Today the category window has a single hardcoded look: taskbar-color
background, one default `SSButtonConfig` for every button, system fonts. This
feature adds a three-layer **style cascade**, CSS-like in spirit:

1. **Built-in defaults** — today's behavior. Always the base layer.
2. **Board style** — a per-language style stored under the reserved category
   name **`$$board`**: window properties (background, separator color, text
   style) plus defaults for *all* category buttons and *all* phrase buttons.
3. **Category style** — per-category overrides: colors for that category's own
   button and any property for the phrase buttons shown when it is selected.

A property not set at a layer resolves from the layer above ("do nothing").
Styles travel with categories through the registry, `.ssc`, and `.ssz`
formats, so a fully styled communication board can be shared as one file.

**Immediate goal:** SPC communication boards (Sistema Pictográfico de
Comunicación, modified Fitzgerald key): six word-class categories where each
category button and its phrase buttons share the class color — yellow
`#FFD966` people/pronouns, green `#93C47D` verbs/actions, blue `#6FABDC`
descriptives, orange `#F6B26B` nouns, white `#FFFFFF` miscellanea, purple
`#B4A7D6` social. See Appendix A for a ready-to-import example.

## 2. Background & context

- SPC/Fitzgerald color coding speeds word localization and reduces cognitive
  load for AAC users; therapists share prepared boards between machines, so
  the style must ride inside the existing export bundle rather than in app
  settings.
- The category store already round-trips through three carriers with reserved
  separators: registry values (`<icon>##<name>` name / phrases joined by `|`
  in the data), `.ssc` lines (`<serialized category>=<phrases>`, split at the
  **first `=`**), and `.ssz` (Zip wrapping a `.ssc`). The style syntax and
  storage tokens are chosen to avoid all of those separators (see §8.2).
- No authoring UI ships in v1: the board layer is authored by importing a
  styled `.ssc`/`.ssz` (or hand-editing the registry); the category layer is
  authored in the existing F4 edit dialog via a `::` suffix.

## 3. Goals & non-goals

**Goals**
- Declarative, forward-compatible `property:value;` style strings; malformed
  or unknown input degrades to "no styling", never to an error.
- Three-layer cascade with per-property fallthrough.
- Styles persist with the categories (per language) and round-trip through
  registry, `.ssc`, and `.ssz` unchanged, including unrecognized properties.
- SPC boards expressible with one color property per category.
- Zero UI filtering: pseudo-entries are extracted at the load/save boundary
  and never reach `m_categories` / the button grids.

**Non-goals**
- A style-editing UI (v1 is import/registry-only; see §18).
- Styling the main (taskbar) window, dialogs, or the tray menu.
- Per-phrase styling (the finest grain is "all phrases of a category").
- New rendering capabilities in `SSButton` — only configurations it already
  supports (see [`ssbutton.spec.md`](ssbutton.spec.md)) are exposed.

## 4. Glossary

| Term | Meaning |
|---|---|
| **Style list** | A string of `property:value;` pairs, e.g. `background:#FFD966;phrase-height:120%;`. |
| **Board style** | The global layer: window + all-category-buttons + all-phrase-buttons properties, stored under the reserved `$$board` category name. |
| **Category style** | The per-category layer: the category's own button colors + properties for its phrase buttons. |
| **Cascade** | Per-property resolution order: built-in default → board style → category style. |
| **Style token** | A pseudo-phrase beginning with `$$` carrying a style list in a category's stored data. |
| **`::` style suffix** | Authoring form of the category style in the F4 edit dialog: `<icon>##<name>::<style list>`. |
| **Relative value** | A percentage (`NN%`) of the built-in default for that property; the only relative form. |
| **Smart text layout** | Default label placement — centered within the area left over by the icon (centered overall when none); selected with `text-layout:smart` (see [`ssbutton.spec.md`](ssbutton.spec.md) BTN-F45). |
| **Caption** | Board-supplied label shown in the strip between the separators, replacing the shortcuts hint (`caption`). |
| **Title / Credits** | Board metadata (`title`, `credits`) shown in the import confirmation box; not rendered persistently. |

## 5. Personas & scenarios

- **S1 — Therapist shares an SPC board:** exports a styled board on her PC as
  `.ssz`; the user imports it and gets the six color-coded categories, colored
  phrase keys, and board background with no further setup.
- **S2 — Caregiver tweaks one category:** presses F4 on the "Personas" button
  and appends `::background:#FFD966;` to color that class.
- **S3 — Power user hand-tunes the board:** edits the `$$board` registry value
  (or re-imports an edited `.ssc`) to enlarge phrase keys
  (`phrase-height:130%;`) and change the window background.
- **S4 — Legacy exchange:** a styled `.ssc` opened in SimonSays v0.6 shows the
  `$$board` pseudo-category and `$$…` tokens as harmless junk entries the user
  can delete; nothing crashes.

---

## 6. Requirements (EARS)

Status tags per [`docs/spec.md`](../spec.md) §2.3. Slice 1 (2026-07-08)
delivered parsing + persistence; slice 2 (2026-07-08) wired rendering (§6.5),
verified by the developer's manual pass; slice 3 (2026-07-08) added the
STY-F53 import-replacement prompt, also verified; slice 4 (2026-07-08) added
`text-weight`, `caption`, and `title`/`credits` (STY-F55–F57), plus fixes to
style-only import and caption refresh. All requirements are **[Done]** and all
acceptance criteria **[Pass]**.

### 6.1 Style syntax & parsing

- **STY-F01 [Done]** THE SYSTEM SHALL parse a style list as a sequence of
  `property:value;` pairs (pair separator `;`, name/value separator = the
  **first** `:` in the pair; whitespace around names and values trimmed;
  property names case-insensitive; the trailing `;` optional).
- **STY-F02 [Done]** THE SYSTEM SHALL accept these value forms:
  colors as `#RRGGBB`; sizes as a plain non-negative integer (**pixels**) or
  as `NN%` (**percentage of the built-in default** for that property — the
  only relative form); keywords (`left`, `right`, `top`, `bottom`, `center`,
  `middle`); free text (font names).
- **STY-F03 [Done]** IF a pair has an unknown property name, a malformed
  value, or a value out of the property's accepted form THEN THE SYSTEM SHALL
  ignore **that pair only** and continue parsing the rest ("do nothing" —
  the affected property resolves from the layer above).
- **STY-F04 [Done]** WHEN the same property appears more than once in the
  style lists of one layer THE SYSTEM SHALL apply the **last** occurrence.

### 6.2 Cascade semantics

- **STY-F10 [Done]** THE SYSTEM SHALL resolve every style property per the
  cascade **built-in default → board style → category style**, where each
  layer overrides only the properties it explicitly sets.
- **STY-F11 [Done]** THE hover, pressed, and disabled shades of a styled
  button SHALL remain auto-derived by `SSButton` from the resolved base
  background color (per [`ssbutton.spec.md`](ssbutton.spec.md) BTN-F50);
  styles set base colors only.
- **STY-F12 [Done]** AT the category layer, `background` / `text-color`
  SHALL apply to the category's **own** button AND SHALL also serve as the
  base for that category's **phrase buttons**, unless `phrase-background` /
  `phrase-text-color` override them at the same layer. *(This is what makes
  an SPC class one property: `::background:#FFD966;` colors the key and its
  phrases.)*

### 6.3 Board layer (`$$board`)

- **STY-F20 [Done]** THE SYSTEM SHALL treat the category name **`$$board`**
  as reserved: WHEN loading categories (registry or import) THE SYSTEM SHALL
  extract it into the board style and SHALL NOT expose it in `m_categories`,
  the button grid, or any category operation; WHEN saving/exporting all
  categories THE SYSTEM SHALL re-emit it.
- **STY-F21 [Done]** THE board style SHALL support: **window properties**
  (unprefixed) `background`, `separator-color`, `text-color`, `font-name`,
  `font-size`, `text-weight`, plus the window-only text fields `caption`,
  `title`, `credits`; and **group properties** prefixed `category-` and
  `phrase-`: `background`, `text-color`, `width`, `height`, `corner-radius`,
  `border-width`, `margin`, `icon-position` (`left|right|top|bottom|center`;
  `center` centers the icon and hides the label unless a non-smart
  `text-layout` is set — [`ssbutton.spec.md`](ssbutton.spec.md) BTN-F30),
  `icon-size`, `font-name`, `font-size`, `text-weight`, `text-layout`.
- **STY-F22 [Done]** THE `text-layout` value SHALL be either one or two
  alignment keywords — horizontal `left|center|right` and/or vertical
  `top|middle|bottom` (order free, space-separated), mapped to the `BS_*`
  alignment styles of [`ssbutton.spec.md`](ssbutton.spec.md) BTN-F40 — **or**
  the single keyword `smart` (alias `auto`), which clears explicit alignment so
  the label falls back to SSButton's centered default — centered within the area
  left over by the icon ([`ssbutton.spec.md`](ssbutton.spec.md) BTN-F45). WHEN
  `text-layout` is **unset**, that centered default already applies. In the
  cascade, `smart` resets an inherited alignment and an explicit axis overrides
  it.
- **STY-F55 [Done]** THE `text-weight` value SHALL be `normal`, `bold`, or a
  numeric `LOGFONT` weight `100`–`900`, applied when building the button/strip
  fonts; the selected category button SHALL always render **at least** bold
  (the greater of the styled weight and bold) so the selection marker stays
  visible.
- **STY-F56 [Done]** WHEN `caption` is set THE SYSTEM SHALL show it in the
  strip between the separators in place of the localized shortcuts hint; it
  does **not** change the window title bar. (Free text; see §14.)
- **STY-F57 [Done]** WHEN an imported file's board style carries `title`
  and/or `credits` THE SYSTEM SHALL append them to the import-success message
  box (below the success text), read from the **imported** file's style so
  they appear even if the user declined to replace an existing local board
  style. `title`/`credits` are metadata: they are stored and round-trip but
  are **not** rendered persistently anywhere.
- **STY-F23 [Done]** IF the user attempts to create or rename a category to
  `$$board` (or any name beginning with `$$`) THEN THE SYSTEM SHALL reject it
  with the existing name-conflict message flow. THE reserved name SHALL NOT
  be localized.
- **STY-F24 [Done]** THE board style SHALL have **no authoring UI in v1**;
  it is set by importing a styled file or editing the registry value.

### 6.4 Category layer

- **STY-F30 [Done]** THE category style SHALL support: `background` and
  `text-color` for the category's own button (with the flow-through of
  STY-F12), and the **full `phrase-*` set of STY-F21** for its phrase
  buttons. Width/height/margin of the category's **own** button SHALL NOT be
  stylable at this layer (all category buttons share one grid).
- **STY-F31 [Done]** IN the F4 edit dialog THE SYSTEM SHALL serialize a
  styled category as `<icon>##<name>::<style list>` and SHALL parse the
  suffix back on OK (split at the **first** `::` after the name start; both
  `##` and `::` parts optional and independent).
- **STY-F32 [Done]** WHEN persisting a category with a style THE SYSTEM
  SHALL store the style as a **style token** (a pseudo-phrase `$$<style list>`)
  in the category's phrase data — never in the registry value name or the
  `.ssc` name token. WHEN loading, style tokens SHALL be extracted into the
  category's style field (multiple tokens concatenated in order) and removed
  from the phrase list before the UI sees it.
- **STY-F33 [Done]** THE raw style string SHALL round-trip **verbatim**
  (registry ⇄ memory ⇄ `.ssc`/`.ssz`), including properties this version does
  not recognize, so newer boards survive editing in older styled versions.

### 6.5 Rendering application

- **STY-F40 [Done]** WHEN the board style sets `background` THE SYSTEM
  SHALL apply it to the category window via an instance brush and an explicit
  `WM_ERASEBKGND` handler (so `SSButton` rounded corners seed correctly per
  [`ssbutton.spec.md`](ssbutton.spec.md) BTN-F21), update `WM_CTLCOLORSTATIC`,
  and update the DWM caption color and immersive dark-mode flag currently
  driven by the taskbar color.
- **STY-F41 [Done]** IF a background color is styled and no `text-color` is
  set at any layer THEN THE default window/button text color SHALL be derived
  from the luminance of the **styled** background (light text on dark, dark
  text on light), replacing the taskbar-luminance derivation for that surface.
- **STY-F42 [Done]** WHEN `separator-color` is set THE SYSTEM SHALL render
  the two separator lines in that color (replacing the un-colorable
  `SS_ETCHEDHORZ` statics with painted/owner-drawn lines).
- **STY-F43 [Done]** THE SYSTEM SHALL build each button's effective
  `SSButtonConfig` (and font) from the cascade at (re)creation and on
  category switch; WHEN a category is selected its phrase buttons SHALL use
  that category's resolved phrase style, re-running `LayoutCalcs()` so
  per-category phrase sizing lays out correctly.
- **STY-F44 [Done]** THE selected-category marker SHALL remain the bold
  font + `BS_FLAT` border over the cascade-resolved colors.
- **STY-F45 [Done]** Effective sizes SHALL compute as
  `built-in default × style percentage (if any, else styled absolute) ×
  zoom factor`; styled `font-name`/`font-size` SHALL be honored wherever
  fonts are (re)built, including `SafeTextResize()` zoom rebuilds. A resolved
  size of 0 SHALL be honored only where meaningful (`margin`, `border-width`,
  `corner-radius` = square, `icon-size` = auto); for layout dimensions
  (`width`, `height`, `font-size`) it SHALL be ignored (fall back). Absolute
  `font-size` is **pixels** (`lfHeight = -px`). Styled `text-weight` (STY-F55)
  SHALL be applied in the same font builds.

### 6.6 Persistence & portability

- **STY-F50 [Done]** Styles SHALL be stored **with the categories, per
  language** (same registry key); WHEN categories are (re)loaded — including
  a language switch — THE board style SHALL be reloaded with them.
- **STY-F51 [Done]** Styles SHALL round-trip through all three carriers:
  registry values, `.ssc` files, and `.ssz` bundles, using the same tokens
  (`$$board` value / `$$` style tokens in phrase-position data).
- **STY-F52 [Done]** WHEN exporting **all** categories THE SYSTEM SHALL
  include the `$$board` entry; WHEN exporting a **single selected** category
  THE SYSTEM SHALL include only that category's own style and NOT `$$board`.
- **STY-F53 [Done]** WHEN an import contains a board style AND a local
  board style already exists THE SYSTEM SHALL ask before replacing it
  (localized Yes/No prompt, same pattern as the category-overwrite prompt);
  WHEN no local board style exists it SHALL be applied without prompting;
  WHEN the incoming style is identical to the local one THE SYSTEM SHALL do
  nothing (no prompt). Strings: `IMPORT_BOARD_STYLE_REPLACE_TITLE_ID` /
  `IMPORT_BOARD_STYLE_REPLACE_MESSAGE_ID`, localized in all 17 languages.
- **STY-F54 [Done]** Older SimonSays versions importing a styled file SHALL
  suffer no parse failure: `$$board` appears as an ordinary category and
  style tokens as ordinary phrases (documented limitation, §17).

### 6.7 Non-functional

- **STY-N01 [Done]** Style parsing SHALL never throw, crash, or block on
  malformed input (fuzz-tolerant by construction: skip-on-error per pair); NO
  new library dependencies SHALL be introduced; identifiers and comments in
  English; new user-visible strings (STY-F53 prompt) localized across all
  supported languages.

---

## 7. Architecture & components

### 7.1 Files (planned)

| File | Responsibility |
|---|---|
| `include/BoardStyle.h`, `src/BoardStyle.cpp` *(new)* | Style-list parser, property structs with per-property "set" flags, cascade resolver. UI-free; depends only on `stdafx.h`. |
| [`include/stdafx.h`](../../include/stdafx.h) | `Category` gains a raw `style` string field; reserved-token constants (`$$board`, `$$` prefix, `::` style suffix). |
| [`src/CategoryWindow.cpp`](../../src/CategoryWindow.cpp) | Applies resolved styles: window background/separators/fonts, effective `SSButtonConfig` per button, layout integration. |
| [`src/RegistryManager.cpp`](../../src/RegistryManager.cpp) | Extract/re-emit `$$board` value and `$$` style tokens at load/save. |
| [`src/utils.cpp`](../../src/utils.cpp) | Same extraction/re-emission for `.ssc` serialize/parse; `SerializeCategory`/`DeserializeCategory` learn the `::` suffix. |

### 7.2 Layering rules

- `BoardStyle` depends on nothing UI-side (no `SSButton`, no windows); it maps
  strings ⇄ typed properties. `CategoryWindow` translates resolved properties
  into `SSButtonConfig`/fonts/brushes.
- `RegistryManager`/`utils` handle tokens **only** at the (de)serialization
  boundary; `m_categories` and the button vectors never contain pseudo-entries
  (this is what keeps every existing CRUD/move/import prompt untouched).
- No changes to `SSButton` are required or permitted by this feature
  (capabilities used are already specified in [`ssbutton.spec.md`](ssbutton.spec.md)).

## 8. Detailed design

### 8.1 Cascade resolution

Per property: value = category layer if set, else board layer if set, else
built-in default. The category layer's unprefixed colors flow to its phrase
buttons (STY-F12) *before* `phrase-*` overrides of the same layer are applied.
Resolution happens when buttons are (re)created and on category selection —
no per-paint cost.

### 8.2 Why these separators are safe

| Carrier | Reserved chars | Why styles don't collide |
|---|---|---|
| Registry value name | `##` | Style never stored in the name (STY-F32); name-buffer limits respected. |
| Registry / `.ssc` phrase data | `\|`, `##`, `::` | Style tokens intercepted by the `$$` prefix **before** `DeserializePhrase`; style lists must not contain the pipe separator (§14). |
| `.ssc` line | first `=` | Style pairs use `:` (never `=`); style lives after the `=` anyway (in phrase-position data). |
| F4 dialog category text | `##`, first `::` | `::` chosen as the explicit style suffix; names containing `::` were already ambiguous (§14). |

### 8.3 Board-style storage shape

Registry: one value named `$$board` under the language key; data = one or
more style-list tokens joined by `|` (a leading `$$` on a token is accepted
and stripped). `.ssc`: line `$$board=<same data>`. Category style token:
`$$<style list>` at any phrase position (conventionally first).

## 9. Data model & persistence

- `Category` gains `std::wstring style;` (raw list, verbatim round-trip per
  STY-F33). The board style is held by the categories' owner and passed to
  `CategoryWindow` alongside them.
- Storage mechanism unchanged — same key, values, and formats as
  [`persistence.spec.md`](persistence.spec.md); this spec only defines the two
  reserved tokens inside them. Per-language scope per STY-F50.

## 10. Key interfaces (planned sketch)

```cpp
// BoardStyle.h — names indicative, final signatures at implementation time
struct StyleProps      // every field paired with a "set" flag (no std::optional needed)
{
  COLORREF background, textColor, separatorColor;
  int widthPct, heightPct, widthPx, heightPx, marginPx /*…*/;
  int cornerRadius, borderWidth, iconSize, fontSizePx;
  SSButtonIconPosition iconPosition; DWORD textLayoutStyle;
  std::wstring fontName;
  /* bool has<Field>; … */
};
struct BoardStyle    { StyleProps window, categoryButtons, phraseButtons; };
struct CategoryStyle { StyleProps ownButton, phraseButtons; };

void ParseStyleList( const std::wstring & list, /*selector-aware*/ BoardStyle & out );
void ParseCategoryStyleList( const std::wstring & list, CategoryStyle & out );
StyleProps ResolveEffective( const StyleProps & base, const StyleProps & over );
```

## 11. UI specification

No new windows or dialogs in v1. Visible surface changes when styles are set:
window background/caption color, painted separators, hint-text color/font,
and button colors/geometry/fonts per cascade. The F4 edit dialog shows and
accepts the `::` style suffix on categories (STY-F31). One new localized
prompt: board-style replacement on import (STY-F53).

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where (planned) |
|---|---|---|
| Reserved board name | `$$board` | `stdafx.h` |
| Style-token prefix | `$$` | `stdafx.h` |
| Category style suffix | `::` (reuses `AUDIO_FILE_SEPARATOR`) | `stdafx.h` |
| Pair / name-value separators | `;` / first `:` | `BoardStyle.cpp` |
| Built-in size defaults (100% basis) | `CATEGORY_BUTTON_WIDTH` 120, `HEIGHT` 40, `MARGIN` 10 (phrases alias them) | [`CategoryWindow.h`](../../include/CategoryWindow.h) |
| Font default (100% basis) | `SPI_GETNONCLIENTMETRICS` message font | `CategoryWindow.cpp` |
| `text-weight` values | `normal` (400) / `bold` (700) / `100`–`900` | `BoardStyle.cpp` |
| Import metadata strings | `IMPORT_BOARD_STYLE_REPLACE_*`; success box reuses `IMPORT_SUCCESS_*` | `stdafx.h` / `localized_strings.h` |

## 13. Diagnostics

Ignored pairs are silent by design (STY-F03). Implementation MAY emit
`OutputDebugString` notes for skipped pairs (pattern of the `.ssz` import
diagnostics); no user-facing diagnostics.

## 14. Edge cases & error handling

- Malformed pair (`no colon`, bad color, negative size) → that pair ignored.
- Duplicate property in one layer → last wins (STY-F04).
- Resolved size ≤ 0 (e.g. `phrase-height:0%`) → ignored, layer above applies.
- `border-width` beyond `[0,2]` → clamped by `SSButton` (BTN-F12); other
  geometry sanitization (corner radius) also inherited from `SSButton`.
- Style lists MUST NOT contain `|` (carrier separator); a `|` truncates the
  token at parse time — practically: font names and `caption`/`title`/`credits`
  text containing `|` are unsupported. A `;` inside a value also ends the pair,
  so free-text fields cannot contain `;` either.
- Category names containing `::` are now split at the first `::` in the F4
  dialog (pre-existing ambiguity with the audio suffix; documented, §17).
- `$$board` present but empty / all pairs invalid → behaves as "no board style".
- Import with no local board style → applied without a replacement prompt
  (STY-F53); the import confirmation box still shows.
- Style-only import (no categories taken, but a new board style adopted) →
  restyles, persists, forces a full erase-redraw so a background/separator
  change repaints in place, and shows the import confirmation box (with any
  title/credits). An import that changes nothing (all categories declined and
  the board style identical/declined) shows no box.
- White SPC keys (`#FFFFFF`): bevel colors derive darker shades automatically
  (BTN-F50), so white keys keep a visible border without a border-color property.

## 15. Acceptance criteria (testable)

> All criteria (AC-1…AC-10) verified in the developer's manual passes of
> 2026-07-08 against `x64\Release\SimonSays.exe` (slice 2: rendering/cascade/
> round-trip; slice 3: import-replacement prompt; slice 4: text-weight/
> caption/title/credits + the style-only-import and caption-refresh fixes).

- **AC-1 (STY-F01–F04) [Pass]** A style list with mixed valid, unknown, and
  malformed pairs applies exactly the valid ones; parser never crashes
  (spot-check with garbage input).
- **AC-2 (STY-F10, F12) [Pass]** With board `phrase-background` set and a
  category `background:` set, that category's key and phrases show the
  category color; other categories' phrases show the board color; removing
  the category pair reverts to the board color.
- **AC-3 (STY-F20, F23) [Pass]** `$$board` never appears as a button;
  F4/F5/F6/F7/F8, name-conflict checks, and export-selected prompts never see
  it; creating a category named `$$board` is rejected.
- **AC-4 (STY-F31–F33) [Pass]** Editing a category to
  `X##Name::background:#FFD966;` colors it; re-opening F4 shows the same
  string; an unknown property (`future-prop:1;`) survives save → reload → F4
  verbatim.
- **AC-5 (STY-F40–F42) [Pass]** Board `background`/`separator-color`/
  `text-color` restyle the window, caption, separators, and hint text; with
  no `text-color`, text auto-contrasts against the styled background.
- **AC-6 (STY-F43–F45) [Pass]** `phrase-height:130%;` at zoom 1.2 yields
  `40 × 1.3 × 1.2 = 62 px` buttons; per-category phrase sizing relayouts on
  selection; Ctrl+±/0 zoom keeps styled fonts.
- **AC-7 (STY-F50–F51) [Pass]** A styled board round-trips registry →
  export `.ssc` → delete → import with identical styles; same via `.ssz`.
- **AC-8 (STY-F52–F53) [Pass]** Export-all carries `$$board`;
  export-selected does not; importing a board style over an existing one
  prompts (localized), over none applies silently.
- **AC-9 (Appendix A) [Pass]** Importing the SPC example renders six
  color-coded categories whose phrase keys match their class color.
- **AC-10 (STY-F55–F57) [Pass]** A board `text-weight:600;` thickens the
  labels while the selected category stays at least bold; `caption:My Board;`
  replaces the shortcuts hint in the strip (title bar unchanged); importing a
  file whose `$$board` carries `title`/`credits` shows them in the success box
  (even when declining to replace an existing style), and they never appear in
  the persistent UI.

Build gate: Debug **and** Release x64 compile clean (apart from pre-existing
warnings noted project-wide).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Style parser (`BoardStyle`) | ✅ Done | `src/BoardStyle.cpp` |
| Cascade resolution (default → board → category) | ✅ Done | `ResolveEffectiveStyle` + `CategoryWindow::ApplyBoardStyle` |
| `$$board` extraction / re-emission (registry) | ✅ Done | `LoadCategoriesFromRegistry`/`SaveCategoriesToRegistry` |
| `$$` style tokens + `::` suffix (F4 dialog) | ✅ Done | `SerializeCategoryWithStyle`/`DeserializeCategory`/`ParseCategoryData` |
| Window background / separators / text application | ✅ Done | `WM_ERASEBKGND` + `WM_CTLCOLORSTATIC` + DWM; separator brush |
| Button config & font application, layout integration | ✅ Done | Per-category configs; per-category phrase metrics on selection |
| `.ssc` / `.ssz` round-trip + export scope rules | ✅ Done | Export-all carries `$$board`; export-selected doesn't |
| Import board-style replacement prompt (localized) | ✅ Done | Yes/No prompt, 17 languages; identical incoming style skips the prompt |
| SPC example board (Appendix A) verified | ✅ Done | Manual pass 2026-07-08 (AC-9) |
| `text-weight` / `caption` / `title` / `credits` | ✅ Done | Manual pass 2026-07-08 (AC-10) |

## 17. Known limitations

- **No authoring UI** for the board layer in v1 (import/registry only).
- **Older versions** show styled files' pseudo-entries as junk categories/
  phrases (harmless; user-deletable) — STY-F54.
- Category-layer sizing of the category's **own** button is unsupported by
  design (shared grid).
- No border-color property: bevel shades always derive from the background.
- Styles are per-language; switching language switches (or drops) the board.

## 18. Future work

- Board-style authoring UI (e.g. a "Board style…" editor or gear entry).
- Named color aliases and/or the SPC palette as presets.
- Border-color / focus-color properties if boards need them.
- App-wide (language-independent) board option, if requested.

## 19. Open questions

1. ~~Should STY-F53's prompt offer "keep both" semantics or only Yes/No
   replace?~~ **Resolved (slice 3): Yes/No replace** — No keeps the local
   style; identical incoming styles skip the prompt entirely.
2. ~~`font-size` absolute unit: pixels or points?~~ **Resolved (slice 2):
   pixels** — applied as `lfHeight = -px` before the zoom multiplier (STY-F45).

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

## Appendix A — SPC adult board example (`.ssc`)

Colors per the SPC recommendation (soft tones, adult boards). Import this as
`spc-board.ssc` (UTF-8):

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

Each category sets one property; STY-F12 flows the class color to its phrase
keys. The board line sets the shared look (background, purple separators,
category emoji on top with the name centered in the band below it via smart
text layout, rounded phrase keys), the strip `caption`, and the
`title`/`credits` shown in the import confirmation box. The text-only phrase
keys have no icon, so smart layout centers them in the whole key.

---

*End of STY-SPEC.*
