# Tesla Model 3 BMS Repo Reality Scan (2026-04-04)

## Document Metadata

- Title: Tesla Model 3 BMS Repo Reality Scan
- Version: 0.1
- Date: 2026-04-04
- Scope: Slice 1A documentation reality check
- Status: Investigation artifact (non-canonical by itself)

## Scope and Purpose

This report captures repo-observable reality for documentation alignment in
Slice 1A.

It is an investigation artifact. It does not approve firmware behavior changes.

## Canonical Documentation Index Path

- Status: `PROVEN`
- Path: `docs/Canonical_Documentation_Index_v1_0.md`
- Note: This canonical index is created in this same slice.

## Build System and Target Environments

### Build entry points

- Status: `PROVEN`
- Evidence:
  - `Makefile:79` defines `all` target (`directories images`).
  - `Makefile:95` links firmware binary `stm32_m3bms`.
  - `Makefile:85` and `Makefile:87` generate `.bin` and `.hex` outputs.
  - `Makefile:134` to `Makefile:138` initializes submodules and builds `libopencm3` dependency.

### Toolchain and architecture

- Status: `PROVEN`
- Evidence:
  - `Makefile:21` toolchain prefix defaults to `arm-none-eabi`.
  - `Makefile:32` defines `-DSTM32F1`.
  - `Makefile:33` and `Makefile:36` use `-mcpu=cortex-m3 -mthumb`.

### Continuous integration environment

- Status: `PROVEN`
- Evidence:
  - `.github/workflows/CI-build.yml:9` uses `ubuntu-latest` runner.
  - `.github/workflows/CI-build.yml:20` installs `gcc-arm-none-eabi`.
  - `.github/workflows/CI-build.yml:25` runs `make get-deps`.
  - `.github/workflows/CI-build.yml:29` runs `make`.

### Development container hints

- Status: `PROVEN`
- Evidence:
  - `.gitpod.yml:2` uses `.gitpod.Dockerfile`.
  - `.gitpod.Dockerfile:5` installs `gcc-arm-none-eabi`.

## Pin Allowlists / Defaults

### Digital and analog project pin lists

- Status: `PROVEN`
- Evidence:
  - `include/digio_prj.h:11` defines `DIG_IO_LIST` entries.
  - `include/digio_prj.h:12` to `include/digio_prj.h:18` lists configured digital pins.
  - `include/anain_prj.h:16` defines `ANA_IN_LIST` entries.
  - `include/anain_prj.h:17` and `include/anain_prj.h:18` list configured analog channels.

### Bootloader pin defaults

- Status: `PROVEN`
- Evidence:
  - `src/hwinit.cpp:65` describes bootloader-time floating-pin concern.
  - `src/hwinit.cpp:70` starts `write_bootloader_pininit()`.
  - `src/hwinit.cpp:83` to `src/hwinit.cpp:90` sets explicit default pin commands.

### Pin allowlist policy document

- Status: `UNPROVEN`
- Finding:
  - No separate canonical "pin allowlist policy" document is found in this repo at scan time.

## UI Module List

### In-repo module list for external UI

- Status: `UNPROVEN`
- Findings:
  - This repository includes terminal commands and JSON parameter output hooks,
    but no standalone front-end module catalog.
- Evidence:
  - `src/terminal_prj.cpp:20` and `src/terminal_prj.cpp:21` describe command set for web interface usage.
  - `src/terminal_prj.cpp:47` exposes `json` terminal command.
  - `src/main.cpp:203` to `src/main.cpp:205` triggers `PrintParamsJson` via CANopen Service Data Object (SDO) request path.

## Locked Contracts Detected

### Parameter identifiers and enumeration surfaces

- Status: `PROVEN`
- Evidence:
  - `include/param_prj.h:49` and `include/param_prj.h:50` track next parameter/value identifiers.
  - `include/param_prj.h:52` defines full `PARAM_LIST` macro contract.
  - `include/param_prj.h:239` to `include/param_prj.h:247` define enum-string surfaces.

### Emitted CAN frame identifiers

- Status: `PROVEN`
- Evidence:
  - `src/CAN_Common.cpp:85` emits frame `0x351`.
  - `src/CAN_Common.cpp:103` emits frame `0x355`.
  - `src/CAN_Common.cpp:122` emits frame `0x356`.
  - `src/CAN_Common.cpp:135` emits frame `0x35E`.
  - `src/CAN_Common.cpp:148` emits frame `0x370`.
  - `src/CAN_Common.cpp:172` emits frame `0x373`.

### Incoming CAN filter registration

- Status: `PROVEN`
- Evidence:
  - `src/main.cpp:112` registers `0x100` receive message.
  - `src/main.cpp:123` handles `0x100` in callback.

## Existing Investigations Folder Status

- Status: `PROVEN`
- Finding:
  - Canonical investigations location for this slice is `docs/investigations/`.
  - No second investigation directory is introduced.

## Unknowns and Required Evidence

1. `UNPROVEN`: authoritative external UI module list location.
   - Needed evidence: path to UI repository or in-repo module manifest.
2. `UNPROVEN`: explicit written policy for pin allowlists/default governance.
   - Needed evidence: canonical policy document or maintainers' documented contract.
3. `INFERRED`: which contracts are guaranteed stable across releases beyond current code shape.
   - Needed evidence: versioned compatibility statement in canonical docs.

## Proven / Inferred / Unproven Summary

- `PROVEN`: build system entry points, toolchain/architecture markers, CI steps, pin lists in code, bootloader pin defaults, key CAN frame outputs, parameter list surface.
- `INFERRED`: long-term stability expectations for contract surfaces absent explicit policy text.
- `UNPROVEN`: standalone UI module catalog and standalone pin allowlist governance document.
