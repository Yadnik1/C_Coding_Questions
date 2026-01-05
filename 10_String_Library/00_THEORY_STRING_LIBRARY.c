/*
 * ============================================================================
 *                    STRING LIBRARY FUNCTIONS - COMPLETE THEORY
 * ============================================================================
 *
 * This file teaches you EVERYTHING you need to implement standard C string
 * and memory functions from scratch. Read this before attempting the problems.
 *
 * ============================================================================
 *                         PART 1: HOW STRINGS WORK IN C
 * ============================================================================
 *
 * A string in C is just an ARRAY OF CHARACTERS ending with '\0' (null terminator).
 *
 *   char str[] = "Hello";
 *
 *   Memory layout:
 *   +-----+-----+-----+-----+-----+------+
 *   | 'H' | 'e' | 'l' | 'l' | 'o' | '\0' |
 *   +-----+-----+-----+-----+-----+------+
 *   [0]   [1]   [2]   [3]   [4]   [5]
 *
 *   - The string "Hello" has length 5
 *   - But it uses 6 bytes of memory (5 chars + null terminator)
 *   - '\0' is ASCII value 0 (not the character '0' which is ASCII 48)
 *
 * ============================================================================
 * WHY THE NULL TERMINATOR?
 * ============================================================================
 *
 *   C has no way to know a string's length without scanning it!
 *   Unlike Python/Java, C doesn't store length metadata.
 *
 *   The null terminator '\0' tells functions "STOP HERE - string ends"
 *
 *   WITHOUT '\0':
 *   - strlen would keep counting past the string into random memory
 *   - printf would print garbage until it hits a random 0 byte
 *   - This causes crashes and security vulnerabilities!
 *
 * ============================================================================
 * CHAR VS UNSIGNED CHAR
 * ============================================================================
 *
 *   char can be SIGNED or UNSIGNED depending on compiler/platform!
 *
 *   Signed char:   -128 to 127
 *   Unsigned char:    0 to 255
 *
 *   Problem with extended ASCII (128-255):
 *   - Character 200 as signed char = -56
 *   - Comparing -56 < 65 gives WRONG result for sorting
 *
 *   Solution: Cast to unsigned char for comparisons:
 *     return (unsigned char)*s1 - (unsigned char)*s2;
 *
 * ============================================================================
 *                    PART 2: POINTERS AND STRINGS
 * ============================================================================
 *
 * POINTER BASICS:
 *
 *   char str[] = "Hello";
 *   char *p = str;        // p points to first character
 *
 *   +-----+-----+-----+-----+-----+------+
 *   | 'H' | 'e' | 'l' | 'l' | 'o' | '\0' |
 *   +-----+-----+-----+-----+-----+------+
 *     ^
 *     |
 *     p (p points here, *p == 'H')
 *
 * POINTER OPERATIONS:
 *
 *   *p      Get value at pointer location     (*p == 'H')
 *   p++     Move pointer to next element      (p now points to 'e')
 *   *p++    Get value THEN move pointer       (returns 'H', then p points to 'e')
 *   ++*p    Increment the VALUE at pointer    (changes 'H' to 'I')
 *
 * COMMON PATTERNS:
 *
 *   Pattern 1: Walk through string
 *   while (*p != '\0') {
 *       // do something with *p
 *       p++;
 *   }
 *
 *   Pattern 2: Copy string (compact)
 *   while ((*dest++ = *src++) != '\0');
 *   // This copies character AND advances both pointers
 *   // Loop exits after copying the null terminator
 *
 *   Pattern 3: Compare strings
 *   while (*s1 && *s1 == *s2) {  // *s1 is same as *s1 != '\0'
 *       s1++;
 *       s2++;
 *   }
 *   return *s1 - *s2;
 *
 * ============================================================================
 * POINTER ARITHMETIC
 * ============================================================================
 *
 *   char *start = str;
 *   char *end = str + 5;    // Points to '\0'
 *
 *   Length = end - start;   // Pointer subtraction gives element count!
 *
 *   This is why strlen_ptr works:
 *     const char *p = str;
 *     while (*p) p++;       // Find the end
 *     return p - str;       // Pointer difference = length
 *
 * ============================================================================
 *                    PART 3: VOID POINTERS (FOR mem* FUNCTIONS)
 * ============================================================================
 *
 * void* is a "generic pointer" - can point to ANY data type.
 *
 *   void *ptr;
 *   ptr = &myInt;      // OK
 *   ptr = &myChar;     // OK
 *   ptr = &myStruct;   // OK
 *
 * BUT you can't dereference void* directly:
 *   *ptr = 5;          // ERROR! Compiler doesn't know the size
 *
 * You must CAST it first:
 *   *(int*)ptr = 5;              // Cast to int*, then dereference
 *   *(char*)ptr = 'A';           // Cast to char*, then dereference
 *
 * For mem* functions, cast to unsigned char*:
 *   unsigned char *p = (unsigned char*)ptr;
 *   // Now you can access individual bytes
 *
 * ============================================================================
 *                    PART 4: THE STANDARD STRING FUNCTIONS
 * ============================================================================
 *
 * CATEGORY 1: LENGTH AND SEARCH (Read-only operations)
 * -------------------------------------------------------
 *
 * strlen(str)           Count characters until '\0'
 * strchr(str, c)        Find FIRST occurrence of character c
 * strrchr(str, c)       Find LAST occurrence of character c
 * strstr(hay, needle)   Find substring needle in haystack
 *
 *
 * CATEGORY 2: COPY OPERATIONS (Write to destination)
 * -------------------------------------------------------
 *
 * strcpy(dest, src)          Copy entire src to dest (DANGEROUS!)
 * strncpy(dest, src, n)      Copy at most n chars (may not null-terminate!)
 * strcat(dest, src)          Append src to end of dest (DANGEROUS!)
 * strncat(dest, src, n)      Append at most n chars (ALWAYS null-terminates)
 *
 *
 * CATEGORY 3: COMPARISON
 * -------------------------------------------------------
 *
 * strcmp(s1, s2)             Compare entire strings
 * strncmp(s1, s2, n)         Compare at most n characters
 *
 * Returns:
 *   < 0   s1 comes before s2 (alphabetically or by ASCII)
 *   = 0   strings are equal
 *   > 0   s1 comes after s2
 *
 *
 * CATEGORY 4: MEMORY FUNCTIONS (Work on raw bytes)
 * -------------------------------------------------------
 *
 * memset(ptr, val, n)        Fill n bytes with value val
 * memcpy(dest, src, n)       Copy n bytes (NO overlap allowed!)
 * memmove(dest, src, n)      Copy n bytes (overlap OK)
 * memcmp(p1, p2, n)          Compare n bytes
 *
 * KEY DIFFERENCE from string functions:
 * - mem* functions work on ANY data (not just strings)
 * - mem* functions DON'T stop at '\0'
 * - mem* functions require you to specify the SIZE
 *
 *
 * CATEGORY 5: CONVERSION
 * -------------------------------------------------------
 *
 * atoi(str)            String to integer    "123" -> 123
 * itoa(n, str, base)   Integer to string    123 -> "123" (non-standard!)
 *
 * ============================================================================
 *                    PART 5: HOW TO IMPLEMENT EACH FUNCTION
 * ============================================================================
 *
 * STRLEN - Count characters
 * -------------------------
 *
 *   size_t strlen(const char *str) {
 *       size_t len = 0;
 *       while (str[len] != '\0') {
 *           len++;
 *       }
 *       return len;
 *   }
 *
 *   Key insight: Just count until you hit '\0'
 *
 *
 * STRCPY - Copy string
 * --------------------
 *
 *   char* strcpy(char *dest, const char *src) {
 *       char *ret = dest;           // Save start
 *       while (*src != '\0') {
 *           *dest = *src;           // Copy character
 *           dest++;
 *           src++;
 *       }
 *       *dest = '\0';               // Don't forget null terminator!
 *       return ret;
 *   }
 *
 *   Key insight: Copy char by char, then add '\0'
 *
 *
 * STRCMP - Compare strings
 * ------------------------
 *
 *   int strcmp(const char *s1, const char *s2) {
 *       while (*s1 && *s1 == *s2) {  // While not end AND chars match
 *           s1++;
 *           s2++;
 *       }
 *       return (unsigned char)*s1 - (unsigned char)*s2;
 *   }
 *
 *   Key insight: Find first difference, return the difference
 *
 *
 * STRCAT - Concatenate strings
 * ----------------------------
 *
 *   char* strcat(char *dest, const char *src) {
 *       char *ret = dest;
 *       while (*dest) dest++;       // Find end of dest
 *       while (*src) {
 *           *dest++ = *src++;       // Copy src
 *       }
 *       *dest = '\0';               // Null terminate
 *       return ret;
 *   }
 *
 *   Key insight: Find end of dest, then strcpy from there
 *
 *
 * MEMSET - Fill memory with byte value
 * ------------------------------------
 *
 *   void* memset(void *ptr, int value, size_t n) {
 *       unsigned char *p = (unsigned char*)ptr;
 *       unsigned char v = (unsigned char)value;
 *       while (n--) {
 *           *p++ = v;
 *       }
 *       return ptr;
 *   }
 *
 *   Key insight: Cast to byte pointer, fill n times
 *
 *   TRAP: memset(arr, 1, sizeof(arr)) does NOT set ints to 1!
 *         It sets each BYTE to 1, so int becomes 0x01010101
 *
 *
 * MEMCPY - Copy bytes (no overlap)
 * --------------------------------
 *
 *   void* memcpy(void *dest, const void *src, size_t n) {
 *       unsigned char *d = (unsigned char*)dest;
 *       const unsigned char *s = (const unsigned char*)src;
 *       while (n--) {
 *           *d++ = *s++;
 *       }
 *       return dest;
 *   }
 *
 *   Key insight: Cast to bytes, copy n times
 *
 *
 * MEMMOVE - Copy bytes (overlap OK)
 * ---------------------------------
 *
 *   void* memmove(void *dest, const void *src, size_t n) {
 *       unsigned char *d = (unsigned char*)dest;
 *       const unsigned char *s = (const unsigned char*)src;
 *
 *       if (d < s) {
 *           // Copy forward (same as memcpy)
 *           while (n--) *d++ = *s++;
 *       } else if (d > s) {
 *           // Copy BACKWARD to avoid overwriting source
 *           d += n;
 *           s += n;
 *           while (n--) *--d = *--s;
 *       }
 *       return dest;
 *   }
 *
 *   Key insight: If dest > src, copy from END to avoid corruption
 *
 *   Why backward copy matters:
 *   [A][B][C][D][E][F]
 *      ^     ^
 *      dest  src
 *
 *   Forward copy (WRONG):
 *   Step 1: dest[0] = src[0] -> [A][C][C][D][E][F]
 *   Step 2: dest[1] = src[1] -> [A][C][D][D][E][F]  // C was overwritten!
 *
 *   Backward copy (CORRECT):
 *   Step 1: dest[3] = src[3] -> [A][B][C][D][F][F]
 *   Step 2: dest[2] = src[2] -> [A][B][C][E][F][F]
 *   ... preserves source data
 *
 *
 * STRCHR - Find character
 * -----------------------
 *
 *   char* strchr(const char *str, int c) {
 *       while (*str) {
 *           if (*str == (char)c) return (char*)str;
 *           str++;
 *       }
 *       if (c == '\0') return (char*)str;  // Can search for '\0'
 *       return NULL;
 *   }
 *
 *   Key insight: Linear search, return pointer to match
 *
 *
 * STRSTR - Find substring
 * -----------------------
 *
 *   char* strstr(const char *haystack, const char *needle) {
 *       if (*needle == '\0') return (char*)haystack;
 *
 *       while (*haystack) {
 *           const char *h = haystack;
 *           const char *n = needle;
 *
 *           while (*h == *n && *n) {
 *               h++;
 *               n++;
 *           }
 *
 *           if (*n == '\0') return (char*)haystack;  // Full match!
 *           haystack++;
 *       }
 *       return NULL;
 *   }
 *
 *   Key insight: At each position, try to match entire needle
 *
 *
 * ATOI - String to integer
 * ------------------------
 *
 *   int atoi(const char *str) {
 *       int result = 0;
 *       int sign = 1;
 *
 *       while (*str == ' ') str++;           // Skip whitespace
 *
 *       if (*str == '-') { sign = -1; str++; }
 *       else if (*str == '+') { str++; }
 *
 *       while (*str >= '0' && *str <= '9') {
 *           result = result * 10 + (*str - '0');
 *           str++;
 *       }
 *
 *       return sign * result;
 *   }
 *
 *   Key insight: digit value = char - '0'  (e.g., '5' - '0' = 5)
 *                Build number: result = result * 10 + digit
 *
 *
 * ITOA - Integer to string
 * ------------------------
 *
 *   char* itoa(int value, char *str, int base) {
 *       char *ptr = str;
 *       bool negative = false;
 *
 *       if (value == 0) { *ptr++ = '0'; *ptr = '\0'; return str; }
 *
 *       if (value < 0 && base == 10) {
 *           negative = true;
 *           value = -value;
 *       }
 *
 *       // Extract digits in REVERSE order
 *       while (value) {
 *           int digit = value % base;
 *           *ptr++ = (digit < 10) ? '0' + digit : 'a' + digit - 10;
 *           value /= base;
 *       }
 *
 *       if (negative) *ptr++ = '-';
 *       *ptr = '\0';
 *
 *       // Reverse the string
 *       reverse(str);
 *       return str;
 *   }
 *
 *   Key insight: digit = n % 10 gives rightmost digit
 *                Digits come out reversed, so reverse at end
 *
 * ============================================================================
 *                    PART 6: COMMON TRAPS AND MISTAKES
 * ============================================================================
 *
 * TRAP 1: BUFFER OVERFLOW
 * -----------------------
 *
 *   char dest[5];
 *   strcpy(dest, "Hello World");  // OVERFLOW! dest is only 5 bytes
 *
 *   Fix: Always check size or use strncpy/snprintf
 *
 *
 * TRAP 2: MISSING NULL TERMINATOR
 * -------------------------------
 *
 *   char dest[5];
 *   strncpy(dest, "Hello", 5);    // NO '\0' added! src >= n
 *   printf("%s", dest);           // Undefined behavior!
 *
 *   Fix: strncpy(dest, "Hello", 4); dest[4] = '\0';
 *
 *
 * TRAP 3: OVERLAPPING MEMORY
 * --------------------------
 *
 *   char buf[] = "Hello";
 *   strcpy(buf + 1, buf);         // Undefined behavior!
 *
 *   Fix: Use memmove for overlapping regions
 *
 *
 * TRAP 4: MEMSET WITH NON-ZERO FOR INTS
 * -------------------------------------
 *
 *   int arr[10];
 *   memset(arr, 1, sizeof(arr));  // Each int = 0x01010101, NOT 1!
 *
 *   Fix: Use loop, or only memset with 0 or -1
 *
 *
 * TRAP 5: SIGNED CHAR COMPARISON
 * ------------------------------
 *
 *   char a = 200;  // If char is signed, this is -56!
 *   char b = 65;
 *   if (a > b)     // -56 > 65? FALSE! But 200 > 65 should be TRUE
 *
 *   Fix: Cast to unsigned char for comparisons
 *
 *
 * TRAP 6: FORGETTING TO RETURN
 * ----------------------------
 *
 *   char* strcpy(char *dest, const char *src) {
 *       while ((*dest++ = *src++));
 *       // Forgot to return! What does caller get?
 *   }
 *
 *   Fix: Save original pointer and return it
 *
 *
 * TRAP 7: ATOI OVERFLOW
 * ---------------------
 *
 *   atoi("99999999999999");  // Overflow! Undefined behavior
 *
 *   Fix: Check before multiply: if (result > INT_MAX/10) ...
 *
 * ============================================================================
 *                    PART 7: SIZE_T AND CONST
 * ============================================================================
 *
 * SIZE_T:
 * -------
 *   - Unsigned integer type for sizes and counts
 *   - Guaranteed to hold size of any array
 *   - Use for: strlen return, array indices, byte counts
 *   - On 64-bit: typically 8 bytes (can count up to 18 quintillion)
 *
 *   size_t len = strlen(str);  // Correct
 *   int len = strlen(str);     // Works but can truncate on huge strings
 *
 *
 * CONST:
 * ------
 *   const char *str    - Can't modify what str points TO
 *   char *const str    - Can't modify str itself (where it points)
 *
 *   // Function that reads but doesn't modify string:
 *   size_t strlen(const char *str);
 *
 *   // Function that writes to dest but only reads src:
 *   char* strcpy(char *dest, const char *src);
 *
 * ============================================================================
 *                    PART 8: INTERVIEW TIPS
 * ============================================================================
 *
 * 1. ALWAYS handle NULL pointers first
 *    if (str == NULL) return 0;
 *
 * 2. ALWAYS check for buffer overflow potential
 *    "What if dest is too small?" - Ask interviewer or document assumption
 *
 * 3. REMEMBER the return value
 *    Most string functions return the dest pointer for chaining
 *
 * 4. CAST to unsigned char for comparisons
 *    return (unsigned char)*s1 - (unsigned char)*s2;
 *
 * 5. DON'T forget the null terminator
 *    *dest = '\0';  // Critical final step!
 *
 * 6. KNOW the difference between str* and mem* functions
 *    str*: stops at '\0', for strings only
 *    mem*: works on n bytes, any data type
 *
 * 7. KNOW when to use memmove vs memcpy
 *    memcpy: faster, but NO overlap
 *    memmove: slightly slower, but handles overlap
 *
 * ============================================================================
 *                    QUICK REFERENCE CARD
 * ============================================================================
 *
 *   FUNCTION        SIGNATURE                              NOTE
 *   --------        ---------                              ----
 *   strlen          size_t strlen(const char *s)           Count to '\0'
 *   strcpy          char* strcpy(char *d, const char *s)   DANGEROUS
 *   strncpy         char* strncpy(char *d, const char *s, size_t n)  May not '\0'
 *   strcat          char* strcat(char *d, const char *s)   DANGEROUS
 *   strncat         char* strncat(char *d, const char *s, size_t n)  Always '\0'
 *   strcmp          int strcmp(const char *s1, const char *s2)
 *   strncmp         int strncmp(const char *s1, const char *s2, size_t n)
 *   strchr          char* strchr(const char *s, int c)     First occurrence
 *   strrchr         char* strrchr(const char *s, int c)    Last occurrence
 *   strstr          char* strstr(const char *hay, const char *needle)
 *   memset          void* memset(void *p, int v, size_t n) Fills BYTES
 *   memcpy          void* memcpy(void *d, const void *s, size_t n)  NO overlap
 *   memmove         void* memmove(void *d, const void *s, size_t n) Overlap OK
 *   memcmp          int memcmp(const void *p1, const void *p2, size_t n)
 *   atoi            int atoi(const char *s)                No error checking
 *   itoa            char* itoa(int val, char *s, int base) NON-STANDARD!
 *
 * ============================================================================
 */

#include <stdio.h>

int main() {
    printf("=== String Library Theory ===\n\n");

    printf("This file contains comprehensive theory for implementing\n");
    printf("standard C string and memory functions.\n\n");

    printf("Topics covered:\n");
    printf("1. How strings work in C (null terminator)\n");
    printf("2. Pointer operations for strings\n");
    printf("3. void pointers for memory functions\n");
    printf("4. Overview of all string/memory functions\n");
    printf("5. Implementation patterns for each function\n");
    printf("6. Common traps and mistakes\n");
    printf("7. size_t and const usage\n");
    printf("8. Interview tips\n\n");

    printf("=== Key Formulas ===\n\n");

    printf("String length:\n");
    printf("  Count characters until '\\0'\n\n");

    printf("Character to digit:\n");
    printf("  digit = char - '0'  (e.g., '5' - '0' = 5)\n\n");

    printf("Build integer from digits:\n");
    printf("  result = result * 10 + digit\n\n");

    printf("Extract digits from integer:\n");
    printf("  digit = n %% 10   (rightmost digit)\n");
    printf("  n = n / 10       (remove rightmost digit)\n\n");

    printf("Pointer difference = element count:\n");
    printf("  length = end_ptr - start_ptr\n\n");

    return 0;
}
