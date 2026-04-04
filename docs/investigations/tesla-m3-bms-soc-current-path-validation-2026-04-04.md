# Tesla Model 3 BMS SOC current-path validation (2026-04-04)

## Document Metadata

- Title: SOC current-path truth validation for idcmode=0
- Version: 0.1
- Date: 2026-04-04
- Scope: Slice 2E-B validation follow-up (docs-only)
- Status: Investigation artifact with bounded next action

## 1) Purpose and scope

Validate repo-observable truth for the SOC current path under `idcmode=0`
without changing firmware behavior.

This slice performs evidence validation only. No SOC math or current-path code
changes are included.

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical docs:
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md` (`DEC-0008`)
3. Prior SOC lock:
   - `docs/investigations/tesla-m3-bms-soc-current-path-lock-2026-04-04.md`
4. Repo-local evidence:
   - `include/param_prj.h`
   - `src/BMSUtil.cpp`
   - `src/BatMan.cpp`
   - `src/main.cpp`
   - `src/CAN_Common.cpp`

## 3) Call path into SOC update

- `PROVEN`: scheduler 100 ms task calls `BMSUtil::UpdateSOC()`.
  - Evidence: `src/main.cpp:60` to `src/main.cpp:79`.
- `PROVEN`: `UpdateSOC()` reads `soc`, `idc`, `nomcap`, then writes `soc`.
  - Evidence: `src/BMSUtil.cpp:35`, `src/BMSUtil.cpp:37`,
    `src/BMSUtil.cpp:52`, `src/BMSUtil.cpp:55`.

## 4) `asDiff` writer/reader table (exact)

| Variable | Location | Operation | Evidence | Status |
|---|---|---|---|---|
| `asDiff` | `src/BMSUtil.cpp` | Declaration/init | `src/BMSUtil.cpp:31` | PROVEN |
| `asDiff` | `src/BMSUtil.cpp` | Read/consume in SOC integration | `src/BMSUtil.cpp:52` | PROVEN |
| `asDiff` | `src/` (repo scan) | Writer/update | No additional `asDiff` references beyond `src/BMSUtil.cpp:31` and `src/BMSUtil.cpp:52` | PROVEN missing path |
| `asDiff` | Parameter/report surfaces | Publication | No `asDiff` parameter entry in `include/param_prj.h` | PROVEN not published |

## 5) idcmode/current-path evidence under idcmode=0

### 5.1 idcmode surfaces

- `PROVEN`: `idcmode` parameter contract exists.
  - Evidence: `include/param_prj.h:63`, `include/param_prj.h:244`,
    `include/param_prj.h:256`.
- `PROVEN`: no runtime `Param::GetInt(Param::idcmode)` branch appears in repo
  source.
  - Evidence: no `Param::GetInt(Param::idcmode)` usage in `src/` scan.

### 5.2 idc source/read/write truth

- `PROVEN`: `idc` is read by SOC and CAN reporting.
  - Evidence: `src/BMSUtil.cpp:37`, `src/CAN_Common.cpp:110`.
- `PROVEN`: in-repo source scan finds no explicit `Param::Set*` writer for
  `Param::idc`.
  - Evidence: `Param::idc` occurrences in `src/` are read sites at
    `src/BMSUtil.cpp:37` and `src/CAN_Common.cpp:110`.
- `UNPROVEN`: any concrete runtime idc source specific to `idcmode=0` in this
  repo.

### 5.3 Additional dead/commented-path evidence

- `PROVEN`: a historical/commented SOC write exists in Model 3 aux update, but
  it is inactive and unrelated to `asDiff` writer restoration.
  - Evidence: `src/BatMan.cpp:758`.
- `PROVEN`: CAN receive handler in this repo does not implement idc update
  logic.
  - Evidence: `src/CAN_Common.cpp:221` to `src/CAN_Common.cpp:225`.

## 6) Validation conclusion

- **PROVEN working path:**
  - SOC update runs every 100 ms and can execute voltage-estimate branch.
  - SOC integration branch uses `asDiff` value if low-current condition is not
    met.
- **PROVEN missing path:**
  - No in-repo `asDiff` writer is present.
  - No in-repo `idc` writer/source is proven for idcmode=0.
- **INFERRED risk:**
  - Current-path SOC behavior can be incomplete or externally dependent in
    idcmode=0 deployments.
- **UNPROVEN claim space:**
  - External/runtime mechanism supplying `idc` in deployed systems.

## 7) Missing-writer classification

- `asDiff` missing writer classification: **PROVEN bug/incomplete path in repo
  implementation** (consumed but no writer present).
- `idc` source classification under idcmode=0: **UNPROVEN in repo**, potentially
  external dependency but not proven from code.

## 8) Bounded next-step recommendation

### Recommendation: docs clarification + bounded instrumentation first

1. Keep SOC behavior code unchanged until runtime `idc` source is evidenced.
2. Run a bounded instrumentation/trace slice to confirm whether `idc` is ever
   updated in deployed idcmode=0 operation.
3. After trace evidence, choose one explicit small action:
   - if external `idc` is confirmed reliable, document dependency clearly;
   - if not confirmed, open a narrow code slice to restore/implement an
     `asDiff` update path with docs-first contract lock.

## 9) Release honesty note

No SOC accuracy claim is supported by this repo-only validation.

