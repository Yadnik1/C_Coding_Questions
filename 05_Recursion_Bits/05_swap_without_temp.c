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
 */

#include <stdio.h>

// XOR method (preferred)
void swapXOR(int *a, int *b) {
    if (a != b) {  // Avoid self-swap issue
        *a = *a ^ *b;
        *b = *a ^ *b;
        *a = *a ^ *b;
    }
}

// Arithmetic method (risk of overflow)
void swapArithmetic(int *a, int *b) {
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

// Traditional with temp (for comparison)
void swapTemp(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    printf("=== Swap Without Temp ===\n\n");

    int x, y;

    // XOR method
    x = 5; y = 3;
    printf("XOR Method:\n");
    printf("Before: x = %d, y = %d\n", x, y);
    swapXOR(&x, &y);
    printf("After:  x = %d, y = %d\n\n", x, y);

    // Arithmetic method
    x = 10; y = 20;
    printf("Arithmetic Method:\n");
    printf("Before: x = %d, y = %d\n", x, y);
    swapArithmetic(&x, &y);
    printf("After:  x = %d, y = %d\n\n", x, y);

    // Step by step XOR
    printf("XOR Step-by-Step (a=5, b=3):\n");
    int a = 5, b = 3;
    printf("Initial: a=%d, b=%d\n", a, b);

    a = a ^ b;
    printf("a = a^b: a=%d, b=%d\n", a, b);

    b = a ^ b;
    printf("b = a^b: a=%d, b=%d\n", a, b);

    a = a ^ b;
    printf("a = a^b: a=%d, b=%d\n", a, b);

    return 0;
}
