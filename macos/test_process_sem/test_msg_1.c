#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/msg.h>

#define debug_log printf("%s:%d--", __FUNCTION__, __LINE__);printf

struct my_msg_st {
    long int my_msg_type;
    char some_text[BUFSIZ];
};

int main(int argc, char *argv[])
{
    int i, ret;
    char op_chars[20];
    int count = 0;
    int msg_id = 0;
    int running = 1;

    struct my_msg_st data;
    long int msg_to_recv = 0;

    debug_log("call the msgget function\n");
    msg_id = msgget((key_t) 1234, 0666 | IPC_CREAT);
    if (msg_id < 0) {
        debug_log("failed on semget\n");
        goto finish2;
    }
    while(running) {
        ret = msgrcv(msg_id, (void*)&data, BUFSIZ, msg_to_recv, 0);
        if (ret == -1) {
           debug_log("failed on msgrcv\n");
           goto finish2;
        }
        debug_log("You wrote: %s", data.some_text);
        if (strncmp(data.some_text, "end", 3) == 0) {
            running = 0;
        }
    }
    debug_log("finish.....\n");
finish2:
finish1:
    if (msgctl(msg_id, IPC_RMID, 0) == -1) {
        debug_log("failed on msgctl\n");
    }
    debug_log("finish test...\n");
    return ret;
}