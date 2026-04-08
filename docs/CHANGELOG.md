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

## v0.9.1-pre2 (current)

Version: v0.9.1-pre2

Date: 2026-04-08

- Pre-production release. Field validation pending.
- This pre-release builds on `v0.9.1-pre1` by adding append-only cell-age diagnostics and raw-slot freshness truth for improved stale/live interpretation before stronger hardware conclusions are made.

## What changed

### Parser / ingest hardening
- Fixed the proven parser buffer traversal overrun risk in `BATMan::GetData()` by bounding the affected loop safely.
- This is intended to reduce the risk of corrupted downstream publication from out-of-bounds traversal without changing broader parser behavior.

### Cell-update freshness diagnostics
Added append-only diagnostics to help determine which ingest groups are actually updating in the current pass:
- `CellGrp0Fresh`
- `CellGrp1Fresh`
- `CellGrp2Fresh`
- `CellGrp3Fresh`
- `CellGrp4Fresh`

These report whether each of the five cell-ingest groups produced valid data in the current pass.

### Group-to-cell interpretation diagnostics
Added append-only diagnostics to show which displayed `u*` ranges are currently fed by each ingest group:
- `CellGrp0First` / `CellGrp0Last`
- `CellGrp1First` / `CellGrp1Last`
- `CellGrp2First` / `CellGrp2Last`
- `CellGrp3First` / `CellGrp3Last`
- `CellGrp4First` / `CellGrp4Last`

These are intended to make the freshness indicators interpretable in the web UI.

### Stale-display warning
Added append-only diagnostic:
- `CellDataStaleWarn`

This warns when later ingest groups are not fresh after earlier groups are fresh, indicating the displayed `u*` list may be front-loaded or partially stale.

### Cell-age diagnostics
Added append-only diagnostics for displayed-cell age/staleness interpretation:
- `CellStaleCount`
- `CellMaxAge`
- `u1Age..u96Age`

These diagnostics are additive and do not redefine existing `u1..u96` numeric meaning.

### Raw-slot freshness truth for age reset
- Added raw per-slot freshness truth at `Voltage[8][15]` granularity in the Model 3 ingest/publication path.
- Age reset now follows accepted current-pass raw updates (`non-0xffff` slot writes), not flattened publication occurrence alone.
- This is intended to improve diagnostic truth for stale-versus-live interpretation; it is not a claim that root cause is fully solved.

### Documentation / operator reference
- Added a bounded parameter meanings reference for the new and clarified diagnostics so operators can interpret them more consistently.

## What did not change
- No balancing threshold or cadence retuning
- No proven SOC/current-path fix yet
- No CAN payload/schema expansion for these diagnostics
- No claim that balancing or SOC behavior is fully field-validated
- No claim that observed stale/static cell behavior is definitively hardware-only

## Update path
- This release is intended for the STM32/main-controller firmware update path via the ESP8266-hosted web interface bridge (`/fwupdate` path expectation).
- This release does not claim ESP8266 self-firmware changes.

## Repo-proven
- Firmware release assets are built and attached on release tags.
- Parser overrun hardening is merged.
- Freshness, range, and stale-warning diagnostics are present in firmware.
- Cell-age diagnostics (`CellStaleCount`, `CellMaxAge`, `u1Age..u96Age`) are present in firmware.
- Model 3 raw-slot freshness truth is wired into age reset behavior.
- Parameter meanings for the new/clarified diagnostics are documented in repo.

## Not yet field-proven
- Runtime behavior of the new freshness/range/warning diagnostics on live hardware
- Runtime behavior of new cell-age diagnostics and raw-slot freshness-driven age reset on live hardware
- Whether later cell groups are stale due to firmware ingest/publication behavior, measurement-path issues, or hardware-side causes
- Final SOC/current-path correctness under `idcmode=0`

## Recommended post-release validation
- Update the STM32 firmware through the expected web UI update path.
- Confirm the unit boots normally after update.
- Capture:
  - `CellGrp0Fresh..CellGrp4Fresh`
  - `CellDataStaleWarn`
  - `CellGrp0First..CellGrp4Last`
  - `CellStaleCount`
  - `CellMaxAge`
  - selected `u*` values: `u1`, `u12`, `u13`, `u96`
  - matching age values: `u1Age`, `u12Age`, `u13Age`, `u96Age`
- Idle capture: verify age fields increase over whole seconds during idle/limited-refresh periods while `u*` values remain numeric.
- Charge/discharge capture: verify actively refreshed cells show age reset behavior and compare `u1/u12` age patterns versus `u13/u96`.
- Verify `CellStaleCount` and `CellMaxAge` move consistently with observed per-cell ages.
- Confirm new age fields appear in expected UI locations and existing `u1..u96` remain unchanged in format/meaning.
- Confirm no regressions in pack voltage interpretation (`udc`, `uavg`), `CellsPresent`, existing parameter display, CAN behavior, and normal pack monitoring.

## Release honesty note
- This is a pre-production build intended to improve diagnosis of live cell-update / stale-display issues before stronger conclusions are drawn about hardware or full-system correctness.
