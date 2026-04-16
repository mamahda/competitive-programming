#include <cstdio>
#include <cstring>

typedef long long ll;
const ll MOD = 1000000007LL;

int readBuf(char* buf, int maxlen) {
    int c, i = 0;
    while ((c = getchar_unlocked()) != EOF && (c == ' ' || c == '\n' || c == '\r' || c == '\t'));
    if (c == EOF) return 0;
    buf[i++] = (char)c;
    while (i < maxlen - 1 && (c = getchar_unlocked()) != EOF && c >= '0' && c <= '9')
        buf[i++] = (char)c;
    buf[i] = '\0';
    return i;
}

ll charToMod(const char* s, ll m) {
    ll v = 0;
    for (int i = 0; s[i] >= '0' && s[i] <= '9'; i++)
        v = (v * 10 + (s[i] - '0')) % m;
    return v;
}

bool isOne(const char* s) {
    return s[0] == '1' && s[1] == '\0';
}

bool isTwo(const char* s) {
    return s[0] == '2' && s[1] == '\0';
}

ll powmod(ll base, ll exp, ll m) {
    if (exp == 0) return 1LL;
    base %= m;
    if (exp % 2 == 0) {
        ll half = powmod(base, exp / 2, m);
        return (half * half) % m;
    }
    return (base * powmod(base, exp - 1, m)) % m;
}

bool isOdd(const char* s) {
    int len = (int)strlen(s);
    if (len == 0) return false;
    char last = s[len - 1];
    return last == '1' || last == '3' || last == '5' || last == '7' || last == '9';
}

int main() {
    char nbuf[2048];
    char kbuf[2048];

    int T;
    scanf("%d", &T);

    while (T--) {
        readBuf(nbuf, sizeof(nbuf));
        readBuf(kbuf, sizeof(kbuf));

        if (strcmp(nbuf, "1") == 0) {
            printf("%lld\n", charToMod(kbuf, MOD));
            continue;
        }

        if (isOne(kbuf)) {
            printf("0\n");
            continue;
        }

        if (isOdd(nbuf) && isTwo(kbuf)) {
            printf("0\n");
            continue;
        }

        ll km1     = charToMod(kbuf, MOD);
        km1 = (km1 - 1 + MOD) % MOD;

        ll exp_mod;
        if (km1 == 0) {
            printf("0\n");
            continue;
        }
        exp_mod = charToMod(nbuf, MOD - 1);

        ll part1 = powmod(km1, exp_mod, MOD);

        ll sign  = isOdd(nbuf) ? (MOD - 1) : 1LL;
        ll part2 = (sign * km1) % MOD;

        ll ans = (part1 + part2) % MOD;
        printf("%lld\n", ans);
    }

    return 0;
}
