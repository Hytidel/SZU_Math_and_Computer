#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> dx = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
vector<int> dy = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

void solve() {
    int n = 6;
    vector<vector<int>> a(n + 5, vector<int>(n + 5));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> a[i][j];
        }
    }

    auto b = a;
    for (int i = 2; i <= n - 1; i++) {
        for (int j = 2; j <= n - 1; j++) {
            vector<int> items;
            for (int k = 0; k < 9; k++) {
                int x = i + dx[k], y = j + dy[k];
                items.push_back(a[x][y]);
            }

            sort(items.begin(), items.end());
            b[i][j] = items[4];
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cout << b[i][j] << " \n"[j == n];
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    // int t; cin >> t; while (t--)
    solve();
    return 0;
}