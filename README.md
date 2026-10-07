# F´ Stm32 Baremetal Reference Project

This project is a reference F´ deployment for embedded systems with strict
hardware and memory constraints. `ReferenceDeployment` contains only the basic
F´ components needed to demonstrate a bare-metal flight-software architecture:
static initialization, a cyclic executive, cooperative active components, and a
bounded communication path.

F´ (F Prime) is a component-driven framework for developing and deploying
spaceflight and other embedded software applications. See the
[F´ website](https://fprime.jpl.nasa.gov).

`lib/fprime-stm32` is a chip-family-portable F´ library (STM32H7 today, other
STM32 families addable without forking it): reusable OSAL delegates, drivers,
and a bootstrap allocator that any consuming project gets "for free," while
everything CubeMX-generated or board-specific (this board's `.ioc` output,
clock/tick-source glue, the linker script) stays owned by this project in
`FprimeStm32BaremetalReference/Hardware/`. See the
[`lib/fprime-stm32` module README](lib/fprime-stm32/README.md) for the full
architecture and the driver-configuration pattern described below. Sensor
components (currently an MPU-6050 IMU, wired directly to `Stm32I2cDriver`) come
from the [`fprime-sensors`](https://github.com/fprime-community/fprime-sensors)
submodule and connect directly to the STM32 drivers' ports.

## Configuring which hardware ports are enabled

Every STM32 driver (`Stm32UartDriver`, `Stm32I2cDriver`, `STM32Timer`) reads
which peripheral instance it's allowed to use from
`Stm32Config.hpp` — a `#define`-per-instance file (e.g. `I2C1_INSTANCE`,
`TIM2_INSTANCE`) that keeps a driver from touching a peripheral your board's
`.ioc` never actually configured. The library ships a default copy at
`lib/fprime-stm32/Drv/config/Stm32Config.hpp`; this project overrides it with
its own copy at `FprimeStm32BaremetalReference/config/fprime-stm32/Stm32Config.hpp`
(via `settings.ini`'s `config_directory`) to match this board's real wiring.
When starting a new project from this reference, edit the override copy, not
the library's default. Selecting a disabled instance in a driver's `open()`
call is treated as a build-time mistake and fails a `FW_ASSERT`, not a
runtime condition to recover from — see each sensor's `docs/sdd.md` for the
exact enable-then-select steps.

## Build

The project uses the GNU Arm Embedded toolchain and the `stm32h7` F´ platform.
From the project root:

```shell
source fprime-venv/bin/activate
fprime-util generate -f
fprime-util build -j"$(nproc)"
```

To build the deployment after generation:

```shell
ninja -C build-fprime-stm32h7 ReferenceDeployment
```

## ReferenceDeployment

The deployment runs without threads or an operating system. Its main loop:

1. Initializes the hardware platform and TIM2-backed time source.
2. Configures the generated topology.
3. Triggers the configured rate-group schedule.
4. Calls `Os::Baremetal::TaskRunner::runAll()` to execute one cooperative
   dispatch step for every registered active component.

The cyclic executive contains no blocking waits or delays. Commands, telemetry,
events, and file services are handled by the active components and their fixed
queues. `Svc::OsTime` supplies the deployment time source; the topology does
not depend on a host or newlib real-time clock.

The deployment also initializes the bare-metal MicroFs service with a bounded
configuration. The current MicroFs implementation recognizes `/bin<N>/file<M>`
paths; persistent parameter and file support for the framework's standard paths
remains future work.

## Static memory contract

The deployment is designed to avoid dynamic allocation after initialization.
`Os_Baremetal_OverrideNewDelete` routes C++ allocations through a fixed
bootstrap pool during startup. The pool is locked after topology initialization,
so an allocation request during cyclic execution asserts instead of silently
growing an unbounded heap.

The deployment also wraps `malloc`, `calloc`, `realloc`, and `free` at link time.
`malloc` uses the same bootstrap pool to support toolchain allocations that can
occur before `main()`. The other C heap entry points assert when used.

The reference linker script separates CPU-only state from DMA-visible storage:

| Region | Capacity |
| --- | ---: |
| `FLASH` | 2 MiB |
| `AXI_SRAM` | 512 KiB |
| `DTCM_RAM` | 128 KiB |

The latest recorded `baremetal-size stm32h7` result for the complete deployment
(re-measured September 14, 2026, after the Week 8 topology wiring and Week 9
driver-architecture split) was:

| Region | Used | Remaining |
| --- | ---: | ---: |
| Flash (`.text`+`.data`) | 645,708 bytes | 69.2% |
| AXI SRAM `.bss` | 261,308 bytes | 50.1% |
| DTCM `.dtcm_bss` | 8,584 bytes | 93.5% |
| Bootstrap pool | 112,656 of 131,072 bytes | 14.1% |

Flash and AXI SRAM `.bss` grew modestly (Week 8's `led`/`gpioDriver` topology
wiring plus the Week 9 driver split adding a handful of new members per
driver); DTCM `.dtcm_bss` is unchanged byte-for-byte. The bootstrap-pool
figure is a runtime allocation count, not a static section size, and is
carried over from the last hardware run (no board was attached in this
measurement pass) — it should still be re-verified live once convenient.

The reduced framework configuration in `config/` keeps queue depths, telemetry
hash tables, command tables, and file catalog capacity proportional to this
deployment's actual component set.

## Validation status

`ReferenceDeployment` has been flashed to and run on the physical
STM32H753XI-EVAL2 board. The complete topology has reached `main()`, completed
setup, and run with cooperative dispatch enabled. Recorded validation includes:

- No assertion, abort, exit, HardFault, BusFault, or fatal-handler hits during
  the endurance runs.
- The on-board LED (PF10), driven exclusively through `Stm32::Stm32GpioDriver`
  (no application code touches `HAL_GPIO_*` directly), confirmed blinking
  end-to-end via a GDS-based integration test rather than manual register
  polling.
- All registered active-component queues created and dispatched successfully.
- The USART1 DMA ground link exercised with command, telemetry, and event
  traffic through `fprime-gds`.
- `Stm32::Stm32I2cDriver` (I2C1, blocking/polled) driving a real MPU-6050 IMU
  through `fprime-sensors`' `MpuImu.ImuManager` component, wired directly to
  the driver's `Drv.I2c` ports with no adapter — accel/gyro telemetry and
  range/error events confirmed streaming correctly through `fprime-gds`.

Detailed board pin mappings, clock initialization, DMA cache requirements,
driver behavior, hardware measurements, and the remaining STM32 soak tests are
documented in [`lib/fprime-stm32/README.md`](lib/fprime-stm32/README.md).

## Testing

Two independent layers of automated testing back this deployment:

- **Host unit tests** (`fprime-util check`, run from a component's own
  directory): every STM32 driver under `lib/fprime-stm32/Drv/` splits into a
  hardware-independent implementation and a thin HAL boundary that's swapped
  for a stub on the host, so `Stm32GpioDriver`, `Stm32UartDriver`, and
  `STM32Timer` all get real GTest coverage without an ARM toolchain. See
  ["Host unit testing"](lib/fprime-stm32/README.md#host-unit-testing-fprime-util-check)
  in `lib/fprime-stm32/README.md` for the pattern. `Stm32I2cDriver` follows
  the same `Common`/`Real`/`Stub` split but has no `test/ut/` files written
  yet — `register_fprime_ut` is scaffolded but commented out.
- **Hardware-in-the-loop integration tests** (`pytest` + the F´ GDS
  Integration Test API, against the real board): `led_integration_tests.py`
  and `uart_integration_tests.py` drive commands over the live ground link
  and assert on the resulting events/telemetry.

## Current follow-up work

- Re-verify the bootstrap-pool usage figure live on hardware (it's a runtime
  allocation count, not a static section size, so the host-only
  `baremetal-size` re-measurement above couldn't refresh it).
- Repeat the extended ground-link and GDS soak at the corrected system clock.
- Validate TIM2 rollover, interrupt masking, and long-duration stability.
- Continue hardware-in-the-loop automation for the STM32 target.
- Add persistent file support for the MicroFs-backed services.
- Run the full I2C exit-criterion soak (1,000-iteration read at 400 kHz with
  explicit event evidence for every injected error path — NACK, timeout, bus
  error), not just confirmed-working normal operation.
- `Stm32I2cDriver` has no host unit tests yet, despite already following the
  `Common`/`Real`/`Stub` split; write them.
- When adding another `fprime-sensors` family (e.g. `Bmp280` for Week 11's
  SPI driver), register only its `Types`/`Ports`/`Components` directly in
  `CMakeLists.txt` — don't add the whole library via `library_locations` or
  that family's own top-level `CMakeLists.txt`. Both pull in that family's
  bundled example `Subtopology`, which hardcodes a `Drv.Linux*Driver` type
  that doesn't exist in the fpp model on `stm32h7` and breaks the build. See
  `lib/fprime-stm32/README.md`'s "Adding a sensor" section for the exact
  pattern already used for `MpuImu`.
