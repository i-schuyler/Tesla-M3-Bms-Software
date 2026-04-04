# Tesla Model 3 BMS SOC trace-field lock (2026-04-04)

## Document Metadata

- Title: Append-only SOC/current-path trace field contract lock
- Version: 0.1
- Date: 2026-04-04
- Scope: Tiny docs-only lock before instrumentation code
- Status: Investigation artifact with locked contract choice

## 1) Purpose and scope

This document locks the append-only trace-field contract for the next bounded
SOC/current-path instrumentation slice.

This slice is docs-only. No firmware behavior, SOC math, balancing behavior,
or Controller Area Network (CAN) payload implementation is changed.

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical docs:
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md` (`DEC-0009`)
3. Prior SOC validation lock:
   - `docs/investigations/tesla-m3-bms-soc-current-path-validation-2026-04-04.md`
4. Repo-local evidence:
   - `include/param_prj.h`
   - `src/BMSUtil.cpp`
   - `src/terminal_prj.cpp`
   - `src/main.cpp`

## 3) Current evidence baseline

- `PROVEN`: existing `idc` field exists and remains part of parameter surface.
  - Evidence: `include/param_prj.h:73`.
- `PROVEN`: `asDiff` is consumed in SOC integration path.
  - Evidence: `src/BMSUtil.cpp:31`, `src/BMSUtil.cpp:52`.
- `PROVEN`: prior validation lock classifies in-repo `idc` source as unproven
  and missing `asDiff` writer as a proven missing path.
  - Evidence: `docs/investigations/tesla-m3-bms-soc-current-path-validation-2026-04-04.md:49`,
    `docs/investigations/tesla-m3-bms-soc-current-path-validation-2026-04-04.md:71`,
    `docs/investigations/tesla-m3-bms-soc-current-path-validation-2026-04-04.md:100`.
- `PROVEN`: parameter ordering matters for exposed surfaces.
  - Evidence: ordering in `PARAM_LIST` (`include/param_prj.h:64` to
    `include/param_prj.h:80`), exposed through JSON request path
    (`src/terminal_prj.cpp:47`, `src/main.cpp:203` to `src/main.cpp:205`).

## 4) Locked append-only trace contract for next code slice

### Existing field (unchanged)

- `idc` remains unchanged in name, meaning, and existing use.

### New append-only diagnostic fields

- `IDCTrace` = current `Param::idc` value observed by firmware for
  trace/validation.
- `AsDiffTrace` = current `asDiff` value observed by SOC logic for
  trace/validation.

### Exact placement (locked)

- Place `IDCTrace` immediately after `idc`.
- Place `AsDiffTrace` immediately after `IDCTrace`.
- Placement anchor in current ordering: after `VALUE_ENTRY(idc, ...)` and before
  current next entry (`TempMax`) in `PARAM_LIST` ordering.

## 5) Do-not-change list for next code slice

1. Do not change SOC math behavior.
2. Do not implement/restore `asDiff` writer in this instrumentation slice.
3. Do not add CAN payload fields for trace metrics.
4. Do not rename or remove existing fields (`idc`, `soc`, or others).
5. Do not renumber existing parameter/value identifiers.

## 6) Naming and placement safety check

- `PROVEN`: proposed names `IDCTrace` and `AsDiffTrace` do not conflict with
  existing field names in current `PARAM_LIST` scan.
- `PROVEN`: exact placement can be reasoned safely from established sequential
  ordering conventions in `PARAM_LIST`.

## 7) Next-slice implementation direction (bounded)

1. Add the two fields append-only at the locked position.
2. Publish trace values without modifying SOC branch logic or thresholds.
3. Keep release claims conservative: diagnostics only, not SOC-accuracy proof.

