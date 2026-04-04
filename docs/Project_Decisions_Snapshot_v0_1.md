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

## Append-Only Rule

- Do not rewrite old decisions.
- Add a new decision entry to supersede or refine prior direction.
