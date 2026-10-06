// ======================================================================
// \title  LedTester.hpp
// \author ivanlara
// \brief  hpp file for Led component test harness implementation class
// ======================================================================

#ifndef LedBlinker_LedTester_HPP
#define LedBlinker_LedTester_HPP

#include "FprimeStm32BaremetalReference/Components/Led/Led.hpp"
#include "FprimeStm32BaremetalReference/Components/Led/LedGTestBase.hpp"

namespace LedBlinker {

class LedTester final : public LedGTestBase {
  public:
    // ----------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------

    // Maximum size of histories storing events, telemetry, and port outputs
    static const FwSizeType MAX_HISTORY_SIZE = 10;

    // Instance ID supplied to the component instance under test
    static const FwEnumStoreType TEST_INSTANCE_ID = 0;

    // Queue depth supplied to the component instance under test
    static const FwSizeType TEST_INSTANCE_QUEUE_DEPTH = 10;

  public:
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    //! Construct object LedTester
    LedTester();

    //! Destroy object LedTester
    ~LedTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! Test the blinking functionality of the LED component
    void testBlinking();

    //! Test the functionality of turning off the blinking LED
    void testBlinkingOff();

    //! Test the blink interval adjustment functionality of the LED component
    void testBlinkInterval();

    //! Test handling of invalid blink interval parameter
    void testInvalidParameter();

    //! Test the run handler branches of the LED component
    void testRunHandlerBranches();

    //! Test turning off the blinking LED while it is actively ON
    void testBlinkingOffWhileOn();

    //! Test the run handler behavior when the blink interval parameter is invalid
    void testRunHandlerInvalidParam();

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    Led component;
};

}  // namespace LedBlinker

#endif
