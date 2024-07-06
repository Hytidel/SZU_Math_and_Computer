#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> dx = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
vector<int> dy = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

void solve() {
    int n = 5;
    vector<vector<int>> a(n + 5, vector<int>(n + 5));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> a[i][j];
        }
    }

    auto b = a;

    auto sigma08 = [&]() {
        vector<int> w = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };

        for (int i = 2; i <= 4; i++) {
            for (int j = 2; j <= 4; j++) {
                int sum = 0;
                for (int k = 0; k < 9; k++) {
                    int x = i + dx[k], y = j + dy[k];
                    sum += a[x][y] * w[k];
                }

                b[i][j] = round(sum / 16.0);
            }
        }
    };

    sigma08();

    auto sigma1 = [&]() {
        vector<int> w = { 1, 1, 1, 1, 2, 1, 1, 1, 1 };

        for (int i = 2; i <= 4; i++) {
            for (int j = 2; j <= 4; j++) {
                int sum = 0;
                for (int k = 0; k < 9; k++) {
                    int x = i + dx[k], y = j + dy[k];
                    sum += a[x][y] * w[k];
                }

                b[i][j] = round(sum / 10.0);
            }
        }
    };

    // sigma1();

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