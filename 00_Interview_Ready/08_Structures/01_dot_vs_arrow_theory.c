/*
 * ============================================================================
 *            STRUCTURES IN C: . (DOT) vs -> (ARROW) OPERATORS
 * ============================================================================
 *
 * THE GOLDEN RULE:
 *   Node x;   -> x IS struct    -> USE x.data   (DOT)
 *   Node* x;  -> x is POINTER   -> USE x->data  (ARROW)
 *
 * MEMORY TRICK: "Has asterisk? Use arrow. No asterisk? Use dot."
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

int main() {
    // CASE 1: Stack struct (no asterisk) -> USE DOT
    Node n1;
    n1.data = 100;       // DOT - n1 IS the struct
    n1.next = NULL;

    // CASE 2: Heap pointer (has asterisk) -> USE ARROW
    Node* ptr = malloc(sizeof(Node));
    ptr->data = 200;     // ARROW - ptr POINTS TO struct
    ptr->next = NULL;

    // CASE 3: Pointer to stack struct -> USE ARROW
    Node* p = &n1;
    p->data = 300;       // ARROW - p is pointer (even though it points to stack)

    // CASE 4: The linked list dummy pattern
    Node dummy;          // Stack struct
    dummy.next = NULL;   // DOT - dummy IS struct
    Node* tail = &dummy; // Pointer to dummy
    tail->next = ptr;    // ARROW - tail is pointer

    printf("n1.data = %d (DOT)\n", n1.data);
    printf("ptr->data = %d (ARROW)\n", ptr->data);
    printf("dummy.next->data = %d\n", dummy.next->data);

    free(ptr);
    return 0;
}

/*
 * ============================================================================
 * DRY RUN: The Dummy Head Pattern
 * ============================================================================
 *
 * Code:
 *   Node dummy;           // Stack struct
 *   dummy.next = NULL;    // DOT
 *   Node* tail = &dummy;  // Pointer to stack struct
 *   tail->next = newNode; // ARROW
 *
 * Memory Layout:
 *
 *   STACK:
 *   +--------+
 *   | tail   | ----+
 *   | 0x2000 |     |     (tail holds address of dummy)
 *   +--------+     |
 *                  v
 *   +--------+--------+
 *   | dummy           |
 *   | data  |  next   |
 *   |  ??   |  NULL   |
 *   +--------+--------+
 *   address 0x2000
 *
 * WHY dummy.next (DOT)?
 *   Declaration: Node dummy;  <- No asterisk!
 *   dummy IS the struct, not a pointer.
 *
 * WHY tail->next (ARROW)?
 *   Declaration: Node* tail;  <- Has asterisk!
 *   tail is a pointer to the struct.
 *
 * BOTH ACCESS THE SAME MEMORY!
 *   dummy.next and tail->next refer to the same field
 *   because tail = &dummy (tail points to dummy).
 *
 * ============================================================================
 * QUICK REFERENCE:
 *   Node x;      -> x.data    (DOT)
 *   Node* x;     -> x->data   (ARROW)
 *   Node arr[n]; -> arr[i].data (DOT - arr[i] is struct, not pointer)
 *
 * ARROW IS SHORTHAND:
 *   ptr->data  ===  (*ptr).data
 * ============================================================================
 */
