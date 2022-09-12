#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char * argv [ ])
{
	int *p = NULL;

	p = (int*)malloc(sizeof(int));
	if (p == NULL)
	{
		perror("malloc failed");
	}
    printf("address [0x%p]\r\n", p);
	delete p;

    return 0;
}
