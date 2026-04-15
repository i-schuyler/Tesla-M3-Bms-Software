# Tesla Model 3 BMS Firmware Diagnostics Block Lock (2026-04-15)

## Document Metadata

- Title: Firmware-side grouped diagnostics block and source/counter lock
- Version: 0.2
- Date: 2026-04-15
- Scope: Docs-first lock for a bounded firmware diagnostics follow-up slice
- Status: Investigation artifact (non-canonical until promoted)
- Session Anchor: `HL-2026-04-03 — optimize_bms`

## 1) Purpose and scope

This document locks a firmware-only diagnostics parameter grouping plan so
follow-up implementation can proceed without ambiguity.

This slice is docs-only.

Explicit scope lock:

- no user interface (UI)/browser work in this repository
- firmware-side diagnostics parameter ordering and semantics only

Explicit non-goals:

- no firmware behavior changes in this slice
- no balancing logic changes
- no State of Charge (SOC)/current-path changes
- no Controller Area Network (CAN)/reporting implementation changes
- no Continuous Integration (CI)/release/Makefile changes
- no silent redefinition of existing field meanings

## 2) Authorities consulted

1. `00_Identity` (session authority order)
2. Canonical project docs:
   - `AGENTS.md`
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Existing implementation/order surfaces:
   - `include/param_prj.h`
   - `src/BatMan.cpp`
4. Current release-train and lock context:
   - `docs/CHANGELOG.md`
   - `docs/investigations/tesla-m3-bms-targeted-raw-debug-lock-2026-04-13.md`

## 3) Evidence block (mandatory)

### 3.1 Current parameter-ordering constraints (`PARAM_LIST`)

- `PARAM_LIST` ordering contract is explicit in `include/param_prj.h:44` through
  `include/param_prj.h:48`.
- Value identifier continuity and add-only expectations are explicit at
  `include/param_prj.h:29` through `include/param_prj.h:39`.
- Current next-value marker is `2301` at `include/param_prj.h:50`.

### 3.2 Existing legacy fields to be mirrored in grouped diagnostics

- Existing source fields for mirror aliases exist now:
  - `udc` at `include/param_prj.h:72`
  - `u1`, `u12`, `u13`, `u86`, `u96` at `include/param_prj.h:102`,
    `include/param_prj.h:113`, `include/param_prj.h:114`,
    `include/param_prj.h:187`, `include/param_prj.h:197`

### 3.3 Existing newer diagnostics fields already in repository

- Existing age/stale diagnostics are present:
  - `CellStaleCount` and `CellMaxAge` at `include/param_prj.h:82` through
    `include/param_prj.h:83`
  - `u1Age..u96Age` at `include/param_prj.h:258` through
    `include/param_prj.h:353`
- Existing targeted raw/fresh diagnostics are present:
  - `DbgU1Raw..DbgU96Fresh` at `include/param_prj.h:354` through
    `include/param_prj.h:363`
- Existing freshness/warn/presence diagnostics are present:
  - `CellGrp0Fresh..CellGrp4Fresh` at `include/param_prj.h:76` through
    `include/param_prj.h:80`
  - `CellDataStaleWarn` at `include/param_prj.h:81`
  - `CellsPresent` at `include/param_prj.h:101`
- Current release history for these newer diagnostics is documented in
  `docs/CHANGELOG.md:96` through `docs/CHANGELOG.md:104` and
  `docs/CHANGELOG.md:54` through `docs/CHANGELOG.md:70`.
- **Correction lock:** `CellsPresent`, `CellGrp0Fresh..CellGrp4Fresh`, and
  `CellDataStaleWarn` are grouped directly using existing names and are **not**
  duplicated as new `Diag*` mirror fields.

### 3.4 Current targeted raw debug lock

- Targeted raw/fresh semantics and prior placement lock are documented in
  `docs/investigations/tesla-m3-bms-targeted-raw-debug-lock-2026-04-13.md:14`
  through
  `docs/investigations/tesla-m3-bms-targeted-raw-debug-lock-2026-04-13.md:15`
  and
  `docs/investigations/tesla-m3-bms-targeted-raw-debug-lock-2026-04-13.md:175`
  through
  `docs/investigations/tesla-m3-bms-targeted-raw-debug-lock-2026-04-13.md:190`.

### 3.5 Existing docs/decisions constraining append-only behavior and boundary

- Repository scope is firmware-focused in `AGENTS.md:11` through `AGENTS.md:12`
  and `README.md:5` through `README.md:9`.
- Canonical append-only decision governance is explicit in
  `docs/Project_Decisions_Snapshot_v0_1.md:12`,
  `docs/Project_Decisions_Snapshot_v0_1.md:23`, and
  `docs/Project_Decisions_Snapshot_v0_1.md:464` through
  `docs/Project_Decisions_Snapshot_v0_1.md:467`.
- Investigation-vs-canonical promotion rule is explicit in
  `docs/Canonical_Documentation_Index_v1_0.md:49` through
  `docs/Canonical_Documentation_Index_v1_0.md:57`.
- External front-end catalog is not established in this repo reality scan
  (`UNPROVEN`) at
  `docs/investigations/tesla-m3-bms-repo-reality-scan-2026-04-04.md:84` through
  `docs/investigations/tesla-m3-bms-repo-reality-scan-2026-04-04.md:92`.

## 4) Locked grouped diagnostics block design

### 4.0 Correction applied in this revision

- Prior revision text incorrectly treated these as new mirrored `Diag*` params:
  `DiagCellsPresent`, `DiagCellGrp0Fresh`, `DiagCellGrp1Fresh`,
  `DiagCellGrp2Fresh`, `DiagCellGrp3Fresh`, `DiagCellGrp4Fresh`, and
  `DiagCellDataStaleWarn`.
- This corrected revision locks those surfaces as existing newer diagnostics that
  are grouped directly by existing names: `CellsPresent`,
  `CellGrp0Fresh..CellGrp4Fresh`, and `CellDataStaleWarn`.
- Only true legacy surfaces remain mirrored by new `Diag*` aliases in Section
  4.2.

### 4.1 Placement lock (for follow-up implementation)

- Add one contiguous grouped diagnostics block near the top of `PARAM_LIST` for
  coworker testing visibility.
- Place this block immediately after `AsDiffTrace` and before existing
  `CellGrp0Fresh` declaration location in `include/param_prj.h`.
- Preserve existing pre-pre1 ordering dependencies by **not moving** legacy
  originals; use `Diag*` mirrors for those legacy surfaces.

### 4.2 Field class A — Mirrored legacy fields (new mirror aliases)

The following are new `Diag*` mirror fields and do not replace/move originals:

- `DiagUdc`
- `DiagU1`
- `DiagU12`
- `DiagU13`
- `DiagU86`
- `DiagU96`

### 4.3 Field class B — Moved newer diagnostics fields

Move existing newer diagnostics fields into the grouped block (do not duplicate):

- `u1Age`
- `u12Age`
- `u13Age`
- `u86Age`
- `u96Age`
- `DbgU1Raw`
- `DbgU12Raw`
- `DbgU13Raw`
- `DbgU86Raw`
- `DbgU96Raw`
- `DbgU1Fresh`
- `DbgU12Fresh`
- `DbgU13Fresh`
- `DbgU86Fresh`
- `DbgU96Fresh`
- `CellStaleCount`
- `CellMaxAge`
- `CellGrp0Fresh`
- `CellGrp1Fresh`
- `CellGrp2Fresh`
- `CellGrp3Fresh`
- `CellGrp4Fresh`
- `CellDataStaleWarn`
- `CellsPresent`

### 4.4 Field class C — Newly added source/slot/update-counter fields

Add these new fields in the grouped block:

- `DbgU1SrcChip`
- `DbgU1SrcSlot`
- `DbgU1UpdCnt`
- `DbgU12SrcChip`
- `DbgU12SrcSlot`
- `DbgU12UpdCnt`
- `DbgU13SrcChip`
- `DbgU13SrcSlot`
- `DbgU13UpdCnt`
- `DbgU86SrcChip`
- `DbgU86SrcSlot`
- `DbgU86UpdCnt`
- `DbgU96SrcChip`
- `DbgU96SrcSlot`
- `DbgU96UpdCnt`

## 5) Exact block ordering lock (for testing usability)

The grouped diagnostics block order is locked exactly as:

1. `DiagUdc`
2. `DiagU1`
3. `u1Age`
4. `DbgU1Raw`
5. `DbgU1Fresh`
6. `DbgU1SrcChip`
7. `DbgU1SrcSlot`
8. `DbgU1UpdCnt`
9. `DiagU12`
10. `u12Age`
11. `DbgU12Raw`
12. `DbgU12Fresh`
13. `DbgU12SrcChip`
14. `DbgU12SrcSlot`
15. `DbgU12UpdCnt`
16. `DiagU13`
17. `u13Age`
18. `DbgU13Raw`
19. `DbgU13Fresh`
20. `DbgU13SrcChip`
21. `DbgU13SrcSlot`
22. `DbgU13UpdCnt`
23. `DiagU86`
24. `u86Age`
25. `DbgU86Raw`
26. `DbgU86Fresh`
27. `DbgU86SrcChip`
28. `DbgU86SrcSlot`
29. `DbgU86UpdCnt`
30. `DiagU96`
31. `u96Age`
32. `DbgU96Raw`
33. `DbgU96Fresh`
34. `DbgU96SrcChip`
35. `DbgU96SrcSlot`
36. `DbgU96UpdCnt`
37. `CellStaleCount`
38. `CellMaxAge`
39. `CellGrp0Fresh`
40. `CellGrp1Fresh`
41. `CellGrp2Fresh`
42. `CellGrp3Fresh`
43. `CellGrp4Fresh`
44. `CellDataStaleWarn`
45. `CellsPresent`

## 6) Semantic lock requirements

### 6.1 Mirror/move invariants

- Mirrored legacy fields preserve original source meaning exactly and are added
  only to support grouped diagnostics visibility without disturbing older
  ordering dependencies.
- Moved newer diagnostics preserve their existing meaning exactly.
- `CellsPresent`, `CellGrp0Fresh..CellGrp4Fresh`, and `CellDataStaleWarn` are
  moved/grouped using existing names and are not mirrored as new `Diag*` fields.
- All new fields in this lock remain numeric and machine-readable.
- No browser/UI semantics are introduced in this firmware repository lock.

### 6.2 New source/slot/counter semantics

For each targeted displayed index in `{1, 12, 13, 86, 96}`:

- `DbgU*SrcChip` = chip index of the mapped internal source feeding the
  displayed cell in the current pass.
- `DbgU*SrcSlot` = slot index of the mapped internal source feeding the
  displayed cell in the current pass.
- `DbgU*UpdCnt` = runtime-local monotonic count of accepted current-pass updates
  observed for that mapped source path.

Reference implementation surfaces supporting this lock context:

- Accepted mapped-source updates are already gated by `tempvol != 0xffff` in
  `src/BatMan.cpp:427` through `src/BatMan.cpp:430` and similarly in the other
  read-group handlers (`src/BatMan.cpp:447` through `src/BatMan.cpp:511`).
- Flattened publication path and mapped-source usage are visible at
  `src/BatMan.cpp:769` through `src/BatMan.cpp:823`.

## 7) Follow-up implementation direction (locked)

1. Firmware follow-up slice remains firmware-side only; no UI/browser tasks in
   this repository.
2. Add the 6 mirrored `Diag*` fields from Section 4.2; keep original source
   fields where they currently are.
3. Move the 24 newer diagnostics fields from Section 4.3 into the grouped block
   (no duplication).
4. Add the 15 new source/slot/update-counter fields from Section 4.4.
5. Implement exact ordering from Section 5.
6. Keep cleanup/removal easy by maintaining contiguous grouped diagnostics and
   preserving non-diagnostics legacy declarations.

## 8) Unknowns and proof gaps

- Hardware validation of the new source/slot/update-counter fields is not part
  of this docs-only slice.
- Counter width and wrap behavior for `DbgU*UpdCnt` require explicit selection in
  implementation (`UNPROVEN` in this document).
- External UI/front-end presentation behavior remains outside this repo lock and
  is not claimed here.

## 9) PROVEN / INFERRED / UNPROVEN

### PROVEN

1. Existing parameter ordering and identifier constraints are explicit in
   `include/param_prj.h:29` through `include/param_prj.h:50`.
2. Existing legacy source fields and newer diagnostics fields already exist in
   `include/param_prj.h:72` through `include/param_prj.h:363`.
3. Mapped-source write path and accepted-update gating exist in `src/BatMan.cpp`
   (`src/BatMan.cpp:427`, `src/BatMan.cpp:795`, `src/BatMan.cpp:800` through
   `src/BatMan.cpp:818`).
4. Append-only canonical decision governance exists in
   `docs/Project_Decisions_Snapshot_v0_1.md:23` and
   `docs/Project_Decisions_Snapshot_v0_1.md:464` through
   `docs/Project_Decisions_Snapshot_v0_1.md:467`.

### INFERRED

1. Grouping diagnostics near the top of `PARAM_LIST` improves coworker testing
   visibility while preserving legacy source declarations.
2. Moving newer diagnostics (instead of duplicating) minimizes long-term cleanup
   burden when this temporary testing block is retired.

### UNPROVEN

1. Hardware/runtime quality of newly added source/slot/update-counter values is
   not validated by this docs-only lock.
2. Exact front-end rendering behavior for grouped diagnostics remains out of
   scope for this repository and is not asserted.
