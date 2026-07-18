# SimonSays Board Editor — architecture

Stateless WordPress plugin: a front-end, browser-only editor for SimonSays
communication boards. The **normative contracts** are the two format reference
guides owned by the desktop repository — every format decision here traces to
them. This plugin implements the format **as of SimonSays desktop 0.7
(development, 2026-07-17)**.

- Vendored, pinned copies: [`../docs/format/ssc-ssz-format-reference.md`](../docs/format/ssc-ssz-format-reference.md) (PORT contract) · [`../docs/format/board-style-reference.md`](../docs/format/board-style-reference.md) (STY contract)
- Canonical (desktop repo): [ssc-ssz-format-reference.md](https://github.com/juan-rey/SimonSays/blob/main/docs/guides/ssc-ssz-format-reference.md) · [board-style-reference.md](https://github.com/juan-rey/SimonSays/blob/main/docs/guides/board-style-reference.md)

## Module map (module → contract section)

| Module | Implements |
|---|---|
| `assets/js/ssc.js` | `.ssc` byte layout & line grammar (format ref §2–3); reserved-marker validation (§10). Note: `♫` is **allowed in phrase text** — it is the documented inline sound-marker syntax — and rejected in category names. |
| `assets/js/style.js` | Style-list syntax & value forms (style ref §2–3), scope routing (§5), cascade merge (§4). `upsertPair`/`removePairs` edit the raw string segment-wise so **unknown pairs round-trip verbatim** (STY-F33). |
| `assets/js/zip.js` | Zip container (format ref §4). Writer emits STORED entries (resources are pre-compressed formats); reader extracts STORED + DEFLATE via native `DecompressionStream('deflate-raw')` with CRC-32 verification. Zero dependencies. |
| `assets/js/ssz.js` | Bundle structure + hardening (format ref §4–6): single root `categories.ssc`, flat whitelisted `resources/`, entry-name safety, symlink rejection, entry/total/ratio caps (admin caps clamp *below* the format caps), content sniffing. |
| `assets/js/sniff.js` | Magic-byte whitelist (`.ico .png .jpg .jpeg .wav .mp3`; never SVG, never extension trust). |
| `assets/js/validate.js` | The §10 generation checklist as a findings engine (errors vs warnings). |
| `assets/js/preview.js` | Cascade-resolved approximation of the category window (STY-F10/F12/F40-45): grids, caption strip, derived hover shades, RTL mirroring. Board text only ever enters the DOM as text nodes (XSS-safe). |
| `assets/js/arasaac.js` | Direct browser client for the official ARASAAC API (CORS verified: `Access-Control-Allow-Origin: *` on both hosts). Session-cached; mandatory attribution constant. |
| `assets/js/ui.js` + `app.js` | Panels, schema-driven style forms, toolbar, IndexedDB autosave/recovery, open/export, size-cap enforcement at add/import/export. |
| `includes/class-ssbe-embed.php` | Block + shortcode, conditional enqueue, `SSBE_DATA` (settings/strings/templates/languages). No board-related endpoints exist for visitors. |
| `includes/class-ssbe-settings.php` | Admin settings: exposure, ARASAAC, per-file/total size caps, default template; hosts the template manager section. |
| `includes/class-ssbe-templates.php` | Admin-managed templates (upload/rename/delete, per-template language). Files in `uploads/simonsays-board-templates/`; uploads validated server-side (.ssc header; .ssz structure via `ZipArchive` mirroring the client rules). Admin-only surface — visitor statelessness unchanged. |
| `includes/class-ssbe-activation.php` | ARASAAC license gate: activation affirmation (who/when), shown & revocable in Settings. |

## Dependencies

None beyond WordPress/PHP built-ins and browser platform APIs. The one place
the plan allowed a library (client-side zip) is covered by `zip.js` +
`DecompressionStream('deflate-raw')` instead — supported by all evergreen
browsers (the editor shows an import error on legacy browsers without it).

## Tests

```
node --test tests/js/test-formats.test.mjs tests/js/test-ssz.test.mjs
```

Round-trips (including the SPC example byte-for-byte and unknown-style-pair
preservation) plus hostile fixtures (zip-slip names, nested resources,
ratio bombs, symlinks, mislabeled content, cap violations).

## i18n workflow

All user-visible strings live in PHP (reaching JS via `wp_localize_script`),
so gettext is the single pipeline:

```
node tools/make-pot.mjs   # regenerate languages/simonsays-board-editor.pot
# edit/extend languages/simonsays-board-editor-<locale>.po
node tools/po2mo.mjs      # compile every .po to .mo
```

Shipped locales: `es_ES`, `fr_FR`, `it_IT`, `de_DE` (source strings in English).

## Deviations from the build prompt (documented on purpose)

- `♫` is not rejected in phrase text (see module map — it is the inline-sound
  feature of the desktop app; a blanket ban would break legitimate boards).
- Import additionally sniffs resource content against its extension (stricter
  than the desktop app): a web editor should not carry mislabeled bytes forward.
- Autosave uses IndexedDB rather than localStorage (binary resources don't fit
  localStorage quotas); it remains strictly client-side.
- Preview phrase clicks speak the raw text via the Web Speech API (or play the
  phrase's sound file); inline `♫file♫` markers inside text are spoken as-is
  rather than segmented like the desktop playback engine.
- Downloaded pictograms are named after the search text (sanitized, uniqued);
  resource renaming keeps the extension locked and rewrites every board
  reference (`renameResourceReferences`, unit-tested).
