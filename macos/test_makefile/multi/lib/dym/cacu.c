#include "cacu.h"
// gcc -fPIC -shared -o libcacu.so cacu.c

int cacu(int a, int b) {
    return a * b + a + b;
}