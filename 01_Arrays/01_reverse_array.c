/*
 * ============================================================================
 * PROBLEM: Reverse an Array
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. TWO-POINTER TECHNIQUE:
 *    - Place pointers at both ends
 *    - Move toward center while swapping
 *    - Stop when pointers meet/cross
 *
 * 2. IN-PLACE vs EXTRA SPACE:
 *    - In-place: Modify original array (O(1) space)
 *    - Extra space: Create new array (O(n) space)
 *    - Always prefer in-place for interviews!
 *
 * 3. SWAP OPERATION:
 *    temp = a; a = b; b = temp;
 *
 * ============================================================================
 * VISUAL UNDERSTANDING:
 * ============================================================================
 *
 * Array: [1, 2, 3, 4, 5]
 *
 * Step 1:
 *   [1, 2, 3, 4, 5]
 *    ^           ^
 *   left       right
 *   Swap 1 and 5 → [5, 2, 3, 4, 1]
 *
 * Step 2:
 *   [5, 2, 3, 4, 1]
 *       ^     ^
 *      left  right
 *   Swap 2 and 4 → [5, 4, 3, 2, 1]
 *
 * Step 3:
 *   [5, 4, 3, 2, 1]
 *          ^
 *      left=right
 *   STOP! Pointers met.
 *
 * Result: [5, 4, 3, 2, 1]
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   left = 0, right = n-1
 *   while (left < right):
 *       swap(arr[left], arr[right])
 *       left++, right--
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - We traverse n/2 elements (half the array)
 * - Each swap is O(1)
 * - O(n/2) = O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only 3 variables: left, right, temp
 * - No extra array needed
 * - Constant space - OPTIMAL!
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. Empty array (n=0): Nothing to reverse
 * 2. Single element (n=1): Already reversed
 * 3. Two elements: One swap
 * 4. Odd length: Middle element stays in place
 *
 * ============================================================================
 */

#include <stdio.h>

void reverse_array(int arr[], int n) {
    int left = 0;
    int right = n - 1;

    while (left < right) {
        // Swap elements
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;

        // Move pointers toward center
        left++;
        right--;
    }
}

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    int arr1[] = {1, 2, 3, 4, 5};
    int n1 = 5;

    printf("Original: ");
    print_array(arr1, n1);

    reverse_array(arr1, n1);

    printf("Reversed: ");
    print_array(arr1, n1);

    // Test with even length
    int arr2[] = {1, 2, 3, 4};
    int n2 = 4;

    printf("\nOriginal: ");
    print_array(arr2, n2);

    reverse_array(arr2, n2);

    printf("Reversed: ");
    print_array(arr2, n2);

    return 0;
}
