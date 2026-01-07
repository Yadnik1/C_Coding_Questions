// Move all zeros to end while maintaining order of non-zero elements
// Time: O(n), Space: O(1) - Single pass with write pointer

#include <stdio.h>

// Say: "Consistent helper to print arrays for verification"
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

void move_zeros(int arr[], int n) {
    // Say: "I use a write pointer to track where the next non-zero should go"
    int write = 0;  // Say: "Position where next non-zero element will be placed"

    // Say: "First pass: move all non-zero elements to front"
    for (int i = 0; i < n; i++) {
        // Say: "If current element is non-zero, place it at write position"
        if (arr[i] != 0) {
            arr[write] = arr[i];  // Say: "Copy non-zero to write position"
            write++;              // Say: "Advance write pointer"
        }
        // Say: "If zero, we skip it - it will be overwritten later"
    }

    // Say: "Second pass: fill remaining positions with zeros"
    // Say: "At this point, write points to first position that should be zero"
    while (write < n) {
        arr[write] = 0;  // Say: "Fill with zero"
        write++;         // Say: "Move to next position"
    }
    // Say: "All non-zeros are at front in original order, zeros at end"
}

int main() {
    int arr[] = {0, 1, 0, 3, 12, 0, 5};
    int n = sizeof(arr) / sizeof(arr[0]);  // Say: "Calculate array length"

    printf("Original: ");
    print_array(arr, n);

    move_zeros(arr, n);

    printf("After moving zeros: ");
    print_array(arr, n);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I solve this using a two-pass approach with a write pointer.

 First pass (collect non-zeros):
 - Iterate through array with read pointer
 - When I find a non-zero, copy it to write position
 - Increment write pointer after each copy
 - This preserves relative order of non-zero elements

 Second pass (fill zeros):
 - Start from write pointer position
 - Fill all remaining positions with zeros

 Example walkthrough for [0, 1, 0, 3, 12]:
 - i=0: arr[0]=0, skip
 - i=1: arr[1]=1, write to pos 0, write=1
 - i=2: arr[2]=0, skip
 - i=3: arr[3]=3, write to pos 1, write=2
 - i=4: arr[4]=12, write to pos 2, write=3
 - Fill positions 3,4 with zeros
 - Result: [1, 3, 12, 0, 0]

 Time: O(n) - two passes through array
 Space: O(1) - only one extra variable (write pointer)

 Alternative: Single-pass with swap
 - Swap non-zero with position at write pointer
 - Also O(n) time, O(1) space, but more swaps"
*/
