# Tesla Model 3 BMS Cell-Group Range/Interpretation Field Lock (2026-04-04)

## Document Metadata

- Title: Append-only cell-group range interpretation field contract lock
- Version: 0.1
- Date: 2026-04-04
- Scope: Docs-only lock before interpretation-field implementation
- Status: Investigation artifact (non-canonical until promoted)

## 1) Purpose and scope

This document locks the append-only diagnostic contract for group-to-published
cell index interpretation fields, so web user interface (UI) freshness signals
can be interpreted without changing existing behavior contracts.

This slice is docs-only. It does not implement firmware behavior changes.

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical docs:
   - `AGENTS.md`
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Prior investigations/locks:
   - `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md`
   - `docs/investigations/tesla-m3-bms-cell-group-freshness-lock-2026-04-04.md`
   - `docs/investigations/tesla-m3-bms-stale-display-warning-lock-2026-04-04.md`
4. Repo implementation and ordering surface:
   - `src/BatMan.cpp`
   - `include/param_prj.h`

## 3) Repo evidence motivating group interpretation fields

- **PROVEN:** prior live-cell investigation calls for additional operator-facing
  interpretation for stale/fresh status in published `u*` values
  (`docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md:178`).
- **PROVEN:** group handling already exists as explicit ingest groups
  (`0x47..0x4B`) with slot bands (`g=0..2`, `3..5`, `6..8`, `9..11`,
  `12..14`) (`src/BatMan.cpp:403`, `src/BatMan.cpp:422`, `src/BatMan.cpp:441`,
  `src/BatMan.cpp:461`, `src/BatMan.cpp:480`).
- **PROVEN:** publication path uses `Param::u1 + h`, where `h` advances only on
  valid measurements, making interpretation of group contribution useful to
  operators (`src/BatMan.cpp:738`, `src/BatMan.cpp:750`, `src/BatMan.cpp:762`).
- **PROVEN:** existing append-only ordering already contains
  `CellGrp0Fresh..CellGrp4Fresh` and locked `CellDataStaleWarn` placement after
  freshness block (`include/param_prj.h:76`, `include/param_prj.h:80`,
  `docs/Project_Decisions_Snapshot_v0_1.md:246`).

## 4) Locked append-only field contract

### 4.1 Field names (locked)

- `CellGrp0First`
- `CellGrp0Last`
- `CellGrp1First`
- `CellGrp1Last`
- `CellGrp2First`
- `CellGrp2Last`
- `CellGrp3First`
- `CellGrp3Last`
- `CellGrp4First`
- `CellGrp4Last`

Name-conflict check result:

- **PROVEN:** no existing symbol/field collisions for these names were found in
  current repository search.

### 4.2 Exact meanings (locked)

- Each `CellGrpNFirst` and `CellGrpNLast` field is diagnostic-only.
- For ingest group `N` (`N = 0..4`), values represent the first and last
  currently published `u*` index fed by that group during the current pass.
- These fields are interpretability diagnostics for web UI consumption and are
  not balancing, State of Charge (SOC), current-path, or Controller Area
  Network (CAN) control inputs.
- Value model for next implementation slice is intentionally simple and
  append-only:
  - positive index values for valid first/last observed `u*` indices,
  - zero when no valid published index exists for that group in pass context.

### 4.3 Exact placement (locked)

- All ten fields must be contiguous.
- Preserve already locked placement of `CellDataStaleWarn` immediately after
  `CellGrp4Fresh` (`docs/Project_Decisions_Snapshot_v0_1.md:246`).
- Therefore lock these ten range/interpretation fields as an append-only
  contiguous block immediately after `CellDataStaleWarn`.
- Ordering rationale:
  - current freshness block ends at `CellGrp4Fresh` (`include/param_prj.h:80`),
  - `CellDataStaleWarn` is already canonically reserved immediately after that
    freshness block (`docs/Project_Decisions_Snapshot_v0_1.md:246`),
  - current next-value marker is `2182` (`include/param_prj.h:50`), so this
    block can be appended without renumbering/removing existing fields.

## 5) Explicit out-of-scope list for the next code slice

- no balancing logic changes
- no SOC/current-path changes
- no CAN additions
- no renaming/removal of existing fields
- no forced clearing/rewrite of `u*` values in this slice

## 6) Bounded implementation guardrail

- Keep next implementation as instrumentation-only interpretation fields.
- Do not redesign parser flow or alter ingest semantics.
- Do not replace existing freshness fields; extend append-only diagnostics only.

## Release honesty reminder

This lock does not claim balancing is fixed, SOC is accurate, or full 96-cell
behavior is hardware-validated.
