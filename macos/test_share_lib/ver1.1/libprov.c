#include <stdio.h>
#include "libprov.h"

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

int prov_lib_init(const char *init_info)
{
    if (init_info == NULL) {
        debug_log("bad input parameter\n");
        return -1;
    }

    debug_log("do init: %s\n", init_info);

    return 0;
}

int prov_lib_do(const char *do_info)
{
    if (do_info == NULL) {
        debug_log("bad input parameter\n");
        return -1;
    }

    debug_log("do init: %s\n", do_info);

    return 0;
}

void prov_lib_cleanup()
{
    debug_log("do cleanup\n");
}

int __attribute__((constructor(5))) prov_lib_init_auto()
{
    debug_log("call the auto init\n");
    return 0;
}

void __attribute__((destructor(10))) prov_lib_cleanup_auto()
{
    debug_log("call the auto clean\n");
}