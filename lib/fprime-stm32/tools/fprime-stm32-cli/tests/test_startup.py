from pathlib import Path

from fprime_stm32.startup import patch_startup_script

FIXTURE = Path(__file__).parent / "fixtures" / "cubemx_stm32h753" / "startup_stm32h753xx.s"


def test_inserts_dtcm_bss_zero_loop_and_allocator_hook():
    text = FIXTURE.read_text()
    patched, actions = patch_startup_script(text)

    assert patched.count("_sdtcm_bss") == 1
    assert patched.count("_edtcm_bss") == 1
    assert patched.count("bl Stm32_registerBootstrapAllocator") == 1
    assert len(actions) == 2

    # inserted before the call to static constructors / main
    dtcm_pos = patched.index("_sdtcm_bss")
    libc_pos = patched.index("__libc_init_array")
    assert dtcm_pos < libc_pos


def test_idempotent_on_already_patched_input():
    text = FIXTURE.read_text()
    once, _ = patch_startup_script(text)
    twice, actions = patch_startup_script(once)

    assert once == twice
    assert actions == ["startup script already patched, skipped"]
