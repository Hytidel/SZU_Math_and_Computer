#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

const ll INFF = 0x3f3f3f3f3f3f3f3f;

struct Kruskal {
    int n, m;  // 节点数、边数
    struct Edge {
        int u, v, w;

        bool operator<(const Edge& p) const {
            return w < p.w;
        }
    };
    vector<Edge> edges;
    vector<int> fa;

    Kruskal(int  _n) :n(_n), m(0) {
        fa.resize(n + 1);
        iota(all(fa), 0);
    }

    int find(int x) {
        return x == fa[x] ? x : fa[x] = find(fa[x]);
    }
    
    ll kruskal() {  // 求MST,返回MST的边权和
        sort(all(edges));

        ll res = 0;  // MST的边权和
        int cnt = 0;  // 当前连的边数
        for (int i = 0; i < m; i++) {
            auto [u, v, w] = edges[i];
            
            u = find(u), v = find(v);
            if (u != v) {
                fa[u] = v;
                res += w;
                cnt++;
            }
        }
        return cnt < n - 1 ? INFF : res;
    }
};

void solve() {
    int n; 
    while (cin >> n) {
        vector<vector<int>> d(n + 1, vector<int>(n + 1));
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) cin >> d[i][j];

        CaseT{
            int u, v; cin >> u >> v;
            d[u][v] = 0;
        }

        Kruskal solver(n);
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                solver.edges.push_back({ i, j, d[i][j] });
                solver.m++;
            }
        }
        cout << solver.kruskal() << endl;
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    // init();
    // CaseT
    solve();
    return 0;
}