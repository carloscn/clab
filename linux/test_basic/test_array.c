#include <stdio.h>
#include <stdint.h>


struct Test {
    unsigned short int a:5;
    unsigned short int b:5;
    unsigned short int c:6;
};

int sem = 8;

void fun1(char a1[3]) {
    printf("a1 = %ld\n", sizeof(a1));
}
void fun2(char a2[]) {
    printf("a2 = %ld\n", sizeof(a2));
}

int main(void)
{
    int sem = sem;
    char s[3];
    struct Test test;
    printf("sem = %d\n", sem);
    test.a = 16;
    test.b = 4;
    test.c = 0;
    int i = *(short*)&test;
    printf("0x%x\n", i);
    fun1(s);
    fun2(s);
    return 0;


    char *p = "hello\n";
    int b = 2;
    p += 2;
    //p[0] = 'x';  /* cannot be changed, because the memory on the readonly area. */
    printf("%c\n", p[0]);
    printf("%c\n", p[b]);
    return 0;
}

