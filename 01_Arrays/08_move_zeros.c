/*
 * ============================================================================
 * PROBLEM: Move All Zeros to End
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: Very High
 *
 * Move all zeros in an array to the end while maintaining relative order
 * of non-zero elements.
 *
 * Example:
 * Input:  [0, 1, 0, 3, 12]
 * Output: [1, 3, 12, 0, 0]
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. TWO-POINTER TECHNIQUE:
 *    - One pointer (write) for position to place non-zero
 *    - One pointer (read) to scan array
 *
 * 2. IN-PLACE MODIFICATION:
 *    - Don't use extra array
 *    - Swap or overwrite elements
 *
 * 3. MAINTAIN ORDER:
 *    - Non-zero elements keep their relative order
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * Array: [0, 1, 0, 3, 12]
 *
 * Initialize: write = 0
 *
 * Step 1: read = 0, arr[0] = 0
 *   Zero found, skip
 *   write = 0
 *
 * Step 2: read = 1, arr[1] = 1
 *   Non-zero! Move to write position
 *   arr[write] = arr[read] → arr[0] = 1
 *   write++
 *
 *   Array: [1, 1, 0, 3, 12]
 *           ^
 *        write=1
 *
 * Step 3: read = 2, arr[2] = 0
 *   Zero found, skip
 *
 * Step 4: read = 3, arr[3] = 3
 *   Non-zero! Move to write position
 *   arr[write] = arr[read] → arr[1] = 3
 *   write++
 *
 *   Array: [1, 3, 0, 3, 12]
 *              ^
 *           write=2
 *
 * Step 5: read = 4, arr[4] = 12
 *   Non-zero! Move to write position
 *   arr[write] = arr[read] → arr[2] = 12
 *   write++
 *
 *   Array: [1, 3, 12, 3, 12]
 *                 ^
 *              write=3
 *
 * Step 6: Fill remaining with zeros
 *   arr[3] = 0, arr[4] = 0
 *
 *   Array: [1, 3, 12, 0, 0] ✓
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   write = 0
 *
 *   // Move non-zeros to front
 *   for read = 0 to n-1:
 *       if arr[read] != 0:
 *           arr[write++] = arr[read]
 *
 *   // Fill rest with zeros
 *   while write < n:
 *       arr[write++] = 0
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass through array
 * - Each element visited once
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - In-place modification
 * - Only pointer variables
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. All zeros: [0, 0, 0] → [0, 0, 0]
 * 2. No zeros: [1, 2, 3] → [1, 2, 3]
 * 3. Single element: [0] → [0], [5] → [5]
 * 4. Zeros at end: [1, 2, 0, 0] → [1, 2, 0, 0]
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Overwrite vs swap method - which is better?"
 * A1: Both are O(n) time, O(1) space. Trade-offs:
 *     - Overwrite: Two passes (move non-zeros, then fill zeros)
 *     - Swap: One pass, fewer writes when many non-zeros
 *
 *     Swap is slightly better for arrays with few zeros.
 *
 * -------------------------------------------------------------------------
 * Q2: "What if we want to move zeros to the FRONT?"
 * A2: Scan from right to left, or use write pointer from end.
 *     Same concept, just reversed direction.
 *
 * -------------------------------------------------------------------------
 * Q3: "Does this maintain relative order of non-zeros?"
 * A3: Yes! Both methods preserve the order of non-zero elements.
 *     This is called a "stable" partition.
 *
 * -------------------------------------------------------------------------
 * Q4: "Why check write != read before swapping?"
 * A4: Optimization to avoid unnecessary swaps when positions are same.
 *     Swapping element with itself wastes CPU cycles.
 *
 * ============================================================================
 */

#include <stdio.h>

// Method 1: Overwrite and fill
void move_zeros_v1(int arr[], int n) {
    // Initialize write pointer to track position for non-zero elements
    // Say: "I'll use a write pointer to track where to place the next non-zero element"
    int write = 0;

    // Move all non-zeros to front
    // Say: "First, I scan through the array and move all non-zero elements to the front"
    for (int read = 0; read < n; read++) {
        // Check if current element is non-zero
        // Say: "For each element, I check if it's not zero"
        if (arr[read] != 0) {
            // Copy non-zero element to write position
            // Say: "If it's non-zero, I write it at the write position and increment write"
            arr[write++] = arr[read];
        }
    }

    // Fill remaining with zeros
    // Say: "After moving all non-zeros, I fill the rest of the array with zeros"
    while (write < n) {
        // Set current position to zero
        // Say: "I set each remaining position to zero"
        arr[write++] = 0;
    }
    // Say: "Now all zeros are at the end while maintaining the order of non-zeros"
}

// Method 2: Swap method (single pass)
void move_zeros_v2(int arr[], int n) {
    // Initialize write pointer
    // Say: "For the swap method, I also use a write pointer"
    int write = 0;

    // Scan through array and swap non-zeros to front
    // Say: "I scan through and swap non-zero elements to their correct position"
    for (int read = 0; read < n; read++) {
        // Check if current element is non-zero
        // Say: "Whenever I find a non-zero element"
        if (arr[read] != 0) {
            // Swap only if positions are different
            // Say: "I swap it with the element at write position, but only if they're different"
            if (write != read) {
                // Perform swap using temp variable
                // Say: "I use a temp variable to swap the two elements"
                int temp = arr[write];
                arr[write] = arr[read];
                arr[read] = temp;
            }
            // Move write pointer forward
            // Say: "Then I move the write pointer forward"
            write++;
        }
    }
    // Say: "This naturally moves all zeros to the end through swapping"
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
    int arr1[] = {0, 1, 0, 3, 12};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);

    printf("Original: ");
    print_array(arr1, n1);

    move_zeros_v1(arr1, n1);
    printf("\nAfter:    ");
    print_array(arr1, n1);

    // Test swap method
    int arr2[] = {0, 0, 1, 0, 3, 0, 12};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    printf("\n\nOriginal: ");
    print_array(arr2, n2);

    move_zeros_v2(arr2, n2);
    printf("\nAfter:    ");
    print_array(arr2, n2);

    // Edge cases
    int arr3[] = {0, 0, 0};
    printf("\n\nAll zeros: ");
    print_array(arr3, 3);
    move_zeros_v1(arr3, 3);
    printf(" → ");
    print_array(arr3, 3);

    int arr4[] = {1, 2, 3};
    printf("\nNo zeros:  ");
    print_array(arr4, 3);
    move_zeros_v1(arr4, 3);
    printf(" → ");
    print_array(arr4, 3);

    return 0;
}
