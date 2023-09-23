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
struct HuffmanTree {
    int n;  // 原序列长度

    struct Node {
        int id;
        char data;
        int w;
        int l, r;
        int fa;

        Node() : l(0), r(0), fa(0) {}
        Node(int _id, int _w, int _l, int _r) :id(_id), w(_w), l(_l), r(_r) {}
        
        bool operator>(const Node& p)const {
            return w != p.w ? w > p.w : id > p.id;
        }
    }Tree[MAXN];
    int idx;  // 当前用到的节点编号

    map<char, string> mp;  // 编码结果

    // 新建一个权值为w的节点,左右儿子节点分别为l、r
    Node newNode(int w, int l = 0, int r = 0) {
        return Tree[idx] = { ++idx, w, l, r };
    }

    HuffmanTree(int _n, const vector<int>& _a, const vector<char>& _chs): 
        n(_n) {  // a[]下标从1开始
        idx = 0;

        priority_queue<Node, vector<Node>, greater<Node>> heap;
        for (int i = 1; i <= n; i++) {
            heap.push(newNode(_a[i]));
            Tree[i].data = _chs[i];
        }

        while (heap.size() >= 2) {
            auto left = heap.top(); heap.pop();
            auto right = heap.top(); heap.pop();
            auto root = newNode(left.w + right.w, left.id, right.id);
            heap.push(root);
            Tree[left.id].fa = Tree[right.id].fa = root.id;
        }
    }

    void print() {
        for (int i = 1; i <= idx; i++) {
            cout << Tree[i].id << ' ' << Tree[i].w << ' '
                << Tree[i].l << ' ' << Tree[i].r << endl;
        }
    }

    void encode() {
        for (int i = 1; i <= n; i++) {  // 枚举叶子节点
            string res;
            for (int u = i; u != idx; u = Tree[u].fa) {
                if (Tree[Tree[u].fa].l == u) res.push_back('0');
                else res.push_back('1');
            }

            reverse(all(res));
            cout << Tree[i].data << " :" << res << endl;
            mp[Tree[i].data] = res;
        }
    }

    string encode(string s) {
        string res;
        for (auto ch : s) res += mp[ch];
        return res;
    }

    void decode(string s) {
        int u = idx;
        string res;
        for (auto ch : s) {
            if (ch == '0') {
                if (Tree[u].l) u = Tree[u].l;
                else {
                    cout << "error!" << endl;
                    return;
                }
            }
            else {
                if (Tree[u].r) u = Tree[u].r;
                else {
                    cout << "error!" << endl;
                    return;
                }
            }

            if (1 <= Tree[u].id && Tree[u].id <= n) {  // 叶子节点
                res.push_back(Tree[u].data);
                u = idx;  // 回到根节点
            }
        }
        cout << (u == idx ? res : "error!") << endl;
    }
};

void solve() {
    int n; cin >> n;
    vector<char> chs(n + 1);
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> chs[i];
    for (int i = 1; i <= n; i++) cin >> a[i];

    HuffmanTree tr(n, a, chs);
    tr.encode();

    string s; cin >> s;
    cout << tr.encode(s) << endl;
    
    cin >> s;
    tr.decode(s);
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