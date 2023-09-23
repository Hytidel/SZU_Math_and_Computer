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
struct Student {
	string id, name;
	int score;

	friend ostream& operator<< (ostream & out, const Student & p) {
		out << p.id << ' ' << p.name << ' ' << p.score;
		return out;
	}
};

void solve() {
	int n, op; cin >> n >> op;
	vector<Student> stus(n);
	for (auto& [id, name, score] : stus) cin >> id >> name >> score;

	if (op == 1) {
		sort(all(stus), [&](const Student& A, const Student& B) {
			return A.id < B.id;
			});
	}
	else if (op == 2) {
		sort(all(stus), [&](const Student& A, const Student& B) {
			return A.name != B.name ? A.name < B.name : A.id < B.id;
			});
	}
	else {
		sort(all(stus), [&](const Student& A, const Student& B) {
			return A.score != B.score ? A.score < B.score : A.id < B.id;
			});
	}

	for (auto stu : stus) cout << stu << endl;
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