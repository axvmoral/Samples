#include "cs19_c_strings.h"

namespace cs19 {


unsigned atoi(const char *str) {
    int istr = 0;
    int decimal = 1;
    for (int i = strlen(str) - 1; i >= 0; i--) {
        istr += decimal * (*(str + i) - 48);
        decimal *= 10;
    }
    return istr;
}


const char *strchr(const char *haystack, const char needle) {
    for (; *haystack; ++haystack)
        if (*haystack == needle)
            return haystack;
    return nullptr;
}

int strcmp(const char *str1, const char *str2) {
    int istr1 = *str1;
    int istr2 = *str2;
    return istr1 - istr2;
}


std::size_t strlen(const char *str) {
    int size = 0;
    for (; *str; ++str)
        size += 1;
    return size;
}


const char *strpbrk(const char *haystack, const char *char_list) {
    for (; *haystack; ++haystack) {
        int i = 0;
        while (*(char_list + i)) {
            if (*haystack == *(char_list + i)) {
                return haystack;
            }
            i += 1;
        }
    }
    return nullptr;
}

const char *strrchr(const char *haystack, const char needle) {
    bool match_check = false;
    int match_index = 0;
    int i = 0;
    while (*(haystack + i)) {
        if (*(haystack + i) == needle) {
            match_check = true;
            match_index = i;
        }
        i += 1;
    }
    if (match_check) {
        return haystack + match_index;
    } else {
        return nullptr;
    }
}

char *strrev(char *str) {
    char *ptr = str;
    char first;
    char *ptr_to_last = &(str[strlen(str) - 1]);
    while (ptr_to_last > str) {
        first = *str;
        *str = *ptr_to_last;
        str++;
        *ptr_to_last = first;
        ptr_to_last--;
    }
    return ptr;
}

const char *strstr(const char *haystack, const char *needle) {
    int lindex = strlen(needle) - 1;
    for (; *haystack; haystack++) {
        for (int i = 0; *(needle + i) && *haystack == *(needle + i); i++) {
            if (i == lindex) {
                return haystack;
            }
        }
    }
    return nullptr;
}

void strzip(const char *str1, const char *str2, char *output) {
    for (int i = 0; *str1 || *str2; ++output) {
        if (*str1 && i % 2 == 0) {
            *output = *str1;
            ++str1;
        }
        if (*str2 && i % 2 != 0) {
            *output = *str2;
            ++str2;
        }
        ++i;
    }
}


char *str_rot13(char *str) {
    char *ptr = str;
    int istr;
    for (; *str; ++str) {
        istr = *str;
        if ((istr >= 65 && istr <= 90) || (istr >= 97 && istr <= 122)) {
            istr += 13;
            if ((istr > 90 && istr < 97) || (istr > 122)) {
                istr -= 26;
            }
            *str = static_cast<char>(istr);
        }
    }
    return ptr;
}
}  // namespace cs19