/*
 * ============================================================================
 * PROBLEM: Reverse String Using Stack
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 5 mins | FREQUENCY: High
 *
 * Use stack's LIFO property to reverse a string.
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "hello"
 * ============================================================================
 *
 * Push all characters:
 *
 *   +---+
 *   | o | ← top
 *   +---+
 *   | l |
 *   +---+
 *   | l |
 *   +---+
 *   | e |
 *   +---+
 *   | h |
 *   +---+
 *
 * Pop all characters:
 *   Pop 'o' → "o"
 *   Pop 'l' → "ol"
 *   Pop 'l' → "oll"
 *   Pop 'e' → "olle"
 *   Pop 'h' → "olleh"
 *
 * Result: "olleh"
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(n)
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    char arr[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack* s) { s->top = -1; }
int isEmpty(Stack* s) { return s->top == -1; }
void push(Stack* s, char c) { s->arr[++s->top] = c; }
char pop(Stack* s) { return s->arr[s->top--]; }

void reverseString(char str[]) {
    Stack s;
    initStack(&s);

    // Push all characters
    for (int i = 0; str[i] != '\0'; i++) {
        push(&s, str[i]);
    }

    // Pop all characters back
    for (int i = 0; !isEmpty(&s); i++) {
        str[i] = pop(&s);
    }
}

int main() {
    printf("=== Reverse String Using Stack ===\n\n");

    char str1[] = "hello";
    printf("Original: \"%s\"\n", str1);
    reverseString(str1);
    printf("Reversed: \"%s\"\n", str1);

    char str2[] = "stack";
    printf("\nOriginal: \"%s\"\n", str2);
    reverseString(str2);
    printf("Reversed: \"%s\"\n", str2);

    return 0;
}
