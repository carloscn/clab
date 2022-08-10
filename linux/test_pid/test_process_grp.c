#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

// arm-linux-gnueabihf-gcc test_process_grp.c -o test_process_grp.elf
// scp -r test_process_grp.elf  root@192.168.31.210:/home/root/test_process_grp.elf

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed\n");
        exit(1);
    }
    /* Child PID */
    if (0 == pid) {
        debug_log("Child current PID = %d, PGID = %d\n", getpid(), getpgid(0));
        sleep(7);
        debug_log("Group ID of child  id change to %d\n", getpgid(0));
        exit(0);
    /* Parent PID */
    } else if (pid > 0) {
        sleep(1);
        debug_log("-------------------> I'm Parent process! and set pgid to %d\n", pid);
        setpgid(pid, pid); // set parent process to group leader.
        sleep(13);
        debug_log("-------------------> Current PID = %d, PPID = %d, PGID = %d\n", getpid(), getppid(), getpgid(0));
        sleep(5);
        debug_log("-------------------> I'm Parent process! and set pgid to %d\n", getppid());
        setpgid(getpid(), getppid());  //set parent group id to parent's parent.
        printf("------------------->Group ID of parent is change to %d\n", getpgid(0));
    } else {
        debug_log("fork() failed \n");
    }
    return 0;
}
