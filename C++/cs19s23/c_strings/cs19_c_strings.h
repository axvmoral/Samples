#ifndef CS19_C_STRINGS_H_
#define CS19_C_STRINGS_H_

#include <cstddef>  // for std::size_t

namespace cs19 {

/**
 * Converts a C string to an int. The string is assumed to consist only of decimal-digit characters.
 * For example, atoi("42") returns 42. Overflow may occur if the result exceeds int's range.
 *
 * @param str the string to convert
 * @return the converted value
 */
unsigned atoi(const char *str);

/**
 * Finds the first occurrence of a character in a C string.
 *
 * @param haystack the string to search
 * @param needle the char to search for
 * @return a pointer to the first char in haystack that matches needle,
 *         or nullptr if no such value exists.
 */
const char *strchr(const char *haystack, const char needle);

/**
 * Compares two C strings for lexicographic ordering, i.e. ordering with respect to the numeric
 * encodings of the characters. For example, strcmp("Apple", "Banana") returns a negative value,
 * strcmp("wat", "wat") returns 0, and strcmp("apple", "Banana") returns a positive value.
 *
 * @return an integer less than, equal to, or greater than zero if str1 is found, respectively, to
 * be less than, to match, or be greater than str2.
 */
int strcmp(const char *str1, const char *str2);

/**
 * Calculates the length of a C string, excluding the terminating null byte ('\0').
 * For example, strlen("Hello") returns 5.
 *
 * @param str the string to measure
 * @return the number of characters in the string pointed to by str
 */
std::size_t strlen(const char *str);

/**
 * Searches a C string for any of a set of characters.
 *
 * @param haystack the string to search
 * @param char_list a string containing the set of characters for which to search
 * @return a pointer to the first char in haystack that matches one of the char in char_list,
 *         or nullptr if no such value exists.
 */
const char *strpbrk(const char *haystack, const char *char_list);

/**
 * Finds the last occurrence of a character in a C string.
 *
 * @param haystack the string to search
 * @param needle the char to search for
 * @return a pointer to the last char in haystack that matches needle,
 *         or nullptr if no such value exists.
 */
const char *strrchr(const char *haystack, const char needle);

/**
 * Reverses a C string in place.
 *
 * @param str the string to reverse
 * @return a pointer to the now-reversed string
 */
char *strrev(char *str);

/**
 * Finds the first occurrence of a substring needle in a C string.
 * The terminating null bytes ('\0') are not compared.
 *
 * @param haystack the string in which to search
 * @param needle the string for which to search
 * @return a pointer to the beginning of the first occurrence of needle in haystack,
 *         or nullptr if no such value exists.
 */
const char *strstr(const char *haystack, const char *needle);

/**
 * Composes an output C string with alternating characters from two other strings.
 * See the suggested testing code for an example.
 *
 * @param str1 the first source string
 * @param str2 the second source string
 * @param[out] output filled with characters from str1 and str2, alternating between the two. The
 * length of this array is assumed to be at least strlen(str1) + strlen(str2) + 1.
 */
void strzip(const char *str1, const char *str2, char *output);

/**
 * Performs the ROT13 encoding on a C string, in place. The ROT13 encoding simply shifts every
 * English letter by 13 places in the alphabet while leaving non-English-alpha characters untouched.
 * Encoding and decoding are done by the same function: Passing an encoded string as argument will
 * return the original version.
 *
 * See: https://en.wikipedia.org/wiki/ROT13
 *
 * @return a pointer to the encoded string
 */
char *str_rot13(char *str);

}  // namespace cs19

#endif  // CS19_C_STRINGS_H_
