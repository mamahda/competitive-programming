#include <iostream>

long long power(long long n){
  if(n == 0) return 1;
  return 2 * power(n-1) % 1000000007;
}

int main(){
  long long n, ans;
  std::cin >> n;
  ans = power(n);
  std::cout << ans << std::endl;

  return 0;
}
