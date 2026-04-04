# Tesla-M3-Bms-Software

## Overview

`Tesla-M3-Bms-Software` is firmware for an **STM32F103-based controller** used
with Tesla Model 3 Battery Management Board (BMB) hardware.

The repository contains embedded firmware source, build scripts, and board-level
configuration code.

## Current Status and Validation Honesty

This project is actively documented and investigated.

Claims in this repository are intentionally conservative and should be treated
as code-evidence based unless explicitly hardware-validated.

## Documentation Spine

- Canonical documentation index:
  `docs/Canonical_Documentation_Index_v1_0.md`
- Decisions snapshot:
  `docs/Project_Decisions_Snapshot_v0_1.md`
- Assumptions registry:
  `docs/Project_Assumptions_Registry_v0_1.md`
- Investigation notes directory:
  `docs/investigations/`
- Current repo reality scan:
  `docs/investigations/tesla-m3-bms-repo-reality-scan-2026-04-04.md`

## Build and Environment (repo-evidenced)

- Primary build entry point: `Makefile`
- Toolchain family: `arm-none-eabi`
- Continuous Integration workflow: `.github/workflows/CI-build.yml`

## Related Hardware Reference

- Hardware and additional background:
  https://github.com/damienmaguire/Tesla-Model-3-Battery-BMS
