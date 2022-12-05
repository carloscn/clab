#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc, char *argv[])
{
    char *p = NULL;
    int i = 0;
    if (argc == 1) {
        printf("the process 1 entry : %p\n", &main);
    } else {
        printf("the process 2 entry : %p\n", &main);
    }
    while(1) {
        p = malloc(1024);
        if (argc == 1)
            printf("process 1: p value is : %p\n", p);
        else
            printf("process 2: p value is : %p\n", p);
        p[295] = 7;
        free(p);
        p = NULL;
        sleep(3);
    }
    return 0;
}