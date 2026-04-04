# Tesla Model 3 BMS balancing semantics lock (2026-04-04)

## Document Metadata

- Title: Balancing/reporting semantics lock for Slice 2D-A
- Version: 0.1
- Date: 2026-04-04
- Scope: Docs-first semantic lock before balancing/reporting code work
- Status: Investigation artifact with lock recommendation

## 1) Purpose and scope

This document locks terminology and contract intent for balancing-related
reporting surfaces before any balancing/reporting code changes.

This slice is documentation-only. It does not modify firmware behavior,
Controller Area Network (CAN) payload implementation, or reporting code paths.

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical project docs:
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Existing investigation anchors:
   - `docs/investigations/tesla-m3-bms-idcmode0-fix-matrix-2026-04-04.md`
   - `docs/investigations/tesla-m3-bms-investigation-2026-04-03.md`
4. Repo-local implementation evidence:
   - `src/BatMan.cpp`
   - `include/param_prj.h`
   - `src/terminal_prj.cpp`
   - `src/main.cpp`
   - `src/CAN_Common.cpp`

## 3) Definitions (locked terms for this planning stage)

- **Balancing candidate cells**:
  Cells that pass selection criteria in `upDateCellVolts()` before odd/even
  pass masking.
- **Commanded cells this pass**:
  Cells that remain set after odd/even masking in `WriteCfg()` for the current
  write pass.
- **Reported/displayed balancing field**:
  Existing `CellsBalancing` parameter surface (`VALUE_ENTRY`) and any output
  channel that exposes parameter values.

## 4) Current repo evidence

### 4.1 Candidate selection path

- `PROVEN`: balancing eligibility gate is threshold-driven.
  - Evidence: `src/BatMan.cpp:658` (`BallVthres < umax`).
- `PROVEN`: candidate bit is set before mask and counter increments at that
  point.
  - Evidence: `src/BatMan.cpp:714` sets `CellBalCmd[Xr]` bit.
  - Evidence: `src/BatMan.cpp:715` increments `CellBalancing`.

### 4.2 Command masking path

- `PROVEN`: odd/even masking is applied in `WriteCfg()` using `0xAA`/`0x55`.
  - Evidence: `src/BatMan.cpp:556` to `src/BatMan.cpp:564`.
- `PROVEN`: transmitted config payload is built from masked `tempData` bytes.
  - Evidence: `src/BatMan.cpp:551` to `src/BatMan.cpp:553`,
    `src/BatMan.cpp:572` to `src/BatMan.cpp:574`.

### 4.3 Publication/reporting surfaces

- `PROVEN`: `CellsBalancing` value is published directly from `CellBalancing`
  counter in `upDateCellVolts()`.
  - Evidence: `src/BatMan.cpp:745`.
- `PROVEN`: `CellsBalancing` is an existing reportable parameter field.
  - Evidence: `include/param_prj.h:232`.
- `PROVEN`: parameter surfaces are exposed via JSON output path.
  - Evidence: `src/terminal_prj.cpp:47`, `src/main.cpp:203` to
    `src/main.cpp:205`.
- `PROVEN`: current explicit CAN frames in `CAN_Common.cpp` do not show a
  dedicated `CellsBalancing` payload field.
  - Evidence: frame sends at `src/CAN_Common.cpp:85`, `src/CAN_Common.cpp:103`,
    `src/CAN_Common.cpp:122`, `src/CAN_Common.cpp:135`.

## 5) Distinction lock: candidate vs commanded vs reported

1. **Candidate cells**
   - Status: `PROVEN`
   - Meaning in current code: cells counted when selection condition passes in
     `upDateCellVolts()` prior to odd/even masking.

2. **Commanded cells this pass**
   - Status: `PROVEN` (masking exists), `UNPROVEN` (no separate published count)
   - Meaning in current code: masked command bits used to build write payload in
     `WriteCfg()`.
   - Gap: no dedicated counter/field for post-mask commanded count is published.

3. **Reported/displayed field (`CellsBalancing`)**
   - Status: `PROVEN`
   - Current meaning by code path: pre-mask candidate-count style value.
   - Operator ambiguity status: `INFERRED` (name does not explicitly say
     candidate vs commanded).

## 6) Contract choice for v0.x (semantic lock)

### Locked choice

- **Preserve existing field meaning** for `CellsBalancing` in v0.x as the
  current pre-mask candidate-count behavior.
- **Do not silently redefine** `CellsBalancing` to mean post-mask commanded
  cells.
- If commanded-count visibility is required later, use an **append-only
  addition** (new field/counter) in a dedicated code slice.

Rationale:

- `PROVEN` code path shows counting occurs before odd/even masking.
- Silent redefinition risks breaking current user-facing interpretation and
  tooling contracts.

## 7) Recommended implementation direction for next code slice

1. Keep existing `CellsBalancing` write path unchanged for backward
   compatibility.
2. Add a separate post-mask commanded-count metric only if needed, using
   append-only contract expansion.
3. Document both metrics with explicit labels (candidate vs commanded) in
   follow-up docs and release notes.

## 8) Unknowns / proof gaps

1. `UNPROVEN`: exact downstream consumer expectation for `CellsBalancing`
   (candidate vs commanded) outside this repo.
2. `UNPROVEN`: whether operators rely on `CellsBalancing` as a strict proxy for
   commanded bits per pass.
3. `INFERRED`: ambiguity risk from current field naming in UI/reporting
   contexts.

## 9) Proven / inferred / unproven summary

- `PROVEN`: candidate increment timing, odd/even masking behavior, and
  publication of `CellsBalancing` from pre-mask counting path.
- `INFERRED`: operator-facing ambiguity due to field naming.
- `UNPROVEN`: external consumer contract assumptions for commanded-count
  semantics.

