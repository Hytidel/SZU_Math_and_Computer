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
namespace Floyd {
    int n;  // 节点数
    bool d[MAXN][MAXN];  // 图的可达矩阵:d[u][v]表示节点u与v间的是否有边

    void floyd() {
        for (int k = 1; k <= n; k++) {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++)
                    d[i][j] |= d[i][k] & d[k][j];
            }
        }
    }
}
using namespace Floyd;

void solve() {
    cin >> n;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) cin >> d[i][j];

    floyd();
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++)
            cout << d[i][j] << " \n"[j == n];
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