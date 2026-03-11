#include <cstdio>

using namespace std;
typedef long long ll;

int main(){
  ll tc;
  scanf("%lld", &tc);

  while(tc--){
    ll n; scanf("%lld", &n);
    ll k = 0;

    while(true) {
      ll mx = (k+1) * (1 << (k+1)) - 1;
      if (mx >= n){
        printf("%lld\n", k);
        break;
      }
      k++;
    }
  }

  return 0;
}
