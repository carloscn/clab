#include <stdio.h>
#include <stdlib.h>

unsigned int max = 0;

int main()
{
    unsigned int blocksize[] = {1024 * 1024, 1024, 1};
    int i, count;
    for (i = 0; i < 3; i++)
    {
        for (count = 1; ; count++)
        {
            void* block = malloc(max + blocksize[i] * count);
            if (block)  //malloc ok
            {
                max += blocksize[i] * count;
                free(block);
            }
            else
            {
                break;
            }
        }
    }
    printf("max malloc size = %uB\n", max);
    return 0;
}
