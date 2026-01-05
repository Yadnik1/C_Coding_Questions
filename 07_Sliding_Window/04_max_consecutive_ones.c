/*
 * ============================================================================
 * PROBLEM: Max Consecutive Ones III (with K flips)
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: HIGH
 *
 * Pattern: VARIABLE SIZE SLIDING WINDOW
 *
 * Given binary array, find longest subarray of 1s if you can flip at most K 0s.
 *
 * ============================================================================
 * VISUALIZATION: arr = [1,1,1,0,0,0,1,1,1,1,0], k = 2
 * ============================================================================
 *
 *   [1,1,1,0,0,0,1,1,1,1,0]
 *    ^       ^
 *   left   right
 *   zeros = 2 (flipped two 0s)
 *   length = 5
 *
 *   Expand right, hit third 0:
 *   zeros = 3 > k, must shrink
 *
 *   Shrink left until zeros <= k:
 *   [0,0,0,1,1,1,1,0]
 *        ^         ^
 *   zeros = 2, length = 6 ← MAX
 *
 * ============================================================================
 * KEY INSIGHT:
 * ============================================================================
 *
 *   Track count of zeros in window.
 *   If zeros > k, shrink from left.
 *   Window always contains at most k zeros (which we "flip" to 1s).
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why use while loop to shrink, not just left++?"
 * A1: If zeros becomes much larger than k, we might need to shrink multiple
 *     times. But actually, with binary array, one shrink suffices since each
 *     element only adds 0 or 1 to zeros count. The while is more general.
 *
 * -------------------------------------------------------------------------
 * Q2: "Can we solve this without modifying the array?"
 * A2: Yes, this solution doesn't modify the array at all! We just count zeros
 *     in the window. No actual "flipping" happens - we're just allowing k
 *     zeros in the window, treating them as if they were 1s.
 *
 * -------------------------------------------------------------------------
 * Q3: "What if k = 0?"
 * A3: Then we're finding the longest contiguous sequence of 1s with no flips.
 *     The algorithm still works - any zero triggers shrinking until that zero
 *     is excluded from the window.
 *
 * -------------------------------------------------------------------------
 * Q4: "How is this different from 'longest substring with k distinct chars'?"
 * A4: Same pattern! Here we track zeros count, there we track distinct char
 *     count. Both expand right, shrink left when condition violated.
 *     Variable-size sliding window template applies to both!
 *
 * ============================================================================
 */

#include <stdio.h>

int longestOnes(int arr[], int n, int k) {
    // Initialize left pointer of the window
    // Say: "I'll start with the left pointer at index 0"
    int left = 0;

    // Initialize count of zeros in current window
    // Say: "I'll track the count of zeros in the current window"
    int zeros = 0;

    // Initialize maximum length found
    // Say: "I'll initialize the maximum length to 0"
    int maxLen = 0;

    // Expand window using right pointer
    // Say: "Now I'll expand the window using the right pointer"
    for (int right = 0; right < n; right++) {
        // Check if current element is zero
        // Say: "I check if the element at the right pointer is a zero"
        if (arr[right] == 0) {
            // Increment zero count
            // Say: "Yes, so I increment the count of zeros"
            zeros++;
        }

        // Shrink window while we have too many zeros
        // Say: "If I have more than k zeros, I need to shrink the window from the left"
        while (zeros > k) {
            // Check if element at left is zero
            // Say: "I check if the element at the left pointer is a zero"
            if (arr[left] == 0) {
                // Decrement zero count
                // Say: "Yes, so I decrement the count of zeros"
                zeros--;
            }

            // Move left pointer forward
            // Say: "I move the left pointer forward to shrink the window"
            left++;
        }

        // Calculate current window length
        // Say: "I calculate the current window length as right minus left plus one"
        int len = right - left + 1;

        // Check if current length is maximum
        // Say: "I check if this is the longest valid window so far"
        if (len > maxLen) {
            // Update maximum length
            // Say: "Yes, so I update the maximum length"
            maxLen = len;
        }
    }

    // Return the maximum length found
    // Say: "Finally, I return the maximum length"
    return maxLen;
}

void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(",");
    }
    printf("]");
}

int main() {
    printf("=== Max Consecutive Ones (with K flips) ===\n\n");

    int arr1[] = {1,1,1,0,0,0,1,1,1,1,0};
    int n1 = 11, k1 = 2;
    printf("Array: "); printArray(arr1, n1);
    printf(", k = %d\n", k1);
    printf("Max length: %d\n\n", longestOnes(arr1, n1, k1));

    int arr2[] = {0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1};
    int n2 = 19, k2 = 3;
    printf("Array: "); printArray(arr2, n2);
    printf(", k = %d\n", k2);
    printf("Max length: %d\n\n", longestOnes(arr2, n2, k2));

    int arr3[] = {1,1,1,1};
    int n3 = 4, k3 = 0;
    printf("Array: "); printArray(arr3, n3);
    printf(", k = %d\n", k3);
    printf("Max length: %d\n", longestOnes(arr3, n3, k3));

    return 0;
}
