#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define MOD 1000000007

ll modPow(ll a, ll b) {
    ll res = 1;
    a %= MOD;
    while (b > 0) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

ll modInverse(ll x) {
    return modPow(x, MOD - 2);
}

ll faktorial(ll n) {
    ll res = 1;
    for (ll i = 1; i <= n; i++) {
        res = (res * i) % MOD;
    }
    return res;
}

int main() {
    ll n;
    cin >> n;

    ll A = (faktorial(n) * faktorial(2 * n)) % MOD;
    ll B = (faktorial(n + 1) * faktorial(n)) % MOD;
    ll invB = modInverse(B);

    ll res = (A * invB) % MOD;

    cout << res << "\n";
}

