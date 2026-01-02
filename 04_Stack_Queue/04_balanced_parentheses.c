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
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    char arr[MAX_SIZE];
    int top;
} Stack;

void initStack(Stack* s) { s->top = -1; }
bool isEmpty(Stack* s) { return s->top == -1; }
void push(Stack* s, char c) { s->arr[++s->top] = c; }
char pop(Stack* s) { return isEmpty(s) ? '\0' : s->arr[s->top--]; }

bool isOpening(char c) {
    return c == '(' || c == '[' || c == '{';
}

bool isClosing(char c) {
    return c == ')' || c == ']' || c == '}';
}

bool matches(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

bool isBalanced(char* expr) {
    Stack s;
    initStack(&s);

    for (int i = 0; expr[i] != '\0'; i++) {
        char c = expr[i];

        if (isOpening(c)) {
            push(&s, c);
        }
        else if (isClosing(c)) {
            if (isEmpty(&s)) return false;
            char top = pop(&s);
            if (!matches(top, c)) return false;
        }
    }

    return isEmpty(&s);
}

int main() {
    printf("=== Balanced Parentheses ===\n\n");

    char* tests[] = {
        "{[()]}",
        "{[(])}",
        "((()))",
        "([)]",
        "",
        "(((",
        ")))"
    };
    int n = 7;

    for (int i = 0; i < n; i++) {
        printf("\"%s\" → %s\n", tests[i],
               isBalanced(tests[i]) ? "BALANCED" : "NOT balanced");
    }

    return 0;
}
