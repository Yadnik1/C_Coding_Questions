/*
 * ============================================================================
 *    COUPLING CAPACITORS & ELECTRONIC NOISE FOR EMBEDDED ENGINEERS
 * ============================================================================
 *
 * This is CRITICAL knowledge for debugging real hardware issues.
 * Many firmware bugs are actually hardware/noise problems!
 *
 * ============================================================================
 *                    TYPES OF CAPACITOR USAGE
 * ============================================================================
 *
 * 1. DECOUPLING (Bypass) Capacitors
 *    - Placed between VCC and GND near IC
 *    - Provides local charge reservoir
 *    - Filters high-frequency noise on power rail
 *
 * 2. COUPLING Capacitors
 *    - Placed in SIGNAL path (not power)
 *    - Passes AC signal, blocks DC offset
 *    - Used in audio, RF, sensor interfaces
 *
 * 3. BULK Capacitors
 *    - Large value (10-1000µF)
 *    - Smooths low-frequency ripple
 *    - After voltage regulators
 *
 * ============================================================================
 *                    DECOUPLING vs COUPLING
 * ============================================================================
 *
 * DECOUPLING (Power filtering):
 *
 *     VCC ----+----+---- MCU VCC
 *             |    |
 *           [10µF][100nF]   ← Caps to GND
 *             |    |
 *     GND ----+----+---- MCU GND
 *
 *     Purpose: Keep VCC clean, supply transient current
 *
 *
 * COUPLING (Signal path):
 *
 *     Signal IN ---[C]--- Signal OUT
 *                  |
 *              (DC blocked)
 *
 *     Purpose: Pass AC signal, block DC component
 *
 * ============================================================================
 *                    COUPLING CAPACITORS IN DETAIL
 * ============================================================================
 *
 * WHY USE COUPLING CAPS?
 *
 * 1. AC COUPLING IN AUDIO:
 *
 *    Microphone output has DC bias + AC signal:
 *
 *        DC bias (1.5V) + Audio signal (±50mV)
 *                |
 *               [C] 10µF coupling cap
 *                |
 *        Pure AC audio signal (±50mV around 0V)
 *                |
 *            MCU ADC (with its own bias)
 *
 *
 * 2. SENSOR SIGNAL COUPLING:
 *
 *    Some sensors output signal with DC offset different from MCU:
 *
 *    Sensor (5V logic)        MCU (3.3V logic)
 *         |                        |
 *    DC: 2.5V                 DC: 1.65V
 *    AC: ±1V         →       AC: ±1V
 *         |                        |
 *        [C]--------------------[R bias]
 *
 *
 * 3. RF COUPLING:
 *
 *    Antenna → [C] → LNA (Low Noise Amplifier)
 *
 *    Blocks DC from antenna, passes RF signal
 *
 *
 * COUPLING CAP VALUE SELECTION:
 *
 *    Xc = 1 / (2π × f × C)   ← Capacitor impedance
 *
 *    For good coupling (low impedance at signal frequency):
 *    Xc should be << Load impedance
 *
 *    Example: Audio (100Hz to 20kHz)
 *    - At 100Hz with 10µF: Xc = 159Ω
 *    - At 100Hz with 1µF:  Xc = 1.59kΩ (might attenuate bass!)
 *
 *    Rule of thumb:
 *    - Audio: 1-10µF (electrolytics OK)
 *    - RF: 100pF-10nF (ceramics required)
 *
 * ============================================================================
 *                    ELECTRONIC NOISE SOURCES
 * ============================================================================
 *
 * 1. POWER SUPPLY NOISE:
 *
 *    VCC (ideal)      VCC (reality)
 *        ___             /\/\/\
 *       |   |          /        \
 *    ___|   |___      /          \
 *                    Ripple + noise
 *
 *    Sources:
 *    - Switching regulator ripple (at switching frequency)
 *    - Load transients (MCU drawing current spikes)
 *    - Shared ground impedance
 *
 *    FIX: Decoupling caps (100nF + 10µF)
 *
 *
 * 2. SWITCHING NOISE (Digital IC crosstalk):
 *
 *    Every digital edge creates noise:
 *
 *    Clock line: _____|‾‾‾‾‾|_____|‾‾‾‾‾|_____
 *                     ↓     ↓     ↓     ↓
 *                   Noise spikes couple to nearby traces
 *
 *    FIX:
 *    - Separate analog and digital sections
 *    - Ground plane between traces
 *    - Keep sensitive analog traces away from clocks
 *
 *
 * 3. GROUND BOUNCE:
 *
 *    When multiple outputs switch simultaneously:
 *
 *    MCU ----[trace inductance]---- GND
 *             ↑
 *       V = L × (di/dt)
 *
 *    Large di/dt from switching → voltage spike on GND
 *    GND is no longer 0V! (can be 100s of mV)
 *
 *    FIX:
 *    - Multiple GND pins
 *    - Wide GND traces or planes
 *    - Decouple each IC separately
 *
 *
 * 4. EMI (Electromagnetic Interference):
 *
 *    External sources:
 *    - Motors, relays, solenoids
 *    - WiFi, Bluetooth, cellular
 *    - ESD (static discharge)
 *    - Lightning, power line transients
 *
 *    FIX:
 *    - Shielding (metal enclosure)
 *    - Filtering (ferrite beads on cables)
 *    - TVS diodes for ESD
 *    - Keep cables short
 *
 *
 * 5. THERMAL NOISE (Johnson-Nyquist):
 *
 *    All resistors generate noise voltage:
 *    Vn = √(4 × k × T × R × BW)
 *
 *    k = Boltzmann constant
 *    T = Temperature (Kelvin)
 *    R = Resistance
 *    BW = Bandwidth
 *
 *    Example: 10kΩ resistor at room temp, 10kHz BW
 *    Vn ≈ 1.3µV (usually negligible, but matters for precision)
 *
 *    FIX: Use lower resistance where possible
 *
 *
 * 6. ADC NOISE:
 *
 *    Sources:
 *    - Reference voltage noise
 *    - Sampling clock jitter
 *    - Quantization noise
 *    - Input signal noise
 *
 *    FIX:
 *    - Oversample and average
 *    - Low-pass filter before ADC
 *    - Stable voltage reference
 *    - Sample when MCU is quiet (no SPI, etc.)
 *
 * ============================================================================
 *                    NOISE FILTERING TECHNIQUES
 * ============================================================================
 *
 * 1. RC LOW-PASS FILTER:
 *
 *    Signal ---[R]---+--- Filtered
 *                    |
 *                   [C]
 *                    |
 *                   GND
 *
 *    fc = 1 / (2π × R × C)
 *
 *    Example: R=10kΩ, C=100nF → fc = 159Hz
 *    Passes signals below 159Hz, attenuates higher
 *
 *
 * 2. FERRITE BEAD:
 *
 *    Power ---[FB]--- IC VCC
 *
 *    Acts like inductor at high frequencies
 *    Blocks HF noise, passes DC
 *    Common on USB VBUS, power inputs
 *
 *    Schematic symbol: ──▌▌──
 *
 *
 * 3. LC FILTER:
 *
 *    VCC ---[L]---+--- Filtered VCC
 *                 |
 *                [C]
 *                 |
 *                GND
 *
 *    Better filtering than RC (no power loss in resistor)
 *    Used after switching regulators
 *
 *
 * 4. PI FILTER (CLC):
 *
 *    VCC ---+---[L]---+--- Filtered VCC
 *           |         |
 *          [C]       [C]
 *           |         |
 *          GND       GND
 *
 *    Even better filtering for sensitive analog sections
 *
 *
 * 5. COMMON MODE CHOKE:
 *
 *    For differential signals (USB, Ethernet, CAN):
 *
 *    Signal+ ---))))---  → Passes differential
 *    Signal- ---))))---  → Blocks common mode noise
 *
 *
 * 6. TVS DIODE (ESD Protection):
 *
 *    Signal ---+--- To MCU
 *              |
 *           [TVS]
 *              |
 *             GND
 *
 *    Clamps voltage spikes (ESD, transients)
 *    Normally open, conducts when voltage exceeds threshold
 *
 * ============================================================================
 *                DECOUPLING CAPACITOR PLACEMENT
 * ============================================================================
 *
 * WRONG:
 *
 *    VCC ─────────────────────────┬──── MCU
 *         long trace             [C]
 *    GND ─────────────────────────┴────
 *
 *    Problem: Trace inductance defeats capacitor purpose
 *
 *
 * CORRECT:
 *
 *    VCC ──┬─────────────────────────── MCU
 *          │  ← very short!
 *         [C]
 *          │
 *    GND ──┴───────────────────────────
 *
 *    Rule: < 3mm from IC power pins
 *
 *
 * MULTIPLE CAPS STRATEGY:
 *
 *    VCC ──┬────┬────┬──── MCU VCC
 *          │    │    │
 *       [10µF][1µF][100nF]  ← Different values
 *          │    │    │
 *    GND ──┴────┴────┴──── MCU GND
 *
 *    Why multiple values?
 *    - 100nF: High-frequency noise (MHz)
 *    - 1µF: Medium frequency (100kHz)
 *    - 10µF: Low-frequency ripple, bulk charge
 *
 *    Each cap has resonant frequency where it's most effective
 *
 * ============================================================================
 *                    DEBUGGING NOISE ISSUES
 * ============================================================================
 *
 * SYMPTOMS OF NOISE PROBLEMS:
 *
 * 1. ADC readings jump around (even with stable input)
 * 2. Random resets or brownouts
 * 3. Communication errors (UART, SPI, I2C)
 * 4. Different behavior in different environments
 * 5. Problems appear/disappear when touching board
 * 6. Works on bench, fails in field
 *
 *
 * DEBUG STEPS:
 *
 * 1. CHECK POWER SUPPLY:
 *    - Measure VCC with oscilloscope (not just multimeter!)
 *    - Look for ripple, spikes, dropouts
 *    - Check during MCU activity (not just idle)
 *
 * 2. CHECK DECOUPLING:
 *    - Is 100nF cap present and close?
 *    - Is bulk cap present?
 *    - Are caps actually soldered properly?
 *
 * 3. CHECK GROUNDING:
 *    - Is there a ground plane?
 *    - Are analog and digital grounds separate but connected at one point?
 *    - Any ground loops?
 *
 * 4. CHECK SIGNAL INTEGRITY:
 *    - Look at clock signals on scope
 *    - Check for ringing, overshoot
 *    - Check analog signals for noise
 *
 * 5. CHECK ENVIRONMENT:
 *    - Motors nearby?
 *    - Long cables acting as antennas?
 *    - ESD issues?
 *
 *
 * FIRMWARE MITIGATIONS:
 *
 * 1. OVERSAMPLE AND AVERAGE:
 *    Take 16 ADC readings, average them
 *    Reduces random noise by √16 = 4x
 *
 * 2. DIGITAL FILTERING:
 *    Moving average or EMA on sensor data
 *
 * 3. DEBOUNCING:
 *    Ignore rapid state changes on inputs
 *
 * 4. RETRY WITH BACKOFF:
 *    Communication failed? Wait and retry
 *
 * 5. WATCHDOG:
 *    Recover from noise-induced crashes
 *
 * 6. CRC/CHECKSUMS:
 *    Detect corrupted data
 *
 * ============================================================================
 */

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Calculate capacitor impedance at given frequency
float calc_cap_impedance(float capacitance_f, float frequency_hz) {
    return 1.0f / (2.0f * M_PI * frequency_hz * capacitance_f);
}

// Calculate RC filter cutoff frequency
float calc_rc_cutoff(float r_ohms, float c_farads) {
    return 1.0f / (2.0f * M_PI * r_ohms * c_farads);
}

// Calculate thermal noise voltage
float calc_thermal_noise(float resistance, float bandwidth, float temp_kelvin) {
    const float k = 1.38e-23f;  // Boltzmann constant
    return sqrtf(4.0f * k * temp_kelvin * resistance * bandwidth);
}

// Simple oversampling: average N ADC readings
uint16_t oversample_adc(uint16_t readings[], int count) {
    uint32_t sum = 0;
    for (int i = 0; i < count; i++) {
        sum += readings[i];
    }
    return (uint16_t)(sum / count);
}

// EMA filter for noise reduction
typedef struct {
    float value;
    float alpha;  // 0.0 to 1.0, lower = more smoothing
} EMAFilter;

void ema_init(EMAFilter* f, float alpha, float initial) {
    f->alpha = alpha;
    f->value = initial;
}

float ema_update(EMAFilter* f, float new_sample) {
    f->value = f->alpha * new_sample + (1.0f - f->alpha) * f->value;
    return f->value;
}

// Moving average filter
typedef struct {
    float buffer[16];
    int index;
    int count;
    float sum;
} MovingAvgFilter;

void mavg_init(MovingAvgFilter* f) {
    for (int i = 0; i < 16; i++) f->buffer[i] = 0;
    f->index = 0;
    f->count = 0;
    f->sum = 0;
}

float mavg_update(MovingAvgFilter* f, float new_sample) {
    // Remove oldest from sum
    f->sum -= f->buffer[f->index];
    // Add new sample
    f->buffer[f->index] = new_sample;
    f->sum += new_sample;
    // Update index
    f->index = (f->index + 1) % 16;
    if (f->count < 16) f->count++;
    return f->sum / f->count;
}

int main() {
    printf("=== COUPLING CAPS & NOISE CALCULATIONS ===\n\n");

    // Capacitor impedance at different frequencies
    printf("Capacitor Impedance (Xc):\n");
    printf("  10µF at 100Hz:  Xc = %.1f Ω\n", calc_cap_impedance(10e-6, 100));
    printf("  10µF at 1kHz:   Xc = %.1f Ω\n", calc_cap_impedance(10e-6, 1000));
    printf("  100nF at 1MHz:  Xc = %.2f Ω\n", calc_cap_impedance(100e-9, 1e6));
    printf("  100nF at 10MHz: Xc = %.3f Ω\n\n", calc_cap_impedance(100e-9, 10e6));

    // RC filter cutoff
    printf("RC Filter Cutoff Frequencies:\n");
    printf("  R=10kΩ, C=100nF: fc = %.1f Hz\n", calc_rc_cutoff(10000, 100e-9));
    printf("  R=1kΩ, C=10µF:   fc = %.1f Hz\n", calc_rc_cutoff(1000, 10e-6));
    printf("  R=10kΩ, C=10nF:  fc = %.0f Hz\n\n", calc_rc_cutoff(10000, 10e-9));

    // Thermal noise
    printf("Thermal Noise (Johnson-Nyquist):\n");
    float vn = calc_thermal_noise(10000, 10000, 300);  // 10kΩ, 10kHz BW, 300K
    printf("  10kΩ resistor, 10kHz BW: Vn = %.2f µV\n\n", vn * 1e6);

    // Oversampling demo
    printf("Oversampling Demo:\n");
    uint16_t noisy_readings[] = {1000, 1020, 990, 1010, 1005, 995, 1015, 985,
                                  1008, 1012, 998, 1002, 1018, 992, 1006, 1004};
    uint16_t averaged = oversample_adc(noisy_readings, 16);
    printf("  Individual readings: 1000, 1020, 990, 1010, ...\n");
    printf("  16-sample average: %d\n\n", averaged);

    // EMA filter demo
    printf("EMA Filter Demo (alpha=0.2):\n");
    EMAFilter ema;
    ema_init(&ema, 0.2, 1000);
    float noisy[] = {1050, 950, 1030, 970, 1010, 990, 1020, 980};
    printf("  Raw → Filtered:\n");
    for (int i = 0; i < 8; i++) {
        float filtered = ema_update(&ema, noisy[i]);
        printf("  %4.0f → %6.1f\n", noisy[i], filtered);
    }

    return 0;
}

/*
 * ============================================================================
 *                    INTERVIEW QUESTIONS & ANSWERS
 * ============================================================================
 *
 * Q1: "What's the difference between coupling and decoupling capacitor?"
 * A1:
 *   DECOUPLING: VCC to GND, filters power supply noise
 *   COUPLING: In signal path, blocks DC and passes AC
 *
 *   Decoupling = power filtering
 *   Coupling = signal passing (DC removal)
 *
 * Q2: "ADC readings are noisy. How do you fix it?"
 * A2:
 *   Hardware fixes:
 *   1. Add decoupling cap on ADC VCC
 *   2. RC low-pass filter on ADC input
 *   3. Check analog and digital ground separation
 *   4. Use ADC's internal sample-and-hold properly
 *
 *   Firmware fixes:
 *   1. Oversample and average (16x → 4x noise reduction)
 *   2. EMA or moving average filter
 *   3. Sample when MCU is quiet (pause SPI/PWM)
 *
 * Q3: "Why use multiple decoupling cap values (100nF + 10µF)?"
 * A3:
 *   Each capacitor has resonant frequency where it's most effective.
 *   - 100nF: Best at high frequencies (MHz) - near IC
 *   - 10µF: Best at lower frequencies (kHz) - bulk charge
 *   Together they cover wide frequency range.
 *   Also, 100nF can't supply large transient currents alone.
 *
 * Q4: "What is ground bounce and how to prevent it?"
 * A4:
 *   When multiple outputs switch simultaneously, current spike
 *   through ground trace inductance creates voltage: V = L × di/dt
 *
 *   GND is no longer 0V - causes logic errors!
 *
 *   Prevention:
 *   - Multiple GND pins on IC
 *   - Wide GND traces or ground plane
 *   - Decoupling caps close to IC
 *   - Don't switch all outputs simultaneously
 *
 * Q5: "Device works in lab but fails in factory. What could be wrong?"
 * A5:
 *   Factory environment has more EMI:
 *   - Motors, conveyors, welders
 *   - Fluorescent lights
 *   - Other equipment nearby
 *
 *   Debug steps:
 *   1. Check with oscilloscope in factory
 *   2. Add filtering on inputs/power
 *   3. Add shielding
 *   4. Check cable lengths (antennas!)
 *   5. Add TVS diodes for ESD
 *
 * Q6: "How does a ferrite bead work?"
 * A6:
 *   Ferrite bead = inductor that absorbs high-frequency noise
 *   - Low resistance at DC (doesn't drop voltage)
 *   - High impedance at high frequencies (blocks noise)
 *   - Converts HF energy to heat (absorbs, not reflects)
 *
 *   Used on power inputs, USB lines, between analog/digital sections.
 *
 * Q7: "What causes random MCU resets?"
 * A7:
 *   Common causes:
 *   1. Power supply dropout (brown-out)
 *   2. ESD spike on reset pin
 *   3. Watchdog timeout from stuck code
 *   4. Stack overflow
 *   5. Noise on power rail triggering brown-out detector
 *
 *   Debug: Check VCC with scope, add decoupling, check reset pin
 *
 * Q8: "How do you design a low-pass filter for ADC input?"
 * A8:
 *   1. Determine signal bandwidth (e.g., 100Hz for temperature)
 *   2. Set cutoff at 2-10x signal frequency (e.g., 500Hz)
 *   3. Calculate: R × C = 1 / (2π × fc)
 *   4. Choose R (consider ADC input impedance, source impedance)
 *   5. Calculate C
 *
 *   Example: fc = 500Hz, R = 10kΩ
 *   C = 1 / (2π × 500 × 10000) = 32nF → use 33nF
 *
 * ============================================================================
 *                        SCHEMATIC PATTERNS
 * ============================================================================
 *
 * STANDARD MCU POWER DECOUPLING:
 *
 *    VCC rail ──┬────┬────┬────┬──── VCC pins
 *               │    │    │    │
 *            [10µF][1µF][100nF][100nF]
 *               │    │    │    │
 *    GND rail ──┴────┴────┴────┴──── GND pins
 *                    ↑
 *              Electrolytic (bulk)
 *
 *
 * ADC INPUT FILTERING:
 *
 *    Sensor ──[R 10k]──┬── ADC Pin
 *                      │
 *                    [C 100nF]
 *                      │
 *                     GND
 *
 *
 * AUDIO COUPLING:
 *
 *    Mic ──[C 10µF]──┬──[R 10k]── ADC
 *                    │
 *                    └── Bias voltage (VCC/2)
 *
 *
 * EMI FILTER ON POWER INPUT:
 *
 *    Ext VCC ──[FB]──┬── Internal VCC
 *                    │
 *                  [C 10µF]
 *                    │
 *    Ext GND ────────┴── Internal GND
 *
 *    FB = Ferrite Bead
 *
 * ============================================================================
 */
