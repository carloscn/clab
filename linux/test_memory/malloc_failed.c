#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
    size_t i = 0;
    char *s = NULL;
    char *p = NULL;
    while(1) {
        p = (char*) malloc(1024);
        memset(p, 0xff, 1029);
        s = (char *)malloc(1024);
        if (NULL == s) {
            printf("malloc failed\n");
        }
        if (p != NULL)
            free(p);
        if (s != NULL)
            free(s);
    }
    return 0;
}