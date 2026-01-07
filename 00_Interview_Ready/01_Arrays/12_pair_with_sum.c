// Find pair with given sum in sorted array
// Time: O(n), Space: O(1) - Two pointer technique

#include <stdio.h>
#include <stdbool.h>

// Say: "Consistent helper to print arrays for verification"
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

bool find_pair_with_sum(int arr[], int n, int target, int *idx1, int *idx2) {
    // Say: "Two pointers - one at each end of sorted array"
    int left = 0;       // Say: "Start from smallest element"
    int right = n - 1;  // Say: "Start from largest element"

    // Say: "Move pointers toward each other until they meet"
    while (left < right) {
        // Say: "Calculate sum of elements at both pointers"
        int sum = arr[left] + arr[right];

        if (sum == target) {
            // Say: "Found the pair - return indices"
            *idx1 = left;
            *idx2 = right;
            return true;
        }
        else if (sum < target) {
            // Say: "Sum too small - need larger value"
            // Say: "Move left pointer right to get larger element"
            left++;
        }
        else {  // sum > target
            // Say: "Sum too large - need smaller value"
            // Say: "Move right pointer left to get smaller element"
            right--;
        }
    }

    // Say: "Pointers crossed without finding pair"
    return false;
}

// Say: "Extended version: find ALL pairs with given sum"
void find_all_pairs(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;
    int count = 0;

    printf("All pairs with sum %d:\n", target);

    while (left < right) {
        int sum = arr[left] + arr[right];

        if (sum == target) {
            printf("  (%d, %d) at indices [%d, %d]\n",
                   arr[left], arr[right], left, right);
            count++;
            left++;   // Say: "Move both to find more pairs"
            right--;
        }
        else if (sum < target) {
            left++;
        }
        else {
            right--;
        }
    }

    if (count == 0) {
        printf("  No pairs found\n");
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = sizeof(arr) / sizeof(arr[0]);  // Say: "Calculate array length"
    int target = 10;

    printf("Array: ");
    print_array(arr, n);
    printf("Target sum: %d\n\n", target);

    // Say: "Find first pair"
    int idx1, idx2;
    if (find_pair_with_sum(arr, n, target, &idx1, &idx2)) {
        printf("Pair found: arr[%d] + arr[%d] = %d + %d = %d\n",
               idx1, idx2, arr[idx1], arr[idx2], target);
    } else {
        printf("No pair found\n");
    }

    printf("\n");

    // Say: "Find all pairs"
    find_all_pairs(arr, n, target);

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use the two-pointer technique for sorted arrays.

 Key Insight:
 - Array is sorted, so we can use binary search principles
 - Start with smallest + largest elements
 - If sum is too small, we need a larger small element (move left right)
 - If sum is too large, we need a smaller large element (move right left)

 Algorithm:
 1. Set left = 0, right = n-1
 2. While left < right:
    - sum = arr[left] + arr[right]
    - If sum == target: found pair
    - If sum < target: left++ (need larger value)
    - If sum > target: right-- (need smaller value)

 Why does this work?
 - We never miss a valid pair
 - When we move left++, we've checked all pairs with arr[left]
 - Similarly for right--

 Example for [1,2,4,5,7,11], target=9:
 - left=0, right=5: 1+11=12 > 9, right--
 - left=0, right=4: 1+7=8 < 9, left++
 - left=1, right=4: 2+7=9 == 9, found!

 Time: O(n) - each pointer moves at most n times
 Space: O(1) - only two pointer variables

 Why is this better than brute force?
 - Brute force: O(n^2) - check all pairs
 - Two pointer: O(n) - linear time
 - But requires sorted array (O(n log n) to sort if unsorted)

 For unsorted arrays, use hash set:
 - For each element x, check if (target - x) exists in set
 - Time: O(n), Space: O(n)

 Common variations:
 1. Find if pair exists (boolean)
 2. Return the pair values
 3. Return indices
 4. Find ALL pairs with target sum
 5. Find pair closest to target sum
 6. Three sum (extend to three elements)

 Edge cases to mention:
 - Empty array: return false
 - Single element: return false (need two elements)
 - No valid pair: return false
 - Multiple pairs: this returns first found
 - Duplicate elements: still works correctly

 Real-world applications:
 - Finding complements in financial calculations
 - Two-sum in algorithm interviews (LeetCode #1)
 - Database query optimization with sorted indices"
*/
