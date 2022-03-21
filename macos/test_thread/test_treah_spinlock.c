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

static char work_area[1024], back_area[1024];
sem_t *sem_1 = NULL;
sem_t *sem_2 = NULL;

static void *thread01_handler(void *arg_s)
{
    while(strncmp("end", work_area, 3) != 0) {
        printf("thread 1: user input %zd characters\n", strlen(work_area) - 1);
        work_area[strlen(work_area) - 1] = '!';
        sem_wait(sem_1);
        sem_post(sem_2);
    }
    pthread_exit("pthread 01: exit");
}

static void *thread02_handler(void *arg_s)
{
    while(strncmp("end", work_area, 3) != 0) {
        strcpy(back_area, work_area);
        memset(work_area, 0, 1024);
        printf("thread 2: back up the work: %s\n", back_area);
        sem_wait(sem_2);
    }
    pthread_exit("pthread 02: exit");
}

static int test1_basic_sem()
{
    int ret = 0;
    pthread_t thread01, thread02;
    void *thread_result = NULL;
#define     SEM01   "sem01"
#define     SEM02   "sem02"
    //ret = sem_init(&sem1, 0, 0);
    sem_1 = sem_open(SEM01, O_CREAT|O_EXCL, S_IRWXU, 0);
    if (sem_1 == NULL) {
        debug_log("failed on sem_init ret = %d\n", ret);
        perror("failed reason: ");
        goto exit;
    }
    //ret = sem_init(&sem2, 0, 0);
    sem_2 = sem_open(SEM02, O_CREAT|O_EXCL, S_IRWXU, 0);
    if (sem_2 == NULL) {
        debug_log("failed on sem_init ret = %d\n", ret);
        goto exit;
    }
    ret = pthread_create(&thread01, NULL, thread01_handler, NULL);
    if (ret != 0) {
        debug_log("create failed, ret = %d\n", ret);
        goto exit;
    }
    ret = pthread_create(&thread02, NULL, thread02_handler, NULL);
    if (ret != 0) {
        debug_log("create failed, ret = %d\n", ret);
        goto exit;
    }

    printf("Input some text. Enter 'end' to finish! \n");
    while (strncmp("end", work_area, 3) != 0) {
        fgets(work_area, 1024, stdin);
        printf("mainloop : get the input %s\n", work_area);
        sem_post(sem_1);
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
    debug_log("back up area is %s\n", back_area);
    debug_log("sub thread msg : %s\n test1 finish\n", (const char *)thread_result);
    //sem_destroy(&sem1);
    //sem_destroy(&sem2);
    sem_unlink(SEM01);
    sem_unlink(SEM02);
exit:
    return ret;
}

int main(void)
{
    int ret = 0;
    ret = test1_basic_sem();

    printf("%s : test done !!\n", __FUNCTION__);

    return ret;
}
