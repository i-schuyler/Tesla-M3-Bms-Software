# Tesla Model 3 BMS Cell-Age Diagnostics Semantics/Placement Lock (2026-04-07)

## Document Metadata

- Title: Cell-age diagnostics semantic and UI-placement lock
- Version: 0.1
- Date: 2026-04-07
- Scope: Docs-first lock before implementation
- Status: Investigation artifact (non-canonical until promoted)

## 1) Purpose and scope

This document locks semantics and exact parameter/UI placement for append-only
cell-age diagnostics to support a bounded follow-up implementation slice.

This slice is docs-only.

Explicit non-goals:

- no firmware/code behavior changes
- no balancing logic changes
- no State of Charge (SOC)/current-path changes
- no Controller Area Network (CAN)/reporting implementation changes
- no Continuous Integration (CI)/release/Makefile changes
- no silent redefinition of existing `u1..u96` fields

## 2) Authorities consulted

1. `00_Identity` (session authority order)
2. Canonical docs:
   - `AGENTS.md`
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Repo implementation/order surfaces:
   - `include/param_prj.h`
   - `src/BatMan.cpp`
   - `src/terminal_prj.cpp`
   - `src/main.cpp`

## 3) Evidence block (mandatory)

### 3.1 Existing `u1..u96` definitions/surface

- `u1..u96` are declared as `VALUE_ENTRY(..., "mV", ...)` in
  `include/param_prj.h:100` through `include/param_prj.h:195`.
- Runtime publication uses numeric assignment into `u` fields via
  `Param::SetFloat((Param::PARAM_NUM)(Param::u1 + h), Voltage[Xr][Yc])` at
  `src/BatMan.cpp:759`.

### 3.2 Existing freshness diagnostics definitions/placement surface

- Existing freshness block:
  `CellGrp0Fresh..CellGrp4Fresh` in `include/param_prj.h:76` through
  `include/param_prj.h:80`.
- Existing stale warning immediately after freshness block:
  `CellDataStaleWarn` at `include/param_prj.h:81`.
- Existing range diagnostics immediately after stale warning:
  `CellGrp0First..CellGrp4Last` at `include/param_prj.h:82` through
  `include/param_prj.h:91`.

### 3.3 Existing UI/order surfaces informing placement

- Parameter ordering is explicit in `PARAM_LIST` and documented as ordered
  entries in `include/param_prj.h:44` through `include/param_prj.h:52`.
- The ESP8266-facing terminal command set includes JSON parameter export via
  `TerminalCommands::PrintParamsJson` at `src/terminal_prj.cpp:47`.
- Main loop services JSON print requests via
  `TerminalCommands::PrintParamsJson` at `src/main.cpp:203` through
  `src/main.cpp:206`.

### 3.4 Existing docs/decisions that constrain append-only behavior/meaning

- No silent user-facing contract changes rule in `AGENTS.md:40` through
  `AGENTS.md:43`.
- Existing lock that `CellDataStaleWarn` follows `CellGrp4Fresh`:
  `docs/Project_Decisions_Snapshot_v0_1.md:242` through
  `docs/Project_Decisions_Snapshot_v0_1.md:247`.
- Existing lock that `CellGrp0First..CellGrp4Last` are contiguous after
  `CellDataStaleWarn`: `docs/Project_Decisions_Snapshot_v0_1.md:269` through
  `docs/Project_Decisions_Snapshot_v0_1.md:271`.
- Decision-append mechanism (supersede/refine by new entry) in
  `docs/Project_Decisions_Snapshot_v0_1.md:308` through
  `docs/Project_Decisions_Snapshot_v0_1.md:311`.

## 4) Current meaning lock for existing `u1..u96`

### PROVEN

- Existing `u1..u96` fields are numeric millivolt display values by declaration
  and write path (`include/param_prj.h:100`, `src/BatMan.cpp:759`).

### LOCK

- `u1..u96` remain unchanged as numeric last-known displayed cell values.
- No age text is appended into existing `u*` values.

## 5) Why inline formatting like `u1 3850 (5s)` is rejected

### PROVEN

- Existing `u*` values are numeric fields (`"mV"` value entries) and are updated
  with numeric `SetFloat` writes (`include/param_prj.h:100`,
  `src/BatMan.cpp:759`).
- Project safety constraints prohibit silent user-facing contract changes
  (`AGENTS.md:40`).

### LOCK

- String decoration of existing `u*` values is rejected because it would alter
  the established numeric field contract and force parser/UI/client behavior
  changes outside this slice.

## 6) Locked semantic definitions

### 6.1 Existing fields

- **`u1..u96` (LOCKED):** last known numeric displayed cell value in millivolts.

### 6.2 New per-cell age fields

- **`u1Age..u96Age` (LOCKED):** age in whole seconds since that displayed
  `uN` value was last refreshed from current-pass data.
- Whole seconds are integer seconds (`0, 1, 2, ...`), not fractional.

### 6.3 New summary fields

- **`CellStaleCount` (LOCKED):** count of displayed cells considered stale by the
  locked stale rule below.
- **`CellMaxAge` (LOCKED):** maximum whole-second age across displayed
  `u1..u96` values.

### 6.4 Locked stale rule

- **Stale rule (LOCKED):** a displayed cell is stale when `uNAge > 0`.
- Therefore `CellStaleCount` counts how many of `u1Age..u96Age` are `> 0`.

Rationale for this lock:

- Keeps semantics deterministic without introducing new threshold parameters.
- Avoids timing-threshold guessing in this docs-first slice.

## 7) Exact UI/parameter placement lock

This section intentionally uses exact placement wording for copy-forward into the
next implementation prompt.

### 7.1 Placement of `CellStaleCount` and `CellMaxAge`

- **LOCKED placement:**
  - `CellStaleCount` is inserted immediately after `CellGrp4Last`.
  - `CellMaxAge` is inserted immediately after `CellStaleCount`.

Relative order reference in current list:

1. `CellGrp0Fresh..CellGrp4Fresh`
2. `CellDataStaleWarn`
3. `CellGrp0First..CellGrp4Last`
4. `CellStaleCount`
5. `CellMaxAge`
6. then existing `TempMax` and later fields.

### 7.2 Placement of `u1Age..u96Age`

- **LOCKED placement:** `u1Age..u96Age` are append-only and placed at the bottom
  of the display-value parameter list.
- They are appended after the current last display field (`cpuload`) in
  contiguous order `u1Age`, `u2Age`, ..., `u96Age`.
- They are therefore after existing per-cell voltage block, after existing
  temperature/chip diagnostics, and at the bottom of the UI parameter list.

## 8) PROVEN / INFERRED / UNPROVEN classification

### PROVEN

1. Existing `u*` fields are numeric and written numerically.
2. Existing freshness/range diagnostics placement is fixed in current
   `PARAM_LIST`.
3. Existing project guidance requires append-only behavior and avoids silent
   user-facing contract changes.

### INFERRED

1. UI rendering order follows parameter declaration order exported by the
   terminal JSON path; explicit downstream web sort logic is not in this repo
   snapshot.

### UNPROVEN

1. Exact runtime timebase chosen by implementation (`rtc`, loop timestamp, or
   scheduler-derived) is not fixed by current code and must be locked in the
   next implementation slice.

## 9) Recommended implementation direction (next bounded code slice)

- Keep existing `u1..u96` fields unchanged.
- Add append-only new fields only:
  - summary: `CellStaleCount`, `CellMaxAge`
  - per-cell: `u1Age..u96Age`
- Track per-cell last-refresh second when a `uN` value is refreshed from
  current-pass data.
- Publish `uNAge` as current-second minus last-refresh-second.
- Compute `CellStaleCount` using locked stale rule (`age > 0`) and `CellMaxAge`
  as max age each publication cycle.
- Keep out of scope: balancing/SOC/current path/CAN/report-contract redesign.

## 10) Unknowns / proof gaps

- Exact timebase contract is implementation-open and must be chosen explicitly.
- No runtime/hardware evidence in this slice validates resulting age dynamics;
  this lock covers semantics/placement only.
