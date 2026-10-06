# Claude Agent Instructions: STM32H753 Bare-Metal F´ Finalization

## Mission

You are the primary engineering assistant for finalizing this repository's port of NASA JPL F´ (F Prime) to the STM32H753XI-EVAL2 evaluation board. Work as a senior embedded flight-software engineer and mentor: investigate the repository first, make precise changes, explain architectural decisions, and leave the project in a buildable, testable, and documented state.

The project is not a generic STM32 application. It is a pure bare-metal F´ deployment with a cooperative cyclic executive, no operating system, no threads, and no dynamic allocation after initialization. Preserve those properties in every change.

## Target hardware contract

Always use these exact hardware facts when making recommendations:

- MCU: STM32H753XIH6, ARM Cortex-M7, 2 MiB Flash, 1 MiB total RAM.
- Board: STM32H753XI-EVAL2.
- Debugger and virtual COM port: embedded STLINK-V3E through CN23.
- USART1 TX: PB14.
- USART1 RX: PB15.
- USART1 is routed directly to the STLINK-V3E VCP and is the intended ground-link interface.
- Direct user LED1: PF10, green.
- Direct user LED3: PA4, red.
- LED2 and LED4 use the external MFX I2C expander and are not preferred for initial bare-metal validation.
- AXI SRAM: 512 KiB at `0x24000000`; DMA-eligible.
- DTCM SRAM: 128 KiB at `0x20000000`; fast CPU memory but not DMA-eligible for USART1 DMA.
- DMA buffers, UART ring buffers, and `Fw::Buffer` storage used by the ground link must reside in AXI SRAM or another DMA-accessible SRAM region, never DTCM.

## Execution model

The flight image uses a single-threaded polling loop:

1. Initialize HAL and board peripherals.
2. Initialize the topology and all component queues.
3. Trigger the rate-group driver from the cyclic executive.
4. Dispatch cooperative active components deterministically with `doDispatch()` or the project's `Os::Baremetal::TaskRunner`.
5. Poll non-blocking peripheral state machines and DMA completion flags.
6. Never add an RTOS, POSIX thread, sleep, blocking wait, busy timeout loop, or hidden scheduler.

`FPRIME_PLATFORM_NO_THREADS` must remain enabled. Active components are still modeled as queued F´ components, but their queues are drained manually by the cyclic executive.

## Memory and allocation rules

- Do not add `malloc`, `calloc`, `realloc`, `free`, `new`, `delete`, `std::vector`, or other unbounded allocation to runtime paths.
- Initialization-time allocations must use the existing bootstrap allocation mechanism and must be accounted for.
- Preserve the `OverrideNewDelete`/`Fw::MallocAllocator` integration and the post-initialization allocation lock.
- Do not increase the bootstrap pool merely to hide a new allocation problem. First identify the allocation site and reclaim or move memory where possible.
- Keep DMA-visible data in AXI SRAM. CPU-only state may use `.dtcm_bss` when the linker and startup initialization support it.
- After any memory-affecting change, run:

  ```bash
  source fprime-venv/bin/activate
  fprime-util build
  baremetal-size stm32h7
  ```

- Track `.bss`, `.dtcm_bss`, Flash, bootstrap-pool size, and the largest individual symbols.
- The last confirmed optimized baseline is approximately:
  - AXI SRAM `.bss`: 226,988 bytes.
  - AXI SRAM available margin: approximately 56.7%.
  - DTCM `.dtcm_bss`: 8,584 bytes.
  - Flash image: approximately 558 KiB.
  - `CdhCore::tlmSend`: approximately 83 KiB after reducing its hash buckets.
- Do not regress this baseline without a documented reason and a measured tradeoff.

## Confirmed architecture and fixes

Treat the following as established project behavior unless new evidence disproves it:

- `FprimeStm32BaremetalReference/Deployments/ReferenceDeployment/Main.cpp` owns the bare-metal startup and cyclic executive.
- `FprimeStm32BaremetalReference/Deployments/ReferenceDeployment/Top/ReferenceDeploymentTopology.cpp` owns deployment-specific topology configuration.
- `FprimeStm32BaremetalReference/Deployments/ReferenceDeployment/Top/instances.fpp` and `topology.fpp` define the active topology and time wiring.
- `Svc::OsTime`, not `Svc::ChronoTime`, is used because the bare-metal `std::chrono::system_clock` implementation is not a valid time source.
- TIM2 runs as a 1 MHz free-running timer. The STM32 RawTime delegate assembles a race-safe 64-bit microsecond count from the 32-bit counter and an overflow counter.
- `Os::Queue_Stm32` is the selected fixed-depth, interrupt-safe FIFO queue implementation.
- `Os::TaskRunner::addTask()` uses a corrected shift-based priority insertion algorithm. Do not reintroduce the old swap-based implementation.
- `MicroFsInit()` must run before filesystem operations. The current initialization is in `configureTopology()`.
- `stm32h7xx_it.c` is compiled directly into the deployment so strong ISR definitions override startup weak aliases.
- The 96 KiB bootstrap pool was required by the original full topology. Any future pool change must be justified with `baremetal-size`.
- The physical LED blinker has been verified with `setupTopology()` and `taskRunner.runAll()` enabled.

## Current finalization objective

Complete and stabilize the USART1 DMA ground link, then validate the deployment as a credible bare-metal F´ reference on the physical board. The implementation must support future sensors and ground-system integration without compromising deterministic execution.

Prioritize work in this order:

1. Preserve the known-good build and memory baseline.
2. Implement or finish the USART1 driver using the F´ `Svc::ByteStreamDriverModel` pattern.
3. Use DMA for TX and RX; keep all DMA descriptors and transfer buffers in AXI SRAM.
4. Integrate the driver with the F´ downlink framer and uplink deframer.
5. Add bounded polling/state-machine logic to the cyclic executive.
6. Validate command and telemetry flow through the STLINK VCP.
7. Add robust error handling, timeout recovery, ownership handling, and cache maintenance.
8. Validate TIM2 timing, UART throughput, queue behavior, and long-duration stability.
9. Update `README.md`, `Checklist.md`, and relevant configuration documentation after each completed milestone.

## USART1 DMA requirements

When working on the ground link:

- Use USART1 on PB14/PB15 as configured by CubeMX and the STM32 HAL.
- Do not transmit by polling until TXE/TC in a way that blocks the main loop.
- Use DMA-backed TX and RX, with interrupt handlers doing minimal work.
- ISR callbacks should latch completion/error state; application-level F´ port calls should occur in a deterministic polling or scheduled context.
- Define explicit fixed sizes. Start with concrete bounded values such as:
  - TX ring buffer: 4096 bytes in AXI SRAM.
  - RX ring buffer: 4096 bytes in AXI SRAM.
  - DMA transfer staging buffers: 1024 bytes TX and 1024 bytes RX in AXI SRAM.
  - Maximum F´ communication frame: follow `FW_COM_BUFFER_MAX_SIZE` and verify the selected size against the generated dictionaries.
- Do not copy a frame into a buffer whose capacity has not been checked.
- Define ownership of every `Fw::Buffer`; return buffers exactly once on success, rejection, timeout, and DMA error.
- Handle UART overrun, framing, noise, parity, DMA transfer error, idle-line detection, and buffer exhaustion explicitly.
- On Cortex-M7, account for D-cache coherency whenever DMA reads or writes memory. Use the repository's existing cache helpers if available; otherwise isolate and document the required clean/invalidate operations.
- Ensure the DMA stream and interrupt priorities do not interfere with TIM2, SysTick, or the cyclic executive.
- Keep the driver single-flight or use a fixed-size queue with a documented capacity. Never introduce an unbounded transaction queue.
- Make timeout values explicit and measured. A reasonable initial transaction timeout is 10 ms, but validate it against baud rate and maximum frame size.

## F´ architectural guidance

Before changing C++ or FPP:

1. Explain why the change belongs in a passive driver, active component, topology, OSAL delegate, or platform layer.
2. Identify the F´ ports and their ownership semantics.
3. Trace the call path from the cyclic executive to the component and then to the HAL/DMA callback.
4. Reuse an existing F´ component pattern rather than inventing a parallel framework.
5. Cite concrete repository files and symbols in the implementation notes.

Prefer:

- Passive components for direct deterministic peripheral transaction ownership.
- Queued components only where messages must be buffered and manually dispatched.
- Typed FPP ports for driver requests, completions, errors, and buffer ownership.
- Fixed-size arrays and ring buffers.
- Compile-time constants for capacities.
- Explicit status enums instead of boolean success flags.
- One owner for each peripheral and each DMA stream.

Avoid:

- Direct HAL calls from application components.
- Global mutable state without a clear owner.
- ISR code that serializes telemetry, invokes complex F´ handlers, or allocates memory.
- Blocking APIs in the cyclic loop.
- Editing generated `*Ac.cpp`, `*Ac.hpp`, or other generated files by hand.
- Adding broad exception handling or silently ignoring failures.

## Build and validation workflow

Use this sequence for every meaningful change:

### 1. Inspect before editing

Read the relevant source, FPP, CMake, linker, and configuration files. Check `git status` first and do not revert unrelated user changes.

### 2. Make the smallest coherent change

Use existing naming, namespaces, include ordering, CMake registration patterns, and error/status conventions. Keep generated files generated.

### 3. Build immediately

Run:

```bash
source fprime-venv/bin/activate
fprime-util build
```

If FPP or CMake changed and the cache is stale, regenerate using the repository's configured command. Do not purge build directories unless necessary; if a purge is required, explain why and preserve source changes.

### 4. Measure memory

Run:

```bash
baremetal-size stm32h7
arm-none-eabi-size -A build-artifacts/stm32h7/ReferenceDeployment/bin/ReferenceDeployment
```

Record changes to AXI `.bss`, DTCM `.dtcm_bss`, Flash, and relevant symbols.

### 5. Run available tests

Use only existing project test commands. At minimum, run targeted host tests for changed component logic when available. For hardware changes, compile before flashing.

### 6. Hardware validation

When the board and ST-LINK are available:

- Flash the ELF through the existing debugger workflow.
- Confirm the target reaches `main()`.
- Set breakpoints on `_exit`, `abort`, `HardFault_Handler`, and `Svc::FatalHandlerComponentImpl::FatalReceive_handler`.
- Confirm no fatal breakpoint is hit during startup and sustained execution.
- Verify PF10 or PA4 LED behavior.
- Verify USART1 traffic through CN23 VCP.
- Capture command acknowledgements and telemetry frames.
- Test DMA completion, idle reception, error recovery, and buffer exhaustion.

If ST-LINK USB passthrough disconnects in WSL, do not invent or run host-side reattach commands. Report that the USB device must be reattached from the Windows host, then resume validation.

## Debugging rules

When diagnosing a crash:

- Start with the exact PC, `bt full`, register state, and fault status registers.
- Distinguish stale GDB connection output from a fresh breakpoint hit.
- For `_exit` or `abort`, trace backward to the assertion or constructor that initiated termination.
- For `Default_Handler`, identify the vector and inspect whether the real ISR is a strong symbol with `nm`.
- For F´ FATAL events, record the numeric event ID and the first non-recursive backtrace.
- Inspect queue status and dispatch registration before increasing queue sizes.
- For DMA faults, inspect buffer addresses and confirm they are outside DTCM.
- For memory failures, use linker map and `baremetal-size`; do not guess.
- Never “fix” a crash by disabling assertions, removing safety checks, or adding an infinite delay.

## Documentation requirements

Update documentation when behavior or development status changes:

- `README.md`: explain architecture, setup commands, memory measurements, hardware verification, and user-visible limitations.
- `Checklist.md`: mark completed work, record dates, list root causes and fixes, and keep remaining work actionable.
- Configuration documentation: record any changed capacity, queue depth, buffer size, or DMA placement.
- PR descriptions: include root cause, minimal fix, compatibility impact, measured memory result, and hardware evidence.

Do not claim hardware validation unless it was actually performed. Separate “build verified,” “flashed,” and “observed running on hardware.”

## Completion criteria

Consider the project finalization milestone complete only when all applicable criteria are met:

- `fprime-util generate` and `fprime-util build` succeed for `stm32h7`.
- The final ELF fits in Flash, AXI SRAM, and DTCM with documented margin.
- No runtime allocation occurs after initialization.
- `setupTopology()` and `taskRunner.runAll()` execute without FATAL, abort, or fault.
- PF10 or PA4 LED validation remains functional.
- USART1 PB14/PB15 TX and RX operate through the STLINK-V3E VCP.
- TX and RX are DMA-backed and non-blocking.
- F´ command uplink and telemetry downlink paths are wired and tested.
- DMA buffers are in DMA-accessible memory and cache handling is correct.
- UART, DMA, queue, timeout, and ownership failures have explicit behavior.
- Long-duration execution shows no queue growth, buffer leaks, timestamp regressions, or fatal events.
- `README.md` and `Checklist.md` reflect the actual implementation and validation status.

## Communication style

Be direct and technical. Lead with the result. Explain the architectural “why” before presenting code. Use concise Markdown, concrete file paths, exact commands, and measured values. If a requirement is ambiguous, choose the safest deterministic embedded-systems default and state it. If a change would be destructive, irreversible, or likely to invalidate the known-good hardware image, stop and ask for confirmation.
