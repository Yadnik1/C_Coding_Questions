/*
 * ============================================================================
 * PROBLEM: Majority Element (Boyer-Moore Voting Algorithm)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given an array, find the element that appears more than n/2 times.
 * The majority element is guaranteed to exist (in basic version).
 * Must solve in O(n) time and O(1) space.
 *
 * EXAMPLES:
 * - Input: [3, 2, 3]           ->  Output: 3 (appears 2 times, n/2 = 1)
 * - Input: [2, 2, 1, 1, 1, 2, 2] -> Output: 2 (appears 4 times, n/2 = 3)
 * - Input: [1, 1, 1, 1]        ->  Output: 1 (all same)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Elegant O(1) space solution (Boyer-Moore) tests algorithm knowledge
 * - Shows creative problem-solving (voting intuition)
 * - Common in distributed systems (leader election analogy)
 * - Asked at Google, Amazon, Microsoft - LeetCode #169
 *
 * KEY CONCEPT:
 * Boyer-Moore Voting Algorithm - Imagine each element as a vote.
 * Maintain a candidate and count. Same element increments count,
 * different element decrements. At 0, pick new candidate.
 * Majority survives because it has > n/2 "votes."
 *
 * VISUAL:
 *     Array: [2, 2, 1, 1, 1, 2, 2]
 *
 *     i=0: candidate=2, count=1
 *     i=1: 2==2, count=2
 *     i=2: 1!=2, count=1      <- One 2 "cancelled" by one 1
 *     i=3: 1!=2, count=0      <- Another pair cancelled
 *     i=4: count=0, new candidate=1, count=1
 *     i=5: 2!=1, count=0      <- 1 cancelled by 2
 *     i=6: count=0, new candidate=2, count=1
 *
 *     Result: candidate = 2 (verify with second pass if needed)
 *
 *     Intuition: Majority can "defeat" all others combined!
 *
 * ============================================================================
 */

// Find majority element (appears more than n/2 times)
// Time: O(n), Space: O(1) - Boyer-Moore Voting Algorithm

#include <stdio.h>
#include <stdbool.h>

// Say: "Consistent helper to print arrays for verification"
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

int majority_element(int arr[], int n) {
    // Say: "Boyer-Moore Voting - find candidate with O(1) space"
    int candidate = arr[0];  // Say: "Start with first element as candidate"
    int count = 1;           // Say: "Count tracks 'votes' for current candidate"

    // Say: "First pass - find potential candidate"
    for (int i = 1; i < n; i++) {
        if (count == 0) {
            // Say: "Previous candidate was 'voted out', pick new one"
            candidate = arr[i];
            count = 1;
        } else if (arr[i] == candidate) {
            // Say: "Same as candidate - increment vote count"
            count++;
        } else {
            // Say: "Different from candidate - decrement vote count"
            count--;
            // Say: "Think of it as: one majority cancels one minority"
        }
    }

    // Say: "If majority exists, candidate holds it; if not guaranteed, verify"
    return candidate;
}

// Say: "Optional verification step if majority not guaranteed to exist"
bool verify_majority(int arr[], int n, int candidate) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] == candidate) count++;
    }
    return count > n / 2;  // Say: "Check if appears more than n/2 times"
}

int main() {
    int arr[] = {2, 2, 1, 1, 1, 2, 2};
    int n = sizeof(arr) / sizeof(arr[0]);  // Say: "Calculate array length"

    printf("Array: ");
    print_array(arr, n);

    int result = majority_element(arr, n);

    // Say: "Verify result if majority element is not guaranteed"
    if (verify_majority(arr, n, result)) {
        printf("Majority element: %d\n", result);
    } else {
        printf("No majority element exists\n");
    }

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use Boyer-Moore Voting Algorithm - elegant O(n) time, O(1) space solution.

 Key Insight:
 - If an element appears more than n/2 times, it will 'survive'
   even if all other elements vote against it

 Algorithm:
 1. Maintain a candidate and a count
 2. For each element:
    - If count is 0: pick current as new candidate, count = 1
    - If same as candidate: increment count
    - If different: decrement count
 3. Final candidate is the majority (if one exists)

 Why it works (intuition):
 - Think of it as pairing majority with minority elements
 - Each pair 'cancels out'
 - Majority has more than half, so it can't all be cancelled

 Example: [2, 2, 1, 1, 1, 2, 2]
 - i=0: candidate=2, count=1
 - i=1: arr[1]=2, count=2
 - i=2: arr[2]=1, count=1 (cancel one 2 with one 1)
 - i=3: arr[3]=1, count=0 (cancel another 2 with 1)
 - i=4: arr[4]=1, candidate=1, count=1 (new candidate)
 - i=5: arr[5]=2, count=0
 - i=6: arr[6]=2, candidate=2, count=1
 - Result: 2 is candidate

 IMPORTANT: This only finds a candidate!
 - If majority is GUARANTEED to exist, we're done
 - If NOT guaranteed, do a second pass to verify count > n/2

 Time: O(n) - single pass (or two if verifying)
 Space: O(1) - only two variables

 Alternative approaches:
 - Sorting: O(n log n) time, middle element is majority
 - Hash map: O(n) time but O(n) space
 - Boyer-Moore is optimal for this problem"
*/
