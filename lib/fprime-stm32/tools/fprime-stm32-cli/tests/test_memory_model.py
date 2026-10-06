from pathlib import Path

from fprime_stm32.memory_model import parse_memory_block

FIXTURE = Path(__file__).parent / "fixtures" / "cubemx_stm32h753" / "STM32H753xx_FLASH.ld"


def test_parses_flash_dtcm_axi_regions():
    memory_map = parse_memory_block(FIXTURE.read_text())

    assert memory_map.flash.origin == 0x08000000
    assert memory_map.flash.length_bytes == 2048 * 1024

    assert memory_map.dtcm.name == "DTCMRAM"
    assert memory_map.dtcm.origin == 0x20000000
    assert memory_map.dtcm.length_bytes == 128 * 1024

    assert memory_map.axi_sram.name == "RAM"
    assert memory_map.axi_sram.origin == 0x24000000
    assert memory_map.axi_sram.length_bytes == 512 * 1024

    assert memory_map.warnings == []
