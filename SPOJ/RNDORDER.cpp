#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
  int T;
  cin >> T;
  cout << fixed << setprecision(10);

  while (T--)
  {
    int n;
    cin >> n;
    if (n == 1)
    {
      cout << "1.0\n";
      continue;
    }
    double prob = 1.0;
    for (int i = 1; i < n; ++i)
    {
      prob *= (2.0 * i - 1.0) / (2.0 * i);
    }
    cout << prob << '\n';
  }

  return 0;
}