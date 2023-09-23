#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

void solve() {
    int n; cin >> n;
    vector<int> a(n), b;  // a[]为哈希表、b[]为有效数据
    for (auto& ai : a) {
        cin >> ai;
        if (~ai) b.push_back(ai);
    }

    sort(all(b));

    vector<bool> used(n), vis(n);  // 每个数据是否已放好、每个位置是否已被占
    vector<int> ans;
    while ((int)ans.size() < (int)b.size()) {
        for (int i = 0; i < b.size(); i++) {
            if (used[i]) continue;

            bool ok = false;
            for (int j = b[i] % n;; j = (j + 1) % n) {
                if (!vis[j] && b[i] == a[j]) {  // 找到数据的位置
                    used[i] = vis[j] = true;
                    ans.push_back(b[i]);
                    ok = true;
                    break;
                }

                if (!vis[j]) break;  // 哈希表该位置为空,不再往后找
            }
            if (ok) break;
        }
    }

    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    // init();
    // CaseT
    solve();
    return 0;
}