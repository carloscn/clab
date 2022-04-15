static int a;
extern int b;
extern void lib_sum();

void bar()
{
    a = 1;
    b = 2;
}

void foo()
{
    bar();
    lib_sum(a, b);
}
