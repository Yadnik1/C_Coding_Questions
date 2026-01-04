/*
 * ============================================================================
 * PROBLEM: Dutch National Flag (Sort 0, 1, 2)
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * Given array with only 0s, 1s, and 2s, sort in single pass without extra space.
 *
 * Example:
 * Input:  [2, 0, 1, 2, 1, 0]
 * Output: [0, 0, 1, 1, 2, 2]
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. THREE-POINTER PARTITIONING:
 *    - low:  boundary between 0s and 1s (next position for 0)
 *    - mid:  current element being examined
 *    - high: boundary between 1s and 2s (next position for 2)
 *
 * 2. INVARIANT MAINTAINED:
 *    - [0...low-1]    → all 0s
 *    - [low...mid-1]  → all 1s
 *    - [mid...high]   → unknown (to be processed)
 *    - [high+1...n-1] → all 2s
 *
 * 3. WHY "DUTCH NATIONAL FLAG"?
 *    - Named by Edsger Dijkstra
 *    - Netherlands flag has 3 horizontal stripes: red, white, blue
 *    - Similar to sorting 3 colors/values
 *
 * ============================================================================
 * THE KEY INSIGHT:
 * ============================================================================
 *
 *   We partition array into 4 regions:
 *
 *   |  0s   |   1s   | unknown |  2s   |
 *   0      low      mid       high    n-1
 *
 *   Process elements in "unknown" region one by one:
 *   - If 0: swap to "0s" region, expand both boundaries
 *   - If 1: already in correct place, just move mid
 *   - If 2: swap to "2s" region, shrink unknown from right
 *
 * ============================================================================
 * STEP-BY-STEP VISUALIZATION:
 * ============================================================================
 *
 * Array: [2, 0, 1, 2, 1, 0]
 *         0  1  2  3  4  5
 *
 * INITIAL STATE:
 *   low = 0, mid = 0, high = 5
 *
 *   [2, 0, 1, 2, 1, 0]
 *    ^              ^
 *   low            high
 *   mid
 *
 *   Regions: | unknown [0...5] |
 *
 * -------------------------------------------------------------------------
 * STEP 1: arr[mid] = arr[0] = 2
 *         2 goes to the right → swap(arr[mid], arr[high]), high--
 *         DON'T increment mid (need to check swapped element)
 *
 *   swap arr[0] and arr[5]:
 *   [0, 0, 1, 2, 1, 2]
 *    ^           ^
 *   low        high
 *   mid
 *
 *   low = 0, mid = 0, high = 4
 *
 * -------------------------------------------------------------------------
 * STEP 2: arr[mid] = arr[0] = 0
 *         0 goes to the left → swap(arr[low], arr[mid]), low++, mid++
 *         (swapping with itself here since low == mid)
 *
 *   [0, 0, 1, 2, 1, 2]
 *       ^        ^
 *      low     high
 *      mid
 *
 *   low = 1, mid = 1, high = 4
 *   Regions: | 0s [0] | unknown [1...4] | 2s [5] |
 *
 * -------------------------------------------------------------------------
 * STEP 3: arr[mid] = arr[1] = 0
 *         0 goes to the left → swap(arr[low], arr[mid]), low++, mid++
 *
 *   [0, 0, 1, 2, 1, 2]
 *          ^     ^
 *         low  high
 *         mid
 *
 *   low = 2, mid = 2, high = 4
 *   Regions: | 0s [0,1] | unknown [2...4] | 2s [5] |
 *
 * -------------------------------------------------------------------------
 * STEP 4: arr[mid] = arr[2] = 1
 *         1 is already in correct position → mid++
 *
 *   [0, 0, 1, 2, 1, 2]
 *          ^  ^  ^
 *         low|  high
 *           mid
 *
 *   low = 2, mid = 3, high = 4
 *   Regions: | 0s [0,1] | 1s [2] | unknown [3,4] | 2s [5] |
 *
 * -------------------------------------------------------------------------
 * STEP 5: arr[mid] = arr[3] = 2
 *         2 goes to the right → swap(arr[mid], arr[high]), high--
 *
 *   swap arr[3] and arr[4]:
 *   [0, 0, 1, 1, 2, 2]
 *          ^  ^
 *         low|
 *           mid
 *           high
 *
 *   low = 2, mid = 3, high = 3
 *
 * -------------------------------------------------------------------------
 * STEP 6: arr[mid] = arr[3] = 1
 *         1 is already in correct position → mid++
 *
 *   [0, 0, 1, 1, 2, 2]
 *          ^     ^
 *         low   mid
 *            high
 *
 *   low = 2, mid = 4, high = 3
 *
 * -------------------------------------------------------------------------
 * STOP: mid (4) > high (3)
 *
 * FINAL RESULT: [0, 0, 1, 1, 2, 2]
 *
 *   Regions: | 0s [0,1] | 1s [2,3] | 2s [4,5] |
 *
 * ============================================================================
 * WHY DON'T WE INCREMENT MID AFTER SWAPPING WITH HIGH?
 * ============================================================================
 *
 * When we swap arr[mid] with arr[high], the element that comes from high
 * is UNKNOWN - we haven't examined it yet!
 *
 * Example:
 *   [2, 0, 1]      mid=0, high=2
 *    ^     ^
 *   mid   high
 *
 *   arr[mid]=2, swap with high:
 *   [1, 0, 2]
 *    ^  ^
 *   mid high
 *
 *   Now arr[mid]=1, we need to process it!
 *   If we had incremented mid, we'd skip checking this 1.
 *
 * But when swapping with low, arr[low] always contains a 1 (or we're
 * swapping with ourselves), so it's safe to increment mid.
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   low = 0, mid = 0, high = n - 1
 *
 *   while (mid <= high):
 *       if arr[mid] == 0:
 *           swap(arr[low], arr[mid])
 *           low++
 *           mid++
 *       else if arr[mid] == 1:
 *           mid++
 *       else:  // arr[mid] == 2
 *           swap(arr[mid], arr[high])
 *           high--
 *           // Don't increment mid!
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Single pass through array
 * - Each element examined at most twice
 * - Linear time guaranteed
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only three pointer variables
 * - In-place sorting
 * - Constant extra space
 *
 * ============================================================================
 * EDGE CASES:
 * ============================================================================
 * 1. All same: [0, 0, 0] → [0, 0, 0]
 * 2. Already sorted: [0, 1, 2] → [0, 1, 2]
 * 3. Reverse sorted: [2, 1, 0] → [0, 1, 2]
 * 4. Only two values: [0, 2, 0, 2] → [0, 0, 2, 2]
 * 5. Single element: [1] → [1]
 *
 * ============================================================================
 * VARIATIONS:
 * ============================================================================
 *
 * 1. Sort with k distinct values (k-way partition):
 *    - Use counting sort O(n+k) or multiple passes
 *
 * 2. Three-way quicksort partition:
 *    - Same algorithm, pivot instead of 1
 *    - Elements < pivot, == pivot, > pivot
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why don't we increment mid after swapping with high?"
 * A1: When we swap arr[mid] with arr[high], the element that comes FROM
 *     high is UNKNOWN - we haven't examined it yet!
 *
 *     Example: [2, 0, 1] with mid=0, high=2
 *     After swap: [1, 0, 2] - now arr[mid]=1, we need to process it!
 *
 *     But when swapping with low, arr[low] always contains a 1 (or we're
 *     swapping with ourselves when low==mid), so it's safe to move on.
 *
 * -------------------------------------------------------------------------
 * Q2: "Why is it called Dutch National Flag?"
 * A2: Named by Edsger Dijkstra (Dutch computer scientist).
 *     The Netherlands flag has three horizontal stripes: red, white, blue.
 *     Sorting 0s, 1s, 2s is analogous to arranging these three colors.
 *
 * -------------------------------------------------------------------------
 * Q3: "Can we use counting sort instead? What are the trade-offs?"
 * A3: Yes! Counting sort works:
 *     - Count 0s, 1s, 2s in first pass
 *     - Overwrite array in second pass
 *
 *     Trade-offs:
 *     - Counting sort: 2 passes, O(1) space, simpler code
 *     - Dutch flag: 1 pass, O(1) space, slightly more complex
 *
 *     Dutch flag is preferred when elements are objects (not just integers)
 *     because counting sort would require extra space for object copies.
 *
 * -------------------------------------------------------------------------
 * Q4: "What invariant does this algorithm maintain?"
 * A4: At any point during execution:
 *     - arr[0...low-1]     = all 0s
 *     - arr[low...mid-1]   = all 1s
 *     - arr[mid...high]    = unknown (to be processed)
 *     - arr[high+1...n-1]  = all 2s
 *
 *     The algorithm terminates when mid > high (unknown region is empty).
 *
 * -------------------------------------------------------------------------
 * Q5: "How would you modify this for 4 distinct values (0, 1, 2, 3)?"
 * A5: You can't do it in one pass with O(1) space!
 *     Options:
 *     1. Counting sort: O(n) time, O(1) space, 2 passes
 *     2. Two-pass Dutch flag: First sort 0,1,2 vs 3, then sort 0,1 vs 2
 *     3. General in-place: O(n) time but multiple passes
 *
 *     The Dutch National Flag problem is specifically for 3 values.
 *
 * ============================================================================
 */

#include <stdio.h>

void swap(int* a, int* b) {
    // Save first value in temp
    // Say: "I swap two values using a temporary variable"
    int temp = *a;

    // Copy second to first
    *a = *b;

    // Copy temp to second
    *b = temp;
}

void dutchNationalFlag(int arr[], int n) {
    // Initialize low pointer for 0s boundary
    // Say: "I use three pointers for the Dutch National Flag algorithm"
    // Say: "Low marks the boundary between zeros and ones"
    int low = 0;

    // Initialize mid pointer for current element
    // Say: "Mid is the current element I'm examining"
    int mid = 0;

    // Initialize high pointer for 2s boundary
    // Say: "And high marks the boundary between ones and twos"
    int high = n - 1;

    // Process until mid crosses high
    // Say: "I keep processing while mid hasn't crossed high"
    while (mid <= high) {
        // Check if current element is 0
        // Say: "If the current element is zero"
        if (arr[mid] == 0) {
            // Swap with low and advance both
            // Say: "I swap it with the low position and advance both low and mid"
            swap(&arr[low], &arr[mid]);
            low++;
            mid++;
        } else if (arr[mid] == 1) {
            // Element is 1, just advance mid
            // Say: "If it's one, it's already in the right section, so I just move mid forward"
            mid++;
        } else {  // arr[mid] == 2
            // Swap with high and advance high only
            // Say: "If it's two, I swap with high and move high backward"
            swap(&arr[mid], &arr[high]);
            high--;
            // Don't increment mid - need to check swapped element
            // Say: "I don't move mid because I need to check what I just swapped in"
        }
    }
    // Say: "When mid crosses high, all elements are partitioned into zeros, ones, and twos"
}

void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    printf("=== Dutch National Flag (Sort 0, 1, 2) ===\n\n");

    int arr1[] = {2, 0, 1, 2, 1, 0};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("Before: "); printArray(arr1, n1);
    dutchNationalFlag(arr1, n1);
    printf("After:  "); printArray(arr1, n1);
    printf("\n");

    int arr2[] = {0, 1, 2, 0, 1, 2};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Before: "); printArray(arr2, n2);
    dutchNationalFlag(arr2, n2);
    printf("After:  "); printArray(arr2, n2);
    printf("\n");

    int arr3[] = {2, 2, 1, 1, 0, 0};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("Before: "); printArray(arr3, n3);
    dutchNationalFlag(arr3, n3);
    printf("After:  "); printArray(arr3, n3);

    return 0;
}
