# F´ STM32H7 Platform Support

This repository contains the STM32H7 hardware, OSAL, and driver support used by the
bare-metal F´ reference deployment. It targets the STM32H753XI-EVAL2 evaluation
board and is intended to be reusable as a Git submodule.

The implementation is designed for a bare-metal cyclic executive:

- No FreeRTOS or other operating system is required.
- `Os::Task` is cooperative and creates no thread.
- `Os::Mutex` provides bounded critical sections by preserving and restoring
  Cortex-M7 `PRIMASK`.
- `Os::Queue` is a fixed-depth, interrupt-safe FIFO allocated during startup.
- `Os::RawTime` provides a race-safe microsecond clock from a free-running TIM2
  counter and overflow interrupt.
- DMA buffers are placed in DMA-accessible AXI SRAM rather than DTCM.

## Supported hardware

The current configuration is for the STM32H753XIH6 on the
STM32H753XI-EVAL2:

- Cortex-M7, 2 MiB Flash, 1 MiB RAM
- 25 MHz HSE input and a 480 MHz PLL1 system clock
- USART1 through the embedded ST-LINK-V3E VCP
- USART1 TX on PB14 and RX on PB15
- Direct user LEDs on PF10 (LED1, green) and PA4 (LED3, red)
- TIM2 as the 1 MHz, 32-bit free-running time source

LED2 and LED4 are routed through the external MFX I2C expander and are not
used by the direct bare-metal GPIO examples.

## Contents

- `Os/Stm32H7`: STM32H7 bare-metal delegates for Task, Mutex, Queue, and RawTime
- `Drv/STM32GpioDriver`: passive GPIO input/output driver
- `Drv/STM32Timer`: channel-2 output-compare tick driver, instance-selectable
  (`Stm32::TimerInstance::Tim1`/`Tim2`/`Tim3`/`Tim4`/`...`) via `open()`
- `Drv/STM32UartDriver`: USART1 DMA-backed byte-stream driver
- `Drv/STM32I2cDriver`: blocking/polled I2C master driver (`Drv.I2c`)
- `Drv/config`: driver-tuning headers (e.g. `UartDriverConfig.hpp`) and
  `Stm32Config.hpp`, the per-peripheral-instance enable/disable switchboard
  used by every driver below (see "Enabling and selecting peripheral
  instances")
- `Allocator`: fixed-pool bootstrap allocator + newlib `--wrap` traps
  enforcing the no-heap-after-bootstrap rule -- fully hardware-agnostic, so
  every project gets it without hand-rolling one
- `Core/CortexM7`: ARM-core-level helpers (currently just D-cache
  maintenance), grouped by core rather than by ST family since they only
  depend on which Cortex-M core is in use

This library is hardware-agnostic: it contains no CubeMX-generated code and
no board-specific source. It only expects a CMake target named `FprimeStm32`
to already exist -- built and exposed by the *consuming* project, along with
its public include paths for `main.h`, `stm32h7xx_hal_conf.h`, and the rest
of the CubeMX/HAL headers. In this repository that target is defined in
`FprimeStm32BaremetalReference/Hardware/CMakeLists.txt`, which builds the actual
CubeMX-generated project (regenerable in place from its own `.ioc` file) plus
a handful of hand-written, project-specific clock/tick-source glue (which
timer backs `Os::RawTime`, the exact PLL/oscillator sequence -- these are
peripheral-*role* choices baked into one board's `.ioc`, not family-wide
constants, so they stay project-owned even though they rarely change). This
split means retargeting this library to different hardware never requires
forking it -- only editing the consuming project's own `Hardware/` directory.
The real interrupt implementation (`stm32h7xx_it.c`) must be linked directly
into each deployment executable, rather than only through the `FprimeStm32`
static library, so its strong handlers override the startup file's weak
`Default_Handler` aliases -- see the NOTE in
`FprimeStm32BaremetalReference/Hardware/CMakeLists.txt`.

### Enabling and selecting peripheral instances

`Stm32UartDriver`, `Stm32I2cDriver`, and `STM32Timer` all resolve their
peripheral instance (`USART1_UART_INSTANCE`, `I2C1_INSTANCE`, `TIM2_INSTANCE`,
...) through `#define`s in `Drv/config/Stm32Config.hpp`, each defaulting to
`false` except the instances this reference board already uses. A driver's
`toHalHandle(instance)` returns `nullptr` for a `false` instance, and the
calling code immediately `FW_ASSERT`s on that `nullptr` — selecting a
disabled instance in an `open()` call is a build-time configuration mistake,
not a runtime condition to gracefully handle.

The library's copy of `Stm32Config.hpp` is the default; a consuming project
overrides it by placing its own copy at the same relative path under its
`settings.ini`-configured `config_directory` (this project's override lives
at `FprimeStm32BaremetalReference/config/fprime-stm32/Stm32Config.hpp`). When
retargeting this library to a new board, edit only the override copy — enable
the instances your `.ioc` actually configured, and pass the matching enum
value (`Stm32::I2cInstance::I2c1`, `Stm32::TimerInstance::Tim2`, ...) to the
driver's `open()` call from the topology's `configureTopology()`. Each
sensor's own `docs/sdd.md` under `lib/fprime-sensors` documents this
enable-then-select pairing for its specific port.

### Adding a new chip family

`Os/Stm32H7` and the `Drv/STM32*` drivers only reach HAL functionality
through CubeMX's own per-peripheral headers (`main.h`, `gpio.h`, `usart.h`,
`i2c.h`, `tim.h`, `dma.h`) -- CubeMX generates these under the *same* names
for every STM32 family, unlike the family-named umbrella headers
(`stm32h7xx_hal.h`, `stm32f4xx_hal.h`, ...). Never include a family-named HAL
header directly from library code; include the matching CubeMX per-peripheral
header instead (it chains to the right family headers with the right
pre-defines already set up). This is what keeps `Drv/STM32*` family-portable
without any per-family duplication.

`Os/Stm32H7` itself *is* family-specific (its name says so) because OSAL
backends for different families are mutually exclusive per build. To add a
new family: create `Os/Stm32<Family>/` alongside it with the same four
`register_os_implementation` calls (`SUFFIX` = the new family name), and add
an `elseif (FPRIME_PLATFORM STREQUAL "stm32<family>")` branch in
`Os/CMakeLists.txt`. The `Drv/STM32*/CMakeLists.txt` real/stub selection
already matches any `stm32*` platform, so no change is needed there.

## Integration and build

The parent F´ project selects this platform through its `stm32h7` CMake
configuration. From the parent project:

```shell
source fprime-venv/bin/activate
fprime-util generate -f
fprime-util build -j"$(nproc)"
```

To build the reference deployment directly after generation:

```shell
ninja -C build-fprime-stm32h7 ReferenceDeployment
```

The deployment must call the hardware initialization routines in this order:

1. `HAL_Init()`
2. `FprimeStm32_ClockInit()`
3. `SCB_EnableICache()` and `SCB_EnableDCache()`
4. `Stm32_Tim2ClockInit()`
5. topology setup and the cyclic executive

The clock initialization is required before configuring TIM2. The cache
initialization is required before using the DMA cache-maintenance helpers;
Cortex-M7 cache tags and data are undefined at reset.

## USART1 DMA ground link

`Drv::Stm32UartDriver` provides a non-blocking USART1 DMA transport over PB14/PB15
through the ST-LINK-V3E VCP. It uses fixed-size TX and RX rings, aligned DMA
staging buffers, idle-line detection for RX, and a polled DMA state machine so
the cyclic executive is never blocked waiting for serial I/O.

The driver was validated on the physical STM32H753XI-EVAL2 with:

- 115200 baud, 8N1 operation
- DMA on both transmit and receive
- 183 KiB transmitted in 180 seconds with zero TX/RX error counts
- 6,144 bytes captured from the VCP in 6.0 seconds, matching the driver's
  internal byte counter
- 96 injected uplink bytes received and drained without buffer leaks
- CCSDS Space Packet framing through the `ComCcsds` subtopology
- A continuous 21+ minute `fprime-gds` session with command uplink, telemetry
  downlink, and event downlink

The GDS validation decoded more than 12,000 telemetry samples at approximately
10 samples per second. The command round trip included `CMD_NO_OP`, string
commands, and an oversized string that correctly returned `FORMAT_ERROR`.

The ground-link tests above were performed before the September 3 clock-tree
fix, while the board was still running from the approximately 64 MHz HSI. The
USART baud rate self-adjusted from the live peripheral clock query, so the link
remained valid, but an extended ground-link soak at the corrected PLL clock is
still required.

## I2C bus (`Stm32I2cDriver`)

`Stm32::Stm32I2cDriver` implements the framework's `Drv.I2c` interface
(guarded, synchronous `write`/`read`/`writeRead` ports, each returning
`Drv::I2cStatus` directly) against I2C1 on PB6/PB7 (SCL/SDA). Unlike
`Stm32UartDriver`, it is deliberately blocking/polled, not interrupt-driven:
`HAL_I2C_MspInit()` only enables the peripheral clock/GPIO, no NVIC
event/error interrupt is armed, and every `HAL_I2C_Master_Transmit`/`_Receive`
call is bounded by a fixed 10 ms watchdog passed as the HAL's own `Timeout`
parameter. This is a deliberate choice, not a shortcut: `Drv.I2c` is a
synchronous contract (the same one `Drv::LinuxI2cDriver` implements by
blocking on `ioctl`), the cyclic executive has no thread to free up by not
blocking, and a real I2C transaction at 400 kHz is sub-millisecond. Interrupts
would only earn their complexity back for `writeRead`'s one real limitation:
it is two back-to-back blocking calls (STOP then START), not a single
electrically-held repeated START, which is safe on this single-master bus but
would need the sequential IT/DMA API (with I2C1's NVIC interrupt enabled) for
a sensor that strictly requires the bus held across the register-address
write.

`open(instance, busSpeed)` selects both the peripheral (`I2cInstance::I2c1`
today; `I2c2`/`I2c3`/`I2c4` are declared for other boards but not yet
CubeMX-configured) and a bus speed preset (`I2cBusSpeed::Standard`/`Fast`/
`FastPlus` — CubeMX-computed `Timing` register values for this project's
actual D2PCLK1 clock, since the H7 I2C peripheral has no runtime baud-rate
formula the way UART does). It follows the same `Common`/`Real`/`Stub` HAL
boundary convention described below, with one variant: `open()` itself is
implemented directly in `Stm32I2cDriver.cpp`/`Stm32I2cDriverStub.cpp` rather
than delegating to a private `hwOpen()`, since (unlike UART's DMA/ISR setup)
there's no separate hardware-independent work for a shared `Common.cpp` to do
around it.

Validated live against a real MPU-6050 IMU (wake-up register write, then
repeated 14-byte accel/gyro/temp reads) — see "Adding a sensor" below for how
that's wired without any application code touching the I2C bus directly.

### Adding a sensor

Connect a sensor's ports directly to `Stm32I2cDriver`'s `write`/`read`/
`writeRead` — if the sensor component already imports the framework's
`Drv.I2c`/`Drv.I2cWriteRead` port types (check its `.fpp`), no adapter
component is needed. This is exactly how the MPU-6050 was wired:
`fprime-sensors`' `MpuImu.ImuManager` (a `queued` component with its own
internal reset/enable/configure/read state machine) declares `busWrite:
Drv.I2c` / `busWriteRead: Drv.I2cWriteRead` output ports, connected straight
to `i2cDriver.write`/`i2cDriver.writeRead` in `Top/topology.fpp`. Its standard
command/event/telemetry/param/time ports wire themselves via the topology's
existing pattern-graph specifiers (`command connections instance
CdhCore.cmdDisp`, etc.) — the only manual connections needed were the two I2C
ports and a rate-group tick into its `run` port.

**Don't use the sensor library's own bundled example `Subtopology`
(`MpuImuSubtopologyConfig.fpp` etc.), and don't add the whole library via
`settings.ini`'s `library_locations`.** Those bundled Subtopologies hardcode a
`Drv.Linux*Driver` instance type (e.g. `Drv.LinuxI2cDriver` for `MpuImu`,
`Drv.LinuxSpiDriver` for `Bmp280`). Since Linux-only driver modules are
skipped outright on `stm32h7` — not just their C++ target, the type doesn't
exist in the fpp model at all — `fpp-to-cpp` fails with `"symbol Drv is not
defined"` the instant any bundled Subtopology config gets registered, whether
or not the deployment references it. Instead, register only the specific
modules actually needed (for `MpuImu`: `Helpers`, `MpuImu/Types`,
`MpuImu/Ports`, `MpuImu/Components`) directly via `add_fprime_subdirectory` in
the project's top-level `CMakeLists.txt`, skipping each family's own
top-level `CMakeLists.txt` (that's what pulls in `Subtopology`). Because the
library's own sources `#include "fprime-sensors/..."` (paths relative to the
library root), also add `lib/fprime-sensors` as a plain
`include_directories()` root — both the source tree and
`${CMAKE_CURRENT_BINARY_DIR}/lib/fprime-sensors`, where fpp generates the
matching `*Ac.hpp` headers when the library isn't registered through
`library_locations`.

## Hardware validation

The complete reference topology has been run on the physical board with the
non-blocking cyclic executive and cooperative dispatch enabled. Validation
included:

- Topology setup and all active-component queues created successfully
- No hits on assertion, abort, exit, HardFault, BusFault, or fatal-handler
  breakpoints during the recorded endurance runs
- PF10 LED activity, driven exclusively through `Drv::Stm32GpioDriver` (wired
  into `instances.fpp`/`topology.fpp` and opened from `configureTopology()`),
  confirmed via a GDS-based integration test (`led_integration_tests.py`)
  rather than manual `GPIOF_ODR` register polling
- TIM2 measured at approximately 997.9 kHz over an undisturbed 30-second
  interval after PLL clock initialization
- The 100 Hz timer tick and rate-group tick counters remained synchronized
- USART1 DMA continued transmitting correctly after the corrected clock was
  enabled

The clock-tree bug was fixed by calling `FprimeStm32_ClockInit()` after
`HAL_Init()`. Before that change, TIM2 advanced at approximately 269.5 kHz,
which matched the unconfigured 64 MHz HSI divided by the intended TIM2
prescaler. The corrected implementation selects PLL1 and restores the intended
480 MHz system clock.

## Memory and placement

The STM32 linker configuration defines these regions:

| Region | Capacity |
| --- | ---: |
| `FLASH` | 2 MiB |
| `AXI_SRAM` | 512 KiB |
| `DTCM_RAM` | 128 KiB |

CPU-only framework state can be placed in DTCM, while DMA-visible buffers and
the bootstrap allocation pool remain in AXI SRAM. The latest recorded
`baremetal-size stm32h7` result for the complete reference deployment was:

| Region | Used | Remaining |
| --- | ---: | ---: |
| Flash (`.text`+`.data`) | 645,708 bytes | 69.2% |
| AXI SRAM `.bss` | 261,308 bytes | 50.1% |
| DTCM `.dtcm_bss` | 8,584 bytes | 93.5% |
| Bootstrap pool | 112,656 of 131,072 bytes | 14.1% |

Flash and AXI SRAM `.bss` grew modestly from the Week 8 `led`/`gpioDriver`
topology wiring and the Week 9 `Common`/`Real`/`Stub` driver split (a few new
members per driver); DTCM `.dtcm_bss` is unchanged byte-for-byte. The
bootstrap-pool row is a runtime allocation count rather than a static ELF
section, so it's carried over from the last hardware run and still needs
live re-verification.

The reference deployment locks the bootstrap allocator after topology setup and
wraps the C heap symbols so post-initialization allocations assert instead of
silently using an unbounded heap. New components should be evaluated against
both the AXI SRAM margin and the remaining bootstrap-pool capacity.

## Host unit testing (`fprime-util check`)

Every driver under `Drv/` splits into three files sharing one HAL-free
header, so `fprime-util check` can compile and run its GTest unit test on
the host (x86_64 Linux) without any ARM/CMSIS toolchain:

- `<Driver>Common.cpp` — hardware-independent logic (validation, ring
  buffers, state machines, event/telemetry emission). Always built, on
  every platform. This is where unit tests get real coverage.
- `<Driver>.cpp` — the real implementation, built only for the `stm32h7`
  target. Every HAL/CMSIS touch (register access, `HAL_*` calls, ISR
  callbacks) lives here behind a small set of private boundary methods
  (named `hw*`) declared in the header. This is the only file allowed to
  `#include` a vendor CMSIS/HAL header.
- `<Driver>Stub.cpp` — built only for host unit tests. Implements the same
  `hw*` boundary methods with fixed, no-HAL-dependency behavior (e.g.
  "always succeeds," a settable fake counter). Never included in a
  flight build.

Each driver's `CMakeLists.txt` always registers the production module
(`register_fprime_module`/`register_fprime_library`) — only the choice of
`<Driver>.cpp` vs `<Driver>Stub.cpp` (and the matching `DEPENDS`) is
platform-conditional — and always registers `register_fprime_ut` (never
gated by `restrict_platforms`, which would make the UT target itself
unreachable and `fprime-util check` fail with `NoTargetFoundException`).
`FprimeStm32` (the real vendor HAL static library) and its `Os/`
subdirectory are gated to the `stm32h7` target in this directory's own
`CMakeLists.txt`.

**Adding a new driver:** don't add `#ifdef BUILD_UT`/`#ifndef` to
production code. If the driver only needs HAL calls that map cleanly onto
a boundary method, follow the `Common`/`Real`/`Stub` split above (copy an
existing driver's `CMakeLists.txt`). If a routine is genuinely hard to
fake (e.g. an ISR callback with no user-context pointer, like
`HAL_UART_TxCpltCallback`), do what `Stm32UartDriver`/`STM32Timer` do:
route it through a public `signalX()`/`hwArmY()` method on the component
so a unit test can call it directly to simulate the hardware event, and
keep a single-instance callback trampoline (a file-scope pointer set once
in the real `open()`) in the real `.cpp` only.

Verification commands:

```sh
fprime-util generate --ut -f   # regenerate the host/native UT build cache
fprime-util check              # from a driver's directory: build + run its UT
fprime-util check --coverage   # same, plus a line/function/branch coverage report
```

## Known follow-up work

- Repeat the extended USART1 DMA and GDS soak at the corrected 480 MHz clock.
- Validate TIM2 rollover, interrupt masking, and long-duration stability.
- Continue hardware-in-the-loop automation for the STM32 target.
- Add real persistent file support for the MicroFs-backed services; the current
  conservative configuration recognizes only `/bin<N>/file<M>` paths.
- Re-verify the bootstrap-pool usage figure in "Memory and placement" live on
  hardware; it's a runtime allocation count, not a static ELF section, so it
  couldn't be refreshed by the host-only `baremetal-size` re-measurement.
- `Stm32I2cDriver` has no `test/ut/` files yet, despite already following the
  `Common`/`Real`/`Stub` split (`register_fprime_ut` is scaffolded but
  commented out in its `CMakeLists.txt`) — write them.
- Run the full I2C exit-criterion soak on real hardware: 1,000 iterations at
  400 kHz with explicit event evidence for every injected error path (NACK,
  timeout, bus error), not just confirmed-working normal operation.
- If a sensor needs a true repeated START (loses its register pointer across
  a STOP), upgrade `writeRead` to the sequential IT/DMA API with I2C1's NVIC
  interrupt enabled — the current STOP-then-START is safe on this
  single-master bus but isn't electrically a repeated START.
