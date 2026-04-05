# Tesla Model 3 BMS Live Cell-Update Path Investigation (2026-04-04)

## Document Metadata

- Title: Live cell-update / measurement-path investigation
- Version: 0.1
- Date: 2026-04-04
- Scope: Slice 3B docs-first investigation only
- Status: Investigation artifact (non-canonical until promoted)

## 1) Purpose and scope

This document investigates a reported runtime pattern where only `u1..u12` appear
to move with charge/discharge while later cell values appear broadly static.

This slice is docs-only. It does not change firmware behavior.

Explicit non-goals in this slice:

- no firmware/code changes
- no balancing logic changes
- no State of Charge (SOC) or current-path changes
- no Controller Area Network (CAN) payload/contract changes
- no Continuous Integration (CI) or release-process changes

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical docs:
   - `AGENTS.md`
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Implementation evidence:
   - `src/BatMan.cpp`
   - `include/BatMan.h`
   - `src/main.cpp`
   - `include/param_prj.h`

## 3) Field evidence being investigated

Session-provided observations:

- `u86` reportedly stuck at `3616` for weeks.
- `u13..u96` remain broadly in the `3600` range.
- Only `u1..u12` visibly rise/fall with charge/discharge.

Evidence status note:

- These field observations are treated as operator evidence for investigation
  framing.
- They are not, by themselves, a hardware-fault proof.

## 4) Repo-local measurement-path evidence

### 4.1 `GetData()` ingestion path across read groups

- Model 3 read flow calls `GetData()` in grouped sequence after snapshot:
  `0x47`, `0x48`, `0x49`, `0x4A`, then `0x4B`
  (`src/BatMan.cpp:257`, `src/BatMan.cpp:259`, `src/BatMan.cpp:261`,
  `src/BatMan.cpp:263`, `src/BatMan.cpp:277`).
- `GetData()` reads request response bytes into `Fluffer` via a fixed loop
  (`src/BatMan.cpp:380`).
- Group parse mapping is fixed by `ReqID`:
  - `0x47 -> g=0..2` (`src/BatMan.cpp:392`)
  - `0x48 -> g=3..5` (`src/BatMan.cpp:406`)
  - `0x49 -> g=6..8` (`src/BatMan.cpp:420`)
  - `0x4A -> g=9..11` (`src/BatMan.cpp:435`)
  - `0x4B -> g=12..14` (`src/BatMan.cpp:449`)

### 4.2 Chip/group/tag handling

- Chip traversal is fixed-width in parse loops (`h < 8`) and bounded in runtime
  publication by `ChipNum` (clamped to `8`) (`src/BatMan.cpp:393`,
  `src/BatMan.cpp:450`, `src/BatMan.cpp:192`).
- Group handling is explicit via register IDs and slot ranges (`g` bands above).
- Response parsing uses positional offsets (`h * 9`, `g * 2`) without explicit
  per-frame/per-chip tag validation in this path (`src/BatMan.cpp:397`,
  `src/BatMan.cpp:411`, `src/BatMan.cpp:425`, `src/BatMan.cpp:440`,
  `src/BatMan.cpp:454`).

### 4.3 Storage arrays for raw cell data

- Raw receive buffer: `Fluffer[72]` (`src/BatMan.cpp:59`).
- Parsed cell cache: `Voltage[8][15]` (`src/BatMan.cpp:65`).
- Chip voltage and aux caches: `ChipV[8]`, `Temp1[8]`, `Temp2[8]`, `Volts5v[8]`
  (`src/BatMan.cpp:80`, `src/BatMan.cpp:82`, `src/BatMan.cpp:83`).

### 4.4 `upDateCellVolts()` propagation into `u1..u96`

- Publication traverses `Voltage[Xr][Yc]` and writes to contiguous `u` values
  through `Param::u1 + h` (`src/BatMan.cpp:698`, `src/BatMan.cpp:714`).
- `h` increments only when a value is considered present (`Voltage > 10`), so
  mapping is "next valid measurement" rather than fixed absolute slot index
  (`src/BatMan.cpp:702`, `src/BatMan.cpp:726`).
- Cells-present and per-chip counts are derived from the same traversal
  (`src/BatMan.cpp:734`, `src/BatMan.cpp:745`).

### 4.5 Short (23-cell) vs long (25-cell) module handling

- Source/header comments explicitly state 23-cell short and 25-cell long module
  behavior (`src/BatMan.cpp:5`, `include/BatMan.h:5`).
- Runtime handling appears implicit: values are considered present only if valid
  (`>10`) in `upDateCellVolts()` (`src/BatMan.cpp:702`) rather than an explicit
  per-module-type map.

### 4.6 Where stale values can persist

- Parsed `Voltage[h][g]` entries are only overwritten when parsed word is not
  `0xffff`; otherwise old cache values remain (`src/BatMan.cpp:398`,
  `src/BatMan.cpp:412`, `src/BatMan.cpp:426`, `src/BatMan.cpp:441`,
  `src/BatMan.cpp:455`).
- `u*` publication is only written for entries passing `Voltage > 10`
  (`src/BatMan.cpp:702`, `src/BatMan.cpp:714`); there is no explicit clear of
  untouched `u*` slots in this function.
- The ingest loop reads with `count2 <= 72` while `Fluffer` is length `72`,
  creating an out-of-bounds write risk at tail indices
  (`src/BatMan.cpp:59`, `src/BatMan.cpp:380`, `src/BatMan.cpp:384`).

## 5) Candidate causes classification

### PROVEN

1. **Stale cache retention path exists in software**:
   `Voltage[][]` is not refreshed when parser sees `0xffff`; prior value is
   retained (`src/BatMan.cpp:398`, `src/BatMan.cpp:412`, `src/BatMan.cpp:426`,
   `src/BatMan.cpp:441`, `src/BatMan.cpp:455`).
2. **Published `u*` values are write-on-valid only**:
   `u1 + h` updates happen only when `Voltage > 10` (`src/BatMan.cpp:702`,
   `src/BatMan.cpp:714`), with no explicit stale-slot reset in this function.
3. **Flattened mapping favors earliest valid stream entries**:
   sequential `h` only advances on valid measurements (`src/BatMan.cpp:726`),
   so visible movement can concentrate at low-index `u*` if later reads are not
   refreshing.
4. **Raw ingest has a proven bounds defect risk**:
   fixed buffer `Fluffer[72]` with `count2 <= 72` loop bound can overrun tail
   bytes (`src/BatMan.cpp:59`, `src/BatMan.cpp:380`, `src/BatMan.cpp:384`).

### INFERRED

1. **Pattern match to partial group freshness**:
   if only early register groups are receiving fresh values while later groups
   intermittently fail/skip, observed "`u1..u12` moves, later cells static"
   behavior is consistent with the current write-on-valid + stale-retention
   design.
2. **No explicit response-tag validation increases ambiguity**:
   fixed-offset parsing without explicit tag/freshness checks can mask whether
   data is new, delayed, or stale in mixed-quality comms conditions.

### UNPROVEN

1. A confirmed physical cell/module hardware fault as the primary cause.
2. A single isolated chip failure being the only explanation.
3. A definitive one-to-one mapping from the observed `u86` symptom to a single
   board/channel fault without additional runtime freshness evidence.

## 6) Why this does or does not prove a hardware issue

Current repo evidence does **not** isolate a hardware-only explanation.

Reason: the software path demonstrably allows stale values to persist in both
raw cache (`Voltage[][]`) and published `u*` outputs when reads are skipped or
invalid (`0xffff`) and when values fail validity gating (`>10`). That software
behavior can reproduce a "some values live, many values static" presentation
without requiring immediate hardware-fault conclusion.

Therefore, hardware fault is `UNPROVEN` from repo inspection plus the supplied
field observations.

## 7) Bounded next-step recommendation

Recommendation: **instrumentation first** (smallest correct next action).

Bounded next action for follow-up slice:

1. Add per-cycle freshness instrumentation for each read group (`0x47..0x4B`):
   count of updated cell words and a loop-age marker for last successful update.
2. Add a compact stale-age indicator for published `u*` ranges so operators can
   distinguish "unchanged because stable" from "unchanged because not refreshed."
3. Only after freshness instrumentation confirms persistent no-refresh behavior
   in specific chips/groups should hardware-side checks be prioritized.

This keeps next work narrow and evidence-driven while avoiding premature
hardware-fault claims.

## 8) What to tell operators right now

- Current evidence does not confirm a hardware fault.
- The observed pattern is consistent with a software-side stale-refresh path.
- Treat high-index static `u*` readings as potentially stale until freshness
  instrumentation confirms update cadence across all groups.

