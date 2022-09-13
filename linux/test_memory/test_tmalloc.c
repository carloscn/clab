#include <talloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// sudo apt install libtalloc-dev
// gcc test_tmalloc.c -ltalloc

int main(void)
{
    size_t i = 0;
    /* allocate 1KiB in a pool */
    TALLOC_CTX *pool_ctx = talloc_pool(NULL, 1024);
    mempool_alloc();
    /* Take 512B from the pool, 512B is left there */
    char *ptr = (char *)talloc_size(pool_ctx, 512);
    memset(ptr, '1', 512);
    for (i = 0; i < 512; i++) {
        printf("%c, ", ptr[i]);
    }
    /* 1024B > 512B, this will create new talloc chunk outside
    the pool */
    void *ptr2 = talloc_size(ptr, 1024);

    /* The pool still contains 512 free bytes
    * this will take 200B from them. */
    void *ptr3 = talloc_size(ptr, 200);

    /* This will destroy context 'ptr3' but the memory
    * is not freed, the available space in the pool
    * will increase to 512B. */
    talloc_free(ptr3);

    /* This will free memory taken by 'pool_ctx'
    * and 'ptr2' as well. */
    talloc_free(pool_ctx);
}