#include<bits/stdc++.h>
using namespace std;
#define endl '\n'

void solve() {
	int n; cin >> n;
	vector<int> a(n), b(n);
	for (auto& ai : a) cin >> ai;
	for (auto& bi : b) cin >> bi;

	int i, j;
	for (i = 0; i + 1 < n && b[i] <= b[i + 1]; i++);
	for (j = i + 1; j < n && a[j] == b[j]; j++);

	if (j == n) {
		cout << "Insertion Sort" << endl;
		sort(a.begin(), a.begin() + i + 2);
	}
	else {
		cout << "Merge Sort" << endl;
		int len = 1;  // 当前归并长度
		while (true) {
			bool ok = true;
			for (int i = 0; i < n; i++) {
				if (a[i] != b[i]) {
					ok = false;
					break;
				}
			}

			len <<= 1;
			for (int i = 0; i < n / len; i++) 
				sort(a.begin() + i * len, a.begin() + (i + 1) * len);
			sort(a.begin() + n / len * len, a.end());

			if (ok) break;
		}
	}
	
	for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
    solve();
    return 0;
}