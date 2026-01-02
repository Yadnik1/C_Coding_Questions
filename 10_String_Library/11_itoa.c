/*
 * ============================================================================
 * PROBLEM: Implement itoa()
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * itoa = Integer TO ASCII (int → string)
 *
 * ============================================================================
 * VISUALIZATION: itoa(-456)
 * ============================================================================
 *
 *   Input: -456
 *
 *   Step 1: Note negative, work with 456
 *
 *   Step 2: Extract digits (reverse order):
 *           456 % 10 = 6, 456 / 10 = 45
 *           45 % 10 = 5, 45 / 10 = 4
 *           4 % 10 = 4, 4 / 10 = 0 → STOP
 *
 *   Step 3: Digits collected: "654"
 *
 *   Step 4: Reverse: "456"
 *
 *   Step 5: Add sign: "-456"
 *
 * ============================================================================
 * TIME: O(log n) | SPACE: O(1) - modifies provided buffer
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

// itoa with base support (2, 10, 16, etc.)
char* my_itoa(int value, char* str, int base) {
    if (str == NULL || base < 2 || base > 36) {
        return NULL;
    }

    char* ptr = str;
    char* start = str;
    bool negative = false;

    // Handle 0
    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return str;
    }

    // Handle negative (only for base 10)
    if (value < 0 && base == 10) {
        negative = true;
        if (value == INT_MIN) {
            // Special case: can't negate INT_MIN
            *ptr++ = '8';
            value = -214748364;
        } else {
            value = -value;
        }
    } else if (value < 0) {
        // For other bases, treat as unsigned
        value = (unsigned int)value;
    }

    // Extract digits in reverse order
    while (value != 0) {
        int digit = value % base;
        if (digit < 10) {
            *ptr++ = '0' + digit;
        } else {
            *ptr++ = 'a' + (digit - 10);
        }
        value /= base;
    }

    // Add negative sign
    if (negative) {
        *ptr++ = '-';
    }

    *ptr = '\0';

    // Reverse the string
    ptr--;
    while (start < ptr) {
        char temp = *start;
        *start = *ptr;
        *ptr = temp;
        start++;
        ptr--;
    }

    return str;
}

// Simple itoa (base 10 only)
char* my_itoa_simple(int value, char* str) {
    if (str == NULL) return NULL;

    char buffer[20];
    int i = 0;
    bool negative = false;

    // Handle 0
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    // Handle negative
    if (value < 0) {
        negative = true;
        value = -value;
    }

    // Extract digits (reverse order)
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    // Build result
    int j = 0;
    if (negative) {
        str[j++] = '-';
    }

    // Copy in reverse
    while (i > 0) {
        str[j++] = buffer[--i];
    }
    str[j] = '\0';

    return str;
}

int main() {
    printf("=== itoa Implementation ===\n\n");

    char buffer[50];

    printf("1. Basic conversion (base 10):\n");
    printf("   itoa(123)  → \"%s\"\n", my_itoa(123, buffer, 10));
    printf("   itoa(-456) → \"%s\"\n", my_itoa(-456, buffer, 10));
    printf("   itoa(0)    → \"%s\"\n\n", my_itoa(0, buffer, 10));

    printf("2. Different bases:\n");
    printf("   itoa(255, base 10) → \"%s\"\n", my_itoa(255, buffer, 10));
    printf("   itoa(255, base 16) → \"%s\" (hex)\n", my_itoa(255, buffer, 16));
    printf("   itoa(255, base 2)  → \"%s\" (binary)\n", my_itoa(255, buffer, 2));
    printf("   itoa(255, base 8)  → \"%s\" (octal)\n\n", my_itoa(255, buffer, 8));

    printf("3. Larger numbers:\n");
    printf("   itoa(12345)     → \"%s\"\n", my_itoa(12345, buffer, 10));
    printf("   itoa(-98765)    → \"%s\"\n", my_itoa(-98765, buffer, 10));
    printf("   itoa(INT_MAX)   → \"%s\"\n", my_itoa(INT_MAX, buffer, 10));
    printf("   itoa(INT_MIN)   → \"%s\"\n\n", my_itoa(INT_MIN, buffer, 10));

    printf("4. Hex values:\n");
    printf("   itoa(4096, base 16)  → \"%s\"\n", my_itoa(4096, buffer, 16));
    printf("   itoa(65535, base 16) → \"%s\"\n\n", my_itoa(65535, buffer, 16));

    printf("=== Key Points ===\n");
    printf("- Extract digits using %% 10 (reverse order)\n");
    printf("- Divide by base to get next digit\n");
    printf("- Reverse the string at the end\n");
    printf("- Handle 0 and negative specially\n");
    printf("- For hex: digits 10-15 → 'a'-'f'\n");

    return 0;
}
