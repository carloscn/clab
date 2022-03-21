#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

/*
 * The test content is :
 * 1> create new thread, and trans the paramster "hello"
 * 2> sub-thread needs to sleep (5) s
 * 3> main loop need to wait the thread ending.
 * 4> main loop printf "done".
 */
static void *thread_handler(void *arg_s)
{
    debug_log("param: %s\n", (const char*)arg_s);
    debug_log("thread will sleep 5s\n");
    sleep(5);
    debug_log("thread wake up and exit\n");
    pthread_exit("pthread exit: wake up");
}

static int test1_basic_threads()
{
    int ret = 0;
    pthread_t thread;
    void *thread_result = NULL;
    void *args = "hello";

    ret = pthread_create(&thread, NULL, thread_handler, (void*)args);
    if (ret != 0) {
        debug_log("create failed, ret = %d\n", ret);
        goto exit;
    }
    debug_log("Wait for sub thread.\n");
    ret = pthread_join(thread, &thread_result);
    if (ret != 0) {
        debug_log("join failed, ret = %d\n", ret);
        goto exit;
    }
    debug_log("sub thread msg : %s\n test1 finish\n", (const char *)thread_result);

exit:
    return ret;
}

#define TURN_COUNT (100)
static char *msg = "thread x exit   ";

static void *thread0_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 0;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static void *thread1_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 1;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static void *thread2_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 2;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static void *thread3_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 3;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static void *thread4_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 4;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static void *thread5_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 5;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static void *thread6_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 6;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static void *thread7_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 7;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static void *thread8_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 8;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static void *thread9_hdl(void *arg)
{
    int i = TURN_COUNT;
    int d = 9;
    while(i --) {
        printf("%d", d);
    }
    //sprintf(msg, "thread %d exit", d);
    pthread_exit(msg);
}

static int test2_10_threads_no_sync(void)
{
    int ret = 0;
    pthread_t th0, th1, th2, th3, th4, th5, th6, th7, th8, th9;
    void *result = NULL;

    debug_log("create 10 threads\n");
    pthread_create(&th0, NULL, thread0_hdl, NULL);
    pthread_create(&th1, NULL, thread1_hdl, NULL);
    pthread_create(&th2, NULL, thread2_hdl, NULL);
    pthread_create(&th3, NULL, thread3_hdl, NULL);
    pthread_create(&th4, NULL, thread4_hdl, NULL);
    pthread_create(&th5, NULL, thread5_hdl, NULL);
    pthread_create(&th6, NULL, thread6_hdl, NULL);
    pthread_create(&th7, NULL, thread7_hdl, NULL);
    pthread_create(&th8, NULL, thread8_hdl, NULL);
    pthread_create(&th9, NULL, thread9_hdl, NULL);
    debug_log("created 10 treads.\n");

    pthread_join(th0, result);
    debug_log("thread 0 exit %s\n", (const char*)result);
    pthread_join(th1, result);
    debug_log("thread 1 exit %s\n", (const char*)result);
    pthread_join(th2, result);
    debug_log("thread 2 exit %s\n", (const char*)result);
    pthread_join(th3, result);
    debug_log("thread 3 exit %s\n", (const char*)result);
    pthread_join(th4, result);
    debug_log("thread 4 exit %s\n", (const char*)result);
    pthread_join(th5, result);
    debug_log("thread 5 exit %s\n", (const char*)result);
    pthread_join(th6, result);
    debug_log("thread 6 exit %s\n", (const char*)result);
    pthread_join(th7, result);
    debug_log("thread 7 exit %s\n", (const char*)result);
    pthread_join(th8, result);
    debug_log("thread 8 exit %s\n", (const char*)result);
    pthread_join(th9, result);
    debug_log("thread 9 exit %s\n", (const char*)result);
    debug_log("test 2 finish \n");

    return ret;
}

int main(void)
{
    int ret = 0;
    // test 1: thread create exit join;
    //ret = test1_basic_threads();
    ret = test2_10_threads_no_sync();

    printf("%s : test done !!\n", __FUNCTION__);

    return ret;
}
