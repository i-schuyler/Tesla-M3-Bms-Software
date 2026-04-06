# Project Decisions Snapshot v0.1

## Document Metadata

- Title: Project Decisions Snapshot
- Version: 0.1
- Purpose: Record project decisions in append-only form
- Scope: Canonical documentation governance and decision history

## Purpose

This file records project decisions in append-only form.

Entries document what is decided, what remains investigation-only, and what
evidence supports each status.

## Status Labels

- `LOCKED`: approved for use as canonical project direction.
- `PROVISIONAL`: accepted for current planning but not yet hardware-validated.
- `INVESTIGATION-FINDING`: documented finding, not an approved behavior change.

## Decision Log (append-only)

### DEC-0001 — Establish canonical docs spine

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Canonical project docs are now defined by `AGENTS.md`, this decisions
    snapshot, the assumptions registry, and the canonical index.
  - Investigation notes remain in `docs/investigations/` and require explicit
    promotion before becoming canonical.
- Evidence:
  - `docs/Canonical_Documentation_Index_v1_0.md`
  - `docs/Project_Assumptions_Registry_v0_1.md`

### DEC-0002 — Keep behavior unchanged in Slice 1A

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Slice 1A is documentation-only and does not approve firmware behavior
    changes.
  - No balancing, State of Charge (SOC), Controller Area Network (CAN), or
    state-machine behavior changes are implied.
- Evidence:
  - Scope and constraints captured in `AGENTS.md`
  - Repo Reality Scan at
    `docs/investigations/tesla-m3-bms-repo-reality-scan-2026-04-04.md`

### DEC-0003 — Current investigation context remains finding-level

- Date: 2026-04-04
- Status: `INVESTIGATION-FINDING`
- Decision:
  - Current Tesla Model 3 BMS investigation context is retained as
    investigation output only.
  - Investigation findings are not treated as silently approved code changes.
- Evidence:
  - `docs/investigations/tesla-m3-bms-repo-reality-scan-2026-04-04.md`
- Notes:
  - Promotion requires explicit follow-up decision entries with proof and
    scope.

### DEC-0004 — Lock idcmode=0 fix ordering and defer idcmode>0 path

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - For the current planning context (`numbmbs = 4`, `idcmode = 0`), firmware
    follow-up slices are ordered as: bounds safety first, slot-coverage
    alignment second, balancing-metric semantics third.
  - Items that depend on `idcmode > 0` current-path behavior are explicitly
    deferred and must not block idcmode=0 safety fixes.
  - This lock is planning-only and does not itself approve firmware behavior
    changes.
- Evidence:
  - `docs/investigations/tesla-m3-bms-idcmode0-fix-matrix-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-investigation-2026-04-03.md`

### DEC-0005 — Tighten first firmware slice to proven idcmode=0 defects

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Refine ordering so the first firmware slice combines the strongest-evidence,
    idcmode=0-relevant correctness fixes in one bounded pass:
    - proven `<= 8` bounds defects on `[8]` arrays,
    - proven slot/index `14` coverage mismatch.
  - Keep inferred chip-count guardrails after that first correctness slice.
  - Keep balancing semantics after correctness/guardrail slices.
  - Keep idcmode>0 current-path and dependent State of Charge (SOC) items
    deferred.
- Evidence:
  - `docs/investigations/tesla-m3-bms-idcmode0-fix-matrix-2026-04-04.md`

### DEC-0006 — Lock balancing reporting semantics for v0.x

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Preserve existing `CellsBalancing` meaning for v0.x as current pre-mask
    candidate-count behavior.
  - Do not silently redefine `CellsBalancing` to post-mask commanded-count
    behavior.
  - If commanded-count visibility is required, implement it as an append-only
    field/counter in a dedicated follow-up slice.
- Evidence:
  - `docs/investigations/tesla-m3-bms-balancing-semantics-lock-2026-04-04.md`
  - `src/BatMan.cpp`
  - `include/param_prj.h`

### DEC-0007 — Lock append-only commanded-balancing field contract

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Preserve `CellsBalancing` as pre-mask candidate count.
  - Add `CellsBalancingCmd` as append-only post-mask commanded-count field in a
    follow-up code slice.
  - Place `CellsBalancingCmd` immediately after `CellsBalancing` in parameter
    ordering (before `LoopCnt`).
  - Keep next code slice out of scope for CAN additions and balancing
    threshold/cadence changes.
- Evidence:
  - `docs/investigations/tesla-m3-bms-balancing-commanded-field-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-balancing-semantics-lock-2026-04-04.md`

### DEC-0008 — Lock SOC/current-path investigation status before code changes

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Keep SOC/current-path changes blocked until a bounded validation slice
    confirms runtime `idc` source behavior.
  - Treat `asDiff` as "consumed but no writer proven in repo" for planning.
  - Preserve conservative release language: no SOC accuracy claim from repo-only
    evidence.
- Evidence:
  - `docs/investigations/tesla-m3-bms-soc-current-path-lock-2026-04-04.md`
  - `src/BMSUtil.cpp`
  - `include/param_prj.h`

### DEC-0009 — Validate idcmode=0 SOC current-path truth and keep fixes gated

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Validation confirms `asDiff` is consumed but has no writer in current repo
    code.
  - Validation does not prove any in-repo `idc` source path under `idcmode=0`.
  - Keep direct SOC behavior changes gated pending bounded runtime
    instrumentation evidence.
- Evidence:
  - `docs/investigations/tesla-m3-bms-soc-current-path-validation-2026-04-04.md`
  - `src/BMSUtil.cpp`
  - `src/main.cpp`
  - `src/CAN_Common.cpp`

### DEC-0010 — Lock append-only SOC trace fields and placement

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Keep existing `idc` field unchanged.
  - Add append-only diagnostics in the next instrumentation slice:
    - `IDCTrace` for observed `Param::idc` trace value,
    - `AsDiffTrace` for observed `asDiff` trace value.
  - Lock placement as immediate sequence after `idc` (`IDCTrace` first,
    `AsDiffTrace` second).
  - Keep next slice out of scope for SOC math changes, `asDiff` writer
    restoration, CAN additions, and existing-field renames/removals.
- Evidence:
  - `docs/investigations/tesla-m3-bms-soc-trace-field-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-soc-current-path-validation-2026-04-04.md`
  - `include/param_prj.h`

### DEC-0011 — Lock live cell-update anomaly as measurement-path first investigation

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Treat the observed "only `u1..u12` moving" runtime pattern as unresolved
    measurement/update-path behavior until bounded freshness instrumentation is
    captured.
  - Do not claim hardware fault confirmation from current repo evidence plus
    field observations alone.
  - Keep next action narrow: instrumentation-first validation of read-group and
    publication freshness before any hardware-only conclusion.
- Evidence:
  - `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md`
  - `src/BatMan.cpp`
  - `include/BatMan.h`
  - `src/main.cpp`

### DEC-0012 — Lock append-only cell-group freshness fields and placement

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Lock append-only diagnostic fields for cell-group freshness as:
    `CellGrp0Fresh`, `CellGrp1Fresh`, `CellGrp2Fresh`, `CellGrp3Fresh`,
    `CellGrp4Fresh`.
  - Lock meaning as per-group freshness indicators for the five Model 3
    cell-ingest read groups (`0x47..0x4B`), diagnostic-only.
  - Lock placement as five contiguous fields immediately after `AsDiffTrace`.
  - Keep next code slice explicitly out of scope for balancing logic changes,
    SOC/current-path behavior changes, CAN additions, and existing-field
    renames/removals.
- Evidence:
  - `docs/investigations/tesla-m3-bms-cell-group-freshness-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md`
  - `src/BatMan.cpp`
  - `include/param_prj.h`

### DEC-0013 — Lock bounded scope for proven parser/ingest overrun fix

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Lock the next parser safety code slice to one proven defect only:
    `BATMan::GetData()` raw ingest overrun risk from `Fluffer[72]` storage with
    loop bound `count2 <= 72`.
  - Keep implementation bounded to ingest bound hardening only; do not alter
    balancing behavior, State of Charge (SOC)/current path, or Controller Area
    Network (CAN) payload/contracts.
  - Keep parser group semantics and publication contracts unchanged while
    removing overrun risk.
- Evidence:
  - `docs/investigations/tesla-m3-bms-parser-risk-lock-2026-04-04.md`
  - `src/BatMan.cpp`
  - `src/CAN_Common.cpp`
  - `src/main.cpp`
  - `include/param_prj.h`

### DEC-0014 — Lock append-only stale-display warning field and placement

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Lock new append-only diagnostic field name as `CellDataStaleWarn`.
  - Lock meaning as a diagnostic warning that one or more later cell-ingest
    groups are not fresh, so displayed `u*` values may be partially stale or
    front-loaded toward earlier groups.
  - Lock placement as immediate insertion after `CellGrp4Fresh` in
    parameter ordering.
  - Keep next code slice out of scope for balancing logic changes,
    SOC/current-path changes, CAN additions, existing-field renames/removals,
    and forced clearing/rewrite of `u*` values.
- Evidence:
  - `docs/investigations/tesla-m3-bms-stale-display-warning-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-cell-group-freshness-lock-2026-04-04.md`
  - `include/param_prj.h`

### DEC-0015 — Lock append-only cell-group range interpretation fields

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Lock new append-only diagnostic interpretation fields as:
    `CellGrp0First`, `CellGrp0Last`, `CellGrp1First`, `CellGrp1Last`,
    `CellGrp2First`, `CellGrp2Last`, `CellGrp3First`, `CellGrp3Last`,
    `CellGrp4First`, `CellGrp4Last`.
  - Lock meaning as first/last currently published `u*` index fed by each
    ingest group in the current pass, diagnostic-only for web UI
    interpretability.
  - Preserve existing lock that `CellDataStaleWarn` is immediately after
    `CellGrp4Fresh`; place these ten fields contiguously immediately after
    `CellDataStaleWarn`.
  - Keep next code slice out of scope for balancing logic changes,
    SOC/current-path changes, CAN additions, existing-field renames/removals,
    and forced clearing/rewrite of `u*` values.
- Evidence:
  - `docs/investigations/tesla-m3-bms-cell-group-range-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-cell-group-freshness-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-stale-display-warning-lock-2026-04-04.md`
  - `src/BatMan.cpp`
  - `include/param_prj.h`

## Append-Only Rule

- Do not rewrite old decisions.
- Add a new decision entry to supersede or refine prior direction.
