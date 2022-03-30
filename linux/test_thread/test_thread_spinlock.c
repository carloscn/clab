#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

/*
 * The test content is :
 * 1> the common area work_area[1024]
 * 2> main loop: record user input, recv "end" for mainloop returning.
 * 3> thread1 : print user input and count the input number.
 * 4> thread2 : backup the user input to work_backup[1024] and clear the work_area
 *
 * we need to protect the work_area using the sem.
 */

#define enable_lock 1
static char work_area[1024], back_area[1024];
pthread_spinlock_t lock01;

static void *thread01_handler(void *arg_s)
{
    int ret = EBUSY;
    int i = 0;
    int t_n = *(int *)arg_s;
#if enable_lock
    pthread_spin_lock(&lock01);
#endif
    for (i = 0; i < 1024; i ++) {
       work_area[i] = t_n;
    }
    for (i = 0; i < 1024; i ++) {
        printf("%d ", work_area[i]);
    }
#if enable_lock
    pthread_spin_unlock(&lock01);
#endif
    debug_log("thread %d: exit\n", t_n);
    pthread_exit("pthread 01: exit");
}

static int test1_basic_spinlock()
{
    int ret = 0;
    pthread_t thread01, thread02;
    void *thread_result = NULL;

    ret = pthread_spin_init(&lock01, 0);
    if (ret != 0) {
        debug_log("failed on spin_init ret = %d\n", ret);
        perror("failed reason: ");
        goto exit;
    }
    int a = 0, b = 1;
    ret = pthread_create(&thread01, NULL, thread01_handler, (void *)&a);
    if (ret != 0) {
        debug_log("create failed, ret = %d\n", ret);
        goto exit;
    }
    ret = pthread_create(&thread02, NULL, thread01_handler, (void *)&b);
    if (ret != 0) {
        debug_log("create failed, ret = %d\n", ret);
        goto exit;
    }
    printf("Input some text. Enter 'end' to finish! \n");

    debug_log("Wait for sub thread.\n");
    ret = pthread_join(thread01, &thread_result);
    if (ret != 0) {
        debug_log("join failed, ret = %d\n", ret);
        goto exit;
    }
    debug_log("sub thread msg : %s\n test1 finish\n", (const char *)thread_result);

    ret = pthread_join(thread02, &thread_result);
    if (ret != 0) {
        debug_log("join failed, ret = %d\n", ret);
        goto exit;
    }
    debug_log("back up area is %s\n", back_area);
    debug_log("sub thread msg : %s\n test1 finish\n", (const char *)thread_result);
    pthread_spin_destroy(&lock01);
exit:
    return ret;
}


pthread_spinlock_t lock;
pthread_spinlock_t lock_live;

static void *test2_handler(void *arg)
{
    pthread_spin_lock(&lock);
    debug_log("t2 running...\n");
    pthread_exit("exit performance test");
}


static volatile int ctrl_c_flag = 0;

static void *test3_handler(void *arg)
{
    static int count = 999;
    while(1) {
        pthread_spin_lock(&lock_live);
        debug_log("t3 locked\n");
        debug_log("t3 running %d times...\n", count);
        sleep(1);
        debug_log("t3 unlocked\n");
        pthread_spin_unlock(&lock_live);
        if (ctrl_c_flag == 1) {
            goto h_exit;
        }
    }
h_exit:
    pthread_exit("exit performance test");
}

static void ctrl_c_handler(int sig)
{
    (void)sig;
    ctrl_c_flag = 1;
    debug_log("recv ctrl-c signal\n");
}

static int test2_spinlock_performance()
{
    int ret = 0;
    pthread_t t0, t1;
    void *result = NULL;
    pthread_spin_init(&lock, 0);
    pthread_spin_init(&lock_live, 0);
    pthread_spin_lock(&lock);
    (void) signal(SIGINT, &ctrl_c_handler);
    ret = pthread_create(&t0, NULL, test2_handler, NULL);
    if (ret != 0) {
        debug_log("create t2 failed, ret = %d\n", ret);
        goto exit;
    }
    ret = pthread_create(&t1, NULL, test3_handler, NULL);
    if (ret != 0) {
        debug_log("create t3 failed, ret = %d\n", ret);
        goto exit;
    }
    debug_log("wait for the t1 thread .....\n");
    ret = pthread_join(t1, &result);
    debug_log("t1 done .....\n");
    debug_log("wait for the t0 thread .....\n");
    ret = pthread_join(t0, &result);
    debug_log("t0 done .....\n");
    pthread_spin_unlock(&lock);
exit:
    pthread_spin_destroy(&lock);
    pthread_spin_destroy(&lock_live);
    return ret;
}

int main(void)
{
    int ret = 0;
    ret = test1_basic_spinlock();
    ret = test2_spinlock_performance();

    printf("%s : test done !!\n", __FUNCTION__);

    return ret;
}
