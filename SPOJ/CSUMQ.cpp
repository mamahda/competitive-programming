#include <cstdio>
#include <vector>

using namespace std;
typedef long long ll;

int main() {
  ll n, q, j, k;
  scanf("%lld", &n);
  vector<long long> prefix(n + 1, 0);

  for (ll i = 0; i < n; i++) {
    ll tmp;
    scanf("%lld", &tmp);
    prefix[i + 1] = prefix[i] + tmp;
  }

  scanf("%lld", &q);
  while (q--) {
    scanf("%lld %lld", &j, &k);
    printf("%lld\n", prefix[k + 1] - prefix[j]);
  }

  return 0;
}
