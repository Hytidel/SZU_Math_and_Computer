#include<bits/stdc++.h>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while (CaseT--)
const int INF = 0x3f3f3f3f;

const int MAXN = 1e5 + 5;
struct BinarySearchTree {
    struct Node {
        int l, r;  // 左右儿子节点
        int val;  // 节点的值
        int cnt;  // 该权值出现的次数
    }BST[MAXN];
    int root;  // 根节点
    int idx = 0;  // 当前用到的节点编号

    BinarySearchTree() { build(); }

    int newNode(int x) {  // 新建一个节点
        BST[++idx].val = x;
        BST[idx].cnt = 1;
        BST[idx].l = BST[idx].r = 0;
        return idx;
    }

    void build() {
        newNode(-INF), newNode(INF);  // 哨兵
        BST[root = 1].r = 2;  // 根节点-INF的右儿子为INF
    }
    void inorderTraversal(int u, vector<int>& res) {  // 中序遍历
        if (BST[u].l) inorderTraversal(BST[u].l, res);
        if (abs(BST[u].val) != INF) 
            for (int i = 0; i < BST[u].cnt; i++) res.push_back(BST[u].val);
        if (BST[u].r) inorderTraversal(BST[u].r, res);
    }

    void print() {  // 输出BST中存储的非降序序列
        vector<int> res;
        inorderTraversal(root, res);
        int n = res.size();
        for (int i = 0; i < n; i++) cout << res[i] << ' ';
        cout << endl;
    }

    void insert(int& u, int x) {  // 在以u为根节点的子树中插入一个值为x的节点
        if (!u) u = newNode(x);  // 无该节点则创建
        else if (BST[u].val == x) BST[u].cnt++;  // 有该节点则更新cnt值
        else if (BST[u].val > x) insert(BST[u].l, x);  // 递归插入到左子树
        else insert(BST[u].r, x);  // 递归插入到右子树
    }
};

void solve() {
    BinarySearchTree bst;
    int n; cin >> n;
    while(n--) {
        int x; cin >> x;
        bst.insert(bst.root, x);
    }
    bst.print();
    
    CaseT{
        int x; cin >> x;
        bst.insert(bst.root, x);
        bst.print();
    }
}

int main() {
    CaseT
    solve();
    return 0;
}