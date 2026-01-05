/*
 * ============================================================================
 * PROBLEM: Remove Duplicates from Sorted Array
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: HIGH
 *
 * Pattern: SAME DIRECTION (Fast & Slow)
 *
 * Remove duplicates IN-PLACE and return new length.
 *
 * ============================================================================
 * VISUALIZATION: [1, 1, 2, 2, 2, 3]
 * ============================================================================
 *
 *   [1, 1, 2, 2, 2, 3]
 *    ^  ^
 *   slow fast
 *
 *   fast=1: arr[1]=1 == arr[0]=1, skip
 *
 *   fast=2: arr[2]=2 != arr[0]=1
 *           arr[1] = 2, slow=2
 *           [1, 2, 2, 2, 2, 3]
 *
 *   fast=3,4: skip (duplicates of 2)
 *
 *   fast=5: arr[5]=3 != arr[1]=2
 *           arr[2] = 3, slow=3
 *           [1, 2, 3, 2, 2, 3]
 *
 *   Return slow=3, unique elements: [1, 2, 3]
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why compare with arr[slow-1] instead of arr[fast-1]?"
 * A1: We need to compare with the last UNIQUE element, not the last scanned.
 *     arr[slow-1] is the most recently placed unique element. arr[fast-1]
 *     could be a duplicate we skipped!
 *
 * -------------------------------------------------------------------------
 * Q2: "What about allowing at most 2 duplicates?"
 * A2: Same pattern! Compare with arr[slow-2] instead of arr[slow-1].
 *     Generalize: for at most k duplicates, compare with arr[slow-k].
 *
 * -------------------------------------------------------------------------
 * Q3: "Why does this only work on sorted arrays?"
 * A3: Because duplicates are adjacent in sorted arrays. For unsorted,
 *     you'd need a hash set to track seen elements - O(n) extra space.
 *     Or sort first (O(n log n)) then use this algorithm.
 *
 * -------------------------------------------------------------------------
 * Q4: "What happens to elements after new length?"
 * A4: They're garbage - original values or overwritten duplicates.
 *     The problem says to return new length and only consider elements
 *     before that. Don't access or rely on elements after new length!
 *
 * ============================================================================
 */

#include <stdio.h>

int removeDuplicates(int arr[], int n) {
    if (n == 0) return 0;

    int slow = 1;  // Position for next unique element

    for (int fast = 1; fast < n; fast++) {
        if (arr[fast] != arr[slow - 1]) {
            arr[slow] = arr[fast];
            slow++;
        }
    }

    return slow;  // New length
}

void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    printf("=== Remove Duplicates (Two Pointer) ===\n\n");

    int arr1[] = {1, 1, 2, 2, 2, 3, 4, 4};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("Before: "); printArray(arr1, n1);
    int newLen1 = removeDuplicates(arr1, n1);
    printf("After:  "); printArray(arr1, newLen1);
    printf("New length: %d\n\n", newLen1);

    int arr2[] = {1, 1, 1};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Before: "); printArray(arr2, n2);
    int newLen2 = removeDuplicates(arr2, n2);
    printf("After:  "); printArray(arr2, newLen2);
    printf("New length: %d\n", newLen2);

    return 0;
}
