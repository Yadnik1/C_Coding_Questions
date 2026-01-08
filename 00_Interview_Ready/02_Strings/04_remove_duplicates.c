/*
 * ============================================================================
 * PROBLEM: Remove Duplicate Characters from String (In-Place)
 * ============================================================================
 *
 * WHAT IS THIS PROBLEM?
 * Given a string, remove all duplicate characters, keeping only the FIRST
 * occurrence of each character.
 *
 * EXAMPLES:
 * - "programming" -> "progamin"  (removes 2nd 'r', 2nd 'g', 2nd 'm')
 * - "aabbccdd"    -> "abcd"      (removes all repeated chars)
 * - "hello"       -> "helo"      (removes 2nd 'l')
 * - "abcdef"      -> "abcdef"    (no duplicates, unchanged)
 *
 * WHY IS THIS ASKED IN INTERVIEWS?
 * - Tests the "read/write pointer" technique (very common pattern!)
 * - Shows understanding of in-place array modification
 * - Tests knowledge of using arrays as hash sets
 * - Embedded systems often need to remove duplicates from sensor data
 *
 * LIBRARY FUNCTIONS USED:
 * - stdbool.h: Provides bool, true, false
 *
 * KEY CONCEPT - READ/WRITE POINTERS:
 * - READ pointer scans through entire string
 * - WRITE pointer tracks where to put the next unique character
 * - WRITE is always <= READ (we never overwrite unread data)
 *
 * VISUAL for "aabbcc":
 *   Initial: a a b b c c
 *            ^
 *            R,W (both start at 0)
 *
 *   Step 1: 'a' is new, copy to write, advance both
 *            a a b b c c
 *              ^ ^
 *              W R
 *
 *   Step 2: 'a' already seen, skip (only advance R)
 *            a a b b c c
 *              ^   ^
 *              W   R
 *
 *   Step 3: 'b' is new, copy to W, advance both
 *            a b b b c c
 *                ^ ^
 *                W R
 *   ... and so on until R reaches end
 *
 *   Final:  a b c \0
 *                  ^ (null terminate at W)
 *
 * ============================================================================
 */

// Time: O(n), Space: O(1) - using fixed size seen array

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void remove_duplicates(char *str) {
    // Say: "I'll use a write pointer and a boolean array to track seen characters"
    // Say: "Read pointer scans string, write pointer tracks where to put unique chars"

    if (str == NULL || str[0] == '\0') {
        return;  // Say: "Handle empty or null string"
    }

    // Say: "Boolean array to track which characters we've seen"
    // Say: "256 covers all ASCII characters, this is O(1) space"
    bool seen[256] = {false};

    int write = 0;  // Say: "Write pointer - where to place next unique char"

    // Say: "Read through entire string"
    for (int read = 0; str[read] != '\0'; read++) {
        // Say: "Get ASCII value of current character as array index"
        int index = (unsigned char)str[read];  // Say: "Cast to handle extended ASCII"

        if (!seen[index]) {
            // Say: "First time seeing this char - keep it"
            seen[index] = true;           // Say: "Mark as seen"
            str[write] = str[read];       // Say: "Copy char to write position"
            write++;                       // Say: "Advance write pointer"
        }
        // Say: "If already seen, just skip it (don't increment write)"
    }

    // Say: "Null terminate the string at new length"
    str[write] = '\0';
}

// Alternative: O(n^2) solution without extra space (for interviews asking O(1) without array)
void remove_duplicates_no_array(char *str) {
    // Say: "For each char, check if it appeared before"
    // Say: "This is truly O(1) space but O(n^2) time"

    if (str == NULL || str[0] == '\0') return;

    int write = 1;  // Say: "First char is always unique, start write at 1"

    for (int read = 1; str[read] != '\0'; read++) {
        int j;
        // Say: "Check if current char exists in the kept portion [0, write)"
        for (j = 0; j < write; j++) {
            if (str[j] == str[read]) {
                break;  // Say: "Found duplicate"
            }
        }

        if (j == write) {
            // Say: "No duplicate found - keep this char"
            str[write] = str[read];
            write++;
        }
    }

    str[write] = '\0';
}

int main() {
    char str1[] = "programming";
    printf("Original: %s\n", str1);
    remove_duplicates(str1);
    printf("After removing duplicates: %s\n", str1);  // Expected: "progamin"

    char str2[] = "aabbccdd";
    printf("\nOriginal: %s\n", str2);
    remove_duplicates(str2);
    printf("After removing duplicates: %s\n", str2);  // Expected: "abcd"

    char str3[] = "abcdef";
    printf("\nOriginal: %s\n", str3);
    remove_duplicates(str3);
    printf("After removing duplicates: %s\n", str3);  // Expected: "abcdef" (no change)

    // Test the O(n^2) version
    char str4[] = "hello";
    printf("\nOriginal: %s\n", str4);
    remove_duplicates_no_array(str4);
    printf("After removing duplicates (O(n^2)): %s\n", str4);  // Expected: "helo"

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I'll remove duplicates in-place using a write pointer and a seen array.

 APPROACH:
 - Use a boolean array of size 256 to track seen characters
 - Read pointer scans through string character by character
 - Write pointer tracks where to place the next unique character
 - If char not seen: mark seen, copy to write position, advance write
 - If char seen: skip it (read advances, write stays)
 - Null terminate at write position when done

 WHY WRITE POINTER TECHNIQUE:
 - In-place modification without extra string
 - Write pointer always <= read pointer
 - Original string space is reused efficiently

 WHY BOOLEAN ARRAY OF 256:
 - ASCII has 256 possible characters
 - O(1) lookup to check if seen
 - Fixed size = O(1) space complexity
 - For just lowercase letters, 26 would suffice

 EDGE CASES:
 - Empty string: return immediately
 - All unique: write follows read, no change
 - All same: only first char kept
 - Null pointer: check and return

 TRADE-OFFS:
 - O(n) time with 256-size array: practical O(1) space
 - O(n^2) time with nested loops: truly O(1) space
 - O(n) time with hash set: O(k) space where k is unique chars

 COMPLEXITY:
 - Time: O(n) - single pass through string
 - Space: O(1) - fixed 256-element array, doesn't grow with input"
*/
