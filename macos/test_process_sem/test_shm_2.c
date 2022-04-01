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
    char buffer[BUFSIZ];

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
    debug_log("allocate memory attached at %x and main entry %x\n", (int*)share_memory, (int)&main);
    share_stuff = (struct shared_use_st *)share_memory;
    share_stuff->written_by_you = 0;
    while (running) {
        while (share_stuff->written_by_you == 1) {
            sleep(1);
            debug_log("waiting for client\n");
        }
        debug_log("Enter some test: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(share_stuff->some_text, buffer, TEXT_SZ);
        share_stuff->written_by_you = 1;
        if (strncmp(buffer, "end", 3) == 0) {
            running = 0;
        }
    }
    debug_log("finish.....\n");
finish2:
    if (shmdt(share_memory) == -1) {
        debug_log("failed on share_memory\n");
    }
    debug_log("finish test...\n");
    return ret;
}