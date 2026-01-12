/*
 * ============================================================================
 *               TRANSISTORS & MOSFETs FOR EMBEDDED ENGINEERS
 * ============================================================================
 *
 * As a firmware engineer, you'll use transistors/MOSFETs as SWITCHES.
 * This covers practical embedded use, not analog amplifier design.
 *
 * ============================================================================
 *                    BJT (Bipolar Junction Transistor)
 * ============================================================================
 *
 * TWO TYPES:
 *
 *   NPN:                    PNP:
 *
 *      C (collector)          C (collector)
 *      |                      |
 *      |/                     |\
 *   B--<                   B--<
 *      |\                     |/
 *      v                      v
 *      E (emitter)            E (emitter)
 *
 *   NPN: Current flows C→E when B is HIGH
 *   PNP: Current flows E→C when B is LOW
 *
 * BJT AS A SWITCH (NPN - most common):
 *
 *        VCC
 *         |
 *       [LOAD] (relay, LED, motor)
 *         |
 *         C
 *          \
 *   MCU ---[Rb]----B  (NPN transistor)
 *          /
 *         E
 *         |
 *        GND
 *
 *   MCU HIGH → Transistor ON → Current flows through load
 *   MCU LOW  → Transistor OFF → No current
 *
 * BASE RESISTOR CALCULATION:
 *
 *   Rb = (V_MCU - Vbe) / Ib
 *
 *   Vbe ≈ 0.7V (base-emitter drop)
 *   Ib = Ic / β  (β = current gain, typically 100-300)
 *
 *   Example: Drive 100mA load, β = 100
 *   Ib_min = 100mA / 100 = 1mA
 *   For saturation, use 2-3× → Ib = 3mA
 *   Rb = (3.3V - 0.7V) / 3mA = 867Ω → use 820Ω or 1kΩ
 *
 * ============================================================================
 *                    MOSFET (Metal Oxide Semiconductor FET)
 * ============================================================================
 *
 * MOSFETs are PREFERRED for embedded switching:
 *   - Voltage controlled (no base current needed!)
 *   - Very low on-resistance
 *   - Fast switching
 *   - Easy to drive from MCU
 *
 * TWO TYPES:
 *
 *   N-Channel:              P-Channel:
 *
 *      D (drain)              D (drain)
 *      |                      |
 *    |=|                    |=|
 *   G-|                    G-|  (arrow points in for P)
 *    |=|                    |=|
 *      |                      |
 *      S (source)             S (source)
 *
 * N-CHANNEL AS LOW-SIDE SWITCH (MOST COMMON):
 *
 *        VCC
 *         |
 *       [LOAD]
 *         |
 *         D
 *         |
 *   MCU --G  (N-channel MOSFET)
 *         |
 *         S
 *         |
 *        GND
 *
 *   MCU HIGH (> Vth) → MOSFET ON → Load powered
 *   MCU LOW          → MOSFET OFF → Load off
 *
 * P-CHANNEL AS HIGH-SIDE SWITCH:
 *
 *        VCC
 *         |
 *         S
 *         |
 *   MCU --G  (P-channel MOSFET, often needs level shift!)
 *         |
 *         D
 *         |
 *       [LOAD]
 *         |
 *        GND
 *
 *   MCU LOW  → MOSFET ON (Vgs < -Vth)
 *   MCU HIGH → MOSFET OFF
 *
 * KEY MOSFET PARAMETERS:
 *
 *   Vth (threshold voltage): Gate voltage to turn on (typically 1-3V)
 *   Vgs(max): Maximum gate-source voltage (don't exceed!)
 *   Rds(on): On-resistance when fully on (want LOW)
 *   Id(max): Maximum drain current
 *
 * LOGIC-LEVEL MOSFET:
 *   Vth < 2.5V, fully on with 3.3V gate
 *   Example: IRLZ44N, Si2302
 *
 * GATE RESISTOR (optional but recommended):
 *
 *   MCU ---[Rg 100Ω]--- G
 *
 *   - Limits current spike when charging gate capacitance
 *   - Reduces EMI
 *   - Typical: 10Ω to 1kΩ
 *
 * ============================================================================
 *                    WHICH TO USE? BJT vs MOSFET
 * ============================================================================
 *
 *   BJT:
 *   + Cheaper
 *   + Works at lower voltages
 *   - Needs base current (wastes MCU current)
 *   - Slower switching
 *
 *   MOSFET:
 *   + Voltage controlled (no gate current in DC)
 *   + Very low Rds(on) possible
 *   + Fast switching
 *   + Better for high current loads
 *   - Need logic-level type for 3.3V drive
 *
 *   RECOMMENDATION: Use N-channel logic-level MOSFET for new designs
 *
 * ============================================================================
 *                    PRACTICAL CIRCUITS
 * ============================================================================
 *
 * 1. DRIVE A RELAY (with flyback diode):
 *
 *        VCC
 *         |
 *        [Relay Coil]
 *         |-------|
 *         |      -|+
 *         |     [D1] (flyback diode, 1N4148)
 *         |       |
 *         D       |
 *         |-------+
 *   MCU --G  (MOSFET)
 *         |
 *         S
 *         |
 *        GND
 *
 *   Flyback diode REQUIRED! Protects MOSFET from voltage spike
 *   when relay turns off (back-EMF from inductor)
 *
 * 2. DRIVE A DC MOTOR:
 *
 *   Same as relay - needs flyback diode
 *   For bidirectional: use H-bridge
 *
 * 3. LED STRIP (high current):
 *
 *        12V
 *         |
 *       [LED Strip]
 *         |
 *         D
 *         |
 *   MCU --G  (logic-level N-MOSFET, rated for strip current)
 *         |
 *         S
 *         |
 *        GND
 *
 * 4. LEVEL SHIFTING (3.3V MCU to 5V logic):
 *
 *        5V
 *         |
 *        [Rpu] 10kΩ pull-up
 *         |
 *         D------------- 5V Output
 *         |
 *   3.3V--G  (N-MOSFET, e.g., BSS138)
 *         |
 *         S
 *         |
 *        GND
 *
 *   3.3V HIGH → MOSFET ON → Output pulled LOW
 *   3.3V LOW  → MOSFET OFF → Output pulled HIGH by Rpu
 *   (Logic is inverted! Good for I2C, open-drain)
 *
 * ============================================================================
 */

#include <stdio.h>

// Calculate BJT base resistor
float calc_base_resistor(float v_mcu, float v_be, float i_load, float beta, float safety) {
    float ib = (i_load / beta) * safety;  // Extra margin for saturation
    return (v_mcu - v_be) / ib;
}

// Calculate power dissipation in MOSFET
float calc_mosfet_power(float i_load, float rds_on) {
    return i_load * i_load * rds_on;  // P = I²R
}

int main() {
    printf("=== TRANSISTOR CALCULATIONS ===\n\n");

    // BJT base resistor
    printf("BJT Base Resistor (NPN):\n");
    printf("  V_MCU = 3.3V, V_be = 0.7V\n");
    printf("  I_load = 100mA, β = 100, safety = 3×\n");
    float rb = calc_base_resistor(3.3, 0.7, 0.1, 100, 3);
    printf("  Rb = %.0f Ω (use 820Ω or 1kΩ)\n\n", rb);

    // MOSFET power dissipation
    printf("MOSFET Power Dissipation:\n");
    printf("  I_load = 2A, Rds(on) = 0.04Ω\n");
    float p_mosfet = calc_mosfet_power(2.0, 0.04);
    printf("  P = %.2f W (need heatsink if > 1W)\n\n", p_mosfet);

    // MOSFET for LED strip
    printf("Example: 5A LED Strip\n");
    printf("  Rds(on) = 0.02Ω (IRLZ44N)\n");
    float p_strip = calc_mosfet_power(5.0, 0.02);
    printf("  Power in MOSFET = %.2f W\n", p_strip);

    return 0;
}

/*
 * ============================================================================
 * INTERVIEW QUESTIONS & ANSWERS
 * ============================================================================
 *
 * Q1: "How do you switch a 12V relay from a 3.3V MCU?"
 * A1:
 *   - Use N-channel logic-level MOSFET (Vth < 2V)
 *   - MOSFET drain connects to relay, source to GND
 *   - MCU GPIO drives gate directly (add 100Ω series resistor)
 *   - ADD FLYBACK DIODE across relay coil! (cathode to +12V)
 *   - Pull-down resistor on gate (10kΩ) for defined off-state at boot
 *
 * Q2: "Why do you need a flyback diode?"
 * A2:
 *   - Relay/motor coils are inductors
 *   - Inductors resist current change
 *   - When switched off, inductor creates voltage spike (back-EMF)
 *   - Spike can be hundreds of volts, destroys MOSFET
 *   - Flyback diode provides path for current to dissipate safely
 *
 * Q3: "What's the difference between BJT and MOSFET?"
 * A3:
 *   BJT: Current controlled (needs base current to stay on)
 *   MOSFET: Voltage controlled (no gate current in steady state)
 *   MOSFET preferred for most switching applications
 *
 * Q4: "Why use logic-level MOSFET?"
 * A4:
 *   - Standard MOSFETs need 10V on gate to fully turn on
 *   - 3.3V MCU can't drive them properly
 *   - Logic-level MOSFETs fully turn on with 3.3V gate voltage
 *   - Look for Vth < 2V in datasheet
 *
 * Q5: "Your MOSFET gets hot when switching 3A. Why?"
 * A5:
 *   Possible causes:
 *   1. Rds(on) too high - choose MOSFET with lower Rds(on)
 *   2. Not fully turned on - check Vgs is above Vth
 *   3. Switching losses at high frequency
 *   4. Gate not driven hard enough
 *   Solution: P = I²R, reduce R or add heatsink
 *
 * Q6: "Draw a circuit to control a motor from an MCU"
 * A6:
 *
 *   VCC (motor voltage)
 *    |
 *   [Motor]
 *    |-------|
 *    |      -|+
 *    |     [D1] (flyback diode)
 *    |       |
 *    D       |
 *    |-------+
 *   [Rg]-G  (N-MOSFET)
 *    |
 *    S
 *    |
 *   GND
 *
 *   MCU ---> Rg (100Ω) ---> Gate
 *
 * Q7: "Low-side vs high-side switch - when to use each?"
 * A7:
 *   Low-side (N-channel between load and GND):
 *   - Easier to drive from MCU
 *   - Load's negative side is switched
 *   - Ground reference changes during switch
 *
 *   High-side (P-channel between VCC and load):
 *   - Load's positive side is switched
 *   - Ground stays constant
 *   - Harder to drive from low-voltage MCU (needs level shift)
 *   - Better for some load types
 *
 * ============================================================================
 * COMMON PARTS:
 *   BJT NPN: 2N2222, BC547
 *   BJT PNP: 2N2907, BC557
 *   MOSFET N-ch logic: IRLZ44N, Si2302, BSS138, AO3400
 *   MOSFET P-ch: Si2301, AO3401
 * ============================================================================
 */
