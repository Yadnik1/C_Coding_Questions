/*
 * ============================================================================
 * PROBLEM: Two Sum in Sorted Array
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * Pattern: OPPOSITE ENDS (Converging Pointers)
 *
 * Given a SORTED array, find two numbers that add up to target.
 *
 * ============================================================================
 * VISUALIZATION: arr = [1, 2, 4, 6, 8, 10], target = 10
 * ============================================================================
 *
 *   Step 1: left=0, right=5
 *           sum = 1 + 10 = 11 > 10
 *           Too big! Move right--
 *
 *   Step 2: left=0, right=4
 *           sum = 1 + 8 = 9 < 10
 *           Too small! Move left++
 *
 *   Step 3: left=1, right=4
 *           sum = 2 + 8 = 10 == target
 *           FOUND! Return [1, 4]
 *
 * WHY THIS WORKS:
 *   - If sum > target → need smaller sum → move right (decrease larger)
 *   - If sum < target → need larger sum → move left (increase smaller)
 *   - Sorted property guarantees we don't miss the answer
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why doesn't this work for unsorted arrays?"
 * A1: The greedy decision (move left for bigger, right for smaller) relies on
 *     sorted order. In unsorted array, moving a pointer might skip the answer!
 *     For unsorted, use hash map O(n) time O(n) space.
 *
 * -------------------------------------------------------------------------
 * Q2: "How do you extend to Three Sum?"
 * A2: Fix one element (iterate through array), then two-pointer on remainder.
 *     Total: O(n²). Sort first, then skip duplicates for unique triplets.
 *
 * -------------------------------------------------------------------------
 * Q3: "What if there are duplicates and we need all pairs?"
 * A3: When found, don't just return - record the pair, then skip all duplicates
 *     from both ends (left++, right--) while values are equal to avoid
 *     duplicate pairs in the result.
 *
 * -------------------------------------------------------------------------
 * Q4: "Can two pointers find pairs closest to target?"
 * A4: Yes! Track minimum |sum - target| seen so far. Update best pair when
 *     closer. Move pointers same way: if sum < target, left++; else right--.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>

bool twoSum(int arr[], int n, int target, int* idx1, int* idx2) {
    int left = 0;
    int right = n - 1;

    while (left < right) {
        int sum = arr[left] + arr[right];

        if (sum == target) {
            *idx1 = left;
            *idx2 = right;
            return true;
        } else if (sum < target) {
            left++;   // Need bigger sum
        } else {
            right--;  // Need smaller sum
        }
    }

    return false;
}

int main() {
    printf("=== Two Sum in Sorted Array ===\n\n");

    int arr[] = {1, 2, 4, 6, 8, 10};
    int n = 6;

    printf("Array: [1, 2, 4, 6, 8, 10]\n\n");

    int targets[] = {10, 7, 14, 3};
    int numTargets = 4;

    for (int i = 0; i < numTargets; i++) {
        int idx1, idx2;
        printf("Target %d: ", targets[i]);
        if (twoSum(arr, n, targets[i], &idx1, &idx2)) {
            printf("Found at [%d, %d] → %d + %d = %d\n",
                   idx1, idx2, arr[idx1], arr[idx2], targets[i]);
        } else {
            printf("Not found\n");
        }
    }

    return 0;
}
