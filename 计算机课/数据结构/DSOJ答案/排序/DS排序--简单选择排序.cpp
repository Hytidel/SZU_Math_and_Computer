#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

const int MAXN = 1005;
int n;
int a[MAXN];

void print() {
    for (int i = 1; i <= n; i++) cout << a[i] << " \n"[i == n];
}

void selectSort() {
    for (int i = 1; i <= n; i++) {
        int minidx = i;
        for (int j = i + 1; j <= n; j++) 
            if (a[j] < a[minidx]) minidx = j;
        swap(a[i], a[minidx]);
        print();
    }
}

void solve() {
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    selectSort();
    cout << endl;
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    // init();
    CaseT
    solve();
    return 0;
}
}