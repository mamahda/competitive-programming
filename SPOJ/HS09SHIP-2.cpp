#include <stdio.h>
#include <math.h>

double logfak[1001];

void precompute() {
    logfak[0] = 0.0;
    for (int i = 1; i <= 1000; i++) {
        logfak[i] = logfak[i-1] + log((double)i);
    }
}

double logC(int n, int r) {
    return logfak[n] - logfak[r] - logfak[n - r];
}

double logP1D(int k, int d) {
    if (d < 0) d = -d;
    if (k == 0) {
        if (d == 0) return 0.0;
        else return -1e18;
    }
    if ((k + d) % 2 != 0) return -1e18;
    int r = (k + d) / 2;
    if (r < 0 || r > k) return -1e18;
    return logC(k, r) - (double)k * log(2.0);
}

int main() {
    precompute();

    double log3 = log(3.0);

    int T;
    scanf("%d", &T);

    while (T--) {
        int n;
        scanf("%d", &n);

        int x1, y1, z1, x2, y2, z2;
        scanf("%d %d %d", &x1, &y1, &z1);
        scanf("%d %d %d", &x2, &y2, &z2);

        int dx = x2 - x1;
        int dy = y2 - y1;
        int dz = z2 - z1;
        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        if (dz < 0) dz = -dz;

        double totalProb = 0.0;

        for (int a = 0; a <= n; a++) {
            double lpx = logP1D(a, dx);
            if (lpx <= -1e17) continue;

            for (int b = 0; b <= n - a; b++) {
                int c = n - a - b;

                double lpy = logP1D(b, dy);
                if (lpy <= -1e17) continue;

                double lpz = logP1D(c, dz);
                if (lpz <= -1e17) continue;

                double logMulti = logfak[n] - logfak[a] - logfak[b] - logfak[c];
                double log13n = -(double)n * log3;
                double logP = logMulti + log13n + lpx + lpy + lpz;

                totalProb += exp(logP);
            }
        }

        printf("%.10f\n", totalProb);
    }

    return 0;
}
