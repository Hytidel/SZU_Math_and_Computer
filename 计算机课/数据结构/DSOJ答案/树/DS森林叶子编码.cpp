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

const int MAXN = 1e4 + 5;
namespace Forest {
    int n, m;  // 树的个数、每棵树的最多分叉数
    int tr[MAXN][105];  // 多叉树
    char ch[MAXN];  // 节点的数据
    vi roots;  // 每棵树的根节点
    int idx;  // 当前用到的节点的编号
    string s;
    int len;  // s的长度
    int begin;  // 当前读到的s的下标
    int fa[MAXN];
    struct Node {
        char data;
        int l, r;  // 左右儿子
        int fa;  // 父亲节点
    }Tree[MAXN];

    int build() {
        while (begin < len && s[begin] == ' ') begin++;
        if (begin >= len) return 0;

        if (s[begin] == '0') {
            begin++;
            while (begin < len && s[begin] == ' ') begin++;
            return 0;
        }

        int cur = ++idx;
        ch[idx] = s[begin++];
        while (begin < len && s[begin] == ' ') begin++;
        for (int i = 0; i < m; i++) tr[cur][i] = build();
        return cur;
    }

    void init() {  // 预处理fa[]
        for (int i = 1; i <= idx; i++) {
            for (int j = 0; j < m; j++)
                if (tr[i][j]) fa[tr[i][j]] = i;
        } 
    }

    void print() {  // 输出各节点的连接情况
        for (int i = 1; i <= idx; i++) {
            cout << i << ' ' << ch[i] << ' ';
            for (int j = 0; j < m; j++) cout << tr[i][j] << ' ';
            cout << endl;
        }

        cout << endl;
        for (int i = 1; i <= idx; i++) {
            cout << i << ' ' << ch[i] << ' ' << Tree[i].l << ' ' << Tree[i].r << endl;
        }
    }

    void dfs(int u) {  // 当前节点、前驱节点
        for (int i = 0; i < m; i++) {
            if (tr[u][i]) {
                Tree[u].l = tr[u][i];
                dfs(Tree[u].l);
                break;
            }
        }

        bool ok = false;
        for (int i = 0; i < m; i++) {
            if (tr[fa[u]][i] == u) {
                ok = true;
                continue;
            }
            if (!ok || !tr[fa[u]][i]) continue;

            Tree[u].r = tr[fa[u]][i];
            dfs(Tree[u].r);
            break;
        }
    }

    void tran() {  // 转换为二叉树
        init();
        for (int i = 0; i < roots.size(); i++) {
            dfs(roots[i]);
            if (i) {
                if (Tree[roots[i - 1]].l) Tree[roots[i - 1]].r = roots[i];
                else Tree[roots[i - 1]].l = roots[i];
            }
        }
    }

    vi encode(int u) {
        vi res;
        while (u > 1) {
            if (Tree[Tree[u].fa].l == u) res.push_back(0);
            else res.push_back(1);
            u = Tree[u].fa;
        }
        reverse(all(res));
        return res;
    }

    void encode() {
        vi leaves;
        Tree[1].fa = 1;
        for (int i = 1; i <= idx; i++) {  // 预处理叶子节点
            if (!Tree[i].l && !Tree[i].r) leaves.push_back(i);
            if (Tree[i].l) Tree[Tree[i].l].fa = i;
            if (Tree[i].r) Tree[Tree[i].r].fa = i;
        }

        for (auto u : leaves) {
            auto res = encode(u);
            for (int i = 0; i < res.size(); i++) cout << res[i] << " \n"[i == res.size() - 1];
        }
    }
}
using namespace Forest;

void solve() {
    cin >> n >> m; cin.ignore(1, ' ');
    for (int i = 0; i < n; i++) {
        getline(cin, s);
        len = s.length(), Forest::begin = 0;
        roots.push_back(idx + 1);
        build();
    }
  
    tran();
    encode();
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