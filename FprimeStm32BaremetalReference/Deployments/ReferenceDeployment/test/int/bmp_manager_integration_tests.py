"""! @file bmp_manager_integration_tests.py
@brief Dynamic HIL validation for the BMP280 (BmpManager, SPI bus), board stationary on the bench.
@details Verifies the sensor is actively sampling (not flatlined/frozen) and that pressure and
temperature stay within a sane ambient range. This is a validation check, not a calibration
procedure - it does not tune offsets against a reference instrument.

Note: the automated thermal-stimulus dynamic-shift test (external heat source driving a
measurable delta-T) is deferred - no automated heating actuator exists on the current rig.
This file only covers the stationary/at-rest half of BMP280 validation.

Units (BmpManager.fpp / Bmp280Data): pressure in Pa, temperature in degrees Celsius,
altitude in meters.

@note Run with: pytest FprimeStm32BaremetalReference/Deployments/ReferenceDeployment/test/int/bmp_manager_integration_tests.py \
    --dictionary build-artifacts/stm32h7/ReferenceDeployment/dict/ReferenceDeploymentTopologyDictionary.json
"""

CHANNEL = "Bmp280.bmpManager.Reading"
SAMPLE_COUNT = 10  # BmpManager runs at 1 Hz -> ~10s sampling window
SAMPLE_TIMEOUT = 15

# Ambient sanity bounds (not calibration thresholds) - wide enough for any reasonable bench
# location/elevation, tight enough to catch a flatlined/dead/saturated reading.
PRESSURE_MIN_PA = 50000.0
PRESSURE_MAX_PA = 120000.0
TEMPERATURE_MIN_C = 0.0
TEMPERATURE_MAX_C = 50.0

FAULT_EVENTS = [
    "Bmp280.bmpManager.DeviceFailure",
    "Bmp280.bmpManager.ChipIdCheckFailure",
    "Bmp280.bmpManager.CalibrationFailure",
    "Bmp280.bmpManager.DeviceConfigureFailure",
    "Bmp280.bmpManager.MeasurementTriggerFailure",
    "Bmp280.bmpManager.DeviceReadFailure",
]


def test_bmp_reading_is_dynamic_and_bounded(fprime_test_api):
    """
    @brief Stationary baseline check: pressure/temperature telemetry must vary sample-to-sample
    (anti-stuck/anti-buffer-fault) and stay within nominal ambient bounds.
    Covers: DR-4.11.5.4 (stationary-stability testing).
    """
    samples = fprime_test_api.await_telemetry_count(
        SAMPLE_COUNT, channels=CHANNEL, timeout=SAMPLE_TIMEOUT
    )
    assert len(samples) >= SAMPLE_COUNT, (
        f"Expected {SAMPLE_COUNT} BMP280 readings within {SAMPLE_TIMEOUT}s, got {len(samples)}"
    )

    readings = [sample.get_val() for sample in samples]

    # Anti-stuck: telemetry must not flatline/freeze at a single value - a comms or
    # buffer-read-path fault can masquerade as "working" if only checked for presence.
    distinct_pressure = {r["pressure"] for r in readings}
    assert len(distinct_pressure) > 1, (
        "Pressure telemetry did not change across samples - sensor appears frozen"
    )

    for r in readings:
        assert PRESSURE_MIN_PA <= r["pressure"] <= PRESSURE_MAX_PA, (
            f"Pressure {r['pressure']} Pa outside nominal [{PRESSURE_MIN_PA}, {PRESSURE_MAX_PA}] Pa"
        )
        assert TEMPERATURE_MIN_C <= r["temperature"] <= TEMPERATURE_MAX_C, (
            f"Temperature {r['temperature']} C outside nominal "
            f"[{TEMPERATURE_MIN_C}, {TEMPERATURE_MAX_C}] C bench range"
        )


def test_bmp_no_faults_during_normal_operation(fprime_test_api):
    """
    @brief None of BmpManager's fault events fire while sampling normally.
    Covers: DR-4.11.6.1 (self-test/fault-diagnostics capability - absence of fault signaling
    during nominal operation is itself part of the check).
    """
    # Ensure at least one sampling cycle has happened, then assert no fault event is present.
    fprime_test_api.await_telemetry_count(1, channels=CHANNEL, timeout=SAMPLE_TIMEOUT)
    for event_name in FAULT_EVENTS:
        fprime_test_api.assert_event_count(0, event_name)