/*
 * ============================================================================
 * PROBLEM: Reverse a Linked List
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (MUST KNOW!)
 *
 * Reverse a singly linked list in-place.
 *
 * Example:
 * Input:  1 -> 2 -> 3 -> 4 -> 5 -> NULL
 * Output: 5 -> 4 -> 3 -> 2 -> 1 -> NULL
 *
 * ============================================================================
 * WHAT YOU MUST KNOW BEFORE SOLVING:
 * ============================================================================
 *
 * 1. LINKED LIST BASICS:
 *    - Each node has: data + pointer to next
 *    - Head points to first node
 *    - Last node points to NULL
 *
 * 2. THREE POINTER TECHNIQUE:
 *    - prev: tracks previous node (starts NULL)
 *    - curr: current node being processed
 *    - next: temporarily stores next node
 *
 * 3. REVERSE DIRECTION:
 *    - Change curr->next to point to prev
 *    - Move all pointers one step forward
 *
 * ============================================================================
 * VISUAL WALKTHROUGH:
 * ============================================================================
 *
 * Original: 1 -> 2 -> 3 -> NULL
 *
 * Initial state:
 *   prev = NULL
 *   curr = 1
 *   next = ?
 *
 *   NULL   1 -> 2 -> 3 -> NULL
 *    ^     ^
 *   prev  curr
 *
 * Step 1: Save next, reverse link
 *   next = curr->next (save 2)
 *   curr->next = prev (1 points to NULL)
 *   prev = curr (prev moves to 1)
 *   curr = next (curr moves to 2)
 *
 *   NULL <- 1    2 -> 3 -> NULL
 *           ^    ^
 *          prev curr
 *
 * Step 2: Repeat
 *   next = 3
 *   curr->next = prev (2 points to 1)
 *   prev = 2
 *   curr = 3
 *
 *   NULL <- 1 <- 2    3 -> NULL
 *                ^    ^
 *               prev curr
 *
 * Step 3: Repeat
 *   next = NULL
 *   curr->next = prev (3 points to 2)
 *   prev = 3
 *   curr = NULL
 *
 *   NULL <- 1 <- 2 <- 3
 *                     ^
 *                    prev (new head!)
 *
 * curr is NULL, STOP
 * Return prev as new head
 *
 * Result: 3 -> 2 -> 1 -> NULL
 *
 * ============================================================================
 * POINTER MOVEMENT VISUALIZATION:
 * ============================================================================
 *
 *   +------+    +------+    +------+
 *   |  1   | -> |  2   | -> |  3   | -> NULL
 *   +------+    +------+    +------+
 *
 *   Before each iteration:
 *   1. Save: next = curr->next
 *   2. Reverse: curr->next = prev
 *   3. Move prev: prev = curr
 *   4. Move curr: curr = next
 *
 *   +------+    +------+    +------+
 *   |  1   | <- |  2   | <- |  3   |
 *   +------+    +------+    +------+
 *   NULL <-          new head ->
 *
 * ============================================================================
 * ALGORITHM:
 * ============================================================================
 *
 *   prev = NULL
 *   curr = head
 *
 *   while (curr != NULL):
 *       next = curr->next     // Save
 *       curr->next = prev     // Reverse
 *       prev = curr           // Move prev
 *       curr = next           // Move curr
 *
 *   return prev  // New head
 *
 * ============================================================================
 * TIME COMPLEXITY: O(n)
 * ============================================================================
 * - Visit each node exactly once
 * - Constant operations per node
 *
 * ============================================================================
 * SPACE COMPLEXITY: O(1)
 * ============================================================================
 * - Only three pointers
 * - In-place reversal
 *
 * ============================================================================
 * RECURSIVE VERSION:
 * ============================================================================
 *
 *   Node* reverseRecursive(Node* head) {
 *       if (head == NULL || head->next == NULL)
 *           return head;
 *
 *       Node* newHead = reverseRecursive(head->next);
 *       head->next->next = head;
 *       head->next = NULL;
 *
 *       return newHead;
 *   }
 *
 *   Space: O(n) due to recursion stack
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
 *     int data;
 *     struct Node* next;
 * } Node;
 *
 * LINE 1: typedef struct Node {
 * ---------------------------------
 * - "typedef" = We're creating a new type name (alias)
 * - "struct Node" = We're defining a structure AND giving it a tag name "Node"
 * - The "{" opens the structure definition
 * - We use "struct Node" as the tag so we can reference it inside itself (for next pointer)
 *
 * LINE 2: int data;
 * ---------------------------------
 * - This is the PAYLOAD - the actual value stored in the node
 * - Can be any type: int, char, float, or even another struct
 * - Each node carries one piece of data
 *
 * LINE 3: struct Node* next;
 * ---------------------------------
 * - This is the LINK - pointer to the next node in the list
 * - "struct Node*" = pointer to another Node structure
 * - We MUST say "struct Node*" here (not just "Node*") because
 *   the typedef alias "Node" isn't complete yet at this point!
 * - This is what makes it a "linked" list - nodes point to each other
 * - Last node has next = NULL (end of list marker)
 *
 * LINE 4: } Node;
 * ---------------------------------
 * - "}" closes the structure definition
 * - "Node" after the "}" completes the typedef
 * - Now we can use "Node" as a type instead of "struct Node"
 * - Example: Node myNode; instead of struct Node myNode;
 *
 * MEMORY LAYOUT (assuming 64-bit system):
 * ---------------------------------
 *   +------------------+------------------+
 *   |      data        |      next        |
 *   |    (4 bytes)     |    (8 bytes)     |
 *   +------------------+------------------+
 *   Total: 12-16 bytes (with padding)
 *
 * VISUAL REPRESENTATION:
 * ---------------------------------
 *   Node A          Node B          Node C
 *   +------+---+    +------+---+    +------+------+
 *   | data |  -+--->| data |  -+--->| data | NULL |
 *   +------+---+    +------+---+    +------+------+
 *      10              20              30
 *
 * ============================================================================
 */

// Node structure definition
typedef struct Node {
    // The data field stores the actual value (payload) of this node
    // Say: "Each node has a data field to store the integer value"
    int data;

    // The next pointer stores the address of the next node in the list
    // Say: "And a next pointer that points to the next node, or NULL if this is the last node"
    // NOTE: We use "struct Node*" because the typedef "Node" isn't complete yet
    struct Node* next;
} Node;  // "Node" is now an alias for "struct Node"

/*
 * ============================================================================
 * CREATE NODE FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* createNode(int data) {
 *     Node* newNode = (Node*)malloc(sizeof(Node));
 *     newNode->data = data;
 *     newNode->next = NULL;
 *     return newNode;
 * }
 *
 * LINE 1: Node* createNode(int data) {
 * ---------------------------------
 * - "Node*" = This function RETURNS a pointer to a Node
 * - "createNode" = Function name (descriptive!)
 * - "int data" = The value we want to store in the new node
 * - This is a FACTORY FUNCTION - it creates and returns new nodes
 *
 * LINE 2: Node* newNode = (Node*)malloc(sizeof(Node));
 * ---------------------------------
 * - "Node* newNode" = Declare a pointer that will hold address of new node
 * - "malloc(sizeof(Node))" = Request memory from heap for one Node
 *   - sizeof(Node) = calculates exact bytes needed for one Node structure
 *   - malloc returns void* (generic pointer) to the allocated memory
 *   - Returns NULL if allocation fails (out of memory)
 * - "(Node*)" = Cast the void* to Node* (required in C++)
 * - The node is allocated on the HEAP (persists after function returns)
 *   - Unlike local variables which are on STACK (destroyed after function)
 *
 * WHY HEAP ALLOCATION?
 * - Stack memory is freed when function returns
 * - Heap memory persists until we explicitly free() it
 * - Linked list nodes must outlive the function that creates them
 *
 * LINE 3: newNode->data = data;
 * ---------------------------------
 * - "newNode->data" = Access the 'data' field of the node pointed to by newNode
 * - "->" is the arrow operator: shorthand for (*newNode).data
 * - "= data" = Copy the parameter value into the node's data field
 * - This stores the actual value in our new node
 *
 * LINE 4: newNode->next = NULL;
 * ---------------------------------
 * - "newNode->next" = Access the 'next' pointer field
 * - "= NULL" = Set it to NULL (no next node yet)
 * - IMPORTANT: Always initialize pointers! Uninitialized pointers = bugs
 * - New nodes are "orphans" - not connected to any list yet
 * - The caller will set 'next' to link this node into a list
 *
 * LINE 5: return newNode;
 * ---------------------------------
 * - Return the pointer to our newly created node
 * - Caller receives the address of the node on the heap
 * - Caller is responsible for:
 *   1. Linking this node into a list
 *   2. Eventually calling free() to prevent memory leaks
 *
 * MEMORY VISUALIZATION:
 * ---------------------------------
 *
 *   BEFORE malloc:
 *   Stack                    Heap
 *   +--------+              (empty)
 *   | data=5 |
 *   +--------+
 *   |newNode |---> ???
 *   +--------+
 *
 *   AFTER malloc:
 *   Stack                    Heap
 *   +--------+              +------+------+
 *   | data=5 |              | ???  | ???  |
 *   +--------+              +------+------+
 *   |newNode |------------->| 0x100       |
 *   +--------+              +------+------+
 *
 *   AFTER initialization:
 *   Stack                    Heap
 *   +--------+              +------+------+
 *   | data=5 |              |  5   | NULL |
 *   +--------+              +------+------+
 *   |newNode |------------->| 0x100       |
 *   +--------+              +------+------+
 *
 * ============================================================================
 */

// Create a new node with given data
Node* createNode(int data) {
    // Allocate memory on the HEAP for one Node structure
    // malloc returns pointer to allocated memory, or NULL if failed
    // sizeof(Node) calculates the exact number of bytes needed
    // (Node*) casts the void* returned by malloc to Node*
    // Say: "First, I allocate memory for a new node using malloc"
    Node* newNode = (Node*)malloc(sizeof(Node));

    // OPTIONAL: Check if malloc succeeded (good practice!)
    // if (newNode == NULL) return NULL;

    // Store the data value in the node's data field
    // The -> operator accesses struct members through a pointer
    // This is equivalent to: (*newNode).data = data;
    // Say: "Then I store the data value in the node's data field"
    newNode->data = data;

    // Initialize next pointer to NULL (not connected to any node yet)
    // CRITICAL: Always initialize pointers to avoid undefined behavior
    // Say: "I set the next pointer to NULL since this node isn't linked yet"
    newNode->next = NULL;

    // Return pointer to the newly created node
    // Caller will link this node into a list
    // Say: "Finally, I return the pointer to the new node"
    return newNode;
}

// Iterative reversal (PREFERRED)
Node* reverseIterative(Node* head) {
    // Initialize previous pointer to NULL
    // Say: "I'll use three pointers: prev, curr, and next"
    Node* prev = NULL;

    // Current pointer starts at head
    // Say: "Current starts at the head of the list"
    Node* curr = head;

    // Next pointer for temporary storage
    // Say: "Next will temporarily store the node ahead"
    Node* next = NULL;

    // Traverse until we reach the end
    // Say: "I'll iterate through the list until curr becomes NULL"
    while (curr != NULL) {
        // Save the next node before we change pointers
        // Say: "First, save the next node so we don't lose the rest of the list"
        next = curr->next;

        // Reverse the current node's pointer
        // Say: "Now reverse the link by pointing current's next to prev"
        curr->next = prev;

        // Move prev forward one step
        // Say: "Move prev forward to current node"
        prev = curr;

        // Move curr forward one step
        // Say: "Move curr forward to the next node we saved earlier"
        curr = next;
    }

    // When loop ends, prev points to new head
    // Say: "When we exit the loop, prev is pointing to the new head"
    return prev;
}

// Recursive reversal
Node* reverseRecursive(Node* head) {
    // Base case: empty list or single node
    // Say: "Base case: if empty or single node, just return it"
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // Recursively reverse rest of list
    // Say: "Recursively reverse everything after the current node"
    Node* newHead = reverseRecursive(head->next);

    // Make next node point back to current
    // Say: "Make the next node point back to me"
    head->next->next = head;

    // Current node becomes tail, points to NULL
    // Say: "Current node is now at the end, so point to NULL"
    head->next = NULL;

    // Return the new head
    // Say: "Return the new head which came from recursion"
    return newHead;
}

/*
 * ============================================================================
 * PRINT LIST FUNCTION - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * void printList(Node* head):
 *   - "void" = This function doesn't return anything, just prints
 *   - "Node* head" = Takes pointer to the first node of the list
 *
 * Node* curr = head:
 *   - Create a traversal pointer starting at head
 *   - We use 'curr' instead of modifying 'head' directly
 *   - This preserves the original head pointer
 *
 * while (curr != NULL):
 *   - Continue looping until we reach the end of the list
 *   - NULL marks the end (last node's next is NULL)
 *
 * printf("%d", curr->data):
 *   - Print the data value of the current node
 *   - curr->data accesses the data field through the pointer
 *
 * if (curr->next != NULL) printf(" -> "):
 *   - Print arrow separator only if there's a next node
 *   - This prevents printing " -> " after the last node
 *
 * curr = curr->next:
 *   - Move to the next node in the list
 *   - This is how we traverse the linked list
 *
 * printf(" -> NULL\n"):
 *   - Print NULL at the end to show the list terminates
 *   - Visual representation of the list ending
 *
 * ============================================================================
 */
// Print linked list - traverses and prints each node's data
void printList(Node* head) {
    // Create a traversal pointer - don't modify head directly!
    // Say: "I'll use a curr pointer to traverse without losing the head"
    Node* curr = head;

    // Traverse until we reach the end (NULL)
    // Say: "I loop through each node until curr becomes NULL"
    while (curr != NULL) {
        // Print the current node's data value
        // Say: "Print the data value of the current node"
        printf("%d", curr->data);

        // Print arrow if there's a next node (for formatting)
        // Say: "If there's a next node, print an arrow separator"
        if (curr->next != NULL) {
            printf(" -> ");
        }

        // Move to the next node
        // Say: "Move curr to point to the next node"
        curr = curr->next;
    }

    // Print NULL to show end of list
    // Say: "Print NULL to indicate the end of the list"
    printf(" -> NULL\n");
}

/*
 * ============================================================================
 * CREATE LIST FROM ARRAY - LINE BY LINE EXPLANATION
 * ============================================================================
 *
 * Node* createList(int arr[], int n):
 *   - Returns pointer to the head of the newly created list
 *   - "int arr[]" = Array of values to convert to linked list
 *   - "int n" = Number of elements in the array
 *
 * if (n == 0) return NULL:
 *   - Edge case: empty array means empty list
 *   - Return NULL to represent an empty linked list
 *
 * Node* head = createNode(arr[0]):
 *   - Create the first node with the first array element
 *   - This becomes the head of our list
 *
 * Node* curr = head:
 *   - Create a pointer to track the current (last) node
 *   - We'll use this to append new nodes
 *
 * for (int i = 1; i < n; i++):
 *   - Start from index 1 (we already used index 0 for head)
 *   - Loop through remaining array elements
 *
 * curr->next = createNode(arr[i]):
 *   - Create a new node with the current array element
 *   - Link it to the current node's next pointer
 *   - This appends the new node to the end of the list
 *
 * curr = curr->next:
 *   - Move curr to point to the newly added node
 *   - Now curr is at the end of the list again
 *
 * return head:
 *   - Return the pointer to the first node
 *   - Caller can access entire list through this head pointer
 *
 * VISUALIZATION of createList([1, 2, 3], 3):
 * ---------------------------------
 *
 *   After head = createNode(arr[0]):
 *   head -> [1|NULL]
 *   curr -> [1|NULL]
 *
 *   After i=1 (arr[1]=2):
 *   head -> [1| ]-->[2|NULL]
 *   curr ---------> [2|NULL]
 *
 *   After i=2 (arr[2]=3):
 *   head -> [1| ]-->[2| ]-->[3|NULL]
 *   curr -----------------> [3|NULL]
 *
 *   Return head (pointing to node with 1)
 *
 * ============================================================================
 */
// Create a linked list from an array of values
Node* createList(int arr[], int n) {
    // Edge case: empty array means empty list
    // Say: "If the array is empty, return NULL for an empty list"
    if (n == 0) return NULL;

    // Create the head node with the first array element
    // Say: "Create the first node with arr[0] as the head"
    Node* head = createNode(arr[0]);

    // Create a pointer to track the current (last) node
    // Say: "Use curr to keep track of where to append new nodes"
    Node* curr = head;

    // Loop through remaining array elements (starting from index 1)
    // Say: "For each remaining element, create a node and link it"
    for (int i = 1; i < n; i++) {
        // Create new node and link it to current node's next
        // Say: "Create a new node and attach it to curr's next pointer"
        curr->next = createNode(arr[i]);

        // Move curr to the newly created node (new end of list)
        // Say: "Move curr forward to the newly added node"
        curr = curr->next;
    }

    // Return head pointer - provides access to entire list
    // Say: "Return the head pointer to access the complete list"
    return head;
}

int main() {
    printf("=== Reverse Linked List ===\n\n");

    // Test iterative
    int arr1[] = {1, 2, 3, 4, 5};
    Node* list1 = createList(arr1, 5);

    printf("Original: ");
    printList(list1);

    list1 = reverseIterative(list1);
    printf("Reversed (iterative): ");
    printList(list1);

    // Test recursive
    int arr2[] = {10, 20, 30};
    Node* list2 = createList(arr2, 3);

    printf("\nOriginal: ");
    printList(list2);

    list2 = reverseRecursive(list2);
    printf("Reversed (recursive): ");
    printList(list2);

    // Single element
    int arr3[] = {42};
    Node* list3 = createList(arr3, 1);

    printf("\nSingle element: ");
    printList(list3);
    list3 = reverseIterative(list3);
    printf("Reversed: ");
    printList(list3);

    return 0;
}
