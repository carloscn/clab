#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/msg.h>

#define debug_log printf("%s:%d--", __FUNCTION__, __LINE__);printf
#define MAX_TEXT 512
struct my_msg_st {
    long int my_msg_type;
    char some_text[MAX_TEXT];
};

int main(int argc, char *argv[])
{
    int i, ret;
    char op_chars[20];
    int count = 0;
    int msg_id = 0;
    int running = 1;
    char buffer[BUFSIZ];

    struct my_msg_st data;
    long int msg_to_recv = 0;

    debug_log("call the msgget function\n");
    msg_id = msgget((key_t) 1234, 0666 | IPC_CREAT);
    if (msg_id < 0) {
        debug_log("failed on semget\n");
        goto finish2;
    }
    while(running) {
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        data.my_msg_type = 1;
        strcpy(data.some_text, buffer);
        ret = msgsnd(msg_id, (void*)&data, MAX_TEXT, 0);
        if (ret == -1) {
           debug_log("failed on msgsnd\n");
           goto finish2;
        }
        if (strncmp(data.some_text, "end", 3) == 0) {
            running = 0;
        }
    }
    debug_log("finish.....\n");
finish2:
finish1:
    debug_log("finish test...\n");
    return ret;
}