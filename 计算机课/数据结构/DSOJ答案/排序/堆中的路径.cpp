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
int n;
int heap[MAXN], idx;

void up(int u) {
	while (u >> 1 && heap[u] < heap[u >> 1]) {
		swap(heap[u], heap[u >> 1]);
		u >>= 1;
	}
}

void insert(int x) {
	heap[++idx] = x;
	up(idx);
}

void solve() {
	int m; cin >> n >> m;
	for (int i = 0; i < n; i++) {
		int x; cin >> x;
		insert(x);
	}

	while (m--) {
		int x; cin >> x;
		vector<int> res;
		for (; x; x >>= 1) res.push_back(x);
		for (int i = 0; i < res.size(); i++) 
			cout << heap[res[i]] << " \n"[i == res.size() - 1];
	}
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