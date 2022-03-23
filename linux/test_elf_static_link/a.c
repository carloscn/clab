extern int b_share;
extern int b_func(int c, int d);

static int a_0 = 0x7f;
static const char *a_1 = "aaaaaaaaaaaa";
static int a_3;
static int a_4 = 0;

static int a_func(int a, int b)
{
    return a - b;
}

int main(void)
{
    int m = 0;
    a_0 ++;
    a_1[5];
    a_3 = 1;
    a_4 = 5;
    m = b_func(m, b_share);
    m = a_func(m, b_share);
    return 0;
}