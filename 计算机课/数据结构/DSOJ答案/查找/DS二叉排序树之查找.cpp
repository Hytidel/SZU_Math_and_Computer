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

    int getPrev(int u, int x) {  // 在以u为根节点的子树中找到<x的最大值所在节点,即中序遍历的前驱
        if (!u) return -INF;  // 不存在该节点
        else if (BST[u].val < x) return u;  // 第一个值<x的节点
        else if (BST[u].val >= x) return getPrev(BST[u].l, x);  // 递归到左子树找
        else return getPrev(BST[u].r, x);  // 递归到右子树找
    }

    void remove(int& u, int x) {  // 在以u为根节点的子树中删除一个值为x的节点,注意传引用
        if (!u) return;  // 不存在该节点

        if (BST[u].val == x) {  // 找到值为x的节点
            if (BST[u].cnt > 1) BST[u].cnt--;  // 无需删除节点,只需更新cnt值
            else if (!BST[u].l && !BST[u].r) u = 0;  // 叶子节点,直接删除
            else if (BST[u].l && !BST[u].r) u = BST[u].l;  // 只有左子树,没有右子树
            else if (!BST[u].l && BST[u].r) u = BST[u].r;  // 只有右子树,没有左子树
            else {  // 既有左子树,也有右子树
                int pre = getPrev(u, x);  // 找到中序遍历中节点u的前驱节点
                int r = BST[u].r;  // 暂存节点u的右子树的根节点
                BST[u = pre].r = r;  // 用节点u的前驱替换节点u
            }
        }
        else if (BST[u].val > x) remove(BST[u].l, x);  // 递归到左子树中删除
        else remove(BST[u].r, x);  // 递归到右子树中删除
    }

    int find(int u, int x, int step) {  // 在以u为根节点的子树中查找值为x的节点,返回查找次数,不存在时查找次数为INF
        if (!u) return INF;  // 不存在值为x的节点
        else if (BST[u].val == x) return step;  // 找到值为x的节点
        else if (BST[u].val > x) return find(BST[u].l, x, step + 1);  // 递归到左子树找
        else return find(BST[u].r, x, step + 1);  // 递归到右子树找
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
        int ans = bst.find(bst.root, x, -1);  // 因有哨兵节点,查找次数从-1开始
        cout << (ans == INF ? -1 : ans) << endl;
    }
}

int main() {
    CaseT
    solve();
    return 0;
}