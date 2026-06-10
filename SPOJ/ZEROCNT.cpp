/*
 * Algoritma: Digit DP pada representasi biner angka 1..N
 *
 * Masalah:
 *   Tulis semua angka 1..N dalam biner (tanpa leading zeros).
 *   Tiap angka punya "zero-group": run maksimal 0 berturutan, dinomori 1,2,3,... dari kiri.
 *   Group ke-1, K+1, 2K+1, ... diwarnai. Hitung total nol berwarna di semua angka 1..N.
 *
 * Pendekatan (Digit DP):
 *   O(N) terlalu lambat (N < 2^31). Kita bangun angka bit per bit dari MSB ke LSB.
 *   Setiap bit 0 yang diletakkan langsung berkontribusi +1 ke jawaban jika berada
 *   di zero-group yang diwarnai. Kontribusi dikali jumlah completion valid (w*p),
 *   karena satu prefix yang sama bisa dimiliki banyak an gka sekaligus.
 *
 * State DP -- dimemo hanya saat tidak tight:
 *   i : posisi bit saat ini, 0=MSB (bit30) s/d 31=selesai
 *   g : (indeks zero-group saat ini) % K, 0-indexed -- diwarnai jika g==0
 *   z : 1 jika sedang di dalam zero-run, 0 jika tidak
 *   s : 1 jika sudah menempatkan leading 1 (angka sudah dimulai), 0 jika belum
 *   t : tight -- 1 jika prefix saat ini masih persis == prefix N, 0 jika sudah bebas
 *
 * Transisi saat menempatkan digit d:
 *   !s && d==1         -> angka dimulai: s=1, z=0
 *   s  && d==0         -> masuk/lanjut zero-group: z=1, w=1 jika g==0 (diwarnai)
 *   s  && d==1 && z==1 -> tutup zero-group: g=(g+1)%K, z=0
 *
 * Fungsi f mengisi c (jumlah completion) dan r (total nol berwarna).
 * r += w*p: kontribusi bit ini (w) dikali jumlah angka yang berbagi prefix ini (p).
 *
 * Tight tidak di-memo karena bergantung pada N yang berubah tiap query.
 * Path tight hanya 31 node per query, jadi aman tanpa memo.
 *
 * K di-cap di 16 karena angka < 2^31 punya maksimal 15 zero-group,
 * sehingga g % K tidak pernah kembali ke 0 jika K > 15.
 *
 * Kompleksitas: O(31 * 16 * 2 * 2) = ~2000 state, O(log^2 N) per query.
 * 
 * Size sebelum laporan: 671 byte
 * Size setelah laporan: 2523 byte
 */

#include<cstdio>
#include<cstring>
int K;long long C[32][16][2][2],S[32][16][2][2];bool v[32][16][2][2];void f(int i,int g,int z,int s,int t,int n,long long&c,long long&r){if(i==31){c=1;r=0;return;}if(!t&&v[i][g][z][s]){c=C[i][g][z][s];r=S[i][g][z][s];return;}c=r=0;for(int d=0,l=t?n>>(30-i)&1:1;d<=l;d++){int ng=g,ni=z,ns=s;long long w=0;if(!s){if(d)ns=1,ni=0;}else if(!d){if(!z)ni=1;w=!g;}else if(z){ng=(g+1)%K;ni=0;}long long p,q;f(i+1,ng,ni,ns,t&d==l,n,p,q);c+=p;r+=q+w*p;}if(!t){v[i][g][z][s]=1;C[i][g][z][s]=c;S[i][g][z][s]=r;}}int main(){int n,k;while(scanf("%d%d",&n,&k)==2){K=k<16?k:16;memset(v,0,sizeof(v));long long c,a;f(0,0,0,0,1,n,c,a);printf("%lld\n",a);}}