/*
 * ============================================================================
 * PROBLEM: Sort Linked List (Merge Sort)
 * ============================================================================
 *
 * DIFFICULTY: Medium | TIME: 15 mins | FREQUENCY: HIGH
 *
 * Sort a linked list in O(n log n) time using merge sort.
 *
 * ============================================================================
 * WHY MERGE SORT FOR LINKED LISTS?
 * ============================================================================
 *
 * - Quick sort: Bad because random access is O(n)
 * - Merge sort: Perfect because we only need sequential access
 * - Splitting: Easy with slow/fast pointer to find middle
 * - Merging: Easy with linked structure
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 * 1. Find middle of list (slow/fast pointer)
 * 2. Split into two halves
 * 3. Recursively sort both halves
 * 4. Merge two sorted halves
 *
 * ============================================================================
 * VISUALIZATION:
 * ============================================================================
 *
 *   Original: 4 → 2 → 1 → 3
 *
 *   Split:    4 → 2    1 → 3
 *
 *   Split:    4   2    1   3
 *
 *   Merge:    2 → 4    1 → 3
 *
 *   Merge:    1 → 2 → 3 → 4
 *
 * ============================================================================
 * TIME: O(n log n) | SPACE: O(log n) recursion stack
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why merge sort instead of quick sort for linked lists?"
 * A1: Random access is the key difference:
 *
 *     Quick Sort: Needs random access for partitioning - O(n) in linked list
 *     Merge Sort: Only needs sequential access - perfect for linked lists!
 *
 *     Also, merge sort is STABLE (preserves order of equal elements).
 *
 * -------------------------------------------------------------------------
 * Q2: "Why start fast at head->next for finding middle?"
 * A2: This gives us the FIRST middle for even-length lists:
 *
 *     1 → 2 → 3 → 4
 *     We want slow to stop at 2 (not 3) so we can split evenly.
 *
 *     With fast = head->next: slow stops at node 2
 *     With fast = head: slow stops at node 3 (uneven split)
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you do this iteratively (without recursion)?"
 * A3: Yes! Bottom-up merge sort:
 *
 *     1. Merge pairs of 1-element sublists
 *     2. Merge pairs of 2-element sublists
 *     3. Merge pairs of 4-element sublists...
 *     Continue until entire list is sorted
 *
 *     This uses O(1) space but is more complex to implement.
 *
 * -------------------------------------------------------------------------
 * Q4: "What's the space complexity really?"
 * A4: O(log n) for recursion stack:
 *
 *     - Each recursive call splits the list in half
 *     - Maximum depth = log₂(n) calls
 *     - Each call uses constant space (just pointers)
 *
 *     For truly O(1) space, use iterative bottom-up approach.
 *
 * -------------------------------------------------------------------------
 * Q5: "Why use a dummy node in merge function?"
 * A5: Simplifies the code by avoiding special case for head:
 *
 *     Without dummy: Need to handle "which list has smaller first element"
 *     With dummy: Just append to tail->next, return dummy.next
 *
 *     Stack-allocated dummy (not malloc) so no memory leak risk.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * ============================================================================
 * NODE STRUCTURE - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * typedef struct Node {
 *     int data;           // PAYLOAD: The actual value stored
 *     struct Node* next;  // LINK: Pointer to the next node
 * } Node;
 *
 * - "typedef" creates an alias so we can write "Node" instead of "struct Node"
 * - "int data" stores the value (can be any type: int, char, float, etc.)
 * - "struct Node* next" points to the next node (NULL if last node)
 * - We use "struct Node*" inside because typedef isn't complete yet
 *
 * ============================================================================
 */
typedef struct Node {
    int data;           // The data field stores the actual value (payload)
    struct Node* next;  // Pointer to the next node in the list (or NULL)
} Node;

/*
 * ============================================================================
 * CREATE NODE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * malloc(sizeof(Node)): Allocates memory on HEAP for one Node
 * node->data = data:    Stores the parameter value using -> operator
 * node->next = NULL:    Initializes pointer (CRITICAL to avoid bugs)
 * return node:          Returns pointer to heap-allocated node
 *
 * ============================================================================
 */
Node* createNode(int data) {
    // Allocate memory on HEAP for one Node structure
    // Say: "I allocate memory for a new node using malloc"
    Node* node = (Node*)malloc(sizeof(Node));

    // Store the data value in the node
    // Say: "I store the data value in the node's data field"
    node->data = data;

    // Initialize next pointer to NULL (not connected yet)
    // Say: "I set next to NULL since this node isn't linked to anything yet"
    node->next = NULL;

    // Return pointer to the new node
    // Say: "I return the pointer to the newly created node"
    return node;
}

/*
 * ============================================================================
 * GET MIDDLE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* getMiddle(Node* head):
 *   - Returns "Node*" = pointer to the middle node (FIRST middle for even length)
 *   - "Node* head" = pointer to the list to find middle of
 *   - Purpose: Find the split point for merge sort
 *
 * WHY FIRST MIDDLE FOR EVEN LENGTH?
 * ---------------------------------
 * For merge sort, we need to split evenly:
 *   [1, 2, 3, 4] should split as [1, 2] and [3, 4]
 *   Middle should be at 2 (not 3)
 *
 * To get FIRST middle, we start fast at head->next (one ahead)
 * To get SECOND middle, we'd start fast at head
 *
 * VISUALIZATION:
 * ---------------------------------
 * List: 1 → 2 → 3 → 4 → NULL
 *
 * Start: slow=1, fast=2 (one ahead!)
 * Step 1: slow=2, fast=4
 * fast->next = NULL, STOP
 *
 * Return 2 (FIRST middle) ✓
 * Split: [1→2] and [3→4] - even split!
 *
 * TIME COMPLEXITY: O(n/2) = O(n)
 * ---------------------------------
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 *
 * ============================================================================
 */
// Find middle node for merge sort (returns FIRST middle for even length)
// Say: "I'll find the middle using slow/fast pointers to split the list"
Node* getMiddle(Node* head) {
    // =========================================================================
    // EDGE CASE: Empty or single node has no real middle
    // =========================================================================
    // Say: "I handle the base case of empty or single node"
    // WHY return head? Single node is its own middle; empty returns NULL
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // =========================================================================
    // INITIALIZE SLOW/FAST POINTERS
    // =========================================================================
    // Say: "I initialize slow and fast pointers"
    Node* slow = head;  // Tortoise: moves 1 step per iteration

    // Fast starts ONE AHEAD for first middle
    // Say: "Fast starts at head->next to get the first middle for even lists"
    // WHY one ahead? Makes slow stop at FIRST middle, not second
    // This gives us even splits for merge sort
    Node* fast = head->next;

    // =========================================================================
    // TRAVERSE: Find the middle
    // =========================================================================
    // Say: "I move slow by 1 and fast by 2 until fast reaches the end"
    // WHY this condition? Standard slow/fast middle-finding
    while (fast != NULL && fast->next != NULL) {
        // Move slow one step forward
        // Say: "I move slow forward one step"
        slow = slow->next;

        // Move fast two steps forward
        // Say: "I move fast forward two steps"
        fast = fast->next->next;
    }

    // =========================================================================
    // RETURN: Slow is at the middle (first middle for even length)
    // =========================================================================
    // Say: "I return the middle node"
    // After splitting at slow: head→...→slow | slow->next→...→NULL
    return slow;
}

/*
 * ============================================================================
 * MERGE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* merge(Node* left, Node* right):
 *   - Returns "Node*" = pointer to head of merged sorted list
 *   - "Node* left" = first sorted list
 *   - "Node* right" = second sorted list
 *   - Purpose: Merge two SORTED lists into one SORTED list
 *
 * ALGORITHM: Two-Pointer Merge
 * ---------------------------------
 * Same as merge step in merge sort for arrays:
 *   1. Compare heads of both lists
 *   2. Take the smaller, attach to result
 *   3. Advance that list's pointer
 *   4. Repeat until one is exhausted
 *   5. Attach remaining list
 *
 * WHY STACK-ALLOCATED DUMMY NODE?
 * ---------------------------------
 * "Node dummy;" allocates on STACK - no malloc/free needed!
 * Avoids special case for "which list has smaller first element"
 *
 * WHY "<=" INSTEAD OF "<"?
 * ---------------------------------
 * Using <= makes merge STABLE (preserves order of equal elements)
 * Important when elements have same key but different data
 *
 * TIME COMPLEXITY: O(n + m)
 * ---------------------------------
 * - n = length of left, m = length of right
 * - Each node visited exactly once
 *
 * SPACE COMPLEXITY: O(1)
 * ---------------------------------
 * - Stack-allocated dummy, few pointers
 * - No new nodes created - reuses existing nodes
 *
 * ============================================================================
 */
// Merge two sorted lists into one sorted list
// Say: "I'll merge two sorted lists by comparing and attaching smaller elements"
Node* merge(Node* left, Node* right) {
    // =========================================================================
    // DUMMY NODE: Simplifies building the merged list
    // =========================================================================
    // Say: "I create a stack-allocated dummy node to simplify merging"
    // WHY stack? Automatic cleanup, no memory leak risk
    Node dummy;
    dummy.next = NULL;

    // Tail tracks the end of our merged list
    // Say: "Tail pointer tracks where to attach the next node"
    // WHY point to dummy? First real node will be dummy.next
    Node* tail = &dummy;

    // =========================================================================
    // MAIN MERGE LOOP: Compare and attach smaller element
    // =========================================================================
    // Say: "I compare nodes from both lists and attach the smaller one"
    // WHY both conditions? We can only compare while both have nodes
    while (left != NULL && right != NULL) {
        // Compare current heads of both lists
        // Say: "I compare the current values from both lists"
        // WHY <=? Ensures stability (equal elements maintain relative order)
        if (left->data <= right->data) {
            // Left is smaller or equal - take it
            // Say: "Left's value is smaller or equal, so I attach it"
            tail->next = left;

            // Move left pointer forward
            // Say: "I move left forward to its next node"
            left = left->next;
        } else {
            // Right is smaller - take it
            // Say: "Right's value is smaller, so I attach it"
            tail->next = right;

            // Move right pointer forward
            // Say: "I move right forward to its next node"
            right = right->next;
        }

        // Move tail to the node we just added
        // Say: "I move tail forward to stay at the end"
        tail = tail->next;
    }

    // =========================================================================
    // ATTACH REMAINING: One list is exhausted, attach the other
    // =========================================================================
    // Say: "I attach whichever list still has remaining nodes"
    // WHY if/else? Exactly one of them might still have nodes
    // The remaining list is already sorted, just attach it directly
    if (left != NULL) {
        tail->next = left;
    } else {
        tail->next = right;
    }

    // =========================================================================
    // RETURN: Skip dummy, return the actual merged list
    // =========================================================================
    // Say: "I return the merged list starting from dummy's next"
    // WHY dummy.next? Dummy was just a placeholder; real list starts at next
    return dummy.next;
}

/*
 * ============================================================================
 * MERGE SORT FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* mergeSort(Node* head):
 *   - Returns "Node*" = pointer to head of sorted list
 *   - "Node* head" = pointer to the unsorted list
 *   - Purpose: Sort the linked list in O(n log n) time
 *
 * ALGORITHM: Divide and Conquer (Merge Sort)
 * ---------------------------------
 * 1. BASE CASE: If list has 0 or 1 nodes, it's already sorted
 * 2. DIVIDE: Find middle, split list into two halves
 * 3. CONQUER: Recursively sort each half
 * 4. COMBINE: Merge the two sorted halves
 *
 * WHY MERGE SORT FOR LINKED LISTS?
 * ---------------------------------
 * Quick Sort is bad for linked lists:
 *   - Needs random access for partitioning → O(n) per access
 *   - Total: O(n²) in practice
 *
 * Merge Sort is perfect:
 *   - Only needs sequential access
 *   - Finding middle: O(n) with slow/fast pointers
 *   - Merging: O(n) sequential traversal
 *   - Total: O(n log n) guaranteed!
 *
 * VISUALIZATION:
 * ---------------------------------
 *   [4, 2, 1, 3]
 *        |
 *   [4, 2]  [1, 3]      ← Split
 *    |   |   |   |
 *   [4] [2] [1] [3]     ← Split to single elements
 *    |   |   |   |
 *   [2, 4]  [1, 3]      ← Merge pairs
 *        |
 *   [1, 2, 3, 4]        ← Merge final
 *
 * RECURSION DEPTH:
 * ---------------------------------
 * Each level splits lists in half:
 *   Level 0: 1 list of n
 *   Level 1: 2 lists of n/2
 *   Level 2: 4 lists of n/4
 *   ...
 *   Level log(n): n lists of 1
 *
 * Total depth: O(log n) → This is the stack space used
 *
 * TIME COMPLEXITY: O(n log n)
 * ---------------------------------
 * - log n levels of recursion
 * - Each level processes all n nodes (finding middle + merging)
 * - Total: O(n) × O(log n) = O(n log n)
 *
 * SPACE COMPLEXITY: O(log n)
 * ---------------------------------
 * - Recursion depth is log n
 * - Each call uses O(1) space (just pointers)
 * - No extra arrays needed (unlike array merge sort)
 *
 * ============================================================================
 */
// Merge sort for linked list - O(n log n) guaranteed
// Say: "I'll sort using merge sort: divide, recursively sort, then merge"
Node* mergeSort(Node* head) {
    // =========================================================================
    // BASE CASE: Empty or single node is already sorted
    // =========================================================================
    // Say: "Base case: if empty or single node, it's already sorted"
    // WHY these conditions? 0 or 1 elements are trivially sorted
    // This is where recursion stops and starts returning
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // =========================================================================
    // STEP 1: DIVIDE - Find the middle to split the list
    // =========================================================================
    // Say: "Step 1: I find the middle using slow/fast pointers"
    // WHY getMiddle? We need to know where to split
    Node* middle = getMiddle(head);

    // Save the start of the right half (after middle)
    // Say: "I save the start of the right half"
    // WHY middle->next? Right half starts after the middle node
    Node* rightHalf = middle->next;

    // Break the link to actually split the list into two
    // Say: "I break the link at middle to split into two separate lists"
    // WHY set to NULL? This separates left half from right half
    // Left: head → ... → middle → NULL
    // Right: rightHalf → ... → NULL
    middle->next = NULL;

    // =========================================================================
    // STEP 2: CONQUER - Recursively sort both halves
    // =========================================================================
    // Say: "Step 2: I recursively sort both halves"

    // Sort the left half (head to middle)
    // Say: "Recursively sort the left half"
    // WHY head? Left half starts at original head
    Node* left = mergeSort(head);

    // Sort the right half (rightHalf to end)
    // Say: "Recursively sort the right half"
    // WHY rightHalf? Right half starts after the split point
    Node* right = mergeSort(rightHalf);

    // =========================================================================
    // STEP 3: COMBINE - Merge the two sorted halves
    // =========================================================================
    // Say: "Step 3: I merge the two sorted halves into one sorted list"
    // WHY merge? Both halves are now sorted; merge them maintaining order
    return merge(left, right);
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void printList(Node* head):
 *   - "void" = Function doesn't return anything (just prints to console)
 *   - "Node* head" = Receives a COPY of the head pointer (pass by value)
 *   - We can safely modify this copy without affecting caller's pointer
 *
 * WHY USE head DIRECTLY (no separate traversal pointer)?
 * ---------------------------------
 * - "head" is a LOCAL COPY of the pointer passed in
 * - Modifying head inside the function does NOT affect caller's pointer
 * - This is more concise - no need for extra "curr" variable
 * - Both approaches work; this is the cleaner style
 *
 * while (head != NULL):
 *   - Loop continues while head points to a valid node
 *   - When head becomes NULL, we've printed all nodes
 *   - NULL marks the end of a properly terminated linked list
 *
 * printf("%d", head->data):
 *   - Access the data field of the node head points to
 *   - %d prints the integer value
 *   - "->" is shorthand for (*head).data
 *
 * if (head->next) printf(" -> "):
 *   - Shorthand for "if (head->next != NULL)"
 *   - In C, non-NULL pointers evaluate to TRUE
 *   - Only print arrow if there's another node
 *   - Cleaner output: no trailing arrow after last node
 *
 * head = head->next:
 *   - Move to the next node in the list
 *   - This is the TRAVERSAL step
 *   - Original caller's head pointer is UNCHANGED
 *
 * printf(" -> NULL\n"):
 *   - Show that the list properly terminates
 *   - Visual indicator of list end
 *   - \n adds newline for clean output
 *
 * ============================================================================
 */
// Print list - traverse and print each node's data
// Say: "I'll traverse the list and print each node's value"
void printList(Node* head) {
    // Loop until we reach the end (NULL)
    // Say: "I loop while head is not NULL"
    // WHY use head directly? It's a local copy, won't affect caller
    while (head != NULL) {
        // Print current node's data
        // Say: "I print the current node's data"
        printf("%d", head->data);

        // Print arrow if there's a next node
        // Say: "If there's a next node, I print an arrow"
        if (head->next) printf(" -> ");

        // Move to next node
        // Say: "I advance head to the next node"
        head = head->next;
    }

    // Print NULL to show end of list
    // Say: "I print NULL to show the end of the list"
    printf(" -> NULL\n");
}

/*
 * ============================================================================
 * CREATE LIST FROM ARRAY - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Key insight: We need TWO pointers:
 *   - "head" stays at first node (to return it)
 *   - "current" moves forward as we build
 *
 * If we only used head and moved it, we'd lose the list start!
 *
 * ============================================================================
 */
// Create list from array
// Say: "I'll create a linked list from the given array"
Node* createList(int arr[], int n) {
    // Edge case: empty array
    // Say: "First, I check if the array is empty"
    if (n == 0) return NULL;

    // Create head with first element
    // Say: "I create the head node with the first element"
    Node* head = createNode(arr[0]);

    // current tracks where to add next node
    // Say: "I use current to track the end of the list"
    Node* current = head;

    // Add remaining elements
    // Say: "I loop through remaining elements"
    for (int i = 1; i < n; i++) {
        // Create and link
        // Say: "I create a new node and link it"
        current->next = createNode(arr[i]);

        // Move forward
        // Say: "I move current forward"
        current = current->next;
    }

    // Return head (never moved!)
    // Say: "I return head to access the list"
    return head;
}

/*
 * ============================================================================
 * FREE LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void freeList(Node* head):
 *   - "void" = Function doesn't return anything
 *   - "Node* head" = Takes pointer to the first node
 *   - Purpose: Deallocate ALL memory used by the linked list
 *
 * WHY WE NEED THIS FUNCTION:
 *   - Every malloc() MUST have a corresponding free()
 *   - Without freeing, we get MEMORY LEAKS
 *   - Memory leaks can crash long-running programs
 *   - In interviews, mentioning memory cleanup shows professionalism
 *
 * NOTE FOR MERGE SORT:
 *   - mergeSort() relinks existing nodes, doesn't create new ones
 *   - Same nodes, just different order - one freeList() call suffices
 *
 * ============================================================================
 */
// Free all nodes in the linked list to prevent memory leaks
// Say: "I'll free all nodes by saving next before freeing current"
void freeList(Node* head) {
    // Temporary pointer to save next node before freeing
    // Say: "I need a temp pointer to save the next node before freeing"
    Node* temp;

    // Loop through all nodes
    // Say: "I loop through each node until the end"
    while (head != NULL) {
        // Save the next pointer BEFORE freeing (critical!)
        // Say: "I save the next pointer before freeing the current node"
        temp = head->next;

        // Free the current node
        // Say: "I free the current node, returning its memory to the heap"
        free(head);

        // Move to the next node using saved pointer
        // Say: "I move to the next node using the saved pointer"
        head = temp;
    }
}

int main() {
    printf("=== Sort Linked List (Merge Sort) ===\n\n");

    // Test 1
    int arr1[] = {4, 2, 1, 3};
    Node* list1 = createList(arr1, 4);
    printf("Before: "); printList(list1);
    list1 = mergeSort(list1);
    printf("After:  "); printList(list1);
    freeList(list1);
    printf("\n");

    // Test 2
    int arr2[] = {5, 1, 8, 3, 7, 2, 9};
    Node* list2 = createList(arr2, 7);
    printf("Before: "); printList(list2);
    list2 = mergeSort(list2);
    printf("After:  "); printList(list2);
    freeList(list2);
    printf("\n");

    // Test 3 - already sorted
    int arr3[] = {1, 2, 3, 4, 5};
    Node* list3 = createList(arr3, 5);
    printf("Before: "); printList(list3);
    list3 = mergeSort(list3);
    printf("After:  "); printList(list3);
    freeList(list3);
    printf("\n");

    printf("=== Algorithm ===\n");
    printf("1. Find middle (slow/fast pointer)\n");
    printf("2. Split into two halves\n");
    printf("3. Recursively sort both\n");
    printf("4. Merge sorted halves\n");
    printf("\nTime: O(n log n), Space: O(log n) stack\n");

    return 0;
}
