#include<bits/stdc++.h>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
typedef pair<int, int> pii;

const int MAXN = 15;
namespace HashTable {
    const int MOD = 11;
    vector<int> ha[MAXN];

    void insert(int x) {
        ha[x % MOD].push_back(x);
    }

    pii find(int x) {  // 返回所在位置、查找次数
        int n = ha[x % MOD].size();
        for (int i = 0; i < n; i++)
            if (ha[x % MOD][i] == x) return { x % MOD,i + 1 };
        return { -1,-1 };
    }
}
using namespace HashTable;

void solve() {
    int n; cin >> n;
    while (n--) {
        int x; cin >> x;
        insert(x);
    }

    CaseT{
        int x; cin >> x;
        auto [u,v] = find(x);
        if (~u) cout << u << ' ' << v << endl;
        else {
            cout << "error" << endl;
            insert(x);
        }
    }
}

int main() {
    solve();
    return 0;
}