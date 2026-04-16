#include <stdio.h>

typedef long long ll;
#define MOD 314159

ll modexp(ll b, ll e, ll m) {
  ll r = 1;
  while(e > 0){
    if((e & 1) == 1){
      r = (r * b) % m;
    }

    e >>= 1;
    b = (b * b) % m;
  }
   return (ll)r;
}

int main() {
    int tc;
    ll inv2 = modexp(2, MOD-2, MOD);
    scanf("%d", &tc);

    while (tc--) {
        ll n;
        scanf("%lld", &n);

        ll ans = ((modexp(8, n, MOD) + modexp(10, n, MOD)) * inv2) % MOD;

        printf("%lld\n", ans);
    }
    return 0;
}
