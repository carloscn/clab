#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

int main(int argc, char *argv[])
{
    pid_t pid = fork();

    while(1) {
        if (0 == pid) {
            debug_log("Current PID = %d, PPID = %d\n", getpid(), getppid());
            debug_log("I'm Child process!\n");
            sleep(1);
        } else if (pid > 0) {
            debug_log("Current PID = %d, PPID = %d\n", getpid(), getppid());
            debug_log("I'm Parent process!\n");
            sleep(2);
        } else {
            debug_log("fork() failed \n");
        }
    }

    return 0;
}
