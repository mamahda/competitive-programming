#include <cstdio>
#include <algorithm>
using namespace std;

#define ll long long

int main(){
  ll t;
  scanf("%lld", &t);
  while(t--){
    ll n, s, m, total = 0, minwarna[200005];
    scanf("%lld %lld %lld", &n, &s ,&m);

    for(ll i = 0; i < 3; i++){
      for(ll j = 0; j < n; j++){
        ll temp;
        scanf("%lld", &temp);
        total += temp;
        if(i == 0) minwarna[j] = temp;
        else minwarna[j] = min(minwarna[j], temp);
      }
    }

    sort(minwarna, minwarna+n, greater<ll>());
    for(ll i = s-1; i < n; i++) total -= minwarna[i];
    total++;

    ll Sn = m * (m + 1) / 2;
    ll res = total / Sn * m;
    ll sisa = total % Sn;

    if (sisa > 0) {
      ll ambil = 0;
      for (ll k = m; k >= 1; k--) {
        ambil += k;
        res++;
        if (ambil >= sisa) break;
      }
    }

    printf("%lld\n", res);
  }
}
