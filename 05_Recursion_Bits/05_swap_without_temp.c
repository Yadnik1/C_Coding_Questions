/*
 * ============================================================================
 * PROBLEM: Swap Two Numbers Without Temp Variable
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 2 mins | FREQUENCY: High
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. XOR METHOD (Best):
 *    - a ^ a = 0
 *    - a ^ 0 = a
 *
 * 2. ARITHMETIC METHOD:
 *    - Using addition/subtraction
 *    - Risk of overflow!
 *
 * ============================================================================
 * XOR SWAP VISUALIZATION:
 * ============================================================================
 *
 * a = 5 = 0101
 * b = 3 = 0011
 *
 * Step 1: a = a ^ b
 *   a = 0101 ^ 0011 = 0110 (6)
 *   b = 0011 (3)
 *
 * Step 2: b = a ^ b
 *   a = 0110 (6)
 *   b = 0110 ^ 0011 = 0101 (5) ← Original a!
 *
 * Step 3: a = a ^ b
 *   a = 0110 ^ 0101 = 0011 (3) ← Original b!
 *   b = 0101 (5)
 *
 * SWAPPED! a = 3, b = 5
 *
 * ============================================================================
 * WHY XOR WORKS:
 * ============================================================================
 *
 * Let original values be A and B
 *
 * After step 1: a = A ^ B
 * After step 2: b = (A ^ B) ^ B = A ^ (B ^ B) = A ^ 0 = A
 * After step 3: a = (A ^ B) ^ A = (A ^ A) ^ B = 0 ^ B = B
 *
 * ============================================================================
 * ARITHMETIC METHOD:
 * ============================================================================
 *
 * a = 5, b = 3
 *
 * Step 1: a = a + b = 8
 * Step 2: b = a - b = 8 - 3 = 5 (original a)
 * Step 3: a = a - b = 8 - 5 = 3 (original b)
 *
 * WARNING: Can overflow for large numbers!
 *
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why check a != b in XOR swap?"
 * A1: If a and b point to SAME memory location, XOR swap fails!
 *     a = a ^ a = 0, then both variables become 0.
 *     Self-swap bug! Always check pointers are different.
 *
 * -------------------------------------------------------------------------
 * Q2: "Which method is better: XOR or temp variable?"
 * A2: Temp variable is better in production! It's clearer, no edge cases,
 *     and modern compilers optimize it to register swaps anyway.
 *     XOR swap is for interviews and embedded constraints only.
 *
 * -------------------------------------------------------------------------
 * Q3: "When would you actually use no-temp swap?"
 * A3: Embedded systems with extreme memory constraints, or swapping register
 *     values in assembly. In C, the compiler often does better optimization
 *     with temp variable than with XOR tricks.
 *
 * -------------------------------------------------------------------------
 * Q4: "Can XOR swap work with floating point?"
 * A4: Not directly! XOR is a bitwise integer operation. For floats,
 *     you'd need to reinterpret bits as integers, swap, then convert back.
 *     Just use temp variable for floats!
 *
 * ============================================================================
 */

#include <stdio.h>

// XOR method (preferred)
void swapXOR(int *a, int *b) {
    // Say: "First, I'll check if a and b point to the same location"
    // Avoid self-swap issue (would result in zero)
    if (a != b) {  // Avoid self-swap issue
        // Say: "Step 1: I'll XOR a with b and store in a"
        // a now contains a ^ b
        *a = *a ^ *b;

        // Say: "Step 2: I'll XOR the new a with b to recover original a, storing in b"
        // b now contains original a
        *b = *a ^ *b;

        // Say: "Step 3: I'll XOR the new a with new b to recover original b, storing in a"
        // a now contains original b
        *a = *a ^ *b;
    }
}

// Arithmetic method (risk of overflow)
void swapArithmetic(int *a, int *b) {
    // Say: "Step 1: I'll add a and b, storing the sum in a"
    // a now contains sum of both
    *a = *a + *b;

    // Say: "Step 2: I'll subtract b from the sum to get original a, storing in b"
    // b now contains original a
    *b = *a - *b;

    // Say: "Step 3: I'll subtract new b from the sum to get original b, storing in a"
    // a now contains original b
    *a = *a - *b;
}

// Traditional with temp (for comparison)
void swapTemp(int *a, int *b) {
    // Say: "I'll use a temporary variable to hold a"
    // Store original a in temp
    int temp = *a;

    // Say: "Then I'll copy b to a"
    // Copy b to a
    *a = *b;

    // Say: "Finally, I'll copy the saved value to b"
    // Copy saved a to b
    *b = temp;
}

int main() {
    printf("=== Swap Without Temp ===\n\n");

    // Declare variables for testing
    int x, y;

    // XOR method
    // Initialize test values
    x = 5; y = 3;
    printf("XOR Method:\n");
    printf("Before: x = %d, y = %d\n", x, y);
    // Perform XOR swap
    swapXOR(&x, &y);
    printf("After:  x = %d, y = %d\n\n", x, y);

    // Arithmetic method
    // Initialize test values
    x = 10; y = 20;
    printf("Arithmetic Method:\n");
    printf("Before: x = %d, y = %d\n", x, y);
    // Perform arithmetic swap
    swapArithmetic(&x, &y);
    printf("After:  x = %d, y = %d\n\n", x, y);

    // Step by step XOR
    printf("XOR Step-by-Step (a=5, b=3):\n");
    // Initialize variables
    int a = 5, b = 3;
    printf("Initial: a=%d, b=%d\n", a, b);

    // Say: "Step 1: a = a ^ b"
    // XOR a and b, store in a
    a = a ^ b;
    printf("a = a^b: a=%d, b=%d\n", a, b);

    // Say: "Step 2: b = a ^ b"
    // XOR a and b, store in b (b now has original a)
    b = a ^ b;
    printf("b = a^b: a=%d, b=%d\n", a, b);

    // Say: "Step 3: a = a ^ b"
    // XOR a and b, store in a (a now has original b)
    a = a ^ b;
    printf("a = a^b: a=%d, b=%d\n", a, b);

    return 0;
}
