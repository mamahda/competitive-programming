#include <cstdio>

typedef long long LL;
#define MAXN 100002

int n, m;
int t[MAXN];

bool enough(LL time)
{
  LL cnt = 0;
  for (int i = 0; i < n; ++i)
  {
    LL here = time / t[i];
    if (here >= m || here + cnt >= m)
      return true;
    cnt += here;
  }
  return cnt >= m;
}

int main()
{
  int T;
  scanf("%d", &T);
  while (T--)
  {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; ++i)
      scanf("%d", &t[i]);
    LL low = 0, high = (LL)t[0] * m;
    while (high - low > 1)
    {
      LL mid = (low + high) / 2;
      (enough(mid) ? high : low) = mid;
    }
    printf("%lld\n", high);
  }
  return 0;
}