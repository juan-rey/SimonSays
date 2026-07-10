# AGENT.md — Working instructions for AI agents in the SimonSays repository

> Tool-agnostic guide. It applies to **any** LLM / IDE coding agent, not a
> specific vendor. If you are a human contributor, this also tells you how the
> spec-driven workflow in this repo is meant to work.

SimonSays is a Win32 C++ desktop AAC (Augmentative and Alternative Communication)
text-to-speech utility that lives in the Windows taskbar/tray, built for people
with ALS and other speech impairments. See [`README.md`](README.md) and
[`HELP.md`](HELP.md) for the product description and user-facing behavior.

---

## Contents
- [1. Anchored Spec-Driven Development (Anchored SDD) in this repo](#1-anchored-spec-driven-development-anchored-sdd-in-this-repo)
- [2. How to work (read → change → update)](#2-how-to-work-read--change--update)
- [3. Working agreement (project-wide)](#3-working-agreement-project-wide)
- [4. Spec vs. code conflict resolution](#4-spec-vs-code-conflict-resolution)
- [5. Build & run](#5-build--run)
- [6. Git / committing](#6-git--committing)
- [7. ChangeLog & TODO maintenance](#7-changelog--todo-maintenance)
- [8. Quick checklist for an agent making a change](#8-quick-checklist-for-an-agent-making-a-change)

---

## 1. Anchored Spec-Driven Development (Anchored SDD) in this repo

This project is developed **spec-first**, in an *anchored* form: stable `REQ-*`
requirement IDs are the anchors every change traces to, and **no code or spec
edit happens before a validation plan has been posted and approved** (see §2
step 2 and Working agreement rule 1). Behavior is described in prescriptive
specifications before and alongside the code. The specs are the source of truth
for *intended* behavior; the code is the source of truth for *actual* behavior.
When the two disagree, see §4.

### Where the specs live

- **Entry point / master index:** [`docs/spec.md`](docs/spec.md)
  Product overview, shared conventions, and the table of all module sub-specs.
- **Per-module sub-specs:** [`docs/specs/`](docs/specs/)
  One `*.spec.md` per module (e.g. `docs/specs/tts.spec.md`).
- **Reference example of a mature sub-spec:** [`docs/specs/dwell.spec.md`](docs/specs/dwell.spec.md)
  The Gaze / Dwell-click spec. Use it as the canonical template for format,
  EARS phrasing, requirement IDs, status tags, acceptance criteria, and the
  status matrix. **Do not rewrite it** as part of unrelated work.

> Note: every module spec — including the Dwell reference — lives under
> [`docs/specs/`](docs/specs/). The master index in [`docs/spec.md`](docs/spec.md)
> links to all of them.

---

## 2. How to work (read → change → update)

1. **Read before coding.** Before changing behavior in a module, open that
   module's sub-spec (find it via the index in [`docs/spec.md`](docs/spec.md)) and
   read the relevant `REQ-*` requirements, acceptance criteria, and status matrix.
   If no sub-spec exists yet for the area, read [`docs/spec.md`](docs/spec.md) and
   the closest existing sub-spec, and treat `README.md` / `HELP.md` as the
   behavioral reference.
2. **Post a validation plan and STOP for approval, before changing anything.**
   The plan goes at the read → change boundary: *after* reading the spec and the
   relevant code, *before* the first edit to any file (code, spec, or docs).
   It must be an explicit, checkable block in this shape:

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
3. **Re-anchor when scope grows.** If new work appears mid-task (a bug found
   during review, a needed refactor, a doc error), add it to the plan with a
   fix-now / report-only proposal and get it approved before editing for it —
   don't silently fold it into the change.
4. **Treat requirements as a contract.** Implement against the EARS requirements
   and verify against the **Acceptance criteria** section before claiming a task
   is complete.
5. **Update the spec in the same change.** When you change behavior, update the
   matching `REQ-*` requirement(s) **and** the module's status matrix in the same
   commit/PR. A behavior change with no spec update is incomplete. If the change
   qualifies as significant (see §7), also update `ChangeLog` and/or `TODO` in
   the same change.
6. **Keep IDs stable.** Don't renumber existing `REQ-*` IDs; they are traceability
   anchors. Add new ones at the end of the relevant range (see §3 of
   [`docs/spec.md`](docs/spec.md) for the numbering scheme).

---

## 3. Working agreement (project-wide)

These rules generalize the working agreement established in
[`docs/specs/dwell.spec.md`](docs/specs/dwell.spec.md) §0 so they apply to
**every** module:

0. **Assume nothing; ask if unsure.** Do not presuppose any requirement, file
   path, registry key, format, or behavior that isn't established by a spec or the
   code. If something needed to proceed is unspecified or ambiguous, **ask** rather
   than guess — a wrong assumption committed to a spec is worse than a question.
1. **Post a validation plan and wait for approval before any changes.** Before
   touching code or specs — and *after* reading them, at the read → change
   boundary — post the validation-plan block from §2 step 2 (the specific
   `REQ-*` requirement(s), **Acceptance criteria**, verification method, and
   any out-of-scope findings with a fix/report proposal). Then **stop and wait
   for the developer's OK**; do not proceed unprompted. Re-anchor and re-ask
   when scope grows mid-task (§2 step 3).
2. **`REQ-*` is the contract.** When you change behavior, update the matching
   requirement and the module's status matrix in the same change.
3. **Don't regress to unblock.** Do **not** regress a **[Done]** requirement to
   make a **[Planned]** one work.
4. **Verify before claiming done.** Check your change against the module's
   **Acceptance criteria** before declaring completion.
5. **Honor layering / dependency rules.** Respect the architecture and
   dependency constraints stated in each module's spec (and the cross-module
   ones in [`docs/spec.md`](docs/spec.md)). Don't introduce dependencies a spec
   forbids.
6. **Match existing code style.** Follow the brace/indent/naming conventions of
   the surrounding code, and the project's localization, registry, and dialog
   conventions. Identifiers and comments in English; user-visible strings
   localized.
7. **Code wins on conflict, then fix the spec.** If a fact in a spec conflicts
   with the actual code, the code is authoritative for *current* behavior — fix
   the spec and flag the discrepancy. Confirm device-/environment-specific facts
   against real hardware/OS where applicable.
8. **Flag incomplete or outdated sub-specs.** If, while working, you find a
   sub-spec that is missing sections, still a skeleton, or visibly stale
   against the code, **tell the developer and ask them to update it** (or for
   the go-ahead to update it yourself). Small fixes — typos, broken links, a
   stale line item, the concrete conflict-fix of rule 7 — don't need approval;
   just make them and mention them. What needs the developer is **substantial**
   spec authoring or restructuring nobody asked for. A stale spec is never
   license to ignore it.

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

- **[Done]** — implemented and verified.
- **[Done*]** — implemented; correctness depends on an environment-specific
  assumption validated in a limited setting (note it).
- **[Partial]** — implemented in a hardcoded/simplified form of the full intent.
- **[Planned]** — specified but not implemented.

---

## 4. Spec vs. code conflict resolution

- The **code** is authoritative for what the app *does today*.
- The **spec** is authoritative for what the app *should do*.
- If you find the code doing something the spec doesn't describe (or contradicts),
  do **not** silently "fix" either side. Decide whether it's a code bug or a stale
  spec, make the change, and update the spec to match — noting the resolution.

---

## 5. Build & run

Win32 C++ / MSBuild / Visual Studio toolchain (see `README.md` and
[`docs/specs/dwell.spec.md`](docs/specs/dwell.spec.md) §20):

```
MSBuild SimonSays.vcxproj /p:Configuration={Debug|Release} /p:Platform=x64
```

- Verify features against the built **`x64\Release\SimonSays.exe`** (not the
  installed Program Files copy).
- New source/header files must be registered in `SimonSays.vcxproj` and
  `SimonSays.vcxproj.filters`.

> Build gate (per dwell.spec.md): Debug **and** Release x64 should compile clean,
> apart from pre-existing warnings noted in the specs.

---

## 6. Git / committing

**Never commit or push.** The developer reviews all diffs before they go into
history. After completing any set of file changes, write a short plain-English
description of what was changed (files touched and why) so the developer can
stage and commit on their own.

---

## 7. ChangeLog & TODO maintenance

The repo keeps two plain-text planning/history files at the root, and agents are
expected to maintain them **in the same change** as the work itself:

- [`ChangeLog`](ChangeLog) — user-facing release history.
- [`TODO`](TODO) — future plans and deferred work.

### When a change MUST get a ChangeLog entry

Add an entry when the change is **significant**, meaning any of:

- a **new feature** (new capability, setting, UI element, file format, etc.);
- a **user-visible fix** (crash, wrong behavior, UI/localization defect);
- an **architecture or behavior change** (threading model, playback pipeline,
  registry layout, dependency changes, installer/deployment changes);
- a **help-content enhancement** (`HELP.md` and/or `docs/help/*` — remember the
  sync script workflow for `HELP_CONTENT_ID`).

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

### Process

Declare the planned ChangeLog entry / TODO adjustment in the validation plan
(the `ChangeLog/TODO:` line in §2 step 2) so it is approved together with the
rest of the change. `n/a` is a valid value — but state it explicitly.

---

## 8. Quick checklist for an agent making a change

- [ ] Found and read the relevant module sub-spec (via [`docs/spec.md`](docs/spec.md)).
- [ ] Resolved any unknowns by asking — assumed nothing unstated (rule 0).
- [ ] Posted the validation-plan block (`REQ-*` + Acceptance criteria + verification + out-of-scope findings) at the read → change boundary and **got the developer's OK** before the first edit (rule 1).
- [ ] Re-anchored (plan updated + re-approved) for any scope that grew mid-task (§2 step 3).
- [ ] Implemented against the `REQ-*` requirements.
- [ ] Verified against the module's Acceptance criteria.
- [ ] Updated the changed `REQ-*` and the status matrix in the same change.
- [ ] Kept the file's **Table of contents** in sync if any top-level section was added, removed, or renamed ([`docs/spec.md`](docs/spec.md) §2.8).
- [ ] Added/updated the `ChangeLog` entry and adjusted `TODO` if the change qualifies (§7).
- [ ] Weighed the change against the engineering guidelines (§3) and declared any significant impact in the plan's `Impact:` line.
- [ ] Flagged (or minimally fixed) any incomplete/outdated sub-spec found along the way (rule 8).
- [ ] Respected layering/dependency rules and existing code style.
- [ ] Registered any new files in `SimonSays.vcxproj(.filters)`.
- [ ] Built Debug + Release x64 clean.
