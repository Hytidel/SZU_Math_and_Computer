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

const int MAXN = 5005;
map<string, int> mp;
map<int, string> pm;
vii edges[MAXN];
struct Edge {
    int u, v, w;
};
vector<Edge> path;
int idx;
namespace Prim {
    int n;  // 节点数
    int d[MAXN][MAXN];  // 邻接矩阵
    int dis[MAXN];  // dis[u]表示当前节点u到连通块的最小距离
    bool state[MAXN];  // 记录当前每个节点是否在连通块中

    ll prim(int s) {  // 求MST,返回MST的边权和
        memset(dis, INF, so(dis));
        dis[s] = 0;

        ll res = 0;  // MST的边权和
        int last = s;
        for (int i = 0; i < n; i++) {  // 做n次迭代
            int t = -1;  // 不在连通块中的与连通块距离最近的节点
            for (int j = 1; j <= n; j++)
                if (!state[j] && (t == -1 || dis[t] > dis[j])) t = j;

            if (i && dis[t] == INF) return INFF;  // 图不连通,不存在MST

            if (i) {  // 第一个节点无边,注意先更新res再更新dis[]
                res += dis[t];
                for (auto [u, w] : edges[t]) {
                    if (w == dis[t]) {
                        path.push_back({ u,t,w });
                        break;
                    }
                }
            }
            state[t] = true;  // 将节点t加入连通块
            for (int j = 1; j <= n; j++)  // 用节点t更新其他节点到连通块的最小距离
                dis[j] = min(dis[j], d[t][j]);  // 注意不是dis[j] + dis[t][j]
        }
        return res;
    }
}
using namespace Prim;

void solve() {
    memset(d, INF, so(d));

    cin >> n;
    for (int i = 1; i <= n; i++) {
        string a; cin >> a;
        mp[a] = i, pm[i] = a;
    }

    int m; cin >> m;
    for (int i = 0; i < m; i++) {
        string a, b; int w; cin >> a >> b >> w;
        int u = mp[a], v = mp[b];
        d[u][v] = d[v][u] = min(d[u][v], w);
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++)
            if (d[i][j] != INF) edges[i].push_back({ j, d[i][j] }), edges[j].push_back({ i,d[i][j] });
    }

    string s; cin >> s;
    ll ans = prim(mp[s]);

    if (ans == INFF) cout << -1;
    else {
        cout << ans << endl;
        for (auto [u, v, w] : path)
            cout << pm[u] << ' ' << pm[v] << ' ' << w << endl;
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