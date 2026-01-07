// Swap two numbers without using temporary variable
// Time: O(1), Space: O(1)

#include <stdio.h>

void swap_xor(int *a, int *b) {
    // Say: "XOR swap - uses property that X ^ X = 0"
    // Say: "Works because XOR is its own inverse"

    if (a == b) return;  // Say: "Same address would zero both values"

    *a = *a ^ *b;  // Say: "a now holds XOR of original a and b"
    *b = *a ^ *b;  // Say: "b = (a^b) ^ b = a (original a)"
    *a = *a ^ *b;  // Say: "a = (a^b) ^ a = b (original b)"

    // Say: "After this, values are swapped"
}

int main() {
    int x = 5, y = 10;

    printf("Before: x=%d, y=%d\n", x, y);
    swap_xor(&x, &y);
    printf("After:  x=%d, y=%d\n", x, y);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use XOR swap based on XOR properties:
 - A ^ A = 0 (XOR with itself gives 0)
 - A ^ 0 = A (XOR with 0 gives original)
 - XOR is commutative and associative

 Step by step for a=5, b=3:
 - a = a ^ b = 5 ^ 3 = 6    (a holds encoded value)
 - b = a ^ b = 6 ^ 3 = 5    (b now has original a)
 - a = a ^ b = 6 ^ 5 = 3    (a now has original b)

 IMPORTANT CAVEAT:
 - If a and b point to same memory location, this zeros both!
 - That's why we check if (a == b) first

 WHY THIS MATTERS IN EMBEDDED:
 - Sometimes used in memory-constrained systems
 - But modern compilers optimize temp variable swap well
 - Usually prefer temp variable for clarity

 Time: O(1), Space: O(1)"
*/
