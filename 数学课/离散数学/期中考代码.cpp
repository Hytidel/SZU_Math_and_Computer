#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdlib>
#include<iomanip>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<complex>
#include<array>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<list>
#include<unordered_set>
#include<unordered_map>
#include<bitset>
#include<valarray>
#include<sstream>
#include<functional>
#include<cassert>
#include<random>
#include<numeric>
using namespace std;
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<bool> vb;
typedef queue<int> qi;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef pair<double, double> pdd;
typedef tuple<int, int, int> tiii;
typedef vector<pii> vii;
typedef vector<pll> vll;
typedef queue<pii> qii;
typedef complex<double> cp;
#define umap unordered_map
#define uset unordered_set
#define pque priority_queue
#define IOS ios::sync_with_stdio(0); cin.tie(0), cout.tie(0);
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define npt nullptr
#define so sizeof
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define hashset_finetune(p) (p).reserve(1024); (p).max_load_factor(0.25);  // 哈希表防卡
const double eps = 1e-8;
const double pi = acos(-1.0);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;
const int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
// ----------------------------------------------------------------
// ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
// ll lcm(ll a, ll b) { return a * b / gcd(a, b); }
ll lowbit(ll x) { return x & (-x); }
int cmp(double a, double b) {
    if (fabs(a - b) < eps) return 0;
    else if (a > b) return 1;
    else return -1;
}
int sgn(double x) {
    if (fabs(x) < eps) return 0;
    else if (x > 0) return 1;
    else return -1;
}

template<typename T>
void read(T& x) {
    x = 0;
    T sgn = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') sgn = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = (x << 3) + (x << 1) + (ch & 15);
        ch = getchar();
    }
    x *= sgn;
}

template<typename T>
void write(T x) {
    if (x < 0) {
        putchar('-');
        x *= -1;
    }
    if (x < 10) putchar(x + 48);
    else write(x / 10), putchar(x % 10 + 48);
}

//ll qpow(ll a, ll k, ll MOD) {
//    ll res = 1;
//    while (k) {
//        if (k & 1) res = res * a % MOD;
//        k >>= 1;
//        a = a * a % MOD;
//    }
//    return res;
//}
//
//ll qpow(ll a, ll k) {  // 注意可能爆ll
//    ll res = 1;
//    while (k) {
//        if (k & 1) res = res * a;
//        k >>= 1;
//        a *= a;
//    }
//    return res;
//}
// ----------------------------------------------------------------

const int MAXN = 3e5 + 5;
const int MOD = 998244353;
namespace Math {
    int primes[MAXN], cnt;  // 素数及其个数
    bool state[MAXN];  // 记录每个数是否被筛掉
    int phi[MAXN];  // Euler函数

    void init() {  // 预处理出1e7以内的素数和Euler函数
        phi[1] = 1;
        for (int i = 2; i < MAXN; i++) {
            if (!state[i]) primes[cnt++] = i, phi[i] = i - 1;

            for (int j = 0; (ll)primes[j] * i < MAXN; j++) {
                state[primes[j] * i] = true;
                if (i % primes[j] == 0) {
                    phi[primes[j] * i] = phi[i] * primes[j];
                    break;
                }
                else phi[primes[j] * i] = phi[i] * (primes[j] - 1);
            }
        }
    }

    ll gcd(ll a, ll b) {  // 求两数的gcd
        if (b) {
            cout << "gcd(" << b << ", " << a << " mod " << b << ") = gcd(" << b << ", " << a % b << ") = ";
            return gcd(b, a % b);
        }
        else {
            cout << a;
            return a;
        }
    }

    ll lcm(ll a, ll b) {  // 求两数的lcm
        return a * b / gcd(a, b);
    }

    ll exgcd(ll a, ll b, ll& x, ll& y) {  // 扩展Euclid算法
        cout << a << "x + " << b << "y = " << std::gcd(a, b) << endl;
        if (!b) {
            x = 1, y = 0;
            return a;
        }

        ll d = exgcd(b, a % b, y, x);
        y -= a / b * x;
        return d;
    }

    ll inv(ll a, ll MOD) {  // 求a模MOD的逆元
        ll x, y;
        exgcd(a, MOD, x, y);
        x = (x % MOD + MOD) % MOD;  // 防止出现负数
        return x;
    }

    ll qpow(ll a, ll k, ll MOD) {  // 快速幂求a的k次方模MOD5
        ll res = 1;
        int cnt = 0;
        while (k) {
            cout << "bit" << cnt++ << " = " << (k & 1) << ", ";
            if (k & 1) {
                cout << "res = " << res << " * " << a << " mod " << MOD << " = " << res * a % MOD << ", ";
                res = res * a % MOD;
            }
            else cout << "res = " << res << ", ";
            k >>= 1;
            cout << "a = " << a << " * " << a << " mod " << MOD << " = " << a * a % MOD << endl;
            a = a * a % MOD;
        }
        return res;
    }

    void getFactors(ll n) {  // 将n素因数分解
        cout << "n = ";
        vii res;
        for (ll i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                int s = 0;
                while (n % i == 0) n /= i, s++;
                res.push_back({ i,s });
            }
        }
        if (n > 1) res.push_back({ n,1 });

        for (int i = 0; i < res.size(); i++) {
            cout << res[i].first << '^' << res[i].second;
            if (i != res.size() - 1) cout << " + ";
        }
    }

    void expressLinear(ll a, ll b) {
        while (true) {
            cout << a << " = " << a / b << " * " << b << " + " << a % b << endl;
            int r = a % b;
            if (!r) break;
            else a = b, b = r;
        }
    }

    ll exGcd(ll a, ll b, ll& x, ll& y) {  // 扩欧
        if (!b) {
            x = 1, y = 0;
            return a;
        }

        int d = exGcd(b, a % b, y, x);
        y -= a / b * x;
        return d;
    }

    void CRT() {
        int n; cin >> n;
        ll a1, m1; cin >> a1 >> m1;

        bool flag = true;  // 判断是否有解
        for (int i = 0; i < n - 1; i++) {  // 每次合并一个方程
            ll a2, m2; cin >> a2 >> m2;

            ll k1 = 0, k2 = 0;
            ll d = exgcd(a1, a2, k1, k2);

            if ((m2 - m1) % d) {  // 判断同余方程是否有解
                flag = false;
                break;
            }

            k1 *= (m2 - m1) / d;
            ll t = a2 / d;
            k1 = (k1 % t + t) % t;  // 得到非负余数

            m1 = a1 * k1 + m1;
            a1 = abs(a1 / d * a2);  // 存正的lcm
        }

        if (flag) cout << (m1 % a1 + a1) % a1;  // 输出非负余数
        else cout << -1 << endl;
    }
}
using namespace Math;

void solve() {
    init();
    cout << phi[4] << ' ' << phi[3] << ' ' << phi[7];

}

int main() {
    IOS;
#ifndef ONLINE_JUDGE
    clock_t my_clock = clock();
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    // ----------------------------------------------------------------	
    // init();
    // CaseT  // 单测时注释掉该行
    solve();
    // ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
    cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
    return 0;
}