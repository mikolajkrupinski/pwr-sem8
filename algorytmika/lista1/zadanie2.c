#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// python: str.startswith(prefix: str)
bool is_prefix(const char *string, const char *prefix);

// python: str.endswith(postfix: str)
bool is_postfix(const char *string, const char *postfix);

int main(void) {
    const char *string = "abcdcba";

    printf("%d\n", is_prefix(string, "ab"));        // 1
    printf("%d\n", is_prefix(string, "aba"));       // 0
    printf("%d\n", is_prefix(string, ""));          // 1
    printf("%d\n", is_prefix(string, "abcdcba"));   // 1
    printf("%d\n", is_prefix(string, "abcdcbaa"));  // 0

    printf("\n");

    printf("%d\n", is_postfix(string, "ba"));       // 1
    printf("%d\n", is_postfix(string, "aba"));      // 0
    printf("%d\n", is_postfix(string, ""));         // 1
    printf("%d\n", is_postfix(string, "abcdcba"));  // 1
    printf("%d\n", is_postfix(string, "aabcdcba")); // 0

    return 0;
}

bool is_prefix(const char const *string, const char const *prefix) {
    if (strlen(string) < strlen(prefix)) {
        return false;
    }

    for (size_t i = 0; i < strlen(prefix); i++) {
        if (string[i] != prefix[i]) {
            return false;
        }
    }

    return true;
}

bool is_postfix(const char const *string, const char const *postfix) {
    ssize_t offset = strlen(string) - strlen(postfix);

    if (offset < 0) {
        return false;
    }

    for (ssize_t i = strlen(postfix) - 1; i >= 0; i--) {
        if (string[offset + i] != postfix[i]) {
            return false;
        }
    }

    return true;
}