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
struct HashTable {
    const int MOD = 11;
    int len;  // 表长
    int ha[MAXN];

    HashTable(int _len) :len(_len) {
        for (int i = 0; i < len; i++) ha[i] = -1;
    }

    void insert(int x) {
        int pos = x % MOD;
        while (pos < len && ~ha[pos]) pos = (pos + 1) % len;
        ha[pos] = x;
    }

    pair<int, int> find(int x) {  // 返回查找次数、查找成功的位置
        int pos = x % MOD;
        int step = 1;  // 查找次数
        for (int i = pos; i < len; i++, step++) {
            if (ha[i] == -1) return { step, -1 };
            else if (ha[i] == x) return { step, i + 1 };
        }

        for (int i = 0; i < pos; i++, step++) {
            if (ha[i] == -1) return { step, -1 };
            else if (ha[i] == x) return { step, i + 1 };
        }
        return { step, -1 };
    }

    void print() {
        for (int i = 0; i < len; i++)
            cout << (~ha[i] ? to_string(ha[i]) : "NULL") << " \n"[i == len - 1];
    }
};

void solve() {
    int len, n; cin >> len >> n;
    HashTable hat(len);
    while (n--) {
        int x; cin >> x;
        hat.insert(x);
    }
    hat.print();

    CaseT{
        int x; cin >> x;
    
        auto [u, v] = hat.find(x);
        if (~v) cout << 1 << ' ' << u << ' ' << v << endl;
        else cout << 0 << ' ' << u << endl;
    }
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