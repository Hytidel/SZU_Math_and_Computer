#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

const int MAXN = 1e5 + 5;
int n;
int a[MAXN];
int ans;

void bubbleSort() {
    for (int i = n - 1; i; i--) {
        for (int j = 0; j < i; j++)
            if (a[j] > a[j + 1]) swap(a[j], a[j + 1]), ans++;
    }
}

void solve() {
    while (cin >> n) {
        ans = 0;
        for (int i = 0; i < n; i++) cin >> a[i];

        bubbleSort();
        cout << ans << endl;
    }
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