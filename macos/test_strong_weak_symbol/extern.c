#include <stdio.h>

// strong symbol
int symbol = 7;

int extern_func(int a, int b)
{
    printf("extern.c : the value a is %d, b is %d\n", a, b);
    return a + b;
}
