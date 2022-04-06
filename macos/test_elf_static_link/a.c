extern int b_share;
extern int b_func(int c, int d);
extern char start_of_text[];
int bsssss;

int main(void)
{
    int m = 0;
    m = b_func(m, b_share);
    return 0;
}
