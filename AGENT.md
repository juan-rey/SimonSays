# AGENT.md — Working instructions for AI agents in the SimonSays repository

> Tool-agnostic guide. It applies to **any** LLM / IDE coding agent, not a
> specific vendor. If you are a human contributor, this also tells you how the
> spec-driven workflow in this repo is meant to work.

SimonSays is a Win32 C++ desktop AAC (Augmentative and Alternative Communication)
text-to-speech utility that lives in the Windows taskbar/tray, built for people
with ALS and other speech impairments. See [`README.md`](README.md) and
[`HELP.md`](HELP.md) for the product description and user-facing behavior.

> **Three golden rules**
> 1. Post a validation plan and **wait for the developer's OK** before your
>    first edit (§2 step 2).
> 2. Code and spec move **in the same change** — a behavior change without its
>    spec update is incomplete (§2 step 5).
> 3. **Never commit or push** — describe your changes so the developer can
>    review and commit (§6).

---

## Contents
- [1. Anchored Spec-Driven Development (Anchored SDD) in this repo](#1-anchored-spec-driven-development-anchored-sdd-in-this-repo)
- [2. How to work (read → plan → change → update)](#2-how-to-work-read--plan--change--update)
- [3. Working agreement (project-wide)](#3-working-agreement-project-wide)
- [4. Spec vs. code conflict resolution](#4-spec-vs-code-conflict-resolution)
- [5. Build & run](#5-build--run)
- [6. Git / committing](#6-git--committing)
- [7. ChangeLog, TODO & help-content maintenance](#7-changelog-todo--help-content-maintenance)
- [8. Quick checklist for an agent making a change](#8-quick-checklist-for-an-agent-making-a-change)

---

## 1. Anchored Spec-Driven Development (Anchored SDD) in this repo

This project is developed **spec-first**, in an *anchored* form: behavior is
described in prescriptive specifications before and alongside the code, every
change traces to stable requirement IDs, and no edit happens before a
validation plan has been approved (§2 step 2). The specs are the source of
truth for *intended* behavior; the code is the source of truth for *actual*
behavior. When the two disagree, see §4.

**Requirement IDs.** Each module uses a short prefix — `TTS-F##`, `BTN-N##`,
`REG-F##`, … (`F` functional, `N` non-functional); the Dwell module is
grandfathered with bare `REQ-F*`/`REQ-N*` IDs. This guide writes `REQ-*` as
shorthand for **all** of them. The full scheme lives in
[`docs/spec.md`](docs/spec.md) §2.2.

### Where the specs live

- **Entry point / master index:** [`docs/spec.md`](docs/spec.md) — product
  overview, shared conventions (ID scheme, status tags, section structure,
  TOC rule), and the table of all module sub-specs.
- **Per-module sub-specs:** [`docs/specs/`](docs/specs/) — one `*.spec.md`
  per module (e.g. `docs/specs/tts.spec.md`), all linked from the master index.
- **Reference template:** [`docs/specs/dwell.spec.md`](docs/specs/dwell.spec.md)
  — the canonical example for format, EARS phrasing, requirement IDs, status
  tags, acceptance criteria, and the status matrix. **Do not rewrite it** as
  part of unrelated work.

---

## 2. How to work (read → plan → change → update)

The canonical workflow. The numbered rules in §3 are the invariants behind it.

1. **Read before coding.** Open the module's sub-spec (via the index in
   [`docs/spec.md`](docs/spec.md)) and read the relevant `REQ-*` requirements,
   acceptance criteria, and status matrix — plus the code you are about to
   change. If no sub-spec exists for the area, read
   [`docs/spec.md`](docs/spec.md) and the closest existing sub-spec, and treat
   `README.md` / `HELP.md` as the behavioral reference.

2. **Post a validation plan and STOP for approval.** The plan goes at the
   read → change boundary: *after* reading the spec and the relevant code,
   *before* the first edit to any file (code, spec, or docs). It must be an
   explicit, checkable block in this shape:

   ```
   Validation plan
   - Requirements: REQ-Fxx (amend: <what>), REQ-Fyy (new: <one-line EARS intent>)
   - Acceptance: AC-n (amend/new), expected Pass/Pending state
   - Verify: MSBuild Debug+Release x64 clean; <manual steps left Pending, if any>
   - Impact: <significant effects on performance / responsiveness / safety / reliability / memory / binary size, or "none"> (see §3 Engineering guidelines)
   - ChangeLog/TODO: <planned ChangeLog entry summary and/or TODO adjustment, or n/a> (see §7)
   - Out of scope found while reading: <each finding, with a fix-now / report-only proposal>
   ```

   Then **wait for the developer's OK** — do not proceed on your own. A
   one-line paraphrase of the task is not a plan; if any requirement, AC, or
   verification step can't be stated concretely yet, that is a question to ask
   (rule 0), not a reason to skip the plan.

   **Only exception — small fixes:** typos, broken links, a stale line item,
   or the concrete spec-side fix of a code/spec conflict (§4). These need no
   plan line and no prior approval; just make them and mention them in your
   summary.

3. **Re-anchor when scope grows.** If new work appears mid-task (a bug found
   during review, a needed refactor, a doc error), add it to the plan with a
   fix-now / report-only proposal and get it approved before editing for it —
   don't silently fold it into the change.

4. **Implement and verify against the contract.** Implement against the EARS
   requirements, then check the result against the module's **Acceptance
   criteria** and the build gate (§5) before claiming the task complete.

5. **Update the spec — and everything else — in the same change.** A behavior
   change updates the matching `REQ-*` requirement(s) and the module's status
   matrix in the same commit/PR; a behavior change with no spec update is
   incomplete. In the same change also: keep each edited spec's **Table of
   contents** in sync ([`docs/spec.md`](docs/spec.md) §2.8), update
   `ChangeLog`/`TODO` when the change qualifies (§7), and run the help sync
   script when help content changed (§7).

6. **Keep IDs stable.** Don't renumber existing `REQ-*` IDs; they are
   traceability anchors. Add new ones at the end of the relevant range
   ([`docs/spec.md`](docs/spec.md) §2.2).

7. **Hand off.** Describe what changed (files touched and why) for the
   developer's review — and never commit or push (§6).

---

## 3. Working agreement (project-wide)

The invariants behind §2, generalized from the working agreement first
established in [`docs/specs/dwell.spec.md`](docs/specs/dwell.spec.md) §0.
Other documents cite these rules **by number** (e.g. "rule 7"), so the
numbering is stable — never renumber. Rules whose process detail lives in §2
are kept here as one-liners so this list stays the complete index.

0. **Assume nothing; ask if unsure.** Do not presuppose any requirement, file
   path, registry key, format, or behavior that isn't established by a spec or
   the code. If something needed to proceed is unspecified or ambiguous,
   **ask** rather than guess — a wrong assumption committed to a spec is worse
   than a question.
1. **Validation plan before any change.** Post the plan and wait for the
   developer's OK at the read → change boundary; re-anchor when scope grows
   (§2 steps 2–3). Small fixes are the only exception (§2 step 2).
2. **`REQ-*` is the contract.** A behavior change updates the matching
   requirement(s) and the module's status matrix in the same change (§2 step 5).
3. **Don't regress to unblock.** Do **not** regress a **[Done]** requirement
   to make a **[Planned]** one work.
4. **Verify before claiming done.** Check the change against the module's
   **Acceptance criteria** and the build gate (§2 step 4, §5).
5. **Honor layering / dependency rules.** Respect the architecture and
   dependency constraints stated in each module's spec (and the cross-module
   ones in [`docs/spec.md`](docs/spec.md)). Don't introduce dependencies a
   spec forbids.
6. **Match existing code style.** Follow the brace/indent/naming conventions
   of the surrounding code, and the project's localization, registry, and
   dialog conventions; user-visible strings are localized, never hardcoded.
7. **Code wins on conflict, then fix the spec** — resolution procedure in §4.
8. **Update incomplete or outdated sub-specs — by default.** A sub-spec that
   is missing sections, still a skeleton, or visibly stale against the code is
   updated as part of the change: declare it in the validation plan (or
   re-anchor, §2 step 3), bring it in line with the code (§4), and summarize
   what was brought up to date. Small fixes skip the plan (§2 step 2). A stale
   spec is never license to ignore it.
9. **English everywhere (except translations).** Every change to code or
   documentation — identifiers, comments, specs, `ChangeLog`, `TODO`, help
   content, and the change descriptions written for the developer — is written
   in **English**, regardless of the language of the conversation with the
   developer. The only exception is translated content, written in its target
   language: the localized help files (`docs/help/HELP_<lang>.md`), the
   localized string tables, and the per-language default phrase sets.

### Engineering guidelines (every change)

Every change — new feature, fix, or refactor — is designed and judged against
these qualities:

- **Performance** and **fluent UI responsiveness** — never block or stutter
  the UI; this is an AAC tool someone depends on to speak.
- **Safety** and **reliability** — no crashes, no data loss, degrade
  gracefully on bad input.
- **Memory footprint** — keep the always-running tray app light.
- **Code reuse & consolidation** — prefer extending an existing helper over
  duplicating logic.
- **Binary size** — avoid new dependencies; keep the single-exe deployment
  lean.

When designing a new feature, choose the approach that best satisfies these
guidelines, and **warn the developer in the validation plan whenever a change
may significantly impact any of them** (e.g. a new thread, a new library, a
busy polling loop, large caches, heavier startup) — that's what the `Impact:`
line in the §2 step 2 template is for.

### Status tags (used in every sub-spec)

Defined in [`docs/spec.md`](docs/spec.md) §2.3 and mirrored here for
convenience — if they ever change, update both places.

- **[Done]** — implemented and verified.
- **[Done*]** — implemented; correctness depends on an environment-specific
  assumption validated in a limited setting (note it).
- **[Partial]** — implemented in a hardcoded/simplified form of the full intent.
- **[Planned]** — specified but not implemented.

---

## 4. Spec vs. code conflict resolution

- The **code** is authoritative for what the app *does today*; the **spec** is
  authoritative for what the app *should do*.
- If you find the code doing something the spec doesn't describe (or
  contradicts), do **not** silently "fix" either side. Decide whether it's a
  code bug or a stale spec, make the corresponding change, and update the spec
  to match — noting the resolution.
- Confirm device-/environment-specific facts against real hardware/OS where
  applicable before recording them in a spec.

---

## 5. Build & run

Win32 C++ / MSBuild / Visual Studio toolchain (see `README.md`):

```
MSBuild SimonSays.vcxproj /p:Configuration={Debug|Release} /p:Platform=x64
```

- Verify features against the built **`x64\Release\SimonSays.exe`** (not the
  installed Program Files copy).
- New source/header files must be registered in `SimonSays.vcxproj` and
  `SimonSays.vcxproj.filters`.

> **Build gate:** Debug **and** Release x64 compile clean, apart from
> pre-existing warnings noted in the specs.

---

## 6. Git / committing

**Never commit or push.** The developer reviews all diffs before they go into
history. After completing any set of file changes, write a short plain-English
description of what was changed (files touched and why) so the developer can
stage and commit on their own.

---

## 7. ChangeLog, TODO & help-content maintenance

The repo keeps two plain-text planning/history files at the root, maintained
**in the same change** as the work itself (§2 step 5):

- [`ChangeLog`](ChangeLog) — user-facing release history.
- [`TODO`](TODO) — future plans and deferred work.

### When a change MUST get a ChangeLog entry

Add an entry when the change is **significant**, meaning any of:

- a **new feature** (new capability, setting, UI element, file format, etc.);
- a **user-visible fix** (crash, wrong behavior, UI/localization defect);
- an **architecture or behavior change** (threading model, playback pipeline,
  registry layout, dependency changes, installer/deployment changes);
- a **help-content enhancement** (`HELP.md` and/or `docs/help/*` — see the
  help-content workflow below).

What does **not** get an entry: trivial refactors, typo fixes, comment or
formatting-only edits, and internal cleanups with no user-visible or
architectural effect.

### How to write the entry

- Put it under the **topmost unreleased version heading** (e.g.
  `2026-07-31 Version 0.7`). If no unreleased section exists, **ask** the
  developer for the target version rather than inventing one (rule 0).
- Follow the existing format: bullets grouped under `New features:`,
  `Fixes and improvements:`, and `Notes:` as applicable; past tense;
  user-facing wording (describe what the user gets, not the diff).
- Name key classes/settings in backticks the way existing entries do
  (e.g. `` `SSButton` ``, `` `Stop previous playback` ``).

### When to touch TODO

- **Future plans go to `TODO`, never into the ChangeLog.** Deferred work, ideas
  found mid-task, and "later" items agreed with the developer are added under
  `Before 1.0:` or `Possible Long Term Goals:` as appropriate.
- When a change **implements** a TODO item, remove that item (or trim it down
  to the remaining part) in the same change.

### Help-content workflow (`HELP.md` / `docs/help/*`)

The in-app help (`F1`) is compiled in: `HELP_CONTENT_ID` in
[`include/localized_strings.h`](include/localized_strings.h) is **generated**
from [`HELP.md`](HELP.md) (English) and the localized
`docs/help/HELP_<lang>.md` files.

1. Edit the Markdown source(s) — **never** edit `HELP_CONTENT_ID` by hand.
2. Run `scripts/sync_help_content.ps1` to regenerate `localized_strings.h`.
3. Rebuild — the regenerated header is a code change, so the §5 build gate
   applies.

### Process

Declare the planned ChangeLog entry / TODO adjustment in the validation plan
(the `ChangeLog/TODO:` line in §2 step 2) so it is approved together with the
rest of the change. `n/a` is a valid value — but state it explicitly.

---

## 8. Quick checklist for an agent making a change

In workflow order (§2):

- [ ] Read the relevant module sub-spec (via [`docs/spec.md`](docs/spec.md)) and the code (step 1).
- [ ] Resolved any unknowns by asking — assumed nothing unstated (rule 0).
- [ ] Posted the validation-plan block and **got the developer's OK** before the first edit (step 2) — or the change qualifies as a small fix (step 2 exception).
- [ ] Re-anchored (plan updated + re-approved) for any scope that grew mid-task (step 3).
- [ ] Implemented against the `REQ-*` requirements, honoring layering rules and existing code style (rules 5–6).
- [ ] Wrote everything (code, specs, docs, ChangeLog/TODO) in English — translations excepted (rule 9).
- [ ] Verified against the module's Acceptance criteria (step 4).
- [ ] Built Debug + Release x64 clean (§5) and registered any new files in `SimonSays.vcxproj(.filters)`.
- [ ] Updated the changed `REQ-*` and the status matrix in the same change (step 5), plus any stale sub-spec found along the way (rule 8).
- [ ] Kept each edited spec's **Table of contents** in sync ([`docs/spec.md`](docs/spec.md) §2.8).
- [ ] Added/updated `ChangeLog` / `TODO` if the change qualifies, and ran `scripts/sync_help_content.ps1` if help content changed (§7).
- [ ] Declared any significant engineering impact in the plan's `Impact:` line (§3 guidelines).
- [ ] Wrote a short description of the changes for the developer — no commit, no push (§6).
