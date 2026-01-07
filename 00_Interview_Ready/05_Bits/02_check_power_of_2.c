// Check if number is power of 2
// Time: O(1), Space: O(1)

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void print_binary(uint32_t n) {
    printf("0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i != 0) printf("_");
    }
}

bool is_power_of_2(uint32_t n) {
    // Say: "Power of 2 has exactly ONE bit set"
    // Say: "Example: 8 = 1000, 16 = 10000"

    // Say: "n & (n-1) clears the rightmost set bit"
    // Say: "If n is power of 2, clearing that one bit gives 0"
    // Say: "Also check n != 0 because 0 is not a power of 2"

    return (n != 0) && ((n & (n - 1)) == 0);
}

int main() {
    uint32_t test_values[] = {0, 1, 2, 3, 4, 8, 15, 16, 32, 100};
    int n = sizeof(test_values) / sizeof(test_values[0]);

    printf("Checking power of 2:\n");
    for (int i = 0; i < n; i++) {
        printf("%3u: ", test_values[i]);
        print_binary(test_values[i]);
        printf(" -> %s\n", is_power_of_2(test_values[i]) ? "YES" : "NO");
    }

    return 0;
}

/*
INTERVIEW EXPLANATION:
"Power of 2 has exactly one bit set (like 1, 10, 100, 1000 in binary).

 I use: (n != 0) && ((n & (n-1)) == 0)

 Why n & (n-1)?
 - (n-1) flips all bits after rightmost 1, including that 1
 - If n has only one 1 bit, n & (n-1) becomes 0
 - If n has multiple 1s, result is non-zero

 Example: n = 8 (1000)
 - n-1 = 7 (0111)
 - n & (n-1) = 1000 & 0111 = 0000 ✓ Power of 2

 Example: n = 6 (0110)
 - n-1 = 5 (0101)
 - n & (n-1) = 0110 & 0101 = 0100 ✗ Not power of 2

 Why check n != 0?
 - 0 & (-1) = 0, but 0 is not a power of 2

 Time: O(1), Space: O(1) - single bitwise operation"
*/
