#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf


#define FIFO_NAME "./fifo"
#define BUFFER_SIZE PIPE_BUF

int main(void)
{
    char buffer[BUFFER_SIZE + 1];
    int chars_read;
    int ret = 0;
    int fd = ~0;
    int bytes_read = 0;
    int count = 0;

    if (access(FIFO_NAME, F_OK) == -1) {
        debug_log("mkfifo failed ret = %d\n", ret);
        goto exit;
    }
    debug_log("process %d opening fifo with O_WR_ONLY\n", getpid());
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd < 0) {
        debug_log("open fifo file failed, ret = %d\n", fd);
        ret = -1;
        goto exit;
    }
    debug_log("process %d will read data by fifo\n", getpid());
    do {
        ret = read(fd, buffer, BUFFER_SIZE);
        bytes_read += ret;
        count ++;
        printf("process read times %d, read bytes %d, sum %d\n", count, ret, bytes_read);
    } while(ret > 0);

    debug_log("process %d finished, %d bytes read\n", getpid(), bytes_read);
    close(fd);
exit:
    return 0;
}