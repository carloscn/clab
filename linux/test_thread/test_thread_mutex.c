#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

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
pthread_mutex_t mutex;

static void *thread01_handler(void *arg_s)
{
    int ret = EBUSY;
    int i = 0;
    int t_n = *(int *)arg_s;
#if enable_lock
    pthread_mutex_lock(&mutex);
#endif
    for (i = 0; i < 1024; i ++) {
       work_area[i] = t_n;
    }
    for (i = 0; i < 1024; i ++) {
        printf("%d ", work_area[i]);
    }
#if enable_lock
    pthread_mutex_unlock(&mutex);
#endif
    debug_log("thread %d: exit\n", t_n);
    pthread_exit("pthread 01: exit");
}

static int test1_basic_mutex()
{
    int ret = 0;
    pthread_t thread01, thread02;
    void *thread_result = NULL;

    ret = pthread_mutex_init(&mutex, NULL);
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
    pthread_mutex_destroy(&mutex);
exit:
    return ret;
}

pthread_mutex_t mt;

static void *test2_handler(void *arg)
{
    pthread_mutex_lock(&mt);
    debug_log("t2 running...\n");
    pthread_exit("exit performance test");

}

static int test2_mutex_performance()
{
    int ret = 0;
    pthread_t t0;
    void *result = NULL;
    pthread_mutex_init(&mt, NULL);
    pthread_mutex_lock(&mt);
    ret = pthread_create(&t0, NULL, test2_handler, NULL);
    if (ret != 0) {
        debug_log("create failed, ret = %d\n", ret);
        goto exit;
    }
    debug_log("wait for the t0 thread .....\n");
    ret = pthread_join(t0, &result);
    pthread_mutex_unlock(&mt);
exit:
    pthread_mutex_destroy(&mt);
    return ret;
}

int main(void)
{
    int ret = 0;
    ret = test1_basic_mutex();
    ret = test2_mutex_performance();
    printf("%s : test done !!\n", __FUNCTION__);

    return ret;
}
