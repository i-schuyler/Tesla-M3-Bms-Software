# Tesla Model 3 BMS Boot Path Cell Cache Investigation (2026-04-16)

## Document Metadata

- Title: Boot-time initialization and restore-path investigation for cell cache and displayed cell values
- Version: 0.1
- Date: 2026-04-16
- Session Anchor: HL-2026-04-03 — optimize_bms
- Scope: Docs-only investigation (no firmware behavior changes)
- Status: Investigation artifact (non-canonical until explicitly promoted)

## Question Being Answered

From repository evidence, what happens on boot for:

- `Voltage[8][15]`
- `VoltageFresh[8][15]`
- displayed `u1..u96`
- `u1Age..u96Age`, `CellStaleCount`, and `CellMaxAge`

And can any boot/load/restore path make cell values appear persistent across reboot?

## Mandatory Evidence Block

1. `Voltage[8][15]` definition and declaration-time initialization:
   - `src/BatMan.cpp:65`
   - explicit zero-filled initializer rows at `src/BatMan.cpp:67`
2. `VoltageFresh[8][15]` definition and declaration-time initialization:
   - `src/BatMan.cpp:79`
3. Startup/init/BatStart/state-machine reset paths:
   - `BATMan::BatStart()` entry at `src/BatMan.cpp:232`
   - `BatStart()` resets `CellLastRefreshSec` and `CellSeenOnce` only at `src/BatMan.cpp:242` and `src/BatMan.cpp:243`
   - state-machine per-cycle reset of `VoltageFresh` at `src/BatMan.cpp:307`
   - no equivalent runtime clear of `Voltage` found; parser writes occur at `src/BatMan.cpp:467`, `src/BatMan.cpp:487`, `src/BatMan.cpp:507`, `src/BatMan.cpp:528`, `src/BatMan.cpp:548`
4. `parm_load()` / parameter restore/load surfaces:
   - startup callsite: `src/main.cpp:154`
   - flash-load intent language in parameter manifest comments: `include/param_prj.h:29`, `include/param_prj.h:30`, `include/param_prj.h:31`, `include/param_prj.h:34`
   - parameter save/load implementation appears framework-scoped (submodule/build wiring): `.gitmodules:1`, `.gitmodules:2`, `.gitmodules:3`, `Makefile:31`, `Makefile:34`, `Makefile:41`, `Makefile:46`, `Makefile:47`
5. Publication paths for `u1..u96`:
   - Model 3 runtime path enters from scheduler task: `src/main.cpp:60`, `src/main.cpp:68`, `src/main.cpp:70`
   - publication function call from state machine case 7: `src/BatMan.cpp:371`
   - publish write to flattened display fields: `src/BatMan.cpp:853`
6. Publication paths for `u1Age..u96Age`, `CellStaleCount`, `CellMaxAge`:
   - age-parameter mapping helper: `src/BatMan.cpp:88`, `src/BatMan.cpp:92`, `src/BatMan.cpp:118`
   - age write for all 96 cells: `src/BatMan.cpp:1005`
   - stale counters write: `src/BatMan.cpp:1017`, `src/BatMan.cpp:1018`

## Evidence Summary

- `Voltage` and `VoltageFresh` are both source-level zero-initialized globals (`src/BatMan.cpp:65`, `src/BatMan.cpp:79`).
- `BatStart()` does not clear `Voltage` or `VoltageFresh`; it initializes age-tracking arrays (`src/BatMan.cpp:242`, `src/BatMan.cpp:243`).
- `VoltageFresh` is cleared each acquisition cycle (state 4) before reads (`src/BatMan.cpp:307`), but `Voltage` is only overwritten when parsed words are valid (`!= 0xffff`) (`src/BatMan.cpp:465`, `src/BatMan.cpp:467`).
- Displayed cells are written by runtime `upDateCellVolts()` in state 7 (`src/BatMan.cpp:371`, `src/BatMan.cpp:853`); no separate Model 3 boot-only `u1..u96` publisher is visible in this repo snapshot.
- Parameter restore is invoked very early (`src/main.cpp:154`), before `BatStart()` (`src/main.cpp:186`), but restore internals are not present in this repository snapshot as first-party source.

## Boot Path Walk-Through

1. `main()` executes startup hardware setup, then calls `parm_load()` (`src/main.cpp:154`).
2. For Model 3 mode, `BATMan::BatStart()` runs (`src/main.cpp:184`, `src/main.cpp:186`).
3. `BatStart()` sets `ChipNum` and resets age trackers (`CellLastRefreshSec`, `CellSeenOnce`) (`src/BatMan.cpp:234`, `src/BatMan.cpp:242`, `src/BatMan.cpp:243`).
4. `BatStart()` does not explicitly clear `Voltage` or `VoltageFresh`.
5. Runtime scheduling then drives `BATMan::loop()` every 100 ms (`src/main.cpp:60`, `src/main.cpp:70`).
6. In state machine case 4, `VoltageFresh` is reset to zero before group reads (`src/BatMan.cpp:299`, `src/BatMan.cpp:307`).
7. In case 7, publication functions run (`src/BatMan.cpp:370`, `src/BatMan.cpp:371`).

## Runtime Path Walk-Through

1. `GetData(0x47..0x4B)` parses grouped cell words and writes `Voltage[h][g]` only when parsed value is not `0xffff` (`src/BatMan.cpp:455`, `src/BatMan.cpp:465`, `src/BatMan.cpp:467`, `src/BatMan.cpp:546`, `src/BatMan.cpp:548`).
2. When a slot is updated, matching `VoltageFresh[h][g]` is set to `1` (`src/BatMan.cpp:468`, `src/BatMan.cpp:488`, `src/BatMan.cpp:549`).
3. `upDateCellVolts()` publishes `u1..u96` via `Param::SetFloat(Param::u1 + h, mappedVoltage)` (`src/BatMan.cpp:853`).
4. For fresh slots, `CellLastRefreshSec[h]` and `CellSeenOnce[h]` are updated (`src/BatMan.cpp:921`, `src/BatMan.cpp:923`, `src/BatMan.cpp:924`).
5. Age diagnostics are recomputed and written every publication pass (`src/BatMan.cpp:992`, `src/BatMan.cpp:1005`, `src/BatMan.cpp:1017`, `src/BatMan.cpp:1018`).

## PROVEN / INFERRED / UNPROVEN

### PROVEN

1. `Voltage[8][15]` and `VoltageFresh[8][15]` are explicitly declared and initialized to zero in source (`src/BatMan.cpp:65`, `src/BatMan.cpp:67`, `src/BatMan.cpp:79`).
2. `BatStart()` explicitly initializes age trackers but does not include explicit clears for `Voltage`/`VoltageFresh` (`src/BatMan.cpp:240`, `src/BatMan.cpp:242`, `src/BatMan.cpp:243`).
3. `VoltageFresh` is explicitly reset during runtime state-machine cycle setup (`src/BatMan.cpp:307`).
4. Model 3 `u1..u96` publication in this repo snapshot occurs in runtime `upDateCellVolts()` (`src/BatMan.cpp:371`, `src/BatMan.cpp:853`).
5. `u1Age..u96Age`, `CellStaleCount`, and `CellMaxAge` are written in `upDateCellVolts()` (`src/BatMan.cpp:1005`, `src/BatMan.cpp:1017`, `src/BatMan.cpp:1018`).

### INFERRED

1. Source-level zero initializers strongly indicate cache arrays start as zero after a full firmware startup sequence.
2. If `parm_load()` restores display-value fields before first fresh read/publication pass, operators can temporarily observe pre-existing `u*` values after reboot.
3. Even without boot-time restore certainty, runtime stale-replay behavior exists inside a running session because `Voltage` is only overwritten on valid parse words (`!= 0xffff`).

### UNPROVEN

1. Whether `parm_load()` restores `VALUE_ENTRY` fields like `u1..u96` in this build/runtime, because restore internals are not present in this repository snapshot.
2. Whether any non-visible startup framework behavior publishes `u1..u96` before `BATMan::upDateCellVolts()`.
3. Whether observed “same values after reboot” in field logs is primarily due to parameter restore, transport timing, or external test procedure.

## Most Likely Explanations for “Same Values After Reboot”

1. Early parameter restore could repopulate display fields before first fresh measurement pass (`src/main.cpp:154` precedes `src/main.cpp:186`), but mechanism remains unproven in-repo.
2. Runtime publication lag (state progression plus command delays) can create a window where old displayed values remain visible before first new publish (`src/BatMan.cpp:282`, `src/BatMan.cpp:292`, `src/BatMan.cpp:312`, `src/BatMan.cpp:371`).
3. In-session stale replay from `Voltage` cache can preserve old-looking values when incoming words are invalid (`src/BatMan.cpp:465`, `src/BatMan.cpp:467`, `src/BatMan.cpp:546`, `src/BatMan.cpp:548`).

## Smallest Next Step if Still Unresolved

Perform a docs-only evidence-extension slice that inspects the checked-out `libopeninv` parameter save/load implementation used by `parm_load()` (no behavior change), then explicitly classify whether `VALUE_ENTRY` fields are restored on boot and in what order relative to first Model 3 publication.

## Stop-Condition Check

- Stop condition encountered: repository snapshot is insufficient to prove `parm_load()` internals, so restore behavior is kept `UNPROVEN` rather than guessed.
- No firmware/code behavior changes were made in this slice.
