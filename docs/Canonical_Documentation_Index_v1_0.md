# Canonical Documentation Index v1.0

## Document Metadata

- Title: Canonical Documentation Index
- Version: 1.0
- Purpose: Define canonical documentation order and promotion rules
- Scope: Repository-wide documentation governance

## Purpose

This index defines the canonical documentation spine for
`Tesla-M3-Bms-Software`.

It establishes reading order, authority, and the relationship between stable
project documentation and time-bounded investigations.

## Authority and Reading Order

Read documents in this order when evaluating design intent, constraints, or
change scope:

1. `AGENTS.md`
2. `docs/Canonical_Documentation_Index_v1_0.md` (this file)
3. `docs/Project_Decisions_Snapshot_v0_1.md`
4. `docs/Project_Assumptions_Registry_v0_1.md`
5. `README.md`
6. `docs/investigations/*.md` (investigation records)

## Canonical Document Set

- `AGENTS.md`: collaboration guardrails and public-quality expectations.
- `docs/Canonical_Documentation_Index_v1_0.md`: canonical map and authority
  order.
- `docs/Project_Decisions_Snapshot_v0_1.md`: append-only decisions and
  decision status.
- `docs/Project_Assumptions_Registry_v0_1.md`: explicit assumptions, defaults,
  and confidence labels.
- `README.md`: public-facing overview and navigation entry point.

## Relationship to Investigations

- Investigation notes live under `docs/investigations/`.
- Investigations are evidence snapshots and working analysis artifacts.
- Investigations **inform** canonical docs but do not automatically override
  them.
- Findings only become canonical after explicit promotion into a canonical
  document and clear status update.

## Promotion Rule

When investigation findings are promoted:

1. Add or update the relevant entry in
   `docs/Project_Decisions_Snapshot_v0_1.md`.
2. Update `docs/Project_Assumptions_Registry_v0_1.md` if assumptions changed.
3. Update `README.md` only if public-facing behavior claims changed and are
   proven.
