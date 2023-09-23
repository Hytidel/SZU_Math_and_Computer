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
namespace BinaryTree {
    int n;
    char s[MAXN];
    int begin = 0;  // 数组用到的下标

    struct Node {
        char data;
        int l, r;  // 左右儿子节点的下标
    }Tree[MAXN];
    int idx = 0;  // 当前用到的节点的编号

    int fa[MAXN];  // fa[u]表示节点u的父亲节点
    int depth[MAXN];  // depth[u]表示节点u所在的深度

    void init() {
        begin = idx = 0;
        memset(fa, 0, so(fa));
        memset(depth, 0, so(depth));
    }

    int build() {  // 建树
        if (begin == n) return 0;

        if (s[begin] == '0') {
            begin++;
            return 0;
        }

        int cur = ++idx;
        Tree[cur].data = s[begin++];
        Tree[cur].l = build(), Tree[cur].r = build();
        return cur;
    }

    void print() {  // 输出各节点的连接情况
        for (int i = 1; i <= idx; i++)
            cout << i << ' ' << Tree[i].data << ' ' << Tree[i].l << ' ' << Tree[i].r << endl;
    }

    void dfs(int u, int pre) {  // 当前节点、前驱节点:预处理fa[]、depth[]
        fa[u] = pre, depth[u] = depth[pre] + 1;

        if (Tree[u].l) dfs(Tree[u].l, u);
        if (Tree[u].r) dfs(Tree[u].r, u);
    }

    string bfs() {
        string res;

        qi que;
        que.push(1);
        while (que.size()) {
            auto u = que.front(); que.pop();
            res.push_back(Tree[u].data);

            if (Tree[u].l) que.push(Tree[u].l);
            if (Tree[u].r) que.push(Tree[u].r);
        }

        return res;
    }

    int get_height() {  // 求二叉树的高度
        dfs(1, 1);  // 从根节点开始搜,根节点的前驱节点是它本身

        return *max_element(depth + 1, depth + idx + 1);
    }

    void PreorderTraversal(int u, string& res) {  // 先序遍历
        res.push_back(' '), res.push_back(Tree[u].data);
        if (Tree[u].l) PreorderTraversal(Tree[u].l, res);
        if (Tree[u].r) PreorderTraversal(Tree[u].r, res);
    }

    void InorderTraversal(int u, string& res) {  // 中序遍历
        if (Tree[u].l) InorderTraversal(Tree[u].l, res);
        res.push_back(' '), res.push_back(Tree[u].data);
        if (Tree[u].r) InorderTraversal(Tree[u].r, res);
    }

    void PostorderTraversal(int u, string& res) {  // 后序遍历
        if (Tree[u].l) PostorderTraversal(Tree[u].l, res);
        if (Tree[u].r) PostorderTraversal(Tree[u].r, res);
        res.push_back(' '), res.push_back(Tree[u].data);
    }
};
using namespace BinaryTree;

vi ans;

void DFS(int u, int pre) {
    fa[u] = pre;

    bool flag = true;
    if (Tree[u].l) DFS(Tree[u].l, u), flag = false;
    if (Tree[u].r) DFS(Tree[u].r, u), flag = false;

    if (flag) ans.push_back(u);
}

void solve() {
    init();
    ans.clear();

    cin >> s;
    n = strlen(s);

    build();

    cout << bfs() << endl;
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
    CaseT  // 单测时注释掉该行
    solve();
    // ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
    cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
    return 0;
}