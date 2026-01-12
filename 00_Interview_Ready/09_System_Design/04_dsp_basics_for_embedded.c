/*
 * ============================================================================
 *              DSP CONSIDERATIONS FOR EMBEDDED ENGINEERS
 * ============================================================================
 *
 * This is NOT a DSP course. This is what firmware engineers need to know
 * when working with real sensor data, ADCs, and signal processing on MCUs.
 *
 * ============================================================================
 *                    WHAT IS DSP IN EMBEDDED CONTEXT?
 * ============================================================================
 *
 * DSP = Taking messy real-world signals and making them usable.
 *
 * YOUR SENSORS ARE NOT PERFECT:
 *   - Temperature sensor has noise (+/- 0.5°C jitter)
 *   - Accelerometer picks up vibrations
 *   - ADC readings fluctuate
 *   - Power supply noise couples into signals
 *
 * DSP in firmware means:
 *   - Cleaning up noisy sensor data
 *   - Smoothing out readings
 *   - Detecting patterns (zero-crossings, peaks)
 *   - Converting between representations
 *
 * ============================================================================
 *                       SAMPLING BASICS (INTUITION)
 * ============================================================================
 *
 * SAMPLING RATE (Fs):
 *   How often you read the sensor. 100 Hz = 100 readings per second.
 *
 * NYQUIST RULE (THE IMPORTANT ONE):
 *   To capture a frequency, you must sample at LEAST 2x that frequency.
 *
 *   Example: To capture a 50 Hz vibration, sample at >= 100 Hz.
 *   If you sample too slow, you get ALIASING (wrong frequency appears).
 *
 * ALIASING (THE PROBLEM):
 *   Fast signal + slow sampling = fake slow signal in your data.
 *   Like a spinning wheel in movies appearing to go backwards.
 *
 *   FIX: Either sample faster, or filter out high frequencies BEFORE ADC.
 *
 * PRACTICAL RULE OF THUMB:
 *   Sample at 5-10x the highest frequency you care about.
 *   For 50 Hz signal: sample at 250-500 Hz.
 *
 * ============================================================================
 *                         FILTERING (CONCEPTUAL)
 * ============================================================================
 *
 * LOW-PASS FILTER (LPF):
 *   Lets slow changes through, blocks fast noise.
 *   Use for: Smoothing sensor readings, removing high-freq noise.
 *
 *   ANALOG: RC circuit before ADC (hardware anti-aliasing)
 *   DIGITAL: Moving average, IIR filter in code
 *
 * HIGH-PASS FILTER (HPF):
 *   Blocks slow changes, lets fast changes through.
 *   Use for: Removing DC offset, detecting sudden changes.
 *
 * MOVING AVERAGE (SIMPLEST DIGITAL LPF):
 *   output = (sample[0] + sample[1] + ... + sample[N-1]) / N
 *   Easy, effective, costs N samples of memory.
 *
 * EXPONENTIAL MOVING AVERAGE (EMA):
 *   output = alpha * new_sample + (1 - alpha) * old_output
 *   Only needs one variable! Great for embedded.
 *   alpha = 0.1 (heavy smoothing) to 0.9 (light smoothing)
 *
 * ============================================================================
 *                    TIME DOMAIN vs FREQUENCY DOMAIN
 * ============================================================================
 *
 * TIME DOMAIN: "What is the value at this moment?"
 *   - Raw ADC readings over time
 *   - What you normally work with in firmware
 *
 * FREQUENCY DOMAIN: "What frequencies make up this signal?"
 *   - FFT converts time -> frequency
 *   - Useful for: Vibration analysis, audio, finding periodic patterns
 *
 * WHEN DO YOU NEED FFT?
 *   - Identifying vibration frequencies in motors
 *   - Audio processing
 *   - Finding the "dominant frequency" of a signal
 *
 * WHEN DON'T YOU NEED FFT?
 *   - Most sensor smoothing (moving average is enough)
 *   - Simple threshold detection
 *   - Basic filtering
 *
 * ============================================================================
 *                    FIXED-POINT vs FLOATING-POINT
 * ============================================================================
 *
 * FLOATING-POINT (float, double):
 *   + Easy to use, natural for math
 *   - Slow on MCUs without FPU (software emulation)
 *   - Uses more flash/RAM
 *
 * FIXED-POINT (integers representing fractions):
 *   + Fast on any MCU (just integer ops)
 *   + Less memory
 *   - Harder to code, must track decimal point manually
 *   - Risk of overflow
 *
 * COMMON FIXED-POINT FORMAT: Q15 (1 sign bit, 15 fractional bits)
 *   Range: -1.0 to +0.999...
 *   32767 represents ~1.0, -32768 represents -1.0
 *
 * PRACTICAL ADVICE:
 *   - Cortex-M4F, M7: Use float freely (hardware FPU)
 *   - Cortex-M0, M3: Consider fixed-point for tight loops
 *   - Profile first! Float may be fast enough.
 *
 * ============================================================================
 *                       REAL SIGNALS ARE MESSY
 * ============================================================================
 *
 * EXPECT IN REAL SYSTEMS:
 *   - Noise (random variations)
 *   - Offset drift (zero point shifts with temperature)
 *   - Spikes/glitches (EMI, power fluctuations)
 *   - Non-linearity (sensor not perfectly linear)
 *   - Quantization (ADC has limited resolution)
 *
 * DEFENSIVE CODING:
 *   - Always filter/smooth sensor data
 *   - Use median filter to reject spikes
 *   - Calibrate offsets at startup
 *   - Sanity-check readings (is value physically possible?)
 *   - Consider hysteresis for threshold decisions
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Simple Exponential Moving Average filter
typedef struct {
    float alpha;   // Smoothing factor (0.0 to 1.0)
    float output;  // Current filtered value
    int initialized;
} EMAFilter;

void ema_init(EMAFilter* f, float alpha) {
    f->alpha = alpha;
    f->output = 0;
    f->initialized = 0;
}

float ema_update(EMAFilter* f, float new_sample) {
    if (!f->initialized) {
        f->output = new_sample;  // First sample: no filtering
        f->initialized = 1;
    } else {
        f->output = f->alpha * new_sample + (1.0f - f->alpha) * f->output;
    }
    return f->output;
}

// Simple Moving Average filter
#define MA_SIZE 8

typedef struct {
    float buffer[MA_SIZE];
    int index;
    float sum;
    int count;
} MAFilter;

void ma_init(MAFilter* f) {
    f->index = 0;
    f->sum = 0;
    f->count = 0;
    for (int i = 0; i < MA_SIZE; i++) f->buffer[i] = 0;
}

float ma_update(MAFilter* f, float new_sample) {
    f->sum -= f->buffer[f->index];       // Remove oldest
    f->buffer[f->index] = new_sample;    // Store new
    f->sum += new_sample;                // Add new
    f->index = (f->index + 1) % MA_SIZE; // Wrap around
    if (f->count < MA_SIZE) f->count++;
    return f->sum / f->count;
}

int main() {
    // Simulate noisy sensor data
    float noisy_data[] = {10.2, 10.8, 9.5, 11.0, 10.1, 10.9, 9.8, 10.3, 10.5, 10.0};
    int n = sizeof(noisy_data) / sizeof(noisy_data[0]);

    EMAFilter ema;
    MAFilter ma;
    ema_init(&ema, 0.3);  // alpha=0.3 for moderate smoothing
    ma_init(&ma);

    printf("Sample\t| Raw\t| EMA\t| MA\n");
    printf("--------|-------|-------|------\n");
    for (int i = 0; i < n; i++) {
        float ema_out = ema_update(&ema, noisy_data[i]);
        float ma_out = ma_update(&ma, noisy_data[i]);
        printf("%d\t| %.1f\t| %.2f\t| %.2f\n", i, noisy_data[i], ema_out, ma_out);
    }

    return 0;
}

/*
 * ============================================================================
 * DRY RUN: EMA Filter with alpha=0.3
 * ============================================================================
 *
 * Formula: output = 0.3 * new + 0.7 * old_output
 *
 * Sample 0 (10.2): First sample, output = 10.2 (no filtering)
 * Sample 1 (10.8): output = 0.3*10.8 + 0.7*10.2 = 3.24 + 7.14 = 10.38
 * Sample 2 (9.5):  output = 0.3*9.5 + 0.7*10.38 = 2.85 + 7.27 = 10.12
 * Sample 3 (11.0): output = 0.3*11.0 + 0.7*10.12 = 3.30 + 7.08 = 10.38
 *
 * NOTICE: Noisy input (9.5 to 11.0), but output stays around 10.
 *         Filter smooths out the variations!
 *
 * ============================================================================
 * INTERVIEW TIPS:
 * - Know EMA formula: output = alpha * new + (1-alpha) * old
 * - Moving average needs buffer, EMA needs only one variable
 * - Lower alpha = more smoothing, slower response
 * - Nyquist: sample at 2x max frequency (practically: 5-10x)
 * - Fixed-point for MCUs without FPU, float otherwise
 * ============================================================================
 */
