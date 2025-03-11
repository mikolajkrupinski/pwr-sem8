#include <stdio.h>
#include <string.h>

size_t common_sufix_len(const char *str1, const char *str2);

int main(void) {
    printf("%ld\n", common_sufix_len("kayak", "kayak"));    // 5
    printf("%ld\n", common_sufix_len("abcdef", ""));        // 0
    printf("%ld\n", common_sufix_len("abcdef", "xyz"));     // 0
    printf("%ld\n", common_sufix_len("abcd", "xyzab"));     // 2

    return 0;
}

size_t common_sufix_len(const char const *str1, const char const *str2) {
    size_t str_min_len = strlen(str1) < strlen(str2) ? strlen(str1) : strlen(str2);
    const char *str2_end = str2 + strlen(str2);

    size_t k_max = 0;
    for (size_t k = 1;  k <= str_min_len; k++) {
        if (strncmp(str1, str2_end - k, k) == 0) {
            k_max = k;
        }
    }

    return k_max;
}
