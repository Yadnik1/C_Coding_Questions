/*
 * ============================================================================
 *                    STRING LIBRARY FUNCTIONS - COMPLETE THEORY
 * ============================================================================
 *
 * This file teaches you EVERYTHING you need to implement standard C string
 * and memory functions from scratch. Read this COMPLETELY before attempting
 * the problems. Every concept is explained from absolute basics.
 *
 * ============================================================================
 *                         PART 1: HOW STRINGS WORK IN C
 * ============================================================================
 *
 * WHAT IS A STRING IN C?
 * ----------------------
 * A string in C is just an ARRAY OF CHARACTERS ending with '\0' (null terminator).
 * There's no special "string type" like in Python or Java - it's just chars in a row!
 *
 *   char str[] = "Hello";
 *
 *   Memory layout (each box is 1 byte):
 *   +-----+-----+-----+-----+-----+------+
 *   | 'H' | 'e' | 'l' | 'l' | 'o' | '\0' |
 *   +-----+-----+-----+-----+-----+------+
 *   [0]   [1]   [2]   [3]   [4]   [5]
 *    |     |     |     |     |     |
 *   72    101   108   108   111    0     <-- ASCII values
 *
 *   - The string "Hello" has length 5 (we count the actual characters)
 *   - But it uses 6 bytes of memory (5 chars + 1 null terminator)
 *   - '\0' is ASCII value 0 (not the character '0' which is ASCII 48!)
 *
 *
 * THE DIFFERENCE BETWEEN '0' AND '\0':
 * ------------------------------------
 *   '0'  = the CHARACTER zero, ASCII value 48
 *   '\0' = the NULL character, ASCII value 0 (used to end strings)
 *
 *   Think of '\0' as an invisible "STOP" sign at the end of every string.
 *
 *
 * THREE WAYS TO CREATE STRINGS:
 * -----------------------------
 *
 *   Method 1: String literal (compiler adds '\0' automatically)
 *   char str1[] = "Hello";     // Size is 6 (5 chars + '\0')
 *
 *   Method 2: Character array (YOU must add '\0'!)
 *   char str2[] = {'H', 'e', 'l', 'l', 'o', '\0'};  // Must include '\0'!
 *
 *   Method 3: Fixed size array
 *   char str3[10] = "Hello";   // "Hello\0" + 4 unused bytes
 *
 *
 * ============================================================================
 * WHY THE NULL TERMINATOR IS SO IMPORTANT
 * ============================================================================
 *
 * C has NO WAY to know a string's length without scanning it!
 * Unlike Python/Java, C doesn't store length metadata anywhere.
 *
 * The null terminator '\0' tells functions "STOP HERE - string ends"
 *
 * WITHOUT '\0', BAD THINGS HAPPEN:
 * --------------------------------
 *
 *   char bad[] = {'H', 'e', 'l', 'l', 'o'};  // NO '\0' - DANGEROUS!
 *
 *   strlen(bad);   // Keeps counting past 'o' into random memory!
 *                  // Returns garbage value (could be 5, 50, or 5000)
 *
 *   printf("%s", bad);  // Prints "Hello" + random garbage until it
 *                       // accidentally hits a 0 byte somewhere
 *
 *   strcpy(dest, bad);  // Copies forever, crashes program or worse
 *
 * This is one of the most common bugs in C and causes security vulnerabilities!
 *
 *
 * ============================================================================
 * CHAR VS UNSIGNED CHAR - WHY IT MATTERS FOR COMPARISONS
 * ============================================================================
 *
 * THE PROBLEM:
 * char can be SIGNED or UNSIGNED depending on compiler/platform!
 *
 *   Signed char range:   -128 to 127
 *   Unsigned char range:    0 to 255
 *
 * EXAMPLE OF THE BUG:
 *
 *   // On a system where char is SIGNED:
 *   char a = 200;    // 200 doesn't fit! Wraps to -56
 *   char b = 65;     // This is 'A'
 *
 *   if (a > b)       // Is -56 > 65? NO!
 *                    // But 200 > 65 should be TRUE!
 *
 * WHY THIS MATTERS FOR strcmp:
 *
 *   Extended ASCII characters (128-255) like accented letters (e, u, n)
 *   would be negative when char is signed, causing wrong sort order!
 *
 * THE FIX - Always cast to unsigned char:
 *
 *   return (unsigned char)*s1 - (unsigned char)*s2;
 *
 *   Now 200 stays as 200, and comparisons work correctly.
 *
 *
 * ============================================================================
 *                    PART 2: POINTERS AND STRINGS - THE FOUNDATION
 * ============================================================================
 *
 * You CANNOT implement string functions without understanding pointers.
 * Let's build up from absolute basics.
 *
 *
 * WHAT IS A POINTER?
 * ------------------
 * A pointer is a variable that stores a MEMORY ADDRESS (not a value).
 *
 *   int x = 42;       // x holds the VALUE 42
 *   int *p = &x;      // p holds the ADDRESS of x
 *
 *   Memory visualization:
 *
 *   Address:    1000    1004    1008    1012
 *              +------+-------+------+------+
 *   Memory:    |  42  | 1000  |      |      |
 *              +------+-------+------+------+
 *   Variable:     x       p
 *
 *   p contains 1000 (the address where x lives)
 *
 *
 * POINTER OPERATORS:
 * ------------------
 *
 *   &x     "Address of x" - gives you where x lives in memory
 *   *p     "Dereference p" - gives you the value at the address p holds
 *
 *   Example:
 *   int x = 42;
 *   int *p = &x;    // p = address of x
 *   printf("%d", *p);  // Prints 42 (value at address p)
 *   *p = 100;       // Changes x to 100!
 *
 *
 * POINTERS AND ARRAYS - THE KEY RELATIONSHIP:
 * -------------------------------------------
 *
 * In C, an array name IS a pointer to its first element!
 *
 *   char str[] = "Hello";
 *   char *p = str;        // p points to first character 'H'
 *
 *   These are EQUIVALENT:
 *   str[0]  ==  *str      ==  *p       // All give 'H'
 *   str[1]  ==  *(str+1)  ==  *(p+1)   // All give 'e'
 *   str[i]  ==  *(str+i)  ==  *(p+i)   // General rule
 *
 *   Memory visualization:
 *   +-----+-----+-----+-----+-----+------+
 *   | 'H' | 'e' | 'l' | 'l' | 'o' | '\0' |
 *   +-----+-----+-----+-----+-----+------+
 *     ^
 *     |
 *     p and str both point here
 *
 *
 * POINTER ARITHMETIC:
 * -------------------
 * Adding/subtracting moves pointer by element size, not bytes!
 *
 *   char *p = str;   // Points to 'H'
 *   p = p + 1;       // Now points to 'e' (moved 1 char = 1 byte)
 *   p = p + 2;       // Now points to 'l' at index 3
 *
 *   int *q = arr;    // Points to arr[0]
 *   q = q + 1;       // Points to arr[1] (moved 1 int = 4 bytes!)
 *
 *
 * POINTER SUBTRACTION - GETTING LENGTH:
 * -------------------------------------
 *
 *   char *start = str;      // Points to 'H'
 *   char *end = str + 5;    // Points to '\0'
 *
 *   size_t length = end - start;   // 5 (number of elements between pointers)
 *
 *   This is how pointer-based strlen works!
 *
 *
 * THE FOUR ESSENTIAL POINTER OPERATIONS FOR STRINGS:
 * --------------------------------------------------
 *
 *   Operation    Meaning                          Example
 *   ---------    -------                          -------
 *   *p           Get value at p                   *p == 'H'
 *   p++          Move p to next element           p now points to 'e'
 *   *p++         Get value, THEN move p           Returns 'H', p moves to 'e'
 *   ++*p         Increment VALUE at p             'H' becomes 'I'
 *
 *   UNDERSTANDING *p++:
 *   This is (*p)++ NOT *(p++). Postfix ++ has higher precedence!
 *   But it evaluates as: get *p first, THEN increment p.
 *
 *   char *p = "Hello";
 *   char c = *p++;      // c = 'H', then p moves to 'e'
 *
 *
 * ============================================================================
 * COMMON POINTER PATTERNS FOR STRING FUNCTIONS
 * ============================================================================
 *
 * PATTERN 1: Walk through a string (for strlen, strchr, etc.)
 * -----------------------------------------------------------
 *
 *   while (*p != '\0') {     // While we haven't hit the end
 *       // do something with *p (current character)
 *       p++;                 // Move to next character
 *   }
 *
 *   SHORTER VERSION (experts write this):
 *   while (*p) {             // *p is false when *p == '\0' (which is 0)
 *       // do something
 *       p++;
 *   }
 *
 *
 * PATTERN 2: Copy a string (for strcpy)
 * -------------------------------------
 *
 *   VERBOSE VERSION (easier to understand):
 *   while (*src != '\0') {
 *       *dest = *src;    // Copy current character
 *       dest++;          // Move dest pointer
 *       src++;           // Move src pointer
 *   }
 *   *dest = '\0';        // Don't forget null terminator!
 *
 *   COMPACT VERSION (common in interviews):
 *   while ((*dest++ = *src++) != '\0');
 *
 *   How the compact version works:
 *   1. *src gets the source character
 *   2. *dest = ... assigns it to destination
 *   3. Both pointers increment AFTER the assignment
 *   4. The result of assignment is the assigned value
 *   5. Loop continues until we assign '\0' (which is false/0)
 *
 *
 * PATTERN 3: Compare strings (for strcmp)
 * ---------------------------------------
 *
 *   while (*s1 && *s1 == *s2) {   // While s1 not at end AND chars match
 *       s1++;
 *       s2++;
 *   }
 *   return *s1 - *s2;             // Difference of first non-matching chars
 *
 *   STEP BY STEP for strcmp("abc", "abd"):
 *   - *s1='a', *s2='a', match! Advance both
 *   - *s1='b', *s2='b', match! Advance both
 *   - *s1='c', *s2='d', NOT match! Exit loop
 *   - Return 'c' - 'd' = 99 - 100 = -1 (negative = s1 < s2)
 *
 *
 * PATTERN 4: Find end of string then work from there (for strcat)
 * ---------------------------------------------------------------
 *
 *   // First, find end of dest
 *   while (*dest) dest++;     // Now dest points to '\0'
 *
 *   // Then copy src starting there
 *   while (*src) {
 *       *dest++ = *src++;
 *   }
 *   *dest = '\0';
 *
 *
 * ============================================================================
 *                    PART 3: VOID POINTERS (FOR mem* FUNCTIONS)
 * ============================================================================
 *
 * WHAT IS void*?
 * --------------
 * void* is a "generic pointer" - it can point to ANY data type.
 * It's like a universal adapter.
 *
 *   void *ptr;
 *   int x = 42;
 *   char c = 'A';
 *   double d = 3.14;
 *
 *   ptr = &x;      // OK! ptr now points to an int
 *   ptr = &c;      // OK! ptr now points to a char
 *   ptr = &d;      // OK! ptr now points to a double
 *
 * WHY mem* FUNCTIONS USE void*:
 * -----------------------------
 * They need to work on ANY data type - int arrays, char arrays, structs, etc.
 *
 *   memcpy(dest, src, n);   // Copies n bytes, doesn't care what type
 *
 *   int arr1[10], arr2[10];
 *   memcpy(arr1, arr2, sizeof(arr1));  // Works!
 *
 *   char buf1[100], buf2[100];
 *   memcpy(buf1, buf2, 100);           // Also works!
 *
 *
 * THE CATCH - CAN'T DEREFERENCE void* DIRECTLY:
 * ---------------------------------------------
 *
 *   void *ptr = &someData;
 *   *ptr = 5;           // ERROR! Compiler doesn't know size
 *                       // Is it 1 byte? 4 bytes? 8 bytes?
 *
 * YOU MUST CAST FIRST:
 *
 *   *(int*)ptr = 5;           // Cast to int*, now compiler knows it's 4 bytes
 *   *(char*)ptr = 'A';        // Cast to char*, now it's 1 byte
 *
 *
 * HOW TO USE void* IN mem* FUNCTIONS:
 * -----------------------------------
 * Cast to unsigned char* to work with individual bytes:
 *
 *   void* memset(void *ptr, int value, size_t n) {
 *       // Cast void* to unsigned char* so we can access bytes
 *       unsigned char *p = (unsigned char*)ptr;
 *
 *       // Now we can work byte-by-byte
 *       while (n--) {
 *           *p++ = (unsigned char)value;
 *       }
 *
 *       return ptr;  // Return original pointer
 *   }
 *
 * WHY unsigned char (NOT just char)?
 * ----------------------------------
 * 1. It's always 1 byte (CHAR_BIT bits, usually 8)
 * 2. Values 0-255 are predictable (no signed/unsigned ambiguity)
 * 3. C standard guarantees you can alias ANY type through unsigned char*
 *
 *
 * ============================================================================
 *                    PART 4: THE STANDARD STRING FUNCTIONS OVERVIEW
 * ============================================================================
 *
 * Before implementing, understand what each function DOES:
 *
 *
 * CATEGORY 1: LENGTH AND SEARCH (Read-only, don't modify anything)
 * -----------------------------------------------------------------
 *
 *   strlen(str)
 *   - Count characters until '\0'
 *   - Returns: size_t (the count)
 *   - Example: strlen("Hello") returns 5
 *
 *   strchr(str, c)
 *   - Find FIRST occurrence of character c
 *   - Returns: pointer to found char, or NULL if not found
 *   - Example: strchr("Hello", 'l') returns pointer to first 'l'
 *
 *   strrchr(str, c)
 *   - Find LAST occurrence of character c
 *   - Returns: pointer to found char, or NULL if not found
 *   - Example: strrchr("Hello", 'l') returns pointer to second 'l'
 *
 *   strstr(haystack, needle)
 *   - Find first occurrence of substring needle in haystack
 *   - Returns: pointer to start of match, or NULL if not found
 *   - Example: strstr("Hello World", "Wor") returns pointer to 'W'
 *
 *
 * CATEGORY 2: COPY OPERATIONS (Write to destination buffer)
 * ---------------------------------------------------------
 *
 *   strcpy(dest, src)
 *   - Copy entire src string to dest (including '\0')
 *   - WARNING: DANGEROUS! No bounds checking!
 *   - Returns: dest pointer
 *
 *   strncpy(dest, src, n)
 *   - Copy at most n characters from src to dest
 *   - WARNING: May NOT null-terminate if src >= n chars!
 *   - Returns: dest pointer
 *
 *   strcat(dest, src)
 *   - Append src to end of dest
 *   - WARNING: DANGEROUS! No bounds checking!
 *   - Returns: dest pointer
 *
 *   strncat(dest, src, n)
 *   - Append at most n characters from src to dest
 *   - ALWAYS adds '\0' at the end (safer than strncpy!)
 *   - Returns: dest pointer
 *
 *
 * CATEGORY 3: COMPARISON
 * ----------------------
 *
 *   strcmp(s1, s2)
 *   - Compare entire strings character by character
 *   - Returns:
 *     < 0  if s1 comes before s2 (s1 < s2)
 *     = 0  if strings are equal
 *     > 0  if s1 comes after s2 (s1 > s2)
 *
 *   strncmp(s1, s2, n)
 *   - Compare at most n characters
 *   - Same return values as strcmp
 *
 *   IMPORTANT: Return value is NOT just -1, 0, 1!
 *   It's the DIFFERENCE of first differing characters.
 *   strcmp("abc", "abd") returns -1 (or any negative)
 *   strcmp("abd", "abc") returns +1 (or any positive)
 *
 *
 * CATEGORY 4: MEMORY FUNCTIONS (Work on raw bytes, not just strings)
 * ------------------------------------------------------------------
 *
 *   memset(ptr, value, n)
 *   - Fill n bytes with value (cast to unsigned char)
 *   - Returns: ptr
 *   - TRAP: value fills each BYTE, not each int!
 *
 *   memcpy(dest, src, n)
 *   - Copy exactly n bytes from src to dest
 *   - WARNING: Undefined behavior if regions overlap!
 *   - Returns: dest
 *
 *   memmove(dest, src, n)
 *   - Copy exactly n bytes, correctly handles overlap
 *   - Slower than memcpy but safer
 *   - Returns: dest
 *
 *   memcmp(ptr1, ptr2, n)
 *   - Compare exactly n bytes
 *   - Does NOT stop at '\0' (unlike strcmp)
 *   - Returns: same as strcmp (<0, 0, >0)
 *
 *
 * KEY DIFFERENCES: str* vs mem* functions
 * ---------------------------------------
 *
 *   str* functions:
 *   - Work only on null-terminated strings
 *   - STOP when they hit '\0'
 *   - Don't need size parameter (use '\0' as end marker)
 *
 *   mem* functions:
 *   - Work on ANY data (strings, ints, structs, binary)
 *   - DON'T stop at '\0' - process exactly n bytes
 *   - REQUIRE size parameter (don't know when to stop otherwise)
 *
 *
 * CATEGORY 5: CONVERSION
 * ----------------------
 *
 *   atoi(str)
 *   - ASCII TO Integer: convert string to int
 *   - Handles: whitespace, +/- sign, digits
 *   - Stops at first non-digit
 *   - Example: atoi("  -123abc") returns -123
 *
 *   itoa(value, str, base)  [NON-STANDARD!]
 *   - Integer TO ASCII: convert int to string
 *   - Works with any base (2, 8, 10, 16, etc.)
 *   - Example: itoa(255, buf, 16) gives "ff"
 *
 *
 * ============================================================================
 *                    PART 5: HOW TO IMPLEMENT EACH FUNCTION
 * ============================================================================
 *
 * For each function, I'll show:
 * 1. The algorithm in plain English
 * 2. Step-by-step visualization
 * 3. The code with detailed comments
 * 4. Key insight to remember
 *
 *
 * ============================================================================
 * STRLEN - Count characters until null terminator
 * ============================================================================
 *
 * ALGORITHM:
 * 1. Start at beginning of string
 * 2. Count each character until you hit '\0'
 * 3. Return the count
 *
 * VISUALIZATION for strlen("Hello"):
 *
 *   Step 0: len=0, looking at 'H'  -> count, len=1
 *   Step 1: len=1, looking at 'e'  -> count, len=2
 *   Step 2: len=2, looking at 'l'  -> count, len=3
 *   Step 3: len=3, looking at 'l'  -> count, len=4
 *   Step 4: len=4, looking at 'o'  -> count, len=5
 *   Step 5: len=5, looking at '\0' -> STOP! Return 5
 *
 * CODE (Array indexing version - easier to understand):
 *
 *   size_t my_strlen(const char *str) {
 *       if (str == NULL) return 0;    // Safety check
 *
 *       size_t len = 0;               // Initialize counter
 *
 *       while (str[len] != '\0') {    // While not at end
 *           len++;                    // Count this character
 *       }
 *
 *       return len;                   // Return total count
 *   }
 *
 * CODE (Pointer version - common in interviews):
 *
 *   size_t my_strlen_ptr(const char *str) {
 *       if (str == NULL) return 0;
 *
 *       const char *p = str;          // Save start position
 *
 *       while (*p != '\0') {          // While not at end
 *           p++;                      // Move to next char
 *       }
 *
 *       return p - str;               // Pointer difference = length
 *   }
 *
 * KEY INSIGHT: Count until '\0', don't count the '\0' itself.
 *
 *
 * ============================================================================
 * STRCPY - Copy entire string from source to destination
 * ============================================================================
 *
 * ALGORITHM:
 * 1. Copy each character from src to dest
 * 2. Stop when you've copied '\0'
 * 3. Return original dest pointer
 *
 * VISUALIZATION for strcpy(dest, "Hi"):
 *
 *   Before: src = ['H']['i']['\0']
 *           dest = [?][?][?][?][?]
 *
 *   Step 1: Copy 'H' -> dest = ['H'][?][?][?][?]
 *   Step 2: Copy 'i' -> dest = ['H']['i'][?][?][?]
 *   Step 3: Copy '\0' -> dest = ['H']['i']['\0'][?][?]
 *   Done!
 *
 * CODE:
 *
 *   char* my_strcpy(char *dest, const char *src) {
 *       if (dest == NULL || src == NULL) return NULL;
 *
 *       char *original = dest;        // Save for return
 *
 *       while (*src != '\0') {        // While source has chars
 *           *dest = *src;             // Copy character
 *           dest++;                   // Move dest pointer
 *           src++;                    // Move src pointer
 *       }
 *
 *       *dest = '\0';                 // CRITICAL: Add null terminator!
 *
 *       return original;              // Return start of dest
 *   }
 *
 * COMPACT VERSION:
 *
 *   char* my_strcpy_compact(char *dest, const char *src) {
 *       char *ret = dest;
 *       while ((*dest++ = *src++) != '\0');  // Copy including '\0'
 *       return ret;
 *   }
 *
 * KEY INSIGHT: Don't forget to copy the '\0' at the end!
 *
 *
 * ============================================================================
 * STRCMP - Compare two strings character by character
 * ============================================================================
 *
 * ALGORITHM:
 * 1. Compare characters at same position
 * 2. If they match, move to next position
 * 3. If they differ OR we hit '\0', stop
 * 4. Return difference of current characters
 *
 * VISUALIZATION for strcmp("abc", "abd"):
 *
 *   Position 0: 'a' vs 'a' -> EQUAL, continue
 *   Position 1: 'b' vs 'b' -> EQUAL, continue
 *   Position 2: 'c' vs 'd' -> DIFFERENT! Stop.
 *   Return: 'c' - 'd' = 99 - 100 = -1 (negative = first string is "less")
 *
 * CODE:
 *
 *   int my_strcmp(const char *s1, const char *s2) {
 *       if (s1 == NULL || s2 == NULL) {
 *           if (s1 == s2) return 0;
 *           return s1 == NULL ? -1 : 1;
 *       }
 *
 *       // Continue while: s1 not at end AND characters match
 *       while (*s1 != '\0' && *s1 == *s2) {
 *           s1++;
 *           s2++;
 *       }
 *
 *       // Return difference (cast to unsigned char for extended ASCII)
 *       return (unsigned char)*s1 - (unsigned char)*s2;
 *   }
 *
 * RETURN VALUE EXAMPLES:
 *   strcmp("abc", "abc") = 0   (equal)
 *   strcmp("abc", "abd") < 0   (first is "less")
 *   strcmp("abd", "abc") > 0   (first is "greater")
 *   strcmp("ab", "abc")  < 0   ('\0' < 'c')
 *   strcmp("abc", "ab")  > 0   ('c' > '\0')
 *
 * KEY INSIGHT: Stop at first difference OR end of string.
 *              Cast to unsigned char for correct extended ASCII comparison.
 *
 *
 * ============================================================================
 * STRCAT - Concatenate (append) src to end of dest
 * ============================================================================
 *
 * ALGORITHM:
 * 1. Find the end of dest (where '\0' is)
 * 2. Copy src starting from that position
 * 3. Return original dest pointer
 *
 * VISUALIZATION for strcat("Hi", "!!"):
 *
 *   Before: dest = ['H']['i']['\0'][?][?][?]
 *           src  = ['!']['!']['\0']
 *
 *   Step 1: Find end of dest (position 2)
 *   Step 2: Copy '!' at position 2 -> ['H']['i']['!'][?][?][?]
 *   Step 3: Copy '!' at position 3 -> ['H']['i']['!']['!'][?][?]
 *   Step 4: Add '\0' at position 4 -> ['H']['i']['!']['!']['\0'][?]
 *
 *   Result: "Hi!!"
 *
 * CODE:
 *
 *   char* my_strcat(char *dest, const char *src) {
 *       if (dest == NULL || src == NULL) return dest;
 *
 *       char *original = dest;        // Save for return
 *
 *       // Step 1: Find end of dest
 *       while (*dest != '\0') {
 *           dest++;
 *       }
 *       // Now dest points to the '\0' at end of original string
 *
 *       // Step 2: Copy src (same as strcpy from here)
 *       while (*src != '\0') {
 *           *dest = *src;
 *           dest++;
 *           src++;
 *       }
 *
 *       // Step 3: Add null terminator
 *       *dest = '\0';
 *
 *       return original;
 *   }
 *
 * KEY INSIGHT: strcat = find end of dest, then strcpy.
 *
 * WHY REPEATED strcat IS SLOW:
 *   strcat(s, "a");  // Find end (scan n chars), then append
 *   strcat(s, "b");  // Find end again (scan n+1 chars), then append
 *   strcat(s, "c");  // Find end again (scan n+2 chars), then append
 *   ...
 *   Total: O(n + n+1 + n+2 + ...) = O(n*k) for k appends = O(n^2)
 *   This is called "Schlemiel the painter's algorithm"!
 *
 *
 * ============================================================================
 * MEMSET - Fill memory with a byte value
 * ============================================================================
 *
 * ALGORITHM:
 * 1. Cast pointer to byte pointer
 * 2. Write the value to each byte, n times
 * 3. Return original pointer
 *
 * VISUALIZATION for memset(buf, 'A', 5):
 *
 *   Before: buf = [?][?][?][?][?]
 *   After:  buf = ['A']['A']['A']['A']['A']
 *                  65   65   65   65   65  (ASCII values)
 *
 * CODE:
 *
 *   void* my_memset(void *ptr, int value, size_t n) {
 *       if (ptr == NULL) return NULL;
 *
 *       // Cast to byte pointer to access individual bytes
 *       unsigned char *p = (unsigned char*)ptr;
 *
 *       // Cast value to byte (only lowest 8 bits used)
 *       unsigned char v = (unsigned char)value;
 *
 *       // Fill n bytes
 *       while (n > 0) {
 *           *p = v;     // Write the byte value
 *           p++;        // Move to next byte
 *           n--;        // Decrement count
 *       }
 *
 *       return ptr;     // Return original pointer
 *   }
 *
 * THE MEMSET TRAP - DON'T USE FOR INTS (except 0 and -1):
 *
 *   int arr[4];
 *   memset(arr, 1, sizeof(arr));
 *
 *   What you might expect:  arr = [1, 1, 1, 1]
 *   What actually happens:  arr = [16843009, 16843009, 16843009, 16843009]
 *
 *   WHY? memset fills each BYTE with 1:
 *   One int = 4 bytes = [0x01][0x01][0x01][0x01] = 0x01010101 = 16843009
 *
 *   SAFE VALUES:
 *   memset(arr, 0, sizeof(arr));   // All bytes 0x00 -> ints are 0
 *   memset(arr, -1, sizeof(arr));  // All bytes 0xFF -> ints are -1
 *   memset(arr, 1, sizeof(arr));   // WRONG! ints become 0x01010101
 *
 * KEY INSIGHT: memset fills BYTES, not elements. Use loops for non-zero int values.
 *
 *
 * ============================================================================
 * MEMCPY - Copy n bytes (NO OVERLAP ALLOWED)
 * ============================================================================
 *
 * ALGORITHM:
 * 1. Cast both pointers to byte pointers
 * 2. Copy n bytes from src to dest
 * 3. Return original dest pointer
 *
 * VISUALIZATION for memcpy(dest, "ABC", 3):
 *
 *   Before: src  = ['A']['B']['C']
 *           dest = [?][?][?]
 *
 *   Step 1: Copy byte 0 ('A')
 *   Step 2: Copy byte 1 ('B')
 *   Step 3: Copy byte 2 ('C')
 *
 *   After:  dest = ['A']['B']['C']
 *
 * CODE:
 *
 *   void* my_memcpy(void *dest, const void *src, size_t n) {
 *       if (dest == NULL || src == NULL) return NULL;
 *
 *       unsigned char *d = (unsigned char*)dest;
 *       const unsigned char *s = (const unsigned char*)src;
 *
 *       while (n > 0) {
 *           *d = *s;    // Copy one byte
 *           d++;        // Move dest pointer
 *           s++;        // Move src pointer
 *           n--;        // Decrement count
 *       }
 *
 *       return dest;
 *   }
 *
 * WHY "NO OVERLAP" MATTERS:
 *
 *   char buf[] = "ABCDEF";
 *   memcpy(buf + 2, buf, 4);  // Copy "ABCD" to position 2
 *
 *   EXPECTED: buf = "ABABCD"
 *
 *   WHAT MIGHT HAPPEN (undefined behavior!):
 *   Step 1: buf[2] = buf[0] = 'A' -> "ABACEF"
 *   Step 2: buf[3] = buf[1] = 'B' -> "ABABEF"
 *   Step 3: buf[4] = buf[2] = 'A' -> "ABABAF" (WRONG! Should be 'C')
 *   Step 4: buf[5] = buf[3] = 'B' -> "ABABAB" (WRONG! Should be 'D')
 *
 *   Result: "ABABAB" instead of "ABABCD"!
 *
 * KEY INSIGHT: memcpy is fast but ONLY use when regions don't overlap!
 *
 *
 * ============================================================================
 * MEMMOVE - Copy n bytes (HANDLES OVERLAP CORRECTLY)
 * ============================================================================
 *
 * ALGORITHM:
 * 1. If dest < src: copy forward (same as memcpy)
 * 2. If dest > src: copy BACKWARD (from end to start)
 * 3. This prevents overwriting source data before copying it
 *
 * WHY BACKWARD COPY WORKS:
 *
 *   buf = [A][B][C][D][E][F]
 *             ^     ^
 *           dest   src
 *
 *   Copying 4 bytes from src to dest:
 *
 *   FORWARD COPY (WRONG when dest > src and overlapping):
 *   dest[0] = src[0] -> [A][C][C][D][E][F]  // 'C' overwrites 'B'
 *   dest[1] = src[1] -> [A][C][D][D][E][F]  // but we needed 'C' for src[0]!
 *   dest[2] = src[2] -> [A][C][D][D][E][F]  // WRONG - 'C' was already overwritten
 *   dest[3] = src[3] -> [A][C][D][D][E][F]  // WRONG
 *
 *   BACKWARD COPY (CORRECT):
 *   dest[3] = src[3] -> [A][B][C][D][E][F] (src[3]='F')
 *   dest[2] = src[2] -> [A][B][C][D][E][F] (src[2]='E')
 *   dest[1] = src[1] -> [A][B][C][D][E][F] (src[1]='D')
 *   dest[0] = src[0] -> [A][C][D][E][E][F] (src[0]='C')
 *
 *   Wait, that's still not right. Let me redo with correct overlap:
 *
 *   buf = [A][B][C][D][E][F]
 *   memmove(buf+2, buf, 4) means copy buf[0..3] to buf[2..5]
 *
 *   src  = positions 0,1,2,3 (values A,B,C,D)
 *   dest = positions 2,3,4,5
 *
 *   BACKWARD: start from end
 *   buf[5] = buf[3] = 'D' -> [A][B][C][D][E][D]
 *   buf[4] = buf[2] = 'C' -> [A][B][C][D][C][D]
 *   buf[3] = buf[1] = 'B' -> [A][B][C][B][C][D]
 *   buf[2] = buf[0] = 'A' -> [A][B][A][B][C][D]
 *
 *   Result: "ABABCD" - CORRECT!
 *
 * CODE:
 *
 *   void* my_memmove(void *dest, const void *src, size_t n) {
 *       if (dest == NULL || src == NULL || n == 0) return dest;
 *
 *       unsigned char *d = (unsigned char*)dest;
 *       const unsigned char *s = (const unsigned char*)src;
 *
 *       if (d < s) {
 *           // Safe to copy forward
 *           while (n--) {
 *               *d++ = *s++;
 *           }
 *       } else if (d > s) {
 *           // Must copy backward to avoid overwriting source
 *           d += n;   // Point to end
 *           s += n;   // Point to end
 *           while (n--) {
 *               *--d = *--s;  // Pre-decrement, then copy
 *           }
 *       }
 *       // If d == s, nothing to do
 *
 *       return dest;
 *   }
 *
 * KEY INSIGHT: If dest > src, copy backward. Otherwise copy forward.
 *
 *
 * ============================================================================
 * STRCHR - Find first occurrence of character in string
 * ============================================================================
 *
 * ALGORITHM:
 * 1. Walk through string
 * 2. At each position, check if character matches
 * 3. If match, return pointer to that position
 * 4. If reach '\0' without finding, return NULL
 *
 * VISUALIZATION for strchr("Hello", 'l'):
 *
 *   Position 0: 'H' == 'l'? NO
 *   Position 1: 'e' == 'l'? NO
 *   Position 2: 'l' == 'l'? YES! Return pointer to position 2
 *
 * CODE:
 *
 *   char* my_strchr(const char *str, int c) {
 *       if (str == NULL) return NULL;
 *
 *       while (*str != '\0') {
 *           if (*str == (char)c) {
 *               return (char*)str;   // Found it!
 *           }
 *           str++;
 *       }
 *
 *       // Special case: searching for '\0' itself
 *       if ((char)c == '\0') {
 *           return (char*)str;       // Return pointer to '\0'
 *       }
 *
 *       return NULL;                 // Not found
 *   }
 *
 * KEY INSIGHT: Return POINTER to match, not the character or index!
 *              Also handle searching for '\0' as a special case.
 *
 *
 * ============================================================================
 * STRSTR - Find substring (needle) in string (haystack)
 * ============================================================================
 *
 * ALGORITHM:
 * 1. At each position in haystack, try to match entire needle
 * 2. If all of needle matches, return pointer to start of match
 * 3. If needle doesn't match, move to next position in haystack
 * 4. If reach end of haystack, return NULL
 *
 * VISUALIZATION for strstr("Hello World", "Wor"):
 *
 *   Position 0: "Hel" vs "Wor"? 'H' != 'W', move on
 *   Position 1: "ell" vs "Wor"? 'e' != 'W', move on
 *   ...
 *   Position 6: "Wor" vs "Wor"? 'W'=='W', 'o'=='o', 'r'=='r', MATCH!
 *   Return pointer to position 6
 *
 * CODE:
 *
 *   char* my_strstr(const char *haystack, const char *needle) {
 *       if (haystack == NULL || needle == NULL) return NULL;
 *
 *       // Empty needle matches at start (standard behavior)
 *       if (*needle == '\0') return (char*)haystack;
 *
 *       // Try matching at each position
 *       while (*haystack != '\0') {
 *           const char *h = haystack;  // Current position in haystack
 *           const char *n = needle;    // Start of needle
 *
 *           // Try to match entire needle starting here
 *           while (*h == *n && *n != '\0') {
 *               h++;
 *               n++;
 *           }
 *
 *           // Did we match all of needle?
 *           if (*n == '\0') {
 *               return (char*)haystack;  // Yes! Return start position
 *           }
 *
 *           // No match, try next position
 *           haystack++;
 *       }
 *
 *       return NULL;  // Needle not found in haystack
 *   }
 *
 * TIME COMPLEXITY: O(n*m) where n=haystack length, m=needle length
 *                  (Can be improved to O(n+m) with KMP algorithm)
 *
 * KEY INSIGHT: At each position, try to match the ENTIRE needle.
 *
 *
 * ============================================================================
 * ATOI - Convert string to integer
 * ============================================================================
 *
 * ALGORITHM:
 * 1. Skip leading whitespace
 * 2. Handle optional +/- sign
 * 3. Convert each digit: digit_value = char - '0'
 * 4. Build number: result = result * 10 + digit
 * 5. Stop at first non-digit
 *
 * VISUALIZATION for atoi("  -123abc"):
 *
 *   "  -123abc"
 *    ^^
 *    Skip whitespace
 *
 *   "  -123abc"
 *      ^
 *      Found '-', remember sign = -1
 *
 *   "  -123abc"
 *       ^
 *       '1' is digit: result = 0*10 + 1 = 1
 *
 *   "  -123abc"
 *        ^
 *        '2' is digit: result = 1*10 + 2 = 12
 *
 *   "  -123abc"
 *         ^
 *         '3' is digit: result = 12*10 + 3 = 123
 *
 *   "  -123abc"
 *          ^
 *          'a' is NOT digit: STOP
 *
 *   Apply sign: -123
 *
 * HOW "digit = char - '0'" WORKS:
 *
 *   ASCII: '0'=48, '1'=49, '2'=50, ..., '9'=57
 *
 *   '5' - '0' = 53 - 48 = 5  (the numeric value!)
 *   '9' - '0' = 57 - 48 = 9
 *
 *   This trick converts character digit to its numeric value.
 *
 * HOW "result = result*10 + digit" BUILDS THE NUMBER:
 *
 *   For "123":
 *   result = 0
 *   result = 0*10 + 1 = 1
 *   result = 1*10 + 2 = 12
 *   result = 12*10 + 3 = 123
 *
 *   Each *10 shifts existing digits left, making room for new digit.
 *
 * CODE:
 *
 *   int my_atoi(const char *str) {
 *       if (str == NULL) return 0;
 *
 *       int result = 0;
 *       int sign = 1;
 *
 *       // Skip whitespace
 *       while (*str == ' ' || *str == '\t' || *str == '\n') {
 *           str++;
 *       }
 *
 *       // Handle sign
 *       if (*str == '-') {
 *           sign = -1;
 *           str++;
 *       } else if (*str == '+') {
 *           str++;
 *       }
 *
 *       // Convert digits
 *       while (*str >= '0' && *str <= '9') {
 *           int digit = *str - '0';           // Convert char to digit
 *           result = result * 10 + digit;     // Build number
 *           str++;
 *       }
 *
 *       return sign * result;
 *   }
 *
 * OVERFLOW HANDLING (for production code):
 *
 *   Before: result = result * 10 + digit
 *   Check:  if (result > INT_MAX/10) -> overflow!
 *           if (result == INT_MAX/10 && digit > 7) -> overflow!
 *
 * KEY INSIGHT: digit = char - '0' to convert character to number.
 *              result = result * 10 + digit to build the number.
 *
 *
 * ============================================================================
 * ITOA - Convert integer to string (NON-STANDARD!)
 * ============================================================================
 *
 * ALGORITHM:
 * 1. Handle zero and negative specially
 * 2. Extract digits using % (modulo) - gives rightmost digit
 * 3. Remove digit using / (division)
 * 4. Digits come out in REVERSE order!
 * 5. Reverse the string at the end
 *
 * VISUALIZATION for itoa(-456):
 *
 *   Step 1: Negative, so save sign and work with 456
 *
 *   Step 2: Extract digits (reverse order):
 *   456 % 10 = 6, store '6', 456 / 10 = 45
 *   45 % 10 = 5, store '5', 45 / 10 = 4
 *   4 % 10 = 4, store '4', 4 / 10 = 0, STOP
 *
 *   Step 3: Buffer has "654"
 *
 *   Step 4: Add '-': buffer has "654-"
 *
 *   Step 5: Reverse: "-456"
 *
 * HOW "digit = n % 10" and "n = n / 10" WORK:
 *
 *   456 % 10 = 6  (remainder when dividing by 10 = last digit)
 *   456 / 10 = 45 (integer division removes last digit)
 *
 *   45 % 10 = 5
 *   45 / 10 = 4
 *
 *   4 % 10 = 4
 *   4 / 10 = 0
 *
 * CODE:
 *
 *   char* my_itoa(int value, char *str, int base) {
 *       if (str == NULL || base < 2 || base > 36) return NULL;
 *
 *       char *ptr = str;
 *       bool negative = false;
 *
 *       // Handle zero
 *       if (value == 0) {
 *           *ptr++ = '0';
 *           *ptr = '\0';
 *           return str;
 *       }
 *
 *       // Handle negative (only for base 10)
 *       if (value < 0 && base == 10) {
 *           negative = true;
 *           value = -value;
 *       }
 *
 *       // Extract digits in reverse order
 *       while (value != 0) {
 *           int digit = value % base;
 *           // Convert digit to character
 *           if (digit < 10) {
 *               *ptr++ = '0' + digit;       // 0-9
 *           } else {
 *               *ptr++ = 'a' + digit - 10;  // a-z for bases > 10
 *           }
 *           value /= base;
 *       }
 *
 *       // Add negative sign
 *       if (negative) {
 *           *ptr++ = '-';
 *       }
 *
 *       *ptr = '\0';  // Null terminate
 *
 *       // Reverse the string
 *       char *start = str;
 *       char *end = ptr - 1;
 *       while (start < end) {
 *           char temp = *start;
 *           *start = *end;
 *           *end = temp;
 *           start++;
 *           end--;
 *       }
 *
 *       return str;
 *   }
 *
 * KEY INSIGHT: % gives rightmost digit, / removes it.
 *              Digits come out reversed, so reverse at end.
 *
 *
 * ============================================================================
 *                    PART 6: COMMON TRAPS AND MISTAKES
 * ============================================================================
 *
 * TRAP 1: BUFFER OVERFLOW
 * -----------------------
 *   char dest[5];
 *   strcpy(dest, "Hello World");  // OVERFLOW! dest is only 5 bytes
 *                                 // "Hello World" needs 12 bytes
 *
 *   FIX: Check sizes or use strncpy/snprintf
 *   strncpy(dest, "Hello World", sizeof(dest) - 1);
 *   dest[sizeof(dest) - 1] = '\0';
 *
 *
 * TRAP 2: MISSING NULL TERMINATOR
 * -------------------------------
 *   char dest[5];
 *   strncpy(dest, "Hello", 5);    // Copies "Hello" but NO '\0'!
 *   printf("%s", dest);           // Undefined behavior - no terminator!
 *
 *   FIX: Always ensure null termination
 *   strncpy(dest, "Hello", sizeof(dest) - 1);
 *   dest[sizeof(dest) - 1] = '\0';  // Force null terminator
 *
 *
 * TRAP 3: OVERLAPPING MEMORY WITH memcpy
 * --------------------------------------
 *   char buf[] = "Hello";
 *   memcpy(buf + 1, buf, 4);      // UNDEFINED! Regions overlap
 *
 *   FIX: Use memmove for overlapping regions
 *   memmove(buf + 1, buf, 4);     // Safe!
 *
 *
 * TRAP 4: MEMSET FOR INT ARRAYS
 * -----------------------------
 *   int arr[10];
 *   memset(arr, 1, sizeof(arr));  // Each int = 0x01010101 = 16843009!
 *
 *   FIX: Use loop for non-zero values, or only use 0/-1
 *   for (int i = 0; i < 10; i++) arr[i] = 1;  // Correct
 *   memset(arr, 0, sizeof(arr));              // This works
 *   memset(arr, -1, sizeof(arr));             // This works too
 *
 *
 * TRAP 5: SIGNED CHAR COMPARISON
 * ------------------------------
 *   char a = 200;  // On signed char systems, this is -56!
 *   char b = 65;
 *   if (a > b)     // -56 > 65? FALSE! (should be TRUE)
 *
 *   FIX: Cast to unsigned char
 *   if ((unsigned char)a > (unsigned char)b)  // 200 > 65? TRUE!
 *
 *
 * TRAP 6: FORGETTING RETURN VALUE
 * -------------------------------
 *   char* my_strcpy(char *dest, const char *src) {
 *       while ((*dest++ = *src++));
 *       // Oops! Forgot to return!
 *   }
 *
 *   FIX: Save original pointer and return it
 *   char* my_strcpy(char *dest, const char *src) {
 *       char *ret = dest;
 *       while ((*dest++ = *src++));
 *       return ret;  // Return original dest
 *   }
 *
 *
 * TRAP 7: ATOI OVERFLOW
 * ---------------------
 *   int n = atoi("99999999999999");  // Way bigger than INT_MAX!
 *                                    // Undefined behavior
 *
 *   FIX: Check before overflow occurs
 *   if (result > INT_MAX/10 || (result == INT_MAX/10 && digit > 7)) {
 *       return (sign == 1) ? INT_MAX : INT_MIN;
 *   }
 *
 *
 * ============================================================================
 *                    PART 7: SIZE_T AND CONST EXPLAINED
 * ============================================================================
 *
 * WHAT IS size_t?
 * ---------------
 * size_t is an UNSIGNED integer type designed for sizes and counts.
 *
 * WHY USE size_t INSTEAD OF int?
 * 1. size_t is UNSIGNED - can't be negative (sizes are never negative)
 * 2. size_t is BIG ENOUGH to hold any array size on the system
 * 3. On 64-bit systems: int is 4 bytes, size_t is 8 bytes
 *
 * EXAMPLE:
 *   int len = strlen(hugeString);      // Could overflow on huge strings!
 *   size_t len = strlen(hugeString);   // Always safe
 *
 *
 * WHAT IS const?
 * --------------
 * const means "this value cannot be modified through this variable."
 *
 * TWO WAYS TO USE const WITH POINTERS:
 *
 *   const char *str       // Can't modify what str points TO
 *   char *const str       // Can't modify str itself (where it points)
 *   const char *const str // Can't modify either!
 *
 * EXAMPLE:
 *   const char *p = "Hello";
 *   p[0] = 'J';           // ERROR! Can't modify string through p
 *   p = "World";          // OK! Can change where p points
 *
 *   char *const q = buf;
 *   q[0] = 'X';           // OK! Can modify the string
 *   q = otherBuf;         // ERROR! Can't change where q points
 *
 * WHY FUNCTIONS USE const:
 *   size_t strlen(const char *str);
 *   // Promise: strlen won't modify the string you pass
 *
 *   char* strcpy(char *dest, const char *src);
 *   // Will modify dest, but won't modify src
 *
 *
 * ============================================================================
 *                    PART 8: INTERVIEW TIPS
 * ============================================================================
 *
 * 1. ALWAYS HANDLE NULL POINTERS FIRST
 *    if (str == NULL) return 0;  // or return NULL, or handle appropriately
 *
 * 2. SAVE THE ORIGINAL POINTER BEFORE MODIFYING
 *    char *ret = dest;
 *    // ... modify dest ...
 *    return ret;  // Return original, not modified pointer
 *
 * 3. ALWAYS ADD NULL TERMINATOR
 *    *dest = '\0';  // Don't forget this!
 *
 * 4. CAST TO UNSIGNED CHAR FOR COMPARISONS
 *    return (unsigned char)*s1 - (unsigned char)*s2;
 *
 * 5. KNOW str* vs mem* DIFFERENCES
 *    str*: stops at '\0', for strings
 *    mem*: processes exactly n bytes, for any data
 *
 * 6. KNOW memcpy vs memmove
 *    memcpy: fast, NO overlap allowed
 *    memmove: handles overlap, slightly slower
 *
 * 7. VERBALIZE YOUR APPROACH
 *    "First I'll check for NULL, then find the end of the string..."
 *
 * 8. TEST WITH EDGE CASES
 *    - Empty string ""
 *    - NULL pointer
 *    - Single character "A"
 *    - Strings of equal length
 *
 *
 * ============================================================================
 *                    QUICK REFERENCE CARD
 * ============================================================================
 *
 *   FUNCTION        SIGNATURE                              NOTE
 *   --------        ---------                              ----
 *   strlen          size_t strlen(const char *s)           Count to '\0'
 *   strcpy          char* strcpy(char *d, const char *s)   DANGEROUS
 *   strncpy         char* strncpy(d, s, n)                 May not '\0'
 *   strcat          char* strcat(char *d, const char *s)   DANGEROUS
 *   strncat         char* strncat(d, s, n)                 Always '\0'
 *   strcmp          int strcmp(s1, s2)                     <0, 0, >0
 *   strncmp         int strncmp(s1, s2, n)                 Compare n chars
 *   strchr          char* strchr(s, c)                     First match
 *   strrchr         char* strrchr(s, c)                    Last match
 *   strstr          char* strstr(hay, needle)              Find substring
 *   memset          void* memset(p, val, n)                Fill BYTES
 *   memcpy          void* memcpy(d, s, n)                  NO overlap!
 *   memmove         void* memmove(d, s, n)                 Overlap OK
 *   memcmp          int memcmp(p1, p2, n)                  Compare bytes
 *   atoi            int atoi(s)                            No error check
 *   itoa            char* itoa(val, s, base)               NON-STANDARD
 *
 *
 * ============================================================================
 *                    KEY FORMULAS TO MEMORIZE
 * ============================================================================
 *
 *   Character to digit:     digit = char - '0'
 *   Digit to character:     char = digit + '0'
 *   Build integer:          result = result * 10 + digit
 *   Extract rightmost digit: digit = n % 10
 *   Remove rightmost digit:  n = n / 10
 *   Pointer to length:       length = end_ptr - start_ptr
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
    printf("2. Pointer operations for strings (detailed)\n");
    printf("3. void pointers for memory functions\n");
    printf("4. All string/memory functions overview\n");
    printf("5. Step-by-step implementations with visualizations\n");
    printf("6. Common traps and how to avoid them\n");
    printf("7. size_t and const explained\n");
    printf("8. Interview tips and quick reference\n\n");

    printf("=== Key Formulas ===\n\n");

    printf("Character to digit:\n");
    printf("  digit = char - '0'  (e.g., '5' - '0' = 5)\n\n");

    printf("Digit to character:\n");
    printf("  char = digit + '0'  (e.g., 5 + '0' = '5')\n\n");

    printf("Build integer from digits:\n");
    printf("  result = result * 10 + digit\n\n");

    printf("Extract rightmost digit:\n");
    printf("  digit = n %% 10\n\n");

    printf("Remove rightmost digit:\n");
    printf("  n = n / 10\n\n");

    printf("Pointer difference = element count:\n");
    printf("  length = end_ptr - start_ptr\n\n");

    printf("=== Common Patterns ===\n\n");

    printf("Walk through string:\n");
    printf("  while (*p) { /* use *p */ p++; }\n\n");

    printf("Copy string:\n");
    printf("  while ((*dest++ = *src++));\n\n");

    printf("Compare strings:\n");
    printf("  while (*s1 && *s1 == *s2) { s1++; s2++; }\n");
    printf("  return *s1 - *s2;\n\n");

    return 0;
}
