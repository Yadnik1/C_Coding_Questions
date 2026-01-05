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
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why does Boyer-Moore work?"
 * A1: Majority element appears > n/2 times. When we cancel different elements,
 *     majority will always have at least one "survivor" because it has more
 *     than half the total count.
 *
 * -------------------------------------------------------------------------
 * Q2: "Do we always need the verification phase?"
 * A2: Only if majority existence isn't guaranteed!
 *     - Problem says "guaranteed majority exists": Skip verification
 *     - Problem says "find IF exists": Need verification
 *
 * -------------------------------------------------------------------------
 * Q3: "What about finding element appearing n/3 times?"
 * A3: Use Boyer-Moore with TWO candidates!
 *     At most 2 elements can appear > n/3 times.
 *     Keep count1, count2, candidate1, candidate2.
 *
 * -------------------------------------------------------------------------
 * Q4: "Can you use hash map instead?"
 * A4: Yes, but O(n) space vs Boyer-Moore's O(1) space.
 *     Hash map: Count occurrences, find element with count > n/2.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

// Boyer-Moore Voting Algorithm
int find_majority(int arr[], int n) {
    // Phase 1: Find candidate
    // Say: "I'll use Boyer-Moore voting algorithm to find the majority element"

    // Initialize candidate to zero
    // Say: "I start with candidate set to zero"
    int candidate = 0;

    // Initialize vote count to zero
    // Say: "And count set to zero"
    int count = 0;

    // Iterate through all elements
    // Say: "I scan through the array treating it like voting"
    for (int i = 0; i < n; i++) {
        // Check if count is zero
        // Say: "Whenever count reaches zero, I pick a new candidate"
        if (count == 0) {
            // Set new candidate to current element
            // Say: "I set the candidate to the current element"
            candidate = arr[i];

            // Initialize count to 1
            // Say: "And reset count to 1"
            count = 1;
        } else if (arr[i] == candidate) {
            // Current element matches candidate
            // Say: "If the element matches the candidate, I increment the count"
            count++;
        } else {
            // Current element is different
            // Say: "If it's different, I decrement the count, canceling out one vote"
            count--;
        }
    }

    // Return the candidate as majority element
    // Say: "The remaining candidate is the majority element"
    return candidate;
}

// With verification (when majority not guaranteed)
int find_majority_verified(int arr[], int n, bool *found) {
    // Phase 1: Find candidate
    // Say: "First, I find the candidate using the voting algorithm"
    int candidate = find_majority(arr, n);

    // Phase 2: Verify
    // Say: "Then I verify if this candidate actually appears more than n over 2 times"

    // Initialize counter for verification
    // Say: "I'll count how many times the candidate appears"
    int count = 0;

    // Count occurrences of candidate
    // Say: "I scan through the array counting occurrences of the candidate"
    for (int i = 0; i < n; i++) {
        // Check if element equals candidate
        // Say: "Each time I find the candidate, I increment the count"
        if (arr[i] == candidate) {
            count++;
        }
    }

    // Verify if count is greater than n/2
    // Say: "Finally, I check if the count is greater than n divided by 2"
    if (count > n / 2) {
        // Majority element found
        // Say: "If yes, I've found the majority element"
        *found = true;
        return candidate;
    } else {
        // No majority element
        // Say: "Otherwise, there's no majority element in the array"
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
    int n1 = sizeof(arr1) / sizeof(arr1[0]);

    printf("Array: ");
    print_array(arr1, n1);
    printf("\nMajority element: %d\n", find_majority(arr1, n1));

    int arr2[] = {3, 2, 3};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    printf("\nArray: ");
    print_array(arr2, n2);
    printf("\nMajority element: %d\n", find_majority(arr2, n2));

    // Test with no majority
    int arr3[] = {1, 2, 3, 4};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
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
