/*
 * Laser Mirror Grid Solver
 *
 * Ada grid X baris x Y kolom. Di setiap pinggirnya ada laser dengan label angka.
 * Tiap label muncul tepat 2 kali (masuk & keluar).
 * Tugas: isi setiap sel dengan cermin '/' atau '\' supaya laser label L
 *        yang masuk dari satu sisi keluar di sisi lain yang berlabel L juga.
 *
 * Pendekatan:
 *   1. Susun semua port (laser) searah jarum jam.
 *   2. Cek apakah pasangan-pasangan label bisa dicocokkan tanpa saling silang
 *      (seperti kurung buka-tutup yang valid).
 *   3. Trace laser dari tiap endpoint "terakhir" (urutan clockwise lebih besar),
 *      isi sel kosong dengan cermin belok-kanan.
 *   4. Isi sisa sel kosong dengan '/'.
 *   5. Verifikasi semua jalur benar.
 */

#include <cstdio>
#include <cstring>

#define MAXN 105     // batas X atau Y
#define MAXLABEL 205 // batas jumlah label berbeda (X + Y)
#define MAXPERIM 405 // batas total port di perimeter (2*X + 2*Y)

// ---- Representasi satu port (titik masuk/keluar laser di pinggir grid) ----
struct Port
{
    int label; // nomor label laser
    int r, c;  // posisi "di luar" grid (bisa negatif atau >= X/Y)
    int dir;   // arah gerak saat masuk ke dalam grid
    int idx;   // urutan clockwise (0 = pertama)
};

int X, Y; // dimensi grid

// Label laser di setiap sisi
int topLabel[MAXN], bottomLabel[MAXN];
int leftLabel[MAXN], rightLabel[MAXN];

// Grid isi cermin
char grid[MAXN][MAXN];

// Daftar semua port, diurutkan clockwise
Port perimeter[MAXPERIM];
int perimSize;

// Untuk tiap label, simpan ke-2 port-nya
Port endpoints[MAXLABEL][2];
int endpointCount[MAXLABEL];

// Delta baris dan kolom untuk tiap arah: 0=atas, 1=kanan, 2=bawah, 3=kiri
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// ---- Hitung arah baru setelah laser menabrak cermin ----
// Cermin '/' memantulkan: atas<->kanan, bawah<->kiri
// Cermin '\' memantulkan: atas<->kiri,  bawah<->kanan
int turnDir(int dir, char mirror)
{
    if (mirror == '/')
    {
        if (dir == 0)
            return 1; // atas  -> kanan
        if (dir == 1)
            return 0; // kanan -> atas
        if (dir == 2)
            return 3; // bawah -> kiri
        return 2;     // kiri  -> bawah
    }
    else
    { // '\'
        if (dir == 0)
            return 3; // atas  -> kiri
        if (dir == 3)
            return 0; // kiri  -> atas
        if (dir == 1)
            return 2; // kanan -> bawah
        return 1;     // bawah -> kanan
    }
}

// ---- Pilih cermin yang membuat laser belok ke kanan ----
// Laser bergerak lurus -> kita paksa belok 90 derajat ke kanan.
// Ternyata: gerak vertikal (atas/bawah) -> pakai '/'
//           gerak horizontal (kiri/kanan) -> pakai '\'
char mirrorForRightTurn(int dir)
{
    if (dir == 0 || dir == 2)
        return '/';
    return '\\';
}

// ---- Ambil label di posisi (r, c) yang sudah keluar dari grid ----
int getExitLabel(int r, int c)
{
    if (r < 0)
        return topLabel[c];
    if (r >= X)
        return bottomLabel[c];
    if (c < 0)
        return leftLabel[r];
    return rightLabel[r];
}

// ---- Tambah satu port ke daftar perimeter dan endpoints ----
void addPort(int label, int r, int c, int dir)
{
    Port p;
    p.label = label;
    p.r = r;
    p.c = c;
    p.dir = dir;
    p.idx = perimSize;
    perimeter[perimSize++] = p;

    int cnt = endpointCount[label];
    endpoints[label][cnt] = p;
    endpointCount[label]++;
}

/*
 * ---- Cek non-crossing matching dengan stack ----
 *
 * Bayangkan port-port di perimeter seperti kurung buka-tutup.
 * Label yang pertama muncul = "kurung buka", kemunculan kedua = "kurung tutup".
 * Agar tidak ada jalur laser yang bersilangan, setiap "tutup" harus menutup
 * "buka" yang paling terakhir dibuka (seperti kurung yang valid).
 *
 * Contoh valid   : 1 2 2 1  (seperti "(())")
 * Contoh invalid : 1 2 1 2  (seperti "()()" -> jalur 1 dan 2 akan bersilang)
 */
int checkNonCrossing()
{
    int stack[MAXPERIM];
    int stackTop = 0;
    int seen[MAXLABEL];
    memset(seen, 0, sizeof(seen));

    for (int i = 0; i < perimSize; i++)
    {
        int v = perimeter[i].label;
        if (!seen[v])
        {
            // Kemunculan pertama: push ke stack
            seen[v] = 1;
            stack[stackTop++] = v;
        }
        else
        {
            // Kemunculan kedua: harus menutup yang di atas stack
            if (stackTop == 0 || stack[stackTop - 1] != v)
            {
                return 0; // ada persilangan
            }
            stackTop--;
        }
    }
    return (stackTop == 0); // semua pasangan berhasil ditutup
}

/*
 * ---- Trace laser dari port 'start', isi cermin yang belum terisi ----
 *
 * Laser masuk ke grid, bergerak lurus sampai kena cermin.
 * Sel kosong ('.') diisi dengan cermin belok-kanan.
 * Sel yang sudah ada cerminnya dibiarkan.
 * Return 1 jika laser keluar di label yang sama, 0 jika tidak.
 */
int traceFrom(Port start)
{
    int label = start.label;
    int r = start.r + dr[start.dir];
    int c = start.c + dc[start.dir];
    int dir = start.dir;

    int steps = 0;
    int maxSteps = 4 * X * Y + 10; // batas aman agar tidak infinite loop

    while (r >= 0 && r < X && c >= 0 && c < Y)
    {
        if (++steps > maxSteps)
            return 0;

        if (grid[r][c] == '.')
        {
            grid[r][c] = mirrorForRightTurn(dir);
        }
        dir = turnDir(dir, grid[r][c]);
        r += dr[dir];
        c += dc[dir];
    }

    return (getExitLabel(r, c) == label);
}

/*
 * ---- Verifikasi akhir: semua laser harus mencapai label yang benar ----
 */
int verifyFinal()
{
    for (int i = 0; i < perimSize; i++)
    {
        int r = perimeter[i].r + dr[perimeter[i].dir];
        int c = perimeter[i].c + dc[perimeter[i].dir];
        int dir = perimeter[i].dir;

        int steps = 0;
        int maxSteps = 4 * X * Y + 10;

        while (r >= 0 && r < X && c >= 0 && c < Y)
        {
            if (++steps > maxSteps)
                return 0;
            dir = turnDir(dir, grid[r][c]);
            r += dr[dir];
            c += dc[dir];
        }

        if (getExitLabel(r, c) != perimeter[i].label)
            return 0;
    }
    return 1;
}

/*
 * ---- Insertion sort untuk mengurutkan endpoint berdasarkan idx clockwise ----
 *
 * Kita perlu memproses endpoint ke-2 (yang lebih "belakang" dalam urutan clockwise)
 * dari urutan terkecil ke terbesar, supaya trace tidak menimpa cermin yang sudah ada.
 */
struct OrderItem
{
    int idx;   // idx clockwise dari endpoint ke-2
    int label; // label laser
};

void insertionSort(OrderItem arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        OrderItem key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].idx > key.idx)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ========== MAIN ==========
int main()
{
    scanf("%d %d", &X, &Y);

    memset(endpointCount, 0, sizeof(endpointCount));
    perimSize = 0;

    // Baca input label di setiap sisi
    for (int j = 0; j < Y; j++)
        scanf("%d", &topLabel[j]);
    for (int i = 0; i < X; i++)
        scanf("%d %d", &leftLabel[i], &rightLabel[i]);
    for (int j = 0; j < Y; j++)
        scanf("%d", &bottomLabel[j]);

    /*
     * Tambah semua port searah jarum jam:
     *   Top    : kiri -> kanan  (laser menembak ke bawah,  dir=2)
     *   Right  : atas -> bawah  (laser menembak ke kiri,   dir=3)
     *   Bottom : kanan -> kiri  (laser menembak ke atas,   dir=0)
     *   Left   : bawah -> atas  (laser menembak ke kanan,  dir=1)
     */
    for (int j = 0; j < Y; j++)
        addPort(topLabel[j], -1, j, 2);
    for (int i = 0; i < X; i++)
        addPort(rightLabel[i], i, Y, 3);
    for (int j = Y - 1; j >= 0; j--)
        addPort(bottomLabel[j], X, j, 0);
    for (int i = X - 1; i >= 0; i--)
        addPort(leftLabel[i], i, -1, 1);

    // Validasi: setiap label harus muncul tepat 2 kali
    int possible = 1;
    for (int v = 1; v <= X + Y; v++)
    {
        if (endpointCount[v] != 2)
        {
            possible = 0;
            break;
        }
    }

    // Validasi non-crossing
    if (possible && !checkNonCrossing())
    {
        possible = 0;
    }

    if (!possible)
    {
        printf("-1\n");
        return 0;
    }

    // Inisialisasi grid kosong
    for (int i = 0; i < X; i++)
    {
        for (int j = 0; j < Y; j++)
            grid[i][j] = '.';
        grid[i][Y] = '\0';
    }

    /*
     * Susun daftar endpoint ke-2 tiap label, urutkan berdasarkan idx clockwise.
     * Proses dari yang terkecil agar cermin yang sudah diisi tidak ditimpa.
     */
    OrderItem order[MAXLABEL];
    int orderSize = 0;

    for (int v = 1; v <= X + Y; v++)
    {
        order[orderSize].idx = endpoints[v][1].idx;
        order[orderSize].label = v;
        orderSize++;
    }

    insertionSort(order, orderSize);

    for (int i = 0; i < orderSize && possible; i++)
    {
        int v = order[i].label;
        if (!traceFrom(endpoints[v][1]))
        {
            possible = 0;
        }
    }

    if (!possible)
    {
        printf("-1\n");
        return 0;
    }

    // Isi sel yang masih kosong dengan '/'
    for (int i = 0; i < X; i++)
        for (int j = 0; j < Y; j++)
            if (grid[i][j] == '.')
                grid[i][j] = '/';

    // Verifikasi final
    if (!verifyFinal())
    {
        printf("-1\n");
        return 0;
    }

    // Output grid
    for (int i = 0; i < X; i++)
        printf("%s\n", grid[i]);

    return 0;
}
