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

## v0.9.1-pre3 (current)

Version: v0.9.1-pre3

Date: 2026-04-14

- Pre-production release. Field validation pending.
- This pre-release builds on `v0.9.1-pre2` by adding targeted mapped-source raw/fresh visibility for key displayed-cell indices.

## What changed

### Targeted mapped-source raw debug diagnostics
Added append-only targeted raw mirrors:
- `DbgU1Raw`
- `DbgU12Raw`
- `DbgU13Raw`
- `DbgU86Raw`
- `DbgU96Raw`

These expose the mapped internal source value used when flattened publication processes displayed indices `1`, `12`, `13`, `86`, and `96`.

### Targeted same-pass freshness diagnostics
Added append-only targeted freshness flags:
- `DbgU1Fresh`
- `DbgU12Fresh`
- `DbgU13Fresh`
- `DbgU86Fresh`
- `DbgU96Fresh`

Each flag reports `1` when the mapped source slot is fresh in the current pass by existing raw-slot freshness truth (`VoltageFresh[Xr][Yc] != 0`), else `0`.

## What did not change
- No balancing logic or balancing reporting semantics changes
- No State of Charge (SOC)/current-path logic changes
- No Controller Area Network (CAN) payload/schema changes
- No redefinition of existing `u1..u96`, `u1Age..u96Age`, `CellStaleCount`, or `CellMaxAge`

## Validation / field test focus
- Compare `u1/u12/u13/u86/u96` against matching `DbgU*Raw` to confirm mapped-source visibility.
- Compare `DbgU*Fresh` against expected current-pass ingest behavior during idle and charge/discharge transitions.
- Continue using `CellGrp*Fresh`, `CellDataStaleWarn`, `CellStaleCount`, and `CellMaxAge` as complementary diagnostics.

## Release honesty note
- This is a diagnostics-focused pre-production release.
- It improves visibility into mapped-source versus freshness behavior but does not claim full resolution of the observed `u1..u12` / `u13..u96` runtime pattern.

## v0.9.1-pre2

Version: v0.9.1-pre2

Date: 2026-04-08

- Pre-production release. Field validation pending.
- This pre-release builds on `v0.9.1-pre1` by adding append-only age diagnostics and raw-slot freshness truth for stronger stale/live interpretation.

## What changed

### Cell-age diagnostics
Added append-only diagnostics for displayed-cell age/staleness interpretation:
- `CellStaleCount`
- `CellMaxAge`
- `u1Age..u96Age`

### Raw-slot freshness truth for age reset
- Added raw per-slot freshness truth at `Voltage[8][15]` granularity in the Model 3 ingest/publication path.
- Age reset follows accepted current-pass raw updates (`non-0xffff` slot writes), not flattened publication occurrence alone.

## What did not change
- No balancing threshold or cadence retuning
- No proven SOC/current-path fix yet
- No CAN payload/schema expansion for these diagnostics
- No claim that balancing or SOC behavior is fully field-validated

## Validation / field test focus
- Capture `CellStaleCount`, `CellMaxAge`, and representative `uNAge` values across idle and active pack conditions.
- Compare age reset behavior against observed ingest freshness.
- Confirm existing `u1..u96` remain numeric and unchanged in meaning.

## Release honesty note
- This is a diagnostics-focused pre-production release and not a claim that root cause is fully solved.

## v0.9.1-pre1

Version: v0.9.1-pre1

Date: 2026-04-04

- Pre-production release. Field validation pending.
- This pre-release builds on `v0.9.0-pre1` by improving parser safety and adding append-only freshness/range/warning diagnostics for stale-display investigation.

## What changed

### Parser / ingest hardening
- Fixed the proven parser buffer traversal overrun risk in `BATMan::GetData()` by bounding the affected loop safely.

### Cell-update freshness diagnostics
Added append-only diagnostics for current-pass ingest-group freshness:
- `CellGrp0Fresh`
- `CellGrp1Fresh`
- `CellGrp2Fresh`
- `CellGrp3Fresh`
- `CellGrp4Fresh`

### Group-to-cell interpretation diagnostics
Added append-only diagnostics for currently published `u*` ranges fed by each ingest group:
- `CellGrp0First` / `CellGrp0Last`
- `CellGrp1First` / `CellGrp1Last`
- `CellGrp2First` / `CellGrp2Last`
- `CellGrp3First` / `CellGrp3Last`
- `CellGrp4First` / `CellGrp4Last`

### Stale-display warning
Added append-only diagnostic:
- `CellDataStaleWarn`

### Documentation / operator reference
- Added a bounded parameter meanings reference for new/clarified diagnostics.

## What did not change
- No balancing threshold or cadence retuning
- No proven SOC/current-path fix yet
- No CAN payload/schema expansion for these diagnostics
- No claim that balancing or SOC behavior is fully field-validated

## Validation / field test focus
- Capture `CellGrp0Fresh..CellGrp4Fresh`, `CellDataStaleWarn`, and `CellGrp0First..CellGrp4Last` on live hardware.
- Compare representative displayed cells (`u1`, `u12`, `u13`, `u24`, `u86`, `u96`) against ingest-group diagnostics.
- Confirm no regressions in normal pack monitoring behavior.

## Release honesty note
- This is a pre-production diagnostics release intended to improve evidence quality before stronger hardware or full-system claims.
