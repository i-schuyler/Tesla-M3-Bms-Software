# Tesla Model 3 BMS commanded-balancing field lock (2026-04-04)

## Document Metadata

- Title: Append-only commanded-balancing field contract lock
- Version: 0.1
- Date: 2026-04-04
- Scope: Slice 2D-A tiny docs lock before code changes
- Status: Investigation artifact with locked contract choice

## 1) Purpose and scope

This document locks the append-only commanded-balancing field contract for the
next code slice.

This is docs-only. No firmware behavior, balancing cadence/thresholds, CAN
payload implementation, or Continuous Integration (CI)/release workflow is
changed here.

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical docs:
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Prior semantics lock:
   - `docs/investigations/tesla-m3-bms-balancing-semantics-lock-2026-04-04.md`
4. Repo-local evidence:
   - `include/param_prj.h`
   - `src/BatMan.cpp`

## 3) Current state (evidence-based)

- `PROVEN`: existing `CellsBalancing` field exists and is user/report surface.
  - Evidence: `include/param_prj.h:232`.
- `PROVEN`: current `CellsBalancing` value is published from pre-mask candidate
  counting logic.
  - Evidence: candidate increment `src/BatMan.cpp:715`, publish
    `src/BatMan.cpp:745`.
- `PROVEN`: post-mask command bits are formed in `WriteCfg()` using odd/even
  masking.
  - Evidence: `src/BatMan.cpp:551` to `src/BatMan.cpp:564`.
- `PROVEN`: prior lock already preserves `CellsBalancing` meaning and requires
  append-only commanded-count exposure.
  - Evidence: `docs/investigations/tesla-m3-bms-balancing-semantics-lock-2026-04-04.md:105`.

## 4) Locked field contract for next code slice

### Existing field (unchanged)

- `CellsBalancing` = pre-mask candidate count.
- Status: `LOCKED` (preserve current meaning).

### New append-only field (to be added in next code slice)

- Field name: `CellsBalancingCmd`
- Meaning: post-mask commanded cell count for the current balancing pass.
- Status: `LOCKED` for implementation target.

### Exact placement (locked)

- Insert `CellsBalancingCmd` immediately after `CellsBalancing` in
  parameter/UI-exposed ordering.
- Concretely: place between `CellsBalancing` and `LoopCnt` in `PARAM_LIST`
  ordering (`include/param_prj.h:232` to `include/param_prj.h:233`).

## 5) Do-not-change list for next code slice

1. Do not rename or redefine `CellsBalancing`.
2. Do not remove or renumber existing parameter/value identifiers.
3. Do not add CAN payload fields for this metric in that slice.
4. Do not alter balancing threshold/cadence logic.
5. Do not change State of Charge (SOC)/current-path behavior.

## 6) Recommended implementation direction (next slice)

1. Keep current candidate-count publication path intact.
2. Compute commanded-count after odd/even mask is applied for each pass.
3. Publish only to the new append-only `CellsBalancingCmd` field.
4. Keep all existing field semantics backward-compatible.

## 7) Unknowns / proof gaps

1. `UNPROVEN`: downstream consumer dependence on future commanded-count field
   naming beyond this repo.
2. `UNPROVEN`: whether operators will require commanded-count history versus
   current-pass value only.

## 8) Proven / inferred / unproven summary

- `PROVEN`: current candidate-count behavior and post-mask command path exist.
- `INFERRED`: adding commanded-count as append-only is lowest-risk compatibility
  strategy.
- `UNPROVEN`: external consumer expectations for the new field.

