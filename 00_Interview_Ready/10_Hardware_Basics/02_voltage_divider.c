/*
 * ============================================================================
 *                    VOLTAGE DIVIDER - COMPLETE GUIDE
 * ============================================================================
 *
 * THE most asked hardware question in embedded interviews!
 * If you remember ONE circuit, remember this one.
 *
 * ============================================================================
 *                         THE CIRCUIT
 * ============================================================================
 *
 *        VIN
 *         |
 *        [R1]
 *         |
 *         +------ VOUT
 *         |
 *        [R2]
 *         |
 *        GND
 *
 * ============================================================================
 *                         THE FORMULA
 * ============================================================================
 *
 *              R2
 *   VOUT = --------- × VIN
 *           R1 + R2
 *
 *   Or equivalently:
 *
 *   VOUT     R2
 *   ---- = -------
 *   VIN    R1 + R2
 *
 * ============================================================================
 *                         WHY IT WORKS
 * ============================================================================
 *
 * Current flows through R1 and R2 in series:
 *
 *   I = VIN / (R1 + R2)
 *
 * Voltage across R2:
 *
 *   VOUT = I × R2 = VIN × R2 / (R1 + R2)
 *
 * ============================================================================
 *                    COMMON USE CASES
 * ============================================================================
 *
 * USE CASE 1: LEVEL SHIFTING (5V sensor → 3.3V MCU)
 * ------------------------------------------------
 *
 *   5V Sensor Output
 *         |
 *        [R1] 10kΩ
 *         |
 *         +------ To MCU ADC (3.3V max!)
 *         |
 *        [R2] 20kΩ
 *         |
 *        GND
 *
 *   VOUT = 5V × 20k / (10k + 20k) = 5V × 20/30 = 3.33V ✓
 *
 *   WARNING: This only works for HIGH IMPEDANCE inputs (like ADC)
 *   If load draws current, voltage will drop!
 *
 * USE CASE 2: READING BATTERY VOLTAGE
 * ------------------------------------
 *
 *   Battery (0-12V)
 *         |
 *        [R1] 30kΩ
 *         |
 *         +------ To ADC (0-3.3V range)
 *         |
 *        [R2] 10kΩ
 *         |
 *        GND
 *
 *   At 12V: VOUT = 12V × 10k / (30k + 10k) = 3.0V ✓
 *   At 0V:  VOUT = 0V
 *
 *   ADC reading → multiply by 4 to get actual battery voltage
 *
 * USE CASE 3: REFERENCE VOLTAGE
 * ------------------------------
 *
 *   Create a fixed voltage from supply
 *
 *   5V
 *    |
 *   [R1] 10kΩ
 *    |
 *    +------ 2.5V reference
 *    |
 *   [R2] 10kΩ
 *    |
 *   GND
 *
 *   Equal resistors = half the voltage
 *
 * ============================================================================
 *                    DESIGN CONSIDERATIONS
 * ============================================================================
 *
 * 1. LOADING EFFECT:
 *    - If you connect a load to VOUT, it's like another resistor in parallel with R2
 *    - This changes the output voltage!
 *    - Solution: Use high impedance loads, or use a buffer (op-amp)
 *
 *         VIN
 *          |
 *         [R1]
 *          |
 *          +------ VOUT (measured)
 *          |
 *         [R2]    [R_load] ← If R_load is low, it pulls VOUT down!
 *          |         |
 *         GND-------GND
 *
 *    Rule of thumb: R_load should be > 10× (R1 || R2)
 *
 * 2. POWER DISSIPATION:
 *    - Current flows continuously through divider
 *    - I = VIN / (R1 + R2)
 *    - Higher resistors = less current = less wasted power
 *    - For battery-powered: use 100kΩ+ resistors
 *
 * 3. NOISE:
 *    - Higher resistors = more thermal noise
 *    - Add small capacitor across R2 for filtering
 *
 * ============================================================================
 *                    COMMON RATIOS
 * ============================================================================
 *
 *   R1 : R2    | VOUT/VIN  | Use case
 *   -----------|-----------|------------------
 *   1 : 1      | 0.5       | Half voltage
 *   1 : 2      | 0.667     | 5V → 3.3V (approx)
 *   2 : 1      | 0.333     | 5V → 1.67V
 *   3 : 1      | 0.25      | 12V → 3V
 *   9 : 1      | 0.1       | 33V → 3.3V
 *
 * ============================================================================
 */

#include <stdio.h>

// Calculate voltage divider output
float voltage_divider(float vin, float r1, float r2) {
    return vin * r2 / (r1 + r2);
}

// Calculate R1 given VIN, VOUT, and R2
float calc_r1(float vin, float vout, float r2) {
    // VOUT = VIN × R2 / (R1 + R2)
    // VOUT × (R1 + R2) = VIN × R2
    // VOUT × R1 + VOUT × R2 = VIN × R2
    // VOUT × R1 = VIN × R2 - VOUT × R2
    // R1 = R2 × (VIN - VOUT) / VOUT
    return r2 * (vin - vout) / vout;
}

// Calculate current through divider
float calc_divider_current(float vin, float r1, float r2) {
    return vin / (r1 + r2);
}

// Calculate power wasted in divider
float calc_divider_power(float vin, float r1, float r2) {
    float current = vin / (r1 + r2);
    return vin * current;  // P = V × I
}

int main() {
    printf("=== VOLTAGE DIVIDER CALCULATIONS ===\n\n");

    // Example 1: 5V to 3.3V level shifting
    printf("Example 1: 5V → 3.3V Level Shifting\n");
    printf("  R1 = 10kΩ, R2 = 20kΩ\n");
    float vout1 = voltage_divider(5.0, 10000, 20000);
    printf("  VOUT = %.2fV\n\n", vout1);

    // Example 2: 12V battery monitoring
    printf("Example 2: 12V Battery Monitoring (ADC 0-3.3V)\n");
    printf("  R1 = 30kΩ, R2 = 10kΩ\n");
    float vout2 = voltage_divider(12.0, 30000, 10000);
    printf("  VOUT = %.2fV (at full battery)\n", vout2);
    float vout2_low = voltage_divider(9.0, 30000, 10000);
    printf("  VOUT = %.2fV (at 9V low battery)\n\n", vout2_low);

    // Example 3: Calculate R1 needed
    printf("Example 3: Design 5V → 2.5V divider with R2 = 10kΩ\n");
    float r1_needed = calc_r1(5.0, 2.5, 10000);
    printf("  R1 needed = %.0fΩ (use 10kΩ)\n\n", r1_needed);

    // Example 4: Power consumption
    printf("Example 4: Power Wasted in Divider\n");
    printf("  12V input, R1 = 30kΩ, R2 = 10kΩ\n");
    float current = calc_divider_current(12.0, 30000, 10000);
    float power = calc_divider_power(12.0, 30000, 10000);
    printf("  Current = %.2f mA\n", current * 1000);
    printf("  Power wasted = %.2f mW\n\n", power * 1000);

    // High-resistance version for battery saving
    printf("Example 5: Low-Power Version (10× higher R)\n");
    printf("  12V input, R1 = 300kΩ, R2 = 100kΩ\n");
    float current_lp = calc_divider_current(12.0, 300000, 100000);
    float power_lp = calc_divider_power(12.0, 300000, 100000);
    printf("  Current = %.3f mA (30µA)\n", current_lp * 1000);
    printf("  Power wasted = %.3f mW\n", power_lp * 1000);

    return 0;
}

/*
 * ============================================================================
 * INTERVIEW QUESTIONS & ANSWERS
 * ============================================================================
 *
 * Q1: "Design a voltage divider to convert 5V to 3.3V"
 * A1:
 *   VOUT/VIN = R2/(R1+R2) = 3.3/5 = 0.66
 *   If R2 = 20kΩ, then R1 = R2 × (VIN-VOUT)/VOUT = 20k × 1.7/3.3 ≈ 10kΩ
 *   Verify: 5V × 20k/(10k+20k) = 3.33V ✓
 *
 * Q2: "Why can't you use a voltage divider to power a device?"
 * A2:
 *   - Voltage divider has high output impedance
 *   - When load draws current, voltage drops significantly
 *   - No regulation - output varies with load
 *   - Wastes power even with no load
 *   - Use a voltage regulator instead!
 *
 * Q3: "How do you measure a 0-24V signal with a 3.3V ADC?"
 * A3:
 *   Need VOUT = 3.3V when VIN = 24V
 *   Ratio = 3.3/24 = 0.1375
 *   R2/(R1+R2) = 0.1375
 *   If R2 = 10kΩ: R1 = R2 × (1/0.1375 - 1) = 10k × 6.27 ≈ 62kΩ
 *   Use R1 = 68kΩ (standard), gives 3.08V at 24V input
 *
 * Q4: "What's the problem with using 1MΩ resistors in a divider?"
 * A4:
 *   - Very high impedance output
 *   - Susceptible to noise pickup
 *   - ADC input impedance may load the divider
 *   - Slow response due to stray capacitance
 *   - Better: 10kΩ-100kΩ range, add filter capacitor
 *
 * Q5: "Draw a circuit to monitor battery voltage with an MCU"
 * A5:
 *
 *   Battery+  ----[R1 30k]----+----[R2 10k]---- GND
 *                             |
 *                            [C 100nF] (optional filter)
 *                             |
 *                          MCU ADC Pin
 *
 *   - Ratio 3:1, so 12V → 3V (safe for 3.3V ADC)
 *   - Calculate: V_battery = ADC_reading × (R1+R2)/R2 × VREF/ADC_MAX
 *
 * Q6: "Why add a capacitor across R2?"
 * A6:
 *   - Forms low-pass filter with divider resistance
 *   - Filters out high-frequency noise
 *   - Stabilizes ADC readings
 *   - Typical: 100nF, cutoff = 1/(2π × Rparallel × C)
 *
 * ============================================================================
 * ASCII CIRCUIT REFERENCE:
 *
 *   Basic Divider:          With Filter Cap:
 *
 *        VIN                     VIN
 *         |                       |
 *        [R1]                    [R1]
 *         |                       |
 *         +--- VOUT               +---+--- VOUT
 *         |                       |   |
 *        [R2]                    [R2][C]
 *         |                       |   |
 *        GND                     GND-GND
 *
 * ============================================================================
 */
