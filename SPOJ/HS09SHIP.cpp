/*
 * STARSHIP 3D RANDOM WALK - Probabilitas sampai tujuan dalam tepat n langkah
 *
 * Pendekatan Rekurensi:
 *   Dekomposisi gerakan 3D menjadi tiga walk 1D yang independen.
 *   Setiap langkah memilih sumbu x/y/z dengan peluang 1/3 lalu ±1 dengan peluang 1/2.
 *
 * Formula inti:
 *   P(n, dx, dy, dz) =
 *     sum_{ax+ay+az=n} [C(n;ax,ay,az)/3^n] * p1d(ax,dx) * p1d(ay,dy) * p1d(az,dz)
 *
 * Rekurensi yang digunakan:
 *   bin2[m][j] = C(m,j)/2^m   → peluang binomial (p=0.5)
 *              = 0.5*bin2[m-1][j-1] + 0.5*bin2[m-1][j]
 *
 *   bin3[n][k] = C(n,k)*(1/3)^k*(2/3)^(n-k)  → peluang binomial (p=1/3)
 *              = (1/3)*bin3[n-1][k-1] + (2/3)*bin3[n-1][k]
 *
 *   p1d(k, d) = bin2[k][(k+d)/2]   jika k>=d dan (k-d) genap, else 0
 *
 * Semua nilai antara [0,1] → tidak ada overflow!
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1001;

// ─────────────────────────────────────────────────────────────────────────────
// TABEL REKURENSI
// ─────────────────────────────────────────────────────────────────────────────

// bin2[m][j] = C(m,j) / 2^m
// Makna: peluang mendapat tepat j "maju" dalam m lemparan koin adil
double bin2[MAXN][MAXN];

// bin3[n][k] = C(n,k) * (1/3)^k * (2/3)^(n-k)
// Makna: peluang tepat k langkah sumbu-x dari n total langkah (pilih axis dengan p=1/3)
double bin3[MAXN][MAXN];

void precompute() {
    // ── Rekurensi bin2 ────────────────────────────────────────────────────────
    // Basis: bin2[0][0] = 1 (0 langkah, displacement 0, pasti benar)
    // Rekurensi: bin2[m][j] = 0.5*bin2[m-1][j-1] + 0.5*bin2[m-1][j]
    //   (langkah ke-m bisa "+" sehingga sebelumnya j-1, atau "-" sehingga sebelumnya j)
    bin2[0][0] = 1.0;
    for (int m = 1; m < MAXN; m++) {
        bin2[m][0] = bin2[m-1][0] * 0.5;               // hanya bisa dari bin2[m-1][0]
        for (int j = 1; j < m; j++)
            bin2[m][j] = 0.5 * (bin2[m-1][j-1] + bin2[m-1][j]);
        bin2[m][m] = bin2[m-1][m-1] * 0.5;             // hanya bisa dari bin2[m-1][m-1]
    }

    // ── Rekurensi bin3 ────────────────────────────────────────────────────────
    // Basis: bin3[0][0] = 1
    // Rekurensi: bin3[n][k] = (1/3)*bin3[n-1][k-1] + (2/3)*bin3[n-1][k]
    //   (langkah ke-n bisa sumbu-x dengan p=1/3, atau bukan-x dengan p=2/3)
    bin3[0][0] = 1.0;
    for (int n = 1; n < MAXN; n++) {
        bin3[n][0] = bin3[n-1][0] * (2.0/3.0);
        for (int k = 1; k < n; k++)
            bin3[n][k] = (1.0/3.0) * bin3[n-1][k-1] + (2.0/3.0) * bin3[n-1][k];
        bin3[n][n] = bin3[n-1][n-1] * (1.0/3.0);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// p1d(k, d): peluang walk 1D sepanjang k langkah berakhir di displacement d
// ─────────────────────────────────────────────────────────────────────────────
// Menggunakan rumus: C(k, (k+d)/2) / 2^k = bin2[k][(k+d)/2]
//   Harus memenuhi: k >= d  DAN  (k - d) genap  (syarat paritas)
inline double p1d(int k, int d) {
    if (k < d || (k - d) % 2 != 0) return 0.0;
    return bin2[k][(k + d) / 2];
}

// ─────────────────────────────────────────────────────────────────────────────
// MEMOIZATION untuk efisiensi query berulang
// ─────────────────────────────────────────────────────────────────────────────
map<tuple<int,int,int,int>, double> memo;

double solve(int n, int dx, int dy, int dz) {
    // Ambil nilai absolut dan urutkan agar canonical (sumbu x/y/z simetris)
    int a[3] = {abs(dx), abs(dy), abs(dz)};
    sort(a, a + 3);
    dx = a[0]; dy = a[1]; dz = a[2];  // dx <= dy <= dz

    // ── KONDISI PROBABILITAS = 0 ──────────────────────────────────────────────
    // 1) Jarak minimum melebihi n langkah yang tersedia
    if (dx + dy + dz > n) return 0.0;
    // 2) Paritas: n - (dx+dy+dz) harus genap (sisa langkah harus berpasangan)
    if ((n - dx - dy - dz) % 2 != 0) return 0.0;

    // ── CEK CACHE ─────────────────────────────────────────────────────────────
    auto key = make_tuple(n, dx, dy, dz);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;

    // ── HITUNG PROBABILITAS ───────────────────────────────────────────────────
    // P = sum_{ax=dx..n-dy-dz, step 2}
    //       sum_{ay=dy..n-ax-dz, step 2}
    //         bin3[n][ax] * bin2[n-ax][ay] * p1d(ax,dx) * p1d(ay,dy) * p1d(az,dz)
    //
    // Penjelasan setiap faktor:
    //   bin3[n][ax]    : peluang tepat ax dari n langkah jatuh ke sumbu-x
    //   bin2[rem][ay]  : dari sisa (rem = n-ax) langkah, ay ke sumbu-y (az ke sumbu-z)
    //   p1d(ax,dx)     : walk 1D sumbu-x sejauh ax langkah mencapai dx
    //   p1d(ay,dy)     : walk 1D sumbu-y sejauh ay langkah mencapai dy
    //   p1d(az,dz)     : walk 1D sumbu-z sejauh az langkah mencapai dz
    double result = 0.0;

    for (int ax = dx; ax <= n - dy - dz; ax += 2) {
        // Faktor sumbu-x: tetap untuk seluruh iterasi ay
        double fx = bin3[n][ax] * p1d(ax, dx);
        int rem = n - ax;  // sisa langkah untuk y dan z

        for (int ay = dy; ay <= rem - dz; ay += 2) {
            int az = rem - ay;
            result += fx * bin2[rem][ay] * p1d(ay, dy) * p1d(az, dz);
        }
    }

    memo[key] = result;
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    precompute();

    int T;
    cin >> T;
    cout << fixed << setprecision(10);

    while (T--) {
        int n, x1, y1, z1, x2, y2, z2;
        cin >> n;
        cin >> x1 >> y1 >> z1;
        cin >> x2 >> y2 >> z2;

        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int dz = abs(z2 - z1);

        cout << solve(n, dx, dy, dz) << "\n";
    }

    return 0;
}
