# AGENT.md — Working instructions for AI agents in the SimonSays repository

> Tool-agnostic guide. It applies to **any** LLM / IDE coding agent, not a
> specific vendor. If you are a human contributor, this also tells you how the
> spec-driven workflow in this repo is meant to work.

SimonSays is a Win32 C++ desktop AAC (Augmentative and Alternative Communication)
text-to-speech utility that lives in the Windows taskbar/tray, built for people
with ALS and other speech impairments. See [`README.md`](README.md) and
[`HELP.md`](HELP.md) for the product description and user-facing behavior.

---

## 1. Spec-Driven Development (SDD) in this repo

This project is developed **spec-first**. Behavior is described in prescriptive
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
2. **Summarize how you'll validate, before changing anything.** State the
   `REQ-*` and **Acceptance criteria** the change must satisfy and how you'll
   verify them — then make the change. If any of that is unclear, stop and ask
   (see Working agreement rules 0–1).
3. **Treat requirements as a contract.** Implement against the EARS requirements
   and verify against the **Acceptance criteria** section before claiming a task
   is complete.
4. **Update the spec in the same change.** When you change behavior, update the
   matching `REQ-*` requirement(s) **and** the module's status matrix in the same
   commit/PR. A behavior change with no spec update is incomplete.
5. **Keep IDs stable.** Don't renumber existing `REQ-*` IDs; they are traceability
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
1. **Summarize validation before any changes.** Before touching code or specs,
   state up front what the change will be validated against — the specific
   `REQ-*` requirement(s) and **Acceptance criteria** it must satisfy, and how
   you'll verify it — then proceed.
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

## 6. Quick checklist for an agent making a change

- [ ] Found and read the relevant module sub-spec (via [`docs/spec.md`](docs/spec.md)).
- [ ] Resolved any unknowns by asking — assumed nothing unstated (rule 0).
- [ ] Summarized up front what to validate against (`REQ-*` + Acceptance criteria) before changing anything (rule 1).
- [ ] Implemented against the `REQ-*` requirements.
- [ ] Verified against the module's Acceptance criteria.
- [ ] Updated the changed `REQ-*` and the status matrix in the same change.
- [ ] Respected layering/dependency rules and existing code style.
- [ ] Registered any new files in `SimonSays.vcxproj(.filters)`.
- [ ] Built Debug + Release x64 clean.
