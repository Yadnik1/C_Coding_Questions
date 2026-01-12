/*
 * ============================================================================
 *              POWER SUPPLIES & REGULATORS FOR EMBEDDED
 * ============================================================================
 *
 * Every embedded system needs power. Understanding power supply basics
 * helps you debug problems and design reliable systems.
 *
 * ============================================================================
 *                    LINEAR VOLTAGE REGULATORS
 * ============================================================================
 *
 * HOW IT WORKS:
 *   - Acts like a variable resistor
 *   - Drops excess voltage as heat
 *   - Simple, low noise, but inefficient
 *
 * SCHEMATIC:
 *
 *   VIN ----+----[7805]----+---- VOUT (5V)
 *           |              |
 *          [C1]           [C2]
 *          10µF           10µF
 *           |              |
 *          GND------------GND
 *
 *   C1: Input capacitor (stabilizes input)
 *   C2: Output capacitor (stabilizes output)
 *
 * COMMON LINEAR REGULATORS:
 *
 *   78xx Series (fixed output):
 *   - 7805: 5V output
 *   - 7833: 3.3V output
 *   - Dropout: ~2V (need VIN > VOUT + 2V)
 *
 *   LDO (Low Dropout):
 *   - AMS1117-3.3: 3.3V, ~1V dropout
 *   - LM1117: Similar
 *   - Can work with smaller VIN-VOUT difference
 *
 * POWER DISSIPATION (THE BIG PROBLEM!):
 *
 *   P = (VIN - VOUT) × IOUT
 *
 *   Example: 12V to 5V at 500mA
 *   P = (12 - 5) × 0.5 = 3.5W  (needs heatsink!)
 *
 *   EFFICIENCY = VOUT / VIN = 5/12 = 42% (terrible!)
 *
 * WHEN TO USE LINEAR:
 *   - Low current (<100mA)
 *   - Small VIN-VOUT difference
 *   - Need low noise (audio, analog sensors)
 *   - Simple design
 *
 * ============================================================================
 *                    SWITCHING REGULATORS (DC-DC)
 * ============================================================================
 *
 * HOW IT WORKS:
 *   - Rapidly switches power on/off
 *   - Uses inductor to store energy
 *   - Much more efficient (80-95%)
 *   - More complex, some noise
 *
 * TYPES:
 *
 *   BUCK (Step-Down): VIN > VOUT
 *     12V → 5V, 24V → 3.3V
 *     Most common for embedded
 *
 *   BOOST (Step-Up): VIN < VOUT
 *     3.7V → 5V (battery to USB)
 *     Single cell Li-ion to 5V
 *
 *   BUCK-BOOST: VIN can be above or below VOUT
 *     Battery that varies 3.0V-4.2V → stable 3.3V
 *
 * BASIC BUCK CONVERTER:
 *
 *   VIN ---[SW]---+---[L]---+--- VOUT
 *                 |         |
 *                [D]       [C]
 *                 |         |
 *                GND-------GND
 *
 *   SW: MOSFET switch (controlled by PWM)
 *   D: Schottky diode (freewheeling)
 *   L: Inductor (energy storage)
 *   C: Output capacitor (smoothing)
 *
 * COMMON BUCK CONVERTER MODULES:
 *   - MP1584: Adjustable, up to 3A
 *   - LM2596: Adjustable, 3A
 *   - TPS563200: Fixed 3.3V, 3A
 *   - LMR14206: Tiny, 0.6A
 *
 * EFFICIENCY COMPARISON:
 *
 *   12V to 5V at 500mA:
 *   Linear (7805): P_loss = 3.5W, Efficiency = 42%
 *   Buck (MP1584): P_loss = 0.3W, Efficiency = 90%
 *
 * ============================================================================
 *                    PRACTICAL CONSIDERATIONS
 * ============================================================================
 *
 * 1. DECOUPLING CAPACITORS:
 *    Always add caps close to regulator:
 *    - Input: 10-100µF electrolytic + 100nF ceramic
 *    - Output: 10-100µF electrolytic + 100nF ceramic
 *
 * 2. THERMAL DESIGN:
 *    Linear regulator heat: P = (VIN-VOUT) × I
 *    If P > 1W, need heatsink or switch to DC-DC
 *
 * 3. NOISE:
 *    - Linear: Very low noise, good for analog
 *    - Switching: Has ripple at switching frequency
 *    - May need LC filter after switcher for sensitive circuits
 *
 * 4. DROPOUT VOLTAGE:
 *    - Standard regulator: ~2V
 *    - LDO: 0.3V - 1V
 *    - VIN must be > VOUT + dropout!
 *
 * 5. LOAD TRANSIENT RESPONSE:
 *    - How fast regulator responds to sudden load change
 *    - Need sufficient output capacitance
 *
 * ============================================================================
 *                    BATTERY POWER SYSTEMS
 * ============================================================================
 *
 * SINGLE CELL LI-ION (3.0V - 4.2V):
 *
 *   Option 1: 3.3V LDO (AMS1117-3.3)
 *     Simple but battery must stay > 3.6V for regulation
 *
 *   Option 2: Buck-Boost to 3.3V
 *     Works across full battery range
 *     More complex but better utilization
 *
 * USB POWERED (5V):
 *
 *   USB 5V ---[LDO]--- 3.3V MCU
 *
 *   Low current: LDO fine
 *   High current: Use buck converter
 *
 * MULTIPLE RAILS:
 *
 *   VIN (12V)
 *      |
 *      +---[Buck]--- 5V (motors, peripherals)
 *      |
 *      +---[Buck]--- 3.3V (MCU, sensors)
 *      |
 *      +---[LDO]---- 3.3V_ANALOG (clean power for ADC)
 *
 * ============================================================================
 */

#include <stdio.h>

// Linear regulator power dissipation
float linear_power_loss(float vin, float vout, float iout) {
    return (vin - vout) * iout;
}

// Linear regulator efficiency
float linear_efficiency(float vin, float vout) {
    return (vout / vin) * 100.0;
}

// Switching regulator power loss (approximate)
float switching_power_loss(float pout, float efficiency_percent) {
    float efficiency = efficiency_percent / 100.0;
    float pin = pout / efficiency;
    return pin - pout;
}

int main() {
    printf("=== POWER SUPPLY CALCULATIONS ===\n\n");

    // Linear regulator
    printf("Linear Regulator: 12V to 5V at 500mA\n");
    float p_linear = linear_power_loss(12, 5, 0.5);
    float eff_linear = linear_efficiency(12, 5);
    printf("  Power loss = %.2f W\n", p_linear);
    printf("  Efficiency = %.0f%%\n", eff_linear);
    printf("  %s\n\n", p_linear > 1 ? "NEEDS HEATSINK!" : "OK without heatsink");

    // Same with LDO (lower dropout)
    printf("LDO: 5V to 3.3V at 300mA\n");
    float p_ldo = linear_power_loss(5, 3.3, 0.3);
    float eff_ldo = linear_efficiency(5, 3.3);
    printf("  Power loss = %.2f W\n", p_ldo);
    printf("  Efficiency = %.0f%%\n\n", eff_ldo);

    // Switching regulator
    printf("Buck Converter: 12V to 5V at 500mA (90%% efficient)\n");
    float pout = 5 * 0.5;
    float p_buck = switching_power_loss(pout, 90);
    printf("  Output power = %.2f W\n", pout);
    printf("  Power loss = %.2f W\n", p_buck);
    printf("  Efficiency = 90%%\n\n");

    // Battery example
    printf("Battery System: Li-ion (3.7V nom) to 3.3V at 100mA\n");
    float p_batt = linear_power_loss(3.7, 3.3, 0.1);
    printf("  LDO power loss = %.2f W (acceptable)\n", p_batt);

    return 0;
}

/*
 * ============================================================================
 * INTERVIEW QUESTIONS & ANSWERS
 * ============================================================================
 *
 * Q1: "Linear vs switching regulator - when to use each?"
 * A1:
 *   Linear:
 *   - Low current (<100mA)
 *   - Small voltage drop (VIN close to VOUT)
 *   - Need low noise (analog, audio)
 *   - Simple design, fewer components
 *
 *   Switching:
 *   - High current
 *   - Large voltage drop (efficiency matters)
 *   - Battery powered (maximize runtime)
 *   - Heat is a concern
 *
 * Q2: "Your 5V regulator gets very hot. What's wrong?"
 * A2:
 *   P = (VIN - VOUT) × I
 *   Check:
 *   1. What's VIN? (12V input = more heat)
 *   2. What's the current draw?
 *   3. Calculate power: If > 1W, need heatsink or use DC-DC
 *   Solution: Switch to buck converter or reduce current
 *
 * Q3: "Design a power supply for 3.3V MCU from 12V input"
 * A3:
 *   If I < 100mA: LDO (AMS1117-3.3) is fine
 *     But P = (12-3.3) × 0.1 = 0.87W (warm)
 *
 *   If I > 200mA: Use buck converter
 *     MP1584 or similar
 *     90% efficient, much less heat
 *
 *   Don't forget: Input cap, output cap, EMI considerations
 *
 * Q4: "What is dropout voltage?"
 * A4:
 *   Minimum difference between VIN and VOUT for regulation
 *   - Standard 78xx: ~2V (need 7V in for 5V out)
 *   - LDO: 0.3V - 1V (can get 3.3V from 3.6V)
 *   If VIN drops below VOUT + dropout, output falls!
 *
 * Q5: "Why does my MCU reset when motor turns on?"
 * A5:
 *   Motor draws large current spike at startup
 *   This pulls down the supply voltage
 *   If voltage drops below MCU minimum, it resets
 *
 *   Solutions:
 *   1. Add large capacitor on power rail (bulk capacitance)
 *   2. Separate power supplies for motor and MCU
 *   3. Soft-start the motor (PWM ramp up)
 *   4. Use LC filter between motor supply and MCU
 *
 * Q6: "How do you power a 3.3V system from single Li-ion cell?"
 * A6:
 *   Li-ion: 3.0V (empty) to 4.2V (full)
 *
 *   Option 1: LDO
 *     Works only when battery > 3.5V or so
 *     Simple but doesn't use full capacity
 *
 *   Option 2: Buck-boost converter
 *     Works across full range
 *     Better battery utilization
 *     More complex
 *
 * Q7: "What capacitors do you need for a voltage regulator?"
 * A7:
 *   Input side: 10-100µF electrolytic + 100nF ceramic
 *     - Provides charge during transients
 *     - Ceramic for high-frequency decoupling
 *
 *   Output side: 10-100µF electrolytic + 100nF ceramic
 *     - Stabilizes output
 *     - Handles load transients
 *
 *   Place capacitors as close to regulator as possible!
 *
 * ============================================================================
 * COMMON REGULATORS:
 *   Linear: 7805, 7833, AMS1117-3.3, LM1117
 *   Buck: LM2596, MP1584, TPS563200
 *   Boost: MT3608
 *   Buck-Boost: TPS63000
 * ============================================================================
 */
