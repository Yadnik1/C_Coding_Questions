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
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Is stack method better than two-pointer for reversing?"
 * A1: No! Two-pointer is O(1) space, stack uses O(n) space.
 *     But this demonstrates stack's LIFO property clearly.
 *
 * -------------------------------------------------------------------------
 * Q2: "When would you actually use stack for string reversal?"
 * A2: When reversing is part of a larger stack-based algorithm,
 *     like expression evaluation or parentheses matching.
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you reverse words in a sentence using stacks?"
 * A3: Yes! Use two stacks or one stack twice:
 *     1. Push chars to reverse string
 *     2. Push chars of each word to reverse words back
 *
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

// Define maximum stack size
// Say: "I'll define the maximum size for the character stack"
#define MAX_SIZE 100

// Stack structure for characters
// Say: "I'm creating a stack struct to hold characters"
typedef struct {
    char arr[MAX_SIZE];  // Array to store characters
    int top;             // Index of top element
} Stack;

// Initialize stack to empty
// Say: "This function initializes the stack to empty"
void initStack(Stack* s) {
    // Set top to -1 for empty stack
    // Say: "I set top to -1 to indicate an empty stack"
    s->top = -1;
}

// Check if stack is empty
// Say: "This checks if the stack has no elements"
int isEmpty(Stack* s) {
    // Return true if top is -1
    // Say: "I return true when top equals -1"
    return s->top == -1;
}

// Push character onto stack
// Say: "This function pushes a character onto the stack"
void push(Stack* s, char c) {
    // Increment top and store character
    // Say: "I increment top and store the character at that position"
    s->arr[++s->top] = c;
}

// Pop character from stack
// Say: "This function pops and returns the top character"
char pop(Stack* s) {
    // Return character at top and decrement
    // Say: "I return the character at top, then decrement top"
    return s->arr[s->top--];
}

// Reverse string using stack
// Say: "Now I'll implement the main reverseString function"
void reverseString(char str[]) {
    // Declare stack variable
    // Say: "I declare a stack to temporarily hold the characters"
    Stack s;

    // Initialize the stack
    // Say: "I initialize the stack to empty state"
    initStack(&s);

    // Push all characters onto stack
    // Say: "Now I'll push all characters from the string onto the stack"
    for (int i = 0; str[i] != '\0'; i++) {
        // Push current character
        // Say: "I push each character one by one"
        push(&s, str[i]);
    }

    // Pop all characters back to string
    // Say: "Now I pop all characters back, which reverses them due to LIFO"
    for (int i = 0; !isEmpty(&s); i++) {
        // Pop character and store in string
        // Say: "I pop each character and place it back in the string"
        str[i] = pop(&s);
    }
}

int main() {
    // Say: "Let me demonstrate string reversal using a stack"
    printf("=== Reverse String Using Stack ===\n\n");

    // First test string
    // Say: "I'll create the first test string 'hello'"
    char str1[] = "hello";

    // Show original string
    // Say: "Here's the original string"
    printf("Original: \"%s\"\n", str1);

    // Reverse the string
    // Say: "Now I reverse it using the stack"
    reverseString(str1);

    // Show reversed string
    // Say: "And here's the reversed result"
    printf("Reversed: \"%s\"\n", str1);

    // Second test string
    // Say: "Let me try another example with 'stack'"
    char str2[] = "stack";

    // Show original
    // Say: "Here's the original string"
    printf("\nOriginal: \"%s\"\n", str2);

    // Reverse it
    // Say: "I reverse this one too"
    reverseString(str2);

    // Show reversed
    // Say: "And the result is the reversed string"
    printf("Reversed: \"%s\"\n", str2);

    return 0;
}
