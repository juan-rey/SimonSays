# SimonSays — SSButton Custom Button Control — Specification

| | |
|---|---|
| **Spec ID** | BTN-SPEC |
| **Status** | Active — reverse-engineered from shipping source (v0.7); smart text layout + centered icon option added 2026-07-10 |
| **Version** | 1.1 (2026-07-10) |
| **REQ prefix** | `BTN-F##` (functional), `BTN-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Source of truth (code)** | [`include/SSButton.h`](../../include/SSButton.h), [`src/SSButton.cpp`](../../src/SSButton.cpp) |
| **Master spec** | [`docs/spec.md`](../spec.md) |

---

## Contents
- [0. How to use this spec](#0-how-to-use-this-spec)
- [1. Overview](#1-overview)
- [2. Background & context](#2-background--context)
- [3. Goals & non-goals](#3-goals--non-goals)
- [4. Glossary](#4-glossary)
- [5. Personas & scenarios](#5-personas--scenarios)
- [6. Requirements (EARS)](#6-requirements-ears)
- [7. Architecture & components](#7-architecture--components)
- [8. Detailed design](#8-detailed-design)
- [9. Data model & persistence](#9-data-model--persistence)
- [10. Key interfaces](#10-key-interfaces)
- [11. UI specification (visual anatomy)](#11-ui-specification-visual-anatomy)
- [12. Configuration & tuning constants (single source of each)](#12-configuration--tuning-constants-single-source-of-each)
- [13. Diagnostics](#13-diagnostics)
- [14. Edge cases & error handling](#14-edge-cases--error-handling)
- [15. Acceptance criteria (testable)](#15-acceptance-criteria-testable)
- [16. Implementation status matrix](#16-implementation-status-matrix)
- [17. Known limitations & provisional items](#17-known-limitations--provisional-items)
- [18. Future work / out of scope](#18-future-work--out-of-scope)
- [19. Open questions](#19-open-questions)
- [20. Build & run](#20-build--run)

---

## 0. How to use this spec

See [`AGENT.md`](../../AGENT.md) and [`docs/spec.md`](../spec.md) §2 for the shared
working agreement, EARS phrasing, ID scheme, status tags, and acceptance-criteria
/ status-matrix formats. This file is the source of truth for the **`SSButton`
owner-drawn custom button control** — the reusable widget every clickable surface
in SimonSays is built from. Reference format: [`docs/specs/dwell.spec.md`](dwell.spec.md).

This spec was authored **from the existing code**, so per the working agreement
the code is authoritative for current behavior (rule 7): if a fact here drifts
from [`src/SSButton.cpp`](../../src/SSButton.cpp), fix the spec and flag it.

> **Boundary with DWELL-SPEC.** `SSButton` *hosts* the gaze dwell-click feature,
> but the dwell-click behavior, timing, mode resolution, and detection are owned
> by [`docs/specs/dwell.spec.md`](dwell.spec.md) (§6.1/§6.2/§7.4). This spec
> documents the **button surface** the dwell feature plugs into and does **not**
> restate or renumber the dwell requirements; see §6.9.

---

## 1. Overview

`SSButton` is a self-contained, owner-drawn Win32 push-button control wrapping an
`HWND` of a custom registered window class (`"SSButton"`). It replaces the stock
`BUTTON` to give SimonSays full control over appearance (flat / 3D-beveled /
rounded surfaces, custom colors, color-emoji and `.ico` icons, multi-line and
RTL-aware text) while preserving standard button semantics — it fires
`WM_COMMAND` / `BN_CLICKED` to its parent exactly like a real button, supports
mouse, keyboard (Space/Enter), and focus interaction, and integrates with the
dialog manager.

It is also the activation surface for **gaze dwell-click** (§6.9): every
`SSButton` can be triggered by holding the tracking point on it, with an inline
progress bar, driven by the shared `SSDwellConfig` and `GazeProviderChain`.

---

## 2. Background & context

SimonSays renders dense grids of category/phrase buttons plus dialog buttons and
calibration probes, and must theme them consistently (custom palettes, emoji
icons, rounded corners) across light/dark system themes, multiple monitors, RTL
languages, and an experimental gaze workflow. The stock `BUTTON` control cannot
deliver color-emoji glyphs, the bespoke bevel/rounded styling, per-instance dwell
opt-out, or the activation-source tracking the dwell calibration needs — hence a
custom owner-drawn control.

The control is intentionally **dependency-light**: for dwell it touches only
`SSDwellConfig` and `GazeProviderChain` (never concrete gaze providers or the
detector), per the layering rules in [`dwell.spec.md`](dwell.spec.md) §7.4.

---

## 3. Goals & non-goals

**Goals**
- A reusable owner-drawn button with consistent custom theming and standard
  `BN_CLICKED` semantics.
- Color-emoji and `.ico` icon support with flexible placement and auto-sizing.
- Flicker-free rendering; correct behavior across themes, DPI-unaware
  multi-monitor layouts, and RTL.
- Host the gaze dwell-click feature without depending on its internals.
- Movable, non-copyable value type with clean resource ownership.

**Non-goals**
- The dwell-click behavior/timing/detection itself — owned by
  [`dwell.spec.md`](dwell.spec.md).
- Layout/placement of buttons within host windows (owned by the host:
  `CategoryWindow`, `MainWindow`, dialogs).
- Per-monitor-v2 DPI awareness (the process is DPI-unaware; see §17).

---

## 4. Glossary

| Term | Meaning |
|---|---|
| **SSButton** | The custom owner-drawn button control (HWND of class `"SSButton"`). |
| **SSButtonConfig** | The configuration bundle (background, text, border, icon) applied to a button. |
| **Bevel** | The raised/sunken 3D border drawn from derived highlight/shadow colors. |
| **Staging surface** | The process-wide D2D DC-render-target + DIB used to composite color emoji. |
| **Activation source** | How the last `BN_CLICKED` was produced: None / Mouse / Keyboard / Dwell. |
| **Host** | The parent window that owns/places SSButtons and receives their `BN_CLICKED`. |

---

## 5. Personas & scenarios

- **H1 — Category/phrase grid (`CategoryWindow`)**: many SSButtons with emoji
  icons and custom colors; F-keys must reach the host for hotkeys; gaze dwell
  activates them.
- **H2 — Dialog buttons (Settings, Dwell window, Edit)**: OK/Cancel and probes;
  Tab navigation, Space/Enter activation, focus ring.
- **H3 — Dwell calibration probes (Dwell window)**: one dwell-enabled, one
  dwell-disabled SSButton; the host reads `GetLastActivation()` to distinguish a
  gaze dwell from a physical click (see [`dwell.spec.md`](dwell.spec.md) §6.5).

---

## 6. Requirements (EARS)

Status tags per [`docs/spec.md`](../spec.md) §2.3. All requirements below are
implemented in the current source; tags reflect that, with `[Done*]` where
correctness depends on an environment assumption.

### 6.1 Window class & instance lifecycle

- **BTN-F01 [Done]** THE SYSTEM SHALL register a process-wide `WNDCLASSEX` named
  `"SSButton"` (`CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS`, no class background brush)
  exactly once, treating `ERROR_CLASS_ALREADY_EXISTS` as success.
- **BTN-F02 [Done]** WHEN `Create()` is called THE SYSTEM SHALL create a
  `WS_CHILD` window of the `"SSButton"` class with the supplied id/text/rect/
  styles/config and store the instance pointer in `GWLP_USERDATA` (set on
  `WM_CREATE`).
- **BTN-F03 [Done]** THE SSButton SHALL be **movable but non-copyable**; on move
  THE SYSTEM SHALL re-point `GWLP_USERDATA` to the new instance address and leave
  the moved-from object unable to destroy the `HWND`.
- **BTN-F04 [Done]** WHEN an SSButton is destroyed (or `Destroy()` is called) THE
  SYSTEM SHALL destroy the owned `HWND` if valid and release the loaded icon; on
  `WM_DESTROY` it SHALL cancel any running dwell timer **while the HWND is still
  valid** and clear the cached handle.
- **BTN-F05 [Done]** THE SYSTEM SHALL provide a static `ReleaseSharedResources()`
  that frees the process-wide D2D factory, DWrite factory, cached text format,
  and emoji staging surfaces, to be called once after the last button is
  destroyed.

### 6.2 Configuration & appearance model

- **BTN-F10 [Done]** THE SYSTEM SHALL accept an `SSButtonConfig` (background type/
  color, text color type/color, border style/corner-radius/width, icon type/
  position/emoji/path/size/padding) with safe defaults, applied via `SetConfig()`
  with a repaint.
- **BTN-F11 [Done]** WHEN `SetConfig()` changes the icon file path, THE SYSTEM
  SHALL release the previously loaded icon and, for `StandardIcon`, load the
  `.ico` from file at the configured size (or `SSBUTTON_ICON_DEFAULT_SIZE`); WHEN
  the icon type changes away from `StandardIcon` it SHALL release the icon handle
  even if the path is unchanged.
- **BTN-F12 [Done]** THE SYSTEM SHALL clamp `borderWidth` to `[0, 2]`.

### 6.3 Rendering (owner-draw)

- **BTN-F20 [Done]** WHILE handling `WM_PAINT` THE SYSTEM SHALL render the whole
  control into an off-screen buffer and blit it (double-buffered), and SHALL
  suppress background erase (`WM_ERASEBKGND` returns non-zero).
- **BTN-F21 [Done]** THE SYSTEM SHALL fill the surface with the state-resolved
  background color: `Square` via `FillRect`; `Rounded` via `RoundRect` over a
  buffer first seeded with the **parent's** background (so rounded corners show
  the parent through instead of black).
- **BTN-F22 [Done]** THE background color SHALL resolve by state in priority
  order: disabled → disabled color; pressed → pressed color; hovered **or**
  focused → hover color; otherwise base background.
- **BTN-F23 [Done]** WHEN `borderWidth > 0` THE SYSTEM SHALL draw a raised
  (unpressed) or sunken (pressed) bevel from the derived highlight/light/shadow/
  dark-shadow colors, in `Square` or `Rounded` geometry; a `BS_FLAT` button SHALL
  draw a single flat dark outline.
- **BTN-F24 [Done]** THE content area SHALL be inset by `borderWidth`; WHEN
  pressed and not `Rounded` THE SYSTEM SHALL offset content by `(1,1)`.
- **BTN-F25 [Done]** WHILE focused and enabled THE SYSTEM SHALL draw a dotted
  (`PS_DOT`) focus outline just outside the border (`Square` or `Rounded`), using
  the resolved text color as the pen.

### 6.4 Icons & emoji

- **BTN-F30 [Done]** THE SYSTEM SHALL place the icon/emoji at the configured edge
  (`Left`/`Right`/`Top`/`Bottom`), auto-sizing it from the content dimension
  perpendicular to that edge when `iconSize == 0`, and shrink the label area
  accordingly. WHEN the position is `Center` THE SYSTEM SHALL center the icon in
  the whole content area (no band carved; auto-size from `min(width, height)`);
  while an icon is present it SHALL **suppress the label** under smart layout
  (icon-only), but IF an explicit `BS_*` alignment is set (a non-smart
  `text-layout`) THEN it SHALL draw the label at that alignment over the
  centered icon.
- **BTN-F31 [Done]** THE SYSTEM SHALL draw a `StandardIcon` `HICON` centered in
  its area via `DrawIconEx`.
- **BTN-F32 [Done*]** THE SYSTEM SHALL render an `Emoji` with Segoe UI Emoji
  **color** glyphs through a Direct2D `ID2D1Factory1` DC render target
  (`PREMULTIPLIED` alpha, `GRAYSCALE` AA) over a top-down 32-bpp DIB, composited
  to the target via `AlphaBlend`, using `ID2D1DeviceContext::DrawText` with
  `ENABLE_COLOR_FONT`. *Color output requires `ID2D1Factory1` + Windows 8.1+;
  see BTN-F33 fallback.*
- **BTN-F33 [Done]** IF the device-context color path is unavailable (pre-8.1)
  THEN THE SYSTEM SHALL draw the emoji **monochrome**; WHILE the button is
  disabled THE emoji SHALL be drawn grey.
- **BTN-F34 [Done*]** THE emoji render target SHALL be created at **96 DPI**
  (DIPs == pixels) and its staging surfaces cached process-wide by pixel size,
  recreated on `D2DERR_RECREATE_TARGET`. *Correctness of px==DIP relies on the
  process being DPI-unaware (see [`dwell.spec.md`](dwell.spec.md) REQ-F72, §17).*

### 6.5 Text & font

- **BTN-F40 [Done]** THE SYSTEM SHALL draw the label honoring `BS_LEFT`/
  `BS_RIGHT`/`BS_CENTER` horizontally and `BS_TOP`/`BS_BOTTOM`/`BS_VCENTER`
  vertically; an axis with **no** explicit bit is centered (BTN-F45).
- **BTN-F41 [Done]** WHEN `BS_MULTILINE` is set THE SYSTEM SHALL word-wrap the
  label (`DT_WORDBREAK`) and vertically position the wrapped block (top/bottom/
  center per the resolved vertical alignment) using a `DT_CALCRECT` measure pass.
- **BTN-F45 [Done]** FOR a text axis with **no** explicit `BS_*` bit THE SYSTEM
  SHALL center the label on that axis. Because the label is drawn in the content
  rect **after the icon area has been carved out**, this centers the label
  within the space left over by the icon (icon `Top`→caption centered in the
  band below it, `Left`→text centered in the band beside it, …), or within the
  whole interior when there is no icon. This centered placement is the built-in
  default ("smart" layout); an explicit `BS_*` bit pins the label to that edge
  of the leftover area instead.
- **BTN-F42 [Done]** WHILE disabled THE label SHALL render in `COLOR_GRAYTEXT`.
- **BTN-F43 [Done]** `SetText()` / `WM_SETTEXT` SHALL keep the cached label
  string in sync with the window text; a null text pointer SHALL be treated as
  empty.
- **BTN-F44 [Done]** THE SYSTEM SHALL render with the font supplied via
  `SetFont()` / `WM_SETFONT` (**not owned**) and return it on `WM_GETFONT`.

### 6.6 Colors & theming

- **BTN-F50 [Done]** WHEN custom colors are set THE SYSTEM SHALL derive hover
  (delta +18), pressed (−25), and disabled (+20) shades, and the highlight/light/
  shadow/dark-shadow border colors via brightness factors, from the base
  background; custom text color applies only when text color type is `Custom`.
- **BTN-F51 [Done]** `SetSystemColors()` SHALL source face/text/bevel colors from
  `GetSysColor` and SHALL always cache a concrete text color (the focus pen reads
  it; `CreatePen(CLR_NONE)` would yield garbage).
- **BTN-F52 [Done]** WHEN `WM_SYSCOLORCHANGE` / `WM_THEMECHANGED` arrives and
  background type is `Default` THE SYSTEM SHALL refresh the system-derived colors
  and repaint.

### 6.7 Activation — mouse, keyboard, focus

- **BTN-F60 [Done]** ON activation THE SYSTEM SHALL post `WM_COMMAND` with
  `BN_CLICKED` to the parent, identical to a standard `BUTTON`.
- **BTN-F61 [Done]** WHEN the left mouse button is pressed THE SYSTEM SHALL
  capture the mouse, set focus, and mark pressed; WHEN released **inside** the
  client rect while enabled it SHALL set activation source = `Mouse` and fire the
  click. (Press also cancels any dwell and starts a cooldown — see §6.9 /
  [`dwell.spec.md`](dwell.spec.md) REQ-F16.)
- **BTN-F62 [Done]** WHEN `Space` or `Enter` is pressed and released while enabled
  THE SYSTEM SHALL set activation source = `Keyboard` and fire the click.
- **BTN-F63 [Done]** WHILE the cursor is over the button THE SYSTEM SHALL show the
  hover state and request `WM_MOUSELEAVE`; ON leave it SHALL clear hover/pressed
  (and cancel dwell, §6.9).
- **BTN-F64 [Done]** `WM_SETFOCUS` / `WM_KILLFOCUS` SHALL toggle the focus visual;
  kill-focus SHALL also clear pressed and key-held state.
- **BTN-F65 [Done]** THE SYSTEM SHALL expose the most recent activation source
  (`None`/`Mouse`/`Keyboard`/`Dwell`) via `GetLastActivation()`.

### 6.8 Dialog & layout integration

- **BTN-F70 [Done]** THE SYSTEM SHALL report `DLGC_BUTTON | DLGC_WANTALLKEYS`
  (and **not** `DLGC_WANTTAB`) so it receives all keys — including F-keys — while
  `Tab` still navigates between siblings.
- **BTN-F71 [Done]** WHEN a `WM_KEYDOWN` for `VK_F1`..`VK_F24` arrives THE SYSTEM
  SHALL forward it to the parent and consume it, so host F-key hotkeys (e.g.
  `CategoryWindow` F3–F10) work even when an SSButton has focus.
- **BTN-F72 [Done]** `updateRtlExStyle()` SHALL toggle
  `WS_EX_LAYOUTRTL | WS_EX_RTLREADING` and reframe (`SWP_FRAMECHANGED`); GDI
  mirroring then renders a logical-`Left` icon/text on the visual right.
- **BTN-F73 [Done]** `SetStyle()` / `WM_STYLECHANGED` SHALL update the cached
  style/ex-style and repaint.

### 6.9 Gaze dwell-click hosting (owned by DWELL-SPEC)

> These requirements describe only the **host contract**. The behavior is
> specified and owned by [`dwell.spec.md`](dwell.spec.md) §6.1/§6.2 — do not
> duplicate or renumber those `REQ-*` here.

- **BTN-F80 [Done]** THE SSButton SHALL serve as the dwell-click activation
  surface, providing `RouteGaze()` (static; arm the button under a gaze point),
  `GetTrackingPoint()` (via `GazeProviderChain` per the active mode), the
  `OnDwellMouseMove`/`StartDwell`/`UpdateDwell`/`CancelDwell` fixation state
  machine, the bottom-edge progress bar, per-instance `SetDwellEnabled()` opt-out,
  and cooldown on real click — **all behavior per
  [`dwell.spec.md`](dwell.spec.md) §6.2**.
- **BTN-F81 [Done]** THE dwell shared types — `SSDwellConfig` (thread-safe
  process-wide singleton) and the `SSDwellMode` / `SSDwellModeSelection` /
  `SSButtonActivation` enums — SHALL be **declared in this module's headers**
  ([`include/SSButton.h`](../../include/SSButton.h)) and reused everywhere, per
  [`dwell.spec.md`](dwell.spec.md) §7.4. The control SHALL depend only on
  `SSDwellConfig` and `GazeProviderChain`, never on concrete providers or the
  detector.

### 6.10 Non-functional

- **BTN-N01 [Done]** Win32 + C++ at the project standard (MSVC, toolset `v145`);
  SHALL NOT bump the standard. Links `d2d1.lib`, `dwrite.lib`, `msimg32.lib` (via
  `#pragma comment`) for the emoji pipeline and `AlphaBlend`.
- **BTN-N02 [Done]** Rendering SHALL be flicker-free via full double-buffering.
- **BTN-N03 [Done]** SSButton instances are UI-thread affine; the **only**
  cross-thread surface is `SSDwellConfig` (atomics + a mutex-guarded color), so
  the detector/UI threads may read/write tuning concurrently.
- **BTN-N04 [Done]** No per-button or process leaks: icons are released on
  reconfig/destroy; the process-wide D2D/DWrite/GDI staging is released via
  `ReleaseSharedResources()`.
- **BTN-N05 [Done]** Identifiers and comments in English; user-visible strings
  are supplied by the host (the control renders given text), keeping localization
  in the host/localization layer.

---

## 7. Architecture & components

### 7.1 Files

| File | Responsibility |
|---|---|
| [`include/SSButton.h`](../../include/SSButton.h) | `SSButton` class, `SSButtonConfig`, appearance enums; **also** `SSDwellConfig` + dwell enums (shared, per DWELL-SPEC §7.4). |
| [`src/SSButton.cpp`](../../src/SSButton.cpp) | Window proc, owner-draw paint pipeline, color-emoji D2D/DWrite statics, dwell state machine, `SSDwellConfig` impl. |

### 7.2 Internal structure

- **Window class & wndproc** — `RegisterWindowClass` + static `WindowProc`
  dispatching paint/input/focus/timer/lifecycle messages; instance recovered from
  `GWLP_USERDATA`.
- **Paint pipeline** — `Paint()` composes: parent-seed (rounded) → background →
  bevel border (`OutlineRect` / `OutlineRoundRect` lambdas) → content inset →
  icon/emoji → label → focus ring → dwell progress bar → `BitBlt`.
- **Color-emoji subsystem** — process-wide statics (`s_pD2DFactory` =
  `ID2D1Factory1`, `s_pDWriteFactory`, `s_pTextFmt`, DC render target + DIB
  staging), built lazily by `EnsureEmojiStaging`, drawn by `DrawEmoji`, freed by
  `ReleaseEmojiStaging` / `ReleaseSharedResources`.
- **Dwell surface** — `SSDwellConfig` singleton + the per-instance fixation state
  and timer; arming via `WM_MOUSEMOVE` (cursor) or `RouteGaze` (HID).

### 7.3 Dependency / layering rules

- The control depends on `stdafx.h`, the D2D/DWrite headers, and (for dwell)
  `GazeProviderChain` + `SSDwellConfig` only — **never** on concrete gaze
  providers, the HID reader, or the detector (enforces
  [`dwell.spec.md`](dwell.spec.md) §7.4).
- Hosts depend on `SSButton`; `SSButton` does not depend on any host.

---

## 8. Detailed design

### 8.1 Paint order
Parent background seed (Rounded only, via `WM_ERASEBKGND` to parent with a
viewport offset) → background fill → bevel (skipped if `borderWidth==0` or
`BS_FLAT` single outline) → content rect (inset by border; `(1,1)` offset when
pressed & Square) → icon/emoji (auto-sized, edge-placed) → label (aligned;
multi-line measured for vertical placement) → dotted focus ring (focused &
enabled) → dwell progress strip → `BitBlt` to screen.

### 8.2 Color emoji invariants
Color glyphs require **all three** simultaneously: `ID2D1Factory1` (plain
`ID2D1Factory` DCRT ignores color fonts), `PREMULTIPLIED` alpha (IGNORE degrades
to monochrome), and `GRAYSCALE` text AA (ClearType cannot composite color onto a
transparent target). Draw goes through `ID2D1DeviceContext::DrawText` +
`ENABLE_COLOR_FONT`; absent that QI, a monochrome `DrawText` fallback is used.

### 8.3 State machine (input)
`Idle → (LBUTTONDOWN: capture+pressed) → LBUTTONUP inside → fire(Mouse)`; keyboard
`Space/Enter down → pressed → up → fire(Keyboard)`; `MOUSELEAVE`/`KILLFOCUS`
clear transient state. Dwell adds a parallel timer-driven path — see
[`dwell.spec.md`](dwell.spec.md) §8.1.

### 8.4 Dwell hosting — see [`dwell.spec.md`](dwell.spec.md) §8.1–§8.3.

---

## 9. Data model & persistence

- `SSButton` itself holds **no persistent state**; appearance comes from the
  host-supplied `SSButtonConfig` each session.
- The shared `SSDwellConfig` tuning **is** persisted, but via the dwell/registry
  path — see [`dwell.spec.md`](dwell.spec.md) §9 and
  [`persistence.spec.md`](persistence.spec.md). This module does not own that
  storage. *(§9 otherwise N/A for the control.)*

---

## 10. Key interfaces

```cpp
// Lifecycle / creation
static bool RegisterWindowClass(HINSTANCE);
HWND Create(HWND parent, HINSTANCE, int id, const std::wstring& text,
            int x, int y, int w, int h, DWORD style, DWORD exStyle = 0,
            const SSButtonConfig& config = {});
static void ReleaseSharedResources();          // call once at shutdown

// Appearance
const SSButtonConfig& GetConfig() const; void SetConfig(const SSButtonConfig&);
void SetColors(COLORREF bg, COLORREF text=CLR_NONE, COLORREF hover=CLR_NONE,
               COLORREF pressed=CLR_NONE, COLORREF disabled=CLR_NONE);
void SetSystemColors(bool includeTextColor=false);
void SetIcon(const std::wstring& icoPath, int size=0, bool updatePos=false,
             SSButtonIconPosition=SSButtonIconPosition::Left);
void SetEmoji(const std::wstring& emoji, int size=0, bool updatePos=false,
              SSButtonIconPosition=SSButtonIconPosition::Left);
void NoIcon();
void SetFont(HFONT, bool redraw=true);
void SetText(const std::wstring&); const std::wstring& GetText() const;
void SetStyle(DWORD style, bool reframe=true);
void updateRtlExStyle(bool isRtl);

// Geometry / state
void SetPos(int,int,int,int, UINT swpFlags=SWP_NOZORDER|SWP_NOACTIVATE);
void Invalidate(bool eraseBackground=true);
void SetFocus(); void SetEnabled(bool); bool IsEnabled() const;
bool IsValid() const; void Show(bool=true); void Hide(); void Destroy();
bool GetWindowRect(RECT&) const;

// Activation / dwell host
SSButtonActivation GetLastActivation() const;  // None/Mouse/Keyboard/Dwell
void SetDwellEnabled(bool); bool IsDwellEnabled() const;
static bool RouteGaze(POINT screenPt);         // owned by DWELL-SPEC
```

Configuration types: `SSButtonConfig`; enums `SSButtonBackground`,
`SSButtonTextColor`, `SSButtonBorderStyle`, `SSButtonIconType`,
`SSButtonIconPosition`, `SSButtonActivation`. Shared dwell types (see
DWELL-SPEC): `SSDwellConfig`, `SSDwellMode`, `SSDwellModeSelection`.

---

## 11. UI specification (visual anatomy)

A rendered SSButton, outer→inner: **bevel/flat/rounded border** → **background**
(state-colored) → **icon/emoji** at one edge or centered → **label** (aligned;
multi-line aware; suppressed when the icon is centered under smart layout) →
**dotted focus ring**
(when focused) → **dwell progress bar** (bottom
edge, while dwelling). Honors `BS_FLAT`, `BS_MULTILINE`, `BS_LEFT/RIGHT/CENTER`,
`BS_TOP/BOTTOM/VCENTER`, `WS_TABSTOP`, and RTL ex-styles; unspecified text axes
center within the icon-reduced content area (label centered in the space left
by the icon). No control-owned dialog (it is a child control placed by hosts).

---

## 12. Configuration & tuning constants (single source of each)

| Constant | Value | Where |
|---|---|---|
| Window class name | `"SSButton"` | `SSButton.h` `SSBUTTON_CLASS` |
| Default icon size | 64 px | `SSButton.h` `SSBUTTON_ICON_DEFAULT_SIZE` |
| Dwell timer id | `0xD3E11` | `SSButton.cpp` `SSBUTTON_DWELL_TIMER_ID` |
| Dwell timer interval | 33 ms (~30 fps) | `SSButton.cpp` `SSBUTTON_DWELL_INTERVAL` |
| Progress bar height | 4 px | `SSButton.cpp` `SSBUTTON_DWELL_BAR_HEIGHT` |
| Hover/pressed/disabled deltas | +18 / −25 / +20 | `SSButton.cpp` `SetColors`/`SetSystemColors` |
| Border brightness factors | 1.45 / 1.15 / 0.75 / 0.55 | `SSButton.cpp` `SetColors` |
| Border width clamp | `[0, 2]` | `SSButton.cpp` `SetConfig` |
| Config defaults | cornerRadius 6, borderWidth 2, iconPadding 2, iconSize 0 (auto) | `SSButton.h` `SSButtonConfig` |
| Emoji target DPI | 96 (DIP==px) | `SSButton.cpp` `EnsureEmojiStaging` |
| Dwell tuning defaults | time 800 ms, tol 35 px, cooldown 300 ms, color `RGB(0,120,215)` | `SSButton.h` `SSDwellConfig` — *owned by [`dwell.spec.md`](dwell.spec.md) §9* |

---

## 13. Diagnostics

N/A — the control has no diagnostic dump of its own. (HID/gaze diagnostics live in
[`dwell.spec.md`](dwell.spec.md) §13.)

---

## 14. Edge cases & error handling

- All `HWND`-touching wrappers are **no-ops without a window**.
- Class already registered by a prior instance → `ERROR_CLASS_ALREADY_EXISTS`
  treated as success.
- `D2DERR_RECREATE_TARGET` from the emoji target → staging released and rebuilt
  on next paint; the D2D/DWrite factories are **not** torn down.
- Null `WM_SETTEXT` lParam → empty label.
- Disabled button (`WM_ENABLE` false) cancels any in-progress dwell.
- `WM_CAPTURECHANGED` to another window clears the pressed state.
- Moved-from SSButton will not destroy the (now re-owned) HWND.
- Zero-size icon area (`autoSize <= 0`) → icon/emoji skipped, label fills.

---

## 15. Acceptance criteria (testable)

- **AC-1 (BTN-F01/F02) [Pass]** Creating multiple SSButtons registers the class
  once; a `Create` after the class exists still succeeds.
- **AC-2 (BTN-F20/F21) [Pass]** Rendering is flicker-free; a `Rounded` button
  shows the parent background through its corners (not black).
- **AC-3 (BTN-F22/F23/F25) [Pass]** Background tracks disabled/pressed/hover/focus
  state; bevel inverts when pressed; a dotted focus ring shows when focused.
- **AC-4 (BTN-F30–F34) [Pass]** Emoji render in **color** on Win8.1+, monochrome
  on older systems, grey when disabled; icon/emoji honor Left/Right/Top/Bottom
  and auto-size when `iconSize==0`.
- **AC-5 (BTN-F40/F41/F42) [Pass]** Label honors `BS_*` alignment; `BS_MULTILINE`
  wraps and vertically positions; disabled text is grayed.
- **AC-11 (BTN-F45) [Pass]** With no explicit `BS_*` bits: an icon-`Top`
  button centers its caption in the band below the icon (single- and
  multi-line), icon-`Left` centers the text in the band beside the icon, a
  no-icon button centers on both axes; an explicit `text-layout:top` pins the
  caption to the top of the leftover band.
- **AC-12 (BTN-F30) [Pass]** An icon at position `Center` with smart layout
  renders centered with **no** caption (icon-only); adding an explicit
  `text-layout` (e.g. `bottom`) brings the caption back, drawn at that
  alignment over the centered icon; the same via a board's
  `category-icon-position:center` / `phrase-icon-position:center`; a `Center`
  position on a text-only button (no icon) just shows the centered text.
- **AC-6 (BTN-F60–F62/F65) [Pass]** Mouse-release-inside and Space/Enter each post
  `BN_CLICKED`, and `GetLastActivation()` reports the matching source.
- **AC-7 (BTN-F70/F71) [Pass]** F-keys reach the parent (host hotkeys fire) while
  `Tab` still navigates; SSButton reports `DLGC_BUTTON|DLGC_WANTALLKEYS`.
- **AC-8 (BTN-F72) [Pass]** With RTL ex-styles set, a logical-`Left` icon renders
  on the visual right.
- **AC-9 (BTN-F80/F81/N03) [Pass]** Dwell hosting works while SSButton references
  only `SSDwellConfig`/`GazeProviderChain` (compile-time check); dwell behavior
  per [`dwell.spec.md`](dwell.spec.md) AC-1..AC-9.
- **AC-10 (BTN-N02/N04) [Pass]** No flicker; icons freed on reconfig/destroy;
  `ReleaseSharedResources()` frees D2D/DWrite/GDI singletons at shutdown.

Build gate: Debug **and** Release x64 compile clean (apart from pre-existing
warnings noted project-wide).

---

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| Window class + lifecycle + move semantics | ✅ Done | `GWLP_USERDATA` re-pointed on move |
| Owner-draw paint (double-buffered) | ✅ Done | Square / Rounded / Flat |
| State color model + theme refresh | ✅ Done | Custom + system-derived |
| Bevel border + dotted focus ring | ✅ Done | Raised/sunken; rounded via AngleArc |
| Standard `.ico` icon | ✅ Done | `DrawIconEx`, centered |
| Icon positions incl. `Center` (icon-only) | ✅ Done | Left/Right/Top/Bottom + centered, label suppressed |
| Color emoji pipeline | ⚠️ Done\* | Color needs Factory1 + Win8.1+; monochrome fallback |
| Emoji DPI lock + size cache | ⚠️ Done\* | 96-DPI / px==DIP assumes DPI-unaware process |
| Label alignment + multiline + grayed disabled | ✅ Done | |
| Smart text layout (label centered in leftover area; default) | ✅ Done | Unspecified axes center in the icon-reduced content rect |
| External font (`WM_SETFONT`/`GETFONT`) | ✅ Done | Not owned |
| Mouse / keyboard / focus activation + `BN_CLICKED` | ✅ Done | Activation source tracked |
| Dialog integration (dlgcode, F-key forward) | ✅ Done | F1–F24 → parent; Tab navigates |
| RTL (`updateRtlExStyle`) | ✅ Done | GDI mirroring |
| Dwell-click hosting | ✅ Done | Behavior owned by DWELL-SPEC |
| Shared D2D/DWrite/GDI resource release | ✅ Done | `ReleaseSharedResources()` |

---

## 17. Known limitations & provisional items

- **Color emoji needs `ID2D1Factory1` + Windows 8.1+**; older systems fall back to
  monochrome (BTN-F32/F33).
- **Emoji rendering is DPI-locked at 96** and assumes a DPI-unaware process (px==
  DIP); consistent with [`dwell.spec.md`](dwell.spec.md) REQ-F72 but not correct
  under per-monitor-v2 DPI.
- **Emoji staging cache holds a single size** (last w×h); alternating emoji sizes
  rebuild the staging surface each time.
- **`borderWidth` is capped at 2**; thicker borders are not supported by design.
- **One dwell timer per button** (fixed timer id) — a button dwells one fixation
  at a time.
- **Focus uses the hover color** for its background (BTN-F22) — see §19.

---

## 18. Future work / out of scope

- Per-monitor-v2 DPI awareness for the emoji pipeline (coordinate with the
  app-wide DPI model in [`dwell.spec.md`](dwell.spec.md) §17/§18).
- Optional dark-mode / themed palette presets.
- **PNG icon support** — accept `.png` images as a `StandardIcon` source
  (alongside `.ico` and color emoji), decoded through the existing
  Direct2D/WIC stack, so higher-resolution/alpha icons can be used on buttons.
- (Out of scope here) dwell-click enhancements — tracked in
  [`dwell.spec.md`](dwell.spec.md) §18.

---

## 19. Open questions

1. **Focus vs. hover color** — `ResolvedBgColor` returns the hover color for a
   focused-but-not-hovered button (BTN-F22). Intended, or should focus have its
   own shade?
2. **Pressed content offset** — the `(1,1)` press shift applies to `Square` only,
   not `Rounded` (BTN-F24). Intended visual difference?
3. **Emoji cache thrash** — should the staging cache keep a small set of recent
   sizes rather than a single last-size, given mixed icon sizes in a grid?

---

## 20. Build & run

- Build: `MSBuild SimonSays.vcxproj /p:Configuration={Debug|Release} /p:Platform=x64`.
- Verify against `x64\Release\SimonSays.exe`.
- The control links `d2d1.lib`, `dwrite.lib`, `msimg32.lib` via `#pragma comment`
  in [`src/SSButton.cpp`](../../src/SSButton.cpp); both source files are already
  registered in `SimonSays.vcxproj(.filters)`.

---

*End of BTN-SPEC v1.1.*
