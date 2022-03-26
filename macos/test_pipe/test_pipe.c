#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

#define P_CMD "./helper_process_long_loop.elf 10"


int test_popen_write()
{
    FILE *write_fp = NULL;
    char buffer[BUFSIZ + 1];
    int chars_read;
    sprintf(buffer, "hello my name is Carlos\n");
    write_fp = popen("od -c", "w");
    debug_log("popen start ...\n");
    if (NULL == write_fp) {
        debug_log("popen failed\n");
        return -1;
    }
    chars_read = fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
    debug_log("call pclose, wait ...\n");
    pclose(write_fp);
    debug_log("pclose end ...\n");

    return 0;
}

int test_popen_read()
{
    FILE *read_fp = NULL;
    char buffer[BUFSIZ + 1];
    int chars_read;
    memset(buffer, '\0', sizeof buffer);
    read_fp = popen(P_CMD, "r");
    debug_log("popen start ...\n");
    if (NULL == read_fp) {
        debug_log("popen failed\n");
        return -1;
    }
    // read one time.
#if 1
    //chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
    if (chars_read > 0) {
        debug_log("Output was:  \n\n %s \n\n", buffer);
    }
#else
    // read multi times.
    do {
        chars_read = fread(buffer, sizeof(char), 32, read_fp);
        buffer[chars_read - 1] = '\0';
        debug_log("Output was:  \n\n %s \n\n", buffer);
    } while (chars_read > 0);
#endif
    debug_log("call pclose, wait ...\n");
    pclose(read_fp);
    debug_log("popen end ...\n");

    return 0;
}

int test_pipe_rw()
{
    int ret = 0;
    int data_process = 0;
    int file_pipes[2];
    const char some_data[] = "hello world";
    char buffer[BUFSIZ + 1];
    memset(buffer, '\0', sizeof buffer);

    ret = pipe(file_pipes);
    if (ret != 0) {
        debug_log("pipe failed, ret = %d\n", ret);
        goto exit;
    }

    data_process = write(file_pipes[1], some_data, strlen(some_data));
    debug_log("Wrote the %d bytes\n", data_process);
    data_process = read(file_pipes[0], buffer, BUFSIZ);
    debug_log("read %d bytes: %s\n", data_process, buffer);
exit:
    return ret;
}

int test_pipe_rw_fork()
{
    int ret = 0;
    int data_process = 0;
    int file_pipes[2];
    const char some_data[] = "hello world";
    char buffer[BUFSIZ + 1];
    pid_t pid = 0;
    memset(buffer, '\0', sizeof buffer);

    ret = pipe(file_pipes);
    if (ret != 0) {
        debug_log("pipe failed, ret = %d\n", ret);
        goto exit;
    }

    pid = fork();
    if (pid == -1) {
        debug_log("fork failed, ret = %d\n", ret);
        goto exit;
    }
    // child process
    else if (pid == 0) {
        sleep(5);
        data_process = write(file_pipes[1], some_data, strlen(some_data));
        debug_log("child wrote the %d bytes\n", data_process);
    }
    // parent process
    else {
        data_process = read(file_pipes[0], buffer, BUFSIZ);
        debug_log("parent read %d bytes: %s\n", data_process, buffer);
    }

exit:
    return ret;
}

int test_pipe_rw_fork_dup()
{
    int ret = 0;
    int data_process = 0;
    int file_pipes[2];
    const char some_data[] = "hello world";
    char buffer[BUFSIZ + 1];
    pid_t pid = 0;
    memset(buffer, '\0', sizeof buffer);

    ret = pipe(file_pipes);
    if (ret != 0) {
        debug_log("pipe failed, ret = %d\n", ret);
        goto exit;
    }

    pid = fork();
    if (pid == -1) {
        debug_log("fork failed, ret = %d\n", ret);
        goto exit;
    }
    // child process
    else if (pid == 0) {
        // close STDIN
        close(0);
        // dup pipes[0] -> STDIN_FILENO
        dup(file_pipes[0]);
        close(file_pipes[0]);
        close(file_pipes[1]);
        execlp("od", "od", "-c", (char*) 0);
        debug_log("child wrote the bytes\n");
    }
    // parent process
    else {
        close(file_pipes[0]);
        data_process = write(file_pipes[1], some_data, strlen(some_data));
        close(file_pipes[1]);
        debug_log("parent write %d bytes: %s\n", data_process, some_data);
    }

exit:
    return ret;
}

int test_pipe_rw_exec()
{
    int ret = 0;
    int data_process = 0;
    int file_pipes[2];
    char buffer[BUFSIZ + 1];
    char cmd[BUFSIZ];
    pid_t pid = 0;
    memset(buffer, '\0', sizeof buffer);

    ret = pipe(file_pipes);
    if (ret != 0) {
        debug_log("pipe failed, ret = %d\n", ret);
        goto exit;
    }

    pid = fork();
    if (pid == -1) {
        debug_log("fork failed, ret = %d\n", ret);
        goto exit;
    }
    // child process
    else if (pid == 0) {
        debug_log("run the helper_pipe_write.elf send fd = %d\n", file_pipes[1]);
        cmd[0] = file_pipes[1];
        (void)execl("helper_pipe_write.elf", "helper_pipe_write.elf", cmd, (char *)0);
        debug_log("call the helper_pipe_write.elf wrote the bytes\n");
    }
    // parent process
    else {
        debug_log("go on the main process,.....\n");
        data_process = read(file_pipes[0], buffer, BUFSIZ);
        debug_log("parent read %d bytes: %s\n", data_process, buffer);
    }

exit:
    return ret;
}

int main(void)
{
    //test_popen_read();
    //test_popen_write();
    //test_pipe_rw();
    //test_pipe_rw_fork();
    test_pipe_rw_fork_dup();
    //test_pipe_rw_exec();
    return 0;
}