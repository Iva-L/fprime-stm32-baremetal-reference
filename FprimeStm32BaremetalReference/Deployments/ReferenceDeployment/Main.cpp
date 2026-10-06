// ======================================================================
// \title Main.cpp
// \author ivanlara
// \brief Bare-metal cyclic executive entry point with integrated hardware LED blinker.
// ======================================================================
#include <fprime-stm32/Allocator/BootstrapAllocator.hpp>
#include <ReferenceDeployment/Top/ReferenceDeploymentTopology.hpp>
#include <ReferenceDeployment/Top/ReferenceDeploymentTopologyAc.hpp>

#include <Os/Os.hpp>
#include <fprime-baremetal/Os/TaskRunner/TaskRunner.hpp>
#include <fprime-stm32/Drv/STM32Timer/STM32Timer.hpp>
#include <fprime-stm32/Drv/STM32UartDriver/Stm32UartDriver.hpp>
#include <main.h>
#include <stm32h7_clock.h>
#include <tim2_clock.h>
#include "stm32h7xx_hal.h"

int main() {
    
    SCB_EnableICache();
    SCB_EnableDCache();

    HAL_Init();

    // Initialize the system clocks, including the HSE->PLL1 480 MHz configuration.
    FprimeStm32_ClockInit();
    Stm32_Tim2ClockInit();
    
    Os::init();
    Os::Baremetal::TaskRunner& taskRunner = Os::Baremetal::TaskRunner::getSingleton();

    ReferenceDeployment::TopologyState inputs = {};
    ReferenceDeployment::setupTopology(inputs);
    Stm32::lockBootstrapAllocator();

    while (true) {

        // Run a cooperative state machine step for each active registered component.
        taskRunner.runAll();

        // Poll the TIM2 CH2 hardware tick source and the UART driver.
        ReferenceDeployment::timer.poll();
        ReferenceDeployment::comDriver.poll();
    }
}