"""! @file imu_manager_integration_tests.py
@brief Dynamic HIL validation for the MPU-6050 IMU (ImuManager), board stationary on the bench.
@details Verifies the sensor is actively sampling (not flatlined/frozen/saturated) and that
resting bias is within a sane physical range. This is a validation check, not a calibration
procedure - it does not tune offsets against a reference instrument (that belongs in a
calibration workflow, not here).

Units (confirmed from ImuManager::convert_raw_data / ImuHelpers.cpp in lib/fprime-sensors):
acceleration in g (1 g = resting gravity magnitude, independent of the configured
ACCELEROMETER_RANGE), rotation in deg/s, temperature in degrees Celsius.

@note Run with: pytest FprimeStm32BaremetalReference/Deployments/ReferenceDeployment/test/int/imu_manager_integration_tests.py \
    --dictionary build-artifacts/stm32h7/ReferenceDeployment/dict/ReferenceDeploymentTopologyDictionary.json
"""

import math

CHANNEL = "MpuImu.imuManager.Reading"
SAMPLE_COUNT = 10  # ImuManager runs at 1 Hz -> ~10s sampling window
SAMPLE_TIMEOUT = 15

# Resting-bias sanity bounds. Not calibration thresholds - wide enough to tolerate
# uncalibrated sensor bias/tilt, tight enough to catch a flatlined/dead/saturated channel.
ACCEL_MAGNITUDE_MIN_G = 0.85
ACCEL_MAGNITUDE_MAX_G = 1.15
GYRO_MAX_DEG_PER_S = 20.0


def _vector3(member):
    return (member["x"], member["y"], member["z"])


def _magnitude(member):
    return math.sqrt(member["x"] ** 2 + member["y"] ** 2 + member["z"] ** 2)


def test_imu_reading_is_dynamic_and_bounded(fprime_test_api):
    """
    @brief Stationary baseline check: raw 6-axis telemetry must vary sample-to-sample
    (anti-stuck) and stay within nominal at-rest bounds (anti-saturation).
    Covers: JPL P-2.2.6 (test as you fly - real physical gravity assertion),
    DR-4.11.5.4 (stress/stationary-stability testing).
    """
    samples = fprime_test_api.await_telemetry_count(
        SAMPLE_COUNT, channels=CHANNEL, timeout=SAMPLE_TIMEOUT
    )
    assert len(samples) >= SAMPLE_COUNT, (
        f"Expected {SAMPLE_COUNT} IMU readings within {SAMPLE_TIMEOUT}s, got {len(samples)}"
    )

    readings = [sample.get_val() for sample in samples]

    # Anti-stuck: telemetry must not flatline/freeze at a single value.
    distinct_accel = {_vector3(r["acceleration"]) for r in readings}
    assert len(distinct_accel) > 1, (
        "Acceleration telemetry did not change across samples - sensor appears frozen"
    )

    # Resting bias: gravity magnitude should be close to 1 g regardless of mounting orientation.
    magnitudes = [_magnitude(r["acceleration"]) for r in readings]
    mean_magnitude = sum(magnitudes) / len(magnitudes)
    assert ACCEL_MAGNITUDE_MIN_G <= mean_magnitude <= ACCEL_MAGNITUDE_MAX_G, (
        f"Mean acceleration magnitude {mean_magnitude:.3f} g outside nominal "
        f"[{ACCEL_MAGNITUDE_MIN_G}, {ACCEL_MAGNITUDE_MAX_G}] g at-rest band"
    )

    # Gyro should not be wildly thrashing while stationary (sanity bound, not calibration).
    for r in readings:
        gx, gy, gz = _vector3(r["rotation"])
        assert max(abs(gx), abs(gy), abs(gz)) < GYRO_MAX_DEG_PER_S, (
            f"Gyroscope reading {r['rotation']} exceeds {GYRO_MAX_DEG_PER_S} deg/s while stationary"
        )


def test_imu_reset_command(fprime_test_api):
    """
    @brief RESET command is accepted and the IMU resumes producing telemetry afterward.
    Covers: DR-4.11.4.6 (response to I/O anomalies - recovery path).
    """
    fprime_test_api.send_and_assert_command("MpuImu.imuManager.RESET", timeout=5)
    samples = fprime_test_api.await_telemetry_count(1, channels=CHANNEL, timeout=SAMPLE_TIMEOUT)
    assert len(samples) >= 1, "No IMU telemetry received after RESET"