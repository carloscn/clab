#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#define debug_log printf("%s:%s:%d--",__FILE__, __FUNCTION__, __LINE__);printf

int test_4k_align_using_the_clear()
{
    // find value 4k, 2k, 1k align value.
    uint32_t value = 487878;
    // 4k = 2^12  round_down
    debug_log("value : %d, 4k value : %d \n", value, value & 0xFFFFF000);
}

int main(void)
{
    int ret = 0;
    test_4k_align_using_the_clear();
    return 0;
}
