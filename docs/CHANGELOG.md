# Changelog — Tesla Model 3 BMS Firmware

This changelog describes release-level changes for tagged firmware builds. Pre-release versions may be pending field validation and should be treated as validation candidates until hardware evidence is complete.

---

## Release Notes Template

Copy/paste and fill for each release:
Before tagging, confirm firmware-reported version matches the changelog/release version.

```
Version:
Date:
Summary (1–2 lines):

Highlights:
- 

Behavior changes (user-visible):
- 

Protocol/Contract changes (append-only):
- 

Verification:
- CI:
- Field validation:

Known issues / risks:
- 

Rollback:
- Previous tag:

Artifact:
- Release asset name:
- sha256:
```

---

## v0.9.1 (current)

Version: v0.9.1

Date: 2026-04-04

Summary (1–2 lines): Pre-production release candidate with bounded idcmode=0 correctness fixes, append-only diagnostics/contracts, and conservative SOC/current-path status. Field validation is pending.

Highlights:
- Canonical docs spine, decisions snapshot, assumptions registry, and investigation structure were established and locked for docs-first governance.
- Continuous Integration (CI) workflow includes publish-on-tag release asset upload for firmware binaries.
- M3-ISS-01 and M3-ISS-02 fixes shipped (cell slot/index 14 coverage alignment and bounds-safe traversal).
- M3-ISS-03 chip-count guardrail shipped for fixed-storage-safe ingestion traversal.
- Append-only balancing metric field `CellsBalancingCmd` shipped.
- Append-only SOC/current-path trace diagnostics `IDCTrace` and `AsDiffTrace` shipped.
- SOC/current-path remains instrumented/investigated; no claim that SOC current-path behavior is fully fixed.

Behavior changes (user-visible):
- Improved Tesla Model 3 cell-ingestion correctness for slot coverage and loop-bound safety.
- Added observable diagnostic/report fields: `CellsBalancingCmd`, `IDCTrace`, and `AsDiffTrace`.
- No claim of final State of Charge (SOC) correctness change in this pre-release.

Protocol/Contract changes (append-only):
- Added `CellsBalancingCmd`.
- Added `IDCTrace`.
- Added `AsDiffTrace`.
- Existing field names/meanings remain preserved; no renames/removals.

Verification:
- CI: TODO — confirm green status for `v0.9.1` release run (workflow support exists in-repo).
- Field validation: pending post-release.

Known issues / risks:
- Pre-production build.
- No field validation yet.
- No claim of final SOC/current-path correctness.
- No claim of final balancing accuracy.

Rollback:
- Previous tag: Fresh_Fork

Artifact:
- Release asset name: stm32_m3bms.bin
- sha256: [TODO]
