# Project Assumptions Registry v0.1

## Document Metadata

- Title: Project Assumptions Registry
- Version: 0.1
- Purpose: Track assumptions and confidence labels
- Scope: Documentation and planning guardrails

## Purpose

This registry captures working assumptions and safe defaults.

Each assumption is labeled by confidence so future slices can validate, revise,
or retire it without ambiguity.

## Confidence Labels

- `PROVEN`: directly supported by repository evidence.
- `INFERRED`: strongly suggested by evidence but not directly complete.
- `[TENTATIVE]`: plausible working assumption with missing proof.

## Assumptions

### ASSUME-0001 — Build toolchain and target family

- Label: `PROVEN`
- Statement: The primary firmware build uses GNU Arm Embedded toolchain
  (`arm-none-eabi`) and targets STM32F1-class microcontrollers.
- Evidence:
  - `Makefile:21`
  - `Makefile:31`
  - `Makefile:32`
  - `Makefile:138`
  - `.github/workflows/CI-build.yml:20`

### ASSUME-0002 — Tesla Model 3 BMS controller context

- Label: `PROVEN`
- Statement: Repo intent is firmware for an STM32F103-based controller used
  with Tesla Model 3 Battery Management Board (BMB) hardware.
- Evidence:
  - `README.md:2`
  - `README.md:3`

### ASSUME-0003 — Existing UI module list in this repo

- Label: `[TENTATIVE]`
- Statement: A standalone user interface module list is not maintained inside
  this repository.
- Evidence:
  - `src/terminal_prj.cpp:20`
  - `src/terminal_prj.cpp:21`
  - `src/terminal_prj.cpp:47`
- Validation needed:
  - Confirm whether UI module definitions live in a separate repository and, if
    so, link canonical source.

### ASSUME-0004 — Bootloader pin initialization defaults are contract-relevant

- Label: `INFERRED`
- Statement: Bootloader pin initialization entries should be treated as
  contract-relevant hardware defaults.
- Evidence:
  - `src/hwinit.cpp:65`
  - `src/hwinit.cpp:70`
  - `src/hwinit.cpp:83`
  - `src/hwinit.cpp:90`
- Validation needed:
  - Confirm if additional board revisions require different default pin
    commands.

### ASSUME-0005 — Contract surfaces require explicit change control

- Label: `PROVEN`
- Statement: Parameter IDs and emitted CAN frame identifiers are externally
  consumed and should be treated as locked contracts unless explicitly revised.
- Evidence:
  - `include/param_prj.h:49`
  - `include/param_prj.h:64`
  - `include/param_prj.h:232`
  - `src/CAN_Common.cpp:85`
  - `src/CAN_Common.cpp:103`
  - `src/CAN_Common.cpp:122`
  - `src/CAN_Common.cpp:135`
  - `src/CAN_Common.cpp:148`
  - `src/CAN_Common.cpp:172`

## Safe Defaults for Future Slices

1. Prefer documentation updates before behavioral firmware changes.
2. Treat undocumented behavior as unproven until code evidence and hardware
   evidence are both available.
3. Avoid changing parameter IDs, field naming, and CAN frame IDs in
   mixed-scope slices.
4. Keep release claims conservative unless validated on real hardware.
