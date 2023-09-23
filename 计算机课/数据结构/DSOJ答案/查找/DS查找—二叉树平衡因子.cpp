#include<iostream>
using namespace std;
#define IOS ios::sync_with_stdio(0); cin.tie(0), cout.tie(0);
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define so sizeof

const int MAXN = 1e5 + 5;
namespace BinaryTree {
    int n;
    char s[MAXN];
    int begin = 0;  // 数组用到的下标

    struct Node {
        char data;
        int l, r;  // 左右儿子节点的下标
    }Tree[MAXN];
    int idx = 0;  // 当前用到的节点的编号

    int height[MAXN];
    int res[MAXN], pos = 0;  // 后序遍历的序列

    void init() {
        for (int i = 1; i <= idx; i++) height[i] = 0;
        begin = idx = pos = 0;
    }

    int build(int pos) {  // 建树
        if (pos > n || s[pos] == '0') return 0;
        
        int u = ++idx;
        Tree[u].data = s[pos];
        if ((pos << 1) <= n) Tree[u].l = build(pos << 1);
        if ((pos << 1 | 1) <= n) Tree[u].r = build(pos << 1 | 1);
        return u;
    }

    void print() {  // 输出各节点的连接情况
        for (int i = 1; i <= idx; i++)
            cout << i << ' ' << Tree[i].data << ' ' << Tree[i].l << ' ' << Tree[i].r << endl;
    }

    void postorderTraversal(int u) {  // 后序遍历
        if (Tree[u].l) postorderTraversal(Tree[u].l);
        if (Tree[u].r) postorderTraversal(Tree[u].r);
        res[pos++] = u;
    }

    void dfs(int u) {
        bool leaf = true;
        if (Tree[u].l) {
            dfs(Tree[u].l);
            leaf = false;
        }
        if (Tree[u].r) {
            dfs(Tree[u].r);
            leaf = false;
        }

        if (leaf) height[u] = 1;
        height[u] = max(height[Tree[u].l], height[Tree[u].r]) + 1;
    }
};
using namespace BinaryTree;

void solve() {
    init();

    cin >> n >> s + 1;
    build(1);

    dfs(1);

    postorderTraversal(1);

    for (int i = 0; i < pos; i++) 
        cout << Tree[res[i]].data << ' ' << height[Tree[res[i]].l] - height[Tree[res[i]].r] << endl;
}

int main() {
    IOS;
    CaseT  // 单测时注释掉该行
    solve();
    return 0;
}