include(CheckCXXSourceCompiles)

if(EXISTS "${CMAKE_SOURCE_DIR}/FprimeStm32BaremetalReference/config/fprime-stm32/Stm32Config.hpp")
    list(APPEND CMAKE_REQUIRED_INCLUDES "${CMAKE_SOURCE_DIR}/FprimeStm32BaremetalReference/config/fprime-stm32")
elseif(EXISTS "${CMAKE_CURRENT_LIST_DIR}/config/Stm32Config.hpp")
    list(APPEND CMAKE_REQUIRED_INCLUDES "${CMAKE_CURRENT_LIST_DIR}/config")
endif()

# ==========================================
# UART Driver Check
# ==========================================
check_cxx_source_compiles("
    #include \"Stm32Config.hpp\"
    #if !defined(USART_ENABLED) || (USART_ENABLED == 0)
    #error \"USART is disabled\"
    #endif
    int main() { return 0; }
 " HARDWARE_USART_ENABLED)

if(HARDWARE_USART_ENABLED)
    add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/STM32UartDriver")
endif()

# ==========================================
# GPIO Driver
# ==========================================
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/STM32GpioDriver")

# ==========================================
# Timer Driver Check
# ==========================================
check_cxx_source_compiles("
    #include \"Stm32Config.hpp\"
    #if !defined(TIM_ENABLED) || (TIM_ENABLED == 0)
    #error \"Timer is disabled\"
    #endif
    int main() { return 0; }
" HARDWARE_TIMER_ENABLED)

if(HARDWARE_TIMER_ENABLED)
    add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/STM32Timer")
endif()

# ==========================================
# I2C Driver Check
# ==========================================
check_cxx_source_compiles("
    #include \"Stm32Config.hpp\"
    #if !defined(I2C_ENABLED) || (I2C_ENABLED == 0)
    #error \"I2C is disabled\"
    #endif
    int main() { return 0; }
" HARDWARE_I2C_ENABLED)

if(HARDWARE_I2C_ENABLED)
    add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/STM32I2cDriver")
endif()