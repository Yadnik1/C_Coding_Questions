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

// Alternative: Manual substring check without library functions
bool is_substring(char *haystack, char *needle) {
    // Say: "Simple substring search - O(n*m) but sufficient for interview"
    int h_len = strlen(haystack);
    int n_len = strlen(needle);

    if (n_len > h_len) return false;

    for (int i = 0; i <= h_len - n_len; i++) {
        int j;
        for (j = 0; j < n_len; j++) {
            if (haystack[i + j] != needle[j]) {
                break;
            }
        }
        if (j == n_len) {
            return true;  // Say: "Found complete match"
        }
    }
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
