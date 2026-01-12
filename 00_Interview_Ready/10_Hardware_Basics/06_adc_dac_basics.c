/*
 * ============================================================================
 *                    ADC & DAC FOR EMBEDDED ENGINEERS
 * ============================================================================
 *
 * ADC: Convert analog voltage to digital number
 * DAC: Convert digital number to analog voltage
 *
 * These are how your MCU interfaces with the analog world.
 *
 * ============================================================================
 *                         ADC BASICS
 * ============================================================================
 *
 * WHAT IT DOES:
 *
 *   Analog Voltage (0V to VREF) → Digital Value (0 to 2^N - 1)
 *
 *   Example: 12-bit ADC, VREF = 3.3V
 *   - 0V    → 0
 *   - 1.65V → 2048 (half scale)
 *   - 3.3V  → 4095 (max)
 *
 * KEY FORMULA:
 *
 *   ADC_Value = (Vin / Vref) × (2^N - 1)
 *
 *   Rearranged to get voltage:
 *   Vin = (ADC_Value / (2^N - 1)) × Vref
 *
 *   Or commonly:
 *   Vin = ADC_Value × (Vref / 4095)  (for 12-bit)
 *
 * RESOLUTION:
 *
 *   Smallest voltage change ADC can detect:
 *   LSB = Vref / 2^N
 *
 *   Example: 12-bit, 3.3V reference
 *   LSB = 3.3V / 4096 = 0.8mV
 *
 *   10-bit: LSB = 3.3V / 1024 = 3.2mV
 *   12-bit: LSB = 3.3V / 4096 = 0.8mV
 *   16-bit: LSB = 3.3V / 65536 = 50µV
 *
 * COMMON ADC RESOLUTIONS:
 *
 *   8-bit:  256 levels (old, cheap MCUs)
 *   10-bit: 1024 levels (Arduino)
 *   12-bit: 4096 levels (most modern MCUs)
 *   16-bit: 65536 levels (precision applications)
 *
 * SAMPLING RATE:
 *   How fast ADC converts (samples per second)
 *   ESP32: ~100 kSPS
 *   STM32: Up to 5.3 MSPS (fast ADC)
 *
 *   Remember Nyquist: Sample at > 2× highest frequency in signal
 *
 * ============================================================================
 *                    ADC CONFIGURATION
 * ============================================================================
 *
 * REFERENCE VOLTAGE (VREF):
 *
 *   1. VCC as reference: Simple but noisy
 *      - VREF = 3.3V (or VCC)
 *      - If VCC drops, readings change!
 *
 *   2. Internal reference: More stable
 *      - Built-in 1.1V or 2.5V reference
 *      - Limits input range
 *
 *   3. External reference: Best accuracy
 *      - Dedicated voltage reference IC
 *      - Use for precision applications
 *
 * INPUT IMPEDANCE:
 *
 *   ADC has input capacitor that needs to charge
 *   High source impedance = slow charging = errors
 *
 *   Rule: Source impedance < 10kΩ (check datasheet)
 *   Solution: Buffer with op-amp for high-Z sources
 *
 * ============================================================================
 *                    PRACTICAL ADC EXAMPLE
 * ============================================================================
 *
 * Read battery voltage (0-12V) with 3.3V ADC:
 *
 *   Battery+
 *      |
 *     [R1] 30kΩ
 *      |
 *      +---------- ADC Pin
 *      |
 *     [R2] 10kΩ
 *      |
 *     GND
 *
 *   Divider ratio: 10k/(30k+10k) = 0.25
 *   12V × 0.25 = 3V (safe for 3.3V ADC)
 *
 *   Calculate battery voltage:
 *   V_batt = ADC_reading × (3.3 / 4095) × 4
 *            |            |              |
 *            raw value    voltage at pin divider ratio
 *
 * ============================================================================
 *                         DAC BASICS
 * ============================================================================
 *
 * WHAT IT DOES:
 *
 *   Digital Value (0 to 2^N - 1) → Analog Voltage (0V to VREF)
 *
 *   Example: 12-bit DAC, VREF = 3.3V
 *   - 0    → 0V
 *   - 2048 → 1.65V
 *   - 4095 → 3.3V
 *
 * KEY FORMULA:
 *
 *   Vout = (DAC_Value / (2^N - 1)) × Vref
 *
 * COMMON USES:
 *   - Audio output
 *   - Control voltage generation
 *   - Waveform generation
 *   - Calibration signals
 *
 * PWM AS POOR-MAN'S DAC:
 *
 *   If no DAC available, use PWM + low-pass filter:
 *
 *   PWM Pin ---[R]---+---[C]--- Analog Output
 *                    |
 *                   GND
 *
 *   Duty cycle controls average voltage
 *   50% duty on 3.3V = ~1.65V average
 *   Need filter cutoff << PWM frequency
 *
 * ============================================================================
 *                    COMMON ADC ISSUES
 * ============================================================================
 *
 * 1. NOISE:
 *    - Add 100nF capacitor close to ADC pin
 *    - Use averaging (read multiple samples)
 *    - Avoid reading during WiFi/radio transmission
 *
 * 2. GROUND LOOPS:
 *    - Use single point grounding for analog
 *    - Separate AGND and DGND traces
 *
 * 3. ALIASING:
 *    - Signal frequency > Nyquist frequency
 *    - Add anti-aliasing filter before ADC
 *
 * 4. REFERENCE NOISE:
 *    - Noisy VCC = noisy readings
 *    - Use decoupling caps on VREF pin
 *    - Consider external reference
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>

// Convert ADC reading to voltage
float adc_to_voltage(uint16_t adc_value, float vref, int bits) {
    int max_value = (1 << bits) - 1;  // 2^bits - 1
    return ((float)adc_value / max_value) * vref;
}

// Convert voltage to ADC reading
uint16_t voltage_to_adc(float voltage, float vref, int bits) {
    int max_value = (1 << bits) - 1;
    return (uint16_t)((voltage / vref) * max_value);
}

// Calculate ADC resolution (LSB)
float adc_resolution(float vref, int bits) {
    return vref / (1 << bits);
}

// Calculate battery voltage from voltage divider + ADC
float calc_battery_voltage(uint16_t adc_value, float vref, int bits,
                           float r1, float r2) {
    float v_adc = adc_to_voltage(adc_value, vref, bits);
    float divider_ratio = r2 / (r1 + r2);
    return v_adc / divider_ratio;
}

int main() {
    printf("=== ADC/DAC CALCULATIONS ===\n\n");

    // ADC resolution
    printf("ADC Resolution:\n");
    printf("  10-bit, 3.3V: %.2f mV/LSB\n", adc_resolution(3.3, 10) * 1000);
    printf("  12-bit, 3.3V: %.2f mV/LSB\n", adc_resolution(3.3, 12) * 1000);
    printf("  16-bit, 3.3V: %.3f mV/LSB\n\n", adc_resolution(3.3, 16) * 1000);

    // ADC reading to voltage
    printf("ADC to Voltage (12-bit, 3.3V VREF):\n");
    printf("  ADC = 0:    %.3f V\n", adc_to_voltage(0, 3.3, 12));
    printf("  ADC = 2048: %.3f V\n", adc_to_voltage(2048, 3.3, 12));
    printf("  ADC = 4095: %.3f V\n\n", adc_to_voltage(4095, 3.3, 12));

    // Battery voltage measurement
    printf("Battery Measurement (12V max, R1=30k, R2=10k):\n");
    printf("  At 12V: ADC would read ~%d\n",
           voltage_to_adc(3.0, 3.3, 12));  // 12V × 0.25 = 3V
    printf("  At 9V:  ADC would read ~%d\n",
           voltage_to_adc(2.25, 3.3, 12)); // 9V × 0.25 = 2.25V

    // Reverse calculation
    printf("\n  ADC reads 2792: Battery = %.2f V\n",
           calc_battery_voltage(2792, 3.3, 12, 30000, 10000));

    return 0;
}

/*
 * ============================================================================
 * INTERVIEW QUESTIONS & ANSWERS
 * ============================================================================
 *
 * Q1: "What is ADC resolution and why does it matter?"
 * A1:
 *   Resolution = number of bits
 *   LSB (smallest step) = Vref / 2^N
 *
 *   12-bit with 3.3V: LSB = 0.8mV
 *   10-bit with 3.3V: LSB = 3.2mV
 *
 *   Matters because: You can't measure changes smaller than 1 LSB
 *   If measuring millivolts, need higher resolution
 *
 * Q2: "How do you measure 0-10V with an MCU that has 3.3V ADC?"
 * A2:
 *   Use voltage divider to scale down:
 *   10V → 3V (safe for 3.3V ADC with margin)
 *
 *   Ratio = 3/10 = 0.3
 *   R2/(R1+R2) = 0.3
 *   If R2 = 10kΩ: R1 = 23.3kΩ (use 22kΩ)
 *
 *   Calculate: V_input = V_adc × (R1+R2)/R2 = V_adc × 3.2
 *
 * Q3: "ADC readings are noisy. How do you fix it?"
 * A3:
 *   Hardware:
 *   1. Add 100nF capacitor on ADC pin
 *   2. Use separate analog ground
 *   3. Keep analog traces away from digital/PWM
 *   4. Add RC low-pass filter before ADC
 *
 *   Software:
 *   1. Averaging: Read N samples, divide by N
 *   2. Moving average filter
 *   3. Median filter (rejects spikes)
 *   4. Oversampling + decimation
 *
 * Q4: "What's the difference between SAR and Sigma-Delta ADC?"
 * A4:
 *   SAR (Successive Approximation):
 *   - Fast (100 kSPS to MHz)
 *   - Medium resolution (10-16 bit)
 *   - Most common in MCUs
 *
 *   Sigma-Delta:
 *   - Slow (10-1000 SPS)
 *   - High resolution (16-24 bit)
 *   - Better for precision measurement
 *   - Built-in digital filter
 *
 * Q5: "Convert ADC reading 2500 to voltage (12-bit, 3.3V ref)"
 * A5:
 *   V = (ADC / 4095) × 3.3V
 *   V = (2500 / 4095) × 3.3V
 *   V = 0.610 × 3.3V
 *   V = 2.01V
 *
 * Q6: "Your ADC reads 0 even with 1.5V input. What's wrong?"
 * A6:
 *   Possible issues:
 *   1. Wrong pin configured for ADC
 *   2. ADC not enabled/initialized
 *   3. GPIO still in digital mode (not analog)
 *   4. Voltage reference issue
 *   5. Input impedance too high (source can't drive ADC)
 *   6. Protection diode clamping (check if damaged)
 *
 * Q7: "How does oversampling improve ADC resolution?"
 * A7:
 *   Take 4× samples and average → +1 bit resolution
 *   Take 16× samples → +2 bits
 *   Take 64× samples → +3 bits
 *
 *   Works because noise is random, averaging reduces it
 *   Trade-off: Slower effective sample rate
 *
 *   Formula: Extra bits = log2(√N) = 0.5 × log2(N)
 *
 * ============================================================================
 * COMMON ADC SPECIFICATIONS:
 *   Resolution: 10-16 bits
 *   Sample rate: kSPS to MSPS
 *   INL/DNL: Linearity errors (in LSB)
 *   Input impedance: Usually >1MΩ
 *   Conversion time: µs to ms
 * ============================================================================
 */
