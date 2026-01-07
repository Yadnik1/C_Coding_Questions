// Set, Clear, Toggle specific bit - ESSENTIAL for embedded
// Time: O(1), Space: O(1)

#include <stdio.h>
#include <stdint.h>

void print_binary(uint32_t n) {
    printf("0b");
    for (int i = 7; i >= 0; i--)
        printf("%d", (n >> i) & 1);
}

// Say: "SET bit - use OR with mask"
uint8_t set_bit(uint8_t num, int pos) {
    // Say: "1 << pos creates mask with 1 at position"
    // Say: "OR ensures that bit becomes 1, others unchanged"
    return num | (1 << pos);
}

// Say: "CLEAR bit - use AND with inverted mask"
uint8_t clear_bit(uint8_t num, int pos) {
    // Say: "~(1 << pos) creates mask with 0 at position, 1s elsewhere"
    // Say: "AND clears that bit, keeps others"
    return num & ~(1 << pos);
}

// Say: "TOGGLE bit - use XOR with mask"
uint8_t toggle_bit(uint8_t num, int pos) {
    // Say: "XOR flips the bit: 0->1 or 1->0"
    return num ^ (1 << pos);
}

// Say: "CHECK bit - AND with mask, check if non-zero"
int check_bit(uint8_t num, int pos) {
    return (num >> pos) & 1;  // Say: "Shift bit to position 0, mask with 1"
}

int main() {
    uint8_t reg = 0b00001010;  // Say: "Simulating a hardware register"
    int bit_pos = 2;

    printf("Original:  ");
    print_binary(reg);
    printf(" (bit %d = %d)\n", bit_pos, check_bit(reg, bit_pos));

    printf("Set bit:   ");
    print_binary(set_bit(reg, bit_pos));
    printf("\n");

    printf("Clear bit: ");
    print_binary(clear_bit(reg, bit_pos));
    printf("\n");

    printf("Toggle:    ");
    print_binary(toggle_bit(reg, bit_pos));
    printf("\n");

    return 0;
}

/*
INTERVIEW EXPLANATION:
"These are the 4 fundamental bit operations for embedded:

 SET BIT (make it 1):
 - num | (1 << pos)
 - OR with 1 sets that bit, OR with 0 keeps original

 CLEAR BIT (make it 0):
 - num & ~(1 << pos)
 - AND with 0 clears that bit, AND with 1 keeps original

 TOGGLE BIT (flip it):
 - num ^ (1 << pos)
 - XOR with 1 flips that bit, XOR with 0 keeps original

 CHECK BIT (read it):
 - (num >> pos) & 1
 - Shift bit to position 0, mask to get 0 or 1

 EMBEDDED EXAMPLE - GPIO register:
 - SET: Turn on LED (GPIO |= (1 << LED_PIN))
 - CLEAR: Turn off LED (GPIO &= ~(1 << LED_PIN))
 - TOGGLE: Blink LED (GPIO ^= (1 << LED_PIN))
 - CHECK: Read button state ((GPIO >> BTN_PIN) & 1)

 All operations are O(1) time, O(1) space."
*/
