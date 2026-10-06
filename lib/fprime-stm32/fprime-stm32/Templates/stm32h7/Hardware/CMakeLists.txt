####
# Board hardware support for the FprimeStm32BaremetalReference project.
#
# This is deliberately owned by the project, not by the fprime-stm32
# submodule: fprime-stm32 only expects a target named `FprimeStm32` to exist
# and only supplies hardware-agnostic OSAL delegates/drivers on top of it.
# Swapping to different hardware means editing this directory (and
# regenerating stm32h753_hal/ from its .ioc in CubeMX), never forking the
# submodule.
####

if (FPRIME_PLATFORM STREQUAL "stm32h7")
    set(FPRIME_STM32_HAL_SOURCES
        stm32h753_hal/Core/Src/system_stm32h7xx.c
        stm32h753_hal/Core/Src/stm32h7xx_hal_msp.c
        # NOTE: stm32h753_hal/Core/Src/stm32h7xx_it.c is intentionally NOT
        # compiled into this static library. GNU ld only extracts an .o from a
        # static archive when something already linked has an *undefined*
        # reference into it -- but Hardware/startup/startup_stm32h753xx.s
        # already provides `.weak` Default_Handler aliases for every vector
        # table entry (and is linked directly into the executable, not via an
        # archive), so every vector is "satisfied" before the linker ever
        # needs to search this archive. That silently discards the real ISR
        # bodies (SysTick_Handler/HAL_IncTick, HardFault_Handler,
        # DMA1_Stream0/1_IRQHandler, TIM2_IRQHandler, ...) in favor of the weak
        # infinite-loop aliases. It is instead compiled directly into each
        # deployment executable's own sources -- see
        # FprimeStm32BaremetalReference/Deployments/ReferenceDeployment/CMakeLists.txt
        # -- so its strong definitions are always present and correctly
        # override the weak ones.
        config/src/stm32h7_clock.c
        stm32h753_hal/Core/Src/dma.c
        stm32h753_hal/Core/Src/gpio.c
        stm32h753_hal/Core/Src/i2c.c
        stm32h753_hal/Core/Src/tim.c
        config/src/tim2_clock.cpp
        stm32h753_hal/Core/Src/usart.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
        stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
    )

    add_library(FprimeStm32 STATIC ${FPRIME_STM32_HAL_SOURCES})

    target_include_directories(FprimeStm32 PUBLIC
        "${PROJECT_SOURCE_DIR}"
        "${CMAKE_BINARY_DIR}"
        "${FPRIME_PROJECT_ROOT}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${CMAKE_CURRENT_LIST_DIR}/config/include"
        "${CMAKE_CURRENT_LIST_DIR}/stm32h753_hal/Core/Inc"
        "${CMAKE_CURRENT_LIST_DIR}/stm32h753_hal/Drivers/CMSIS/Include"
        "${CMAKE_CURRENT_LIST_DIR}/stm32h753_hal/Drivers/CMSIS/Device/ST/STM32H7xx/Include"
        "${CMAKE_CURRENT_LIST_DIR}/stm32h753_hal/Drivers/STM32H7xx_HAL_Driver/Inc"
    )

    target_compile_definitions(FprimeStm32 PUBLIC STM32H753xx USE_HAL_DRIVER USE_PWR_LDO_SUPPLY)

    target_compile_options(FprimeStm32 PRIVATE -ffunction-sections -fdata-sections)

    target_link_libraries(FprimeStm32 PUBLIC PlatformStm32h7)
endif()