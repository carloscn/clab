#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUB_PROCESS_NAME ("./helper_process_long_loop.elf")
#define SUB_PROCESS_PARAM1 ("5")
#define SUB_PROCESS_PARAM2 ("7")

static int test1_run_new_process_using_system(void)
{
    int ret = 0;
    char cmd[1024];
    printf("%s : run new process with non-blocking!\n", __FUNCTION__);
    sprintf(cmd, "%s %s &", SUB_PROCESS_NAME, SUB_PROCESS_PARAM1);
    ret = system(cmd);
    printf("%s : nonblocking done!\n", __FUNCTION__);

    printf("%s : run new process with blocking!\n", __FUNCTION__);
    sprintf(cmd, "%s %s", SUB_PROCESS_NAME, SUB_PROCESS_PARAM1);
    ret = system(cmd);
    printf("%s : blocking done!\n", __FUNCTION__);
    return 0;
}

static int test2_run_new_process_using_exec(void)
{
    char *const argv[] = {"seize", "8", 0};
    // just for execve
    char * ps_envp[] = { "0", "TERM=console", 0};
    int ret = 0;

    char workpath[1024];
    char env[1024];

    getcwd(workpath, sizeof workpath);
    sprintf(env, "PATH=%s:/usr/bin:/bin", workpath);
    ps_envp[0] = env;

    // Test exe execl
    //printf("%s : run new process with execl!\n", __FUNCTION__);
    // execl must with env.
    //execl("/bin/ls", ".", NULL);

    // Test exe execlp
    //printf("%s : run new process with execlp!\n", __FUNCTION__);
    // execlp with no-env.
    //execlp("ls", ".", NULL);

    // Test exe execle
    //printf("%s : run new process with execlp!\n", __FUNCTION__);
    // execlp with no-env.
    //execlp("ls", ".", NULL, ps_envp);

    // Test exe execv
    printf("%s : run new process with execvp!\n", __FUNCTION__);
    ret = execve(SUB_PROCESS_NAME, argv, ps_envp);

    // cannot print as following
    if (ret != 0) {
        printf("%s : Failed! ret = %d\n", __FUNCTION__, ret);
    } else {
        printf("%s : Done!\n", __FUNCTION__);
    }
    return 0;
}

static int test3_fork_new_process_using_fork(void)
{
    char *const argv[] = {"seize", "8", 0};
    // just for execve
    char * ps_envp[] = { "0", "TERM=console", 0};
    int ret = 0;

    char workpath[1024];
    char env[1024];
    int i = 3;
    getcwd(workpath, sizeof workpath);
    sprintf(env, "PATH=%s:/usr/bin:/bin", workpath);
    ps_envp[0] = env;
    // Test exe execv
    printf("%s : join!\n", __FUNCTION__);
    while(i --) {
        ret = (int)fork();
    }
    // child
    if (ret == 0) {
        printf("%s : child run new process with execvp!\n", __FUNCTION__);
        ret = execve(SUB_PROCESS_NAME, argv, ps_envp);
        // cannot print as following
        if (ret != 0) {
            printf("%s : Failed! ret = %d\n", __FUNCTION__, ret);
        } else {
            printf("%s : Done!\n", __FUNCTION__);
        }
    }
    // parent
    else {
        int stat_val;
        char cmd[1024];
        printf("%s : parent print this log!\n", __FUNCTION__);
        printf("%s : parent waiting for child\n", __FUNCTION__);
        printf("%s : parent run new process with system!\n", __FUNCTION__);
        sprintf(cmd, "%s %s", SUB_PROCESS_NAME, SUB_PROCESS_PARAM1);
        ret = system(cmd);
        ret = wait(&stat_val);
        printf("%s : parent check the child pid : %d\n", __FUNCTION__, ret);
        if (WIFEXITED(stat_val)) {
            printf("%s : parent : child exits with code %d\n", __FUNCTION__, WEXITSTATUS(stat_val));
        } else {
            printf("%s : parent : child exit abnormally. %d\n", __FUNCTION__, WEXITSTATUS(stat_val));
        }
        printf("%s : parent done!\n", __FUNCTION__);
    }
    printf("%s : Function end!\n", __FUNCTION__);
}

int main(void)
{
    int ret = 0;

    // test 1: run new process
    //ret = test1_run_new_process_using_system();

    // test 2: run new process with exec
    // ret = test2_run_new_process_using_exec();

    // test 3: fork process.
    ret = test3_fork_new_process_using_fork();
    return ret;
}
