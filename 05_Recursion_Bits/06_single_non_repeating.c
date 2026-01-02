/*
 * ============================================================================
 * PROBLEM: Find Single Non-Repeating Element
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * All elements appear twice except one. Find it.
 *
 * Example:
 * Input:  [4, 1, 2, 1, 2]
 * Output: 4
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. XOR PROPERTIES:
 *    - a ^ a = 0 (same numbers cancel)
 *    - a ^ 0 = a (XOR with 0 gives same number)
 *    - XOR is commutative and associative
 *
 * 2. SOLUTION:
 *    - XOR all elements together
 *    - Duplicates cancel out
 *    - Only unique element remains
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: [4, 1, 2, 1, 2]
 * ============================================================================
 *
 * XOR all elements:
 *
 *   4 ^ 1 ^ 2 ^ 1 ^ 2
 *
 * Reorder (commutative):
 *   = 4 ^ (1 ^ 1) ^ (2 ^ 2)
 *   = 4 ^ 0 ^ 0
 *   = 4
 *
 * Result: 4 (the unique element!)
 *
 * ============================================================================
 * BINARY VISUALIZATION:
 * ============================================================================
 *
 *   4 = 100
 *   1 = 001
 *   2 = 010
 *   1 = 001
 *   2 = 010
 *
 *   XOR step by step:
 *     100 ^ 001 = 101
 *     101 ^ 010 = 111
 *     111 ^ 001 = 110
 *     110 ^ 010 = 100 = 4 ✓
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 */

#include <stdio.h>

int findSingle(int arr[], int n) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        result ^= arr[i];
    }
    return result;
}

void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]");
}

int main() {
    printf("=== Find Single Non-Repeating Element ===\n\n");

    int arr1[] = {4, 1, 2, 1, 2};
    int n1 = 5;
    printf("Array: ");
    printArray(arr1, n1);
    printf("\nSingle element: %d\n\n", findSingle(arr1, n1));

    int arr2[] = {2, 3, 5, 4, 5, 3, 4};
    int n2 = 7;
    printf("Array: ");
    printArray(arr2, n2);
    printf("\nSingle element: %d\n\n", findSingle(arr2, n2));

    // Step by step XOR
    printf("Step-by-step XOR for [4, 1, 2, 1, 2]:\n");
    int result = 0;
    int arr[] = {4, 1, 2, 1, 2};
    for (int i = 0; i < 5; i++) {
        printf("  %d ^ %d = %d\n", result, arr[i], result ^ arr[i]);
        result ^= arr[i];
    }
    printf("Final result: %d\n", result);

    return 0;
}
