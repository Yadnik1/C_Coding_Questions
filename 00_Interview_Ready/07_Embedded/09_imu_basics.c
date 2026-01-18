/*
 * ============================================================================
 *                    IMU BASICS FOR EMBEDDED ENGINEERS
 * ============================================================================
 *
 * CRITICAL FOR IRONSITE: Wearable camera needs IMU for:
 *   - Fall detection (worker safety)
 *   - Orientation tracking (camera pointing direction)
 *   - Motion detection (start/stop recording)
 *   - Stabilization hints for video
 *
 * ============================================================================
 *                    WHAT IS AN IMU?
 * ============================================================================
 *
 * IMU = Inertial Measurement Unit
 *
 * Measures motion and orientation using multiple sensors:
 *
 * 3-AXIS ACCELEROMETER:
 *   - Measures linear acceleration (including gravity)
 *   - Output: X, Y, Z acceleration in g or m/s²
 *   - Used for: Tilt detection, fall detection, step counting
 *   - At rest: Reads ~1g pointing down (gravity)
 *
 * 3-AXIS GYROSCOPE:
 *   - Measures angular velocity (rotation rate)
 *   - Output: X, Y, Z rotation in degrees/sec or rad/sec
 *   - Used for: Rotation tracking, stabilization
 *   - At rest: Reads ~0 (but has drift!)
 *
 * 3-AXIS MAGNETOMETER:
 *   - Measures magnetic field (compass)
 *   - Output: X, Y, Z magnetic field in Gauss or Tesla
 *   - Used for: Heading/compass direction
 *   - Easily disturbed by nearby metal/electronics
 *
 * ============================================================================
 *                    6-AXIS vs 9-AXIS IMU
 * ============================================================================
 *
 * 6-AXIS IMU (Most common):
 *   - 3-axis accelerometer + 3-axis gyroscope
 *   - Examples: MPU6050, LSM6DS3, ICM-20602
 *   - Good for: Motion detection, fall detection, orientation
 *   - Limitation: No absolute heading reference
 *
 * 9-AXIS IMU:
 *   - 3-axis accelerometer + 3-axis gyroscope + 3-axis magnetometer
 *   - Examples: MPU9250, LSM9DS1, BNO055
 *   - Good for: Full orientation with heading
 *   - Challenge: Magnetometer needs calibration, affected by environment
 *
 * WHICH TO USE?
 *   - Fall detection: 6-axis is sufficient
 *   - Compass heading needed: 9-axis required
 *   - Ironsite camera: Probably 6-axis (fall + motion, no compass needed)
 *
 * ============================================================================
 *                    IMU DATA AND AXES
 * ============================================================================
 *
 *           +Z (up)
 *            |
 *            |
 *            +------ +Y (right)
 *           /
 *          /
 *        +X (forward)
 *
 * RIGHT-HAND RULE for rotation:
 *   - Thumb points along axis
 *   - Fingers curl in positive rotation direction
 *
 * ACCELEROMETER AT REST:
 *   - Device flat: Z ≈ +1g, X ≈ 0, Y ≈ 0
 *   - Device on side: Y ≈ ±1g
 *   - Device upright: X ≈ ±1g
 *
 * GYROSCOPE AT REST:
 *   - All axes ≈ 0 deg/s (ideally)
 *   - In practice: small bias/offset always present
 *
 * ============================================================================
 *                    COMMON IMU NOISE SOURCES
 * ============================================================================
 *
 * 1. WHITE NOISE (Random noise):
 *    - High-frequency random variations
 *    - Present in all sensors
 *    - Reduced by: Averaging, low-pass filtering
 *
 * 2. BIAS (Offset):
 *    - Constant offset from true zero
 *    - Example: Gyro reads 0.5 deg/s when stationary
 *    - Reduced by: Calibration at startup
 *
 * 3. BIAS DRIFT (Bias Instability):
 *    - Bias changes slowly over time
 *    - Biggest problem for gyroscope integration
 *    - Causes: Temperature changes, aging
 *    - Reduced by: Sensor fusion, periodic recalibration
 *
 * 4. SCALE FACTOR ERROR:
 *    - Sensitivity not exactly as specified
 *    - Example: Should be 1g but reads 1.02g
 *    - Reduced by: Factory calibration, compensation
 *
 * 5. CROSS-AXIS SENSITIVITY:
 *    - Motion in one axis affects another axis reading
 *    - Reduced by: Calibration matrix
 *
 * 6. TEMPERATURE DRIFT:
 *    - Sensor characteristics change with temperature
 *    - Reduced by: Temperature compensation
 *
 * ============================================================================
 *                    ALLAN VARIANCE
 * ============================================================================
 *
 * WHAT IS IT?
 *   - Statistical method to characterize sensor noise
 *   - Identifies different noise types and their magnitudes
 *   - Standard way to specify IMU quality
 *
 * HOW IT WORKS:
 *   1. Collect long stationary data (hours)
 *   2. Compute variance for different averaging times (τ)
 *   3. Plot log(Allan deviation) vs log(τ)
 *   4. Slope of plot reveals noise type
 *
 * ALLAN VARIANCE PLOT:
 *
 *   log(σ)
 *      |
 *      |\
 *      | \  White noise (slope = -1/2)
 *      |  \
 *      |   \____
 *      |        \___  Bias instability (slope = 0, minimum point)
 *      |            \___
 *      |                \  Random walk (slope = +1/2)
 *      +-----------------------> log(τ)
 *
 * KEY PARAMETERS FROM ALLAN VARIANCE:
 *
 * 1. Angle Random Walk (ARW) - for gyroscope:
 *    - White noise contribution
 *    - Units: deg/√hr or deg/√s
 *    - Lower is better
 *    - Read from slope -1/2 region at τ=1
 *
 * 2. Bias Instability:
 *    - Minimum point on the curve
 *    - Units: deg/hr (gyro) or mg (accel)
 *    - Lower is better
 *    - Key spec for long-term accuracy
 *
 * 3. Rate Random Walk:
 *    - Long-term drift
 *    - Slope +1/2 region
 *    - Important for long integrations
 *
 * TYPICAL VALUES:
 *   Consumer MEMS gyro:  ARW ~0.3 deg/√hr, Bias ~10 deg/hr
 *   Tactical grade:      ARW ~0.01 deg/√hr, Bias ~0.1 deg/hr
 *   Navigation grade:    ARW ~0.001 deg/√hr, Bias ~0.01 deg/hr
 *
 * ============================================================================
 *                    NOISE REDUCTION TECHNIQUES
 * ============================================================================
 *
 * 1. LOW-PASS FILTER (Simple, effective):
 *    - Removes high-frequency noise
 *    - Trade-off: Adds latency, smooths real motion too
 *    - Good for: Tilt sensing, slow motion
 *
 * 2. MOVING AVERAGE:
 *    - Average last N samples
 *    - Simple to implement
 *    - Introduces delay of N/2 samples
 *
 * 3. EXPONENTIAL MOVING AVERAGE (EMA):
 *    - filtered = α * new + (1-α) * filtered
 *    - α closer to 0 = more smoothing
 *    - Less memory than moving average
 *
 * 4. MEDIAN FILTER:
 *    - Take median of last N samples
 *    - Good for removing spikes/outliers
 *    - More expensive than average
 *
 * 5. KALMAN FILTER:
 *    - Optimal estimation combining model + measurements
 *    - Handles noise statistically
 *    - More complex but best results
 *
 * 6. COMPLEMENTARY FILTER:
 *    - Combines accelerometer (low-freq) + gyro (high-freq)
 *    - Simple sensor fusion
 *    - Good for orientation estimation
 *
 * ============================================================================
 *                    SENSOR FUSION
 * ============================================================================
 *
 * WHY FUSE SENSORS?
 *   - Accelerometer: Accurate long-term, noisy short-term
 *   - Gyroscope: Accurate short-term, drifts long-term
 *   - Magnetometer: Heading reference, easily disturbed
 *   - Combining them gives best of all!
 *
 * COMPLEMENTARY FILTER (Simple fusion):
 *
 *   angle = α * (angle + gyro_rate * dt) + (1-α) * accel_angle
 *
 *   - gyro_rate * dt: Integrate gyro (accurate short-term)
 *   - accel_angle: Angle from accelerometer (accurate long-term)
 *   - α: Typically 0.96-0.98 (trust gyro 96-98%)
 *
 * KALMAN FILTER (Optimal fusion):
 *   - Predict state using gyro
 *   - Correct using accelerometer/magnetometer
 *   - Automatically adjusts trust based on noise
 *   - More complex but handles varying conditions
 *
 * MADGWICK/MAHONY FILTERS:
 *   - Popular algorithms for IMU fusion
 *   - Computationally efficient
 *   - Output: Quaternion orientation
 *   - Used in many embedded systems
 *
 * ============================================================================
 *                    FALL DETECTION (IRONSITE USE CASE)
 * ============================================================================
 *
 * FALL SIGNATURE:
 *   1. FREE FALL: Total acceleration drops to ~0g
 *   2. IMPACT: Sharp spike (2-10g depending on surface)
 *   3. MOTIONLESS: Low variance after impact
 *
 * DETECTION ALGORITHM:
 *
 *   magnitude = sqrt(ax² + ay² + az²)
 *
 *   if (magnitude < 0.4g) {                    // Free fall
 *       start_fall_timer();
 *   }
 *
 *   if (fall_timer_active && magnitude > 2.0g) { // Impact
 *       potential_fall = true;
 *   }
 *
 *   if (potential_fall && motion_variance < threshold) { // Motionless
 *       FALL_DETECTED();
 *   }
 *
 * FALSE POSITIVE REDUCTION:
 *   - Require all three phases (free fall + impact + motionless)
 *   - Check orientation change (person went horizontal)
 *   - Time windows for each phase
 *   - User confirmation timeout before alerting
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// ============================================================================
//                    IMU DATA STRUCTURES
// ============================================================================

typedef struct {
    float x;
    float y;
    float z;
} Vector3f;

typedef struct {
    Vector3f accel;     // Acceleration in g
    Vector3f gyro;      // Angular rate in deg/s
    Vector3f mag;       // Magnetic field in Gauss (if 9-axis)
    uint32_t timestamp; // Milliseconds
} IMUData;

typedef struct {
    Vector3f accel_bias;    // Accelerometer offset
    Vector3f gyro_bias;     // Gyroscope offset
    Vector3f accel_scale;   // Accelerometer scale factor
    Vector3f gyro_scale;    // Gyroscope scale factor
} IMUCalibration;

// ============================================================================
//                    NOISE FILTERS
// ============================================================================

// Simple Low-Pass Filter (Exponential Moving Average)
typedef struct {
    float alpha;        // Filter coefficient (0-1, lower = more smoothing)
    Vector3f output;    // Filtered output
    bool initialized;
} LowPassFilter;

void lpf_init(LowPassFilter* lpf, float cutoff_hz, float sample_hz) {
    // Calculate alpha from cutoff frequency
    float rc = 1.0f / (2.0f * 3.14159f * cutoff_hz);
    float dt = 1.0f / sample_hz;
    lpf->alpha = dt / (rc + dt);
    lpf->initialized = false;
    printf("[LPF] Alpha = %.3f (cutoff=%.1f Hz, sample=%.1f Hz)\n",
           lpf->alpha, cutoff_hz, sample_hz);
}

Vector3f lpf_update(LowPassFilter* lpf, Vector3f input) {
    if (!lpf->initialized) {
        lpf->output = input;
        lpf->initialized = true;
    } else {
        lpf->output.x = lpf->alpha * input.x + (1 - lpf->alpha) * lpf->output.x;
        lpf->output.y = lpf->alpha * input.y + (1 - lpf->alpha) * lpf->output.y;
        lpf->output.z = lpf->alpha * input.z + (1 - lpf->alpha) * lpf->output.z;
    }
    return lpf->output;
}

// Moving Average Filter
#define MA_WINDOW_SIZE 8

typedef struct {
    Vector3f buffer[MA_WINDOW_SIZE];
    int index;
    int count;
    Vector3f sum;
} MovingAverageFilter;

void ma_init(MovingAverageFilter* ma) {
    memset(ma, 0, sizeof(*ma));
}

Vector3f ma_update(MovingAverageFilter* ma, Vector3f input) {
    // Subtract oldest value from sum
    if (ma->count == MA_WINDOW_SIZE) {
        ma->sum.x -= ma->buffer[ma->index].x;
        ma->sum.y -= ma->buffer[ma->index].y;
        ma->sum.z -= ma->buffer[ma->index].z;
    }

    // Add new value
    ma->buffer[ma->index] = input;
    ma->sum.x += input.x;
    ma->sum.y += input.y;
    ma->sum.z += input.z;

    // Update index and count
    ma->index = (ma->index + 1) % MA_WINDOW_SIZE;
    if (ma->count < MA_WINDOW_SIZE) ma->count++;

    // Return average
    Vector3f avg = {
        .x = ma->sum.x / ma->count,
        .y = ma->sum.y / ma->count,
        .z = ma->sum.z / ma->count
    };
    return avg;
}

// ============================================================================
//                    COMPLEMENTARY FILTER (Simple Sensor Fusion)
// ============================================================================

typedef struct {
    float alpha;        // Gyro trust factor (0.96-0.98 typical)
    float pitch;        // Rotation around Y axis (degrees)
    float roll;         // Rotation around X axis (degrees)
    bool initialized;
} ComplementaryFilter;

void comp_filter_init(ComplementaryFilter* cf, float alpha) {
    cf->alpha = alpha;
    cf->pitch = 0;
    cf->roll = 0;
    cf->initialized = false;
}

void comp_filter_update(ComplementaryFilter* cf, Vector3f accel, Vector3f gyro, float dt) {
    // Calculate angles from accelerometer (long-term reference)
    float accel_pitch = atan2f(-accel.x, sqrtf(accel.y*accel.y + accel.z*accel.z)) * 180.0f / 3.14159f;
    float accel_roll = atan2f(accel.y, accel.z) * 180.0f / 3.14159f;

    if (!cf->initialized) {
        cf->pitch = accel_pitch;
        cf->roll = accel_roll;
        cf->initialized = true;
        return;
    }

    // Complementary filter: combine gyro integration with accel reference
    // Gyro: good for short-term, drifts long-term
    // Accel: good for long-term, noisy short-term
    cf->pitch = cf->alpha * (cf->pitch + gyro.y * dt) + (1 - cf->alpha) * accel_pitch;
    cf->roll = cf->alpha * (cf->roll + gyro.x * dt) + (1 - cf->alpha) * accel_roll;
}

// ============================================================================
//                    GYROSCOPE BIAS CALIBRATION
// ============================================================================

#define CALIBRATION_SAMPLES 100

typedef struct {
    Vector3f sum;
    int count;
    bool complete;
    Vector3f bias;
} GyroCalibrator;

void gyro_cal_init(GyroCalibrator* cal) {
    memset(cal, 0, sizeof(*cal));
}

bool gyro_cal_add_sample(GyroCalibrator* cal, Vector3f gyro) {
    if (cal->complete) return true;

    cal->sum.x += gyro.x;
    cal->sum.y += gyro.y;
    cal->sum.z += gyro.z;
    cal->count++;

    if (cal->count >= CALIBRATION_SAMPLES) {
        cal->bias.x = cal->sum.x / cal->count;
        cal->bias.y = cal->sum.y / cal->count;
        cal->bias.z = cal->sum.z / cal->count;
        cal->complete = true;
        printf("[GYRO CAL] Bias: X=%.3f, Y=%.3f, Z=%.3f deg/s\n",
               cal->bias.x, cal->bias.y, cal->bias.z);
    }

    return cal->complete;
}

Vector3f gyro_cal_apply(GyroCalibrator* cal, Vector3f gyro) {
    Vector3f corrected = {
        .x = gyro.x - cal->bias.x,
        .y = gyro.y - cal->bias.y,
        .z = gyro.z - cal->bias.z
    };
    return corrected;
}

// ============================================================================
//                    FALL DETECTION
// ============================================================================

typedef enum {
    FALL_STATE_NORMAL,
    FALL_STATE_FREEFALL,
    FALL_STATE_IMPACT,
    FALL_STATE_MOTIONLESS,
    FALL_STATE_DETECTED
} FallState;

typedef struct {
    FallState state;
    uint32_t state_start_time;

    // Thresholds
    float freefall_threshold;   // Below this = free fall (g)
    float impact_threshold;     // Above this = impact (g)
    float motionless_threshold; // Variance below this = motionless

    // Timing (ms)
    uint32_t freefall_min_duration;
    uint32_t impact_window;
    uint32_t motionless_duration;

    // History for variance calculation
    float magnitude_history[16];
    int history_index;
} FallDetector;

void fall_detector_init(FallDetector* fd) {
    fd->state = FALL_STATE_NORMAL;
    fd->state_start_time = 0;

    // Default thresholds (tune for your application)
    fd->freefall_threshold = 0.4f;      // 0.4g
    fd->impact_threshold = 2.5f;         // 2.5g
    fd->motionless_threshold = 0.1f;     // Low variance

    fd->freefall_min_duration = 100;     // 100ms free fall
    fd->impact_window = 500;              // Impact within 500ms of free fall
    fd->motionless_duration = 2000;       // 2 seconds motionless

    fd->history_index = 0;
    memset(fd->magnitude_history, 0, sizeof(fd->magnitude_history));
}

float calculate_variance(float* data, int count) {
    float mean = 0;
    for (int i = 0; i < count; i++) mean += data[i];
    mean /= count;

    float variance = 0;
    for (int i = 0; i < count; i++) {
        float diff = data[i] - mean;
        variance += diff * diff;
    }
    return variance / count;
}

bool fall_detector_update(FallDetector* fd, Vector3f accel, uint32_t timestamp_ms) {
    // Calculate acceleration magnitude
    float magnitude = sqrtf(accel.x*accel.x + accel.y*accel.y + accel.z*accel.z);

    // Store in history
    fd->magnitude_history[fd->history_index] = magnitude;
    fd->history_index = (fd->history_index + 1) % 16;

    uint32_t elapsed = timestamp_ms - fd->state_start_time;

    switch (fd->state) {
        case FALL_STATE_NORMAL:
            if (magnitude < fd->freefall_threshold) {
                fd->state = FALL_STATE_FREEFALL;
                fd->state_start_time = timestamp_ms;
                printf("[FALL] Free fall detected (%.2fg)\n", magnitude);
            }
            break;

        case FALL_STATE_FREEFALL:
            if (magnitude > fd->impact_threshold) {
                if (elapsed >= fd->freefall_min_duration) {
                    fd->state = FALL_STATE_IMPACT;
                    fd->state_start_time = timestamp_ms;
                    printf("[FALL] Impact detected (%.2fg)\n", magnitude);
                } else {
                    // Free fall too short, probably false alarm
                    fd->state = FALL_STATE_NORMAL;
                }
            } else if (elapsed > fd->impact_window) {
                // No impact within window, reset
                fd->state = FALL_STATE_NORMAL;
            }
            break;

        case FALL_STATE_IMPACT:
            // Check for motionless period
            {
                float variance = calculate_variance(fd->magnitude_history, 16);
                if (variance < fd->motionless_threshold) {
                    fd->state = FALL_STATE_MOTIONLESS;
                    fd->state_start_time = timestamp_ms;
                    printf("[FALL] Motionless detected (var=%.4f)\n", variance);
                } else if (elapsed > 1000) {
                    // Not becoming motionless, probably not a fall
                    fd->state = FALL_STATE_NORMAL;
                }
            }
            break;

        case FALL_STATE_MOTIONLESS:
            {
                float variance = calculate_variance(fd->magnitude_history, 16);
                if (variance < fd->motionless_threshold) {
                    if (elapsed >= fd->motionless_duration) {
                        fd->state = FALL_STATE_DETECTED;
                        printf("[FALL] *** FALL DETECTED! ***\n");
                        return true;
                    }
                } else {
                    // Started moving again, probably not a fall
                    fd->state = FALL_STATE_NORMAL;
                }
            }
            break;

        case FALL_STATE_DETECTED:
            // Stay in detected state until reset
            break;
    }

    return false;
}

void fall_detector_reset(FallDetector* fd) {
    fd->state = FALL_STATE_NORMAL;
}

// ============================================================================
//                    DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   IMU BASICS DEMO\n");
    printf("============================================\n\n");

    // Demo 1: Low-pass filter
    printf("--- Low-Pass Filter Demo ---\n");
    LowPassFilter lpf;
    lpf_init(&lpf, 5.0f, 100.0f);  // 5Hz cutoff, 100Hz sample rate

    Vector3f noisy_data[] = {
        {1.0, 0.0, 0.0},
        {1.2, 0.1, -0.1},
        {0.9, -0.1, 0.05},
        {1.1, 0.05, -0.05},
        {1.0, 0.0, 0.0}
    };

    printf("Input -> Filtered:\n");
    for (int i = 0; i < 5; i++) {
        Vector3f filtered = lpf_update(&lpf, noisy_data[i]);
        printf("  (%.2f, %.2f, %.2f) -> (%.2f, %.2f, %.2f)\n",
               noisy_data[i].x, noisy_data[i].y, noisy_data[i].z,
               filtered.x, filtered.y, filtered.z);
    }

    // Demo 2: Gyro calibration
    printf("\n--- Gyro Calibration Demo ---\n");
    GyroCalibrator gyro_cal;
    gyro_cal_init(&gyro_cal);

    // Simulate stationary gyro readings with bias
    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        Vector3f gyro = {0.5f, -0.3f, 0.1f};  // Simulated bias
        gyro_cal_add_sample(&gyro_cal, gyro);
    }

    Vector3f raw_gyro = {0.5f, -0.3f, 0.1f};
    Vector3f corrected = gyro_cal_apply(&gyro_cal, raw_gyro);
    printf("Raw: (%.2f, %.2f, %.2f) -> Corrected: (%.2f, %.2f, %.2f)\n",
           raw_gyro.x, raw_gyro.y, raw_gyro.z,
           corrected.x, corrected.y, corrected.z);

    // Demo 3: Complementary filter
    printf("\n--- Complementary Filter Demo ---\n");
    ComplementaryFilter cf;
    comp_filter_init(&cf, 0.98f);

    // Simulate tilted device
    Vector3f accel = {0.0f, 0.0f, 1.0f};  // Flat
    Vector3f gyro = {0.0f, 0.0f, 0.0f};
    comp_filter_update(&cf, accel, gyro, 0.01f);
    printf("Flat: Pitch=%.1f, Roll=%.1f\n", cf.pitch, cf.roll);

    accel = (Vector3f){0.5f, 0.0f, 0.866f};  // Tilted 30 degrees
    comp_filter_update(&cf, accel, gyro, 0.01f);
    printf("Tilted: Pitch=%.1f, Roll=%.1f\n", cf.pitch, cf.roll);

    // Demo 4: Fall detection state machine
    printf("\n--- Fall Detection Demo ---\n");
    FallDetector fd;
    fall_detector_init(&fd);

    // Simulate fall sequence
    typedef struct { Vector3f accel; uint32_t time; } FallSample;
    FallSample fall_sequence[] = {
        {{0.0f, 0.0f, 1.0f}, 0},       // Normal standing
        {{0.1f, 0.1f, 0.1f}, 100},     // Free fall starts
        {{0.05f, 0.05f, 0.05f}, 200},  // Still falling
        {{3.5f, 2.0f, 1.0f}, 300},     // Impact!
        {{0.0f, 0.0f, 1.0f}, 500},     // Lying down
        {{0.01f, 0.01f, 1.0f}, 1000},  // Still motionless
        {{0.0f, 0.02f, 1.0f}, 2000},   // Still motionless
        {{0.01f, 0.0f, 1.0f}, 3000},   // Fall confirmed
    };

    for (int i = 0; i < 8; i++) {
        fall_detector_update(&fd, fall_sequence[i].accel, fall_sequence[i].time);
    }

    printf("\n============================================\n");
    printf("   KEY POINTS FOR INTERVIEW\n");
    printf("============================================\n");
    printf("- 6-axis = accel + gyro, 9-axis = adds magnetometer\n");
    printf("- Gyro drifts over time, accel is noisy but no drift\n");
    printf("- Complementary filter: simple fusion of both\n");
    printf("- Allan variance: characterizes sensor noise types\n");
    printf("- Fall detection: free fall -> impact -> motionless\n");
    printf("- Always calibrate gyro bias at startup!\n");

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW Q&A
 * ============================================================================
 *
 * Q: What is a 6-axis vs 9-axis IMU?
 * A: "6-axis has accelerometer and gyroscope - 3 axes each. 9-axis adds a
 *    magnetometer for compass heading. For most motion detection like fall
 *    detection, 6-axis is sufficient. You need 9-axis if you need absolute
 *    heading reference."
 *
 * Q: Why does a gyroscope drift?
 * A: "Gyroscopes measure angular rate, and you integrate to get angle. Any
 *    small bias error accumulates over time - that's drift. Even a 0.1 deg/s
 *    bias becomes 6 degrees after a minute. That's why we use sensor fusion
 *    to correct with accelerometer which doesn't drift."
 *
 * Q: What is Allan variance?
 * A: "It's a method to characterize different types of sensor noise. You
 *    collect stationary data and compute variance at different averaging
 *    times. The plot reveals white noise, bias instability, and random walk.
 *    The minimum point gives you bias instability - a key spec for IMU quality."
 *
 * Q: How does a complementary filter work?
 * A: "It combines gyro and accelerometer by trusting each for what it's good
 *    at. Gyro is accurate short-term, so we integrate it. Accelerometer is
 *    accurate long-term, so we use it as reference. The formula is:
 *    angle = 0.98 * (angle + gyro*dt) + 0.02 * accel_angle.
 *    The 0.98 means we trust gyro 98% for fast changes."
 *
 * Q: How would you detect a fall?
 * A: "Falls have a signature: first free fall where total acceleration drops
 *    near zero, then a sharp impact spike, then the person stays motionless.
 *    I'd implement a state machine checking for all three phases with time
 *    windows. This reduces false positives compared to just checking impact."
 *
 * Q: How do you calibrate an IMU?
 * A: "For gyro bias: keep device stationary, average readings over a few
 *    seconds, that's your bias offset to subtract. For accelerometer: ideally
 *    do a 6-position calibration - each axis pointing up and down - to get
 *    scale factors and offsets. Magnetometer needs hard/soft iron calibration
 *    by rotating the device in all directions."
 *
 * Q: What causes noise in MEMS sensors?
 * A: "Several sources: thermal noise in the electronics, Brownian motion of
 *    the proof mass, bias drift from temperature changes, and vibration
 *    coupling. You reduce it through filtering, averaging, and sensor fusion.
 *    Allan variance helps identify which type dominates."
 *
 * ============================================================================
 */
