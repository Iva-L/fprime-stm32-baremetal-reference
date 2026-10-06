// ======================================================================
// \title  Stm32I2cDriverCommon.cpp
// \author ivanlara
// \brief  Hardware-independent port handler logic for the STM32H7 I2C
//         blocking master driver: shared by the real (stm32h7) and stubbed
//         (host UT) builds. Contains no HAL/CMSIS dependency -- every
//         register touch is behind hwMasterTransmit()/hwMasterReceive()
//         (declared in Stm32I2cDriver.hpp) or open() itself, implemented in
//         Stm32I2cDriver.cpp (real) / Stm32I2cDriverStub.cpp (host).
// ======================================================================

#include <fprime-stm32/Drv/STM32I2cDriver/Stm32I2cDriver.hpp>
#include <Fw/Types/Assert.hpp>

namespace Stm32 {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Stm32I2cDriver ::Stm32I2cDriver(const char* const compName) : Stm32I2cDriverComponentBase(compName), m_opened(false) {}

Stm32I2cDriver ::~Stm32I2cDriver() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

Drv::I2cStatus Stm32I2cDriver ::write_handler(const FwIndexType portNum, U32 addr, Fw::Buffer& serBuffer) {
    if (!this->m_opened) {
        return Drv::I2cStatus::I2C_OPEN_ERR;
    }
    FW_ASSERT(serBuffer.getData() != nullptr);
    FW_ASSERT_NO_OVERFLOW(addr, U16);
    FW_ASSERT_NO_OVERFLOW(serBuffer.getSize(), U16);

    return this->hwMasterTransmit(static_cast<U16>(addr), serBuffer.getData(), static_cast<U16>(serBuffer.getSize()));
}

Drv::I2cStatus Stm32I2cDriver ::read_handler(const FwIndexType portNum, U32 addr, Fw::Buffer& serBuffer) {
    if (!this->m_opened) {
        return Drv::I2cStatus::I2C_OPEN_ERR;
    }
    FW_ASSERT(serBuffer.getData() != nullptr);
    FW_ASSERT_NO_OVERFLOW(addr, U16);
    FW_ASSERT_NO_OVERFLOW(serBuffer.getSize(), U16);

    return this->hwMasterReceive(static_cast<U16>(addr), serBuffer.getData(), static_cast<U16>(serBuffer.getSize()));
}

// I2C1 is configured in blocking/polled mode only (no NVIC event/error IRQ
// enabled for I2C1 -- see HAL_I2C_MspInit() in src/i2c.c), so there is no
// sequential-transfer (HAL_I2C_Master_Seq_*_IT with I2C_FIRST_FRAME/
// I2C_LAST_FRAME) path available to hold the bus between the write and the
// read with a single electrically-held repeated START.
Drv::I2cStatus Stm32I2cDriver ::writeRead_handler(const FwIndexType portNum,
                                                    U32 addr,
                                                    Fw::Buffer& writeBuffer,
                                                    Fw::Buffer& readBuffer) {
    if (!this->m_opened) {
        return Drv::I2cStatus::I2C_OPEN_ERR;
    }
    FW_ASSERT(writeBuffer.getData() != nullptr);
    FW_ASSERT(readBuffer.getData() != nullptr);
    FW_ASSERT_NO_OVERFLOW(addr, U16);
    FW_ASSERT_NO_OVERFLOW(writeBuffer.getSize(), U16);
    FW_ASSERT_NO_OVERFLOW(readBuffer.getSize(), U16);

    const U16 devAddress = static_cast<U16>(addr);

    const Drv::I2cStatus writeStatus =
        this->hwMasterTransmit(devAddress, writeBuffer.getData(), static_cast<U16>(writeBuffer.getSize()));
    if (Drv::I2cStatus::I2C_OK != writeStatus) {
        return writeStatus;
    }

    return this->hwMasterReceive(devAddress, readBuffer.getData(), static_cast<U16>(readBuffer.getSize()));
}

}  // namespace Stm32