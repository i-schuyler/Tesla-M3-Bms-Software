# Tesla Model 3 BMS Targeted Raw-Cell Debug Semantics/Placement Lock (2026-04-13)

## Document Metadata

- Title: Targeted raw-cell debug diagnostics semantic and UI-placement lock
- Version: 0.1
- Date: 2026-04-13
- Scope: Docs-first lock for bounded follow-up implementation
- Status: Investigation artifact (non-canonical until promoted)
- Session Anchor: `HL-2026-04-03 — optimize_bms`

## 1) Purpose and scope

This document locks semantics and exact placement for append-only targeted
debug fields so the next implementation slice can be executed without guessing.

This slice is docs-only.

Explicit non-goals:

- no firmware/code behavior changes
- no balancing logic changes
- no State of Charge (SOC)/current-path changes
- no Controller Area Network (CAN)/reporting implementation changes
- no Continuous Integration (CI)/release/Makefile changes
- no silent redefinition of existing `u1..u96` meaning

## 2) Authorities consulted

1. `00_Identity` (session authority order)
2. Canonical project docs:
   - `AGENTS.md`
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Existing implementation/order surfaces:
   - `include/param_prj.h`
   - `src/BatMan.cpp`
   - `src/terminal_prj.cpp`
   - `src/main.cpp`
4. Current publication-path investigation:
   - `docs/investigations/tesla-m3-bms-u-display-publication-investigation-2026-04-07.md`

## 3) Evidence block (mandatory)

### 3.1 Existing `u1..u96` definitions

- `u1..u96` are declared as millivolt display values in
  `include/param_prj.h:102` through `include/param_prj.h:197`.
- Runtime publication writes flattened values into `u*` using
  `Param::SetFloat((Param::PARAM_NUM)(Param::u1 + h), (Voltage[Xr][Yc]))` at
  `src/BatMan.cpp:781`.

### 3.2 Existing age diagnostics definitions and placement

- Existing age diagnostics are present as append-only fields
  `u1Age..u96Age` at `include/param_prj.h:258` through
  `include/param_prj.h:353`.
- Existing age summary fields are already defined and placed before the range
  fields in the current parameter list (`CellStaleCount` at
  `include/param_prj.h:82`, `CellMaxAge` at `include/param_prj.h:83`).
- Runtime age publication is implemented in `upDateCellVolts()` via
  `Param::SetInt((Param::PARAM_NUM)(Param::u1Age + cellIdx), cellAge)` at
  `src/BatMan.cpp:866`, with summaries written at `src/BatMan.cpp:878` through
  `src/BatMan.cpp:879`.

### 3.3 Current publication-path investigation

- The current repository investigation documents the raw-to-display mapping as
  `Fluffer[72]` -> `Voltage[8][15]` -> flattened `Param::u1 + h` publication in
  `docs/investigations/tesla-m3-bms-u-display-publication-investigation-2026-04-07.md:120`
  through
  `docs/investigations/tesla-m3-bms-u-display-publication-investigation-2026-04-07.md:124`.
- It also records the observed behavior boundary framing (`u1..u12` moving;
  `u13..u96` appearing steady) in
  `docs/investigations/tesla-m3-bms-u-display-publication-investigation-2026-04-07.md:16`
  through
  `docs/investigations/tesla-m3-bms-u-display-publication-investigation-2026-04-07.md:18`.

### 3.4 Existing UI/order surfaces informing placement

- Parameter ordering contract is explicit in `PARAM_LIST` ordering notes at
  `include/param_prj.h:44` through `include/param_prj.h:48`.
- Terminal JSON publication path exists through
  `TerminalCommands::PrintParamsJson` in `src/terminal_prj.cpp:47` and is served
  in the main loop at `src/main.cpp:203` through `src/main.cpp:206`.

### 3.5 Existing docs/decisions constraining append-only behavior

- Do not silently change user-facing contracts in `AGENTS.md:40` through
  `AGENTS.md:43`.
- Existing append-only decision rule in
  `docs/Project_Decisions_Snapshot_v0_1.md:396` through
  `docs/Project_Decisions_Snapshot_v0_1.md:399`.
- Existing age semantics and placement lock in
  `docs/Project_Decisions_Snapshot_v0_1.md:308` through
  `docs/Project_Decisions_Snapshot_v0_1.md:355`.

## 4) Why these five cells are selected

- `u1` and `u12`: selected from the observed moving front segment documented as
  `u1..u12` in
  `docs/investigations/tesla-m3-bms-u-display-publication-investigation-2026-04-07.md:16`.
- `u13`: selected as the immediate boundary index after that moving segment,
  matching the observed transition to the steady set in
  `docs/investigations/tesla-m3-bms-u-display-publication-investigation-2026-04-07.md:17`.
- `u86`: explicitly required by session lock and consistent with prior field
  context noting long-stable `u86` observations in
  `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md:44`.
- `u96`: selected as deep tail coverage within the steady-range framing
  (`u13..u96`) in
  `docs/investigations/tesla-m3-bms-u-display-publication-investigation-2026-04-07.md:17`.

## 5) Locked append-only field set

### 5.1 Raw mirrors

- `DbgU1Raw`
- `DbgU12Raw`
- `DbgU13Raw`
- `DbgU86Raw`
- `DbgU96Raw`

### 5.2 Fresh-this-pass flags

- `DbgU1Fresh`
- `DbgU12Fresh`
- `DbgU13Fresh`
- `DbgU86Fresh`
- `DbgU96Fresh`

## 6) Exact semantic definitions (LOCKED)

For target index `N` in `{1, 12, 13, 86, 96}`:

- **`DbgUNRaw` (LOCKED):** debug mirror of the internal mapped source value used
  by the flattened `u*` publication path when published index `N` is processed in
  the current pass.
  - Source surface is the same internal value path used for `u*` publication:
    `Voltage[Xr][Yc]` at write point `src/BatMan.cpp:781`.
  - Unit is millivolts.
  - This is diagnostic-only and append-only.

- **`DbgUNFresh` (LOCKED):** fresh-this-pass marker for that mapped source in the
  same pass.
  - `1` means the mapped source slot for `N` is fresh in current pass by the
    same criterion already used by age-refresh logic:
    `VoltageFresh[Xr][Yc] != 0` (`src/BatMan.cpp:782` through
    `src/BatMan.cpp:785`).
  - `0` means not fresh-this-pass by that criterion.
  - This is diagnostic-only and append-only.

## 7) Exact statement of what is and is not mapped raw/internal source

### 7.1 What **is** mapped (LOCKED)

- The mapped source is the internal value selected by the existing flattened
  publication path for a specific displayed index (`Param::u1 + h`) in the
  current pass (`src/BatMan.cpp:758` through `src/BatMan.cpp:781`).
- `DbgUNRaw` mirrors that selected internal mapped value for index `N`.
- `DbgUNFresh` mirrors that mapped value's per-slot freshness truth in the same
  pass (`src/BatMan.cpp:782` through `src/BatMan.cpp:785`).

### 7.2 What is **not** mapped (LOCKED)

- Not a direct raw-byte (`Fluffer`) export.
- Not a physical harness-cell identity claim.
- Not a redefinition of existing `uN` meaning.
- Not a CAN/report contract addition in this docs-only slice.

## 8) Exact placement lock (copy-forward wording)

The following wording is locked for verbatim copy into the next implementation
prompt.

- **Placement block (LOCKED):** append these ten fields contiguously immediately
  after existing `u96Age` at the bottom of the display-value list in
  `PARAM_LIST`.
- **Order block (LOCKED):**
  1. `DbgU1Raw`
  2. `DbgU1Fresh`
  3. `DbgU12Raw`
  4. `DbgU12Fresh`
  5. `DbgU13Raw`
  6. `DbgU13Fresh`
  7. `DbgU86Raw`
  8. `DbgU86Fresh`
  9. `DbgU96Raw`
  10. `DbgU96Fresh`
- **Adjacency lock (LOCKED):** each `DbgUNRaw` must be immediately followed by
  its matching `DbgUNFresh`.

## 9) What remains unchanged (LOCKED)

- `u1..u96` meaning remains unchanged as existing numeric displayed cell values.
- Existing `u1Age..u96Age`, `CellStaleCount`, and `CellMaxAge` semantics remain
  unchanged.
- No balancing, SOC/current-path, CAN/reporting, CI/release, or Makefile
  changes are approved by this lock.

## 10) PROVEN / INFERRED / UNPROVEN

### PROVEN

1. Existing flattened publication path and mapped source write point are present
   in repo (`src/BatMan.cpp:758` through `src/BatMan.cpp:781`).
2. Existing per-slot freshness truth exists (`VoltageFresh`) and already drives
   age refresh behavior (`src/BatMan.cpp:79`, `src/BatMan.cpp:782` through
   `src/BatMan.cpp:785`).
3. Existing ordering/append-only governance is documented in canonical docs
   (`include/param_prj.h:44`,
   `docs/Project_Decisions_Snapshot_v0_1.md:396`).

### INFERRED

1. UI listing will continue to follow parameter declaration order as exposed by
   terminal JSON paths (`src/terminal_prj.cpp:47`, `src/main.cpp:203`).

### UNPROVEN

1. Hardware/runtime validation of these new targeted debug fields is not part of
   this docs-only slice.
2. Physical-cell identity certainty for flattened index `uN` is not claimed by
   this lock.

## 11) Implementation lock for next bounded code slice

The next implementation slice is locked to:

1. Add only the ten append-only fields listed in Section 5.
2. Keep existing field names/IDs/semantics untouched.
3. Implement Section 6 semantics exactly.
4. Implement Section 8 placement/order exactly.
5. Keep out-of-scope items in Section 9 unchanged.

## 12) Verification plan (docs-only)

- Verify this lock references existing repo evidence and line-addressable
  surfaces.
- Verify no firmware/source behavior files are edited in this slice.
- Verify decisions snapshot is updated append-only to promote this lock.
- Verify canonical index update is skipped unless discoverability requires it.
