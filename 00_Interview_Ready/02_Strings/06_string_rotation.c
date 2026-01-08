// Check if one string is rotation of another
// Time: O(n), Space: O(n) for concatenated string

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool is_rotation(char *str1, char *str2) {
    // Say: "Key insight: if str2 is rotation of str1, str2 exists in str1+str1"
    // Say: "For example: 'erbottlewat' is in 'waterbottle' + 'waterbottle'"

    int len1 = strlen(str1);
    int len2 = strlen(str2);

    // Say: "Rotations must have same length"
    if (len1 != len2 || len1 == 0) {
        return false;
    }

    // Say: "Create concatenated string str1 + str1"
    // Say: "Need space for 2*len1 + 1 (for null terminator)"
    char *concat = (char *)malloc(2 * len1 + 1);
    if (concat == NULL) {
        return false;  // Say: "Handle allocation failure"
    }

    // Say: "Build the concatenated string"
    strcpy(concat, str1);   // Say: "First copy of str1"
    strcat(concat, str1);   // Say: "Append second copy of str1"
    // Say: "Now concat = 'waterbottlewaterbottle'"

    // Say: "Check if str2 is a substring of the concatenated string"
    bool result = (strstr(concat, str2) != NULL);
    // Say: "strstr returns pointer to first occurrence, NULL if not found"

    free(concat);  // Say: "Don't forget to free allocated memory"

    return result;
}

// ============================================================
// Alternative: Manual substring check without library functions
// This implements strstr() from scratch - interviewers love this!
// ============================================================
//
// VISUAL EXAMPLE:
// haystack = "waterbottlewaterbottle"  (length 22)
// needle   = "erbottlewat"             (length 11)
//
// We slide a "window" of size 11 across haystack:
//
// i=0:  [waterbottle]waterbottle   vs "erbottlewat"  ✗ (w≠e at j=0)
// i=1:  w[aterbottlew]aterbottle   vs "erbottlewat"  ✗ (a≠e at j=0)
// i=2:  wa[terbottlewa]terbottle   vs "erbottlewat"  ✗ (t≠e at j=0)
// i=3:  wat[erbottlewat]erbottle   vs "erbottlewat"  ✓ MATCH!
//
// ============================================================
bool is_substring(char *haystack, char *needle) {
    // Say: "I implement a simple sliding window substring search"
    // Say: "This is O(n*m) but works well for interviews"

    int h_len = strlen(haystack);  // Say: "Length of string to search in"
    int n_len = strlen(needle);    // Say: "Length of pattern to find"

    // Say: "If needle is longer than haystack, impossible to find"
    if (n_len > h_len) return false;

    // Say: "i is the starting position in haystack for each comparison"
    // Say: "We only need to check up to position (h_len - n_len)"
    // Say: "Because beyond that, there aren't enough characters left"
    //
    // Example: haystack="abcdef" (6), needle="xyz" (3)
    //          Last valid i = 6 - 3 = 3 (positions 0,1,2,3)
    //          At i=3: check "def" vs "xyz"
    //          At i=4: only "ef" left - not enough chars!

    for (int i = 0; i <= h_len - n_len; i++) {
        // Say: "j is the index into needle (the pattern)"
        // Say: "I declare j outside the loop so I can check it after"
        int j;

        // Say: "Compare needle character by character with haystack"
        // Say: "Starting at position i in haystack"
        for (j = 0; j < n_len; j++) {
            // Say: "haystack[i + j] is the character in haystack"
            // Say: "needle[j] is the corresponding character in needle"
            //
            // Visual for i=3, checking "erbottlewat":
            //   j=0: haystack[3+0]='e' vs needle[0]='e' ✓
            //   j=1: haystack[3+1]='r' vs needle[1]='r' ✓
            //   j=2: haystack[3+2]='b' vs needle[2]='b' ✓
            //   ... continues until j=10 (all match)

            if (haystack[i + j] != needle[j]) {
                // Say: "Mismatch found! Break inner loop"
                // Say: "Try next starting position (i+1)"
                break;
            }
        }

        // Say: "After inner loop, check WHY we exited"
        // Say: "If j == n_len, we compared ALL characters successfully"
        // Say: "If j < n_len, we broke early due to mismatch"
        //
        // Example: needle = "abc" (n_len = 3)
        //   - If j=3 after loop: matched all 3 chars → FOUND!
        //   - If j=1 after loop: mismatch at position 1 → keep searching

        if (j == n_len) {
            return true;  // Say: "All characters matched! Found the substring"
        }

        // Say: "No match at position i, loop continues to try i+1"
    }

    // Say: "Tried all positions, needle not found in haystack"
    return false;
}

bool is_rotation_manual(char *str1, char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len1 != len2 || len1 == 0) {
        return false;
    }

    char *concat = (char *)malloc(2 * len1 + 1);
    if (concat == NULL) return false;

    strcpy(concat, str1);
    strcat(concat, str1);

    bool result = is_substring(concat, str2);

    free(concat);
    return result;
}

int main() {
    char *s1 = "waterbottle";
    char *s2 = "erbottlewat";
    printf("'%s' is rotation of '%s': %s\n", s2, s1,
           is_rotation(s1, s2) ? "true" : "false");  // Expected: true

    char *s3 = "hello";
    char *s4 = "llohe";
    printf("'%s' is rotation of '%s': %s\n", s4, s3,
           is_rotation(s3, s4) ? "true" : "false");  // Expected: true

    char *s5 = "hello";
    char *s6 = "lohel";
    printf("'%s' is rotation of '%s': %s\n", s6, s5,
           is_rotation(s5, s6) ? "true" : "false");  // Expected: false

    char *s7 = "abcd";
    char *s8 = "abdc";
    printf("'%s' is rotation of '%s': %s\n", s8, s7,
           is_rotation(s7, s8) ? "true" : "false");  // Expected: false

    return 0;
}

/*
INTERVIEW EXPLANATION:
"I use the clever concatenation trick to check for rotation.

 KEY INSIGHT:
 If str2 is a rotation of str1, then str2 must be a substring of str1 + str1.

 PROOF BY EXAMPLE:
 - str1 = 'waterbottle'
 - str2 = 'erbottlewat' (rotation point after 'wat')
 - str1 + str1 = 'waterbottlewaterbottle'
 - str2 appears in concatenated string starting at index 3!

 WHY THIS WORKS:
 - Rotation means we cut string at some point and swap parts
 - Original: [A][B], Rotated: [B][A]
 - Concatenated: [A][B][A][B] - contains [B][A] as substring!

 APPROACH:
 1. Check if lengths are equal (must be for rotation)
 2. Concatenate str1 with itself
 3. Check if str2 is substring of concatenated string
 4. Free allocated memory

 EDGE CASES:
 - Different lengths: not rotation
 - Empty strings: return false
 - Same string: is rotation of itself
 - Single character: 'a' is rotation of 'a'

 COMPLEXITY:
 - Time: O(n) - concatenation O(n) + substring search O(n) with strstr
 - Space: O(n) - need to store concatenated string

 INTERVIEW TIP:
 This is an elegant solution that shows problem-solving insight.
 Mention that strstr typically uses efficient algorithms internally."
*/
