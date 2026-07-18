# simonsays-web

WordPress plugins companion to **SimonSays – Simply Speak**, the Windows
desktop AAC (text-to-speech) application for people with ALS and other speech
impairments ([juan-rey/SimonSays](https://github.com/juan-rey/SimonSays)).

**License: GPL-2.0-or-later, repository-wide** (the WordPress-compatible
license). The desktop application is a separate work under its own license
(OSL-3.0); no desktop code is included here.

## Plugins

| Plugin | What it does |
|---|---|
| [`simonsays-board-editor/`](simonsays-board-editor/) | Stateless in-browser editor for SimonSays communication boards (`.ssc` / `.ssz`): live preview, style editing, templates, ARASAAC pictogram search. Board files never leave the visitor's browser. |

## Format contracts

The `.ssc` / `.ssz` file formats and the board style system are owned by the
desktop repository. The canonical reference guides live there and are licensed
**CC BY 4.0** so they can be carried here:

- Canonical: [ssc-ssz-format-reference.md](https://github.com/juan-rey/SimonSays/blob/main/docs/guides/ssc-ssz-format-reference.md) · [board-style-reference.md](https://github.com/juan-rey/SimonSays/blob/main/docs/guides/board-style-reference.md)
- Vendored, pinned copies: [`docs/format/`](docs/format/) — each file's header
  states the vendoring date. When the desktop app changes the format, its specs
  mandate updating the canonical guides and refreshing these copies
  (STY-N02 / PORT-N04 in the desktop repo).

## Development

Each plugin folder is self-contained (no build step, no runtime dependencies
beyond WordPress/PHP built-ins and browser platform APIs). See each plugin's
own `README.md` for its architecture and test instructions.
