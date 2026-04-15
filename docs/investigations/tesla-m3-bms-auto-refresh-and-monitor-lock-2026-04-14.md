# Tesla Model 3 BMS Optional 5-Second Auto-Refresh + Near-Top Monitor Block Semantic/UI Lock (2026-04-14)

## Document Metadata

- Title: Optional auto-refresh and grouped monitor semantic/UI lock
- Version: 0.1
- Date: 2026-04-14
- Scope: Docs-first semantic/UI lock for bounded follow-up implementation
- Status: Investigation artifact (non-canonical until promoted)
- Session Anchor: `HL-2026-04-03 — optimize_bms`

## 1) Purpose and scope

This document locks the semantics and exact UI placement requirements for:

1. an optional persisted 5-second auto-refresh control, and
2. a grouped near-top diagnostic monitor block for targeted timed observation.

This slice is docs-only.

Explicit non-goals:

- no firmware/code behavior changes
- no balancing logic changes
- no State of Charge (SOC)/current-path changes
- no Controller Area Network (CAN)/reporting implementation changes
- no Continuous Integration (CI)/release changes
- no Makefile changes
- no silent redefinition of existing field meanings

## 2) Authorities consulted

1. Project canonical docs and authority order:
   - `AGENTS.md`
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
2. Existing field/ordering contracts:
   - `include/param_prj.h`
3. Existing terminal JSON publication path (used by web integration):
   - `src/terminal_prj.cpp`
   - `src/main.cpp`
4. Existing investigation/lock context for current cell diagnostics:
   - `docs/investigations/tesla-m3-bms-cell-age-diagnostics-lock-2026-04-07.md`
   - `docs/investigations/tesla-m3-bms-targeted-raw-debug-lock-2026-04-13.md`

## 3) Current UI/refresh behavior problem statement

For the next targeted testing phase, the operator must watch a bounded set of
values with timing awareness. Current interaction pain is:

- repeated manual refresh effort during timed observation, and
- monitor-relevant values spread across broader parameter surfaces.

This increases observation friction and causes avoidable up/down scrolling while
comparing values over time.

Evidence status:

- `PROVEN` from repo: firmware/terminal side exposes one JSON parameter output
  path (`json` command -> `PrintParamsJson`).
- `UNPROVEN` from repo: exact current browser UI implementation details,
  because UI source is not present in this repository.

## 4) Evidence block (repo-local)

### 4.1 Existing refresh/update data path surface

- Terminal command surface includes `json` mapped to
  `TerminalCommands::PrintParamsJson` in `src/terminal_prj.cpp:47`.
- Main-loop print request path serves JSON through
  `TerminalCommands::PrintParamsJson` in `src/main.cpp:203` and
  `src/main.cpp:205`.

### 4.2 Existing field contract surface for requested monitor values

- Unique-ID and ordering constraints are explicit in `include/param_prj.h:29`
  and `include/param_prj.h:44`.
- Current requested monitor fields already exist in the parameter surface:
  - `udc` at `include/param_prj.h:72`
  - `CellGrp0Fresh..CellGrp4Fresh` at `include/param_prj.h:76` through
    `include/param_prj.h:80`
  - `CellDataStaleWarn` at `include/param_prj.h:81`
  - `CellStaleCount` at `include/param_prj.h:82`
  - `CellMaxAge` at `include/param_prj.h:83`
  - `CellsPresent` at `include/param_prj.h:101`
  - `u1` at `include/param_prj.h:102`
  - `u12` at `include/param_prj.h:113`
  - `u13` at `include/param_prj.h:114`
  - `u86` at `include/param_prj.h:187`
  - `u96` at `include/param_prj.h:197`
  - `u1Age` at `include/param_prj.h:258`
  - `u12Age` at `include/param_prj.h:269`
  - `u13Age` at `include/param_prj.h:270`
  - `u86Age` at `include/param_prj.h:343`
  - `u96Age` at `include/param_prj.h:353`
  - `DbgU1Raw` at `include/param_prj.h:354`
  - `DbgU12Raw` at `include/param_prj.h:356`
  - `DbgU13Raw` at `include/param_prj.h:358`
  - `DbgU86Raw` at `include/param_prj.h:360`
  - `DbgU96Raw` at `include/param_prj.h:362`
  - `DbgU1Fresh` at `include/param_prj.h:355`
  - `DbgU12Fresh` at `include/param_prj.h:357`
  - `DbgU13Fresh` at `include/param_prj.h:359`
  - `DbgU86Fresh` at `include/param_prj.h:361`
  - `DbgU96Fresh` at `include/param_prj.h:363`

### 4.3 Existing append-only governance constraints

- Existing user-facing contract stability requirement is stated in `AGENTS.md`.
- Existing append-only decision governance is in
  `docs/Project_Decisions_Snapshot_v0_1.md`.

## 5) Exact semantics for optional 5-second auto-refresh (LOCKED)

### 5.1 Control type and text

- Control type is a UI checkbox/toggle.
- Label text is locked exactly as: `Auto-refresh every 5s`.

### 5.2 Interval and default

- Interval is locked to exactly 5 seconds (`5000 ms`).
- Default state is locked as OFF.

### 5.3 Persistence semantics

- Auto-refresh ON/OFF selection must persist across browser page
  refresh/reload.
- Persistence scope is local browser storage on the viewing client
  (for example, local storage); cross-device sync is not required.

### 5.4 Data-path consistency lock

- Auto-refresh must call/trigger the same existing refresh/update path used for
  current manual parameter refresh in that UI.
- Auto-refresh must not introduce a second, divergent data-acquisition path.
- Firmware-side semantics remain unchanged; this lock does not define or permit
  new firmware publication contracts.

### 5.5 Edit-state interaction lock

- Preferred behavior (`SHOULD`): suppress/disable automatic refresh while an
  operator is actively editing a field, then resume after edit completion.
- Narrow safe fallback wording (locked due current repo evidence limits):
  implement edit-state suppression where cleanly supported by existing UI
  structure; if not cleanly detectable, keep default OFF and do not expand
  auto-refresh scope beyond read/display refresh.

## 6) Exact semantics for grouped monitor block (LOCKED)

### 6.1 Block purpose

- Create one bounded monitor block for timed observation of targeted cells,
  freshness, age, and pack-level context.
- This is a read/visibility grouping layer and does not redefine any underlying
  field meaning.

### 6.2 Exact field set to include now

The block must include exactly these current fields:

1. `u1`
2. `u12`
3. `u13`
4. `u86`
5. `u96`
6. `u1Age`
7. `u12Age`
8. `u13Age`
9. `u86Age`
10. `u96Age`
11. `DbgU1Raw`
12. `DbgU12Raw`
13. `DbgU13Raw`
14. `DbgU86Raw`
15. `DbgU96Raw`
16. `DbgU1Fresh`
17. `DbgU12Fresh`
18. `DbgU13Fresh`
19. `DbgU86Fresh`
20. `DbgU96Fresh`
21. `CellStaleCount`
22. `CellMaxAge`
23. `CellGrp0Fresh`
24. `CellGrp1Fresh`
25. `CellGrp2Fresh`
26. `CellGrp3Fresh`
27. `CellGrp4Fresh`
28. `CellDataStaleWarn`
29. `CellsPresent`
30. `udc`

### 6.3 Future-extension join rule for same target cells

When added in later slices, these fields must join the same monitor block:

- `DbgU1SrcChip`, `DbgU1SrcSlot`, `DbgU1UpdCnt`
- `DbgU12SrcChip`, `DbgU12SrcSlot`, `DbgU12UpdCnt`
- `DbgU13SrcChip`, `DbgU13SrcSlot`, `DbgU13UpdCnt`
- `DbgU86SrcChip`, `DbgU86SrcSlot`, `DbgU86UpdCnt`
- `DbgU96SrcChip`, `DbgU96SrcSlot`, `DbgU96UpdCnt`

## 7) Exact placement lock for both controls and monitor block (LOCKED)

### 7.1 Near-top placement requirement

- The grouped diagnostic monitor block must be rendered at or near the top of
  the page so targeted timed observation does not require repeated scrolling.

### 7.2 Relative placement requirement

- Place the `Auto-refresh every 5s` checkbox/toggle in the same near-top
  monitor region.
- Place the grouped monitor block above the broader/global full parameter list.

### 7.3 Full-list contract preservation

- Do not reorder the underlying full global parameter list as part of this
  monitor grouping.
- The full parameter list remains available and semantically unchanged.
- The near-top monitor block is an additional grouped view, not a replacement
  contract for full-list ordering.

## 8) What remains unchanged in the underlying full parameter list (LOCKED)

- Existing field meanings remain unchanged for all listed monitor fields.
- Existing parameter identifiers and IDs remain unchanged.
- Existing firmware publication path and CAN/reporting contracts remain
  unchanged in this docs-only slice.
- No balancing, SOC/current-path, CI/release, or Makefile behavior changes are
  approved by this lock.

## 9) Implementation direction for follow-up slices

1. Implement UI-only auto-refresh control with locked label, default OFF,
   5-second interval, and persisted local state.
2. Wire auto-refresh through the existing refresh path currently used for manual
   refresh in the UI.
3. Add one near-top grouped diagnostic monitor block containing exactly the
   locked field set.
4. Preserve existing full global parameter list order and semantics.
5. Add future source/slot/update-counter fields for targeted cells into the same
   grouped monitor block when those fields are introduced.

## 10) Unknowns / proof gaps

1. Current browser UI implementation details (component layout/edit-state hooks)
   are not directly evidenced in this repository snapshot.
2. Exact existing browser-side refresh trigger implementation is not directly
   evidenced in this repository snapshot.
3. Runtime operator ergonomics and edit-state suppression quality require
   implementation-phase validation.

## 11) PROVEN / INFERRED / UNPROVEN

### PROVEN

1. Existing firmware-side JSON publication command path is present and line
   addressable (`src/terminal_prj.cpp:47`, `src/main.cpp:203`,
   `src/main.cpp:205`).
2. Requested monitor fields already exist in the parameter contract surface
   (`include/param_prj.h`, including lines cited in Section 4.2).
3. Parameter identity/order stability constraints are explicit in
   `include/param_prj.h:29` and `include/param_prj.h:44`.

### INFERRED

1. A near-top grouped monitor block reduces timed-observation friction versus
   dispersed full-list scanning.
2. Reusing the existing refresh path for auto-refresh minimizes divergence risk
   compared with introducing a second path.

### UNPROVEN

1. The exact current browser UI code path and edit-state hooks are not proven
   from this repository alone.
2. Hardware/runtime validation of timed-observation workflow improvements is not
   part of this docs-only lock slice.

## 12) Verification plan (docs-only)

- Verify this lock includes all required fields and lock clauses.
- Verify no firmware/source behavior files are edited in this slice.
- Verify decision snapshot is updated append-only to promote this lock.
- Verify canonical index update is skipped unless discoverability requires it.
