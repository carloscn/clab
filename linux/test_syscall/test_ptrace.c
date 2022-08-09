#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

// $host arm-linux-gnueabihf-gcc test_ptrace.c -o test_ptrace.elf
// $host scp -r test_ptrace.elf text.txt root@192.168.31.210:/home/root/
// $remote strace -o log.txt ./test_ptrace.elf
int main(int argc, char *argv[])
{
    int handle, bytes;
    void *ptr = NULL;

    handle = open("text.txt", O_RDONLY);
    ptr = (void*)malloc(150);
    if (ptr == NULL) {
        debug_log("malloc failed\n");
        return 0;
    }

    bytes = read(handle, ptr, 150);
    debug_log("%s\n", (char *)ptr);

    close(handle);
    free(ptr);
    ptr = NULL;
    return 0;
}
