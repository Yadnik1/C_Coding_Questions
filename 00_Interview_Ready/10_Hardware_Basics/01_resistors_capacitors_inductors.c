/*
 * ============================================================================
 *        HARDWARE FUNDAMENTALS FOR EMBEDDED ENGINEERS
 *        PART 1: RESISTORS, CAPACITORS, INDUCTORS
 * ============================================================================
 *
 * You write firmware, but you MUST understand basic hardware.
 * These questions WILL come up in embedded interviews.
 *
 * ============================================================================
 *                           RESISTORS
 * ============================================================================
 *
 * WHAT DOES A RESISTOR DO?
 *   - Limits current flow
 *   - Creates voltage drops
 *   - Dissipates power as heat
 *
 * OHM'S LAW (MEMORIZE THIS!):
 *
 *   V = I × R
 *
 *   V = Voltage (Volts)
 *   I = Current (Amps)
 *   R = Resistance (Ohms)
 *
 *   Rearranged:
 *   I = V / R  (current through resistor)
 *   R = V / I  (resistance needed)
 *
 * SCHEMATIC SYMBOL:
 *
 *   ----/\/\/\----   (US style)
 *   ----[    ]----   (EU style)
 *
 * POWER DISSIPATION:
 *
 *   P = V × I = I²R = V²/R
 *
 *   IMPORTANT: Resistors have power ratings (1/4W, 1/2W, 1W)
 *   If you exceed it, resistor burns up!
 *
 * COMMON USES IN EMBEDDED:
 *
 *   1. CURRENT LIMITING (LED):
 *
 *      VCC (3.3V)
 *        |
 *        R = (VCC - Vf) / I
 *        |
 *       LED (Vf ≈ 2V, I = 10mA)
 *        |
 *       GND
 *
 *      R = (3.3 - 2.0) / 0.010 = 130Ω
 *      Use 150Ω (standard value)
 *
 *   2. PULL-UP / PULL-DOWN:
 *
 *      VCC                     VCC
 *       |                       |
 *      [R] 10kΩ (pull-up)      MCU_PIN----[R] 10kΩ (pull-down)
 *       |                                  |
 *      MCU_PIN---[BUTTON]---GND           GND
 *
 *      - Pull-up: Pin reads HIGH when button open
 *      - Pull-down: Pin reads LOW when button open
 *
 *   3. VOLTAGE DIVIDER (see next file)
 *
 * RESISTORS IN SERIES:
 *
 *   ---[R1]---[R2]---[R3]---
 *
 *   R_total = R1 + R2 + R3
 *
 * RESISTORS IN PARALLEL:
 *
 *      +---[R1]---+
 *   ---+          +---
 *      +---[R2]---+
 *
 *   1/R_total = 1/R1 + 1/R2
 *   R_total = (R1 × R2) / (R1 + R2)  (for 2 resistors)
 *
 * ============================================================================
 *                           CAPACITORS
 * ============================================================================
 *
 * WHAT DOES A CAPACITOR DO?
 *   - Stores energy in electric field
 *   - Blocks DC, passes AC
 *   - Smooths voltage (filtering)
 *   - Timing circuits (RC time constant)
 *
 * KEY PROPERTY: Capacitors RESIST change in VOLTAGE
 *   - Voltage across capacitor cannot change instantly
 *   - Current can change instantly
 *
 * SCHEMATIC SYMBOL:
 *
 *   ---| |---    (non-polarized)
 *   ---|(---     (polarized/electrolytic, + on left)
 *
 * UNITS:
 *   Farad (F) - usually µF (microfarad), nF, pF
 *   1 µF = 1000 nF = 1,000,000 pF
 *
 * CHARGE EQUATION:
 *   Q = C × V
 *   Q = charge (Coulombs)
 *   C = capacitance (Farads)
 *   V = voltage
 *
 * RC TIME CONSTANT:
 *   τ = R × C
 *   Time to charge/discharge to 63.2% of final value
 *   5τ = ~99% charged (considered "fully" charged)
 *
 * COMMON USES IN EMBEDDED:
 *
 *   1. DECOUPLING/BYPASS (MOST IMPORTANT!):
 *
 *      VCC ----+---- To MCU VCC pin
 *              |
 *             [C] 100nF (0.1µF)
 *              |
 *      GND ----+---- To MCU GND pin
 *
 *      - Place as CLOSE to MCU as possible
 *      - Filters high-frequency noise on power supply
 *      - Every IC needs decoupling caps!
 *      - Typical: 100nF ceramic + 10µF electrolytic
 *
 *   2. BULK CAPACITOR (Power smoothing):
 *
 *      After voltage regulator, 10-100µF electrolytic
 *      Smooths out current spikes from MCU
 *
 *   3. RC FILTER (Low-pass):
 *
 *      IN ---[R]---+--- OUT
 *                  |
 *                 [C]
 *                  |
 *                 GND
 *
 *      Cutoff frequency: fc = 1 / (2π × R × C)
 *      Passes low frequencies, blocks high frequencies
 *
 *   4. TIMING:
 *
 *      RC delay = R × C
 *      Example: 10kΩ × 1µF = 10ms time constant
 *
 * CAPACITORS IN SERIES:
 *   1/C_total = 1/C1 + 1/C2  (opposite of resistors!)
 *
 * CAPACITORS IN PARALLEL:
 *   C_total = C1 + C2  (opposite of resistors!)
 *
 * ============================================================================
 *                           INDUCTORS
 * ============================================================================
 *
 * WHAT DOES AN INDUCTOR DO?
 *   - Stores energy in magnetic field
 *   - Blocks AC, passes DC (opposite of capacitor!)
 *   - Used in switching power supplies
 *   - EMI filtering
 *
 * KEY PROPERTY: Inductors RESIST change in CURRENT
 *   - Current through inductor cannot change instantly
 *   - Voltage can change instantly
 *   - Opposite behavior to capacitor!
 *
 * SCHEMATIC SYMBOL:
 *
 *   ----^^^^----  or  ----UUUU----
 *
 * UNITS: Henry (H) - usually µH (microhenry), mH
 *
 * COMMON USES IN EMBEDDED:
 *
 *   1. SWITCHING REGULATORS (Buck/Boost):
 *      Stores energy during switch-on, releases during switch-off
 *      Part of DC-DC converter circuit
 *
 *   2. EMI FILTER (Ferrite bead):
 *      Blocks high-frequency noise
 *      Often on power input and signal lines
 *
 *   3. LC FILTER:
 *      Combined with capacitor for power filtering
 *
 * ============================================================================
 *                    COMPONENT COMPARISON
 * ============================================================================
 *
 *   Component  | Stores     | Resists change in | Blocks | Passes
 *   -----------|------------|-------------------|--------|--------
 *   Resistor   | Nothing    | N/A               | N/A    | N/A
 *   Capacitor  | E-field    | VOLTAGE           | DC     | AC
 *   Inductor   | B-field    | CURRENT           | AC     | DC
 *
 * ============================================================================
 */

#include <stdio.h>
#include <math.h>

// Calculate LED current limiting resistor
float calc_led_resistor(float vcc, float vf_led, float i_led) {
    return (vcc - vf_led) / i_led;
}

// Calculate power dissipation in resistor
float calc_power(float voltage, float current) {
    return voltage * current;
}

// Calculate RC time constant
float calc_rc_time_constant(float r_ohms, float c_farads) {
    return r_ohms * c_farads;
}

// Calculate low-pass filter cutoff frequency
float calc_lpf_cutoff(float r_ohms, float c_farads) {
    return 1.0 / (2.0 * M_PI * r_ohms * c_farads);
}

// Calculate parallel resistance
float calc_parallel_r(float r1, float r2) {
    return (r1 * r2) / (r1 + r2);
}

int main() {
    printf("=== HARDWARE CALCULATIONS ===\n\n");

    // LED resistor calculation
    printf("LED Current Limiting Resistor:\n");
    printf("  VCC = 3.3V, LED Vf = 2.0V, I = 10mA\n");
    float r_led = calc_led_resistor(3.3, 2.0, 0.010);
    printf("  R = %.0f Ω (use 150Ω standard)\n\n", r_led);

    // Power dissipation
    printf("Power Dissipation:\n");
    float v_drop = 3.3 - 2.0;  // Voltage across resistor
    float p = calc_power(v_drop, 0.010);
    printf("  P = %.1f mW (1/4W resistor is fine)\n\n", p * 1000);

    // RC time constant
    printf("RC Time Constant:\n");
    printf("  R = 10kΩ, C = 1µF\n");
    float tau = calc_rc_time_constant(10000, 1e-6);
    printf("  τ = %.3f seconds (10ms)\n", tau);
    printf("  5τ = %.3f seconds (full charge)\n\n", tau * 5);

    // Low-pass filter
    printf("RC Low-Pass Filter Cutoff:\n");
    printf("  R = 10kΩ, C = 100nF\n");
    float fc = calc_lpf_cutoff(10000, 100e-9);
    printf("  fc = %.1f Hz\n\n", fc);

    // Parallel resistors
    printf("Parallel Resistors:\n");
    printf("  R1 = 10kΩ, R2 = 10kΩ\n");
    float r_par = calc_parallel_r(10000, 10000);
    printf("  R_total = %.0f Ω\n", r_par);

    return 0;
}

/*
 * ============================================================================
 * INTERVIEW QUESTIONS & ANSWERS
 * ============================================================================
 *
 * Q1: "Why do we need decoupling capacitors?"
 * A1:
 *   - MCU draws current in short spikes (during clock edges)
 *   - Long PCB traces have inductance, can't supply current fast enough
 *   - Decoupling cap provides local charge reservoir
 *   - Filters high-frequency noise on power rail
 *   - Place 100nF ceramic as close to VCC/GND pins as possible
 *
 * Q2: "What value resistor for an LED?"
 * A2:
 *   R = (VCC - Vf) / I
 *   - VCC = supply voltage (3.3V or 5V)
 *   - Vf = LED forward voltage (~2V for red, ~3V for blue/white)
 *   - I = desired current (typically 5-20mA)
 *   Example: (3.3V - 2V) / 10mA = 130Ω, use 150Ω
 *
 * Q3: "Pull-up vs pull-down resistor - when to use?"
 * A3:
 *   Pull-up: Default HIGH, goes LOW when button pressed
 *     - Used with active-low buttons/signals
 *     - MCU internal pull-ups often available (weak, ~50kΩ)
 *   Pull-down: Default LOW, goes HIGH when button pressed
 *     - Used with active-high signals
 *   Typical value: 4.7kΩ to 10kΩ
 *
 * Q4: "What's the difference between capacitor and inductor behavior?"
 * A4:
 *   Capacitor: Resists voltage change, blocks DC, passes AC
 *   Inductor: Resists current change, blocks AC, passes DC
 *   Remember: "Capacitor is to Voltage as Inductor is to Current"
 *
 * Q5: "Calculate current through a 1kΩ resistor with 5V across it"
 * A5:
 *   I = V / R = 5V / 1000Ω = 5mA = 0.005A
 *
 * Q6: "What happens if decoupling cap is too far from IC?"
 * A6:
 *   - PCB trace inductance defeats the purpose
 *   - High-frequency noise not filtered effectively
 *   - IC may have erratic behavior, resets, data corruption
 *   - Rule: capacitor should be within 3mm of power pins
 *
 * ============================================================================
 * QUICK FORMULAS:
 *   V = I × R             (Ohm's law)
 *   P = V × I = I²R       (Power)
 *   τ = R × C             (RC time constant)
 *   fc = 1/(2πRC)         (RC filter cutoff)
 *   R_parallel = (R1×R2)/(R1+R2)
 * ============================================================================
 */
