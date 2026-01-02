/*
 * ============================================================================
 * PROBLEM: Rotate Array by K Positions
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: Very High
 *
 * Rotate an array to the right by k positions.
 *
 * Example:
 * Input:  [1, 2, 3, 4, 5], k = 2
 * Output: [4, 5, 1, 2, 3]
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. HANDLE K > N:
 *    - If k >= n, use k = k % n
 *    - Rotating by n positions = original array
 *
 * 2. THREE REVERSAL TRICK (OPTIMAL):
 *    - Reverse entire array
 *    - Reverse first k elements
 *    - Reverse remaining n-k elements
 *
 * 3. LEFT vs RIGHT ROTATION:
 *    - Right rotation by k = Left rotation by (n-k)
 *
 * ============================================================================
 * VISUAL UNDERSTANDING - RIGHT ROTATION BY K=2:
 * ============================================================================
 *
 * Original: [1, 2, 3, 4, 5]
 *
 * What happens:
 *   Last k=2 elements move to front
 *   [4, 5] moves to front
 *   [1, 2, 3] shifts right
 *
 * Result: [4, 5, 1, 2, 3]
 *
 * ============================================================================
 * METHOD 1: EXTRA ARRAY (Simple but O(n) space)
 * ============================================================================
 *
 * [1, 2, 3, 4, 5], k=2
 *
 * New position of element at index i = (i + k) % n
 *
 * Index 0 (val=1): new pos = (0+2) % 5 = 2
 * Index 1 (val=2): new pos = (1+2) % 5 = 3
 * Index 2 (val=3): new pos = (2+2) % 5 = 4
 * Index 3 (val=4): new pos = (3+2) % 5 = 0
 * Index 4 (val=5): new pos = (4+2) % 5 = 1
 *
 * New array: [4, 5, 1, 2, 3]
 *
 * ============================================================================
 * METHOD 2: THREE REVERSALS (OPTIMAL - O(1) space)
 * ============================================================================
 *
 * Array: [1, 2, 3, 4, 5], k=2
 *
 * Step 1: Reverse entire array
 *   [1, 2, 3, 4, 5] -> [5, 4, 3, 2, 1]
 *
 * Step 2: Reverse first k=2 elements
 *   [5, 4, 3, 2, 1] -> [4, 5, 3, 2, 1]
 *    ^--^
 *
 * Step 3: Reverse remaining n-k=3 elements
 *   [4, 5, 3, 2, 1] -> [4, 5, 1, 2, 3]
 *         ^-----^
 *
 * Result: [4, 5, 1, 2, 3] ✓
 *
 * ============================================================================
 * WHY THREE REVERSALS WORK:
 * ============================================================================
 *
 * Think of it as two parts: A and B
 *
 * Original: [A, B] where A = [1,2,3] and B = [4,5]
 * Goal:     [B, A]
 *
 * Step 1: Reverse all    = [B^r, A^r]  (reversed B, reversed A)
 * Step 2: Reverse B^r    = [B, A^r]
 * Step 3: Reverse A^r    = [B, A]      ← Goal achieved!
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   k = k % n  // Handle k > n
 *   if (k == 0) return
 *
 *   reverse(arr, 0, n-1)      // Reverse all
 *   reverse(arr, 0, k-1)      // Reverse first k
 *   reverse(arr, k, n-1)      // Reverse rest
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Three reversals, each O(n) at most
 * - Total: O(n) + O(k) + O(n-k) = O(n)
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only use swap variables
 * - In-place modification
 *
 * ============================================================================
 * LEFT ROTATION:
 * ============================================================================
 *
 * To rotate LEFT by k, simply reverse in different order:
 *   1. Reverse first k elements
 *   2. Reverse remaining n-k elements
 *   3. Reverse entire array
 *
 * Or: Right rotate by (n-k)
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. k = 0: No rotation needed
 * 2. k = n: Full rotation = no change
 * 3. k > n: Use k % n
 * 4. n = 1: Single element, no change
 *
 * ============================================================================
 */

#include <stdio.h>

void reverse(int arr[], int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

void rotate_right(int arr[], int n, int k) {
    k = k % n;  // Handle k > n
    if (k == 0) return;

    reverse(arr, 0, n - 1);      // Reverse all
    reverse(arr, 0, k - 1);      // Reverse first k
    reverse(arr, k, n - 1);      // Reverse rest
}

void rotate_left(int arr[], int n, int k) {
    k = k % n;  // Handle k > n
    if (k == 0) return;

    reverse(arr, 0, k - 1);      // Reverse first k
    reverse(arr, k, n - 1);      // Reverse rest
    reverse(arr, 0, n - 1);      // Reverse all
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
    int arr1[] = {1, 2, 3, 4, 5};
    int n1 = 5;

    printf("Original:       ");
    print_array(arr1, n1);

    rotate_right(arr1, n1, 2);
    printf("\nRight rotate 2: ");
    print_array(arr1, n1);

    // Reset array
    int arr2[] = {1, 2, 3, 4, 5};
    rotate_left(arr2, 5, 2);
    printf("\nLeft rotate 2:  ");
    print_array(arr2, 5);

    // Edge case: k > n
    int arr3[] = {1, 2, 3, 4, 5};
    rotate_right(arr3, 5, 7);  // 7 % 5 = 2
    printf("\n\nRight rotate 7 (same as 2): ");
    print_array(arr3, 5);

    return 0;
}
