# Tesla Model 3 BMS SOC/current-path lock (2026-04-04)

## Document Metadata

- Title: SOC/current-path investigation lock for Slice 2E-A
- Version: 0.1
- Date: 2026-04-04
- Scope: Docs-first lock before any State of Charge (SOC) code change
- Status: Investigation artifact with bounded next-step recommendation

## 1) Purpose and scope

This document locks what is currently `PROVEN`, `INFERRED`, and `UNPROVEN`
about SOC current-path behavior in this repository.

This slice is documentation-only. No firmware behavior changes are made.

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical project docs:
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Prior investigation anchors:
   - `docs/investigations/tesla-m3-bms-investigation-2026-04-03.md`
   - `docs/investigations/tesla-m3-bms-idcmode0-fix-matrix-2026-04-04.md`
4. Repo-local evidence:
   - `include/param_prj.h`
   - `src/BMSUtil.cpp`
   - `src/BatMan.cpp`
   - `src/main.cpp`
   - `src/CAN_Common.cpp`

## 3) Live context note

- Session context: `numbmbs = 4`, `idcmode = Off / 0`.
- Evidence status: runtime values above are session-provided and remain
  unproven from repo-only static evidence.
- Repo defaults relevant to this context:
  - `numbmbs` default `1` (`include/param_prj.h:54`)
  - `idcmode` default `0` (`include/param_prj.h:63`)

## 4) Existing investigation claims (carry-forward)

From prior investigation docs:

- `asDiff` update path is not proven in-repo.
- `idc` population path is not proven in-repo for Model 3 path.
- SOC logic uses low-current gate and otherwise integrates with `asDiff`.

Status of carry-forward claims against current repo scan: still consistent.

## 5) Repo-local evidence and lock findings

### 5.1 SOC-related parameters and call path

- `PROVEN`: SOC/current-related fields exist in `PARAM_LIST`.
  - Evidence: `soc` at `include/param_prj.h:66`, `idc` at
    `include/param_prj.h:73`, `idcmode` at `include/param_prj.h:63`,
    `idcgain`/`idcofs` at `include/param_prj.h:61` to
    `include/param_prj.h:62`, `nomcap` at `include/param_prj.h:57`.
- `PROVEN`: `BMSUtil::UpdateSOC()` runs every 100 ms task cycle.
  - Evidence: `src/main.cpp:77`.

### 5.2 `asDiff` declaration, reader, writer, publication

- `PROVEN`: declaration exists as file-scope variable with zero initialization.
  - Evidence: `src/BMSUtil.cpp:31`.
- `PROVEN`: read/consumed in SOC integration branch.
  - Evidence: `src/BMSUtil.cpp:52`.
- `PROVEN`: no writer/update site for `asDiff` is found in repository source
  scan.
  - Evidence: no additional `asDiff` references beyond `src/BMSUtil.cpp:31` and
    `src/BMSUtil.cpp:52`.
- `PROVEN`: no direct publication surface for `asDiff` exists as a parameter.
  - Evidence: no `asDiff` entry in `include/param_prj.h`.

### 5.3 Current (`idc`) read/write evidence

- `PROVEN`: SOC logic reads `idc` for low-current threshold behavior.
  - Evidence: `src/BMSUtil.cpp:37`.
- `PROVEN`: CAN reporting path reads `idc` for frame payload.
  - Evidence: `src/CAN_Common.cpp:110`.
- `PROVEN`: no explicit in-repo `Param::Set*` writer for `Param::idc` is found
  in `src/`.
  - Evidence: repository search shows reads (`src/BMSUtil.cpp:37`,
    `src/CAN_Common.cpp:110`) and no `SetInt/SetFloat(Param::idc, ...)` sites.
- `UNPROVEN`: external update path (for example, tooling/terminal/CANopen path)
  supplying `idc` at runtime.

### 5.4 idcmode relevance and gating

- `PROVEN`: idcmode enum contract exists (`0=Off, 1=AdcSingle, 2=IsaCan`).
  - Evidence: `include/param_prj.h:244`, `include/param_prj.h:254` to
    `include/param_prj.h:257`.
- `PROVEN`: no direct runtime branch on `Param::idcmode` is found in current
  source scan.
  - Evidence: no `Param::GetInt(Param::idcmode)` usage in `src/`.
- `INFERRED`: in `idcmode=0` context, SOC behavior likely depends on whether
  `idc` remains near zero and whether `asDiff` remains unwritten.

### 5.5 SOC working and missing path distinction

- **PROVEN working path**
  - `UpdateSOC()` executes and always writes `Param::soc`.
  - Evidence: `src/BMSUtil.cpp:33` to `src/BMSUtil.cpp:56`.
  - Voltage-estimate branch is implemented when low-current counter exceeds
    threshold.
  - Evidence: `src/BMSUtil.cpp:46` to `src/BMSUtil.cpp:49`,
    `src/BMSUtil.cpp:58` to `src/BMSUtil.cpp:77`.

- **PROVEN missing path**
  - `asDiff` update path is missing in repo code.
  - Evidence: only declaration/read at `src/BMSUtil.cpp:31` and
    `src/BMSUtil.cpp:52`.
  - No explicit in-repo `idc` writer is present in scanned source.

- **INFERRED risk**
  - If `idc` is not actively updated in runtime, SOC behavior may collapse to
    voltage-based updates after `NoCurRun` and/or perform near-no-op integration
    due to `asDiff` staying zero.

- **UNPROVEN claims**
  - End-to-end runtime source and cadence of `idc` in deployed systems.
  - Runtime significance of `idcmode` selection in current codebase.

## 6) Relevance to idcmode=0

- `PROVEN`: idcmode default is `Off` and no in-repo idcmode branch is present.
- `INFERRED`: current SOC result for idcmode=0 depends primarily on observed
  runtime `idc` behavior and missing `asDiff` writer.
- `UNPROVEN`: exact runtime `idc` supply path under idcmode=0 in field use.

## 7) Next-slice recommendation (explicit and bounded)

### Recommended next step: validation/instrumentation first

Reason for recommendation:

1. Missing `asDiff` writer is `PROVEN`, but runtime `idc` source remains
   `UNPROVEN`.
2. A direct SOC behavior change now would risk silently redefining SOC meaning
   without runtime evidence.

Bounded next slice should therefore:

1. Capture runtime traces for `idc`, low-current gate transitions, and SOC path
   branch usage.
2. Confirm whether `asDiff` is intentionally unused versus accidentally unwired.
3. Return with a docs-first lock for one of two explicit outcomes:
   - lock voltage-estimate-dominant SOC behavior for idcmode=0, or
   - implement a bounded `asDiff` update path with explicit semantics.

## 8) Release honesty note

This investigation lock does not claim SOC accuracy. Hardware/runtime validation
is still required before accuracy claims.

