#include <cstdio>

const char rod[] = {'A', 'B', 'C'};
struct Move {
    int disk;
    int src, dst;
};

Move solve(int n, long long a, int src, int dst, int aux) {
    long long half = 1LL << (n - 1);
    if (a == half)
        return {n, src, dst};
    if (a < half)
        return solve(n - 1, a, src, aux, dst);
    else
        return solve(n - 1, a - half, aux, dst, src);
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int N;
        long long a;
        scanf("%d %lld", &N, &a);
        Move m = solve(N, a, 0, 2, 1);
        printf("%d : %c => %c\n", m.disk, rod[m.src], rod[m.dst]);
    }
    return 0;
}
