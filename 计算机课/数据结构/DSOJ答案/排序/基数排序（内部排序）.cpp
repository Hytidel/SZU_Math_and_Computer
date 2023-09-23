#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

const int MAXN = 1005;
int n;
int a[MAXN];

void print(vector<int> a[10]) {
    for (int i = 0; i < 10; i++) {
        cout << i << ':';
        if (!a[i].size()) {
            cout << "NULL" << endl;
            continue;
        }

        for (int j = 0; j < a[i].size(); j++) {
            cout << "->";
            cout << a[i][j];
            if (j == a[i].size() - 1) cout << "->^" << endl;
        }
    }
}

void print() {
    for (int i = 1; i <= n; i++) cout << a[i] << " \n"[i == n];
}

void radixSort() {
    int lim = 0;
    for (int i = 1; i <= n; i++) 
        while ((int)pow(10, lim) <= a[i]) lim++;

    for (int i = 0; i < lim; i++) {
        vector<int> radix[10];
        for (int j = 1; j <= n; j++)
            radix[a[j] / (int)pow(10, i) % 10].push_back(a[j]);
        print(radix);
        
        int idx = 1;
        for (int j = 0; j < 10; j++)
            for (auto ai : radix[j]) a[idx++] = ai;
        print();
    }
}

void solve() {
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    radixSort();
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