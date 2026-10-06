from pathlib import Path

from fprime_stm32.cubemx_cmake import parse_cubemx_cmake

FIXTURE = Path(__file__).parent / "fixtures" / "cubemx_stm32h753" / "cmake" / "stm32cubemx" / "CMakeLists.txt"


def _parse():
    return parse_cubemx_cmake(FIXTURE.read_text())


def test_chip_define():
    info = _parse()
    assert info.chip_define == "STM32H753xx"
    assert "USE_HAL_DRIVER" in info.all_defines
    assert "USE_PWR_LDO_SUPPLY" in info.all_defines


def test_it_source_extracted_separately():
    info = _parse()
    assert info.it_source == "Core/Src/stm32h7xx_it.c"
    assert info.it_source not in info.application_sources


def test_application_sources_exclude_main_and_syscalls():
    info = _parse()
    excluded = {"Core/Src/main.c", "Core/Src/sysmem.c", "Core/Src/syscalls.c"}
    assert not excluded & set(info.application_sources)
    assert not any(path.endswith(".s") for path in info.application_sources)
    for expected in (
        "Core/Src/gpio.c",
        "Core/Src/dma.c",
        "Core/Src/i2c.c",
        "Core/Src/tim.c",
        "Core/Src/usart.c",
        "Core/Src/stm32h7xx_hal_msp.c",
    ):
        assert expected in info.application_sources


def test_driver_sources_match_stm32_drivers_src():
    info = _parse()
    assert "Core/Src/system_stm32h7xx.c" in info.driver_sources
    assert "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c" in info.driver_sources
    assert "Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c" in info.driver_sources


def test_include_dirs_path_stripped():
    info = _parse()
    assert "Core/Inc" in info.include_dirs
    assert "Drivers/CMSIS/Include" in info.include_dirs
    assert all("CMAKE_CURRENT_SOURCE_DIR" not in d for d in info.include_dirs)
