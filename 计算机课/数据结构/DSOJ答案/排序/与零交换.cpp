#include<bits/stdc++.h>
using namespace std;
#define endl '\n'

const int MAXN = 1e5 + 5;
int n;
int p[MAXN];

void solve() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		int x; cin >> x;
		p[x] = i;
	}

	int ans = 0;
	int idx = 1;  // 将要与节点0交换的节点
	while (idx < n) {
		while (p[0]) swap(p[0], p[p[0]]), ans++;  // 将节点0所在的环都变成自环
		while (idx < n && p[idx] == idx) idx++;  // 跳过已是自环的节点
		if (idx < n) swap(p[0], p[idx]), ans++;  // 将不含节点0的环与节点0所在的环合并
	}
	cout << ans << endl;
}

int main() {
	cin.tie(0), cout.tie(0)->sync_with_stdio(false);
	solve();
	return 0;
}