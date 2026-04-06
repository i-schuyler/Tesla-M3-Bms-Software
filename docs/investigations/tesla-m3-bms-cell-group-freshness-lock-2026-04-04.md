# Tesla Model 3 BMS Cell-Group Freshness Field Lock (2026-04-04)

## Document Metadata

- Title: Append-only cell-group freshness field contract lock
- Version: 0.1
- Date: 2026-04-04
- Scope: Docs-only field-contract lock before instrumentation code
- Status: Investigation artifact (non-canonical until promoted)

## 1) Purpose and scope

This document locks the append-only diagnostic field contract for upcoming
cell-group freshness instrumentation.

This slice is docs-only and does not implement firmware changes.

Explicit non-goals in this slice:

- no firmware/code changes
- no balancing logic changes
- no State of Charge (SOC) or current-path behavior changes
- no Controller Area Network (CAN) payload or contract changes
- no Continuous Integration (CI) or release-process changes

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical docs:
   - `AGENTS.md`
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Prior investigation input:
   - `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md`
4. Implementation evidence:
   - `src/BatMan.cpp`
   - `include/param_prj.h`

## 3) Repo evidence supporting a five-group lock

- Prior investigation explicitly recommends per-group freshness
  instrumentation for `0x47..0x4B`
  (`docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md:176`).
- Model 3 measurement path reads the five cell groups through `GetData()` calls
  for `0x47`, `0x48`, `0x49`, `0x4A`, `0x4B`
  (`src/BatMan.cpp:257`, `src/BatMan.cpp:259`, `src/BatMan.cpp:261`,
  `src/BatMan.cpp:263`, `src/BatMan.cpp:277`).
- Parser cases map exactly those five groups into cell slots:
  - `0x47`: `g=0..2` (`src/BatMan.cpp:392`)
  - `0x48`: `g=3..5` (`src/BatMan.cpp:406`)
  - `0x49`: `g=6..8` (`src/BatMan.cpp:420`)
  - `0x4A`: `g=9..11` (`src/BatMan.cpp:435`)
  - `0x4B`: `g=12..14` (`src/BatMan.cpp:449`)

Conclusion: five diagnostic freshness fields are justified by current ingest
grouping and do not require a different group count.

## 4) Locked append-only field contract

### 4.1 Field names (locked)

- `CellGrp0Fresh`
- `CellGrp1Fresh`
- `CellGrp2Fresh`
- `CellGrp3Fresh`
- `CellGrp4Fresh`

Name-conflict check result:

- No existing field/symbol collisions for these names were found in current
  repository search.

### 4.2 Exact meanings (locked)

- Each field is a per-group freshness indicator for one of the five current
  cell-read groups handled through `GetData()` (`0x47..0x4B`).
- These fields are diagnostic-only and are not balancing/SOC control inputs.
- Intended operator use is to distinguish freshly updated group paths from stale
  display groups.

### 4.3 Exact placement (locked)

- All five fields must be contiguous.
- They must be inserted immediately after `AsDiffTrace` in `PARAM_LIST`.
- Existing anchored ordering around this region is currently:
  `idc`, `IDCTrace`, `AsDiffTrace`
  (`include/param_prj.h:73`, `include/param_prj.h:74`,
  `include/param_prj.h:75`).
- This placement preserves append-only ordering expectations and prior lock
  semantics (`docs/Project_Decisions_Snapshot_v0_1.md:170`).

Implementation note for the next code slice:

- Use append-only value IDs starting from the current next-value marker
  (`include/param_prj.h:50`) without reassigning existing IDs.

## 5) Do-not-change list (locked for next code slice)

- no balancing logic changes
- no SOC/current-path behavior changes
- no CAN additions
- no renaming or removal of existing fields

## 6) Contract integrity guardrail

This lock is append-only. If future evidence requires different naming or
placement, supersede via a new decision entry rather than rewriting this lock.

