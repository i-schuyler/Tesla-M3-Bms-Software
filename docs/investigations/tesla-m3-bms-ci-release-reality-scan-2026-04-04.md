# Tesla Model 3 BMS CI and Release Reality Scan (2026-04-04)

## 1) Purpose and scope

This document records the current, repo-evidenced reality of build, Continuous
Integration (CI), and release artifact handling.

Scope is documentation only. This scan does not change firmware logic,
workflows, or release behavior.

## 2) Artifacts/evidence consulted

- CI workflow files under `.github/workflows/`
- `Makefile`
- `README.md`
- Canonical docs under `docs/`
- Project file `stm32_m3bms.cbp`
- Local repository tag list (`git tag -l`)

## 3) Current build system reality

### Build entry points

- `PROVEN`: main build entry point is `make` via `Makefile`.
  - Evidence: `Makefile:79`, `Makefile:83`, `Makefile:95`.
- `INFERRED`: IDE-driven builds also call `make` through Code::Blocks project
  settings.
  - Evidence: `stm32_m3bms.cbp:6`, `stm32_m3bms.cbp:10`,
    `stm32_m3bms.cbp:24`.

### Target/output names (only where proven)

- `PROVEN`: linked output target name is `stm32_m3bms`.
  - Evidence: `Makefile:22`, `Makefile:95`.
- `PROVEN`: `.bin` and `.hex` outputs are generated from `stm32_m3bms`.
  - Evidence: `Makefile:84`, `Makefile:85`, `Makefile:86`, `Makefile:87`.
- `UNPROVEN`: output file named `*.elf` is not explicitly defined.

### Dependency/toolchain assumptions (only where proven)

- `PROVEN`: toolchain prefix defaults to `arm-none-eabi`.
  - Evidence: `Makefile:21`.
- `PROVEN`: CI installs `gcc-arm-none-eabi`.
  - Evidence: `.github/workflows/CI-build.yml:20`.
- `PROVEN`: build depends on submodules and builds `libopencm3` via
  `make get-deps`.
  - Evidence: `Makefile:134`, `Makefile:136`, `Makefile:138`,
    `.github/workflows/CI-build.yml:25`.

## 4) Current CI reality

### Existing workflow files and what they do

- `PROVEN`: one workflow file exists: `.github/workflows/CI-build.yml`.
- `PROVEN`: workflow triggers on `push` and `pull_request`.
  - Evidence: `.github/workflows/CI-build.yml:2`,
    `.github/workflows/CI-build.yml:3`, `.github/workflows/CI-build.yml:4`.
- `PROVEN`: workflow installs toolchain, runs dependency build, runs `make`,
  and uploads `.bin` and `.hex` artifacts.
  - Evidence: `.github/workflows/CI-build.yml:17`,
    `.github/workflows/CI-build.yml:22`, `.github/workflows/CI-build.yml:27`,
    `.github/workflows/CI-build.yml:31`, `.github/workflows/CI-build.yml:36`.

### What CI verifies vs what it does not verify

- `PROVEN` verifies:
  - firmware build command completes (`make`),
  - CI artifact upload steps for `stm32_m3bms.bin` and `stm32_m3bms.hex` run.
- `PROVEN` does not verify (no evidence of these steps in workflow):
  - hardware-in-the-loop behavior,
  - firmware functional tests,
  - release publication,
  - signed artifacts,
  - update/installation path.

## 5) Current release reality

### Tags/releases/artifact workflow evidence in repo

- `PROVEN`: local repository has tags (`0.01D`, `0.02A`, `0.03A`, `Fresh_Fork`)
  based on `git tag -l`.
- `PROVEN`: no dedicated release workflow file is present under
  `.github/workflows/`.
  - Evidence: `.github/workflows/CI-build.yml:1` and workflow directory listing
    showing only this file.
- `UNPROVEN`: GitHub Releases publication process (manual or automated) is not
  described in repository docs.

### Whether packaged firmware artifacts are currently produced by CI

- `PROVEN`: CI uploads workflow artifacts named firmware binary and firmware hex.
  - Evidence: `.github/workflows/CI-build.yml:33`,
    `.github/workflows/CI-build.yml:34`, `.github/workflows/CI-build.yml:38`,
    `.github/workflows/CI-build.yml:39`.
- `UNPROVEN`: those artifacts are published as release assets tied to tags.

## 6) Firmware artifact truth table

| Artifact / path | Status | Evidence |
|---|---|---|
| `stm32_m3bms` (linked output) | `PROVEN` | `Makefile:22`, `Makefile:95` |
| `stm32_m3bms.bin` | `PROVEN` | `Makefile:84`, `Makefile:85`, `.github/workflows/CI-build.yml:34` |
| `stm32_m3bms.hex` | `PROVEN` | `Makefile:86`, `Makefile:87`, `.github/workflows/CI-build.yml:39` |
| `*.elf` named output | `UNPROVEN` | No explicit `*.elf` filename in build rules |
| `*.srec` output | `UNPROVEN` | Clean rule removes it (`Makefile:116`, `Makefile:117`) but no generation rule found |
| `*.list` output | `UNPROVEN` | Clean rule removes it (`Makefile:118`, `Makefile:119`) but no generation rule found |
| Web-UI-uploadable `.bin` path requirement | `UNPROVEN` | No repo doc/workflow defines web UI upload contract |

Notes:

- `INFERRED`: `stm32_m3bms` is likely an ELF-format executable because it is
  input to `objcopy` for `.bin`/`.hex` conversion (`Makefile:85`,
  `Makefile:87`), but the filename itself does not use `.elf`.

## 7) Risks / gaps

- `PROVEN` gap: CI currently proves build success and artifact upload, but not
  firmware behavior correctness.
- `PROVEN` gap: no release workflow is present to bind artifacts to tagged
  releases.
- `UNPROVEN` gap: no documented, repo-local rule for which artifact should be
  used in any external web upload path.
- Risk of overclaim: stating "release-ready pipeline" or "web upload-ready
  artifact path" would overstate current repo evidence.

## 8) Recommended next slice

### Smallest safe next implementation slice (CI/release hardening)

1. Add one bounded release workflow that runs on tag events.
2. Reuse existing build path (`make get-deps`, `make`).
3. Publish exactly two release assets from CI: `stm32_m3bms.bin` and
   `stm32_m3bms.hex`.
4. Add minimal release documentation describing artifact names and provenance.

### Explicit NOT in next slice

- No firmware logic changes.
- No CAN, balancing, SOC, or cell-mapping changes.
- No OTA/update protocol changes.
- No web UI feature changes.
- No broad CI refactor beyond tag-release hardening.

## 9) Unknowns and what evidence would resolve them

1. `UNPROVEN`: Which artifact format is required by downstream tools/UI.
   - Evidence needed: canonical integration contract or operator documentation.
2. `UNPROVEN`: Whether release publication should be automatic or manual.
   - Evidence needed: maintainer policy in canonical docs.
3. `UNPROVEN`: Whether artifacts require signing/checksums before release.
   - Evidence needed: security/release policy document.
4. `UNPROVEN`: Whether tagged releases must include additional metadata files.
   - Evidence needed: release checklist documented in canonical docs.
