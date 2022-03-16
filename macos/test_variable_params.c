#include <stdio.h>
#include <stdarg.h>

static a = 10;

static int test1_add(int p_a, int p_b, ...)
{
    va_list p_list;
    int cnt = 0, i = 0;
    int sum = 0;
    printf("p_a is %d\n", p_a);
    va_start(p_list, p_b);
    for (i = p_b; i ; i = va_arg(p_list, int), cnt ++) {
        sum += i;
        printf("current i = %d\n", i);
    }
    va_end(p_list);
    printf("the cnt value is %d\n", cnt);
    return sum;
}
static int test2_printf(int p_a, int p_b, const char *format, ...) __attribute__((format(printf, 3, 4)));
static int test2_printf(int p_a, int p_b, const char *format, ...)
{
    va_list p_list;
    printf("p_a : %d\n", p_a);
    printf("p_b : %d\n", p_b);
    va_start(p_list, format);
    printf("call the printf to print format.\n");
    printf(format, p_list);
    va_end(p_list);
    return 0;
}

static void test3_local_static()
{
    static a = 100;
    a ++;
    printf("local function a addr: %p value : %d\n", &a, a);
    return;
}

static void test3_sub_static()
{
    a ++;
    printf("sub function a addr: %p value : %d\n", &a, a);
    return;
}

int main(void)
{
    int sum = 0;
    int ret;
    int i = 10;
    // test 1
    sum = test1_add(5,6,7,8,9,10,11,12,13,14,15,16, -1 , 0, -2, -3);
    printf("main : sum value is %d\n", sum);

    // test 2: lack a %d according parameter.
    // use the __attribute__((format(printf...))) have warning.
    // no use it, no warning
    ret = test2_printf(102, -107, "test2 input string %s, %d, %d\n", "life", 0);

    // test 3: static
    while(i --) {
        test3_local_static();
        test3_sub_static();
        a++;
        printf("main function a addr: %p value : %d\n\n", &a, a);
    }
    return 0;
}
