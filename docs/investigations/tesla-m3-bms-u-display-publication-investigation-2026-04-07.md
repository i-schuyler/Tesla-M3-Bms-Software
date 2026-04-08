# Tesla Model 3 BMS `u*` Display Publication Investigation (2026-04-07)

## Document Metadata

- Title: `u1..u96` display publication path investigation
- Version: 0.1
- Date: 2026-04-07
- Scope: Investigation-only (no firmware behavior changes)
- Status: Investigation artifact (non-canonical until promoted)

## 1) Purpose and scope

This slice traces how raw Model 3 chip/group cell data becomes displayed
`u1..u96` values, then evaluates why field behavior can present as:

- `u1..u12` moving during charge/discharge,
- `u13..u96` appearing steady at runtime,
- `u13..u96` appearing steady across reboots.

Non-goals in this slice:

- no firmware/code behavior changes
- no balancing logic changes
- no State of Charge (SOC) or current-path changes
- no Controller Area Network (CAN) contract changes
- no build/Continuous Integration (CI)/release changes

## 2) Input evidence context (session-provided)

External field observations provided for this investigation:

- always exactly `u1..u12` move,
- `u13..u96` do not move during runtime,
- `u13..u96` remain steady across reboots,
- latest PDF snapshot shows `CellsPresent = 96`, `CellGrp0Fresh..CellGrp4Fresh = 1`,
  `CellDataStaleWarn = 0`, and non-contiguous `CellGrp*First/Last` ranges,
- May 2025 PDF values for `u13..u96` differ from newer PDF values.

Evidence status for this section:

- These are treated as operator/field context for hypothesis testing.
- They are not repo-local proof by themselves.

## 3) Mandatory evidence block (repo-local)

### 3.1 `u1..u96` parameter surface definitions

- `u1..u96` display parameters are explicitly defined in
  `include/param_prj.h:100` through `include/param_prj.h:195`.
- `CellsPresent` is defined at `include/param_prj.h:99`.
- Freshness/diagnostic fields used in this investigation are defined at
  `include/param_prj.h:76` through `include/param_prj.h:91`.

### 3.2 Raw ingest path and per-chip/per-slot storage

- Raw receive buffer is `Fluffer[72]` at `src/BatMan.cpp:59`.
- Parsed cell cache is `Voltage[8][15]` at `src/BatMan.cpp:65`.
- Ingest entry point is `BATMan::GetData()` at `src/BatMan.cpp:371`.
- Request/response bytes are read into `Fluffer` in the transfer loop at
  `src/BatMan.cpp:391`.
- Group parse/write mapping into `Voltage[h][g]`:
  - `0x47 -> g=0..2` at `src/BatMan.cpp:403`, write at `src/BatMan.cpp:413`
  - `0x48 -> g=3..5` at `src/BatMan.cpp:422`, write at `src/BatMan.cpp:432`
  - `0x49 -> g=6..8` at `src/BatMan.cpp:441`, write at `src/BatMan.cpp:451`
  - `0x4A -> g=9..11` at `src/BatMan.cpp:461`, write at `src/BatMan.cpp:471`
  - `0x4B -> g=12..14` at `src/BatMan.cpp:480`, write at `src/BatMan.cpp:490`
- A `Voltage` slot is overwritten only when parsed word is not `0xffff`
  (for example at `src/BatMan.cpp:411`, `src/BatMan.cpp:430`,
  `src/BatMan.cpp:449`, `src/BatMan.cpp:469`, `src/BatMan.cpp:488`).

### 3.3 Exact runtime publication functions for displayed cells

- Runtime publication function is `BATMan::upDateCellVolts()` at
  `src/BatMan.cpp:678`.
- Display publication write is `Param::SetFloat((Param::PARAM_NUM)(Param::u1 + h),
  Voltage[Xr][Yc])` at `src/BatMan.cpp:759`.
- Publication validity gate is `if (Voltage[Xr][Yc] > 10)` at
  `src/BatMan.cpp:740`.
- Published-index counter is `h`, incremented at `src/BatMan.cpp:771`.
- `CellsPresent` is set from published count `h` at `src/BatMan.cpp:790`.

### 3.4 Loop/state gating that controls when publication happens

- Model 3 path executes `BATMan::loop()` from the 100 ms scheduler task in
  `src/main.cpp:68` through `src/main.cpp:70`.
- `StateMachine()` performs grouped reads in case 4/5:
  - `GetData(0x47..0x4A)` at `src/BatMan.cpp:263` through `src/BatMan.cpp:270`
  - `GetData(0x4B)` at `src/BatMan.cpp:283`
- Publication occurs in case 7 via `upDateCellVolts()` at
  `src/BatMan.cpp:317`.

### 3.5 Boot-time init vs runtime refresh path evidence

- Startup runs `parm_load()` in `main()` at `src/main.cpp:154`.
- Model 3 startup config calls `BATMan::BatStart()` at `src/main.cpp:186`.
- `BatStart()` sets `ChipNum` from `numbmbs*2` with clamp to 8 at
  `src/BatMan.cpp:193` through `src/BatMan.cpp:197`.
- In current repo, no separate boot-only `u*` publication function was found;
  runtime publish path is `upDateCellVolts()` called from state-machine case 7
  (`src/BatMan.cpp:317`, `src/BatMan.cpp:678`).

### 3.6 Evidence touching `CellGrp*First/Last` and freshness diagnostics

- Group freshness arrays are reset each cycle in case 4 at
  `src/BatMan.cpp:252` through `src/BatMan.cpp:255`.
- Group freshness latches are set per ingest command in `GetData()` at
  `src/BatMan.cpp:418`, `src/BatMan.cpp:437`, `src/BatMan.cpp:456`,
  `src/BatMan.cpp:476`, `src/BatMan.cpp:495`.
- Freshness parameters are published in case 7 at `src/BatMan.cpp:310` through
  `src/BatMan.cpp:314`.
- Group range diagnostics use `groupIdx = Yc / 3` and published index
  `publishedIndex = h + 1` at `src/BatMan.cpp:752` through `src/BatMan.cpp:753`,
  then store first/last at `src/BatMan.cpp:754` through `src/BatMan.cpp:758`.
- `CellDataStaleWarn` uses coarse order rule over group freshness at
  `src/BatMan.cpp:799` through `src/BatMan.cpp:809`, published at
  `src/BatMan.cpp:812`.

## 4) Source-to-display mapping table (repo-proven path)

| Raw source | Intermediate storage | Destination mapping | Freshness / gating involved |
| --- | --- | --- | --- |
| SPI response bytes in `GetData(ReqID)` (`src/BatMan.cpp:371`) | `Fluffer[72]` (`src/BatMan.cpp:59`, `src/BatMan.cpp:391`) | Parsed into `Voltage[h][g]` by ReqID group (`src/BatMan.cpp:403`, `src/BatMan.cpp:422`, `src/BatMan.cpp:441`, `src/BatMan.cpp:461`, `src/BatMan.cpp:480`) | Overwrite only when parsed word `!= 0xffff` (`src/BatMan.cpp:411`) |
| Parsed cell cache slots | `Voltage[8][15]` (`src/BatMan.cpp:65`) | Flattened valid-stream publication to `Param::u1 + h` (`src/BatMan.cpp:759`) | Publish only if `Voltage[Xr][Yc] > 10` (`src/BatMan.cpp:740`); no explicit clear of untouched `u*` slots in this function |
| Per-pass group ingest health | `CellGrpFresh[5]` (`src/BatMan.cpp:78`) | `CellGrp0Fresh..CellGrp4Fresh` params (`src/BatMan.cpp:310`) | Group is fresh if any valid word appears in that group (`src/BatMan.cpp:405`, `src/BatMan.cpp:418`) |
| Per-pass group contribution to flattened stream | `cellGrpFirst[5]`, `cellGrpLast[5]` (`src/BatMan.cpp:684`, `src/BatMan.cpp:685`) | `CellGrpNFirst/Last` params (`src/BatMan.cpp:813`) | Group index from slot band `Yc/3` and destination index from `h+1` (`src/BatMan.cpp:752`, `src/BatMan.cpp:753`) |

## 5) Findings by confidence class

### PROVEN

1. `u1..u96` publication for Model 3 is done in one runtime function:
   `BATMan::upDateCellVolts()` (`src/BatMan.cpp:678`) via `Param::u1 + h`
   writes (`src/BatMan.cpp:759`).
2. Destination index does not restart inside a pass: `h` starts at 0 once
   (`src/BatMan.cpp:683`) and increments on each published valid cell
   (`src/BatMan.cpp:771`).
3. Publication loop is not hard-limited to 12 cells in current code:
   per-chip slot traversal checks `Yc < 15` (`src/BatMan.cpp:738`) and advances
   chip index until `Xr == ChipNum` (`src/BatMan.cpp:785`).
4. Stale-cache replay path exists: `Voltage[h][g]` is not overwritten on
   `0xffff` parse words (`src/BatMan.cpp:411`), and publication reads from
   `Voltage` (`src/BatMan.cpp:740`, `src/BatMan.cpp:759`).
5. `CellGrp*Fresh` and `CellDataStaleWarn` are coarse group-level diagnostics,
   not per-chip/per-slot freshness guarantees (`src/BatMan.cpp:405`,
   `src/BatMan.cpp:799`).
6. `CellGrp*First/Last` are based on flattened published index (`h+1`) and group
   band (`Yc/3`), not physical cell identifiers (`src/BatMan.cpp:752`,
   `src/BatMan.cpp:753`).

### INFERRED

1. The field pattern (`u1..u12` dynamic; `u13..u96` static) is consistent with a
   **partial-refresh + stale-replay** mechanism: early stream entries refresh,
   later entries often reuse retained `Voltage[][]` values.
2. Snapshot state (`CellGrp0Fresh..4 = 1`, `CellDataStaleWarn = 0`) does not
   contradict that mechanism, because one valid word per group can mark group
   fresh even if many chip slots in that group are stale.
3. Non-contiguous `CellGrp*First/Last` ranges are expected from flattened
   validity-gated publication and do not, by themselves, prove a destination
   index bug.

### UNPROVEN

1. A hardware fault as primary root cause for this specific pattern.
2. A physical mapping error between harness cell order and `u*` numbering as the
   principal mechanism for “only first 12 move.”
3. Any boot-time-only publication path overriding runtime publication in current
   repository implementation.
4. Whether `parm_load()` restores display-value fields (`VALUE_ENTRY`) in a way
   that contributes to across-reboot persistence, because parameter framework
   internals are outside this repository snapshot.

## 6) Direct answers to requested investigation questions

1. **Where final displayed `u1..u96` values are written/published:**
   `BATMan::upDateCellVolts()` writes `Param::u1 + h` at `src/BatMan.cpp:759`.
2. **What raw/intermediate storage feeds them:**
   `Fluffer[72]` -> `Voltage[8][15]` in `GetData()` -> flattened runtime publish
   in `upDateCellVolts()`.
3. **Whether destination indexing/base offsets explain “only first 12 move”:**
   current code does not show destination index restart or a fixed 12-cell copy
   cap; this mechanism is not supported by repo evidence.
4. **Whether there is a split between boot/coarse and runtime live-update paths:**
   Model 3 publish path in repo is runtime state-machine-driven; no separate
   boot-only `u*` writer was found.
5. **Most likely bug mechanism from current evidence:**
   stale replay from `Voltage[][]` retained values combined with coarse group
   freshness diagnostics that can report all-fresh while many slots still reuse
   old data.

## 7) Why hardware fault is not the lead hypothesis

Hardware fault is not the lead hypothesis because software evidence already
proves a mechanism that can produce static-looking high-index `u*` values:

- parse path retains old cache on invalid words (`src/BatMan.cpp:411`),
- publish path reuses cache values (`src/BatMan.cpp:740`, `src/BatMan.cpp:759`),
- freshness indicators are coarse enough to miss per-chip/per-slot staleness
  (`src/BatMan.cpp:405`, `src/BatMan.cpp:799`).

This does not rule hardware out; it means hardware-first conclusion is not yet
justified from repository evidence plus the provided snapshots.

## 8) Most likely mechanism, strongest alternative, and bounded next slice

- **Most likely mechanism (INFERRED):** per-slot stale-cache replay in
  `Voltage[][]` plus insufficiently granular freshness signals.
- **Strongest competing alternative (UNPROVEN):** measurement-path/hardware issue
  causing widespread real stability while only first segment changes enough to
  be observed.
- **Smallest bounded next code slice (if approved):**
  - Keep scope to Model 3 publication path only.
  - Add per-chip/per-slot freshness age tracking for the same pass that writes
    `u*`.
  - Gate publication on current-pass freshness (or explicitly clear stale slots)
    so stale cache replay cannot masquerade as live updates.
  - Keep out-of-scope: balancing logic, SOC/current path, CAN contracts,
    Makefile/CI/release changes.

## 9) Stop-condition check

- No stop condition was triggered for this slice.
- Repo evidence was sufficient to trace the full in-repo publication path.
- Root-cause certainty remains `INFERRED` (not fully `PROVEN`) without runtime
  per-slot freshness capture.
