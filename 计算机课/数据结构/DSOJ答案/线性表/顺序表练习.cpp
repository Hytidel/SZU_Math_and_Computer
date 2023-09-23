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
int a[MAXN];

void print() {
	cout << n << ' ';
	for (int i = 1; i <= n; i++) cout << a[i] << ' ';
	cout << endl;
}

void solve() {
	cin >> n;
	for (int i = 1; i <= n; i++) cin >> a[i];
	print();

	int pos, val; cin >> pos >> val;
	if (pos < 1 || pos > n + 1) cout << "ERROR" << endl;
	else {
		for (int i = n; i >= pos; i--) a[i + 1] = a[i];
		n++;
		a[pos] = val;
		print();
	}

	cin >> pos;
	if (pos < 1 || pos > n) cout << "ERROR" << endl;
	else {
		for (int i = pos; i <= n; i++) a[i] = a[i + 1];
		n--;
		print();
	}

	cin >> val;
	for (pos = 1; pos <= n && a[pos] != val; pos++);
	if (pos == n + 1) cout << "ERROR" << endl;
	else {
		for (int i = pos; i <= n; i++) a[i] = a[i + 1];
		n--;
		print();
	}

	cin >> val;
	bool ok = false;
	for (int i = n; i >= 1; i--) {  // 注意从后往前查找
		if (a[i] == val) {
			cout << 1 << ' ' << i << ' ' << n - i + 1 << endl;
			ok = true;
			break;
		}
	}
	if (!ok) cout << 0 << ' ' << 0 << ' ' << n + 1 << endl;
}

int main() {
	cin.tie(0), cout.tie(0)->sync_with_stdio(false);
#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
#endif
	// init();
	CaseT
	solve();
	return 0;
}