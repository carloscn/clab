#include <stdio.h>
#include <stdlib.h>

typedef void (*ctor_t)(void);

void __attribute__((constructor)) my_init(void)
{
    printf("my init!\n");
}
void __attribute__((destructor)) my_exit(void) {
    printf("my exit!\n");
}

void atexit_func(void) {
    printf("atexit !\n");
}

int main(void) {
    atexit(&atexit_func);
    printf("main will return\n");
    return 0;
}
