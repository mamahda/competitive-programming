#include <cstdio>

using namespace std;
typedef long long ll;

int main(){
  int t;
  ll a[32], pow2 = 4ll, n;
  scanf("%d", &t);

  for (int i=2; i<31; i++) {
    a[i-1] = (ll)i * pow2 - 1ll;
    pow2 = pow2 * 2ll;
  }

  while (t--) {
    scanf("%lld", &n);

    for (int i = 1; i <= 31; i++) {
      if(n<=a[i]) {
        printf("%d\n", i);
        i = 99;
      }
    }
  }

  return 0;
}
