#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    u_int16_t count = 0;

    while(1) {
        if (0 == pid) {
            debug_log("Current PID = %d, PPID = %d\n", getpid(), getppid());
            debug_log("I'm Child process\n");
            // just exit child process, and parent process don't call waitpid(),
            // which to create zombie process
            sleep(1);
            count ++;
            if (count > 10) {
                debug_log("child will exit.\n");
                exit(0);
            }
        } else if (pid > 0) {
            // debug_log("-------------------> wait for child exiting.\n");
            // WNOHANG is non block mode.
            // WUNTRACED is block mode.
            // mask the next line code, the zombie process will be generated.
            // pid_t t_pid = waitpid(pid, NULL, WUNTRACED);
            debug_log("-------------------> Current PID = %d, PPID = %d\n", getpid(), getppid());
            debug_log("-------------------> I'm Parent process!\n");
            sleep(2);
        } else {
            debug_log("fork() failed \n");
        }
    }

    return 0;
}
