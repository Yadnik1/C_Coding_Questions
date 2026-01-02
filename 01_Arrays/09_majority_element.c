/*
 * ============================================================================
 * PROBLEM: Find Majority Element
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: Very High
 *
 * A majority element appears more than n/2 times in the array.
 * Find the majority element (guaranteed to exist).
 *
 * Example:
 * Input:  [3, 2, 3]
 * Output: 3 (appears 2 times, n/2 = 1, so 2 > 1)
 *
 * Input:  [2, 2, 1, 1, 1, 2, 2]
 * Output: 2 (appears 4 times, n/2 = 3, so 4 > 3)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. BOYER-MOORE VOTING ALGORITHM:
 *    - Brilliant O(n) time, O(1) space solution
 *    - Treat it like an election/voting process
 *
 * 2. KEY INSIGHT:
 *    - Majority element count > n/2
 *    - If we "cancel out" different elements pairwise,
 *      majority element will remain
 *
 * 3. TWO PHASES:
 *    - Phase 1: Find candidate
 *    - Phase 2: Verify candidate (if not guaranteed)
 *
 * ============================================================================
 * BOYER-MOORE VOTING ALGORITHM VISUALIZATION:
 * ============================================================================
 *
 * Array: [2, 2, 1, 1, 1, 2, 2]
 *
 * Think of it as: Each element votes for itself
 * Different elements cancel each other out
 *
 * Initialize: candidate = ?, count = 0
 *
 * Step 1: arr[0] = 2
 *   count = 0, so candidate = 2, count = 1
 *   "2 becomes the candidate with 1 vote"
 *
 * Step 2: arr[1] = 2
 *   2 == candidate(2), count++
 *   count = 2
 *   "Another vote for 2"
 *
 * Step 3: arr[2] = 1
 *   1 != candidate(2), count--
 *   count = 1
 *   "1 cancels one vote of 2"
 *
 * Step 4: arr[3] = 1
 *   1 != candidate(2), count--
 *   count = 0
 *   "1 cancels another vote of 2"
 *
 * Step 5: arr[4] = 1
 *   count = 0, so candidate = 1, count = 1
 *   "1 becomes new candidate"
 *
 * Step 6: arr[5] = 2
 *   2 != candidate(1), count--
 *   count = 0
 *   "2 cancels vote of 1"
 *
 * Step 7: arr[6] = 2
 *   count = 0, so candidate = 2, count = 1
 *   "2 becomes final candidate"
 *
 * Result: candidate = 2 ✓
 *
 * ============================================================================
 * WHY IT WORKS:
 * ============================================================================
 *
 * Imagine a room where everyone holds up a card with their vote:
 *
 *   [2] [2] [1] [1] [1] [2] [2]
 *
 * Now, people with different votes pair up and leave:
 *
 *   [2] pairs with [1] → both leave
 *   [2] pairs with [1] → both leave
 *   Remaining: [1] [2] [2]
 *
 *   [1] pairs with [2] → both leave
 *   Remaining: [2]
 *
 * The majority survives! Because it has MORE than half.
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   // Phase 1: Find candidate
 *   candidate = 0, count = 0
 *
 *   for each element:
 *       if count == 0:
 *           candidate = element
 *           count = 1
 *       else if element == candidate:
 *           count++
 *       else:
 *           count--
 *
 *   // Phase 2: Verify (if majority not guaranteed)
 *   count = 0
 *   for each element:
 *       if element == candidate:
 *           count++
 *
 *   if count > n/2:
 *       return candidate
 *   else:
 *       return "No majority"
 *
 * ============================================================================
 * COMPARISON OF APPROACHES:
 * ============================================================================
 *
 *   Method          | Time       | Space   | Notes
 *   ----------------|------------|---------|------------------
 *   Brute Force     | O(n^2)     | O(1)    | Count each element
 *   Sorting         | O(n log n) | O(1)    | Middle element
 *   Hash Map        | O(n)       | O(n)    | Count frequencies
 *   Boyer-Moore     | O(n)       | O(1)    | OPTIMAL
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass to find candidate
 * - Optional single pass to verify
 * - Total: O(n) or O(2n) = O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only two variables: candidate and count
 * - Constant space
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Single element: That element is majority
 * 2. Two elements same: That element is majority
 * 3. All same: That element is majority
 * 4. No majority: Verify phase catches this
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Boyer-Moore Voting Algorithm
int find_majority(int arr[], int n) {
    // Phase 1: Find candidate
    int candidate = 0;
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (count == 0) {
            candidate = arr[i];
            count = 1;
        } else if (arr[i] == candidate) {
            count++;
        } else {
            count--;
        }
    }

    return candidate;
}

// With verification (when majority not guaranteed)
int find_majority_verified(int arr[], int n, bool *found) {
    // Phase 1: Find candidate
    int candidate = find_majority(arr, n);

    // Phase 2: Verify
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] == candidate) {
            count++;
        }
    }

    if (count > n / 2) {
        *found = true;
        return candidate;
    } else {
        *found = false;
        return -1;
    }
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
    int arr1[] = {2, 2, 1, 1, 1, 2, 2};
    int n1 = 7;

    printf("Array: ");
    print_array(arr1, n1);
    printf("\nMajority element: %d\n", find_majority(arr1, n1));

    int arr2[] = {3, 2, 3};
    int n2 = 3;

    printf("\nArray: ");
    print_array(arr2, n2);
    printf("\nMajority element: %d\n", find_majority(arr2, n2));

    // Test with no majority
    int arr3[] = {1, 2, 3, 4};
    int n3 = 4;
    bool found;

    printf("\nArray: ");
    print_array(arr3, n3);
    int result = find_majority_verified(arr3, n3, &found);
    if (found) {
        printf("\nMajority element: %d\n", result);
    } else {
        printf("\nNo majority element exists\n");
    }

    // Test single element
    int arr4[] = {5};
    printf("\nArray: ");
    print_array(arr4, 1);
    printf("\nMajority element: %d\n", find_majority(arr4, 1));

    return 0;
}
