/*
 * ============================================================================
 * PROBLEM: Find the Single Number (XOR All Elements)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given an array where every element appears TWICE except for one element
 * that appears only ONCE, find that single element. Solve in O(n) time
 * and O(1) space (no hash maps or extra storage allowed).
 *
 * EXAMPLES (with binary representation):
 * - Input:  [4, 1, 2, 1, 2]
 *   Binary:  4=0100, 1=0001, 2=0010, 1=0001, 2=0010
 *   Output: 4
 *
 *   Why? 4 ^ 1 ^ 2 ^ 1 ^ 2
 *        = 4 ^ (1 ^ 1) ^ (2 ^ 2)   <- pairs cancel out
 *        = 4 ^ 0 ^ 0
 *        = 4
 *
 * - Input:  [2, 2, 1]
 *   Output: 1
 *
 * - Input:  [1]
 *   Output: 1
 *
 * WHY IS THIS ASKED IN EMBEDDED INTERVIEWS?
 * - Classic demonstration of XOR properties
 * - Memory-efficient algorithm (O(1) space)
 * - Finding unique device IDs or error codes
 * - Detecting single bit errors in data transmission
 * - Very common in coding interviews (LeetCode problem)
 *
 * KEY CONCEPT - XOR MAGIC PROPERTIES:
 * 1. A ^ A = 0      (same values cancel out)
 * 2. A ^ 0 = A      (XOR with zero preserves value)
 * 3. XOR is commutative: A ^ B = B ^ A
 * 4. XOR is associative: (A ^ B) ^ C = A ^ (B ^ C)
 *
 * VISUAL:
 *   Array: [4, 1, 2, 1, 2]
 *
 *   Step-by-step XOR:
 *   result = 0
 *
 *   result ^= 4:  0000 ^ 0100 = 0100 (4)
 *   result ^= 1:  0100 ^ 0001 = 0101 (5)
 *   result ^= 2:  0101 ^ 0010 = 0111 (7)
 *   result ^= 1:  0111 ^ 0001 = 0110 (6)    <- first 1 cancelled
 *   result ^= 2:  0110 ^ 0010 = 0100 (4)    <- second 2 cancelled
 *                               ^^^^
 *                         Only 4 remains!
 *
 *   Think of it as: pairs XOR to 0, leaving only the single number.
 *
 * TIME COMPLEXITY: O(n) - single pass through array
 * SPACE COMPLEXITY: O(1) - only one variable needed
 *
 * ============================================================================
 */

// Find single number (all others appear twice)
// Time: O(n), Space: O(1)

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

int find_single(int arr[], int n) {
    // Say: "XOR all elements - duplicates cancel out"
    // Say: "Key property: A ^ A = 0 and A ^ 0 = A"

    int result = 0;

    for (int i = 0; i < n; i++) {
        result ^= arr[i];  // Say: "XOR with each element"
    }

    // Say: "Only the single number remains"
    return result;
}

int main() {
    int arr[] = {4, 1, 2, 1, 2};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array: ");
    print_array(arr, n);

    printf("Single number: %d\n", find_single(arr, n));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I XOR all elements together.

 Why XOR works:
 - A ^ A = 0 (same numbers cancel)
 - A ^ 0 = A (XOR with 0 gives original)
 - XOR is commutative: A ^ B = B ^ A
 - XOR is associative: (A ^ B) ^ C = A ^ (B ^ C)

 Example: [4, 1, 2, 1, 2]
 - 4 ^ 1 ^ 2 ^ 1 ^ 2
 - = 4 ^ (1 ^ 1) ^ (2 ^ 2)  (reorder by associativity)
 - = 4 ^ 0 ^ 0
 - = 4 ✓

 Time: O(n) - single pass
 Space: O(1) - single variable

 This is THE classic bit manipulation interview question!"
*/
