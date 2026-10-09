"""! @file smoke_integration_tests.py
@brief Deployment-level HIL smoke test: proves GDS-over-UART + the flashed board are alive.
@details Run this first against any newly flashed board, before any component-specific
HIL test. It intentionally depends on no single component so it still catches "board
isn't flashed" / "wrong COM port" / "link is dead" failures distinctly from a sensor
or driver bug.
@note Run with: pytest FprimeStm32BaremetalReference/Deployments/ReferenceDeployment/test/int/smoke_integration_tests.py \
    --dictionary build-artifacts/stm32h7/ReferenceDeployment/dict/ReferenceDeploymentTopologyDictionary.json
"""


def test_cmd_no_op(fprime_test_api):
    """
    @brief CMD_NO_OP round trip: the command link to the board works.
    Covers: DR-4.11.5.2 (incremental verification) - baseline check before any other HIL test runs.
    """
    fprime_test_api.send_and_assert_command("CdhCore.cmdDisp.CMD_NO_OP", timeout=5)


def test_telemetry_is_flowing(fprime_test_api):
    """
    @brief At least one telemetry update arrives within a bounded window.
    Proves the downlink isn't just command-ack-only (e.g. a stuck/dead rate group
    would pass test_cmd_no_op but fail this).
    Covers: DR-4.11.4.7 (use of time-outs) - this check itself must not block forever.
    """
    updates = fprime_test_api.await_telemetry_count(1, channels=None, timeout=15)
    assert len(updates) >= 1, "No telemetry received from the board within 15s"