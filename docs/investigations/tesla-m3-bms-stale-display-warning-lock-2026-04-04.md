# Tesla Model 3 BMS Stale-Display Warning Field Lock (2026-04-04)

## Document Metadata

- Title: Append-only stale-display warning field contract lock
- Version: 0.1
- Date: 2026-04-04
- Scope: Docs-only field-contract lock before warning implementation
- Status: Investigation artifact (non-canonical until promoted)

## 1) Purpose and scope

This document locks the append-only contract for a single diagnostic warning
field that helps operators interpret potentially stale `u*` display patterns.

This slice is docs-only and does not implement firmware behavior changes.

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical docs:
   - `AGENTS.md`
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Prior investigations:
   - `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md`
   - `docs/investigations/tesla-m3-bms-cell-group-freshness-lock-2026-04-04.md`
4. Implementation/contract ordering surface:
   - `include/param_prj.h`
   - `src/BatMan.cpp`

## 3) Repo evidence motivating a warning field

- **PROVEN:** Live-cell investigation records a stale-retention presentation
  risk and recommends a compact stale indicator for published `u*` ranges
  (`docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md:178`).
- **PROVEN:** Same investigation advises treating high-index static `u*`
  readings as potentially stale until freshness confirms cadence
  (`docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md:190`).
- **PROVEN:** Freshness instrumentation contract is already locked as
  `CellGrp0Fresh..CellGrp4Fresh`
  (`docs/investigations/tesla-m3-bms-cell-group-freshness-lock-2026-04-04.md:63`,
  `docs/investigations/tesla-m3-bms-cell-group-freshness-lock-2026-04-04.md:67`).
- **PROVEN:** Existing merged parameter ordering places freshness fields after
  `AsDiffTrace`, with `CellGrp4Fresh` immediately before `TempMax`
  (`include/param_prj.h:75`, `include/param_prj.h:80`,
  `include/param_prj.h:81`).
- **INFERRED:** A single append-only warning derived from existing freshness
  indicators is lower-risk than behavior changes to parsing/publication in this
  planning stage.

## 4) Locked append-only field contract

### 4.1 Field name (locked)

- `CellDataStaleWarn`

Name-conflict check result:

- **PROVEN:** no existing symbol/field collision for `CellDataStaleWarn` was
  found in current repository search.

### 4.2 Exact meaning (locked)

- `CellDataStaleWarn` is a diagnostic-only warning flag.
- It indicates that one or more later cell-ingest groups are not fresh, so
  displayed `u*` values may be partially stale or front-loaded toward earlier
  groups.
- Value model is intentionally simple:
  - `0` = no stale-display warning condition detected by the chosen simple rule.
  - `1` = stale-display warning condition detected.
- This field is diagnostic-only and not a balancing, State of Charge (SOC), or
  Controller Area Network (CAN) control input.

### 4.3 Exact placement (locked)

- Insert `CellDataStaleWarn` immediately after `CellGrp4Fresh` in
  `PARAM_LIST`.
- Keep existing ordering intact and append-only.
- Ordering rationale:
  - freshness cluster currently ends at `CellGrp4Fresh`
    (`include/param_prj.h:80`),
  - next entry is currently `TempMax` (`include/param_prj.h:81`),
  - next value identifier marker is `2182` (`include/param_prj.h:50`), matching
    append-only insertion directly after current `CellGrp4Fresh` value `2181`.

## 5) Explicit out-of-scope list for the next code slice

- no balancing logic changes
- no SOC/current-path changes
- no CAN additions
- no renaming/removal of existing fields
- no forced clearing/rewrite of `u*` values in this slice
- no parser redesign

## 6) Contract integrity and complexity guardrail

- Lock this as a single diagnostic warning flag only.
- Do not expand into multi-state warning taxonomy in the same slice.
- If richer warning states are required later, supersede with a new append-only
  decision rather than rewriting this lock.

## Release honesty reminder

This lock does not claim balancing is fixed, SOC is accurate, or full 96-cell
behavior is hardware-validated.
