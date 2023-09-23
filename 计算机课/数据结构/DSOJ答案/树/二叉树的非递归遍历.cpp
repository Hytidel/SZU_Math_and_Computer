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
    int n;  // s的长度
    string s;  // 先序遍历
    int begin;

    struct Node {
        char data;
        int l, r;
    }Tree[MAXN];
    int idx;

    BinaryTree(string _s) :s(_s) {
        n = s.length();
        begin = idx = 0;
        build();
    }

    int build() {
        if (begin == n) return 0;

        if (s[begin] == '#') {
            begin++;
            return 0;
        }

        int cur = ++idx;
        Tree[cur].data = s[begin++];
        Tree[cur].l = build(), Tree[cur].r = build();
        return cur;
    }

    void print() {
        for (int i = 1; i <= n; i++) {
            cout << i << ' ' << Tree[i].data << ' '
                << Tree[i].l << ' ' << Tree[i].r << endl;
        }
    }

    void preorderTraversal(int u, vector<char>& res) {
        stack<int> stk;
        stk.push(u);

        while (stk.size()) {
            auto v = stk.top(); stk.pop();
            res.push_back(Tree[v].data);

            if (Tree[v].r) stk.push(Tree[v].r);
            if (Tree[v].l) stk.push(Tree[v].l);
        }
    }

    void inorderTraversal(int u, vector<char>& res) {
        stack<int> stk;
        while (u || stk.size()) {
            while (u) {
                stk.push(u);
                u = Tree[u].l;
            }

            u = stk.top(); stk.pop();
            res.push_back(Tree[u].data);
            u = Tree[u].r;
        }
    }

    void postorderTraversal(int u, vector<char>& res) {
        stack<int> stk;
        int last = -1;

        while (u || stk.size()) {
            while (u) {
                stk.push(u);
                u = Tree[u].l;
            }

            u = stk.top();
            if (!Tree[u].r || last == Tree[u].r) {
                res.push_back(Tree[u].data);
                stk.pop();
                last = u;
                u = 0;
            }
            else u = Tree[u].r;
        }
    }
};

void solve() {
    string s; cin >> s;
    BinaryTree tr(s);

    cout << "Inorder: ";
    vector<char> res;
    tr.inorderTraversal(1, res);
    for (auto u : res) cout << u << " \n"[u == res.back()];

    cout << "Preorder: ";
    res.clear();
    tr.preorderTraversal(1, res);
    for (auto u : res) cout << u << " \n"[u == res.back()];

    cout << "Postorder: ";
    res.clear();
    tr.postorderTraversal(1, res);
    for (auto u : res) cout << u << " \n"[u == res.back()];
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