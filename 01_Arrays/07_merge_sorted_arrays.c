/*
 * ============================================================================
 * PROBLEM: Merge Two Sorted Arrays
 * ============================================================================
 *
 * DIFFICULTY: Easy-Medium | TIME: 10 mins | FREQUENCY: Very High
 *
 * Merge two sorted arrays into one sorted array.
 *
 * Example:
 * Input:  arr1 = [1, 3, 5], arr2 = [2, 4, 6]
 * Output: [1, 2, 3, 4, 5, 6]
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. TWO-POINTER TECHNIQUE:
 *    - One pointer for each array
 *    - Compare elements, take smaller one
 *    - Advance pointer of taken element
 *
 * 2. HANDLE REMAINING ELEMENTS:
 *    - One array may finish before other
 *    - Copy remaining elements directly
 *
 * 3. THIS IS BASIS FOR MERGE SORT:
 *    - Merge step in merge sort uses this technique
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * arr1 = [1, 3, 5]    arr2 = [2, 4, 6]
 *         ^                   ^
 *         i=0                 j=0
 *
 * Step 1: Compare arr1[0]=1 vs arr2[0]=2
 *   1 < 2, take 1, advance i
 *   result = [1]
 *
 *   [1, 3, 5]    [2, 4, 6]
 *       ^         ^
 *       i=1       j=0
 *
 * Step 2: Compare arr1[1]=3 vs arr2[0]=2
 *   3 > 2, take 2, advance j
 *   result = [1, 2]
 *
 *   [1, 3, 5]    [2, 4, 6]
 *       ^            ^
 *       i=1          j=1
 *
 * Step 3: Compare arr1[1]=3 vs arr2[1]=4
 *   3 < 4, take 3, advance i
 *   result = [1, 2, 3]
 *
 *   [1, 3, 5]    [2, 4, 6]
 *          ^         ^
 *          i=2       j=1
 *
 * Step 4: Compare arr1[2]=5 vs arr2[1]=4
 *   5 > 4, take 4, advance j
 *   result = [1, 2, 3, 4]
 *
 * Step 5: Compare arr1[2]=5 vs arr2[2]=6
 *   5 < 6, take 5, advance i
 *   result = [1, 2, 3, 4, 5]
 *
 * Step 6: arr1 exhausted, copy remaining arr2
 *   result = [1, 2, 3, 4, 5, 6]
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   i = 0, j = 0, k = 0
 *
 *   while (i < n1 AND j < n2):
 *       if arr1[i] <= arr2[j]:
 *           result[k++] = arr1[i++]
 *       else:
 *           result[k++] = arr2[j++]
 *
 *   // Copy remaining from arr1
 *   while (i < n1):
 *       result[k++] = arr1[i++]
 *
 *   // Copy remaining from arr2
 *   while (j < n2):
 *       result[k++] = arr2[j++]
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n + m)
 * ============================================================================
 * - n = length of arr1, m = length of arr2
 * - Each element visited exactly once
 * - Linear time
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(n + m)
 * ============================================================================
 * - Need new array to store result
 * - Size = n + m
 *
 * ============================================================================
 * IN-PLACE VARIATION (O(1) space):
 * ============================================================================
 *
 * If arr1 has extra space at end:
 *
 * arr1 = [1, 3, 5, _, _, _]  (size = n1 + n2)
 * arr2 = [2, 4, 6]
 *
 * ============================================================================
 * WHY START FROM THE END? (THE KEY INSIGHT!)
 * ============================================================================
 *
 * PROBLEM WITH STARTING FROM BEGINNING:
 *
 *   arr1 = [1, 3, 5, _, _, _]    arr2 = [2, 4, 6]
 *           ^                            ^
 *           p1                           p2
 *           ^
 *           p (write position)
 *
 *   Step 1: arr1[0]=1 < arr2[0]=2, so write 1 at position 0
 *           arr1[0] = 1  (OK, same value)
 *           Move p1 to 1, p to 1
 *
 *   Step 2: arr1[1]=3 > arr2[0]=2, so write 2 at position 1
 *           arr1[1] = 2  *** OVERWRITES 3! DATA LOST! ***
 *
 * SOLUTION: START FROM THE END!
 *
 *   The "extra space" is at the END of arr1.
 *   If we fill from the END, we write into empty space first.
 *   By the time we reach the original data, we've already read it!
 *
 * ============================================================================
 * STEP-BY-STEP IN-PLACE MERGE VISUALIZATION:
 * ============================================================================
 *
 * INITIAL STATE:
 *   arr1 = [1, 3, 5, _, _, _]    arr2 = [2, 4, 6]
 *           0  1  2  3  4  5            0  1  2
 *
 *   p1 = 2 (points to 5, last real element in arr1)
 *   p2 = 2 (points to 6, last element in arr2)
 *   p  = 5 (points to last position in arr1, where we'll write)
 *
 *   [1, 3, 5, _, _, _]    [2, 4, 6]
 *          ^p1       ^p         ^p2
 *
 * -------------------------------------------------------------------------
 * STEP 1: Compare arr1[p1]=5 vs arr2[p2]=6
 *         6 > 5, so copy 6 to arr1[p]
 *
 *   arr1[5] = arr2[2] = 6
 *   p2-- (now 1), p-- (now 4)
 *
 *   [1, 3, 5, _, _, 6]    [2, 4, 6]
 *          ^p1    ^p          ^p2
 *
 * -------------------------------------------------------------------------
 * STEP 2: Compare arr1[p1]=5 vs arr2[p2]=4
 *         5 > 4, so copy 5 to arr1[p]
 *
 *   arr1[4] = arr1[2] = 5
 *   p1-- (now 1), p-- (now 3)
 *
 *   [1, 3, 5, _, 5, 6]    [2, 4, 6]
 *       ^p1   ^p              ^p2
 *
 *   Note: arr1[2] still has 5, but p1 moved past it. It's "logically consumed"
 *
 * -------------------------------------------------------------------------
 * STEP 3: Compare arr1[p1]=3 vs arr2[p2]=4
 *         4 > 3, so copy 4 to arr1[p]
 *
 *   arr1[3] = arr2[1] = 4
 *   p2-- (now 0), p-- (now 2)
 *
 *   [1, 3, 5, 4, 5, 6]    [2, 4, 6]
 *       ^p1^p              ^p2
 *
 * -------------------------------------------------------------------------
 * STEP 4: Compare arr1[p1]=3 vs arr2[p2]=2
 *         3 > 2, so copy 3 to arr1[p]
 *
 *   arr1[2] = arr1[1] = 3
 *   p1-- (now 0), p-- (now 1)
 *
 *   [1, 3, 3, 4, 5, 6]    [2, 4, 6]
 *    ^p1^p                 ^p2
 *
 * -------------------------------------------------------------------------
 * STEP 5: Compare arr1[p1]=1 vs arr2[p2]=2
 *         2 > 1, so copy 2 to arr1[p]
 *
 *   arr1[1] = arr2[0] = 2
 *   p2-- (now -1), p-- (now 0)
 *
 *   [1, 2, 3, 4, 5, 6]    [2, 4, 6]
 *    ^p1                   (p2 done)
 *    ^p
 *
 * -------------------------------------------------------------------------
 * STEP 6: p2 < 0, so arr2 is exhausted
 *         arr1 elements are already in place!
 *         (No need to copy remaining arr1 elements)
 *
 * FINAL RESULT:
 *   [1, 2, 3, 4, 5, 6]
 *
 * ============================================================================
 * WHY THIS WORKS - THE MATHEMATICAL GUARANTEE:
 * ============================================================================
 *
 * Key observation: When we write to position p, we have p1 + p2 + 2 elements
 * left to place (p1+1 from arr1, p2+1 from arr2).
 *
 * Since p = p1 + p2 + 1 (initially, and maintained throughout):
 *   - Position p is ALWAYS >= p1
 *   - We NEVER overwrite an unread element from arr1!
 *
 * Proof by example:
 *   - Initially: p=5, p1=2, p2=2 → p > p1 ✓
 *   - After step 1: p=4, p1=2, p2=1 → p > p1 ✓
 *   - After step 2: p=3, p1=1, p2=1 → p > p1 ✓
 *   - After step 3: p=2, p1=1, p2=0 → p > p1 ✓
 *   - After step 4: p=1, p1=0, p2=0 → p > p1 ✓
 *   - After step 5: p=0, p1=0, p2=-1 → p >= p1 ✓
 *
 * ============================================================================
 * ALGORITHM PSEUDOCODE:
 * ============================================================================
 *
 *   p1 = n1-1, p2 = n2-1, p = n1+n2-1
 *
 *   while (p1 >= 0 AND p2 >= 0):
 *       if arr1[p1] > arr2[p2]:
 *           arr1[p--] = arr1[p1--]
 *       else:
 *           arr1[p--] = arr2[p2--]
 *
 *   // Only need to copy remaining arr2 elements
 *   // (arr1 elements are already in correct position)
 *   while (p2 >= 0):
 *       arr1[p--] = arr2[p2--]
 *
 * This is O(1) extra space!
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Empty arr1: Result = arr2
 * 2. Empty arr2: Result = arr1
 * 3. All arr1 < all arr2: Simple concatenation
 * 4. All arr2 < all arr1: Reversed concatenation
 * 5. Duplicate elements: Handle normally
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Standard merge with extra space
int* merge_arrays(int arr1[], int n1, int arr2[], int n2) {
    int *result = (int *)malloc((n1 + n2) * sizeof(int));
    int i = 0, j = 0, k = 0;

    // Compare and merge
    while (i < n1 && j < n2) {
        if (arr1[i] <= arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }

    // Copy remaining from arr1
    while (i < n1) {
        result[k++] = arr1[i++];
    }

    // Copy remaining from arr2
    while (j < n2) {
        result[k++] = arr2[j++];
    }

    return result;
}

// In-place merge (arr1 has extra space)
void merge_inplace(int arr1[], int n1, int arr2[], int n2) {
    // arr1 has size n1 + n2, first n1 elements are actual data
    int p1 = n1 - 1;       // Last element of arr1
    int p2 = n2 - 1;       // Last element of arr2
    int p = n1 + n2 - 1;   // Last position in result

    // Start from end, fill largest elements first
    while (p1 >= 0 && p2 >= 0) {
        if (arr1[p1] > arr2[p2]) {
            arr1[p--] = arr1[p1--];
        } else {
            arr1[p--] = arr2[p2--];
        }
    }

    // Copy remaining from arr2 (arr1 elements already in place)
    while (p2 >= 0) {
        arr1[p--] = arr2[p2--];
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
    int arr1[] = {1, 3, 5};
    int arr2[] = {2, 4, 6};
    int n1 = 3, n2 = 3;

    printf("Array 1: ");
    print_array(arr1, n1);
    printf("\nArray 2: ");
    print_array(arr2, n2);

    int *merged = merge_arrays(arr1, n1, arr2, n2);
    printf("\nMerged:  ");
    print_array(merged, n1 + n2);
    free(merged);

    // In-place merge demo
    printf("\n\n=== In-Place Merge ===\n");
    int arr3[6] = {1, 3, 5, 0, 0, 0};  // Extra space for 3 more elements
    int arr4[] = {2, 4, 6};

    printf("Array 1 (with space): ");
    print_array(arr3, 3);
    printf("\nArray 2: ");
    print_array(arr4, 3);

    merge_inplace(arr3, 3, arr4, 3);
    printf("\nMerged in-place: ");
    print_array(arr3, 6);

    return 0;
}
