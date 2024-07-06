#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const double pi = acos(-1.0);

void solve() {
    int n, m; cin >> n >> m;
    vector<vector<int>> f(n, vector<int>(m));
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) cin >> f[i][j];

    double centerX = n / 2.0, centerY = m / 2.0;
    printf("旋转中心: centerX = %.1lf, centerY = %.1lf .\n", centerX, centerY);

    double theta = pi / 3;
    int N = ceil(n * fabs(cos(theta)) + m * fabs(sin(theta))), 
        M = ceil(n * fabs(sin(theta)) + m * fabs(cos(theta)));
    printf("旋转后尺寸: N = %d, M = %d.\n", N, M);

    vector<vector<double>> ans(N, vector<double>(M));
    for (int X = 0; X < N; X++) {
        for (int Y = 0; Y < M; Y++) {
            double x = centerX + (X - centerX) * cos(theta) + (Y - centerY) * sin(theta), 
                y = centerY - (X - centerX) * sin(theta) + (Y - centerY) * cos(theta);
            printf("旋转后点 (%d, %d) 对应原图的点 (%.2lf, %.2lf) , ", X, Y, x, y);

            if (0 <= x && x <= n - 1 && 0 <= y && y <= m - 1) {
                printf("在原图像素中.\n");

                int x1 = max(int(x), 0), y1 = max(int(y), 0), x2 = min(x1 + 1, n - 1), y2 = min(y1 + 1, m - 1);
                printf("  其近邻的四个点为: \n");
                printf("    (max(floor(%.2lf), 0), max(floor(%.2lf), 0)) = (%d, %d), \n", x, y, x1, y1);
                printf("    (min(%d + 1, n - 1), %d) = (%d, %d), \n", x1, y1, x2, y1);
                printf("    (%d, min(%d + 1, m - 1)) = (%d, %d), \n", x1, y1, x1, y2);
                printf("    (min(%d + 1, n - 1), min(%d + 1, m - 1)) = (%d, %d). \n", x1, y1, x2, y2);

                double w11 = (x2 - x) * (y2 - y), w21 = (x - x1) * (y2 - y), 
                w12 = (x2 - x) * (y - y1), w22 = (x - x1) * (y - y1);
                printf("  权重: \n");
                printf("    w11 = (x2 - x) * (y2 - y) ≈ %.2lf, \n", w11);
                printf("    w21 = (x - x1) * (y2 - y) ≈ %.2lf, \n", w21);
                printf("    w12 = (x2 - x) * (y - y1) ≈ %.2lf, \n", w12);
                printf("    w22 = (x - x1) * (y - y1) ≈ %.2lf, \n", w22);

                double res = f[x1][y1] * w11 + f[x2][y1] * w21 + f[x1][y2] * w12 + f[x2][y2] * w22;
                ans[X][Y] = res;
                printf("  像素值: \n");
                printf("    f'[%d][%d] = f[%d][%d] * w11 + f[%d][%d] * w21 + f[%d][%d] * w12 + f[%d][%d] * w22 = %.2lf.\n", 
                    X, Y, x1, y1, x2, y1, x1, y2, x2, y2, res);
            }
            else {
                printf("不在原图像素中.\n");
            }

            printf("\n");            
            // break;
        }
        // break;
    }

    printf("旋转后图像: \n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            cout << setprecision(5) << ans[i][j] << " \n"[j == M - 1];
    }
}

int main() {
    // cin.tie(0)->sync_with_stdio(false);
    // int t; cin >> t; while (t--)
    solve();
    return 0;
}