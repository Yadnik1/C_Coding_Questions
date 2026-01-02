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
 * Start from END of both arrays, fill arr1 from end:
 *
 *   p1 = n1-1, p2 = n2-1, p = n1+n2-1
 *
 *   while (p1 >= 0 AND p2 >= 0):
 *       if arr1[p1] > arr2[p2]:
 *           arr1[p--] = arr1[p1--]
 *       else:
 *           arr1[p--] = arr2[p2--]
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
