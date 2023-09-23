#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

struct KMP {
    int n, m;  // 模式串、文本串长度
    string p, s;  // 模式串、文本串
    vector<int> nxt;

    KMP(string _p, string _s = "") :
        p(" " + _p), n(_p.length()), s(" " + _s), m(_s.length()) {
        nxt.resize(n + 1);
        
        for (int i = 2, j = 0; i <= n; i++) {
            while (j && p[i] != p[j + 1]) j = nxt[j];
            if (p[i] == p[j + 1]) j++;
            nxt[i] = j;
        }
    }

    vector<int> kmp() {
        vector<int> res;
        for (int i = 1, j = 0; i <= m; i++) {
            while (j && s[i] != p[j + 1]) j = nxt[j];
            if (s[i] == p[j + 1]) j++;

            if (j == n) {
                res.push_back(i - n);
                j = nxt[j];
            }
        }
        return res;
    }
};

void solve() {
    string s, p; cin >> s >> p;
    
    KMP solver(p, s);

    int len = (int)p.length();
    cout << -1 << ' ';
    for (int i = 1; i < len; i++) cout << solver.nxt[i] << ' ';
    cout << endl;
    
    auto ans = solver.kmp();
    cout << (ans.size() ? ans[0] + 1 : 0) << endl;
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