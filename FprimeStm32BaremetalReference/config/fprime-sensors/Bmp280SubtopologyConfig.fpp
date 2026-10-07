module Bmp280 {
    module SubtopologyConfig {
        constant BASE_ID = 0xD0000000
    }

    instance bmpDriver: Stm32.Stm32SpiDriver base id Bmp280.SubtopologyConfig.BASE_ID + 0x00002000 {
        phase Fpp.ToCpp.Phases.configComponents """
        (void) Bmp280::bmpDriver.open(Stm32::SpiInstance::Spi5, Stm32::GpioPort::F, 6, 100);
        """
    }
} 