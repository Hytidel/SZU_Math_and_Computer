#include<bits/stdc++.h>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while (CaseT--)
const int INF = 0x3f3f3f3f;

const int MAXN = 1e5 + 5;
int a[MAXN];
namespace BinaryTree {
    struct Node {
        int data;  // 节点的数据
        int l, r;  // 左右儿子节点
    }Tree[MAXN];
    int idx = 0;  // 当前用到的节点编号

    int build(int l, int r) {
        if (l > r) return 0;

        int u = ++idx;
        Tree[u].data = a[l];
        if (l == r) return u;  // 叶子节点

        int root;  // 右子树的根节点
        for (root = l + 1; root <= r; root++)  // 右子树的根节点是右边第一个值≥a[l]的节点
            if (a[root] >= a[l]) break;
        
        Tree[u].l = build(l + 1, root - 1), Tree[u].r = build(root, r);  // 递归建左右子树
        return u;
    }
    
    void postorderTraversal(int u, vector<int>& res) {  // 后序遍历
        if (Tree[u].l) postorderTraversal(Tree[u].l, res);
        if (Tree[u].r) postorderTraversal(Tree[u].r, res);
        res.push_back(Tree[u].data);
    }
}
using namespace BinaryTree;

bool check(int l, int r) {  // 检查a[l...r]是否满足BST的性质
    if (l >= r) return true;

    int root;  // 右子树的根节点
    for (root = l + 1; root <= r; root++)  // 右子树的根节点是右边第一个值≥a[l]的节点
        if (a[root] >= a[l]) break;

    for (int i = root + 1; i <= r; i++)  // 检查右子树是否满足BST的性质
        if (a[i] < a[l]) return false;
    return check(l + 1, root - 1) && check(root, r);  // 递归检查左、右子树
}

void solve() {
    int n; cin >> n;    
    for (int i = 1; i <= n; i++) cin >> a[i];

    if (!check(1, n)) cout << "NO" << endl;
    else {
        cout << "YES" << endl;
        build(1, n);
        vector<int> res;
        postorderTraversal(1, res);
        int m = res.size();
        for (int i = 0; i < m; i++) cout << res[i] << " \n"[i == m - 1];
    }
}

int main() {
    // CaseT
    solve();
    return 0;
}