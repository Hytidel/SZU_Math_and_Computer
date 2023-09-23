#include<bits/stdc++.h>
using namespace std;
#define endl '\n'

void solve() {
    int n, m; cin >> n >> m;
    priority_queue<int> heap;
    while (n--) {
        int x; cin >> x;
        heap.push(x);
    }

    vector<int> ans;
    while (m--) {
        ans.push_back(heap.top());
        heap.pop();
    }
    for (int i = 0; i < ans.size(); i++) cout << ans[i] << " \n"[i == ans.size() - 1];
}

int main() {
	cin.tie(0), cout.tie(0)->sync_with_stdio(false);
	solve();
	return 0;
}