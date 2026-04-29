#include <cstdio>
#include <algorithm>
using namespace std;

int DP[50005];

int f(int n)
{
  if (n <= 50000)
    return DP[n];
  else if (n % 2 == 0 && n % 3 != 0)
    return 1 + min(f(n / 2), f(n - 1));
  else if (n % 3 == 0 && n % 2 != 0)
    return 1 + f(n / 3);
  else if (n % 3 == 0 && n % 2 == 0)
    return 1 + min(f(n / 3), min(f(n / 2), f(n - 1)));
  else if (n % 3 != 0 && n % 2 != 0)
    return 1 + f(n - 1);
}

int main()
{
  int T, n, cases = 1;

  DP[1] = 0;
  DP[2] = 1;

  for (int i = 3; i <= 50000; i++)
  {
    if (i % 2 == 0 && i % 3 != 0)
      DP[i] = 1 + min(DP[i / 2], DP[i - 1]);
    else if (i % 3 == 0 && i % 2 != 0)
      DP[i] = 1 + DP[i / 3];
    else if (i % 2 == 0 && i % 3 == 0)
      DP[i] = 1 + min(DP[i / 3], min(DP[i / 2], DP[i - 1]));
    else
      DP[i] = 1 + DP[i - 1];
  }

  scanf("%d", &T);
  while (T--)
  {
    scanf("%d", &n);
    printf("Case %d: %d\n", cases, f(n));
    cases++;
  }

  return 0;
}