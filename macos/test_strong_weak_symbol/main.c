#include <stdio.h>

// weak symbol
__attribute__((weak)) int  symbol = 1;
__attribute__((weak)) int extern_func(int a, int b);

int main(void)
{
    static int var_1 = 85;
    static int var_2;
    int c = 6;
    int d;

    if (extern_func) {
        extern_func(c, d);
    } else {
        printf("no extern_func impl\n");
    }
    return c;
}
