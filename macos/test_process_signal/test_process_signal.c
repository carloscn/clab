#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static volatile int ctrl_c_flag = 0;
static void ctrl_c_handler(int sig)
{
    ctrl_c_flag = 1;
}

static int test1_register_ctrl_c(void)
{
    int i = 9999;

    printf("%s : test1: waiting for your inputing ctrl - c!\n", __FUNCTION__);
    (void) signal(SIGINT, ctrl_c_handler);
    ctrl_c_flag = 0;

    while (!ctrl_c_flag) {
        sleep(1);
    }
    printf("%s : recv ctrl - c signal!\n", __FUNCTION__);
    return 0;
}

static int test1_1_register_ctrl_c(void)
{
    int i = 0;
    struct sigaction act;

    act.sa_handler = ctrl_c_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, 0);
    printf("%s : test1: waiting for your inputing ctrl - c!\n", __FUNCTION__);
    ctrl_c_flag = 0;

    while (!ctrl_c_flag) {
        sleep(1);
    }
    printf("%s : recv ctrl - c signal!\n", __FUNCTION__);
    return 0;
}

static volatile int alarm_flag = 0;
static void alarm_timeout_handler(int sig)
{
    alarm_flag = 1;
}

static int test2_alarm(void)
{
    int i = 9999;

    printf("%s : test2: waiting for the alarm timeout signal!\n", __FUNCTION__);
    (void) signal(SIGALRM, alarm_timeout_handler);
    alarm_flag = 0;
    alarm(5);
    while (!alarm_flag) {
        sleep(1);
    }
    printf("%s : recv SIGALRM signal!\n", __FUNCTION__);
    return 0;
}

static int test2_1_alarm(void)
{
    int i = 9999;

    struct sigaction act;
    act.sa_handler = alarm_timeout_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGALRM, &act, 0);
    printf("%s : test2: waiting for the alarm timeout signal!\n", __FUNCTION__);
    alarm_flag = 0;
    alarm(5);

    while (!alarm_flag) {
        sleep(1);
    }
    printf("%s : recv SIGALRM signal!\n", __FUNCTION__);
    return 0;
}

int main(void)
{
    int ret = 0;
    // test 1: register ctrl c;
    ret = test1_register_ctrl_c();
    ret = test1_1_register_ctrl_c();
    ret = test2_alarm();
    ret = test2_1_alarm();

    printf("%s : test done !!\n", __FUNCTION__);

    return ret;
}
