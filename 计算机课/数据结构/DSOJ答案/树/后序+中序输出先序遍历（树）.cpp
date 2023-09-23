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

    BinaryTree(int _n, string _post, string _in) {
        idx = 0;
        build(_post, _in, _n);
    }

    int build(string post, string in, int n) {  // 要建立的节点数
        if (!n) return 0;
        
        int cur = ++idx;
        Tree[cur].data = post.back();

        int pos = in.find(post.back());
        Tree[cur].r = build(post.substr(pos, n - pos - 1), 
            in.substr(pos + 1), n - pos - 1);
        Tree[cur].l = build(post.substr(0, pos), in, pos);
        return cur;
    }

    void inorderTraversal(int u, vector<char>& res) {
        res.push_back(Tree[u].data);
        if (Tree[u].l) inorderTraversal(Tree[u].l, res);
        if (Tree[u].r) inorderTraversal(Tree[u].r, res);
    }
};

void solve() {
    int n; cin >> n;
    string post, in;
    for (int i = 0; i < n; i++) {
        int ch; cin >> ch;
        post.push_back(ch);
    }
    for (int i = 0; i < n; i++) {
        int ch; cin >> ch;
        in.push_back(ch);
    }

    BinaryTree tr(n, post, in);
    vector<char> res;
    tr.inorderTraversal(1, res);
    cout << "Preorder: ";
    for (auto u : res) cout << (int)u << " \n"[u == res.back()];
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