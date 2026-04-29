#include <cmath>
#include <cstdio>
#include <algorithm>

using namespace std;

#define PI acos(-1.0)

int main()
{
  int cases, cid = 1;
  double M;
  scanf("%d", &cases);
  while (cases--)
  {
    scanf("%lf", &M);
    printf("Case %d: %.0lf\n", cid++, floor(((6.0 * pow(M - 3, 2.0)) - 7.0 - (9.0 * pow(-1.0, M - 3)) + (16.0 * cos(2.0 / 3.0 * PI * (M - 3)))) / 72.0));
  }
}