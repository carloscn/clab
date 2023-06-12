#include "add.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

int add(int a, int b)
{
    return a + b;
}

bool utils_str_contains_char(char *input_str, char e)
{
    if (NULL == input_str) {
        return false;
    }

    size_t len = strlen(input_str);
    size_t i = 0;

    for (i = 0; i < len; i ++) {
        if (input_str[i] == e) {
            return true;
        }
    }

    return false;
}

void utils_swap_char(char *a, char *b)
{
    if (a == b)
        return;
    *a = (*a) ^ (*b);
    *b = (*b) ^ (*a);
    *a = (*a) ^ (*b);
}

int32_t utils_str_reverse(char *s)
{
    int32_t ret = 0;
    size_t len = 0;
    size_t i = 0;

    if (s == NULL || (len = strlen(s)) == 0) {
        ret = -1;
        goto finish;
    }

    for (i = 0; i < len / 2; i ++) {
        utils_swap_char(&(s[i]), &(s[len - i - 1]));
    }

finish:
    return ret;
}