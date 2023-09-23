#include<bits/stdc++.h>
using namespace std;
#define endl '\n'

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
    solve();
    return 0;
}