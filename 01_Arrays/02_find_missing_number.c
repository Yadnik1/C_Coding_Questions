/*
 * ============================================================================
 * PROBLEM: Find Missing Number in Array
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * Given an array of n-1 integers in range [1, n], find the missing number.
 *
 * Example:
 * Input:  [1, 2, 4, 5, 6] (n=6, one number missing)
 * Output: 3
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. SUM FORMULA:
 *    Sum of 1 to n = n * (n + 1) / 2
 *    This is GAUSS'S FORMULA - memorize it!
 *
 * 2. XOR PROPERTY:
 *    a ^ a = 0  (same numbers cancel out)
 *    a ^ 0 = a  (XOR with 0 gives same number)
 *
 * 3. TWO APPROACHES:
 *    Method 1: Sum formula (risk of overflow for large n)
 *    Method 2: XOR (no overflow, preferred)
 *
 * ============================================================================
 * METHOD 1: SUM FORMULA VISUALIZATION
 * ============================================================================
 *
 * Array: [1, 2, 4, 5, 6]  (n = 6, missing = 3)
 *
 * Expected sum = n * (n+1) / 2 = 6 * 7 / 2 = 21
 *
 * Actual sum = 1 + 2 + 4 + 5 + 6 = 18
 *
 * Missing = Expected - Actual = 21 - 18 = 3
 *
 * ============================================================================
 * METHOD 2: XOR VISUALIZATION
 * ============================================================================
 *
 * Array: [1, 2, 4, 5, 6]  (n = 6, missing = 3)
 *
 * XOR all array elements:  1 ^ 2 ^ 4 ^ 5 ^ 6
 * XOR all numbers 1 to n:  1 ^ 2 ^ 3 ^ 4 ^ 5 ^ 6
 *
 * Combined XOR:
 *   (1^1) ^ (2^2) ^ 3 ^ (4^4) ^ (5^5) ^ (6^6)
 *   = 0 ^ 0 ^ 3 ^ 0 ^ 0 ^ 0
 *   = 3  ← The missing number!
 *
 * WHY? Every number except 3 appears twice (cancels to 0)
 *
 * ============================================================================
 * ALGORITHM (XOR Method):
 * ============================================================================
 *
 *   xor_result = 0
 *
 *   // XOR all array elements
 *   for i = 0 to n-2:
 *       xor_result ^= arr[i]
 *
 *   // XOR all numbers from 1 to n
 *   for i = 1 to n:
 *       xor_result ^= i
 *
 *   return xor_result  // This is the missing number
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - One pass through array: O(n-1)
 * - One pass through 1 to n: O(n)
 * - Total: O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only one variable for XOR result
 * - Constant space
 *
 * ============================================================================
 * COMPARISON:
 * ============================================================================
 *
 *   Method      | Time  | Space | Overflow Risk
 *   ------------|-------|-------|---------------
 *   Sum Formula | O(n)  | O(1)  | YES (large n)
 *   XOR Method  | O(n)  | O(1)  | NO
 *   Sorting     | O(nlogn) | O(1) | NO
 *   Hash Set    | O(n)  | O(n)  | NO
 *
 *   XOR is the BEST approach!
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Missing first element (1): XOR gives 1
 * 2. Missing last element (n): XOR gives n
 * 3. n = 2, array = [2]: Missing is 1
 *
 * ============================================================================
 */

#include <stdio.h>

// Method 1: Using Sum Formula
int find_missing_sum(int arr[], int n) {
    int expected_sum = n * (n + 1) / 2;
    int actual_sum = 0;

    for (int i = 0; i < n - 1; i++) {
        actual_sum += arr[i];
    }

    return expected_sum - actual_sum;
}

// Method 2: Using XOR (Preferred - No Overflow)
int find_missing_xor(int arr[], int n) {
    int xor_result = 0;

    // XOR all array elements
    for (int i = 0; i < n - 1; i++) {
        xor_result ^= arr[i];
    }

    // XOR all numbers from 1 to n
    for (int i = 1; i <= n; i++) {
        xor_result ^= i;
    }

    return xor_result;
}

int main() {
    int arr[] = {1, 2, 4, 5, 6};
    int n = 6;  // Numbers should be from 1 to 6

    printf("Array: [1, 2, 4, 5, 6]\n");
    printf("Range: 1 to %d\n\n", n);

    printf("Missing (Sum Method): %d\n", find_missing_sum(arr, n));
    printf("Missing (XOR Method): %d\n", find_missing_xor(arr, n));

    // Test case 2
    int arr2[] = {2, 3, 4, 5};
    int n2 = 5;
    printf("\nArray: [2, 3, 4, 5]\n");
    printf("Missing: %d\n", find_missing_xor(arr2, n2));

    // Test case 3
    int arr3[] = {1, 2, 3, 4};
    int n3 = 5;
    printf("\nArray: [1, 2, 3, 4]\n");
    printf("Missing: %d\n", find_missing_xor(arr3, n3));

    return 0;
}
