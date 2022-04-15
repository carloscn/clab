#include <stdio.h>
#include <dlfcn.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

int main(void)
{
    int ret = 0;
    debug_log("main start\n");
    void *handle = NULL;
    int (*prov_init)(const char *) = NULL;
    int (*prov_do)(const char *) = NULL;
    void (*prov_cleanup)(void) = NULL;

    handle = dlopen("./libprov.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }
    prov_init = (int (*)(const char *)) dlsym(handle, "prov_lib_init");
    prov_do = (int (*)(const char *)) dlsym(handle, "prov_lib_do");
    prov_cleanup = (void (*)(void)) dlsym(handle, "prov_lib_cleanup");

    ret = prov_init("main init function\n");
    ret = prov_do("main do prov\n");
    prov_cleanup();

    dlclose(handle);

    debug_log("main end\n");
    return ret;
}