// ======================================================================
// \title  LedTester.cpp
// \author ivanlara
// \brief  cpp file for Led component test harness implementation class
// ======================================================================

#include "LedTester.hpp"

namespace LedBlinker {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

LedTester ::LedTester() : LedGTestBase("LedTester", LedTester::MAX_HISTORY_SIZE), component("Led") {
    this->initComponents();
    this->connectPorts();
}

LedTester ::~LedTester() {
    this->component.deinit();
}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void LedTester ::testBlinking() {
    // This test will make use of parameters. So need to load them.
    this->component.loadParameters();

    // Ensure LED stays off when blinking is disabled
    // The Led component defaults to blinking off
    this->invoke_to_run(0, 0);     // invoke the 'run' port to simulate running one cycle
    this->component.doDispatch();  // Trigger execution of async port

    ASSERT_EVENTS_LedState_SIZE(0);  // ensure no LedState change events were emitted

    ASSERT_from_gpioSet_SIZE(0);  // ensure gpio LED wasn't set

    ASSERT_TLM_LedTransitionCount_SIZE(0);  // ensure no LedTransitionCount values were recorded

    // Send command to enable blinking
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();  // Trigger execution of async command
    ASSERT_CMD_RESPONSE_SIZE(1);   // ensure a command response was emitted
    ASSERT_CMD_RESPONSE(0, Led::OPCODE_BLINKING_ON_OFF, 0,
                        Fw::CmdResponse::OK);  // ensure the expected command response was emitted

    // Step through 3 run cycles to observe LED turning on and off 3 times
    // Cycle 1: LED initalization->On
    this->invoke_to_run(0, 0);
    this->component.doDispatch();  // Trigger execution of async port
    ASSERT_EVENTS_LedState_SIZE(1);
    ASSERT_EVENTS_LedState(0, Fw::On::ON);
    ASSERT_from_gpioSet_SIZE(1);
    ASSERT_from_gpioSet(0, Fw::Logic::HIGH);
    ASSERT_TLM_LedTransitionCount_SIZE(1);
    ASSERT_TLM_LedTransitionCount(0, 1);

    // Cycle 2: LED On->Off
    this->invoke_to_run(0, 0);
    this->component.doDispatch();  // Trigger execution of async port
    ASSERT_EVENTS_LedState_SIZE(2);
    ASSERT_EVENTS_LedState(1, Fw::On::OFF);
    ASSERT_from_gpioSet_SIZE(2);
    ASSERT_from_gpioSet(1, Fw::Logic::LOW);
    // Add assertions for LedTransitionCount telemetry
    ASSERT_TLM_LedTransitionCount_SIZE(2);
    ASSERT_TLM_LedTransitionCount(1, 2);

    // Cycle 3: LED Off->On
    this->invoke_to_run(0, 0);
    this->component.doDispatch();  // Trigger execution of async port
    // Write assertions for third cycle
    ASSERT_EVENTS_LedState_SIZE(3);
    ASSERT_EVENTS_LedState(2, Fw::On::ON);
    ASSERT_from_gpioSet_SIZE(3);
    ASSERT_from_gpioSet(2, Fw::Logic::HIGH);
    ASSERT_TLM_LedTransitionCount_SIZE(3);
    ASSERT_TLM_LedTransitionCount(2, 3);
}

void LedTester ::testBlinkingOff() {
    this->component.loadParameters();

    // Enable then disable blinking
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();

    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::OFF);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(2);
    ASSERT_CMD_RESPONSE(1, Led::OPCODE_BLINKING_ON_OFF, 0, Fw::CmdResponse::OK);
    ASSERT_EVENTS_SetBlinkingState_SIZE(2);
    ASSERT_EVENTS_SetBlinkingState(1, Fw::On::OFF);
    ASSERT_TLM_BlinkingState(1, Fw::On::OFF);

    this->clearHistory();

    // Run cycle while disabled: verify no GPIO or event invocations occur
    this->invoke_to_run(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_LedState_SIZE(0);
    ASSERT_from_gpioSet_SIZE(0);
}

void LedTester ::testBlinkInterval() {
    this->component.loadParameters();

    // Enable LED Blinking
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();
    this->clearHistory();

    // Adjust blink interval to 4 cycles
    U32 blinkInterval = 4;
    this->paramSet_BLINK_INTERVAL(blinkInterval, Fw::ParamValid::VALID);
    this->paramSend_BLINK_INTERVAL(0, 0);
    ASSERT_EVENTS_BlinkingIntervalSet_SIZE(1);
    ASSERT_EVENTS_BlinkingIntervalSet(0, blinkInterval);

    // Cycle 1: Transitions to ON
    this->invoke_to_run(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_LedState_SIZE(1);
    ASSERT_EVENTS_LedState(0, Fw::On::ON);
    ASSERT_from_gpioSet_SIZE(1);
    ASSERT_from_gpioSet(0, Fw::Logic::HIGH);

    // Cycles 2, 3, and 4: LED remains ON (no new state events or GPIO writes)
    for (U32 i = 0; i < 3; ++i) {
        this->invoke_to_run(0, 0);
        this->component.doDispatch();
        ASSERT_EVENTS_LedState_SIZE(1); // Size remains 1
        ASSERT_from_gpioSet_SIZE(1);
    }

    // Cycle 5 (4th cycle after ON): Transitions to OFF
    this->invoke_to_run(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_LedState_SIZE(2);
    ASSERT_EVENTS_LedState(1, Fw::On::OFF);
    ASSERT_from_gpioSet_SIZE(2);
    ASSERT_from_gpioSet(1, Fw::Logic::LOW);
}

void LedTester ::testInvalidParameter() {
    this->component.loadParameters();

    // Test setting an invalid blink interval parameter
    this->paramSet_BLINK_INTERVAL(1, Fw::ParamValid::INVALID);
    this->paramSend_BLINK_INTERVAL(0, 0);

    // Test running the LED component with an invalid parameter set
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();

    this->invoke_to_run(0, 0);
    this->component.doDispatch();
}

void LedTester ::testRunHandlerBranches() {
    this->component.loadParameters();

    // Ensure that m_blinking is disabled and run one cycle
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::OFF);
    this->component.doDispatch();

    this->invoke_to_run(0, 0);
    this->component.doDispatch();

    // Enable blinking and test with a 2-cycle interval
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();

    this->paramSet_BLINK_INTERVAL(2, Fw::ParamValid::VALID);
    this->paramSend_BLINK_INTERVAL(0, 0);

    // Cycle 1: Transitions to ON
    this->invoke_to_run(0, 0);
    this->component.doDispatch();

    // Cycle 2: Increments counter but does NOT transition (exercises the branch m_counter &lt; m_interval)
    this->invoke_to_run(0, 0);
    this->component.doDispatch();

    // Cycle 3: Reaches the interval and transitions to OFF
    this->invoke_to_run(0, 0);
    this->component.doDispatch();
}

void LedTester ::testBlinkingOffWhileOn() {
    this->component.loadParameters();

    // Enable blinking
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();

    // Step 1 cycle so the LED transitions to ON
    this->invoke_to_run(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_LedState(0, Fw::On::ON);
    ASSERT_from_gpioSet(0, Fw::Logic::HIGH);

    this->clearHistory();

    // Disable blinking WHILE the LED is actively ON
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::OFF);
    this->component.doDispatch();

    // Step 1 cycle to allow run_handler / command logic to process the OFF transition
    this->invoke_to_run(0, 0);
    this->component.doDispatch();

    // Verify that lines 53-58 executed: GPIO driven LOW and OFF event logged
    ASSERT_EVENTS_LedState_SIZE(1);
    ASSERT_EVENTS_LedState(0, Fw::On::OFF);
    ASSERT_from_gpioSet_SIZE(1);
    ASSERT_from_gpioSet(0, Fw::Logic::LOW);
}

void LedTester ::testRunHandlerInvalidParam() {
    this->component.loadParameters();

    // Enable blinking
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();

    // Set parameter validity to INVALID in the test harness
    this->paramSet_BLINK_INTERVAL(1, Fw::ParamValid::INVALID);

    // Execute run_handler when the parameter is invalid (exercises the fallback interval branch)
    this->invoke_to_run(0, 0);
    this->component.doDispatch();
}

} // namespace LedBlinker

