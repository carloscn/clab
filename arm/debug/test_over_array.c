#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv [ ])
{
	int *p = NULL;

	p = malloc(4);
	if (p == NULL)
	{
		perror("malloc failed");
	}
    printf("address [0x%p]\r\n", p);
	p[4] = 0;
	free(p);

    return 0;
}
