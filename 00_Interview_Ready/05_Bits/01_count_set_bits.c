// Count number of 1s (set bits) in an integer
// Time: O(k) where k = number of set bits, Space: O(1)

#include <stdio.h>
#include <stdint.h>

// Say: "Consistent helper to print binary representation"
void print_binary(uint32_t n) {
    printf("0b");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0 && i != 0) printf("_");  // Say: "Group by nibbles for readability"
    }
    printf("\n");
}

int count_set_bits(uint32_t n) {
    // Say: "Brian Kernighan's algorithm - most efficient"
    // Say: "Key insight: n & (n-1) clears the rightmost set bit"
    int count = 0;

    while (n) {
        n = n & (n - 1);  // Say: "Clear rightmost 1 bit"
        count++;          // Say: "Count each cleared bit"
    }
    // Say: "Loop runs only k times where k = number of set bits"

    return count;
}

int main() {
    uint32_t num = 0b10110101;  // Say: "Binary literal - 5 set bits"

    printf("Number: %u\n", num);
    printf("Binary: ");
    print_binary(num);

    printf("Set bits: %d\n", count_set_bits(num));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use Brian Kernighan's algorithm.

 Key insight: n & (n-1) clears the RIGHTMOST set bit.

 Example: n = 12 (1100 in binary)
 - n & (n-1) = 1100 & 1011 = 1000 (cleared rightmost 1)
 - n & (n-1) = 1000 & 0111 = 0000 (cleared rightmost 1)
 - n = 0, stop. Count = 2 ✓

 Why is n & (n-1) special?
 - (n-1) flips all bits after rightmost 1, including that 1
 - ANDing with n zeros out that rightmost 1

 Time: O(k) where k = number of set bits
 Space: O(1)

 Alternative: Loop through all 32 bits - O(32) = O(1) but slower
 Alternative: Lookup table - O(1) but uses extra memory"
*/
