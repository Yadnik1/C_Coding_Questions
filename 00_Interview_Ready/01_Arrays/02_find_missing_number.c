/*
 * ============================================================================
 * PROBLEM: Find Missing Number
 * ============================================================================
 *
 * Given an array containing n distinct numbers from 0 to n,
 * find the one number that is missing.
 *
 * EXAMPLES:
 *   [0, 1, 2, 4, 5, 6]  ->  3 (missing from 0-6)
 *   [3, 0, 1]          ->  2 (missing from 0-3)
 *   [0]                ->  1 (missing from 0-1)
 *
 * KEY INSIGHT: Use XOR! a ^ a = 0, so pairs cancel out.
 *
 * TIME COMPLEXITY: O(n)
 * SPACE COMPLEXITY: O(1)
 *
 * ============================================================================
 */

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]");
}

int find_missing(int arr[], int n) {
    int xor_result = n;                    // Start with n (largest expected)
    for (int i = 0; i < n; i++) {
        xor_result ^= i;                   // XOR with expected number
        xor_result ^= arr[i];              // XOR with actual number
    }
    return xor_result;                     // Pairs cancel, missing remains
}

int main() {
    int arr[] = {0, 1, 2, 4, 5, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array: ");
    print_array(arr, n);
    printf("\nMissing: %d\n", find_missing(arr, n));

    return 0;
}

/*
 * ============================================================================
 * DRY RUN: find_missing([0, 1, 3], n=3)
 * ============================================================================
 *
 * Range: 0 to 3, Missing: 2
 *
 * xor_result = n = 3 = 0011 (binary)
 *
 * i=0: xor_result ^= 0 -> 0011 ^ 0000 = 0011
 *      xor_result ^= arr[0]=0 -> 0011 ^ 0000 = 0011
 *
 * i=1: xor_result ^= 1 -> 0011 ^ 0001 = 0010
 *      xor_result ^= arr[1]=1 -> 0010 ^ 0001 = 0011
 *
 * i=2: xor_result ^= 2 -> 0011 ^ 0010 = 0001
 *      xor_result ^= arr[2]=3 -> 0001 ^ 0011 = 0010
 *
 * RESULT: 0010 = 2 (THE MISSING NUMBER!)
 *
 * WHY IT WORKS:
 *   XORed values: 3, 0, 0, 1, 1, 2, 3
 *   Rearranged: (0^0) ^ (1^1) ^ (3^3) ^ 2 = 0 ^ 0 ^ 0 ^ 2 = 2
 *   Pairs cancel, only missing number survives!
 *
 * ============================================================================
 * INTERVIEW TIPS:
 * - XOR is better than sum formula (no overflow risk)
 * - a ^ a = 0 (cancel), a ^ 0 = a (identity)
 * - Same trick works for "find single number" problems
 * ============================================================================
 */
