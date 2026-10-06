# fprime-stm32-cli

Wires an STM32CubeMX-generated STM32H7 project into this repo's F' bare-metal
`Hardware/` layout: patches the CubeMX linker script and startup assembly
file for F's zero-dynamic-memory architecture, regenerates
`Hardware/CMakeLists.txt` so the `FprimeStm32` build target picks up exactly
the HAL sources/includes/defines your CubeMX configuration needs, and wires
`Hardware/`/`config/` and (once you have one) your deployment into the
project's CMake build graph. One command, `fprime-stm32 sync`, replaces what
used to be a multi-file hand port and multiple hand-edited `CMakeLists.txt`
files.

## Install

```bash
pip install -e lib/fprime-stm32/tools/fprime-stm32-cli
```

## Usage

Run from the root of an F' deployment project (a directory containing
`Hardware/`, or containing exactly one `<Deployment>/Hardware/`):

```bash
fprime-stm32 sync Hardware/<name>_hal [--dry-run] [--deployment <name>] [--wire-deployment <name>]
```

`<name>_hal` must be a **direct subdirectory of `Hardware/`** — that's where
you generate your CubeMX project in the first place (e.g.
`Hardware/stm32h743_hal/` for an STM32H743, alongside the existing
`Hardware/stm32h753_hal/`). `--dry-run` prints a unified diff of every file
it would touch without writing anything.

`--wire-deployment <name>` picks which `Deployments/<name>/` to connect to
the stm32h7 platform (see "Deployment build-graph wiring" below) — omit it
and `sync` auto-detects the deployment if there's exactly one. `--deployment
<name>` is unrelated: it disambiguates the *namespace* root (the directory
containing `Hardware/`) when the current directory has more than one.

## The CubeMX project requirement

STM32CubeMX (or CubeIDE) must generate the project with **Project Manager ->
Project -> Toolchain/IDE = "CMake"**. Every other toolchain option
(Makefile, IAR, Keil, ...) skips generating
`<project>/cmake/stm32cubemx/CMakeLists.txt` — and that file is the tool's
only source of truth for which chip, HAL driver modules, and peripheral-init
files your specific configuration needs. Without it, `sync` refuses to run
rather than guess.

## How it works

`sync` produces up to seven artifacts from two inputs — the CubeMX project
you just generated, and (on re-runs) whatever's already in `Hardware/` and
the rest of the project from a previous sync. Nothing here is chip-specific
string-matching; every decision is either read out of CubeMX's own
generated files or keyed off addresses that are fixed in STM32H7 silicon.

### 1. Discovery (`discovery.py`)

- Locates the F' project's `Hardware/` directory (from `cwd`, or
  `<Deployment>/Hardware` if there's exactly one deployment; `--deployment`
  disambiguates otherwise).
- Confirms the CubeMX project path is a direct child of that `Hardware/`
  directory — this is what lets generated paths in `Hardware/CMakeLists.txt`
  be simple relative strings like `stm32h743_hal/Core/Src/gpio.c`.
- Finds the CubeMX-generated linker script (`*FLASH.ld`), startup file
  (`startup_stm32h7*.s`, in `Core/Startup/` or the project root), and the
  `cmake/stm32cubemx/CMakeLists.txt` described above. Each lookup fails
  loudly (a clear error, not a guess) if it's missing or ambiguous.

### 2. Memory map (`memory_model.py`)

Parses the linker script's `MEMORY { ... }` block into named regions, then
classifies them **by origin address, not by name** — CubeMX's own naming for
these regions varies (`RAM`, `DTCMRAM`, `AXI_SRAM`, ...), but the addresses
are fixed:

| Region                    | Address      | Role                                        |
|----------------------------|--------------|----------------------------------------------|
| DTCM RAM                  | `0x20000000` | CPU-only, fast, **not** DMA-visible          |
| AXI SRAM (D1 domain)      | `0x24000000` | DMA-visible, where `Fw::Buffer`/DMA data must live |
| FLASH                     | (from CubeMX)| program storage                              |

If a linker script has no region at one of these two addresses, the tool
raises an error instead of patching — that's the chip-compatibility gate,
see below.

### 3. Linker script patch (`linker.py`)

Applied as targeted regex substitutions on the original CubeMX text (never a
full regeneration from a template), so vendor comments/license headers and
anything the tool doesn't need to touch survive untouched:

1. Rename the DTCM and AXI SRAM regions to `DTCM_RAM` / `AXI_SRAM` (only
   inside actual linker-script syntax — the `MEMORY{}` table entry,
   `ORIGIN()`/`LENGTH()` calls, `>NAME` selectors — never inside comments).
2. Retarget the default `.data`/`.tdata`/`.tbss`/`.bss` output sections from
   DTCM to `AXI_SRAM`. This is what puts `Fw::Buffer` storage and the
   `BootstrapAllocator` static pool (plain `.bss`, see
   `fprime-stm32/Allocator/BootstrapAllocator.cpp`) into DMA-safe memory
   with zero source changes.
3. Insert a new `.dtcm_bss` NOLOAD section (`DTCM_RAM`, 32-byte aligned, for
   D-cache safety) with `_sdtcm_bss`/`_edtcm_bss` boundary symbols, for
   CPU-only globals tagged `__attribute__((section(".dtcm_bss")))` (see
   `Hardware/config/include/PlatformMemory.hpp`'s `ATTR_DTCM_BSS`).
4. Append a `.axi_sram` NOLOAD section (`AXI_SRAM`, 32-byte aligned) with
   `_saxi_sram`/`_eaxi_sram` boundary symbols — an explicit opt-in spot for
   code that wants to place a static allocation in AXI SRAM by attribute,
   additional to (not a replacement for) the default `.data`/`.bss` remap.
5. **If `Hardware/linker/<name>.ld` already exists** (i.e. this isn't the
   first sync), the tool scans its `.dtcm_bss`/`.axi_sram` sections for any
   `*(...)` wildcard beyond the two standard ones it always emits, and
   carries those forward. This matters in practice: this repo's own
   `Hardware/linker/STM32H753xx_FLASH.ld` has two hand-added rules pinning
   specific `-fdata-sections`-split symbols (`CdhCore::cmdDisp`,
   `FileHandling::prmDb`) into DTCM to hit a memory budget. An earlier
   version of this tool silently dropped them on re-sync — a real ~8.5 KiB
   regression caught by rebuilding and diffing `arm-none-eabi-size` output
   before/after. Re-syncing now preserves them and reports
   `Preserved N custom .dtcm_bss placement rule(s)...` in the summary.

### 4. Startup file patch (`startup.py`)

Idempotent (checks for `_sdtcm_bss` first; skips with no changes if already
patched). Inserts, right after the CubeMX-standard `.bss` zero-fill loop and
before `__libc_init_array`:
- A zero-fill loop for `.dtcm_bss`, using the same pattern as CubeMX's own
  `.bss` loop.
- A call to `Stm32_registerBootstrapAllocator`, so the allocator is live
  before any C++ static constructor can call `new`. This assumes the
  deployment links `fprime-stm32`'s `Allocator/BootstrapAllocator.cpp` — if
  it doesn't, the link step fails on the undefined symbol (a clear failure,
  not silent misbehavior).

### 5. `Hardware/CMakeLists.txt` regeneration (`cubemx_cmake.py` + `hardware_cmake.py`)

`cubemx_cmake.py` parses `<cubemx_project>/cmake/stm32cubemx/CMakeLists.txt`
— CubeMX's own machine-generated build description — pulling out:
- the chip define (e.g. `STM32H753xx`), found generically as whichever
  `MX_Defines_Syms` token matches `STM32\w+` (not hardcoded per chip),
- HAL driver sources (`STM32_Drivers_Src`) — exactly the HAL modules your
  enabled peripherals need, nothing more,
- peripheral-init sources (`MX_Application_Src`), minus `main.c`/
  `sysmem.c`/`syscalls.c`/the startup `.s` (F' supplies its own `main`,
  and the CPU-only newlib stubs don't apply to F's allocator model) and
  minus the `*_it.c` file (pulled out separately, see below),
- include directories (`MX_Include_Dirs`).

`hardware_cmake.py` then renders `Hardware/CMakeLists.txt`: the
`FprimeStm32` static library gets those HAL/peripheral sources (prefixed
with your HAL directory name) plus whatever hand-written glue files already
exist under `Hardware/config/src/*.{c,cpp}` (discovered by globbing, not
assumed to be named anything specific — so your own clock-config/timer glue
survives every re-sync). It also exports three `CACHE INTERNAL` CMake
variables:

```cmake
FPRIME_STM32_LINKER_SCRIPT    # -> Hardware/linker/<name>.ld
FPRIME_STM32_STARTUP_SOURCE   # -> Hardware/startup/<name>.s
FPRIME_STM32_IT_SOURCE        # -> <hal_dir>/Core/Src/stm32h7xx_it.c
```

Your deployment's `CMakeLists.txt` references these instead of hardcoding a
chip-specific filename, so it never needs editing again after the first
setup — even if you later re-sync against a different chip. You don't need
to add these references by hand: `sync --wire-deployment <name>` (section 6
below) writes them into your deployment's `CMakeLists.txt` for you.

(`FPRIME_STM32_IT_SOURCE` is linked straight into the deployment executable
rather than through the `FprimeStm32` static archive: GNU ld only pulls an
`.o` out of a static archive when something already-linked has an
*undefined* reference into it, but the startup file's `.weak`
`Default_Handler` aliases satisfy every vector before the linker ever needs
to search the archive — so the real ISR bodies would be silently discarded
in favor of the weak infinite-loop aliases if they stayed in the archive.)

### 6. Project & deployment build-graph wiring (`project_wiring.py`)

Everything above makes `Hardware/CMakeLists.txt` correct in isolation, but a
fresh F' project/deployment doesn't yet *reach* it. `sync` patches up to
four more files, each idempotently (same insert-if-missing pattern as the
linker/startup patchers — re-running `sync` on an already-wired file makes
no further changes, reported as "already wired, no changes needed"):

- **The project's root `CMakeLists.txt`** (one directory above the F'
  namespace root): enables the ASM language (needed to compile
  `Hardware/startup/*.s`) and adds `config/` to the build graph.
- **The namespace `CMakeLists.txt`** (the one with `add_fprime_subdirectory(Components)`):
  adds `Hardware/` to the build graph.
- **The target deployment's `CMakeLists.txt`**: adds `restrict_platforms(stm32h7)`;
  extends `SOURCES` with `${FPRIME_STM32_STARTUP_SOURCE}`/`${FPRIME_STM32_IT_SOURCE}`;
  extends `DEPENDS` with `FprimeStm32`/`FprimeStm32Config`/`FprimeStm32Allocator`/
  `Os_Baremetal_OverrideNewDelete`; appends a `target_link_options(...)` block
  wiring in `${FPRIME_STM32_LINKER_SCRIPT}` and the operator new/delete
  `-Wl,--undefined=...` list `Os_Baremetal_OverrideNewDelete` needs to actually
  get linked in.
- **That deployment's `Top/CMakeLists.txt`**: adds `FprimeStm32Allocator` to
  `DEPENDS` (needed by `Stm32::getBootstrapAllocator()`/`lockBootstrapAllocator()`
  calls in the topology).

Which deployment gets the last two: `--wire-deployment <name>` picks one
explicitly; with no flag, `sync` auto-detects it if exactly one directory
exists under `Deployments/`, skips deployment-wiring with a note in the
report if none exist yet, and errors asking for `--wire-deployment` if
there's more than one. The root/namespace wiring always runs, since it has
no such ambiguity.

Every patch here targets the exact shape `fprime-util new --deployment`'s
cookiecutter template generates by default (`add_fprime_subdirectory(Top/)` +
a bare `register_fprime_deployment(SOURCES ... DEPENDS ...)` for the
deployment, `register_fprime_module(AUTOCODER_INPUTS ... DEPENDS Fw_Logger)`
for `Top/CMakeLists.txt`) — like the linker/startup patchers, this is
targeted insertion into the existing file, never a full-file regeneration,
so any components/sources you've already added to these files survive.

## Which STM32H7 chips this works on

The tool never matches on a part number string. It works by finding
regions at `0x20000000` (DTCM) and `0x24000000` (AXI SRAM, D1 domain) in
whatever `MEMORY{}` block CubeMX generated, and re-deriving everything else
(chip define, HAL sources, includes) from CubeMX's own project files. Those
two addresses are part of the STM32H7 core/bus-fabric design ST has kept
consistent across the whole H7 line — single- and dual-core parts alike
(H742/743/745/747/750/753/755/757, and the H7A3/H7B0/H7B3/H72x/H73x
sub-families) — so `sync` should work unmodified for any of them, provided
the CubeMX project was generated with the CMake toolchain option.

What that claim is and isn't based on:
- **Concretely validated end-to-end** — real `fprime-util build stm32h7`,
  real `arm-none-eabi-size` comparison, real preserved hand-tuned linker
  rules — only on the **STM32H753XIH6** (`FprimeStm32BaremetalReference`'s
  actual reference deployment).
- **Not validated on hardware/toolchain for any other specific part.** If a
  chip's CubeMX output doesn't follow this address layout (or doesn't
  follow the standard CubeMX linker/startup template this tool pattern-
  matches against), `sync` raises `LinkerScriptError`/`StartupScriptError`/
  `CubeMxCMakeError` and refuses to patch — you get a clear failure, not a
  silently wrong linker script.

## What stays manual

By design, not by omission:
- **`SystemClock_Config` / PLL and HSE values** — inherently board-specific
  (depend on your crystal and target `SYSCLK`); regenerate from CubeMX per
  board.
- **Peripheral/pin/timer selection in the topology** (`instances.fpp`,
  `topology.fpp`, the deployment's `Topology.cpp`) — this is exactly where
  F' expects hardware customization to live, not inside a generated file.

## Tests

```bash
pip install -e '.[test]'
pytest
```

Fixtures under `tests/fixtures/cubemx_stm32h753/` are literal copies of the
real raw CubeMX output already in this repo
(`FprimeStm32BaremetalReference/Hardware/stm32h753_hal/`), including its
CubeMX-generated `cmake/stm32cubemx/CMakeLists.txt`.
