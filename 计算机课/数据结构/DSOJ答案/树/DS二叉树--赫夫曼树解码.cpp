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

const int MAXN = 1e5 + 5;
namespace HuffmanTree {
    int n;  // 原序列长度
    int a[MAXN];  // 原序列,下标从1开始

    struct Node {
        int id;  // 节点编号
        int l, r;  // 左右儿子节点
        char data;  // 节点数据
        int w;  // 节点权值
        int fa = 0;  // 父亲节点

        Node() {}
        Node(int _id, int _w, int _l, int _r) :id(_id), w(_w), l(_l), r(_r) {}

        bool operator>(const Node& B)const {
            if (w != B.w) return w > B.w;
            else return id > B.id;
        }
    }Tree[MAXN];
    int idx = 0;  // 当前用到的节点的编号

    pque<Node, vector<Node>, greater<Node>> heap;

    Node newNode(int w, int l = 0, int r = 0) {  // 新建一个权值为w的节点,左右儿子节点分别为l、r
        Tree[idx] = { ++idx,w,l,r };
        return Tree[idx];
    }

    void init() {
        idx = 0;
        heap = pque<Node, vector<Node>, greater<Node>>();
    }

    void build() {
        for (int i = 1; i <= n; i++) heap.push(newNode(a[i]));
        
        while (heap.size() >= 2) {
            auto left = heap.top(); heap.pop();
            auto right = heap.top(); heap.pop();
            auto root = newNode(left.w + right.w, left.id, right.id);
            heap.push(root);
            Tree[left.id].fa = root.id, Tree[right.id].fa = root.id;
        }
    }

    void print() {  // 输出各节点的连接情况
        for (int i = 1; i <= idx; i++)
            cout << Tree[i].id << ' ' << Tree[i].w << ' ' << Tree[i].l << ' ' << Tree[i].r << endl;
    }

    void encode() {
        for (int i = 1; i <= n; i++) {
            int u = i;
            string res;
            while (u != idx) {
                if (Tree[Tree[u].fa].l == u) res.push_back('0');
                else res.push_back('1');
                u = Tree[Tree[u].fa].id;
            }
            reverse(all(res));
            cout << a[i] << '-' << res << endl;
        }
    }

    void decode(string s) {
        string res;
        int u = idx;  // 当前节点
        for (auto ch : s) {
            if (ch == '0') {
                if (Tree[u].l) u = Tree[u].l;
                else {
                    cout << "error" << endl;
                    return;
                }
            }
            else {
                if (Tree[u].r) u = Tree[u].r;
                else {
                    cout << "error" << endl;
                    return;
                }
            }

            if (1 <= Tree[u].id && Tree[u].id <= n) {  // 叶子节点
                res.push_back(Tree[u].data);
                u = idx;  // 回到根节点
            }
        }
        cout << (u == idx ? res : "error") << endl;
    }
}
using namespace HuffmanTree;

void solve() {
    init();
    
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    build();
    for (int i = 1; i <= n; i++) cin >> Tree[i].data;

    CaseT{
        string s; cin >> s;
        decode(s);
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
    CaseT  // 单测时注释掉该行
    solve();
    // ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
    cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
    return 0;
}