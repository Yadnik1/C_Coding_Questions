// Fixed Point Math - ESSENTIAL for MCUs without FPU
// When floating point is too slow or unavailable

#include <stdio.h>
#include <stdint.h>

// ============================================================
// Q16.16 Fixed Point Format
// ============================================================

// Say: "Q16.16 means 16 integer bits, 16 fractional bits"
typedef int32_t fixed_t;

#define FIXED_SHIFT 16
#define FIXED_SCALE (1 << FIXED_SHIFT)  // 65536

// Say: "Convert integer to fixed point"
#define INT_TO_FIXED(x)   ((fixed_t)((x) << FIXED_SHIFT))

// Say: "Convert fixed point to integer (truncate)"
#define FIXED_TO_INT(x)   ((x) >> FIXED_SHIFT)

// Say: "Convert float to fixed point (for initialization)"
#define FLOAT_TO_FIXED(x) ((fixed_t)((x) * FIXED_SCALE))

// Say: "Convert fixed point to float (for display)"
#define FIXED_TO_FLOAT(x) ((float)(x) / FIXED_SCALE)

// Say: "Addition - just add (same scale)"
fixed_t fixed_add(fixed_t a, fixed_t b) {
    return a + b;  // Say: "Simple addition, check for overflow in production"
}

// Say: "Subtraction - just subtract"
fixed_t fixed_sub(fixed_t a, fixed_t b) {
    return a - b;
}

// Say: "Multiplication - must shift result back"
fixed_t fixed_mul(fixed_t a, fixed_t b) {
    // Say: "Use 64-bit intermediate to prevent overflow"
    int64_t result = (int64_t)a * (int64_t)b;
    return (fixed_t)(result >> FIXED_SHIFT);
}

// Say: "Division - must shift numerator first"
fixed_t fixed_div(fixed_t a, fixed_t b) {
    // Say: "Shift numerator before division for precision"
    int64_t temp = (int64_t)a << FIXED_SHIFT;
    return (fixed_t)(temp / b);
}

// ============================================================
// Q8.8 Fixed Point (for smaller MCUs)
// ============================================================

typedef int16_t fixed8_t;

#define FIXED8_SHIFT 8
#define FIXED8_SCALE (1 << FIXED8_SHIFT)  // 256

#define INT_TO_FIXED8(x)   ((fixed8_t)((x) << FIXED8_SHIFT))
#define FIXED8_TO_INT(x)   ((x) >> FIXED8_SHIFT)
#define FLOAT_TO_FIXED8(x) ((fixed8_t)((x) * FIXED8_SCALE))
#define FIXED8_TO_FLOAT(x) ((float)(x) / FIXED8_SCALE)

fixed8_t fixed8_mul(fixed8_t a, fixed8_t b) {
    int32_t result = (int32_t)a * (int32_t)b;
    return (fixed8_t)(result >> FIXED8_SHIFT);
}

// ============================================================
// Practical Example: Temperature Conversion
// ============================================================

// Say: "Convert Celsius to Fahrenheit: F = C * 1.8 + 32"
fixed_t celsius_to_fahrenheit(fixed_t celsius) {
    // Say: "1.8 in Q16.16 = 0x1CCCC"
    fixed_t factor = FLOAT_TO_FIXED(1.8f);
    fixed_t offset = INT_TO_FIXED(32);

    return fixed_add(fixed_mul(celsius, factor), offset);
}

// ============================================================
// Lookup Table for Trig Functions
// ============================================================

// Say: "Sine lookup table (Q15 format, 0-90 degrees, 1 degree steps)"
static const int16_t sin_table[91] = {
    0, 572, 1144, 1715, 2286, 2856, 3425, 3993, 4560, 5126,
    5690, 6252, 6813, 7371, 7927, 8481, 9032, 9580, 10126, 10668,
    11207, 11743, 12275, 12803, 13328, 13848, 14365, 14876, 15384, 15886,
    16384, 16877, 17364, 17847, 18324, 18795, 19261, 19720, 20174, 20622,
    21063, 21498, 21926, 22348, 22763, 23170, 23571, 23965, 24351, 24730,
    25102, 25466, 25822, 26170, 26510, 26842, 27166, 27482, 27789, 28088,
    28378, 28660, 28932, 29197, 29452, 29698, 29935, 30163, 30382, 30592,
    30792, 30983, 31164, 31336, 31499, 31651, 31795, 31928, 32052, 32166,
    32270, 32365, 32449, 32524, 32588, 32643, 32688, 32723, 32748, 32763,
    32767  // sin(90) = 1.0 in Q15
};

// Say: "Get sine value using lookup (Q15 format)"
int16_t fixed_sin(int angle) {
    // Say: "Normalize angle to 0-359"
    angle = angle % 360;
    if (angle < 0) angle += 360;

    // Say: "Use symmetry: sin(x) = sin(180-x), sin(x) = -sin(x-180)"
    int16_t sign = 1;
    if (angle >= 180) {
        angle -= 180;
        sign = -1;
    }
    if (angle > 90) {
        angle = 180 - angle;
    }

    return sign * sin_table[angle];
}

int main() {
    printf("=== Fixed Point Math Demo ===\n\n");

    // Basic operations
    fixed_t a = FLOAT_TO_FIXED(3.5f);
    fixed_t b = FLOAT_TO_FIXED(2.25f);

    printf("Q16.16 Fixed Point Operations:\n");
    printf("a = 3.5,  b = 2.25\n");
    printf("a + b = %.4f\n", FIXED_TO_FLOAT(fixed_add(a, b)));
    printf("a - b = %.4f\n", FIXED_TO_FLOAT(fixed_sub(a, b)));
    printf("a * b = %.4f\n", FIXED_TO_FLOAT(fixed_mul(a, b)));
    printf("a / b = %.4f\n", FIXED_TO_FLOAT(fixed_div(a, b)));

    // Temperature conversion
    printf("\nTemperature Conversion:\n");
    fixed_t temp_c = FLOAT_TO_FIXED(25.0f);
    fixed_t temp_f = celsius_to_fahrenheit(temp_c);
    printf("25.0°C = %.2f°F\n", FIXED_TO_FLOAT(temp_f));

    // Sine lookup
    printf("\nSine Lookup Table (Q15):\n");
    int angles[] = {0, 30, 45, 60, 90, 180, 270};
    for (int i = 0; i < 7; i++) {
        int16_t sin_val = fixed_sin(angles[i]);
        printf("sin(%3d°) = %6d (%.4f)\n",
               angles[i], sin_val, (float)sin_val / 32768.0f);
    }

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Fixed point math represents fractional numbers using integers,
 essential when hardware FPU is absent or too slow.

 Q FORMAT NOTATION:
 - Qm.n: m integer bits, n fractional bits
 - Q16.16: 16 integer, 16 fractional (32-bit total)
 - Q8.8: 8 integer, 8 fractional (16-bit total)
 - Q1.15: 1 sign, 15 fractional (for -1 to +1 range)

 CONVERSION:
 - Int to Fixed: value << n
 - Fixed to Int: value >> n
 - Float to Fixed: value * (1 << n)
 - Fixed to Float: value / (1 << n)

 OPERATIONS:
 - Addition/Subtraction: Direct (same scale)
 - Multiplication: result = (a * b) >> n
 - Division: result = (a << n) / b

 WHY USE 64-BIT INTERMEDIATE:
 - Q16.16 * Q16.16 needs 64 bits before shift
 - Prevents overflow in multiplication
 - Then shift back to 32 bits

 ADVANTAGES:
 1. Fast on MCUs without FPU
 2. Deterministic timing
 3. Smaller code size
 4. Lower power consumption

 LIMITATIONS:
 1. Limited range and precision
 2. Overflow possible
 3. Rounding errors accumulate
 4. More complex code

 WHEN TO USE:
 - 8/16-bit MCUs (no FPU)
 - DSP algorithms
 - Control loops requiring deterministic timing
 - Battery-powered devices (power saving)

 LOOKUP TABLES:
 - Precompute expensive functions (sin, cos, sqrt)
 - Trade memory for speed
 - Interpolate for values between entries"
*/
