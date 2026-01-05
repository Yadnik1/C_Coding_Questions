/*
 * ============================================================================
 * PROBLEM: Check Balanced Parentheses
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: VERY HIGH (Interview Favorite!)
 *
 * Check if expression has balanced brackets: (), [], {}
 *
 * Example:
 * Input:  "{[()]}"  → true
 * Input:  "{[(])}"  → false (wrong nesting)
 *
 * ============================================================================
 * WHAT YOU MUST KNOW:
 * ============================================================================
 *
 * 1. USE STACK:
 *    - Push opening brackets
 *    - Pop and match for closing brackets
 *
 * 2. MATCHING PAIRS:
 *    - '(' matches ')'
 *    - '[' matches ']'
 *    - '{' matches '}'
 *
 * ============================================================================
 * VISUAL WALKTHROUGH: "{[()]}"
 * ============================================================================
 *
 * Char '{': Opening → Push
 *   Stack: [{]
 *
 * Char '[': Opening → Push
 *   Stack: [{, []
 *
 * Char '(': Opening → Push
 *   Stack: [{, [, (]
 *
 * Char ')': Closing
 *   Pop '(' → matches ')' ✓
 *   Stack: [{, []
 *
 * Char ']': Closing
 *   Pop '[' → matches ']' ✓
 *   Stack: [{]
 *
 * Char '}': Closing
 *   Pop '{' → matches '}' ✓
 *   Stack: [] (empty)
 *
 * End: Stack empty → BALANCED!
 *
 * ============================================================================
 * TIME: O(n) | SPACE: O(n)
 * ============================================================================
 *
 * ============================================================================
 * COMMON INTERVIEW QUESTIONS & ANSWERS:
 * ============================================================================
 *
 * Q1: "Why use a stack for this problem?"
 * A1: Stack matches LIFO nature of bracket matching.
 *     Most recent opening bracket must match first closing bracket.
 *     Perfect stack use case!
 *
 * -------------------------------------------------------------------------
 * Q2: "What about only one type of bracket?"
 * A2: Just count! No stack needed. Increment for '(', decrement for ')'.
 *     Invalid if count goes negative. Valid if count ends at 0.
 *
 * -------------------------------------------------------------------------
 * Q3: "Can you solve without extra space?"
 * A3: Not for multiple bracket types! Need to remember opening sequence.
 *     For single type: yes, just use counter.
 *
 * -------------------------------------------------------------------------
 * Q4: "What if string contains other characters?"
 * A4: Simply ignore non-bracket characters - continue to next iteration.
 *     Only process '(', ')', '[', ']', '{', '}'.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Define maximum stack size
// Say: "I'll define the maximum size for the stack"
#define MAX_SIZE 100

// Stack structure for characters
// Say: "I create a stack struct to hold opening brackets"
typedef struct {
    char arr[MAX_SIZE];  // Array for bracket characters
    int top;             // Top index
} Stack;

// Initialize stack
// Say: "This initializes the stack to empty"
void initStack(Stack* s) {
    // Set top to -1
    // Say: "I set top to -1 for an empty stack"
    s->top = -1;
}

// Check if empty
// Say: "This checks if the stack is empty"
bool isEmpty(Stack* s) {
    // Return true if top is -1
    // Say: "I return true when top is -1"
    return s->top == -1;
}

// Push character
// Say: "This pushes a character onto the stack"
void push(Stack* s, char c) {
    // Increment top and store character
    // Say: "I increment top and store the character"
    s->arr[++s->top] = c;
}

// Pop character
// Say: "This pops and returns the top character"
char pop(Stack* s) {
    // Check if empty first
    // Say: "If the stack is empty, I return null character"
    if (isEmpty(s)) {
        return '\0';
    }
    // Return and decrement
    // Say: "Otherwise I return the top character and decrement top"
    return s->arr[s->top--];
}

// Check if character is opening bracket
// Say: "This helper function checks if a character is an opening bracket"
bool isOpening(char c) {
    // Check for (, [, or {
    // Say: "I return true if it's open paren, bracket, or brace"
    return c == '(' || c == '[' || c == '{';
}

// Check if character is closing bracket
// Say: "This checks if a character is a closing bracket"
bool isClosing(char c) {
    // Check for ), ], or }
    // Say: "I return true if it's close paren, bracket, or brace"
    return c == ')' || c == ']' || c == '}';
}

// Check if brackets match
// Say: "This function checks if an opening and closing bracket match"
bool matches(char open, char close) {
    // Check all three pairs
    // Say: "I check if they form a valid pair: (), [], or {}"
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

// Check if expression is balanced
// Say: "Now I'll implement the main function to check if brackets are balanced"
bool isBalanced(char* expr) {
    // Declare stack
    // Say: "I declare a stack to track opening brackets"
    Stack s;

    // Initialize stack
    // Say: "I initialize the stack to empty"
    initStack(&s);

    // Process each character
    // Say: "Now I'll iterate through each character in the expression"
    for (int i = 0; expr[i] != '\0'; i++) {
        // Get current character
        // Say: "I get the current character"
        char c = expr[i];

        // If opening bracket, push it
        // Say: "If it's an opening bracket, I push it onto the stack"
        if (isOpening(c)) {
            // Push opening bracket
            // Say: "I push this opening bracket"
            push(&s, c);
        }
        // If closing bracket, pop and check match
        // Say: "If it's a closing bracket, I need to match it with the top"
        else if (isClosing(c)) {
            // Check if stack is empty (no matching opening)
            // Say: "First I check if the stack is empty - if so, no match exists"
            if (isEmpty(&s)) {
                return false;
            }

            // Pop the top opening bracket
            // Say: "I pop the top opening bracket"
            char top = pop(&s);

            // Check if they match
            // Say: "Then I check if it matches the current closing bracket"
            if (!matches(top, c)) {
                // Mismatch found
                // Say: "If they don't match, the expression is not balanced"
                return false;
            }
        }
        // Ignore other characters
    }

    // Check if stack is empty at end
    // Say: "Finally, the stack must be empty for a balanced expression"
    return isEmpty(&s);
}

int main() {
    // Say: "Let me demonstrate balanced parentheses checking"
    printf("=== Balanced Parentheses ===\n\n");

    // Array of test cases
    // Say: "I'll create an array of test expressions"
    char* tests[] = {
        "{[()]}",   // Balanced
        "{[(])}",   // Not balanced
        "((()))",   // Balanced
        "([)]",     // Not balanced
        "",         // Empty - balanced
        "(((",      // Only opening - not balanced
        ")))"       // Only closing - not balanced
    };

    // Number of tests
    // Say: "I have 7 test cases to check"
    int n = 7;

    // Test each expression
    // Say: "Now I'll test each expression"
    for (int i = 0; i < n; i++) {
        // Check if balanced
        // Say: "For each one, I call isBalanced and display the result"
        printf("\"%s\" → %s\n", tests[i],
               isBalanced(tests[i]) ? "BALANCED" : "NOT balanced");
    }

    return 0;
}
