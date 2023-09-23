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

const int MAXN = 1e5 + 5;
namespace Trie {
    int son[MAXN][2];  // 每个节点的左、右儿子
    int cnt[MAXN];  // 以每个界点结尾的单词数
    int idx;  // 当前用到的下标
    bool is_leaf[MAXN];  // 是否为叶子节点

    void init() {  // 初始化
        memset(son, 0, so(son));
        memset(cnt, 0, so(cnt));
        memset(is_leaf, false, so(is_leaf));
        idx = 0;
    }

    void insert(string str) {  // 插入一个字符串
        int u = 0;
        for (int i = 0; str[i]; i++) {
            if (!son[u][str[i] - '0']) son[u][str[i] - '0'] = ++idx;  // 新建节点
            u = son[u][str[i] - '0'];
        }
        cnt[u]++;  // 更新以u结尾的单词数
    }

    bool dfs(int u) {
        if (cnt[u] && !is_leaf[u]) return false;

        bool ok = true;
        if (son[u][0]) ok &= dfs(son[u][0]);
        if (son[u][1]) ok &= dfs(son[u][1]);
        return ok;
    }

    bool check() {  // 判断是否存在一个单词是另一个单词的前缀
        for (int i = 1; i <= idx; i++)  // 预处理出叶子节点
            if (!son[i][0] && !son[i][1]) is_leaf[i] = true;

        return dfs(0);  // 从根节点开始搜
    }
}
using namespace Trie;

int getWPL(pque<int, vi, greater<int>>& heap) {
    int res = 0;
    while (heap.size() >= 2) {
        int a = heap.top(); heap.pop();
        int b = heap.top(); heap.pop();
        heap.push(a + b);
        res += a + b;
    }
    return res;
}

void solve() {
    int n; cin >> n;
    pque<int, vi, greater<int>> heap;
    vi frequency(26);
    for (int i = 0; i < n; i++) {
        char ch; int x; cin >> ch >> x;
        frequency[ch - 'A'] = x;
        heap.push(x);
    }

    int WPL = getWPL(heap);

    CaseT{
        init();

        int sum = 0;
        for (int i = 0; i < n; i++) {
            char ch; string s; cin >> ch >> s;
            sum += s.length() * frequency[ch - 'A'];
            insert(s);
        }

        if (sum != WPL || !check()) cout << "No" << endl;
        else cout << "Yes" << endl;
    }
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