#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

struct Dijkstra {
    int n;
    vector<vector<pair<int, int>>> edges;
    vector<bool> state;
    vector<ll> dis;

    Dijkstra(int _n, const vector<vector<pair<int, int>>>& _edges) :n(_n), edges(_edges) {
        state.resize(n + 1);
        dis = vector<ll>(n + 1, INF);
    }

    void dijkstra(int s) {
        dis[s] = 0;
        for (int i = 0; i < n - 1; i++) {
            int t = -1, j;
            for (j = 1; j <= n; j++) {
                if (!state[j] && (t == -1 || dis[t] > dis[j]))
                    t = j;
            }
            if (j == n) break;

            state[t] = true;
            for (auto [v, w] : edges[t])
                dis[v] = min(dis[v], dis[t] + w);
        }
    }
};

void solve() {
    int n; cin >> n;
    vector<vector<pair<int, int>>> edges(n + 1);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            int x; cin >> x;
            if (x) edges[i].push_back({ j,x });
        }
    }
    Dijkstra solver(n, edges);

    int s, k; cin >> s >> k;
    solver.dijkstra(++s);
    
    vector<pair<int, int>> nodes;
    for (int u = 1; u <= n; u++) {
        if (u != s)
            nodes.push_back({ solver.dis[u], u });
    }
    sort(all(nodes));

    vector<int> ans;
    for (int i = 0, rk = 0; i < nodes.size(); i++) {
        if (!i || nodes[i].first != nodes[i - 1].first) rk = i + 1;

        if (rk == k) ans.push_back(nodes[i].second);
    }
    sort(all(ans));
    for (auto u : ans) cout << u - 1 << " \n"[u == ans.back()];
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    // init();
    CaseT
    solve();
    return 0;
}