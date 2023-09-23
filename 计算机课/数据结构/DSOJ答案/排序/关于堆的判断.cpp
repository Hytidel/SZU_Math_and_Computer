#include<bits/stdc++.h>
using namespace std;
#define endl '\n'

const int MAXN = 1e5 + 5;
namespace HeapSort {
    int n;
    int a[MAXN];
    map<int, int> fa;

    void sift(int u, int length) {  // 筛选
        int ls = u * 2 + 1, rs = u * 2 + 2;
        if (ls >= length) return;  // 不存在左儿子

        if (rs < length) {  // 存在右儿子
            if (a[ls] < a[rs]) {
                if (a[ls] < a[u]) {
                    swap(a[ls], a[u]);
                    sift(ls, length);  // 筛选左儿子
                }
            }
            else {
                if (a[rs] < a[u]) {
                    swap(a[rs], a[u]);
                    sift(rs, length);  // 筛选右儿子
                }
            }
        }
        else {  // 只有左儿子
            if (a[ls] < a[u]) {
                swap(a[ls], a[u]);
                sift(ls, length);  // 筛选左儿子
            }
        }
    }

    void print() {
        for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];
    }

    void init() {  // 堆初始化
        for (int i = n / 2; i >= 0; i--) sift(i, n);
        // print();

        for (int u = 0; u < n; u++) {
            int ls = u * 2 + 1, rs = u * 2 + 2;
            if (ls < n) fa[a[ls]] = a[u];
            if (rs < n) fa[a[rs]] = a[u];
        }
    }

    void heapSort() {
        for (int i = n - 1; i >= 1; i--) {
            swap(a[0], a[i]);
            sift(0, i);
            print();
        }
    }
};
using namespace HeapSort;

void solve() {
    int m; cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> a[i];
    cin.ignore(' ', 10);

    init();
    
    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        vector<string> tmp;
        string str;
        int op;
        while (ss >> str) {
            if (str == "root") op = 1;
            else if (str == "siblings") op = 2;
            else if (str == "parent") op = 3;
            else if (str == "child") op = 4;

            tmp.push_back(str);
        }
        
        if (op == 1) {
            int u = stoi(tmp[0].c_str());
            cout << ((u == a[0]) ? 'T' : 'F') << endl;
        }
        else if (op == 2) {
            int u = stoi(tmp[0].c_str()), v = stoi(tmp[2].c_str());
            cout << (fa[u] == fa[v] ? 'T' : 'F') << endl;
        }
        else if (op == 3) {
            int u = stoi(tmp[0].c_str()), v = stoi(tmp[5].c_str());
            cout << (fa[v] == u ? 'T' : 'F') << endl;
        }
        else {
            int u = stoi(tmp[0].c_str()), v = stoi(tmp[5].c_str());
            cout << (fa[u] == v ? 'T' : 'F') << endl;
        }
    }
    
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
    solve();
    return 0;
}