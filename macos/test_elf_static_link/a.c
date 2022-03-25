extern int b_share;
extern int b_func(int c, int d);

int main(void)
{
    int m = 0;
    m = b_func(m, b_share);
    return 0;
}