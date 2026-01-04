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
 * Start from the end, fill largest elements first!
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Standard merge with extra space
int* merge_arrays(int arr1[], int n1, int arr2[], int n2) {
    // Allocate memory for the result array
    // Say: "I'll create a result array of size n1 plus n2 to hold all elements"
    int *result = (int *)malloc((n1 + n2) * sizeof(int));

    // Initialize three pointers: i for arr1, j for arr2, k for result
    // Say: "I use three pointers: i for first array, j for second array, and k for result"
    int i = 0, j = 0, k = 0;

    // Compare and merge while both arrays have elements
    // Say: "I compare elements from both arrays and pick the smaller one each time"
    while (i < n1 && j < n2) {
        // Check if element in arr1 is smaller or equal
        // Say: "If the element in array 1 is smaller or equal"
        if (arr1[i] <= arr2[j]) {
            // Take element from arr1 and advance its pointer
            // Say: "I take it from array 1 and move that pointer forward"
            result[k++] = arr1[i++];
        } else {
            // Take element from arr2 and advance its pointer
            // Say: "Otherwise, I take from array 2 and move that pointer forward"
            result[k++] = arr2[j++];
        }
    }

    // Copy remaining from arr1 if any
    // Say: "After one array is exhausted, I copy remaining elements from array 1 if any"
    while (i < n1) {
        // Copy remaining element from arr1
        // Say: "I copy each remaining element from array 1"
        result[k++] = arr1[i++];
    }

    // Copy remaining from arr2 if any
    // Say: "Then I copy any remaining elements from array 2"
    while (j < n2) {
        // Copy remaining element from arr2
        // Say: "I copy each remaining element from array 2"
        result[k++] = arr2[j++];
    }

    // Return the merged result array
    // Say: "Finally, I return the merged sorted array"
    return result;
}

// In-place merge (arr1 has extra space)
void merge_inplace(int arr1[], int n1, int arr2[], int n2) {
    // arr1 has size n1 + n2, first n1 elements are actual data
    // Say: "For in-place merge, array 1 has extra space at the end to hold array 2"

    // Initialize pointer to last element of arr1's data
    // Say: "I start p1 at the last actual element of array 1"
    int p1 = n1 - 1;

    // Initialize pointer to last element of arr2
    // Say: "p2 points to the last element of array 2"
    int p2 = n2 - 1;

    // Initialize pointer to last position in merged result
    // Say: "And p points to the last position where I'll write the result"
    int p = n1 + n2 - 1;

    // Start from end, fill largest elements first
    // Say: "The key insight is to fill from the end, comparing largest elements"
    while (p1 >= 0 && p2 >= 0) {
        // Compare elements and take the larger one
        // Say: "I compare elements from both arrays and take the larger one"
        if (arr1[p1] > arr2[p2]) {
            // Take larger element from arr1
            // Say: "If array 1's element is larger, I place it at position p"
            arr1[p--] = arr1[p1--];
        } else {
            // Take larger element from arr2
            // Say: "Otherwise, I place array 2's element at position p"
            arr1[p--] = arr2[p2--];
        }
    }

    // Copy remaining from arr2 (arr1 elements already in place)
    // Say: "Finally, I copy any remaining elements from array 2"
    while (p2 >= 0) {
        // Copy remaining element from arr2
        // Say: "I copy each remaining element from array 2 to its position"
        arr1[p--] = arr2[p2--];
    }
    // Say: "Array 1 elements are already in correct position, so no need to copy them"
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
