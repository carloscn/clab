#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define debug_log printf("%s:%d--", __FUNCTION__, __LINE__);printf

/*
 * The test content is :
 * 1> the common area work_area[1024]
 * 2> main loop: record user input, recv "end" for mainloop returning.
 * 3> thread1 : print user input and count the input number.
 * 4> thread2 : backup the user input to work_backup[1024] and clear the work_area
 *
 * we need to protect the work_area using the sem.
 */

static char work_area[1024], back_area[1024];
static pthread_mutex_t mutex;
static pthread_cond_t cond;

static int common = 0;
// common ++ after 3s
// the producer will inform the consumer the common ++.
static void *thread_producer(void* args)
{
    for ( ; ; ) {
        pthread_mutex_lock(&mutex);
        common += 2;
        debug_log("thread producer add common is ++++++++++++++++++++++++++%d \n", common);
        debug_log("call pthread_cond_broadcase\n");
        //pthread_cond_signal(&cond);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
    pthread_exit("pthread 01: exit");
}
static void *thread_consumer_1(void* args)
{
    for ( ; ; ) {
        pthread_mutex_lock(&mutex);
        debug_log("call pthread_cond_wait\n");
        pthread_cond_wait(&cond, &mutex);
        debug_log("common --\n");
        common --;
        debug_log("thread consume_1 sub common is ----------------------------%d \n", common);
        if (common < 0) {
            debug_log("common is neg, failed\n");
            goto finish;
        }
        pthread_mutex_unlock(&mutex);
    }
finish:
    pthread_exit("pthread 01: exit");
}

static void *thread_consumer_2(void* args)
{
    for ( ; ; ) {
        pthread_mutex_lock(&mutex);
        debug_log("call pthread_cond_wait\n");
        pthread_cond_wait(&cond, &mutex);
        debug_log("common --\n");
        common --;
        debug_log("thread consume_2 sub common is ---------------------------------%d \n", common);
        if (common < 0) {
            debug_log("common is neg, failed\n");
            goto finish;
        }
        pthread_mutex_unlock(&mutex);
    }
finish:
    pthread_exit("pthread 01: exit");
}

static int test1_basic_cond()
{
    int ret = 0;
    pthread_t thread01, thread02, thread03;
    void *thread_result = NULL;

    ret = pthread_mutex_init(&mutex, NULL);
    if (ret != 0) {
        debug_log("failed on mutex_init ret = %d\n", ret);
        perror("failed reason: ");
        goto exit;
    }
    ret = pthread_cond_init(&cond, NULL);
    if (ret != 0) {
        debug_log("failed on cond_init ret = %d\n", ret);
        perror("failed reason: ");
        goto exit;
    }
    ret = pthread_create(&thread01, NULL, thread_producer, NULL);
    if (ret != 0) {
        debug_log("create failed, ret = %d\n", ret);
        goto exit;
    }
    ret = pthread_create(&thread02, NULL, thread_consumer_1, NULL);
    if (ret != 0) {
        debug_log("create failed, ret = %d\n", ret);
        goto exit;
    }
    ret = pthread_create(&thread02, NULL, thread_consumer_2, NULL);
    if (ret != 0) {
        debug_log("create failed, ret = %d\n", ret);
        goto exit;
    }
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
    ret = pthread_join(thread03, &thread_result);
    if (ret != 0) {
        debug_log("join failed, ret = %d\n", ret);
        goto exit;
    }
    debug_log("back up area is %s\n", back_area);
    debug_log("sub thread msg : %s\n test1 finish\n", (const char *)thread_result);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
exit:
    return ret;
}

int main(void)
{
    int ret = 0;
    ret = test1_basic_cond();
    printf("%s : test done !!\n", __FUNCTION__);

    return ret;
}
