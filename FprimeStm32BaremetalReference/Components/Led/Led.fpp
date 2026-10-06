module LedBlinker {
    @ Component to blink an LED driven by a rate group
    active component Led {

        # One async command/port is required for active components
        # This should be overridden by the developers with a useful command/port

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        @ Command to turn on or off the blinking LED
        async command BLINKING_ON_OFF(
            onOff: Fw.On @< Indicates wheter the blinking should be on or off
        )

        @ Telemetry channel to report blinking state.
        telemetry BlinkingState: Fw.On

        @ Telemetry channel to report the number of on/off transitions.
        telemetry LedTransitionCount: U64

        @ Reports the state we set to blinking.
        event SetBlinkingState($state: Fw.On) \
            severity activity high \
            format "Set blinking state to {}."

        @ Reports the blinking interval.
        event BlinkingIntervalSet(interval: U32) \
            severity activity high \
            format "LED blink interval set to {}."

        @ Report the current LED state.
        event LedState(onOff: Fw.On) \
            severity activity low \
            format "LED is {}."

        @ Port receiving calls from the rate group
        async input port run: Svc.Sched

        @ Port sending calls to the GPIO driver
        output port gpioSet: Drv.GpioWrite

        @ Blinking interval in rate group ticks
        param BLINK_INTERVAL: U32 default 1

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}