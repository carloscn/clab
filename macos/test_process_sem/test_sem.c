#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/sem.h>

#define debug_log printf("%s:%d--", __FUNCTION__, __LINE__);printf

#if defined(__linux__)
union semun {                   /* Used in calls to semctl() */
    int                 val;
    struct semid_ds *   buf;
    unsigned short *    array;
#if defined(__linux__)
    struct seminfo *    __buf;
#endif
};
#endif

static int set_semvalue(int sem_id)
{
    int ret = 0;
    union semun su;
    su.val = 1;
    ret = semctl(sem_id, 0, SETVAL, su);
    if (ret != 0) {
        debug_log("failed on semctl, ret = %d\n", ret);
        goto finish;
    }
finish:
    return ret;
}

static void del_semvalue(int sem_id)
{
    int ret = 0;
    union semun su;
    ret = semctl(sem_id, 0, IPC_RMID, su);
    if (ret != 0) {
        debug_log("failed on semctl, ret = %d\n", ret);
    }
}

static int sem_p(int sem_id)
{
    struct sembuf sem_b;
    int ret = 0;

    sem_b.sem_num = 0;
    sem_b.sem_op = -1;  /* P() */
    sem_b.sem_flg = SEM_UNDO;
    ret = semop(sem_id, &sem_b, 1);
    if (ret != 0) {
        debug_log("semop failed, ret = %d\n", ret);
    }
    return ret;
}

static int sem_v(int sem_id)
{
    struct sembuf sem_b;
    int ret = 0;

    sem_b.sem_num = 0;
    sem_b.sem_op = 1;  /* V() */
    sem_b.sem_flg = SEM_UNDO;
    ret = semop(sem_id, &sem_b, 1);
    if (ret != 0) {
        debug_log("semop failed, ret = %d\n", ret);
    }
    return ret;
}

int main(int argc, char *argv[])
{
    int i, ret;
    char op_chars[20];
    FILE *fd = NULL;
    int count = 0;
    int sem_id = 0;

    fd = fopen("common.txt", "a+");
    if (NULL == fd) {
        debug_log("failed on fopen\n");
        goto finish1;
    }
    sem_id = semget((key_t)1234, 1, 0666|IPC_CREAT);

    if (sem_id < 0) {
        debug_log("failed on semget\n");
        goto finish2;
    }

    if (argc > 1) {
        strcpy(op_chars, "111111111\n");
        debug_log("op char will write 1 to common.txt\n");
        ret = set_semvalue(sem_id);
        if (ret != 0) {
            debug_log("set_semvalue failed\n");
            goto finish2;
        }
    } else {
        strcpy(op_chars, "000000000\n");
        debug_log("op char will write 0 to common.txt\n");
    }

    debug_log("start write file.....\n");
    debug_log("sem_p().....\n");
    ret = sem_p(sem_id);
    if (ret != 0) {
        debug_log("failed on sem_p\n");
        goto finish3;
    }
    for (i = 0; i < 10; i++) {
        ret = fwrite(op_chars, 1, strlen(op_chars), fd);
        if (ret < 0) {
            debug_log("failed on fwrite\n");
            goto finish3;
        }
        count += ret;
        sleep(1);
        debug_log("write 10 bytes to file. total = %d\n", count);
    }
    debug_log("sem_v().....\n");
    ret = sem_v(sem_id);
    if (ret != 0) {
        debug_log("failed on sem_v\n");
    }

finish3:
    if (argc > 1) {
        // it is very important for another process using the semvalue.
        debug_log("hold process .... 20s\n");
        sleep(20);
        del_semvalue(sem_id);
    }
finish2:
    fclose(fd);
finish1:
    debug_log("finish test...\n");
    return ret;
}