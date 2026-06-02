#include <cstdio>
using namespace std;
#define ll long long
#define MOD 1000000007

ll derangement(int n){
    if (n == 0) return 1;
    if (n == 1) return 0;
    ll dp[n + 1];
    dp[0] = 1;
    dp[1] = 0;
    for (int i = 2; i <= n; i++)
        dp[i] = ((i - 1) * (dp[i - 1] + dp[i - 2])) % MOD;
    return dp[n];
}

ll factorial(int n){
    ll res = 1;
    for (int i = 2; i <= n; i++)
        res = (res * i) % MOD;
    return res;
}

int main(){
    ll n;
    scanf("%lld", &n);
    ll d = derangement(n);
    ll f = factorial(n);
    printf("%lld\n", ((f - d - 1) % MOD + MOD) % MOD);
    return 0;
}

