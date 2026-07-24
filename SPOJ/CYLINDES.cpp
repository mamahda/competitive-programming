#include <bits/stdc++.h>
using namespace std;

struct P {
    double h, a, r;
};

P A, B;
double R, H;

// --- jarak bentuk tertutup ---
inline double diskDist(double r1, double a1, double r2, double a2) {
    return sqrt(r1 * r1 + r2 * r2 - 2 * r1 * r2 * cos(a1 - a2));
}

inline double ptRim(double rf, double af, double phi) {
    return sqrt(rf * rf + R * R - 2 * rf * R * cos(phi - af));
}

inline double rimRim(double da) {
    return 2.0 * R * fabs(sin(da * 0.5));
}

inline double hel(double da, double dh) {
    double x = R * da;
    return sqrt(dh * dh + x * x);
}

void preprocess() {
    if (A.a > B.a)
        swap(A, B);

    if (B.a - A.a > 180.0) {
        B.a = 360.0 - B.a + A.a;
        A.a = 0.0;
    }

    A.a *= M_PI / 180.0;
    B.a *= M_PI / 180.0;
}

const double GR = 0.6180339887498949;

template <class F>
double golden(double a, double b, F f) {
    double c = b - GR * (b - a);
    double d = a + GR * (b - a);

    double fc = f(c);
    double fd = f(d);

    while (b - a > 1e-4) {
        if (fc < fd) {
            b = d;
            d = c;
            fd = fc;

            c = b - GR * (b - a);
            fc = f(c);
        } else {
            a = c;
            c = d;
            fc = fd;

            d = a + GR * (b - a);
            fd = f(d);
        }
    }

    return (fc < fd ? fc : fd);
}

double case1() {
    return diskDist(A.r, A.a, B.r, B.a);
}

double case2() {
    if (!(A.h == 0.0 || A.h == H))
        swap(A, B);

    double lo = min(A.a, B.a);
    double hi = max(A.a, B.a);
    double zc = A.h;

    return golden(lo, hi, [&](double phi) {
        return ptRim(A.r, A.a, phi)
             + hel(phi - B.a, zc - B.h);
    });
}

double case3() {
    double lo = min(A.a, B.a);
    double hi = max(A.a, B.a);
    double zA = A.h;
    double zB = B.h;

    return golden(lo, hi, [&](double pc) {
        double d = ptRim(A.r, A.a, pc);

        return golden(pc, hi, [&](double pd) {
            return d
                 + hel(pc - pd, zA - zB)
                 + ptRim(B.r, B.a, pd);
        });
    });
}

double case4cap(double t) {
    double lo = min(A.a, B.a);
    double hi = max(A.a, B.a);

    return golden(lo, hi, [&](double pc) {
        double d = hel(A.a - pc, A.h - t);

        return golden(pc, hi, [&](double pd) {
            return d
                 + rimRim(pc - pd)
                 + hel(pd - B.a, t - B.h);
        });
    });
}

double solve() {
    preprocess();

    if (A.h == B.h && (A.h == H || A.h == 0.0))
        return case1();

    if ((A.h == 0.0 && B.h == H) ||
        (B.h == 0.0 && A.h == H))
        return case3();

    if (A.h == H || B.h == H ||
        A.h == 0.0 || B.h == 0.0)
        return case2();

    return min(
        hel(A.a - B.a, A.h - B.h),
        min(case4cap(0.0), case4cap(H))
    );
}

int main() {
    int T;

    if (scanf("%d", &T) != 1)
        return 0;

    while (T--) {
        scanf("%lf %lf", &R, &H);
        scanf("%lf %lf %lf", &A.h, &A.a, &A.r);
        scanf("%lf %lf %lf", &B.h, &B.a, &B.r);

        printf("%.2f\n", solve());
    }

    return 0;
}
