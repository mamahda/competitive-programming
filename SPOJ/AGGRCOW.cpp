#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool place(vector<long long> stalls, int cows, long long dist) {
  int count = 1;            
  long long lastPos = stalls[0];

  for (int i = 1; i < (int)stalls.size(); i++) {
    if (stalls[i] - lastPos >= dist) {
      count++;
      lastPos = stalls[i];
      if (count == cows)
        return true;
    }
  }
  return false;
}

int main() {
  int t;
  cin >> t;

  while (t--) {
    int N, C;
    cin >> N >> C;

    vector<long long> stalls(N);
    for (int i = 0; i < N; i++)
      cin >> stalls[i];

    sort(stalls.begin(), stalls.end());

    long long low = 0;
    long long high = stalls.back() - stalls.front();
    long long ans = 0;

    while (low <= high) {
      long long mid = (low + high) / 2;
      if (place(stalls, C, mid)) {
        ans = mid;     
        low = mid + 1;
      } else {
        high = mid - 1; 
      }
    }

    cout << ans << "\n";
  }

  return 0;
}

