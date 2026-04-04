# Tesla Model 3 BMS idcmode=0 Fix Matrix (2026-04-04)

## Document Metadata

- Title: idcmode=0 fix matrix and bounded firmware-fix plan
- Version: 0.1
- Date: 2026-04-04
- Scope: Slice 2A documentation lock (planning only)
- Status: Investigation artifact (non-canonical until promoted)

## 1) Purpose and scope

This document locks a docs-first issue matrix for the `idcmode = Off (0)`
runtime context and proposes bounded implementation order for later firmware
slices.

This slice is planning-only. It does not approve or perform firmware behavior
changes.

Explicit non-goals in this slice:

- no balancing logic behavior changes
- no State of Charge (SOC) behavior changes
- no cell enumeration code changes
- no Controller Area Network (CAN) or reporting changes
- no over-the-air (OTA) update-flow changes
- no authentication/logging/schema/state-machine changes

## 2) Authorities consulted

1. `00_Identity` (session-declared highest authority; no repo file provided in
   this slice)
2. Canonical project docs:
   - `docs/Canonical_Documentation_Index_v1_0.md`
   - `docs/Project_Decisions_Snapshot_v0_1.md`
3. Existing investigation evidence:
   - `docs/investigations/tesla-m3-bms-investigation-2026-04-03.md`
4. Repo-local implementation evidence:
   - `src/BatMan.cpp`
   - `src/BMSUtil.cpp`
   - `src/CAN_Common.cpp`
   - `include/param_prj.h`

## 3) Live context for this session

- `numbmbs = 4`
- `idcmode = Off / 0`

Evidence status note:

- The values above are session-provided live context for planning.
- Repo defaults are `numbmbs` default `1` and `idcmode` default `0` at
  `include/param_prj.h:54` and `include/param_prj.h:63`.
- Runtime confirmation for the active controller remains unproven in-repo and
  should be captured in follow-up validation.

## 4) Source inputs

1. Upstream Issue #3 findings (as currently represented in repo documentation)
   - Investigated symptom context: charging imbalance and "first 12 cells"
     behavior framing captured in
     `docs/investigations/tesla-m3-bms-investigation-2026-04-03.md:26`.
2. Existing repo investigation findings
   - Current proven/inferred findings in
     `docs/investigations/tesla-m3-bms-investigation-2026-04-03.md:34`.
3. Repo-local evidence where available
   - Parsing, mapping, and bounds behavior in `src/BatMan.cpp`.
   - Current/SOC pathways in `src/BMSUtil.cpp`, `src/CAN_Common.cpp`, and
     `include/param_prj.h`.

## 5) Issue matrix

| Issue ID | Problem statement | Source(s) | Evidence / repo path(s) | Status: PROVEN / INFERRED / UNPROVEN | Relevance to idcmode=0: YES / DEFER / UNKNOWN | Recommended action |
|---|---|---|---|---|---|---|
| M3-ISS-01 | Cell slot `14` is parsed but not processed in `upDateCellVolts()`, creating a slot coverage mismatch. | Upstream Issue #3 symptom framing; existing investigation note | `src/BatMan.cpp:445` (`g = 12..14` stored), `src/BatMan.cpp:690` (`if (Yc < 14)`), `docs/investigations/tesla-m3-bms-investigation-2026-04-03.md:58` | PROVEN | YES | In firmware slice, align producer/consumer slot bounds (`0..14`) and add focused coverage checks for full per-chip slot handling. |
| M3-ISS-02 | Multiple `GetData()` loops use `h <= 8` while arrays are sized `[8]`, creating out-of-bounds risk at index `8`. | Existing investigation note; repo-local code review | `src/BatMan.cpp:65`, `src/BatMan.cpp:79`, `src/BatMan.cpp:83`, `src/BatMan.cpp:389`, `src/BatMan.cpp:403`, `src/BatMan.cpp:417`, `src/BatMan.cpp:432`, `src/BatMan.cpp:446`, `src/BatMan.cpp:460` | PROVEN | YES | In firmware slice, convert unsafe loops to bounded `< 8` traversal and run targeted regression checks around raw frame ingestion. |
| M3-ISS-03 | Cell enumeration safety depends on `ChipNum = numbmbs * 2`; configured `numbmbs` range can exceed fixed `[8]` storage unless bounded. | Existing investigation note; repo-local code review | `include/param_prj.h:54` (`numbmbs` max `32`), `src/BatMan.cpp:192` (`ChipNum = numbmbs * 2`), fixed arrays at `src/BatMan.cpp:65`, `src/BatMan.cpp:77`, `src/BatMan.cpp:79`, `src/BatMan.cpp:83` | INFERRED | YES | Add explicit chip-count guardrails in firmware slice (clamp/reject out-of-range before indexing) and document supported range for Model 3 path. |
| M3-ISS-04 | `CellsBalancing` appears to count pre-mask candidates, while final commands are odd/even masked later; metric semantics may be ambiguous for operators. | Upstream Issue #3 balancing concern; existing investigation note | Candidate increment at `src/BatMan.cpp:711`, publication at `src/BatMan.cpp:741`, odd/even mask at `src/BatMan.cpp:552` | PROVEN | YES | Keep behavior unchanged in Slice 2A; in later slice, decide whether to preserve current semantics with clearer docs or add a separate commanded-cell counter. |
| M3-ISS-05 | Current acquisition path for `idcmode > 0` is not evidenced in this repo (`idcmode` declared, usage not found). | Existing investigation note; repo-local code search | `include/param_prj.h:63`, `include/param_prj.h:243`, `include/param_prj.h:255`, search shows no `idcmode` branch usage in `src/` | UNPROVEN | DEFER | Defer to dedicated `idcmode > 0` investigation/implementation slice; do not block idcmode=0 cell safety fixes on this item. |
| M3-ISS-06 | `asDiff` (ampere-second delta) is consumed in SOC integration but no writer is proven in-repo. | Existing investigation note | Definition and use at `src/BMSUtil.cpp:31` and `src/BMSUtil.cpp:52`; no writer found in repo search | PROVEN | DEFER | Defer to SOC/current-path slices after idcmode=0 cell safety work; require runtime evidence before any SOC accuracy claim. |
| M3-ISS-07 | "First 12 cells rise first" plausibly maps to register groups `0x47..0x4A` versus `0x4B`, but one-to-one runtime causality is not yet proven. | Upstream Issue #3 symptom framing; existing investigation note | Request sequence at `src/BatMan.cpp:253` to `src/BatMan.cpp:273`; parsing blocks at `src/BatMan.cpp:388` to `src/BatMan.cpp:457`; investigation discussion at `docs/investigations/tesla-m3-bms-investigation-2026-04-03.md:64` | INFERRED | UNKNOWN | Capture timestamped runtime logs (raw `Voltage[Xr][Yc]` and derived `uN`) before asserting exact physical-cell causality. |

## 6) Explicit deferred items (idcmode>0-only or likely idcmode>0-only)

The following items are explicitly deferred from the idcmode=0 firmware-fix
critical path:

1. `idcmode = AdcSingle (1)` and `idcmode = IsaCan (2)` current-source behavior,
   because branch-level implementation evidence is currently unproven in this
   repo.
2. SOC coulomb-integration completion that depends on validated current path
   (`idc` source plus `asDiff` update chain).
3. Any release claim about SOC accuracy under current-driven modes until direct,
   current hardware validation exists.

## 7) Unknowns / evidence needed

1. Runtime proof of active controller settings (`numbmbs = 4`, `idcmode = 0`) via
   parameter dump/log capture.
2. Runtime proof of full slot propagation (`Voltage[Xr][14]` to `uN`, min/max,
   and `deltaV`) after future firmware fix.
3. Runtime correlation for "first 12 cells" symptom against parsed group timing
   and any omitted-slot effects.
4. Operator-facing interpretation of `CellsBalancing` (candidate count versus
   commanded count) with capture of post-mask command bits.

## 8) Recommended implementation order for later slices

1. **Slice 2B (safety bounds hardening):**
   - fix all `<= 8` loop bounds on `[8]` arrays,
   - add defensive chip-count bounds checks before indexed writes.
2. **Slice 2C (cell slot coverage alignment):**
   - resolve `GetData()`/`upDateCellVolts()` slot-bound mismatch,
   - verify each chip slot path is represented in derived `uN` and min/max values.
3. **Slice 2D (balancing metric clarity):**
   - clarify/report `CellsBalancing` semantics (candidate versus commanded)
     without silent contract change.
4. **Slice 3+ (deferred current/SOC path):**
   - investigate and implement `idcmode > 0` current path only in dedicated
     slices with explicit evidence gates.

## 9) Suggested slice boundaries for code work

- Keep `idcmode=0` safety work isolated from SOC/current mode work.
- Separate memory-safety and indexing fixes from reporting semantics decisions.
- Do not combine balancing algorithm changes with parser/bounds corrections.
- Require per-slice evidence notes under `docs/investigations/` before promoting
  behavior claims into canonical docs.

## Release honesty reminder

This matrix is a planning artifact. It does **not** claim that balancing is
fixed, SOC is accurate, or full 96-cell behavior is hardware-validated.

