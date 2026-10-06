"""! @file led_integration_tests.py
@brief Integration tests for the LED component.
@details This file uses the F' test API to send commands to the LED component and assert the expected responses.
@note The script can be executed with `pytest Components/Led/test/int/led_integration_tests.py --dictionary ../build-artifacts/stm32h7/ReferenceDeployment/dict/ReferenceDeploymentTopologyDictionary.json`
"""

import time
from fprime_gds.common.testing_fw import predicates

def test_cmd_no_op(fprime_test_api):
    """
    @brief Test that CMD_NO_OP can be sent and return without any errors
    @param fprime_test_api: The test API instance used to send commands and assert responses
    @return: None
    """
    fprime_test_api.send_and_assert_command("CdhCore.cmdDisp.CMD_NO_OP")

def test_blinking(fprime_test_api):
    """
    @brief Test that the LED component can respond to ground commands for blinking
    @param fprime_test_api: The test API instance used to send commands and assert responses
    @return: None
    """

    # Send command to enable blinking, then assert expected events are emitted
    blink_start_evr = fprime_test_api.get_event_pred("ReferenceDeployment.led.SetBlinkingState", ["ON"])
    led_on_evr = fprime_test_api.get_event_pred("ReferenceDeployment.led.LedState", ["ON"])
    led_off_evr = fprime_test_api.get_event_pred("ReferenceDeployment.led.LedState", ["OFF"])

    fprime_test_api.send_and_assert_event(
        "ReferenceDeployment.led.BLINKING_ON_OFF",
        args=["ON"],
        events=[blink_start_evr, led_on_evr, led_off_evr, led_on_evr],
        timeout=10,
    )

    # Assert that blink command sets blinking state on
    blink_state_on_tlm = fprime_test_api.get_telemetry_pred("ReferenceDeployment.led.BlinkingState", "ON")
    fprime_test_api.assert_telemetry(blink_state_on_tlm)

    # Send command to stop blinking, then assert blinking stops
    # Define blink_stop_evr
    blink_stop_evr = fprime_test_api.get_event_pred("ReferenceDeployment.led.SetBlinkingState", ["OFF"])
    fprime_test_api.send_and_assert_event(
        "ReferenceDeployment.led.BLINKING_ON_OFF", args=["OFF"], events=[blink_stop_evr]
    )

    time.sleep(1)  # Wait one second to let any in-progress telemetry be sent
    
    # Assert that blink command sets blinking state off
    blink_state_off_tlm = fprime_test_api.get_telemetry_pred("ReferenceDeployment.led.BlinkingState", "OFF")
    fprime_test_api.assert_telemetry(blink_state_off_tlm)

    time.sleep(1)  # Wait one second to let any in-progress telemetry be sent
    # Save reference to current telemetry history so we can search against future telemetry
    telem_after_blink_off = fprime_test_api.telemetry_history.size()
    time.sleep(2)  # Wait to receive telemetry after stopping blinking
    # Assert that blinking has stopped and that LedTransitionCount is no longer updating
    fprime_test_api.assert_telemetry_count(
        0, "ReferenceDeployment.led.LedTransitionCount", start=telem_after_blink_off
    )