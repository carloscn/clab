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
#define TEN_MEG (1024*1024*10)

int main(void)
{
    char buffer[BUFFER_SIZE + 1];
    int chars_read;
    int ret = 0;
    int fd = ~0;
    int bytes_sent = 0;
    int count = 0;

    if (access(FIFO_NAME, F_OK) == -1) {
        ret = mkfifo(FIFO_NAME, 0777);
        if (ret != 0) {
            debug_log("mkfifo failed ret = %d\n", ret);
            goto exit;
        }
    }
    debug_log("process %d opening fifo with O_WR_ONLY\n", getpid());
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd < 0) {
        debug_log("open fifo file failed, ret = %d\n", fd);
        ret = -1;
        goto exit;
    }
    debug_log("process %d will send data by fifo\n", getpid());
    while(bytes_sent < TEN_MEG) {
        ret = write(fd, buffer, BUFFER_SIZE);
        if (ret == -1) {
            debug_log("write failed, ret = %d\n", ret);
            goto exit;
        }
        bytes_sent += ret;
        count ++;
        printf("sent %d count, sent bytes %d, sum %d\n", count, ret, bytes_sent);
    }
    close(fd);
    debug_log("process %d finished, %d bytes read\n", getpid(), bytes_sent);
exit:
    return 0;
}