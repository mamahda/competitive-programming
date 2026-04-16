#include<cstdio>
using namespace std;
#define MOD 1000000007
typedef long long ll;
ll n,k,dp[1000001];

int main(){
  scanf("%lld %lld", &n, &k);
  dp[0] = 1;
  for(int a = 1; a <= n; a++){
    if(a-k-1 < 0){
      if(a == k){
        dp[a] = (2 * dp[a-1] % MOD) - 1;
      }
      else {
        dp[a] = (2 * dp[a-1] % MOD);
      }
    }
    else {
      dp[a] = (2 * dp[a-1] % MOD) - dp[a-k-1];
    }
    if(dp[a] < 0) dp[a] = dp[a] + MOD;
  }
  printf("%lld\n", dp[n]);
  return 0;
}
