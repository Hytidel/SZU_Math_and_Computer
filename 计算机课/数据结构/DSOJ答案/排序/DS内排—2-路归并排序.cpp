#include<bits/stdc++.h>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'

const int MAXN = 1e5 + 5;
int n;
string strs[MAXN], tmp[MAXN];

void solve() {
    cin >> n;
    for (int i = 0; i < n; i++) cin >> strs[i];

    int lim;  // ≥n的最小的2的幂次
    for (lim = 1; lim < n; lim <<= 1);

    for (int len = 2; len <= lim; len <<= 1) {  // len表示当前归并的长度
        for (int i = 0; i < n; i += len) {  // 分组
            int l = i, r = i + len / 2;
            if (r > n) break;

            int mid = r - 1;
            int idx = l;
            while (l <= mid && r < min(i + len, n)) {  // 归并
                if (strs[l] > strs[r]) tmp[idx++] = strs[l++];
                else tmp[idx++] = strs[r++];
            }

            // 放完剩下的元素
            while (l <= mid) tmp[idx++] = strs[l++];
            while (r < min(i + len, n)) tmp[idx++] = strs[r++];

            for (int j = i; j < min(i + len, n); j++) strs[j] = tmp[j];
        }

        for (int i = 0; i < n; i++) cout << strs[i] << " \n"[i == n - 1];
    }

    cout << endl;
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
    CaseT
    solve();
    return 0;
}