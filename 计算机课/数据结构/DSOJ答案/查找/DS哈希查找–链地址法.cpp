#include<bits/stdc++.h>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
typedef pair<int, int> pii;

const int MAXN = 15;
namespace HashTable {
    const int MOD = 11;
    list<int> ha[MAXN];

    void insert(int x) {
        ha[x % MOD].insert(ha[x % MOD].begin(), x);
    }

    pii find(int x) {  // 返回所在位置、查找次数
        int n = ha[x % MOD].size();
        int cnt = 1;
        for (auto it = ha[x % MOD].begin(); it != ha[x % MOD].end(); it++, cnt++)
            if (*it == x) return { x % MOD,cnt };
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