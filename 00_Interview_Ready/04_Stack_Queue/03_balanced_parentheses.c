// Check Balanced Parentheses using Stack - Classic interview problem
// Time: O(n), Space: O(n)

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 100

// Say: "Simple stack for characters"
typedef struct {
    char data[MAX_SIZE];
    int top;
} Stack;

void stack_init(Stack *s) { s->top = -1; }
bool is_empty(Stack *s) { return s->top == -1; }
void push(Stack *s, char c) { s->data[++s->top] = c; }
char pop(Stack *s) { return is_empty(s) ? '\0' : s->data[s->top--]; }
char peek(Stack *s) { return is_empty(s) ? '\0' : s->data[s->top]; }

// Say: "Helper to check if characters are matching pair"
bool is_matching(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

// Say: "Check if expression has balanced brackets"
bool is_balanced(char *expr) {
    Stack s;
    stack_init(&s);

    // Say: "Scan expression character by character"
    for (int i = 0; expr[i] != '\0'; i++) {
        char c = expr[i];

        // Say: "If opening bracket, push to stack"
        if (c == '(' || c == '[' || c == '{') {
            push(&s, c);
        }
        // Say: "If closing bracket, check for match"
        else if (c == ')' || c == ']' || c == '}') {
            // Say: "Empty stack means no opening bracket to match"
            if (is_empty(&s)) {
                return false;
            }
            // Say: "Pop and check if it matches"
            char top = pop(&s);
            if (!is_matching(top, c)) {
                return false;  // Say: "Mismatched brackets"
            }
        }
        // Say: "Ignore non-bracket characters"
    }

    // Say: "Stack should be empty if all brackets matched"
    return is_empty(&s);
}

int main() {
    char *test_cases[] = {
        "{[()]}",         // Balanced
        "{[(])}",         // Not balanced - wrong order
        "((()))",         // Balanced
        "(()))",          // Not balanced - extra closing
        "((())",          // Not balanced - missing closing
        "{a + (b * c)}",  // Balanced with other chars
        "",               // Empty - balanced
        "({[]})",         // Balanced
    };

    int n = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < n; i++) {
        printf("'%s' -> %s\n",
               test_cases[i],
               is_balanced(test_cases[i]) ? "Balanced" : "Not Balanced");
    }

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use a stack to match opening and closing brackets.

 ALGORITHM:
 1. Scan expression left to right
 2. Opening bracket? Push to stack
 3. Closing bracket? Pop and check match
 4. After scanning, stack should be empty

 WHY STACK WORKS:
 - Opening brackets need to match in REVERSE order
 - LIFO property of stack handles this naturally
 - Last opened bracket must be first closed

 EXAMPLE WALKTHROUGH for '{[()]}':
 - '{': push -> stack: [{]
 - '[': push -> stack: [{, []
 - '(': push -> stack: [{, [, (]
 - ')': pop '(' matches ')' -> stack: [{, []
 - ']': pop '[' matches ']' -> stack: [{]
 - '}': pop '{' matches '}' -> stack: []
 - Stack empty -> Balanced!

 FAILURE CASES:
 1. Extra closing: ')' when stack empty
 2. Mismatched: '(' with ']'
 3. Extra opening: stack not empty at end

 COMMON MISTAKES:
 - Forgetting to check stack empty before pop
 - Not checking stack empty at end
 - Only handling one type of bracket

 VARIATIONS:
 - Return index of mismatch
 - Count minimum brackets to add to balance
 - Generate all balanced combinations

 TIME/SPACE:
 - Time: O(n) - single pass
 - Space: O(n) - worst case all opening brackets"
*/
