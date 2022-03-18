#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i = atoi(argv[1]); // 10 seconds
    printf(">test new sleep process: will sleep %d s\n", i);
    while(i --) {
        sleep(1);
    }
    printf(">test new sleep process: wakeup done.\n");
    return 0;
}
