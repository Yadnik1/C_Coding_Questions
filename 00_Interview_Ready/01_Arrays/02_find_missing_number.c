/*
 * ============================================================================
 * PROBLEM: Find Missing Number
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given an array containing n distinct numbers from 0 to n (inclusive),
 * find the one number that is missing from the sequence.
 * The array has n elements but the range is 0 to n, so exactly one is missing.
 *
 * EXAMPLES:
 * - Input: [0, 1, 2, 4, 5, 6]  ->  Output: 3 (missing from 0-6)
 * - Input: [3, 0, 1]          ->  Output: 2 (missing from 0-3)
 * - Input: [0]                ->  Output: 1 (missing from 0-1)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Tests mathematical thinking (sum formula vs brute force)
 * - Shows awareness of XOR alternative for overflow prevention
 * - Common in data integrity and validation scenarios
 * - Demonstrates O(n) thinking over O(n log n) sorting approach
 *
 * KEY CONCEPT:
 * Mathematical Sum Formula - Expected sum of 0 to n is n*(n+1)/2.
 * Subtract actual sum of array elements from expected sum.
 * The difference is the missing number.
 *
 * VISUAL:
 *     Array: [0, 1, 3, 4]  (n=4 elements, range 0-4)
 *
 *     Expected sum (0+1+2+3+4) = 4*5/2 = 10
 *     Actual sum   (0+1+3+4)   = 8
 *     Missing = 10 - 8 = 2
 *
 *     Alternative XOR method (no overflow risk):
 *     XOR all numbers 0 to n, then XOR with all array elements
 *     Result = missing number (pairs cancel out)
 *
 * ============================================================================
 */

// Find missing number in array [0, n] with one missing
// Time: O(n), Space: O(1)

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

int find_missing(int arr[], int n) {
    // Say: "I use math formula for sum of 0 to n"
    // Say: "Formula is n*(n+1)/2 for sum from 1 to n"
    int expected_sum = n * (n + 1) / 2;

    // Say: "Calculate actual sum of array elements"
    int actual_sum = 0;
    for (int i = 0; i < n; i++) {
        actual_sum += arr[i];  // Say: "Add each element"
    }

    // Say: "The difference is the missing number"
    return expected_sum - actual_sum;
}

int main() {
    int arr[] = {0, 1, 2, 4, 5, 6};  // Missing: 3
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array: ");
    print_array(arr, n);

    printf("Missing number: %d\n", find_missing(arr, n));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use the mathematical formula for sum of 0 to n.
 Expected sum = n*(n+1)/2
 Then I calculate actual sum by iterating through array.
 The difference gives us the missing number.

 Why this approach vs XOR?
 - Both are O(n) time, O(1) space
 - Sum formula is more intuitive to explain
 - XOR is equally valid: XOR all numbers 0 to n, then XOR with array

 Edge case to mention: Integer overflow for large n
 - For very large arrays, use XOR method instead"
*/
