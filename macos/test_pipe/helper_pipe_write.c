#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

int main(int argc, char **argv)
{
    int ret = 0;
    int data_process = 0;
    int file_pipes = (int) *(char *)argv[1];
    const char some_data[] = "hello world";
    debug_log("helper_pipe_write.elf start, recv the fd = %d ....\n",file_pipes);
    data_process = write(file_pipes, some_data, strlen(some_data));
    debug_log("helper_pipe_write.elf wrote the %d bytes\n", data_process);
    return 0;
}