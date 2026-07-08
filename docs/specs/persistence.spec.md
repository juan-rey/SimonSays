# SimonSays — Settings / Registry Persistence — Specification

| | |
|---|---|
| **Spec ID** | REG-SPEC |
| **Status** | Draft (skeleton — no requirements authored yet) |
| **Version** | 0.1 |
| **REQ prefix** | `REG-F##` (functional), `REG-N##` (non-functional) |
| **Applies to** | SimonSays – Simply Speak (Win32 C++ desktop AAC app) |
| **Master spec** | [`docs/spec.md`](../spec.md) |

---

## 0. How to use this spec

See [`AGENT.md`](../../AGENT.md) and [`docs/spec.md`](../spec.md) §2 for shared
conventions. This file is the **cross-cutting** source of truth for **how**
SimonSays stores settings and default phrase sets in the Windows registry
(HKCU), including first-run defaults and the load/save lifecycle. Reference
format: [`docs/specs/dwell.spec.md`](dwell.spec.md) (note its §9 registry table as
the pattern for documenting stored values).

> Scope reminder: this spec owns the **storage mechanism**. **What** each module
> stores is owned by that module's spec (e.g. dwell values live in
> [`docs/specs/dwell.spec.md`](dwell.spec.md) §9; settings fields in
> [`settings.spec.md`](settings.spec.md); voice/rate in [`tts.spec.md`](tts.spec.md);
> phrases in [`categories-phrases.spec.md`](categories-phrases.spec.md)). Each
> module documents its own values; this spec documents the storage rules they
> all follow. Avoid duplicating per-value tables — link to them.

**Source material to mine (do not invent beyond it):**
- [`README.md`](../../README.md) — "Registry defaults and installation"
  (`HKCU\SOFTWARE\SimonSays\Phrases\<Language>`; defaults created on first run),
  "Updating default phrases on an existing install" (non-overwrite; delete the
  language key to re-install; `InstallDefaultPhrases()`).
- [`HELP.md`](../../HELP.md) — Settings persistence references (remember window
  size; OK saves / Cancel discards).
- [`docs/specs/dwell.spec.md`](dwell.spec.md) §9 — example of HKCU `REG_SZ` values
  with documented defaults (pattern to follow).

---

## 1. Overview

> TODO. Settings and built-in phrase sets persist per-user in the registry under
> `HKCU\SOFTWARE\SimonSays\…`; defaults are created on first run and not
> overwritten thereafter.

## 2. Background & context

> TODO. HKCU only (no admin); existing phrases are preserved on update (per
> README).

## 3. Goals & non-goals

**Goals** — TODO.
**Non-goals** — TODO (per-module value semantics belong to those modules).

## 4. Glossary

| Term | Meaning |
|---|---|
| HKCU | TODO (per-user registry hive) |
| Settings struct | TODO (in-memory settings, persisted as registry values) |
| Default phrase set | TODO (per-language defaults installed on first run) |

## 5. Personas & scenarios

> TODO. e.g. first run creates defaults; re-installing defaults for a language.

## 6. Requirements (EARS)

> TODO — author from actual code/behavior. No requirements invented here.

### 6.1 Storage location & format (functional)

- **REG-F01 [Planned]** TODO (HKCU under the SimonSays key; values as `REG_SZ`
  per the dwell.spec pattern — confirm against code). *(reserve REG-F02 …)*

### 6.2 First-run defaults (functional)

- **REG-F1# [Planned]** TODO (create default phrase sets per language on first
  run; documented defaults for settings).

### 6.3 Non-overwrite of existing data (functional)

- **REG-F2# [Planned]** TODO (existing phrases not overwritten automatically;
  delete language key to re-install; `InstallDefaultPhrases()`).

### 6.4 Load / save lifecycle (functional)

- **REG-F3# [Planned]** TODO (load on startup; save on Settings OK / shutdown;
  benign fallback on malformed values).

### 6.5 Non-functional

- **REG-N01 [Planned]** TODO (HKCU only; no admin required).

## 7. Architecture & components

> TODO (e.g. `RegistryManager` per dwell.spec references — confirm against code).

## 8. Detailed design

> TODO. Key layout under `HKCU\SOFTWARE\SimonSays` (settings key + `Phrases\<Language>`).

## 9. Data model & persistence

> TODO. This is the central place describing storage rules. Index the per-module
> value tables rather than copying them:
> - Dwell values → [`docs/specs/dwell.spec.md`](dwell.spec.md) §9.3
> - Settings fields → [`settings.spec.md`](settings.spec.md)
> - Voice/volume/rate → [`tts.spec.md`](tts.spec.md)
> - Phrases → [`categories-phrases.spec.md`](categories-phrases.spec.md)
> - Board/category style tokens (`$$board` value, `$$` phrase-position tokens) → [`board-style.spec.md`](board-style.spec.md) §8.3/§9

## 10. Key interfaces

> TODO (e.g. load/save entry points; `InstallDefaultPhrases()`).

## 11. UI specification

> TODO / N/A (no dedicated UI; triggered by Settings OK / app lifecycle).

## 12. Configuration & tuning constants

> TODO (e.g. root key path `HKCU\SOFTWARE\SimonSays`).

## 13. Diagnostics

> TODO / N/A.

## 14. Edge cases & error handling

> TODO. e.g. malformed registry values → benign numeric fallback (per dwell.spec
> §14); missing language key → defaults recreated.

## 15. Acceptance criteria

> TODO.
- AC-1 (REG-F0?) [Pending] TODO (settings round-trip; HKCU only; first-run
  defaults).

## 16. Implementation status matrix

| Area | Status | Notes |
|---|---|---|
| HKCU settings storage | ⛔ TODO | |
| First-run default phrase install | ⛔ TODO | |
| Non-overwrite of existing phrases | ⛔ TODO | |
| Load/save lifecycle | ⛔ TODO | |

## 17. Known limitations

> TODO.

## 18. Future work

> TODO.

## 19. Open questions

> TODO. e.g. exact settings key path/name (README documents only the `Phrases`
> subkey); whether export/import touches the registry directly (relates to
> [`import-export.spec.md`](import-export.spec.md)).

## 20. Build & run

See [`docs/spec.md`](../spec.md) §2.7 / [`AGENT.md`](../../AGENT.md) §5.

---

*End of REG-SPEC (skeleton).*
