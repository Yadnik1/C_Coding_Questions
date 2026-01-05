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
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Can you reverse without using a temp variable?"
 * A1: Yes, using XOR swap (but not recommended):
 *
 *     arr[left] ^= arr[right];
 *     arr[right] ^= arr[left];
 *     arr[left] ^= arr[right];
 *
 *     Problem: Fails if left == right (same memory location)!
 *     Stick with temp variable - it's clearer and safer.
 *
 * -------------------------------------------------------------------------
 * Q2: "What's the difference between reversing array vs linked list?"
 * A2: Array reversal is simpler:
 *
 *     - Array: O(1) random access, swap elements directly
 *     - Linked list: O(n) access, need to change pointers
 *
 * -------------------------------------------------------------------------
 * Q3: "How would you reverse only a portion of the array?"
 * A3: Same algorithm, just change the start and end indices:
 *
 *     void reverse_portion(int arr[], int start, int end);
 *
 *     This is used in array rotation (three reversal trick).
 *
 * -------------------------------------------------------------------------
 * Q4: "Can you reverse recursively?"
 * A4: Yes, but iterative is preferred (no stack overhead):
 *
 *     void reverse_recursive(int arr[], int left, int right) {
 *         if (left >= right) return;
 *         swap(arr[left], arr[right]);
 *         reverse_recursive(arr, left + 1, right - 1);
 *     }
 *
 *     O(n/2) stack space - not ideal for embedded systems.
 *
 * ============================================================================
 */

#include <stdio.h>

void reverse_array(int arr[], int n) {
    // Initialize left pointer at the beginning of array
    // Say: "I'll start with left pointer at index 0"
    int left = 0;

    // Initialize right pointer at the end of array
    // Say: "And right pointer at the last index, which is n minus 1"
    int right = n - 1;

    // Keep swapping until pointers meet or cross
    // Say: "I'll keep looping while left is less than right"
    while (left < right) {
        // Store the left element in temporary variable
        // Say: "First, I save the left element in a temp variable"
        int temp = arr[left];

        // Copy right element to left position
        // Say: "Then I copy the right element to the left position"
        arr[left] = arr[right];

        // Copy temp (original left) to right position
        // Say: "And copy the saved left element to the right position"
        arr[right] = temp;

        // Move left pointer one step right
        // Say: "Now I move left pointer forward"
        left++;

        // Move right pointer one step left
        // Say: "And move right pointer backward"
        right--;
    }
    // Say: "When pointers meet or cross, we're done - array is reversed"
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
    int n1 = sizeof(arr1) / sizeof(arr1[0]);

    printf("Original: ");
    print_array(arr1, n1);

    reverse_array(arr1, n1);

    printf("Reversed: ");
    print_array(arr1, n1);

    // Test with even length
    int arr2[] = {1, 2, 3, 4};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    printf("\nOriginal: ");
    print_array(arr2, n2);

    reverse_array(arr2, n2);

    printf("Reversed: ");
    print_array(arr2, n2);

    return 0;
}
