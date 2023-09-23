#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

const int MAXN = 1e3 + 5;
struct SPFA {
    int n;  // 节点数
    int d[MAXN][MAXN];
    ll dis[MAXN];  // 起点到每个节点的最短距离
    bool state[MAXN];  // 记录当前每个节点是否在队列中

    SPFA(int _n) :n(_n) {
        memset(d, INF, sizeof(d));
        memset(state, false, sizeof(state));
    }

    void addEdge(int u, int v, int w) {
        d[u][v] = d[v][u] = min(d[u][v], w);  // 重边保留最短的
    }

    ll spfa(int s) {  // 求起点s到其他节点的最短路
        memset(dis, INF, sizeof(dis));
        
        queue<int> que;
        dis[s] = 0;
        que.push(s);
        state[s] = true;
        
        while (que.size()) {
            auto u = que.front(); que.pop();
            state[u] = false;

            for (int v = 1; v <= n; v++) {
                if (d[u][v] != INF) {
                    if (dis[v] > dis[u] + d[u][v]) {
                        dis[v] = dis[u] + d[u][v];

                        if (!state[v]) {
                            que.push(v);
                            state[v] = true;
                        }
                    }
                }
            }
        }
        return dis[n];
    }
};

void solve() {
    int n; 
    while (cin >> n) {
        SPFA solver(n);
        
        int m; cin >> m;
        while (m--) {
            int u, v, w; cin >> u >> v >> w;
            solver.addEdge(u, v, w);
        }
        
        cout << solver.spfa(1) << endl;
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