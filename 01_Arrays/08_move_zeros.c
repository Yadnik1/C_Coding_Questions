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
 * ALTERNATIVE: SWAP METHOD
 * ============================================================================
 *
 * Instead of overwriting and filling zeros:
 *
 *   write = 0
 *   for read = 0 to n-1:
 *       if arr[read] != 0:
 *           swap(arr[write], arr[read])
 *           write++
 *
 * This naturally moves zeros to end through swaps!
 *
 * ============================================================================
 * SWAP METHOD VISUALIZATION:
 * ============================================================================
 *
 * Array: [0, 1, 0, 3, 12]
 *         ^
 *      write=0, read=0
 *
 * read=0: arr[0]=0, skip
 *
 * read=1: arr[1]=1 != 0
 *   swap(arr[0], arr[1])
 *   [1, 0, 0, 3, 12]
 *      ^
 *   write=1
 *
 * read=2: arr[2]=0, skip
 *
 * read=3: arr[3]=3 != 0
 *   swap(arr[1], arr[3])
 *   [1, 3, 0, 0, 12]
 *         ^
 *   write=2
 *
 * read=4: arr[4]=12 != 0
 *   swap(arr[2], arr[4])
 *   [1, 3, 12, 0, 0]
 *             ^
 *   write=3
 *
 * Done! [1, 3, 12, 0, 0] ✓
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
 */

#include <stdio.h>

// Method 1: Overwrite and fill
void move_zeros_v1(int arr[], int n) {
    int write = 0;

    // Move all non-zeros to front
    for (int read = 0; read < n; read++) {
        if (arr[read] != 0) {
            arr[write++] = arr[read];
        }
    }

    // Fill remaining with zeros
    while (write < n) {
        arr[write++] = 0;
    }
}

// Method 2: Swap method (single pass)
void move_zeros_v2(int arr[], int n) {
    int write = 0;

    for (int read = 0; read < n; read++) {
        if (arr[read] != 0) {
            // Swap only if positions are different
            if (write != read) {
                int temp = arr[write];
                arr[write] = arr[read];
                arr[read] = temp;
            }
            write++;
        }
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
