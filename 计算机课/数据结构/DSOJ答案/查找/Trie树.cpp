#include<bits/stdc++.h>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define pb push_back
#define all(x) (x).begin(), (x).end()

const int MAXN = 3e5 + 5;
namespace Trie {
    int trie[MAXN][26];
    int cnt[MAXN];  // 记录以每个节点为结尾的单词数
    int idx;  // 当前用到的节点编号

    void insert(string s) {
        int u = 0;
        for (auto ch : s) {
            int c = ch - 'a';
            if (!trie[u][c]) trie[u][c] = ++idx;  // 无相应的儿子节点则创建
            u = trie[u][c];  // 走到相应的儿子节点
        }
        cnt[u]++;  // 更新以该节点结尾的单词数
    }

    void bfs() {
        string res;
        queue<int> que;
        que.push(0);
        while (que.size()) {
            auto u = que.front(); que.pop();
            for (int i = 0; i < 26; i++) {
                if (trie[u][i]) {
                    que.push(trie[u][i]);
                    res.push_back('a' + i);
                }
            }
        }
        cout << res << endl;
    }

    int res;
    void dfs(int u) {
        res += cnt[u];
        for (int i = 0; i < 26; i++)
            if (trie[u][i]) dfs(trie[u][i]);
    }

    int query(string s) {
        int u = 0;
        for (auto ch : s) {
            int c = ch - 'a';
            if (trie[u][c]) u = trie[u][c];
            else return 0;
        }

        res = 0;
        dfs(u);
        return res;
    }
}
using namespace Trie;

void solve() {
    string line; getline(cin, line);
    stringstream ss(line);
    string s;
    while (ss >> s) insert(s);

    bfs();

    CaseT{
        string s; cin >> s;
        cout << query(s) << endl;
    }
}

int main() {
    solve();
    return 0;
}