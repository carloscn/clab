#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

#define P_CMD "./helper_process_long_loop.elf 5"

int main(void)
{
    FILE *read_fp = NULL;
    char buffer[BUFSIZ + 1];
    int chars_read;
    memset(buffer, '\0', sizeof buffer);
    read_fp = popen(P_CMD, "r");
    debug_log("popen start ...\n");
    if (NULL == read_fp) {
        debug_log("popen failed\n");
        return -1;
    }
    // read one time.
#if 1
    chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
    if (chars_read > 0) {
        debug_log("Output was:  \n\n %s \n\n", buffer);
    }
#else
    // read multi times.
    do {
        chars_read = fread(buffer, sizeof(char), 32, read_fp);
        buffer[chars_read - 1] = '\0';
        debug_log("Output was:  \n\n %s \n\n", buffer);
    } while (chars_read > 0);
#endif
    debug_log("popen end ...\n");
    pclose(read_fp);
    return 0;
}