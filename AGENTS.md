# AGENTS.md

## Purpose

This repository hosts firmware for a **STM32F103-based Tesla Model 3 Battery Management System (BMS) controller**.

Use this guide to keep contributions public-quality, reviewable, and aligned with repo reality.

## Collaboration Rules

1. **Pull Request first**: prefer small, reviewable pull requests over large bundled changes.
2. **Bounded slices**: each change set should have a narrow scope and explicit non-goals.
3. **Docs before behavior**: update investigation/canonical documentation before changing firmware behavior or reporting semantics.
4. **Evidence over assumptions**: do not claim behavior that is not proven by repository evidence or hardware validation.

## Documentation Expectations

- Write for public readability and mixed audiences.
- Expand abbreviations on first use (for example, State of Charge (SOC)).
- Keep Markdown headings, naming, and whitespace consistent.
- Label uncertain statements explicitly (for example, `[TENTATIVE]` or `unproven`).

## Safety and Integrity Constraints

- Do not silently change user-facing contracts (parameter identifiers, field names, CAN frame IDs, or protocol semantics).
- Do not introduce behavior changes in docs-only slices.
- Keep release notes honest about validation status.

## Release Honesty Rule

Never claim the following without direct, current hardware evidence:

- balancing is fixed,
- SOC is accurate,
- full 96-cell behavior is validated.

If evidence is incomplete, say so directly and describe what validation is still required.
