#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

const double eps = 1e-8;

int sgn(double x) {
    if (fabs(x) < eps) return 0;
    else return x < 0 ? -1 : 1;
}

const int MAXN = 1e3 + 5;
namespace TopologicalSorting {
    int n;
    double edges[MAXN][MAXN];
    int in[MAXN];
    vector<int> res;  // 拓扑序
    double dis[MAXN];
    int pre[MAXN];

    void addEdge(int u, int v, double w) {
        edges[u][v] = w;
        in[v]++;
    }

    void topo() {
        queue<int> que;
        for (int i = 0; i < n; i++)
            if (!in[i]) que.push(i);

        while (que.size()) {
            auto u = que.front(); que.pop();
            res.push_back(u);

            for (int v = 0; v < n; v++) {
                if (sgn(edges[u][v]))
                    if (--in[v] == 0) que.push(v);
            }
        }
    }

    void cal() {
        for (auto u : res) {
            for (int v = 0; v < n; v++) {
                if (sgn(edges[u][v])) {
                    double tmp = dis[u] + edges[u][v];
                    if (dis[v] < tmp) {
                        dis[v] = tmp;
                        pre[v] = u;
                    }
                }
            }
        }
    }
}
using namespace TopologicalSorting;

void solve() {
    int m; cin >> n >> m;
    while (m--) {
        int u, v; double w; cin >> u >> v >> w;
        addEdge(u, v, w);
    }

    topo();
    cal();

    for (int i = 0; i < n; i++) {
        if (i == 5) continue;

        int cur = i;
        vector<int> path;
        while (true) {
            path.push_back(cur);
            cur = pre[cur];

            if (cur == 5) {
                path.push_back(cur);
                break;
            }
        }

        reverse(all(path));
        for (auto u : path) cout << u << " \n"[u == path.back()];
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