/*
 * ============================================================================
 * PROBLEM: Find Second Largest Element
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * Find the second largest element in an array without sorting.
 *
 * Example:
 * Input:  [12, 35, 1, 10, 34, 1]
 * Output: 34
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. SINGLE PASS APPROACH:
 *    - Track both largest and second largest
 *    - Update both as you traverse
 *
 * 2. INITIAL VALUES:
 *    - Use INT_MIN or first element
 *    - Handle edge cases properly
 *
 * 3. UPDATE LOGIC:
 *    - If new element > largest:
 *      second = largest, largest = new
 *    - Else if new element > second AND new != largest:
 *      second = new
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * Array: [12, 35, 1, 10, 34, 1]
 *
 * Initialize: largest = -INF, second = -INF
 *
 * Step 1: arr[0] = 12
 *   12 > -INF → second = -INF, largest = 12
 *   State: largest=12, second=-INF
 *
 * Step 2: arr[1] = 35
 *   35 > 12 → second = 12, largest = 35
 *   State: largest=35, second=12
 *
 * Step 3: arr[2] = 1
 *   1 < 35 and 1 < 12 → no change
 *   State: largest=35, second=12
 *
 * Step 4: arr[3] = 10
 *   10 < 35 but 10 < 12 → no change
 *   State: largest=35, second=12
 *
 * Step 5: arr[4] = 34
 *   34 < 35 but 34 > 12 → second = 34
 *   State: largest=35, second=34
 *
 * Step 6: arr[5] = 1
 *   1 < 35 and 1 < 34 → no change
 *   State: largest=35, second=34
 *
 * Result: Second largest = 34
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   largest = -INF
 *   second = -INF
 *
 *   for each element arr[i]:
 *       if arr[i] > largest:
 *           second = largest
 *           largest = arr[i]
 *       else if arr[i] > second AND arr[i] != largest:
 *           second = arr[i]
 *
 *   return second
 *
 * ============================================================================
 * FLOW DIAGRAM:
 * ============================================================================
 *
 *                    +-------------+
 *                    | arr[i] > L? |
 *                    +------+------+
 *                           |
 *              YES          |          NO
 *         +--------+        |        +--------+
 *         |        v        |        v        |
 *    +---------+            |           +-----------+
 *    | S = L   |            |           | arr[i]>S? |
 *    | L = arr |            |           +-----+-----+
 *    +---------+            |                 |
 *                           |     YES         |        NO
 *                           | +--------+      |   +--------+
 *                           | |        v      v   v        |
 *                           | |   +-----------+           |
 *                           | |   | S = arr[i]|           |
 *                           | |   +-----------+           |
 *                           | |                           |
 *                           +-----------------------------+
 *
 *   L = largest, S = second
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass through array
 * - Constant time operations per element
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only two variables: largest, second
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Array size < 2: No second largest exists
 * 2. All elements same: No second largest
 * 3. All elements same except one: That one is either L or S
 * 4. Negative numbers: Handle with INT_MIN
 * 5. Duplicates of largest: Second should be different
 *
 * ============================================================================
 * COMPARISON WITH OTHER METHODS:
 * ============================================================================
 *
 *   Method              | Time      | Space | Notes
 *   --------------------|-----------|-------|------------------
 *   Sorting             | O(n log n)| O(1)  | Simple but slow
 *   Two passes          | O(2n)     | O(1)  | Find max, then find 2nd
 *   Single pass         | O(n)      | O(1)  | OPTIMAL
 *   Heap (k=2)          | O(n log 2)| O(1)  | Overkill for k=2
 *
 * ============================================================================
 */

#include <stdio.h>
#include <limits.h>

int find_second_largest(int arr[], int n) {
    if (n < 2) {
        return INT_MIN;  // Not enough elements
    }

    int largest = INT_MIN;
    int second = INT_MIN;

    for (int i = 0; i < n; i++) {
        if (arr[i] > largest) {
            // New largest found, old largest becomes second
            second = largest;
            largest = arr[i];
        } else if (arr[i] > second && arr[i] != largest) {
            // New second largest (must be different from largest)
            second = arr[i];
        }
    }

    return second;
}

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]");
}

int main() {
    int arr1[] = {12, 35, 1, 10, 34, 1};
    int n1 = 6;

    printf("Array: ");
    print_array(arr1, n1);
    printf("\nSecond largest: %d\n", find_second_largest(arr1, n1));

    // Test with duplicates of largest
    int arr2[] = {10, 10, 10, 5, 3};
    int n2 = 5;

    printf("\nArray: ");
    print_array(arr2, n2);
    printf("\nSecond largest: %d\n", find_second_largest(arr2, n2));

    // Test with negative numbers
    int arr3[] = {-5, -1, -10, -3};
    int n3 = 4;

    printf("\nArray: ");
    print_array(arr3, n3);
    printf("\nSecond largest: %d\n", find_second_largest(arr3, n3));

    // Test with two elements
    int arr4[] = {5, 10};
    int n4 = 2;

    printf("\nArray: ");
    print_array(arr4, n4);
    printf("\nSecond largest: %d\n", find_second_largest(arr4, n4));

    return 0;
}
