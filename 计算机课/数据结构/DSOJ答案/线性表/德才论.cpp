#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

struct Student {
	string id;
	int de, cai;

	void print() {
		cout << id << ' ' << de << ' ' << cai << endl;
	}
};
vector<Student> group1, group2, group3, group4;

bool cmp(const Student& a, const Student& b) {
	int res1 = a.de + a.cai, res2 = b.de + b.cai;
	if (res1 != res2) return res1 > res2;
	else if (a.de != b.de) return a.de > b.de;
	else return a.id < b.id;
}

void solve() {
	int n, l, h; cin >> n >> l >> h;
	while (n--) {
		string a; int b, c; cin >> a >> b >> c;
		if (b < l || c < l) continue;
		else if (b >= h && c >= h) group1.push_back({ a,b,c });
		else if (b >= h && c < h) group2.push_back({ a,b,c });
		else if (b < h && c < h && b >= c) group3.push_back({ a,b,c });
		else group4.push_back({ a,b,c });
	}

	sort(all(group1), cmp), sort(all(group2), cmp), sort(all(group3), cmp), sort(all(group4), cmp);
	cout << group1.size() + group2.size() + group3.size() + group4.size() << endl;
	for (auto i : group1) i.print();
	for (auto i : group2) i.print();
	for (auto i : group3) i.print();
	for (auto i : group4) i.print();
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