/*
 * ============================================================================
 * PROBLEM: Set, Clear, Toggle, and Check Bits
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Implement the four fundamental bit manipulation operations:
 * - SET a bit (make it 1)
 * - CLEAR a bit (make it 0)
 * - TOGGLE a bit (flip 0<->1)
 * - CHECK a bit (read its value)
 * These are the MOST ESSENTIAL operations for embedded programming!
 *
 * EXAMPLES (with binary, operating on bit position 2):
 *   Original: 0000_1010 (10)
 *                  ^bit 2
 *
 *   SET bit 2:    0000_1110 (14)    <- bit 2 is now 1
 *   CLEAR bit 2:  0000_1010 (10)    <- bit 2 is already 0, unchanged
 *   TOGGLE bit 2: 0000_1110 (14)    <- bit 2 flipped from 0 to 1
 *   CHECK bit 2:  returns 0         <- bit 2 was 0
 *
 *   Another example, bit 3 on 0000_1010:
 *   SET bit 3:    0000_1010 (10)    <- already 1, unchanged
 *   CLEAR bit 3:  0000_0010 (2)     <- bit 3 cleared to 0
 *   TOGGLE bit 3: 0000_0010 (2)     <- bit 3 flipped from 1 to 0
 *   CHECK bit 3:  returns 1         <- bit 3 was 1
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - Controlling GPIO pins (LED on/off, reading buttons)
 * - Setting/clearing flags in hardware registers
 * - Enabling/disabling interrupts
 * - Configuring peripheral modes
 * - Managing status bits in protocols
 * - THIS IS WHAT EMBEDDED ENGINEERS DO DAILY!
 *
 * KEY CONCEPT - THE FOUR OPERATIONS:
 *   SET:    num | (1 << pos)     OR with mask sets the bit
 *   CLEAR:  num & ~(1 << pos)    AND with inverted mask clears
 *   TOGGLE: num ^ (1 << pos)     XOR with mask flips the bit
 *   CHECK:  (num >> pos) & 1     Shift down and mask to get 0 or 1
 *
 * VISUAL:
 *   Creating the mask: (1 << pos) where pos = 2
 *   1 << 2 = 0000_0100
 *                  ^bit 2
 *
 *   SET (OR with mask):
 *   num:        0000_1010
 *   mask:       0000_0100
 *   OR result:  0000_1110    <- bit 2 is set, others unchanged
 *
 *   CLEAR (AND with inverted mask):
 *   num:        0000_1010
 *   ~mask:      1111_1011    <- inverted: 0 only at position 2
 *   AND result: 0000_1010    <- bit 2 cleared, others unchanged
 *
 *   TOGGLE (XOR with mask):
 *   num:        0000_1010
 *   mask:       0000_0100
 *   XOR result: 0000_1110    <- bit 2 flipped, others unchanged
 *
 *   CHECK (shift and mask):
 *   num >> 2:   0000_0010    <- bring bit 2 to position 0
 *   & 1:        0000_0000    <- mask to get just that bit (result: 0)
 *
 * EMBEDDED EXAMPLE - GPIO LED Control:
 *   #define LED_PIN 5
 *   GPIO |= (1 << LED_PIN);      // Turn LED ON
 *   GPIO &= ~(1 << LED_PIN);     // Turn LED OFF
 *   GPIO ^= (1 << LED_PIN);      // Toggle LED (blink)
 *   if (GPIO & (1 << LED_PIN))   // Check if LED is ON
 *
 * TIME COMPLEXITY: O(1)
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

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
