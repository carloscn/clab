#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/shm.h>
#include "test_shm.h"

#define debug_log printf("%s:%d--", __FUNCTION__, __LINE__);printf

int main(int argc, char *argv[])
{
    int i, ret;
    char op_chars[20];
    int count = 0;
    int shm_id = 0;
    int running = 1;

    void *share_memory = NULL;
    struct shared_use_st *share_stuff = NULL;

    debug_log("call the shmget function\n");
    shm_id = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);
    if (shm_id < 0) {
        debug_log("failed on semget\n");
        goto finish2;
    }

    debug_log("call the shmat\n");
    share_memory = shmat(shm_id, NULL, 0);
    if (share_memory == NULL - 1) {
        debug_log("failed on shmat\n");
        goto finish2;
    }
    debug_log("allocate memory attached at %x and main entry %x\n", (int)share_memory, (int)&main);
    share_stuff = (struct shared_use_st *)share_memory;
    share_stuff->written_by_you = 0;
    while(running) {
        if (share_stuff->written_by_you) {
            debug_log("You wrote: %s", share_stuff->some_text);
            sleep( rand() % 4);
            share_stuff->written_by_you = 0;
            if (strncmp(share_stuff->some_text, "end", 3) == 0) {
                running = 0;
            }
        }
    }
    debug_log("finish.....\n");
finish2:
    if (shmdt(share_memory) == -1) {
        debug_log("failed on share_memory\n");
        goto finish1;
    }
finish1:
    if (shmctl(shm_id, IPC_RMID, 0) == -1) {
        debug_log("failed on shmctl\n");
    }
    debug_log("finish test...\n");
    return ret;
}