/*
 * ============================================================================
 * PROBLEM: Count and Say
 * ============================================================================
 *
 * DIFFICULTY: Easy | TIME: 10 mins | FREQUENCY: MEDIUM
 *
 * Generate the nth term of the "count and say" sequence.
 *
 * ============================================================================
 * SEQUENCE:
 * ============================================================================
 *
 *   n=1: "1"           (base case)
 *   n=2: "11"          (one 1)
 *   n=3: "21"          (two 1s)
 *   n=4: "1211"        (one 2, one 1)
 *   n=5: "111221"      (one 1, one 2, two 1s)
 *   n=6: "312211"      (three 1s, two 2s, one 1)
 *
 * ============================================================================
 * VISUALIZATION (n=4 → n=5):
 * ============================================================================
 *
 *   n=4: "1211"
 *
 *   Reading left to right:
 *   - "1"  → one 1    → "11"
 *   - "2"  → one 2    → "12"
 *   - "11" → two 1s   → "21"
 *
 *   n=5: "111221"
 *
 * ============================================================================
 * TIME: O(n * m) where m is length of string | SPACE: O(m)
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

#define MAX_LEN 5000

void countAndSay(int n, char* result) {
    // Base case
    strcpy(result, "1");

    if (n == 1) return;

    char current[MAX_LEN];
    char next[MAX_LEN];

    strcpy(current, "1");

    for (int i = 2; i <= n; i++) {
        int len = strlen(current);
        int nextIdx = 0;

        int j = 0;
        while (j < len) {
            char digit = current[j];
            int count = 1;

            // Count consecutive same digits
            while (j + count < len && current[j + count] == digit) {
                count++;
            }

            // Append count and digit to next
            if (count >= 10) {
                next[nextIdx++] = '0' + (count / 10);
                next[nextIdx++] = '0' + (count % 10);
            } else {
                next[nextIdx++] = '0' + count;
            }
            next[nextIdx++] = digit;

            j += count;
        }

        next[nextIdx] = '\0';
        strcpy(current, next);
    }

    strcpy(result, current);
}

// Print with explanation
void explainCountAndSay(const char* prev, const char* next) {
    printf("   \"%s\" → reading: ", prev);

    int len = strlen(prev);
    int j = 0;

    while (j < len) {
        char digit = prev[j];
        int count = 1;

        while (j + count < len && prev[j + count] == digit) {
            count++;
        }

        printf("%d×'%c' ", count, digit);
        j += count;
    }

    printf("→ \"%s\"\n", next);
}

int main() {
    printf("=== Count and Say ===\n\n");

    char result[MAX_LEN];
    char prev[MAX_LEN];

    printf("Sequence:\n");
    for (int n = 1; n <= 8; n++) {
        if (n > 1) strcpy(prev, result);
        countAndSay(n, result);

        if (n == 1) {
            printf("   n=%d: \"%s\" (base case)\n", n, result);
        } else {
            printf("   n=%d: ", n);
            explainCountAndSay(prev, result);
        }
    }

    printf("\n=== How to Read ===\n");
    printf("Look at previous string, count consecutive digits:\n");
    printf("   \"1211\" has: one 1, one 2, two 1s\n");
    printf("   Write counts: \"11\" + \"12\" + \"21\" = \"111221\"\n");

    return 0;
}
