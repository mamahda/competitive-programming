/*
 * Slash/Backslash Grid Solver
 *
 * Pendekatan: CSP (Constraint Satisfaction Problem)
 *   1. Baca perimeter grid → bangun "region tree" (pohon wilayah bersarang)
 *   2. Setiap vertex (titik sudut) punya "domain" = himpunan region yang mungkin
 *   3. Jalankan AC3 (arc consistency) untuk mempersempit domain
 *   4. Kalau belum selesai, pakai backtracking dengan MRV heuristic
 */

#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

// ---- Batas ukuran ----
const int MAXXY  = 101;
const int MAXN   = MAXXY * MAXXY;   // jumlah maksimum vertex
const int MAXK   = 2 * MAXXY * 2 + 4; // jumlah maksimum region

int X, Y;      // dimensi grid (X baris, Y kolom)
int N;         // jumlah vertex = (X+1)*(Y+1)
int K;         // jumlah region yang terbentuk

// ---- Data region tree ----
int parent_region[MAXK];   // parent[k] = region induk dari region k
int depth_region[MAXK];    // kedalaman region k di pohon

// adj_region[k] = daftar region yang bertetangga langsung dengan region k
// (yaitu parent dan anak-anaknya di pohon)
vector<int> adj_region[MAXK];

// ---- Domain setiap vertex ----
// domain[u] = himpunan region yang masih mungkin untuk vertex u
vector<bool> domain[MAXN];
int domain_size[MAXN];     // cacah elemen true di domain[u]

// ---- Queue untuk AC3 (pakai visited array supaya tidak duplikat) ----
queue<int> bfs_queue;
bool in_queue[MAXN];

void queue_push(int u) {
    if (!in_queue[u]) {
        in_queue[u] = true;
        bfs_queue.push(u);
    }
}
int queue_pop() {
    int u = bfs_queue.front();
    bfs_queue.pop();
    in_queue[u] = false;
    return u;
}
bool queue_empty() { return bfs_queue.empty(); }
void queue_clear() {
    while (!bfs_queue.empty()) bfs_queue.pop();
    memset(in_queue, 0, sizeof(in_queue));
}

// ---- Trail (untuk backtracking / undo) ----
// Setiap kali kita hapus region dari domain[v], kita catat di trail
// supaya bisa di-undo kalau ternyata cabang ini salah.
struct TrailEntry {
    int vertex;  // vertex yang domain-nya diubah
    int region;  // region yang dihapus dari domain-nya
};
vector<TrailEntry> trail;

// ---- Tetangga vertex ----
// Vertex u = r*(Y+1) + c
// Arah: 0=atas, 1=bawah, 2=kiri, 3=kanan
int get_neighbor(int u, int dir) {
    int r = u / (Y + 1);
    int c = u % (Y + 1);
    if (dir == 0) r--;
    else if (dir == 1) r++;
    else if (dir == 2) c--;
    else               c++;
    if (r < 0 || r > X || c < 0 || c > Y) return -1;
    return r * (Y + 1) + c;
}

// ---- Hapus region 'reg' dari domain[v], catat di trail ----
void remove_from_domain(int v, int reg) {
    if (domain[v][reg]) {
        domain[v][reg] = false;
        domain_size[v]--;
        trail.push_back({v, reg});
    }
}

// ---- AC3: propagasi arc consistency ----
// Untuk setiap edge (u, v):
//   Setiap region y di domain[v] harus punya setidaknya satu region x
//   di domain[u] yang bertetangga dengan y.
// Kalau tidak ada, y dihapus dari domain[v].
// Return false kalau ada domain yang kosong (kontradiksi).
bool ac3() {
    while (!queue_empty()) {
        int u = queue_pop();
        for (int dir = 0; dir < 4; dir++) {
            int v = get_neighbor(u, dir);
            if (v == -1) continue;

            bool changed = false;
            for (int y = 0; y < K; y++) {
                if (!domain[v][y]) continue;

                // Cek apakah ada x di domain[u] yang adjacent dengan y
                bool has_support = false;
                for (int x : adj_region[y]) {
                    if (domain[u][x]) {
                        has_support = true;
                        break;
                    }
                }
                if (!has_support) {
                    remove_from_domain(v, y);
                    changed = true;
                    if (domain_size[v] == 0) return false; // kontradiksi
                }
            }
            if (changed) queue_push(v);
        }
    }
    return true;
}

// ---- Backtracking dengan MRV (Minimum Remaining Values) ----
// Pilih vertex dengan domain terkecil (tapi > 1) untuk di-assign duluan.
// Coba setiap nilai di domain-nya, jalankan AC3, rekursi.
// Undo semua perubahan kalau gagal.
bool solve() {
    // Cari vertex dengan domain terkecil yang belum fixed
    int best = -1, best_size = K + 1;
    for (int i = 0; i < N; i++) {
        if (domain_size[i] > 1 && domain_size[i] < best_size) {
            best_size = domain_size[i];
            best = i;
        }
    }
    if (best == -1) return true; // semua vertex sudah fixed → selesai!

    int u = best;

    // Kumpulkan semua kandidat region untuk vertex u
    vector<int> candidates;
    for (int k = 0; k < K; k++) {
        if (domain[u][k]) candidates.push_back(k);
    }

    // Coba satu per satu
    for (int val : candidates) {
        int saved = (int)trail.size();

        // Paksakan domain[u] = {val} saja
        for (int k : candidates) {
            if (k != val && domain[u][k]) {
                remove_from_domain(u, k);
            }
        }
        queue_push(u);

        if (ac3() && solve()) return true;

        // Undo: kembalikan semua perubahan sejak saved
        while ((int)trail.size() > saved) {
            TrailEntry e = trail.back();
            trail.pop_back();
            if (!domain[e.vertex][e.region]) {
                domain[e.vertex][e.region] = true;
                domain_size[e.vertex]++;
            }
        }
        queue_clear();
    }
    return false; // semua kandidat gagal → kontradiksi
}

// ========== MAIN ==========
int main() {
    scanf("%d %d", &X, &Y);
    N = (X + 1) * (Y + 1);

    // Input: clue di setiap sisi grid
    vector<int> Top(Y), Left(X), Right(X), Bottom(Y);
    for (int j = 0; j < Y; j++) scanf("%d", &Top[j]);
    for (int i = 0; i < X; i++) scanf("%d %d", &Left[i], &Right[i]);
    for (int j = 0; j < Y; j++) scanf("%d", &Bottom[j]);

    // =========================================================
    // FASE 1: Perimeter scan → bangun region tree
    //
    // Kita jalan mengelilingi perimeter grid searah jarum jam.
    // Angka di setiap sisi adalah "label wilayah".
    // Kalau label sama dengan yang di stack paling atas → tutup wilayah (pop).
    // Kalau tidak → buka wilayah baru (push), buat region baru.
    // Ini menghasilkan struktur pohon bersarang seperti kurung buka-tutup.
    // =========================================================

    // Susun urutan perimeter: atas → kanan → bawah (terbalik) → kiri (terbalik)
    int seq_len = 2 * X + 2 * Y;
    vector<int> seq(seq_len), vertex_r(seq_len), vertex_c(seq_len);

    int idx = 0;
    for (int j = 0; j < Y; j++) { seq[idx]=Top[j];    vertex_r[idx]=0;   vertex_c[idx]=j+1; idx++; }
    for (int i = 0; i < X; i++) { seq[idx]=Right[i];  vertex_r[idx]=i+1; vertex_c[idx]=Y;   idx++; }
    for (int j = Y-1; j >= 0; j--) { seq[idx]=Bottom[j]; vertex_r[idx]=X;   vertex_c[idx]=j;   idx++; }
    for (int i = X-1; i >= 0; i--) { seq[idx]=Left[i];  vertex_r[idx]=i;   vertex_c[idx]=0;   idx++; }

    // Stack untuk parsing nested regions
    vector<int> stk_id, stk_state; // label dan nomor region di stack

    // region_of[r][c] = nomor region untuk vertex (r, c)
    // -1 berarti belum ditentukan
    vector<vector<int>> region_of(X+1, vector<int>(Y+1, -1));

    K = 1; // region 0 = "luar" (root)
    memset(parent_region, -1, sizeof(parent_region));
    memset(depth_region, 0, sizeof(depth_region));
    parent_region[0] = -1;
    depth_region[0]  = 0;

    region_of[0][0] = 0;
    int cur_state = 0; // region aktif saat ini
    bool valid = true;

    for (int si = 0; si < seq_len && valid; si++) {
        int label = seq[si];

        if (!stk_id.empty() && stk_id.back() == label) {
            // Tutup wilayah: kembali ke parent
            stk_id.pop_back();
            stk_state.pop_back();
            cur_state = parent_region[cur_state];
        } else {
            // Buka wilayah baru
            int new_region = K++;
            parent_region[new_region] = cur_state;
            depth_region[new_region]  = depth_region[cur_state] + 1;
            stk_id.push_back(label);
            stk_state.push_back(new_region);
            cur_state = new_region;
        }

        // Assign region ke vertex ini
        int r = vertex_r[si], c = vertex_c[si];
        if (region_of[r][c] != -1) {
            if (region_of[r][c] != cur_state) valid = false;
        } else {
            region_of[r][c] = cur_state;
        }
    }

    if (!valid || !stk_id.empty()) {
        printf("-1\n");
        return 0;
    }

    // Bangun adjacency list untuk region tree
    // (setiap region bertetangga dengan parent dan anak-anaknya)
    for (int k = 1; k < K; k++) {
        int p = parent_region[k];
        adj_region[k].push_back(p);
        adj_region[p].push_back(k);
    }

    // =========================================================
    // FASE 2: Inisialisasi domain setiap vertex
    //
    // Aturan: di sepanjang setiap garis diagonal (/ atau \),
    // region berganti-ganti antara parent dan child.
    // Akibatnya, vertex di posisi (r+c) genap hanya bisa
    // memegang region dengan kedalaman genap, dan sebaliknya.
    // =========================================================
    for (int i = 0; i <= X; i++) {
        for (int j = 0; j <= Y; j++) {
            int u = i * (Y + 1) + j;
            domain[u].assign(K, false);
            domain_size[u] = 0;

            if (region_of[i][j] != -1) {
                // Vertex sudah diketahui region-nya dari perimeter scan
                domain[u][region_of[i][j]] = true;
                domain_size[u] = 1;
            } else {
                // Vertex interior: bisa jadi region manapun dengan paritas kedalaman yang sesuai
                int parity = (i + j) & 1; // 0 = genap, 1 = ganjil
                for (int k = 0; k < K; k++) {
                    if ((depth_region[k] & 1) == parity) {
                        domain[u][k] = true;
                        domain_size[u]++;
                    }
                }
                if (domain_size[u] == 0) {
                    printf("-1\n");
                    return 0;
                }
            }
        }
    }

    // =========================================================
    // FASE 3: AC3 awal (propagasi constraint ke seluruh grid)
    // =========================================================
    queue_clear();
    for (int u = 0; u < N; u++) queue_push(u);
    trail.clear();

    if (!ac3()) {
        printf("-1\n");
        return 0;
    }

    // =========================================================
    // FASE 4: Backtracking (untuk kasus yang belum selesai)
    // =========================================================
    if (!solve()) {
        printf("-1\n");
        return 0;
    }

    // =========================================================
    // OUTPUT
    //
    // Setiap sel (i, j) ditentukan oleh region dari:
    //   - sudut kiri-atas (tl) = vertex (i, j)
    //   - sudut kanan-bawah (br) = vertex (i+1, j+1)
    //
    // Kalau region tl == region br → diagonal '\' (batas melewati)
    // Kalau region tl != region br → diagonal '/' (batas memotong)
    // =========================================================
    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            int tl = i * (Y + 1) + j;
            int br = (i + 1) * (Y + 1) + (j + 1);

            // Ambil satu-satunya region yang tersisa di domain masing-masing
            int reg_tl = -1, reg_br = -1;
            for (int k = 0; k < K; k++) {
                if (domain[tl][k]) { reg_tl = k; break; }
            }
            for (int k = 0; k < K; k++) {
                if (domain[br][k]) { reg_br = k; break; }
            }

            printf("%c", reg_tl == reg_br ? '\\' : '/');
        }
        printf("\n");
    }

    return 0;
}
