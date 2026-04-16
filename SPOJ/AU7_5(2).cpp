#include <cstdio>
#define mod 5000011
using namespace std;
int dp[100001];

int main(){
    int c,n,k,ans,temp;
    scanf("%d", &c);
    while(c--){
        scanf("%d %d", &n, &k);
        dp[1] = 2;
        for(int i = 2; i <= n; i++){
            if(i-k-1 > 0) dp[i] = (dp[i-1] + dp[i-k-1]) % mod;
            else dp[i] = i+1;
        }
        printf("%d\n", dp[n]%mod);
    }
}
