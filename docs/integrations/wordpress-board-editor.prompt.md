# Build "SimonSays Board Editor" — a WordPress plugin

> Agent prompt. Hand this file to the implementing agent **together with the two
> normative attachments** listed below (they live in this repo under
> [`docs/guides/`](../guides/)).

## Context
SimonSays – Simply Speak is a Windows desktop AAC (text-to-speech) app. Users
organize spoken phrases into categories ("boards") shared as files: `.ssc`
(plain UTF-8 text) and `.ssz` (a Zip bundle: one `categories.ssc` plus
icon/audio resources). Boards can carry a CSS-like style layer (colors, sizes,
fonts, captions) used e.g. for SPC/Fitzgerald color-coded communication boards.

## Normative attachments (the contract — read first)
- `ssc-ssz-format-reference.md` — on-disk format: header, line/marker grammar
  (`##`, `::`, `|`, `$$`, `♫`), Zip layout, security limits, generation checklist.
- `board-style-reference.md` — style system: `property:value;` syntax, value
  forms, three-layer cascade, full property tables, reserved tokens, constraints.

These two documents are authoritative for the formats. Where they leave
something ambiguous, ASK — do not guess. Do not invent format extensions.

## Goal
A WordPress plugin exposing a **stateless, front-end** board editor: visitors
create, import, edit, live-preview, validate, and export SimonSays boards
entirely in the browser. The server stores no board data.

## Architecture (design constraint: client-side-first)
- ALL board processing runs in the browser: `.ssc` parse/serialize, `.ssz`
  read/write, preview, validation. Files open via file picker and save via
  download; **board bytes never reach the server**. One small permissively
  licensed client zip library is acceptable (e.g. fflate) — justify the choice.
  No server upload/download endpoints for boards.
- **ARASAAC search runs directly from the visitor's browser** against the
  official API (a public API intended for third-party apps; pictogram images
  come from ARASAAC's static CDN). Cache results client-side (memory/
  sessionStorage). ONLY if implementation verification demonstrates a CORS
  block, fall back to a single host-pinned, rate-limited, transient-cached
  server proxy — otherwise the plugin ships NO server endpoints beyond
  settings, i18n, and asset delivery.
- Work-loss protection: autosave the in-progress board to localStorage with
  recovery on reload, plus a beforeunload warning. (Client-side only — does
  not violate statelessness.)

## Users & access
- The editor embeds on the front end via a Gutenberg block AND a shortcode
  (e.g. `[simonsays_board_editor]`); an equivalent wp-admin page is optional.
- Settings choose exposure: everyone vs logged-in users only. Anonymous use is
  the expected default — the stateless design makes that safe.
- The Settings page itself is admin-only (capability-gated, nonce-protected).

## Functional requirements
1. **Board lifecycle (stateless)** — new from template or blank; open
   `.ssc`/`.ssz` from disk; save as download; "duplicate" = save-as. No server
   persistence of boards, ever.
2. **Category/phrase editing** — CRUD + drag-reorder: name/text, icon (emoji
   picker or image file), optional audio per phrase. Enforce the format's
   reserved characters in every input (reject `|`, `##`, `::`, `$$`, `=`, `♫`
   in names/texts; `;`/`|` in style free-text) with inline localized errors.
3. **Style editing** — form editors for the full board layer (window +
   category-/phrase- groups) and per-category layer per the style reference:
   colors (#RRGGBB), sizes (px / % of default), fonts, weight, icon
   position/size, text layout, caption/title/credits/resource-folder. Provide
   a raw style-list field as escape hatch. Unrecognized style properties MUST
   round-trip verbatim.
4. **Live preview** — client-side approximation of the app's category window:
   resolve the cascade (built-in → board → category, incl. own-color →
   phrase-button flow-through), category grid + selected category's phrase
   grid + caption strip, emoji and image icons, derived hover shades, and
   **RTL mirroring** when the board language is RTL (Arabic/Hebrew). Debounced
   updates; audio phrases get a play button.
5. **Resources** — added from disk or ARASAAC; held in the browser as part of
   the working board. Whitelist `.ico .png .jpg .jpeg .wav .mp3`, checked by
   magic bytes (never extension alone; no SVG); **per-file size cap from
   Settings enforced at add time**; dedupe by basename; collision and
   dangling-reference warnings.
6. **ARASAAC pictograms** — search by keyword + locale using the OFFICIAL
   ARASAAC API (https://arasaac.org/developers; never scrape the site),
   called directly from the browser per the Architecture section. Selecting a
   pictogram downloads the PNG into the board's resources with a sane
   basename (subject to the same size caps). Attribution is mandatory and
   non-disableable: show it in the picker UI and auto-append an attribution
   line to the board's `credits` style field (CC BY-NC-SA; owner: Government
   of Aragón; author: Sergio Palao). Feature availability is controlled by
   the license gate below.
7. **Templates** — bundled seeds: "Blank" + the SPC adult example from the
   style reference (with its resources). "Save as template" stores to
   localStorage ("My templates") and/or exports a template file — no server
   storage. Template picker on create.
8. **Import validation** — apply the hardening rules from the format
   reference §6 client-side: exact header; for zips: exactly one root
   `categories.ssc`, flat `resources/` with whitelisted extensions, entry-name
   safety (no `..`, `\`, `:`, absolute paths, control chars, symlinks), the
   format's entry/total/ratio caps AND the stricter admin-configured size
   limits (protects the browser tab and downstream apps). Imported files are
   untrusted: **escape all board-derived text in the DOM** (XSS). Reject with
   readable errors; keep nothing on failure.
9. **Export** — `.ssc` (UTF-8 + BOM, exact header, LF) or `.ssz` (flat
   `resources/`, bare-basename references, only files actually referenced).
   Auto-suggest `.ssz` when resources exist, else `.ssc`; suggested filename
   `YYYYMMDD_<title>_<language>`; block with a readable error when the bundle
   exceeds the configured total size limit.
10. **Languages** (three distinct concerns):
    a. Plugin UI translatable the WordPress way (text domain, `.pot`,
       translator comments). Source strings in **English**; ship **Spanish,
       French, Italian, and German** translations.
    b. Each board is tagged with its content language (with RTL flag) —
       drives preview direction and the export filename.
    c. ARASAAC search locale, defaulting to the board language.
11. **Settings (admin)** — exposure (public / logged-in); ARASAAC enablement
    (license-gated, see below) + default search locale; **file-size limits:
    max size per resource file and max total bundle size** (sane defaults;
    passed to the editor via localized script data; enforced client-side at
    add, import, and export); default template; read-only view of the
    attribution text; fallback-proxy cache TTL (only if the proxy exists).
12. **Validate action** — one click runs the generation checklist from the
    format reference; blocking errors vs. warnings, each linked to its field.

## ARASAAC license gate (asked on install)
On first activation show an onboarding notice/modal: ARASAAC pictograms are
CC BY-NC-SA — link the official terms, state the non-commercial condition, and
require the site owner to explicitly affirm compliant use BEFORE the pictogram
search activates. Record the affirmation (user + timestamp), show it in
Settings, and allow revisiting/revoking it there. Until affirmed, the ARASAAC
panel is visible but disabled with an explanatory note. Attribution handling
(req. 6) applies regardless and can never be turned off.

## Non-functional requirements
- **Security:** the editor is public-facing — no board bytes stored or
  transported server-side; the settings endpoints are capability/nonce
  protected; the fallback proxy (if ever needed) is host-pinned (no SSRF) and
  rate-limited; escape all board-derived output; no SVG anywhere.
- **Privacy:** board content never leaves the browser; the only outbound data
  is pictogram search queries sent directly from the visitor's browser to
  ARASAAC (disclose this in readme.txt).
- **Balanced dependencies:** WP/PHP built-ins server-side; client-side plain
  JS or `@wordpress/element` plus at most the one zip library; no Composer
  packages unless irreplaceable; must run on ordinary shared hosting. List
  every dependency added and why.
- **Compatibility & license:** PHP ≥ 8.0, WP ≥ 6.0, multisite-safe;
  GPL-compatible plugin license (required by WordPress).
- **Accessibility:** the audience is the AAC community — full keyboard
  operation, visible focus, ARIA labels, WCAG AA contrast.
- **Performance:** no blocking remote calls on page load; debounced preview;
  client-cached pictogram search; editor assets loaded only on pages that
  embed it.

## Deliverables
Plugin source with `readme.txt` and uninstall cleanup (settings/transients
only — there is no board data); `.pot` plus `es_ES`, `fr_FR`, `it_IT`, `de_DE`
translations; bundled seed templates; JS unit tests for the parser/serializer/
zip round-trips including unknown-style-property preservation and hostile-
fixture rejection; PHP tests for settings (and the proxy, if it exists); a
short architecture README mapping each module to the section of the
format/style reference it implements.

## Acceptance criteria
- AC1: import the SPC example `.ssz` → change nothing → export → content-
  equivalent `.ssc` (lines, markers, and unknown style pairs preserved).
- AC2: a board built from the SPC template with two ARASAAC icons and one
  `.mp3` exports a `.ssz` the SimonSays desktop app imports successfully
  (manual gate).
- AC3: hostile inputs (zip-slip names, nested resources, oversized/ratio-bomb
  zips, disallowed extensions, reserved markers typed into fields) are
  rejected with readable errors and nothing is retained.
- AC4: the SPC example preview matches the style reference (class colors
  flowing to phrase keys, caption strip, icon-on-top smart layout) and
  mirrors correctly for an RTL board.
- AC5: UI fully translatable; the Spanish, French, Italian, and German
  translations load and display; ARASAAC attribution shown whenever used.
- AC6: statelessness — creating/editing/exporting generates zero server
  requests carrying board data (verifiable in devtools), and the editor works
  logged-out when exposure = public.
- AC7: license gate — ARASAAC stays disabled until affirmed at activation;
  the affirmation (who/when) is visible in Settings.
- AC8: size limits — a resource over the per-file cap, an import over the
  configured limits, and an export over the total cap are each rejected with
  a readable, localized error.

## Out of scope (v1)
Server-side board storage, galleries/sharing, pictogram providers other than
ARASAAC, `.mid/.midi` bundling, per-phrase styling.

## Process
Before writing any code, post a plan: module/file layout, client data model,
confirmation of direct ARASAAC browser access (or evidence a proxy is
required), dependency list, and your open questions — then wait for approval.
Ask instead of assuming whenever the attached references leave a gap.

## Open questions
None — the above decisions are final. Raise new questions in your plan only
where the attached references leave a genuine gap.
