#include <stdio.h>
#include <stdint.h>

int& sum(int& a, int& b) {
    a = a + b;
    return a;
}

int main(void)
{
    int a = 1, b = 2;
    sum(a, b) = 5;
    printf("a = %d\n", a);
    return 0;
}

