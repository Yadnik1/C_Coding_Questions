/*
 * ============================================================================
 * PROBLEM: Find Pair with Given Sum
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH
 *
 * Two versions:
 * 1. Sorted array - Two pointer O(n)
 * 2. Unsorted array - Hash/Brute force O(n²) or O(n) with hash
 *
 * ============================================================================
 * VISUALIZATION (Sorted - Two Pointer):
 * ============================================================================
 *
 *   arr = [1, 2, 4, 6, 8, 10], target = 10
 *          ^              ^
 *         left          right
 *
 *   sum = 1 + 10 = 11 > 10 → right--
 *   sum = 1 + 8 = 9 < 10  → left++
 *   sum = 2 + 8 = 10 == target → FOUND!
 *
 * ============================================================================
 * TIME: O(n) sorted, O(n²) unsorted brute | SPACE: O(1)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why does the two-pointer technique work for sorted arrays?"
 * A1: Because of monotonicity - if sum is too big, we move right pointer
 *     left to get smaller value. If sum is too small, we move left pointer
 *     right to get larger value. We're guaranteed not to miss any pair.
 *
 * Q2: "How would you find ALL pairs with the given sum?"
 * A2: Don't return on first match - continue the loop, print each pair found.
 *
 * Q3: "What's the space complexity of the hash approach?"
 * A3: O(n) because we store each element in the hash table.
 *
 * Q4: "How would you handle duplicates?"
 * A4: Skip duplicates after finding a pair to avoid reporting same pair twice.
 *
 * Q5: "What if array has negative numbers for hash approach?"
 * A5: Need to offset indices or use a proper hash map structure.
 *
 * ============================================================================
 */

#include <stdio.h>      // For printf - standard I/O library
#include <stdbool.h>    // For bool type - gives us true/false

/*
 * TWO POINTER APPROACH FOR SORTED ARRAY
 *
 * Say to interviewer: "Since the array is sorted, I'll use two pointers
 * starting from both ends. This gives us O(n) time complexity."
 */
bool pairSumSorted(int arr[], int n, int target, int* idx1, int* idx2) {

    // Initialize left pointer at the beginning of array
    // Say: "I start left pointer at index 0, the smallest element"
    int left = 0;

    // Initialize right pointer at the end of array
    // Say: "Right pointer starts at index n-1, the largest element"
    int right = n - 1;

    // Keep searching until pointers meet
    // Say: "I'll keep moving pointers until they cross each other"
    while (left < right) {

        // Calculate sum of elements at both pointers
        // Say: "Let me calculate the sum of current pair"
        int sum = arr[left] + arr[right];

        // Check if we found the target sum
        // Say: "If sum equals target, we found our pair"
        if (sum == target) {
            *idx1 = left;       // Store first index in output parameter
            *idx2 = right;      // Store second index in output parameter
            return true;        // Return true - pair found!
        }
        // If sum is less than target, we need a bigger sum
        // Say: "Sum is too small, so I move left pointer right to get larger value"
        else if (sum < target) {
            left++;             // Move left pointer to get larger element
        }
        // If sum is greater than target, we need a smaller sum
        // Say: "Sum is too big, so I move right pointer left to get smaller value"
        else {
            right--;            // Move right pointer to get smaller element
        }
    }

    // No pair found after checking all possibilities
    // Say: "Pointers crossed without finding a pair, so no solution exists"
    return false;
}

/*
 * BRUTE FORCE APPROACH FOR UNSORTED ARRAY
 *
 * Say to interviewer: "For unsorted array, I'll check every possible pair.
 * This is O(n²) time but O(1) space."
 */
bool pairSumUnsorted(int arr[], int n, int target, int* idx1, int* idx2) {

    // Outer loop: pick first element of the pair
    // Say: "I'll pick each element as the first element of potential pair"
    for (int i = 0; i < n - 1; i++) {

        // Inner loop: try pairing with every element after it
        // Say: "For each element, I check all elements after it"
        for (int j = i + 1; j < n; j++) {

            // Check if this pair sums to target
            // Say: "If arr[i] + arr[j] equals target, we found our pair"
            if (arr[i] + arr[j] == target) {
                *idx1 = i;      // Store first index
                *idx2 = j;      // Store second index
                return true;    // Pair found!
            }
        }
    }

    // No pair found
    // Say: "Checked all pairs, none sum to target"
    return false;
}

/*
 * HASH TABLE APPROACH FOR UNSORTED ARRAY
 *
 * Say to interviewer: "I'll use a hash table to achieve O(n) time.
 * For each element, I check if its complement (target - element) exists."
 */
#define HASH_SIZE 10000     // Size of our hash table

bool pairSumHash(int arr[], int n, int target, int* idx1, int* idx2) {

    // Create hash table to store element indices
    // Say: "I'll create a hash table to store indices of elements I've seen"
    int hash[HASH_SIZE];

    // Initialize all entries to -1 (meaning "not seen yet")
    // Say: "Initialize hash table with -1 to indicate empty slots"
    for (int i = 0; i < HASH_SIZE; i++) {
        hash[i] = -1;       // -1 means this value hasn't been seen
    }

    // Process each element in the array
    // Say: "Now I'll iterate through the array once"
    for (int i = 0; i < n; i++) {

        // Calculate what value we need to find (complement)
        // Say: "For current element, I calculate what value would complete the pair"
        int complement = target - arr[i];

        // Check if complement exists in hash table
        // Say: "I check if the complement was seen before in the hash table"
        if (complement >= 0 && complement < HASH_SIZE && hash[complement] != -1) {
            // Found it! The complement was seen at hash[complement]
            *idx1 = hash[complement];   // Index where complement was found
            *idx2 = i;                  // Current index
            return true;                // Pair found!
        }

        // Store current element's index in hash table
        // Say: "Add current element to hash table for future lookups"
        if (arr[i] >= 0 && arr[i] < HASH_SIZE) {
            hash[arr[i]] = i;   // Store index where this value appears
        }
    }

    // No pair found
    // Say: "Iterated through entire array, no complement found for any element"
    return false;
}

/*
 * MAIN FUNCTION - Test all three approaches
 */
int main() {
    printf("=== Find Pair with Given Sum ===\n\n");

    // Test 1: Sorted array with two-pointer approach
    printf("1. Sorted Array (Two Pointer):\n");
    int sorted[] = {1, 2, 4, 6, 8, 10};     // Sorted array
    int n1 = 6, target1 = 10;               // Size and target
    int idx1, idx2;                          // To store result indices

    printf("   Array: [1, 2, 4, 6, 8, 10], Target: %d\n", target1);
    if (pairSumSorted(sorted, n1, target1, &idx1, &idx2)) {
        printf("   Found: arr[%d] + arr[%d] = %d + %d = %d\n\n",
               idx1, idx2, sorted[idx1], sorted[idx2], target1);
    }

    // Test 2: Unsorted array with brute force
    printf("2. Unsorted Array (Brute Force):\n");
    int unsorted[] = {8, 3, 5, 2, 9, 1};    // Unsorted array
    int n2 = 6, target2 = 7;

    printf("   Array: [8, 3, 5, 2, 9, 1], Target: %d\n", target2);
    if (pairSumUnsorted(unsorted, n2, target2, &idx1, &idx2)) {
        printf("   Found: arr[%d] + arr[%d] = %d + %d = %d\n\n",
               idx1, idx2, unsorted[idx1], unsorted[idx2], target2);
    }

    // Test 3: Unsorted array with hash approach
    printf("3. Unsorted Array (Hash O(n)):\n");
    printf("   Array: [8, 3, 5, 2, 9, 1], Target: %d\n", target2);
    if (pairSumHash(unsorted, n2, target2, &idx1, &idx2)) {
        printf("   Found: arr[%d] + arr[%d] = %d + %d = %d\n\n",
               idx1, idx2, unsorted[idx1], unsorted[idx2], target2);
    }

    // Print summary of time/space complexity
    printf("=== Summary ===\n");
    printf("Sorted:   Two pointer - O(n) time, O(1) space\n");
    printf("Unsorted: Brute force - O(n²) time, O(1) space\n");
    printf("Unsorted: Hash - O(n) time, O(n) space\n");

    return 0;
}
