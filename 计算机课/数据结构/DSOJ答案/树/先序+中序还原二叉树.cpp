#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

const int MAXN = 1e3 + 5;
struct BinaryTree {
    struct Node {
        char data;
        int l, r;
    }Tree[MAXN];
    int idx;

    BinaryTree(int _n, string _pre, string _in) {
        idx = 0;
        build(_pre, _in, _n);
    }

    int build(string pre, string in, int n) {  // 要建立的节点数
        if (!n) return 0;

        int cur = ++idx;
        Tree[cur].data = pre[0];

        int pos = in.find(pre[0]);
        Tree[cur].l = build(pre.substr(1), in, pos);
        Tree[cur].r = build(pre.substr(pos + 1), in.substr(pos + 1), n - pos - 1);
        return cur;
    }

    int getHeight(int u) {
        if (!u) return 0;
        else return max(getHeight(Tree[u].l), getHeight(Tree[u].r)) + 1;
    }
};

void solve() {
    int n; string pre, in; cin >> n >> pre >> in;

    BinaryTree tr(n, pre, in);
    cout << tr.getHeight(1) << endl;
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