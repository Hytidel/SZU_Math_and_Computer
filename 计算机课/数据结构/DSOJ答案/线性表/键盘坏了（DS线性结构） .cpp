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
    string s;
    while (cin >> s) {
        list<char> ans;
        auto it = ans.begin();
        for (auto ch : s) {
            if (ch == '[') it = ans.begin();
            else if (ch == ']') it = ans.end();
            else ans.insert(it, ch);
        }

        for (auto ch : ans) cout << ch;
        cout << endl;
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