/*
 * ============================================================================
 *                    OP-AMPS FOR EMBEDDED ENGINEERS
 * ============================================================================
 *
 * Op-amps appear in sensor interfaces, filters, and signal conditioning.
 * You don't need to design complex circuits, but MUST understand basics.
 *
 * ============================================================================
 *                         WHAT IS AN OP-AMP?
 * ============================================================================
 *
 * Operational Amplifier = High-gain differential amplifier
 *
 * SYMBOL:
 *
 *              +V (power)
 *               |
 *        (+)---|\
 *              | >-------- OUTPUT
 *        (-)---|/
 *               |
 *              -V (or GND)
 *
 *   (+) = Non-inverting input
 *   (-) = Inverting input
 *
 * GOLDEN RULES (for circuits with negative feedback):
 *
 *   1. NO CURRENT flows into inputs (infinite input impedance)
 *   2. V(+) = V(-) (op-amp adjusts output to make inputs equal)
 *
 * These rules let you analyze almost any op-amp circuit!
 *
 * ============================================================================
 *                    BASIC CONFIGURATIONS
 * ============================================================================
 *
 * 1. VOLTAGE FOLLOWER (BUFFER)
 * ----------------------------
 *
 *   VIN -----(+)---|\
 *                  | >----+---- VOUT
 *            +-(-)---|/    |
 *            |             |
 *            +-------------+
 *
 *   VOUT = VIN (unity gain)
 *
 *   USE: Isolate high-impedance source from low-impedance load
 *        (e.g., buffer voltage divider output)
 *
 *   WHY? Input impedance very high, output impedance very low
 *        Can drive loads without affecting source
 *
 * 2. NON-INVERTING AMPLIFIER
 * --------------------------
 *
 *   VIN -----(+)---|\
 *                  | >----+---- VOUT
 *            +-(-)---|/    |
 *            |             |
 *           [R1]          [Rf] (feedback)
 *            |             |
 *           GND -----------+
 *
 *   GAIN = 1 + Rf/R1
 *
 *   Example: R1 = 10kΩ, Rf = 90kΩ
 *   Gain = 1 + 90k/10k = 10×
 *
 * 3. INVERTING AMPLIFIER
 * ----------------------
 *
 *                  [Rf]
 *            +-----/\/\/\----+
 *            |               |
 *   VIN ---[Rin]---(-)---|\  |
 *                        | >-+---- VOUT
 *           GND ----(+)--|/
 *
 *   GAIN = -Rf / Rin  (negative = inverted)
 *
 *   Example: Rin = 10kΩ, Rf = 100kΩ
 *   Gain = -100k/10k = -10×
 *
 * 4. DIFFERENTIAL AMPLIFIER
 * -------------------------
 *
 *   V1 ---[R1]---+
 *                |
 *   V2 ---[R2]---(-)---|\
 *                      | >---- VOUT
 *           REF--(+)---|/
 *
 *   VOUT = (V2 - V1) × Rf/Rin  (when R1=R2 and both feedback R equal)
 *
 *   USE: Measure difference between two voltages
 *        Common in sensor bridges, current sensing
 *
 * ============================================================================
 *                    COMMON USES IN EMBEDDED
 * ============================================================================
 *
 * 1. BUFFER FOR VOLTAGE DIVIDER
 *
 *   Problem: Voltage divider output changes when loaded
 *   Solution: Buffer with voltage follower
 *
 *   VIN---[R1]---+              +---|\
 *                |              |   | >---VOUT (can drive loads!)
 *               [R2]  -->  VDIV-(+)-|/
 *                |              |
 *               GND         feedback loop
 *
 * 2. AMPLIFY SENSOR SIGNAL
 *
 *   Many sensors output millivolts (e.g., thermocouple: 40µV/°C)
 *   ADC needs 0-3.3V range
 *   Solution: Non-inverting amp with gain = 100×
 *
 *   Sensor(mV) ----(+)---|\
 *                        | >----+---- To ADC (Volts)
 *             +---(-)--- |/     |
 *             |                 |
 *            [R1]             [Rf]
 *             |                 |
 *            GND ---------------+
 *
 * 3. LEVEL SHIFT + AMPLIFY
 *
 *   Bipolar signal (-1V to +1V) → Unipolar (0V to 3.3V)
 *   Add offset voltage to shift, then amplify
 *
 * 4. ACTIVE LOW-PASS FILTER
 *
 *   Better than passive RC filter (no loading, can have gain)
 *
 *                  [Rf]
 *            +-----/\/\/\----+
 *            |      [Cf]     |
 *            +-----||--------+
 *            |               |
 *   VIN ---[Rin]---(-)---|\  |
 *                        | >-+---- VOUT
 *           GND ----(+)--|/
 *
 *   fc = 1 / (2π × Rf × Cf)
 *
 * 5. COMPARATOR (technically misuse, but common)
 *
 *   VIN -----(+)---|\
 *                  | >-------- HIGH if VIN > VREF
 *   VREF ----(-)---|/          LOW if VIN < VREF
 *
 *   Note: Dedicated comparators are faster and have rail-to-rail output
 *
 * ============================================================================
 *                    PRACTICAL CONSIDERATIONS
 * ============================================================================
 *
 * 1. POWER SUPPLY:
 *    - Single supply (0V to VCC): Output can't go below ~0.2V or above VCC-0.2V
 *    - Dual supply (±V): Output can swing positive and negative
 *    - Rail-to-rail op-amps: Output reaches close to power rails
 *
 * 2. INPUT OFFSET VOLTAGE:
 *    - Real op-amps have small offset (millivolts)
 *    - Gets amplified by gain
 *    - High-precision apps need low-offset op-amps or calibration
 *
 * 3. BANDWIDTH (Gain-Bandwidth Product):
 *    - GBW = Gain × Bandwidth
 *    - Higher gain = lower bandwidth
 *    - Example: GBW = 1 MHz, Gain = 100 → BW = 10 kHz
 *
 * 4. SLEW RATE:
 *    - How fast output can change (V/µs)
 *    - Limits high-frequency signals
 *
 * ============================================================================
 */

#include <stdio.h>

// Non-inverting amplifier gain
float non_inv_gain(float rf, float r1) {
    return 1.0 + rf / r1;
}

// Inverting amplifier gain
float inv_gain(float rf, float rin) {
    return -rf / rin;
}

// Calculate Rf for desired non-inverting gain (given R1)
float calc_rf_for_gain(float desired_gain, float r1) {
    // Gain = 1 + Rf/R1
    // Rf/R1 = Gain - 1
    // Rf = R1 × (Gain - 1)
    return r1 * (desired_gain - 1);
}

// Active LPF cutoff frequency
float active_lpf_cutoff(float rf, float cf) {
    return 1.0 / (2.0 * 3.14159 * rf * cf);
}

int main() {
    printf("=== OP-AMP CALCULATIONS ===\n\n");

    // Non-inverting amplifier
    printf("Non-Inverting Amplifier:\n");
    printf("  R1 = 10kΩ, Rf = 90kΩ\n");
    printf("  Gain = %.1f×\n\n", non_inv_gain(90000, 10000));

    // Inverting amplifier
    printf("Inverting Amplifier:\n");
    printf("  Rin = 10kΩ, Rf = 100kΩ\n");
    printf("  Gain = %.1f× (inverted)\n\n", inv_gain(100000, 10000));

    // Design for specific gain
    printf("Design: Need 50× gain with R1 = 10kΩ\n");
    float rf = calc_rf_for_gain(50, 10000);
    printf("  Rf needed = %.0f kΩ\n\n", rf / 1000);

    // Active filter
    printf("Active Low-Pass Filter:\n");
    printf("  Rf = 100kΩ, Cf = 100nF\n");
    float fc = active_lpf_cutoff(100000, 100e-9);
    printf("  Cutoff = %.1f Hz\n", fc);

    return 0;
}

/*
 * ============================================================================
 * INTERVIEW QUESTIONS & ANSWERS
 * ============================================================================
 *
 * Q1: "What are the golden rules of op-amps?"
 * A1:
 *   1. No current flows into inputs (infinite input impedance)
 *   2. V(+) = V(-) when negative feedback is present
 *   These only apply with negative feedback (closed loop)!
 *
 * Q2: "What's the gain of a voltage follower?"
 * A2:
 *   Gain = 1 (unity gain)
 *   Output equals input exactly
 *   Purpose: Buffer - high input impedance, low output impedance
 *
 * Q3: "Calculate the gain of a non-inverting amp with R1=10k, Rf=40k"
 * A3:
 *   Gain = 1 + Rf/R1 = 1 + 40k/10k = 1 + 4 = 5×
 *
 * Q4: "Why use op-amp instead of just a voltage divider?"
 * A4:
 *   - Voltage divider has high output impedance, can't drive loads
 *   - Op-amp buffer has low output impedance, can drive loads
 *   - Op-amp doesn't load the source
 *   - Can add gain if needed
 *
 * Q5: "Why does inverting amp have negative gain?"
 * A5:
 *   - Output is 180° out of phase with input
 *   - When input goes positive, output goes negative
 *   - Magnitude is Rf/Rin
 *
 * Q6: "What limits the maximum frequency an op-amp can amplify?"
 * A6:
 *   - Gain-Bandwidth Product (GBW): GBW = Gain × BW
 *   - Slew rate: max dV/dt output can achieve
 *   - Higher gain = lower bandwidth
 *
 * Q7: "How do you amplify a thermocouple signal for an ADC?"
 * A7:
 *   - Thermocouple outputs ~40µV/°C
 *   - 0-1000°C = 0-40mV
 *   - Need 0-3.3V for ADC
 *   - Gain needed = 3.3V / 0.04V = 82.5×
 *   - Use non-inverting amp: R1 = 10k, Rf = 815k (or close standard value)
 *   - Add low-pass filter to remove noise
 *
 * ============================================================================
 * COMMON OP-AMP ICs:
 *   LM358  - Dual, general purpose, single supply
 *   LM324  - Quad, general purpose
 *   OPA2134 - Audio, low noise
 *   MCP6001 - Low power, rail-to-rail, single supply
 *   OPA333  - Zero-drift, precision
 * ============================================================================
 */
