# Tesla Model 3 BMS investigation note (2026-04-03)

Session anchor: `HL-2026-04-03 — bms_investigation`  
Related issue: #3 (this note is intended to stand on its own)

## 1) Purpose

Document the current, repo-verifiable investigation status for Tesla Model 3 Battery Management Board (BMB) behavior before any implementation changes.

This is a docs-only slice. No firmware logic, balancing logic, state of charge (SOC) logic, schema, user interface, or endpoint changes are included here.

## 2) Questions being investigated

1. Is the voltage-to-SOC table explicitly hardcoded, and where?
2. Is `asDiff` (ampere-second delta) updated anywhere in the repo?
3. Where is `idc` (direct current pack current) populated in the repo, if anywhere?
4. Is `CellsBalancing` a candidate count or an actual command count?
5. Does the code skip the highest stored cell slot per chip?
6. Do the “first 12 cells rise first” symptoms plausibly correspond to a specific parsed register group or chip pair?
7. How are cells mapped into `u1..uN` reporting slots when `numbmbs = 4` in the live controller?

Coworker-observed symptom (verbatim):

> “While I’m charging the deltaV is climbing as the first 12 cell voltages grow without the rest coming up. We want the balancing working properly, charging to come in to all the cells fairly evenly”

## 3) Current confirmed findings

1. **Voltage-to-SOC table is explicitly hardcoded.**
   - Answer: **Yes**. `voltageToSoc[]` is defined inline in `src/BMSUtil.cpp`.
2. **`UpdateSOC()` uses low-current threshold behavior and otherwise uses `asDiff`.**
   - Answer: **Yes**. If `ABS(idc) < NoCurLim`, the no-current counter increments and can force voltage-estimated SOC; otherwise SOC integrates with `asDiff`.
3. **`asDiff` update path is not proven.**
   - Answer: `asDiff` is defined and consumed, but no write/update site was found in this repo.
4. **`idc` population path is not proven in-repo for Tesla Model 3 BMB path.**
   - Answer: no `Param::Set*` for `Param::idc` was found in this repo; `idc` is read for SOC and CAN reporting.
5. **Mapping behavior matches scan-order population.**
   - `ChipNum = numbmbs * 2` and cells are assigned into `u1..uN` using a linear index `h` while scanning `Voltage[Xr][Yc]` by chip (`Xr`) and cell-slot (`Yc`).
6. **Balancing selection and command masking are separate stages.**
   - Candidate selection uses `BallVthres` and `(OldUmin + BalHys) < Voltage[Xr][Yc]` in `upDateCellVolts()`.
   - Odd/even masking is applied later in `WriteCfg()` (`0xAA` or `0x55`), so `CellsBalancing` appears to represent candidate count before final mask.
7. **Main likely bug is repo-verifiable.**
   - `GetData()` stores cell groups through index `14` (`g <= 14` for `0x4B`), but `upDateCellVolts()` processes only `Yc < 14` (indices `0..13`).
8. **Secondary likely bug is repo-verifiable.**
   - Multiple loops use `for (int h = 0; h <= 8; h++)` while arrays are sized `[8]`, creating out-of-bounds risk on index `8`.
9. **Symptom mapping plausibility.**
   - “First 12 cells” plausibly aligns with register groups `0x47..0x4A` (`g=0..11`), while remaining group `0x4B` is `g=12..14`.
   - Exact live chip-pair correspondence is **not yet proven in repo** (requires runtime capture/log correlation).

## 4) Evidence table

| Area | Claim | Evidence (file:line + snippet/description) | Proof status |
|---|---|---|---|
| SOC | Voltage-to-SOC lookup is hardcoded | `src/BMSUtil.cpp:25` defines `uint16_t voltageToSoc[] = { 3300, 3400, 3450, 3500, 3560, 3600, 3700, 3800, 4000, 4100, 4200 };` | Proven |
| SOC | SOC uses low-current threshold path | `src/BMSUtil.cpp:37` checks `ABS(Param::GetFloat(Param::idc)) < NoCurLim`; `src/BMSUtil.cpp:46-49` calls `EstimateSocFromVoltage()` | Proven |
| SOC | SOC otherwise uses `asDiff` | `src/BMSUtil.cpp:52` applies `TempSOC = TempSOC + (100 * asDiff / (3600 * Param::GetInt(Param::nomcap)));` | Proven |
| SOC/current | `asDiff` defined/used but not updated | `src/BMSUtil.cpp:31` defines `float asDiff = 0;`; `src/BMSUtil.cpp:52` consumes it; repo search for `asDiff` finds no writer | Proven for "no writer found in repo" |
| Current path | `idcmode` supports Off/AdcSingle/IsaCan | `include/param_prj.h:243` `#define IDCMODES "0=Off, 1=AdcSingle, 2=IsaCan"`; enum at `include/param_prj.h:255` | Proven |
| Current path | `idc` is read but not explicitly written in repo | Reads: `src/BMSUtil.cpp:37`, `src/CAN_Common.cpp:110`; no `Param::SetInt/SetFloat(Param::idc, ...)` found | `idc` population **not yet proven in repo** |
| Current path | Tesla Model 3 BMB read path does not show direct current extraction | `src/main.cpp:68-70` runs `BATMan::loop()` for Model 3; `src/BatMan.cpp:247-277` requests status/cell/aux/total voltage commands, no explicit current decode | Proven for code shown |
| Mapping | Live-config mapping formula in code path | `src/BatMan.cpp:192` sets `ChipNum = Param::GetInt(Param::numbmbs)*2;` | Proven |
| Mapping | Cell slots are populated in scan order into `u1..uN` | `src/BatMan.cpp:688-728` iterates `Yc` then advances `Xr`; `src/BatMan.cpp:704` sets `Param::u1 + h` | Proven |
| Mapping | `GetData()` stores up to cell slot 14 | `src/BatMan.cpp:445-454` handles `0x4B` with `for (int g = 12; g <= 14; g++)` and writes `Voltage[h][g]` | Proven |
| Mapping/main bug | Update path skips highest stored slot | `src/BatMan.cpp:690` uses `if (Yc < 14)` so index `14` is never processed by `upDateCellVolts()` | Proven |
| Balancing | Threshold and candidate condition | `src/BatMan.cpp:654` `BallVthres < umax`; `src/BatMan.cpp:708-711` candidate when `(OldUmin + BalHys) < Voltage[Xr][Yc]` | Proven |
| Balancing | Odd/even mask happens later in `WriteCfg()` | `src/BatMan.cpp:552-560` masks command bytes with `0xAA` or `0x55` | Proven |
| Balancing/reporting | `CellsBalancing` likely candidate count | Increment happens at candidate selection `src/BatMan.cpp:711`; published at `src/BatMan.cpp:741`; no post-mask recount found | Proven for candidate-counter behavior in code |
| UI/reporting | Cell and balancing values are exposed as reportable parameters | `include/param_prj.h:82` starts `u1`; `include/param_prj.h:189` includes `u108`; `include/param_prj.h:232` defines `CellsBalancing` | Proven |
| UI/reporting | CAN reporting includes `idc` | `src/CAN_Common.cpp:110-113` packs `idc` into frame `0x356` | Proven |
| Secondary bug | Potential out-of-bounds loops use `<= 8` on `[8]` arrays | Arrays sized `[8]`: `src/BatMan.cpp:65`, `src/BatMan.cpp:83`; loops with `h <= 8`: `src/BatMan.cpp:389`, `src/BatMan.cpp:403`, `src/BatMan.cpp:417`, `src/BatMan.cpp:432`, `src/BatMan.cpp:446`, `src/BatMan.cpp:460` | Proven |

## 5) Uncertainties / open questions

1. Live context says `idcmode = Off` and `numbmbs = 4`; these exact runtime values are **not yet proven in repo** (runtime/config dump needed).
2. `idc` source is **not yet proven in repo** for this firmware path (could be external tooling, CANopen/SDO parameter writes, or another integration path not represented here).
3. Exact physical cell-to-`uN` mapping per module/chip pair is **not yet proven in repo** (code shows scan order, not physical harness order).
4. The coworker symptom is plausible from register-group behavior and slot skipping, but exact one-to-one runtime causality is **not yet proven in repo**.

## 6) Risks / likely root causes

1. **High confidence:** slot `14` per chip is parsed into `Voltage[Xr][14]` but never transferred into `u`/min/max/delta calculations (`Yc < 14` gate).
2. **High confidence:** out-of-bounds index usage (`h <= 8` with `[8]`) may corrupt adjacent data and produce unstable or misleading cell behavior.
3. **Medium confidence:** SOC coulomb-count style path is effectively incomplete because `asDiff` update is not proven in this repo.
4. **Medium confidence:** current-dependent SOC branch behavior depends on `idc`, but `idc` population path is not proven in repo.

## 7) Recommended next validation steps

1. Capture one full measurement cycle log (raw `Voltage[Xr][Yc]`, derived `u1..uN`, `CellMin/CellMax`, `deltaV`) to verify whether `Yc=14` is omitted in runtime outputs.
2. Validate loop bounds safety with targeted instrumentation/static analysis around all `h <= 8` loops in `GetData()`.
3. Trace `idc` end-to-end at runtime (parameter source, update frequency, and behavior when `idcmode = Off`) and document whether it is external to Tesla BMB SPI path.
4. Compare `CellsBalancing` against actual transmitted `CellBalCmd` bits after odd/even masking to confirm candidate-vs-command semantics.
5. Correlate “first 12 cells rise first” with register groups (`0x47..0x4A` vs `0x4B`) using timestamped captures.

## 8) Proposed implementation slices (docs-first plan only, no code changes in this slice)

1. **Slice 2 — Validation instrumentation pass:** add temporary diagnostics to prove slot coverage (`Yc=14`), loop bounds behavior, and current-source path.
2. **Slice 3 — Cell mapping/bounds fix:** adjust cell-slot processing and unsafe loop bounds; add focused regression checks for full per-chip slot coverage.
3. **Slice 4 — Balancing semantics clarity:** align `CellsBalancing` meaning (candidate vs commanded) with reporting, or add separate counters.
4. **Slice 5 — SOC current-path completion:** either implement/restore `asDiff` update path or explicitly document voltage-only SOC mode when current is unavailable.
5. **Slice 6 — Release-note and operator guide update:** document verified behavior, known limits, and required live settings for stable balancing.
