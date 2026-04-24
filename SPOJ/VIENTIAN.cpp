#include <cstdio>

const long long INF = 4611686018427387904;

long long memo[512][40][3][3];
bool vis [512][40][3][3];

long long solve(int mask, int n, int src, int dst) {
    if (n == 0 || src == dst) return 0;
    if (vis[mask][n][src][dst]) return memo[mask][n][src][dst];

    vis[mask][n][src][dst] = true;
    long long best = INF;

    if ((mask >> (src * 3 + dst)) & 1) {
        for (int aux = 0; aux < 3; aux++) {
            if (aux == src || aux == dst) continue;
            long long c1 = solve(mask, n - 1, src, aux);
            if (c1 >= INF) continue;
            long long c2 = solve(mask, n - 1, aux, dst);
            if (c2 >= INF) continue;
            long long t = c1 + 1 + c2;
            if (t < best) best = t;
        }
    }

    for (int via = 0; via < 3; via++) {
        if (via == src || via == dst) continue;
        if (!((mask >> (src * 3 + via)) & 1)) continue;
        if (!((mask >> (via * 3 + dst)) & 1)) continue;
        long long c1 = solve(mask, n - 1, src, dst);
        if (c1 >= INF) continue;
        long long c3 = solve(mask, n - 1, dst, src);
        if (c3 >= INF) continue;
        long long t = 2 * c1 + c3 + 2;
        if (t < best) best = t;
    }

    memo[mask][n][src][dst] = best;
    return best;
}

int main() {
    int t;
    scanf("%d", &t);

    while (t--) {
        int mask = 0;
        for (int i = 0; i < 3; i++) {
            char row[8];
            scanf("%s", row);
            for (int j = 0; j < 3; j++)
                if (row[j] == '1')
                    mask |= (1 << (i * 3 + j));
        }
        int n;
        scanf("%d", &n);

        long long result = solve(mask, n, 0, 2);

        if (result >= INF)
            printf("Epic Fail...\n");
        else
            printf("%lld\n", result);
    }
    return 0;
}
