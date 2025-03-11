#include <stdio.h>
#include <string.h>
#include <stdlib.h>

size_t find_pattern(const char *str, const char *pattern);

int main(void) {
    printf("%ld\n", find_pattern("abcdefg", "def")); // 3
    printf("%ld\n", find_pattern("abcdefg", "ab"));  // 0
    printf("%ld\n", find_pattern("abcdefg", "fg"));  // 5
    printf("%ld\n", find_pattern("abcdefg", "cf"));  // LONG_MAX

    return 0;
}

size_t find_pattern(const char const *str, const char const *pattern) {
    ssize_t offset = strlen(str) - strlen(pattern);

    if (offset < 0) {
        return __LONG_MAX__;
    }

    for (size_t i = 0; i <= offset; i++) {
        if (memcmp(str + i, pattern, strlen(pattern)) == 0) {
            return i;
        }
    }

    return __LONG_MAX__;
}