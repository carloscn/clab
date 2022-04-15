#include <stdio.h>
#include "libprov.h"

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

int main(void)
{
    int ret = 0;
    debug_log("main start\n");
    ret = prov_lib_init("main init function\n");
    ret = prov_lib_do("main do prov\n");
    prov_lib_cleanup();
    debug_log("main end\n");
    return ret;
}