#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

int main(int argc, char *argv[])
{
    int i = atoi(argv[1]); // 10 seconds
    debug_log(">test new sleep process: will sleep %d s\n", i);
    while(i --) {
        debug_log(">sleep at %d s\n", i);
        sleep(1);
    }
    debug_log(">test new sleep process: wakeup done.\n");
    return 0;
}