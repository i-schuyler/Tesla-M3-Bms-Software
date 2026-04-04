# Project Decisions Snapshot v0.1

## Document Metadata

- Title: Project Decisions Snapshot
- Version: 0.1
- Purpose: Record project decisions in append-only form
- Scope: Canonical documentation governance and decision history

## Purpose

This file records project decisions in append-only form.

Entries document what is decided, what remains investigation-only, and what
evidence supports each status.

## Status Labels

- `LOCKED`: approved for use as canonical project direction.
- `PROVISIONAL`: accepted for current planning but not yet hardware-validated.
- `INVESTIGATION-FINDING`: documented finding, not an approved behavior change.

## Decision Log (append-only)

### DEC-0001 — Establish canonical docs spine

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Canonical project docs are now defined by `AGENTS.md`, this decisions
    snapshot, the assumptions registry, and the canonical index.
  - Investigation notes remain in `docs/investigations/` and require explicit
    promotion before becoming canonical.
- Evidence:
  - `docs/Canonical_Documentation_Index_v1_0.md`
  - `docs/Project_Assumptions_Registry_v0_1.md`

### DEC-0002 — Keep behavior unchanged in Slice 1A

- Date: 2026-04-04
- Status: `LOCKED`
- Decision:
  - Slice 1A is documentation-only and does not approve firmware behavior
    changes.
  - No balancing, State of Charge (SOC), Controller Area Network (CAN), or
    state-machine behavior changes are implied.
- Evidence:
  - Scope and constraints captured in `AGENTS.md`
  - Repo Reality Scan at
    `docs/investigations/tesla-m3-bms-repo-reality-scan-2026-04-04.md`

### DEC-0003 — Current investigation context remains finding-level

- Date: 2026-04-04
- Status: `INVESTIGATION-FINDING`
- Decision:
  - Current Tesla Model 3 BMS investigation context is retained as
    investigation output only.
  - Investigation findings are not treated as silently approved code changes.
- Evidence:
  - `docs/investigations/tesla-m3-bms-repo-reality-scan-2026-04-04.md`
- Notes:
  - Promotion requires explicit follow-up decision entries with proof and
    scope.

## Append-Only Rule

- Do not rewrite old decisions.
- Add a new decision entry to supersede or refine prior direction.
