#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

const int MAXN = 1e5 + 5;
namespace BinaryTree {
	int n;
	char s[MAXN];
	int begin;
	struct Node {
		int data;
		int l, r;
	}Tree[MAXN];
	int idx = 0;

	int build() {
		if (begin == n) return 0;

		if (s[begin] == '#') {
			begin++;
			return 0;
		}

		int cur = ++idx;
		Tree[cur].data = s[begin++];
		Tree[cur].l = build(), Tree[cur].r = build();
		return cur;
	}

	bool dfs(int u) {
		bool ok = true;
		bool leaf = true;
		if (Tree[u].l) {
			ok &= Tree[u].data >= Tree[Tree[u].l].data && dfs(Tree[u].l);
			leaf = false;
		}
		if (Tree[u].r) {
			ok &= Tree[u].data <= Tree[Tree[u].r].data && dfs(Tree[u].r);
			leaf = false;
		}
		return leaf ? true : ok;
	}
}
using namespace BinaryTree;

void solve() {
	cin >> s;
	n = strlen(s);

	build();

	cout << (dfs(1) ? "Yes" : "No") << endl;
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
#endif
	// init();
	// CaseT
    solve();
    return 0;
}