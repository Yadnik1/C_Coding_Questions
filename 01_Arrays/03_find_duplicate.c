/*
 * ============================================================================
 * PROBLEM: Find Duplicate Element in Array
 * ============================================================================
 *
 * DIFFICULTY: Easy-Medium | TIME: 10 mins | FREQUENCY: Very High
 *
 * Given an array of n+1 integers where each integer is in range [1, n],
 * find the duplicate number. Only one number is repeated.
 *
 * Example:
 * Input:  [1, 3, 4, 2, 2]
 * Output: 2
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. MULTIPLE APPROACHES:
 *    - Brute Force: O(n^2) - nested loops
 *    - Sorting: O(n log n) - sort and check adjacent
 *    - Hash Set: O(n) time, O(n) space
 *    - XOR: Works only if exactly one duplicate
 *    - Floyd's Cycle: O(n) time, O(1) space - OPTIMAL
 *
 * 2. FLOYD'S CYCLE DETECTION (Tortoise & Hare):
 *    - Treat array as linked list where arr[i] points to arr[arr[i]]
 *    - Duplicate creates a cycle
 *    - Find cycle using slow/fast pointers
 *
 * ============================================================================
 * METHOD 1: SORTING VISUALIZATION
 * ============================================================================
 *
 * Array: [1, 3, 4, 2, 2]
 *
 * After sorting: [1, 2, 2, 3, 4]
 *                    ^  ^
 *                 Adjacent duplicates!
 *
 * Scan for arr[i] == arr[i+1] → Found 2
 *
 * ============================================================================
 * METHOD 2: XOR VISUALIZATION (When range is 1 to n-1)
 * ============================================================================
 *
 * Array: [1, 3, 2, 2] (n=4, range 1 to 3)
 *
 * XOR all elements:    1 ^ 3 ^ 2 ^ 2
 * XOR range 1 to n-1:  1 ^ 2 ^ 3
 *
 * Combined: (1^1) ^ (2^2^2) ^ (3^3) = 0 ^ 2 ^ 0 = 2
 *
 * ============================================================================
 * METHOD 3: FLOYD'S CYCLE DETECTION (OPTIMAL)
 * ============================================================================
 *
 * Array: [1, 3, 4, 2, 2]
 * Index:  0  1  2  3  4
 *
 * Treat as linked list: index -> arr[index]
 *
 *   0 -> arr[0]=1 -> arr[1]=3 -> arr[3]=2 -> arr[2]=4 -> arr[4]=2
 *                                            ^                 |
 *                                            |                 |
 *                                            +-----------------+
 *                                            CYCLE! (2 is duplicate)
 *
 * Phase 1: Find intersection point
 *   slow = arr[slow]      (moves 1 step)
 *   fast = arr[arr[fast]] (moves 2 steps)
 *   They will meet inside the cycle
 *
 * Phase 2: Find cycle entrance (duplicate)
 *   Reset slow to start
 *   Move both one step at a time
 *   Meeting point = duplicate
 *
 * ============================================================================
 * ALGORITHM (Floyd's Method):
 * ============================================================================
 *
 *   // Phase 1: Find intersection
 *   slow = arr[0]
 *   fast = arr[0]
 *   do:
 *       slow = arr[slow]
 *       fast = arr[arr[fast]]
 *   while (slow != fast)
 *
 *   // Phase 2: Find entrance
 *   slow = arr[0]
 *   while (slow != fast):
 *       slow = arr[slow]
 *       fast = arr[fast]
 *
 *   return slow  // The duplicate
 *
 * ============================================================================
 * TIME COMPLEXITY:
 * ============================================================================
 *
 *   Method          | Time       | Space
 *   ----------------|------------|-------
 *   Brute Force     | O(n^2)     | O(1)
 *   Sorting         | O(n log n) | O(1)
 *   Hash Set        | O(n)       | O(n)
 *   Floyd's Cycle   | O(n)       | O(1)  <- BEST
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1) for Floyd's method
 * ============================================================================
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Duplicate at start: [2, 1, 2] → 2
 * 2. Duplicate at end: [1, 2, 3, 3] → 3
 * 3. All same: [2, 2, 2] → 2
 * 4. Length 2: [1, 1] → 1
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Method 1: Sorting approach
int find_duplicate_sort(int arr[], int n) {
    // Create copy to avoid modifying original
    int *temp = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) temp[i] = arr[i];

    // Simple bubble sort
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                int t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }

    // Find adjacent duplicate
    for (int i = 0; i < n - 1; i++) {
        if (temp[i] == temp[i + 1]) {
            int result = temp[i];
            free(temp);
            return result;
        }
    }

    free(temp);
    return -1;
}

// Method 2: Floyd's Cycle Detection (OPTIMAL)
int find_duplicate_floyd(int arr[], int n) {
    // Phase 1: Find intersection point in cycle
    int slow = arr[0];
    int fast = arr[0];

    do {
        slow = arr[slow];         // Move 1 step
        fast = arr[arr[fast]];    // Move 2 steps
    } while (slow != fast);

    // Phase 2: Find entrance to cycle (duplicate)
    slow = arr[0];
    while (slow != fast) {
        slow = arr[slow];
        fast = arr[fast];
    }

    return slow;
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
    int arr1[] = {1, 3, 4, 2, 2};
    int n1 = 5;

    printf("Array: ");
    print_array(arr1, n1);
    printf("\nDuplicate (Floyd): %d\n", find_duplicate_floyd(arr1, n1));

    int arr2[] = {3, 1, 3, 4, 2};
    int n2 = 5;

    printf("\nArray: ");
    print_array(arr2, n2);
    printf("\nDuplicate (Floyd): %d\n", find_duplicate_floyd(arr2, n2));

    int arr3[] = {1, 1};
    int n3 = 2;

    printf("\nArray: ");
    print_array(arr3, n3);
    printf("\nDuplicate (Floyd): %d\n", find_duplicate_floyd(arr3, n3));

    return 0;
}
