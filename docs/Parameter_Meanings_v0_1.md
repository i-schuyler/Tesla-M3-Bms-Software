# Parameter Meanings v0.1

## Document Metadata

- Title: Parameter Meanings Reference
- Version: 0.1
- Purpose: Public-readable meanings for newly added or clarified diagnostics and balancing-related fields
- Scope: `CellsBalancing`, `CellsBalancingCmd`, `IDCTrace`, `AsDiffTrace`, `CellGrp*Fresh`, `CellDataStaleWarn`, and `CellGrp*First/Last`
- Slice mode: Docs-only reference (no firmware behavior changes)

## Status key

- `repo-proven`: Field declaration/order and publication path are directly proven in current repository code.
- `field-supported`: Field meaning is locked/documented and code-backed, but upstream source certainty is incomplete.
- `field-validation pending`: Field is implemented, but runtime/hardware validation evidence is still required.

## Balancing-related fields

| Field name | Plain-language meaning | Category | How to interpret it | Known caveats / limits | Validation status | Suggested tooltip |
|---|---|---|---|---|---|---|
| `CellsBalancing` | Count of cells selected as balancing candidates before odd/even command masking. | operational/reporting | Higher values mean more cells passed candidate checks in the current update pass. | This is not the same as the final commanded count sent after odd/even mask filtering. | `repo-proven` | Candidate balancing cells (pre-mask). |
| `CellsBalancingCmd` | Count of balancing bits that remain after odd/even masking and are placed into the config write payload for the pass. | operational/reporting | Compare with `CellsBalancing` to see pre-mask candidates versus masked commanded bits. | This reports commanded bits in firmware payload construction, not proof of physical balancing effectiveness on hardware. | `repo-proven` | Commanded balancing cells (post-mask). |

## SOC/current-path trace fields

| Field name | Plain-language meaning | Category | How to interpret it | Known caveats / limits | Validation status | Suggested tooltip |
|---|---|---|---|---|---|---|
| `IDCTrace` | Trace copy of the `idc` value observed by `UpdateSOC()`. | diagnostic-only | Use as a visibility aid for what SOC logic read at that moment. | In-repo `idc` write/source for `idcmode=0` is not proven from current code evidence. | `field-supported` | SOC-read current trace value. |
| `AsDiffTrace` | Trace copy of the `asDiff` value consumed by SOC integration logic. | diagnostic-only | Helps verify whether the SOC integration branch is seeing non-zero amp-second deltas. | Current repo evidence shows `asDiff` is consumed but no writer path is proven in-repo. | `field-supported` | SOC integration delta trace (`asDiff`). |

## Cell-group freshness and stale-warning fields

| Field name | Plain-language meaning | Category | How to interpret it | Known caveats / limits | Validation status | Suggested tooltip |
|---|---|---|---|---|---|---|
| `CellGrp0Fresh`, `CellGrp1Fresh`, `CellGrp2Fresh`, `CellGrp3Fresh`, `CellGrp4Fresh` | Per-group freshness flags for the five Model 3 ingest read groups (`0x47..0x4B`) in the current pass. | diagnostic-only | `1` means at least one valid (`!= 0xffff`) word was observed in that ingest group during the pass; `0` means none were observed for that group in that pass. | Group-level coarse signal only; does not prove every cell slot in the group was refreshed. | `field-validation pending` | Freshness by ingest group (0..4). |
| `CellDataStaleWarn` | Diagnostic warning flag for front-loaded/partially stale display risk across ingest groups. | diagnostic-only | `1` means the pass encountered a fresh group and then later a non-fresh group; `0` means that specific simple condition was not detected. | Preserve interpretation text: "when the current pass indicates at least one later ingest group is not fresh while earlier groups are fresh enough that the displayed u* list may be front-loaded or partially stale". Rule is intentionally simple and does not classify all stale patterns. | `field-validation pending` | Warning: later groups may be stale. |

## Cell-group published-index range fields

| Field name | Plain-language meaning | Category | How to interpret it | Known caveats / limits | Validation status | Suggested tooltip |
|---|---|---|---|---|---|---|
| `CellGrp0First`, `CellGrp0Last` | First and last published `u*` index contributed by ingest group 0 in the current pass. | diagnostic-only | Positive values mark first/last observed published index from that group; `0` means no valid published index in the pass. | Published index (`u1..`) is a validity-gated flattened stream, not a direct physical-cell identifier. | `field-validation pending` | Group 0 published index range. |
| `CellGrp1First`, `CellGrp1Last` | First and last published `u*` index contributed by ingest group 1 in the current pass. | diagnostic-only | Positive values mark first/last observed published index from that group; `0` means no valid published index in the pass. | Published index (`u1..`) is a validity-gated flattened stream, not a direct physical-cell identifier. | `field-validation pending` | Group 1 published index range. |
| `CellGrp2First`, `CellGrp2Last` | First and last published `u*` index contributed by ingest group 2 in the current pass. | diagnostic-only | Positive values mark first/last observed published index from that group; `0` means no valid published index in the pass. | Published index (`u1..`) is a validity-gated flattened stream, not a direct physical-cell identifier. | `field-validation pending` | Group 2 published index range. |
| `CellGrp3First`, `CellGrp3Last` | First and last published `u*` index contributed by ingest group 3 in the current pass. | diagnostic-only | Positive values mark first/last observed published index from that group; `0` means no valid published index in the pass. | Published index (`u1..`) is a validity-gated flattened stream, not a direct physical-cell identifier. | `field-validation pending` | Group 3 published index range. |
| `CellGrp4First`, `CellGrp4Last` | First and last published `u*` index contributed by ingest group 4 in the current pass. | diagnostic-only | Positive values mark first/last observed published index from that group; `0` means no valid published index in the pass. | Published index (`u1..`) is a validity-gated flattened stream, not a direct physical-cell identifier. | `field-validation pending` | Group 4 published index range. |

## Notes

- This reference is intentionally bounded to the fields listed in this slice.
- This document does not claim balancing is fixed, State of Charge (SOC) is accurate, or full 96-cell behavior is hardware-validated.
