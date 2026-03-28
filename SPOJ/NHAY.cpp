#include <iostream>
#include <string.h>

using namespace std;

int main()
{
  int n;

  while (scanf("%d", &n) != EOF)
  {
    getchar();
    string pattern, haystack;

    cin >> pattern >> haystack;

    int len = haystack.size();
    int idx = 0;

    cout << haystack.find(pattern);
  }
}
