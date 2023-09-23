#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;



void solve() {
    int n; cin >> n;
    vector<int> a(n + 5);
    for (int i = 1; i <= n; i++) cin >> a[i];

    CaseT{
        int x; cin >> x;

        int l = 1, r = n;
        while (l <= r) {
            int mid = l + r >> 1;
            if (a[mid] > x) r = mid - 1;
            else l = mid + 1;
        }
        cout << (a[r] == x ? to_string(r) : "error") << endl;
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