#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

int main(int argc, char **argv)
{
    int ret = 0;
    const char *buf1 = "hello my name is Carlos\n";
    const char *buf2 = "this is the string 2\n";
    const char *buf3 = "this is the string 3\n";
    const char *buf4 = "this is the string 4\n";
    const char *buf5 = "tfjfksdjflkdsjlkfjlksdjflkj\n";

    int origin_fd = 4, fd2 = 5, fd3 = 6, fd4 = 7, fd5 = 8, origin_fd_1 = 9;

    origin_fd = open("out_file.txt", O_RDWR|O_CREAT,0644);
    if (origin_fd == -1) {
        debug_log("origin_fd open failed, ret = %d\n", origin_fd);
        goto exit;
    }
    origin_fd_1 = open("out_file_2.txt", O_RDWR|O_CREAT,0644);
    if (origin_fd_1 == -1) {
        debug_log("origin_fd open failed, ret = %d\n", origin_fd);
        goto exit;
    }
    ret = write(origin_fd_1, buf1, strlen(buf1));
    debug_log("origin_fd write %d bytes on the file, origin_fd = %d\n", ret, origin_fd);

    // test dup, it is doesn't matter that close the original fd.
    fd2 = dup(origin_fd_1);                   // dup doesn't close the original fd.
    ret = write(fd2, buf2, strlen(buf2));
    debug_log("fd2 write %d bytes on the file, fd2 = %d\n", ret, fd2);
    ret = write(origin_fd, buf2, strlen(buf2));
    debug_log("origin_fd write %d bytes on the file\n", ret);

    // test dup2
    fd4 = dup2(fd2, fd3);                   // dup2, fd3 is fd4
    debug_log("fd2 = %d fd3 = %d, fd4 = %d\n", fd2, fd3, fd4);
    ret = write(fd3, buf3, strlen(buf3));
    debug_log("fd3 write %d bytes on the file\n", ret);
    ret = write(fd4, buf4, strlen(buf4));
    debug_log("fd4 write %d bytes on the file, fd3 = %d, fd4 = %d\n", ret, fd3, fd4);
    ret = write(fd2, "using fd2 rewrite file", 22);
    debug_log("fd2 write %d bytes on the file, fd2 = %d\n", ret, fd2);

    // re-directing the stdin
    fd5 = dup2(fileno(stdout), fd4);
    ret = write(fd5, buf5, strlen(buf5));
    debug_log("fd5 write %d bytes on the file, fd5 = %d\n", ret, fd5);


    close(fd2);
    close(fd3);
    close(fd4);
    close(fd5);
    debug_log("test finish!");

exit:
    return 0;
}