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
    int n; string s1, s2; cin >> n >> s1 >> s2;
    
    stack<char> stk;
    string res;
    int idx = 0;  // s2的下标
    vector<string> path;
    for (int i = 0; ; ) {
        if (stk.size() && stk.top() == s2[idx]) {
            res.push_back(stk.top());
            stk.pop();
            idx++;
            path.push_back("out");
        }
        else {
            if (i == n) break;

            stk.push(s1[i]);
            i++;
            path.push_back("in");
        }
    }
    
    if (res == s2) {
        cout << "Yes" << endl;
        for (auto u : path) cout << u << endl;
    }
    else cout << "No" << endl;
    cout << "FINISH" << endl;
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    // init();
    CaseT
    solve();
    return 0;
}