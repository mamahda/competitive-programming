#include <cstdio>

using namespace std;
typedef long long ll;

int main() {
  int t;
  scanf("%d", &t);

  while (t--) {
    ll n;
    scanf("%lld", &n);

    double val = 1.0;

    for (int i = 1; i <= 8; i++) {
      val *= (double)(n + i);
    }

    double result = ((double)n + 9.0) / 9.0 - (40320.0 / val);

    printf("%.6lf\n", result);

  }

  return 0;
}
