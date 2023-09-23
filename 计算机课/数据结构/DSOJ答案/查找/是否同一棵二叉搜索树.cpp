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

const int MAXN = 1e3 + 5;
struct BinarySearchTree {
    struct Node{
        int l = 0, r = 0;  // 左右儿子节点
        int key;  // 节点权值,作为BST中用于排序的关键字
        int cnt;  // 该节点代表的数的出现次数
        int siz;  // 该节点的子树大小
        int dfn;  // DFS序
    }BST[MAXN];
    int root;  // 根节点
    int idx = 0;  // 当前用到的节点的编号
    int tim = 0;  // 时间戳
    map<int, int> mp;  // 每个权值对应的时间戳

    BinarySearchTree() { build(); }

    int newNode(int x) {  // 新建一个key为x的叶子节点
        BST[++idx].key = x;
        BST[idx].cnt = BST[idx].siz = 1;
        return idx;
    }

    void pushUp(int u) {
        BST[u].siz = BST[BST[u].l].siz + BST[BST[u].r].siz + BST[u].cnt;
    }

    void build() {  // 建BST
        newNode(-INF), newNode(INF);  // 哨兵
        BST[root = 1].r = 2;  // key为INF的节点是key为-INF的节点的右儿子节点
        pushUp(root);
    }

    void inorderTraversal(int u, vi& res) {  // 求BST的中序遍历
        if (BST[u].l) inorderTraversal(BST[u].l, res);
        if (BST[u].key != INF && BST[u].key != -INF)  // 哨兵无需加入
            for (int i = 0; i < BST[u].cnt; i++) res.push_back(BST[u].key);
        if (BST[u].r) inorderTraversal(BST[u].r, res);
    }

    void print() {  // 输出BST中存储的非降序序列
        vi res;
        inorderTraversal(root, res);
        int n = res.size();
        for (int i = 0; i < n; i++) cout << res[i] << " \n"[i == n - 1];

        //for (int i = 1; i <= idx; i++)  // 输出此时各节点的连接情况
        //    cout << i << ' ' << BST[i].key << ' ' << BST[i].l << ' ' << BST[i].r << endl;
    }

    void insert(int& u, int x) {  // 在以u为根节点的子树中插入一个key为x的节点,注意传引用
        if (!u) u = newNode(x);  // 若不存在该节点,则新建
        else if (BST[u].key == x) BST[u].cnt++;  // 已存在key为x的节点,更新cnt
        else if (BST[u].key > x) insert(BST[u].l, x);  // 插入到节点u的左子树
        else insert(BST[u].r, x);  // 插入到节点u的右子树

        pushUp(u);
    }

    int getPrev(int u, int x) {  // 在以u为根节点的子树中找到<x的最大数所在的节点
        if (!u) return -INF;  // 不存在该节点
        else if (BST[u].key < x) return u;  // 找到该节点
        else if (BST[u].key >= x) return getPrev(BST[u].l, x);
        else return getPrev(BST[u].r, x);
    }

    void remove(int& u, int x) {  // 在以u为根节点的子树中删除一个key为x的节点,注意传引用
        if (!u) return;  // 不存在该节点,直接返回

        if (BST[u].key == x) {
            if (BST[u].cnt > 1) BST[u].cnt--;  // 无需删除,只需更新cnt值
            else if (!BST[u].l && !BST[u].r) u = 0;  // 叶子节点
            else if (BST[u].l && !BST[u].r) u = BST[u].l;  // 只有左子树
            else if (!BST[u].l && BST[u].r) u = BST[u].r;  // 只有右子树
            else {  // 既有左子树,也有右子树
                int pre = getPrev(u, x);  // 找到节点u的前驱pre
                int r = BST[u].r;  // 暂存节点u的右子树的根节点
                BST[u = pre].r = r;  // 用pre替换u
            }
        }
        else if (BST[u].key > x) remove(BST[u].l, x);  // 递归到左子树删除
        else remove(BST[u].r, x);  // 递归到右子树删除

        pushUp(u);
    }

    // 在以u为根节点的子树中查找key为x的节点,返回查找次数,不存在该节点时步数为INF
    int find(int u, int x, int step) {
        if (!u) return INF;  // 不存在key为x的节点
        else if (BST[u].key == x) return step;  // 找到key为x的节点
        else if (BST[u].key > x) return find(BST[u].l, x, step + 1);  // 递归到左子树找
        else return find(BST[u].r, x, step + 1);  // 递归到右子树找
    }

    void dfs(int u) {
        mp[BST[u].key] = BST[u].dfn = ++tim;
        if (BST[u].l) dfs(BST[u].l);
        if (BST[u].r) dfs(BST[u].r);
    }
    
    bool operator==(BinarySearchTree p) {
        tim = p.tim = 0;
        mp.clear();
        
        dfs(root), p.dfs(p.root);
        return mp == p.mp;
    }
};

void solve() {
    int n;
    while (cin >> n, n) {
        int m; cin >> m;
        BinarySearchTree bst;
        for (int i = 0; i < n; i++) {
            int x; cin >> x;
            bst.insert(bst.root, x);
        }

        while (m--) {
            BinarySearchTree tmpbst;
            for (int i = 0; i < n; i++) {
                int x; cin >> x;
                tmpbst.insert(bst.root, x);
            }
            cout << (bst == tmpbst ? "Yes" : "No") << endl;
        }
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