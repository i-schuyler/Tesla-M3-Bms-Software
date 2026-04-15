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

### DEC-0016 — Lock parameter-meanings reference surface for session diagnostics

- Date: 2026-04-03
- Status: `LOCKED`
- Decision:
  - Adopt `docs/Parameter_Meanings_v0_1.md` as the bounded reference surface for
    meanings of session-added or session-clarified diagnostics and
    balancing-related fields.
  - Keep this reference docs-only: no firmware, Controller Area Network (CAN),
    balancing, or State of Charge (SOC) behavior changes are implied by this
    decision.
  - When meanings for the covered fields are clarified further, update this
    reference append-only or supersede via a new decision entry rather than
    silently redefining user-facing interpretation.
- Evidence:
  - `docs/Parameter_Meanings_v0_1.md`
  - `include/param_prj.h`
  - `src/BatMan.cpp`
  - `src/BMSUtil.cpp`
  - `docs/investigations/tesla-m3-bms-balancing-commanded-field-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-soc-trace-field-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-cell-group-freshness-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-stale-display-warning-lock-2026-04-04.md`
  - `docs/investigations/tesla-m3-bms-cell-group-range-lock-2026-04-04.md`

### DEC-0017 — Lock cell-age diagnostics semantics and exact parameter placement

- Date: 2026-04-07
- Status: `LOCKED`
- Decision:
  - Preserve existing `u1..u96` semantics as numeric last-known displayed cell
    values; do not append age text into existing `u*` fields.
  - Lock new append-only age diagnostics for a future bounded implementation
    slice:
    - summary fields: `CellStaleCount`, `CellMaxAge`
    - per-cell fields: `u1Age..u96Age`
  - Lock semantics:
    - `u1Age..u96Age` are whole-second ages since corresponding displayed `uN`
      was last refreshed from current-pass data.
    - `CellMaxAge` is the maximum whole-second age across displayed `u1..u96`.
    - `CellStaleCount` counts displayed cells stale by locked rule: `uNAge > 0`.
  - Lock exact placement:
    - insert `CellStaleCount` immediately after `CellGrp4Last`,
    - insert `CellMaxAge` immediately after `CellStaleCount`,
    - append `u1Age..u96Age` contiguously after existing `cpuload` at the bottom
      of the display-value parameter list.
  - Keep next slice out of scope for balancing logic changes, State of Charge
    (SOC)/current-path changes, Controller Area Network (CAN) contract changes,
    and renaming/retyping existing fields.
- Evidence:
  - `docs/investigations/tesla-m3-bms-cell-age-diagnostics-lock-2026-04-07.md`
  - `include/param_prj.h`
  - `src/BatMan.cpp`
  - `src/terminal_prj.cpp`
  - `src/main.cpp`
  - `AGENTS.md`

### DEC-0018 — Refine cell-age summary placement before group-range diagnostics

- Date: 2026-04-08
- Status: `LOCKED`
- Decision:
  - Refine placement for cell-age summary diagnostics to match current locked
    implementation slice requirements:
    - `CellStaleCount` immediately after `CellDataStaleWarn`,
    - `CellMaxAge` immediately after `CellStaleCount`.
  - Preserve `CellGrp0First..CellGrp4Last` as a contiguous block that now
    follows `CellMaxAge`.
  - Keep prior lock that `u1Age..u96Age` remain appended at the bottom of the
    display-value list.
  - This entry supersedes only placement clauses in DEC-0017 and DEC-0015 where
    they conflict with this ordering; all other semantics remain unchanged.
- Evidence:
  - `include/param_prj.h`
  - `docs/investigations/tesla-m3-bms-cell-age-diagnostics-lock-2026-04-07.md`
  - Session lock requirements for slice `HL-2026-04-03 — optimize_bms`

### DEC-0019 — Lock targeted raw-cell debug diagnostics and exact placement

- Date: 2026-04-13
- Status: `LOCKED`
- Decision:
  - Preserve existing semantics and contracts for `u1..u96`, `u1Age..u96Age`,
    `CellStaleCount`, and `CellMaxAge`.
  - Lock append-only targeted debug fields:
    - raw mirrors: `DbgU1Raw`, `DbgU12Raw`, `DbgU13Raw`, `DbgU86Raw`,
      `DbgU96Raw`
    - fresh-this-pass flags: `DbgU1Fresh`, `DbgU12Fresh`, `DbgU13Fresh`,
      `DbgU86Fresh`, `DbgU96Fresh`
  - Lock semantics for each target index `N` in `{1, 12, 13, 86, 96}`:
    - `DbgUNRaw` mirrors the internal mapped source value used by flattened
      `u*` publication when published index `N` is processed in the current
      pass.
    - `DbgUNFresh` is `1` when that mapped source is fresh-this-pass by existing
      per-slot freshness truth (`VoltageFresh[Xr][Yc] != 0`); else `0`.
  - Lock exact placement as append-only contiguous insertion immediately after
    existing `u96Age` at the bottom of the display-value list.
  - Lock exact order with adjacent raw/fresh pairing:
    `DbgU1Raw`, `DbgU1Fresh`, `DbgU12Raw`, `DbgU12Fresh`, `DbgU13Raw`,
    `DbgU13Fresh`, `DbgU86Raw`, `DbgU86Fresh`, `DbgU96Raw`, `DbgU96Fresh`.
  - Keep next code slice out of scope for balancing logic changes,
    State of Charge (SOC)/current-path changes, Controller Area Network (CAN)
    contract changes, and any renaming/retyping of existing fields.
- Evidence:
  - `docs/investigations/tesla-m3-bms-targeted-raw-debug-lock-2026-04-13.md`
  - `docs/investigations/tesla-m3-bms-u-display-publication-investigation-2026-04-07.md`
  - `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md`
  - `include/param_prj.h`
  - `src/BatMan.cpp`
  - `src/terminal_prj.cpp`
  - `src/main.cpp`
  - `AGENTS.md`

### DEC-0020 — Lock optional persisted 5-second auto-refresh and near-top grouped monitor block

- Date: 2026-04-14
- Status: `LOCKED`
- Decision:
  - Lock one optional UI auto-refresh control with exact label
    `Auto-refresh every 5s`, exact interval `5000 ms`, and default state OFF.
  - Lock local browser persistence of the auto-refresh ON/OFF setting across
    page refresh/reload.
  - Lock data-path consistency: auto-refresh must use the same existing
    refresh/update path as manual refresh in the UI and must not introduce a
    second divergent data path.
  - Lock grouped near-top diagnostic monitor block for timed observation,
    placed above the broader/global full parameter list, containing exactly:
    - `u1`, `u12`, `u13`, `u86`, `u96`
    - `u1Age`, `u12Age`, `u13Age`, `u86Age`, `u96Age`
    - `DbgU1Raw`, `DbgU12Raw`, `DbgU13Raw`, `DbgU86Raw`, `DbgU96Raw`
    - `DbgU1Fresh`, `DbgU12Fresh`, `DbgU13Fresh`, `DbgU86Fresh`,
      `DbgU96Fresh`
    - `CellStaleCount`, `CellMaxAge`
    - `CellGrp0Fresh`, `CellGrp1Fresh`, `CellGrp2Fresh`, `CellGrp3Fresh`,
      `CellGrp4Fresh`
    - `CellDataStaleWarn`, `CellsPresent`, `udc`
  - Lock future join rule: targeted source/slot/update-counter fields for the
    same cells must join the same grouped monitor block when added:
    - `DbgU1SrcChip`, `DbgU1SrcSlot`, `DbgU1UpdCnt`
    - `DbgU12SrcChip`, `DbgU12SrcSlot`, `DbgU12UpdCnt`
    - `DbgU13SrcChip`, `DbgU13SrcSlot`, `DbgU13UpdCnt`
    - `DbgU86SrcChip`, `DbgU86SrcSlot`, `DbgU86UpdCnt`
    - `DbgU96SrcChip`, `DbgU96SrcSlot`, `DbgU96UpdCnt`
  - Preserve existing full-list parameter ordering/semantics and keep this lock
    docs-first only (no firmware, balancing, SOC/current-path, CAN/reporting,
    CI/release, or Makefile behavior changes approved by this entry).
- Evidence:
  - `docs/investigations/tesla-m3-bms-auto-refresh-and-monitor-lock-2026-04-14.md`
  - `include/param_prj.h`
  - `src/terminal_prj.cpp`
  - `src/main.cpp`
  - `docs/Project_Decisions_Snapshot_v0_1.md`
  - `AGENTS.md`

## Append-Only Rule

- Do not rewrite old decisions.
- Add a new decision entry to supersede or refine prior direction.
