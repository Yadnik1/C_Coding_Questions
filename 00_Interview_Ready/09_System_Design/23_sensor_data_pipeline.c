/*
 * ============================================================================
 *        DESIGN: SENSOR DATA ACQUISITION PIPELINE
 * ============================================================================
 *
 * PRIORITY FOR IRONSITE: ★★★★★ (CRITICAL)
 *   - IMU for motion/fall detection
 *   - Camera frames
 *   - Audio microphone
 *   - Environmental sensors (temp, humidity)
 *   - Battery monitoring
 *
 * ============================================================================
 *                    PIPELINE ARCHITECTURE
 * ============================================================================
 *
 *   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐
 *   │ ACQUIRE │───►│ BUFFER  │───►│ FILTER  │───►│ PROCESS │───►│ OUTPUT  │
 *   │         │    │         │    │         │    │         │    │         │
 *   │ ADC/I2C │    │  Ring   │    │  EMA/   │    │ Detect/ │    │ Storage/│
 *   │ /SPI    │    │ Buffer  │    │  Moving │    │ Classify│    │ Network │
 *   └─────────┘    └─────────┘    └─────────┘    └─────────┘    └─────────┘
 *        │
 *   Interrupt/    Double-buff    Noise           Fall detect    SD card
 *   DMA trigger   for decoupling reduction       PPE detect     MQTT
 *
 * ============================================================================
 *                    SAMPLING THEORY
 * ============================================================================
 *
 * NYQUIST THEOREM:
 *   Sample rate must be > 2× highest frequency of interest
 *
 *   Sensor            | Signal BW  | Min Sample | Typical
 *   ------------------|------------|------------|----------
 *   Temperature       | 0.1 Hz     | 0.2 Hz     | 1 Hz
 *   Accelerometer     | 100 Hz     | 200 Hz     | 400 Hz
 *   Microphone        | 20 kHz     | 40 kHz     | 48 kHz
 *   Video             | 30 fps     | 60 fps     | 30 fps
 *   Battery voltage   | 0.01 Hz    | 0.02 Hz    | 1 Hz
 *
 * ALIASING:
 *   If you sample too slowly, high-freq components fold back
 *   Solution: Low-pass filter BEFORE sampling (anti-aliasing filter)
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

// ============================================================================
// DATA STRUCTURES
// ============================================================================

// Generic sensor sample with timestamp
typedef struct {
    uint32_t timestamp_ms;
    int16_t value;
} SensorSample;

// 3-axis sensor sample (IMU)
typedef struct {
    uint32_t timestamp_ms;
    int16_t x;
    int16_t y;
    int16_t z;
} ImuSample;

// Sensor channel configuration
typedef struct {
    uint8_t channel_id;
    uint16_t sample_rate_hz;
    uint16_t oversample_count;
    bool enabled;
    float scale_factor;
    float offset;
} SensorConfig;

// ============================================================================
// STAGE 1: DATA ACQUISITION
// ============================================================================

/*
 * ACQUISITION METHODS:
 *
 * 1. POLLING (Simple, but wastes CPU)
 *    while(1) {
 *        value = read_adc();
 *        process(value);
 *        delay(interval);
 *    }
 *
 * 2. TIMER-TRIGGERED (Precise timing)
 *    timer_callback() {
 *        start_adc_conversion();
 *    }
 *    adc_complete_callback() {
 *        value = read_adc_result();
 *        buffer_push(value);
 *    }
 *
 * 3. DMA (Best for high-rate continuous)
 *    Configure DMA: ADC -> Buffer
 *    DMA moves data automatically
 *    CPU only handles completed buffers
 *
 * 4. SENSOR FIFO (IMU/Accel common)
 *    Sensor has internal FIFO
 *    Interrupt when FIFO threshold reached
 *    Read burst of samples at once
 */

// Simulated ADC with DMA
#define ADC_BUFFER_SIZE 64

typedef struct {
    int16_t buffer[2][ADC_BUFFER_SIZE];  // Double buffer
    uint8_t active_buffer;               // DMA writing here
    volatile bool buffer_ready[2];       // Ready for processing
    uint32_t sample_count;
} AdcDma;

AdcDma adc = {0};

void adc_dma_init(void) {
    adc.active_buffer = 0;
    adc.buffer_ready[0] = false;
    adc.buffer_ready[1] = false;
    adc.sample_count = 0;
    printf("[ADC] DMA double-buffer initialized\n");
}

// Called by DMA half-complete and complete interrupts
void adc_dma_callback(uint8_t completed_buffer) {
    adc.buffer_ready[completed_buffer] = true;
    adc.active_buffer = 1 - completed_buffer;  // Swap
    adc.sample_count += ADC_BUFFER_SIZE;
}

// Process completed buffer (call from main loop)
int16_t* adc_get_ready_buffer(uint8_t* buffer_idx) {
    for (uint8_t i = 0; i < 2; i++) {
        if (adc.buffer_ready[i]) {
            adc.buffer_ready[i] = false;
            *buffer_idx = i;
            return adc.buffer[i];
        }
    }
    return NULL;
}

// ============================================================================
// STAGE 2: BUFFERING (Ring Buffer with Timestamps)
// ============================================================================

#define SAMPLE_BUFFER_SIZE 128

typedef struct {
    SensorSample buffer[SAMPLE_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} SampleBuffer;

void sample_buffer_init(SampleBuffer* sb) {
    sb->head = 0;
    sb->tail = 0;
}

bool sample_buffer_push(SampleBuffer* sb, int16_t value, uint32_t timestamp) {
    uint16_t next = (sb->head + 1) % SAMPLE_BUFFER_SIZE;
    if (next == sb->tail) return false;  // Full

    sb->buffer[sb->head].value = value;
    sb->buffer[sb->head].timestamp_ms = timestamp;
    sb->head = next;
    return true;
}

bool sample_buffer_pop(SampleBuffer* sb, SensorSample* sample) {
    if (sb->head == sb->tail) return false;  // Empty

    *sample = sb->buffer[sb->tail];
    sb->tail = (sb->tail + 1) % SAMPLE_BUFFER_SIZE;
    return true;
}

// ============================================================================
// STAGE 3: FILTERING
// ============================================================================

/*
 * FILTER TYPES:
 *
 * 1. MOVING AVERAGE
 *    Simple, good for slow-changing signals
 *    y[n] = (x[n] + x[n-1] + ... + x[n-N+1]) / N
 *
 * 2. EXPONENTIAL MOVING AVERAGE (EMA)
 *    Single coefficient, memory efficient
 *    y[n] = α × x[n] + (1-α) × y[n-1]
 *
 * 3. MEDIAN FILTER
 *    Removes outliers/spikes
 *    Sort last N samples, take middle value
 *
 * 4. KALMAN FILTER
 *    Optimal for known noise characteristics
 *    More complex, better performance
 */

// EMA Filter
typedef struct {
    float alpha;
    float value;
    bool initialized;
} EmaFilter;

void ema_init(EmaFilter* f, float alpha) {
    f->alpha = alpha;  // 0.0 to 1.0, lower = more smoothing
    f->value = 0;
    f->initialized = false;
}

float ema_update(EmaFilter* f, float input) {
    if (!f->initialized) {
        f->value = input;
        f->initialized = true;
    } else {
        f->value = f->alpha * input + (1.0f - f->alpha) * f->value;
    }
    return f->value;
}

// Moving Average Filter
#define MA_WINDOW_SIZE 8

typedef struct {
    float buffer[MA_WINDOW_SIZE];
    uint8_t index;
    float sum;
    uint8_t count;
} MovingAvgFilter;

void ma_init(MovingAvgFilter* f) {
    memset(f->buffer, 0, sizeof(f->buffer));
    f->index = 0;
    f->sum = 0;
    f->count = 0;
}

float ma_update(MovingAvgFilter* f, float input) {
    f->sum -= f->buffer[f->index];
    f->buffer[f->index] = input;
    f->sum += input;
    f->index = (f->index + 1) % MA_WINDOW_SIZE;

    if (f->count < MA_WINDOW_SIZE) f->count++;
    return f->sum / f->count;
}

// Median Filter (for spike removal)
#define MEDIAN_WINDOW 5

void swap(float* a, float* b) { float t = *a; *a = *b; *b = t; }

float find_median(float* arr, int n) {
    // Simple bubble sort (OK for small n)
    float sorted[MEDIAN_WINDOW];
    memcpy(sorted, arr, n * sizeof(float));

    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (sorted[j] > sorted[j+1]) {
                swap(&sorted[j], &sorted[j+1]);
            }
        }
    }
    return sorted[n/2];
}

typedef struct {
    float buffer[MEDIAN_WINDOW];
    uint8_t index;
    uint8_t count;
} MedianFilter;

float median_update(MedianFilter* f, float input) {
    f->buffer[f->index] = input;
    f->index = (f->index + 1) % MEDIAN_WINDOW;
    if (f->count < MEDIAN_WINDOW) f->count++;
    return find_median(f->buffer, f->count);
}

// ============================================================================
// STAGE 4: PROCESSING / DETECTION
// ============================================================================

/*
 * FALL DETECTION ALGORITHM:
 *
 * 1. Calculate acceleration magnitude: |a| = sqrt(x² + y² + z²)
 * 2. Detect free-fall: |a| < 0.5g for > 200ms
 * 3. Detect impact: |a| > 3g
 * 4. Check for stillness after impact (person lying still)
 *
 * STATE MACHINE:
 *   NORMAL -> FREE_FALL (|a| < 0.5g)
 *   FREE_FALL -> IMPACT (|a| > 3g)
 *   IMPACT -> FALL_DETECTED (low motion after)
 *   IMPACT -> NORMAL (motion continues = not a fall)
 */

typedef enum {
    MOTION_NORMAL,
    MOTION_FREE_FALL,
    MOTION_IMPACT,
    MOTION_FALL_DETECTED
} MotionState;

typedef struct {
    MotionState state;
    uint32_t state_start_time;
    float magnitude_filtered;
    EmaFilter mag_filter;
    uint16_t impact_count;
    uint16_t fall_count;
} FallDetector;

void fall_detector_init(FallDetector* fd) {
    fd->state = MOTION_NORMAL;
    fd->state_start_time = 0;
    fd->impact_count = 0;
    fd->fall_count = 0;
    ema_init(&fd->mag_filter, 0.3);  // Moderate smoothing
}

MotionState fall_detector_update(FallDetector* fd, ImuSample* sample) {
    // Calculate magnitude (in g, assuming 1g = 1000 raw)
    float x = sample->x / 1000.0f;
    float y = sample->y / 1000.0f;
    float z = sample->z / 1000.0f;
    float magnitude = sqrtf(x*x + y*y + z*z);

    // Filter
    fd->magnitude_filtered = ema_update(&fd->mag_filter, magnitude);

    MotionState prev_state = fd->state;

    switch (fd->state) {
        case MOTION_NORMAL:
            if (fd->magnitude_filtered < 0.5f) {
                fd->state = MOTION_FREE_FALL;
                fd->state_start_time = sample->timestamp_ms;
            }
            break;

        case MOTION_FREE_FALL:
            if (fd->magnitude_filtered > 3.0f) {
                fd->state = MOTION_IMPACT;
                fd->state_start_time = sample->timestamp_ms;
                fd->impact_count++;
            } else if (fd->magnitude_filtered > 0.8f) {
                fd->state = MOTION_NORMAL;  // False alarm
            }
            break;

        case MOTION_IMPACT:
            // Check for stillness (fall) or continued motion (not fall)
            if (sample->timestamp_ms - fd->state_start_time > 1000) {
                if (fd->magnitude_filtered < 1.2f && fd->magnitude_filtered > 0.8f) {
                    fd->state = MOTION_FALL_DETECTED;
                    fd->fall_count++;
                    printf("*** FALL DETECTED! ***\n");
                } else {
                    fd->state = MOTION_NORMAL;
                }
            }
            break;

        case MOTION_FALL_DETECTED:
            // Stay in this state until acknowledged
            break;
    }

    if (fd->state != prev_state) {
        printf("[FALL] State: %d -> %d (mag=%.2f g)\n",
               prev_state, fd->state, fd->magnitude_filtered);
    }

    return fd->state;
}

// ============================================================================
// STAGE 5: OUTPUT / STORAGE
// ============================================================================

/*
 * OUTPUT OPTIONS:
 *
 * 1. LOCAL STORAGE (SD Card)
 *    - Buffer samples, write in chunks
 *    - Handle write latency with double-buffering
 *
 * 2. WIRELESS TRANSMISSION
 *    - Compress/pack data
 *    - Handle connection loss
 *
 * 3. DISPLAY / UI
 *    - Decimate for display rate
 *    - Format for human reading
 *
 * 4. EVENT GENERATION
 *    - Threshold crossing -> event
 *    - State change -> event
 */

#define STORAGE_CHUNK_SIZE 512

typedef struct {
    uint8_t buffer[2][STORAGE_CHUNK_SIZE];
    uint8_t write_buffer;
    uint16_t write_index;
    uint32_t total_bytes;
} StorageWriter;

void storage_init(StorageWriter* sw) {
    sw->write_buffer = 0;
    sw->write_index = 0;
    sw->total_bytes = 0;
}

bool storage_write_sample(StorageWriter* sw, SensorSample* sample) {
    uint8_t* buf = sw->buffer[sw->write_buffer];

    // Pack sample into buffer
    if (sw->write_index + sizeof(SensorSample) > STORAGE_CHUNK_SIZE) {
        // Buffer full - trigger write
        printf("[STORAGE] Buffer %d full, %u bytes total\n",
               sw->write_buffer, sw->total_bytes);

        // Swap buffers
        sw->write_buffer = 1 - sw->write_buffer;
        sw->write_index = 0;

        // In real code: trigger DMA write to SD card
    }

    memcpy(&buf[sw->write_index], sample, sizeof(SensorSample));
    sw->write_index += sizeof(SensorSample);
    sw->total_bytes += sizeof(SensorSample);

    return true;
}

// ============================================================================
// COMPLETE PIPELINE EXAMPLE
// ============================================================================

typedef struct {
    SampleBuffer raw_buffer;
    EmaFilter filter;
    FallDetector detector;
    StorageWriter storage;

    // Statistics
    uint32_t samples_acquired;
    uint32_t samples_processed;
    uint32_t events_generated;
} SensorPipeline;

void pipeline_init(SensorPipeline* p) {
    sample_buffer_init(&p->raw_buffer);
    ema_init(&p->filter, 0.2);
    fall_detector_init(&p->detector);
    storage_init(&p->storage);

    p->samples_acquired = 0;
    p->samples_processed = 0;
    p->events_generated = 0;

    printf("[PIPELINE] Initialized\n");
}

// Called from ISR or DMA callback
void pipeline_input(SensorPipeline* p, int16_t value, uint32_t timestamp) {
    sample_buffer_push(&p->raw_buffer, value, timestamp);
    p->samples_acquired++;
}

// Called from main loop
void pipeline_process(SensorPipeline* p) {
    SensorSample sample;

    while (sample_buffer_pop(&p->raw_buffer, &sample)) {
        // Filter
        float filtered = ema_update(&p->filter, sample.value);

        // For demo: create fake IMU sample
        ImuSample imu = {
            .timestamp_ms = sample.timestamp_ms,
            .x = (int16_t)(filtered * 1000),
            .y = 0,
            .z = 1000  // 1g baseline
        };

        // Process
        MotionState state = fall_detector_update(&p->detector, &imu);

        // Generate event on state change
        if (state == MOTION_FALL_DETECTED) {
            p->events_generated++;
            // Trigger alert!
        }

        // Store
        storage_write_sample(&p->storage, &sample);

        p->samples_processed++;
    }
}

// ============================================================================
// DEMO
// ============================================================================

int main() {
    printf("============================================\n");
    printf("   SENSOR DATA PIPELINE DEMO\n");
    printf("============================================\n");

    SensorPipeline pipeline;
    pipeline_init(&pipeline);

    // Simulate sensor data (normal -> free-fall -> impact -> still)
    printf("\n--- Simulating fall sequence ---\n");

    // Normal motion (1g)
    for (int i = 0; i < 10; i++) {
        pipeline_input(&pipeline, 1000, i * 10);
    }
    pipeline_process(&pipeline);

    // Free-fall (0.3g)
    for (int i = 10; i < 30; i++) {
        pipeline_input(&pipeline, 300, i * 10);
    }
    pipeline_process(&pipeline);

    // Impact (4g)
    for (int i = 30; i < 35; i++) {
        pipeline_input(&pipeline, 4000, i * 10);
    }
    pipeline_process(&pipeline);

    // Still (1g)
    for (int i = 35; i < 200; i++) {
        pipeline_input(&pipeline, 1000, i * 10);
    }
    pipeline_process(&pipeline);

    printf("\n--- Statistics ---\n");
    printf("Samples acquired: %u\n", pipeline.samples_acquired);
    printf("Samples processed: %u\n", pipeline.samples_processed);
    printf("Events generated: %u\n", pipeline.events_generated);
    printf("Fall count: %u\n", pipeline.detector.fall_count);

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW DISCUSSION POINTS
 * ============================================================================
 *
 * Q: "Walk me through your sensor pipeline design."
 * A: "Five stages:
 *    1. ACQUIRE: Timer-triggered or DMA-driven sampling
 *    2. BUFFER: Ring buffer decouples ISR from processing
 *    3. FILTER: EMA/moving average removes noise
 *    4. PROCESS: Algorithm (fall detection, etc.)
 *    5. OUTPUT: Storage or transmission
 *
 *    Key: Each stage runs at its own rate.
 *    ISR samples at 400Hz, processing might run at 100Hz."
 *
 * Q: "Why use double-buffering for DMA?"
 * A: "DMA writes to one buffer while CPU processes the other.
 *    No copying needed, no race conditions.
 *    DMA half-complete interrupt signals first buffer ready.
 *    DMA complete interrupt signals second buffer ready.
 *    CPU always has time to process before buffer reused."
 *
 * Q: "How do you choose filter parameters?"
 * A: "Based on signal characteristics:
 *    - Signal bandwidth: filter cutoff = 2× signal BW
 *    - Noise frequency: filter should attenuate noise
 *    - Latency requirement: more filtering = more delay
 *
 *    For IMU fall detection: α=0.3 (moderate smoothing)
 *    Too much: miss fast impacts
 *    Too little: false triggers from noise"
 *
 * Q: "How would you add another sensor type?"
 * A: "The pipeline is generic:
 *    1. Add sensor config (sample rate, scale)
 *    2. Create sensor-specific acquisition
 *    3. Use same buffer/filter infrastructure
 *    4. Add sensor-specific processing algorithm
 *    5. Route output to appropriate destination"
 *
 * ============================================================================
 */
