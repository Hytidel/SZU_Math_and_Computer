#include<bits/stdc++.h>
using namespace std;
#define endl '\n'

const int MAXN = 1e5 + 5;
namespace HeapSort {
    int n;
    int a[MAXN];

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
        cout << n << ' ';
        for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];
    }

    void init() {  // 堆初始化
        for (int i = n / 2; i >= 0; i--) sift(i, n);
        print();
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
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];

    init();
    heapSort();
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
    solve();
    return 0;
}