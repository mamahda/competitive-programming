#include <cstdio>

int main()
{
    int t;
    scanf("%d", &t);

    while (t--)
    {
        int n;
        scanf("%d", &n);
        if (n == 1)
        {
            printf("1.0\n");
            continue;
        }
        double prob = 1.0;
        for (int i = 1; i < n; ++i)
        {
            prob *= (2.0 * i - 1.0) / (2.0 * i);
        }
        printf("%lf\n", prob);
    }

    return 0;
}
