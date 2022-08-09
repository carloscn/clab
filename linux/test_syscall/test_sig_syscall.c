#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <signal.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

// $host arm-linux-gnueabihf-gcc test_sig_syscall.c -o test_sig_syscall.elf
// $host scp -r test_sig_syscall.elf root@192.168.31.210:/home/root/test_sig_syscall.elf

static volatile int signaled = 0;

static void handler(int signum)
{
    debug_log("signaled called %d\n", signum);
    signaled = 1;
}

int main(int argc, char *argv[])
{
    char ch;
    struct sigaction sigact;
    sigact.sa_handler = handler;
    sigact.sa_flags = SA_RESTART;   // enable restart on system call
    sigaction(SIGINT, &sigact, NULL);

    while(read(STDIN_FILENO, &ch, 1) != 1 && !signaled);

    return 0;
}
