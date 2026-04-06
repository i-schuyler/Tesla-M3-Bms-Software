# Tesla Model 3 BMS Parser/Ingest Overrun Risk Lock (2026-04-04)

## Document Metadata

- Title: Docs-first lock for proven parser/ingest overrun defect
- Version: 0.1
- Date: 2026-04-04
- Scope: Slice 3D-A (documentation-only scope lock)
- Status: Investigation artifact (non-canonical until promoted)

## 1) Purpose and scope

This document locks the exact scope for the next firmware slice that will fix a
proven parser/ingest overrun defect.

This slice is documentation-only. It does not modify firmware behavior.

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical docs and collaboration constraints:
   - `AGENTS.md`
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Prior related investigation:
   - `docs/investigations/tesla-m3-bms-live-cell-update-path-investigation-2026-04-04.md`
4. Repo-local implementation evidence:
   - `src/BatMan.cpp`
   - `src/CAN_Common.cpp`
   - `src/main.cpp`
   - `src/terminal_prj.cpp`
   - `include/param_prj.h`

## 3) Exact proven defect statement

- **PROVEN:** In `BATMan::GetData()`, raw ingest writes into fixed storage
  `Fluffer[72]` using `for (count2 = 0; count2 <= 72; count2 = count2 + 2)`.
  At `count2 == 72`, writes target `Fluffer[72]` and `Fluffer[73]`, exceeding
  valid indices `0..71`.

This lock addresses only that proven parser/ingest overrun defect.

## 4) Exact affected code surfaces (bounded)

- **PROVEN (defect source):**
  - raw storage definition: `src/BatMan.cpp:59`
  - overrun-prone loop and writes: `src/BatMan.cpp:391`,
    `src/BatMan.cpp:394`, `src/BatMan.cpp:395`
- **PROVEN (immediate consumer surface):** switch parse blocks in
  `BATMan::GetData()` that consume `Fluffer` for groups/status:
  `0x47..0x50` at `src/BatMan.cpp:401` to `src/BatMan.cpp:558`.
- **PROVEN (downstream value propagation path):** parsed `Voltage[][]` values
  are copied into public parameter values (`u1 + h`, `umax`, `umin`, `deltaV`,
  `CellsPresent`) in `src/BatMan.cpp:750`, `src/BatMan.cpp:778`,
  `src/BatMan.cpp:779`, `src/BatMan.cpp:780`, `src/BatMan.cpp:781`.

## 5) Why this matters for UI truthfulness and downstream publication

- **PROVEN:** Terminal/Service Data Object (SDO) JSON publication path exists
  (`src/main.cpp:203`, `src/main.cpp:205`) and terminal JSON command is exposed
  (`src/terminal_prj.cpp:47`).
- **PROVEN:** Parameter list includes directly affected display values
  (`u1..`, `umin`, `umax`, `deltaV`, `CellsPresent`) at
  `include/param_prj.h:71`, `include/param_prj.h:84`,
  `include/param_prj.h:86`, `include/param_prj.h:88`, `include/param_prj.h:89`.
- **PROVEN:** Controller Area Network (CAN) frames publish pack extrema derived
  from those parameters (`Param::umin`, `Param::umax`) in
  `src/CAN_Common.cpp:151`, `src/CAN_Common.cpp:156`,
  `src/CAN_Common.cpp:172`.
- **INFERRED:** If ingest overruns adjacent memory, published user-interface
  and CAN values can be distorted or stale in ways that are difficult to
  diagnose from operator-facing output alone.

## 6) Evidence block (mandatory)

### 6.1 Raw buffer/storage definition involved

- **PROVEN:** `uint8_t Fluffer[72];` at `src/BatMan.cpp:59`.

### 6.2 Exact loop/index bounds creating overrun risk

- **PROVEN:** `for (count2 = 0; count2 <= 72; count2 = count2 + 2)` at
  `src/BatMan.cpp:391`.
- **PROVEN:** writes to `Fluffer[count2]` and `Fluffer[count2 + 1]` at
  `src/BatMan.cpp:394` and `src/BatMan.cpp:395`.
- **PROVEN:** final iteration at `count2 == 72` implies writes to
  `Fluffer[72]` and `Fluffer[73]` (out of bounds for length 72).

### 6.3 Parse/group path that consumes affected buffer

- **PROVEN:** `switch (ReqID)` parse entry at `src/BatMan.cpp:401` consumes
  `Fluffer` across grouped cases:
  - `0x47` cell group map (`src/BatMan.cpp:403` to `src/BatMan.cpp:419`)
  - `0x48` cell group map (`src/BatMan.cpp:422` to `src/BatMan.cpp:438`)
  - `0x49` cell group map (`src/BatMan.cpp:441` to `src/BatMan.cpp:457`)
  - `0x4A` cell group map (`src/BatMan.cpp:461` to `src/BatMan.cpp:477`)
  - `0x4B` cell group map (`src/BatMan.cpp:480` to `src/BatMan.cpp:496`)
  - `0x4C`, `0x4D`, `0x50` auxiliary/config parsing
    (`src/BatMan.cpp:499`, `src/BatMan.cpp:511`, `src/BatMan.cpp:542`).

### 6.4 Downstream publication path plausibly distorted by parser corruption

- **PROVEN:** parse output enters published parameter set in
  `upDateCellVolts()` (`src/BatMan.cpp:750`, `src/BatMan.cpp:778` to
  `src/BatMan.cpp:781`).
- **PROVEN:** published parameters are exposed via JSON print request path in
  `src/main.cpp:203` to `src/main.cpp:205`, with JSON command registration at
  `src/terminal_prj.cpp:47`.
- **PROVEN:** CAN publishing consumes derived extrema in
  `src/CAN_Common.cpp:151` to `src/CAN_Common.cpp:172`.
- **INFERRED:** memory corruption at ingest stage can leak into these
  publication paths as false freshness/truthfulness.

### 6.5 Nearby code that should remain untouched in the code slice

- **PROVEN (do-not-touch in next fix slice):** balancing command/write path in
  `BATMan::WriteCfg()` (`src/BatMan.cpp:569` to `src/BatMan.cpp:627`).
- **PROVEN (do-not-touch in next fix slice):** balancing policy and
  State of Charge (SOC)-adjacent pack metrics in `upDateCellVolts()`
  (`src/BatMan.cpp:700` to `src/BatMan.cpp:787`).
- **PROVEN (do-not-touch in next fix slice):** auxiliary temperature request
  flow `GetTempData()` (`src/BatMan.cpp:630` to `src/BatMan.cpp:655`).

## 7) Explicit out-of-scope list for Slice 3D-A and next bounded fix slice

- no balancing logic changes
- no State of Charge (SOC) or current-path changes
- no Controller Area Network (CAN) payload/identifier changes
- no schema/state-machine/authentication/over-the-air/logging changes
- no broad parser redesign or multi-defect refactor
- no changes to parameter naming/identifiers or field ordering

## 8) Bounded recommendation for next code slice

- **Recommended bounded fix target:** only harden the `GetData()` ingest bound
  so writes cannot exceed `Fluffer` storage, while preserving existing read
  group semantics (`0x47..0x50`) and existing publication contracts.
- **Recommended implementation constraint:** use a storage-derived bound
  (`sizeof(Fluffer)`) or equivalent constant-safe limit; do not alter group
  maps, balancing behavior, SOC path, or CAN contracts.
- **Recommended diagnostics scope (bounded):** if diagnostics are added, keep
  them ingest-local and non-contract-breaking (no payload/schema changes).

## 9) Hard-stop check

- Defect statement precision from repo evidence: **PASS**
- Next-step recommendation bounded to local defect (no broad redesign): **PASS**
- Multiple defect mixing avoided in this lock: **PASS**
- No balancing/SOC/CAN semantic change proposed: **PASS**

## Release honesty reminder

This lock does not claim balancing is fixed, SOC is accurate, or full 96-cell
behavior is hardware-validated.
