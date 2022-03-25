#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

#define P_CMD "./helper_process_long_loop.elf "
#define CFG_PARENT_PROCESS_EXEC_TIME  12
#define CFG_CHILD_PROCESS_EXEC_TIME 2

void handler(int sig)
{
    (void)sig;
	pid_t id;
	while((id = waitpid(-1, NULL, WNOHANG)) > 0)
		debug_log("wait child success : %d\n", id);
}

int main(void)
{
    FILE *read_fp = NULL;
    char buffer[BUFSIZ + 1];
    char cmd[BUFSIZ];
    int chars_read;
    pid_t pid = 1;
    int ret = 0;
    memset(buffer, '\0', sizeof buffer);
    sprintf(cmd, "%s%d", P_CMD, CFG_PARENT_PROCESS_EXEC_TIME);
    read_fp = popen(cmd, "r");
    debug_log("popen start: father process %d s...\n", CFG_PARENT_PROCESS_EXEC_TIME);
    if (NULL == read_fp) {
        debug_log("popen failed\n");
        return -1;
    }
    //signal(SIGCHLD, handler);
    //signal(SIGCHLD, SIG_IGN);
    pid = fork();
    if (pid != 0) {
        debug_log("parent process: call pclose, wait ...\n");
        ret = pclose(read_fp);
        debug_log("parent process: popen end with ret = %d...\n", ret);
    } else {
        debug_log("child process: sleep %d s !\n ",CFG_CHILD_PROCESS_EXEC_TIME);
        sleep(CFG_CHILD_PROCESS_EXEC_TIME);
        debug_log("child process: wakeup !\n ");
    }
    return 0;
}