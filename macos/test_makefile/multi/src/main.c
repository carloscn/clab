#include "add.h"
#include "div.h"
#include "mul.h"
#include "sub.h"
#include "print.h"
#include <stdio.h>

int main(void)
{
    int a = 9, b = 8;
    printf("a * b = %d\n", mul(a, b));
    printf("a / b = %d\n", div(a, b));
    printf("a + b = %d\n", add(a, b));
    printf("a - b = %d\n", sub(a, b));
    print(0, "this is the print\n");
}