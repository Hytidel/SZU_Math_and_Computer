#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
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
const double eps = 1e-7;
const double pi = acos(-1.0);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;
// const int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
// ----------------------------------------------------------------
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll lcm(ll a, ll b) { return a * b / gcd(a, b); }
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

ll qpow(ll a, ll k, ll MOD) {
    ll res = 1;
    while (k) {
        if (k & 1) res = res * a % MOD;
        k >>= 1;
        a = a * a % MOD;
    }
    return res;
}

ll qpow(ll a, ll k) {  // 注意可能爆ll
    ll res = 1;
    while (k) {
        if (k & 1) res = res * a;
        k >>= 1;
        a *= a;
    }
    return res;
}
// ----------------------------------------------------------------

const int MAXN = 1005;
struct BinaryTree {
    int n;  // 节点数
    struct Node {
        char data;
        int l, r;  // 左右儿子节点的下标
    }Tree[MAXN];

    int root;  // 根节点
    int fa[MAXN];  // fa[u]表示节点u的父亲节点
    
    void build() {
        cin >> n;
        for (int i = 1; i <= n; i++) {
            char ch1, ch2; cin >> Tree[i].data >> ch1 >> ch2;
            if (ch1 != '-') {
                int u = ch1 - '0' + 1;
                Tree[i].l = u;
                fa[u] = i;
            }
            if (ch2 != '-') {
                int u = ch2 - '0' + 1;
                Tree[i].r = u;
                fa[u] = i;
            }
        }

        for (int i = 1; i <= n; i++) {
            if (!fa[i]) {
                root = i;
                break;
            }
        }
    }
}tr1, tr2;

bool check(int u, int v) {  // 判断以u、v为根节点的子树是否同构
    if (!u && !v) return true;  // 两子树都为空
    if ((!u && v) || (u && !v)) return false;  // 一棵子树为空,另一棵不为空
    if (tr1.Tree[u].data != tr2.Tree[v].data) return false;  // 两子树根节点的数据不同

    if (!tr1.Tree[u].l && !tr2.Tree[v].l)  // 两树的左子树为空
        return check(tr1.Tree[u].r, tr2.Tree[v].r);

    if (tr1.Tree[u].l && tr2.Tree[v].l && tr1.Tree[tr1.Tree[u].l].data == tr2.Tree[tr2.Tree[v].l].data)
        return check(tr1.Tree[u].l, tr2.Tree[v].l) && check(tr1.Tree[u].r, tr2.Tree[v].r);
    else return check(tr1.Tree[u].l, tr2.Tree[v].r) && check(tr1.Tree[u].r, tr2.Tree[v].l);
}

void solve() {
    tr1.build(), tr2.build();

    cout << (check(tr1.root, tr2.root) ? "Yes" : "No");
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