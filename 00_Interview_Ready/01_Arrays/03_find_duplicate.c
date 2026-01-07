// Find duplicate in array [1, n] with one duplicate
// Time: O(n), Space: O(1) - Floyd's Cycle Detection

#include <stdio.h>

void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++)
        printf("%d%s", arr[i], i < n-1 ? ", " : "");
    printf("]\n");
}

int find_duplicate(int arr[], int n) {
    // Say: "I treat array values as pointers - if arr[i]=3, next is arr[3]"
    // Say: "Duplicate creates a cycle, like linked list cycle detection"

    int slow = arr[0];  // Say: "Slow pointer moves one step"
    int fast = arr[0];  // Say: "Fast pointer moves two steps"

    // Say: "Phase 1: Find where slow and fast meet inside cycle"
    do {
        slow = arr[slow];         // Move one step
        fast = arr[arr[fast]];    // Move two steps
    } while (slow != fast);
    // Say: "They will definitely meet because duplicate creates cycle"

    // Say: "Phase 2: Find cycle start (the duplicate)"
    slow = arr[0];  // Reset slow to beginning
    while (slow != fast) {
        slow = arr[slow];  // Both move one step
        fast = arr[fast];
    }
    // Say: "Where they meet is the duplicate value"

    return slow;
}

int main() {
    int arr[] = {1, 3, 4, 2, 2};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array: ");
    print_array(arr, n);

    printf("Duplicate: %d\n", find_duplicate(arr, n));

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use Floyd's Cycle Detection (tortoise and hare).

 Key insight: Array values are in range [1,n], so each value
 can be used as an index. If there's a duplicate, following
 these 'pointers' creates a cycle.

 Phase 1: Move slow by 1, fast by 2 until they meet.
 Phase 2: Reset slow to start, move both by 1 until they meet.
 Meeting point in phase 2 is the cycle start = duplicate.

 Why this works:
 - When slow enters cycle, fast is already inside
 - Math proves meeting point in phase 2 is cycle start
 - Same algorithm used for linked list cycle detection"
*/
