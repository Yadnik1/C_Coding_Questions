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
    // Say: "I'll initialize result to 0"
    // Initialize result to 0 (XOR identity)
    int result = 0;

    // Say: "Then I'll XOR all elements together"
    // Loop through all elements
    for (int i = 0; i < n; i++) {
        // Say: "XOR current element with result"
        // XOR current element into result (duplicates cancel)
        result ^= arr[i];
    }

    // Say: "After XORing all elements, duplicates cancel and only the unique element remains"
    // Return the unique element
    return result;
}

void printArray(int arr[], int n) {
    // Print opening bracket
    printf("[");

    // Loop through array elements
    for (int i = 0; i < n; i++) {
        // Print element
        printf("%d", arr[i]);

        // Print comma if not last element
        if (i < n - 1) printf(", ");
    }

    // Print closing bracket
    printf("]");
}

int main() {
    printf("=== Find Single Non-Repeating Element ===\n\n");

    // Test case 1
    // Declare and initialize array
    int arr1[] = {4, 1, 2, 1, 2};
    // Calculate array size
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("Array: ");
    // Print the array
    printArray(arr1, n1);
    // Find and print the single element
    printf("\nSingle element: %d\n\n", findSingle(arr1, n1));

    // Test case 2
    // Declare and initialize array
    int arr2[] = {2, 3, 5, 4, 5, 3, 4};
    // Calculate array size
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Array: ");
    // Print the array
    printArray(arr2, n2);
    // Find and print the single element
    printf("\nSingle element: %d\n\n", findSingle(arr2, n2));

    // Step by step XOR
    printf("Step-by-step XOR for [4, 1, 2, 1, 2]:\n");
    // Initialize result to 0
    int result = 0;
    // Declare test array
    int arr[] = {4, 1, 2, 1, 2};

    // Loop through array
    for (int i = 0; i < 5; i++) {
        // Print XOR operation
        printf("  %d ^ %d = %d\n", result, arr[i], result ^ arr[i]);
        // Perform XOR
        result ^= arr[i];
    }

    // Print final result
    printf("Final result: %d\n", result);

    return 0;
}
