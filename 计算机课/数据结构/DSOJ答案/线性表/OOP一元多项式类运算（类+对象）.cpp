#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<sstream>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef queue<int> qi;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef pair<double, double> pdd;
#define umap unordered_map
#define IOS ios::sync_with_stdio(0); cin.tie(0), cout.tie(0);
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define so sizeof
#define pb push_back
#define npt nullptr
const double eps = 1e-8;
const double pi = acos(-1);
const int INF = 0x3f3f3f3f;  // 十六进制数,int的无穷大
const ll INFF = 0x3f3f3f3f3f3f3f3f;

class Polynomial {
public:
	int n;  // 多项式的最高次
	vector<pii> coes;  // 系数及其指数

	Polynomial() {}
	Polynomial(int _n) :n(_n) {}

	void read() {
		int m; cin >> m;
		while (m--) {
			int c, s; cin >> c >> s;  // 系数、指数
			n = s;
			coes.push_back(make_pair(c, s));
		}
	}

	void write() {
		string ans;
		for (auto items : coes) {
			if (items.first == 0) continue;

			if (items.first > 0) {
				if (items.first != 1) ans += "+" + to_string(items.first);
				else if (items.second == 0) ans += "1";
				else ans += "+";
			}
			else {
				if (items.first != -1) ans += "-" + to_string(abs(items.first));
				else if (items.second == 0) ans += "-1";
				else ans += "-";
			}

			if (items.second == 0) continue;
			else if (items.second == 1) ans += "x";
			else ans += "x^" + to_string(items.second);
		}

		if (ans == "") cout << 0 << endl;
		else if (ans[0] == '-' && ans[1] == '-') cout << ans.substr(1) << endl;
		else if (ans[0] == '+') cout << ans.substr(1) << endl;
		else cout << ans << endl;
	}
};

Polynomial add(Polynomial A, Polynomial B) {
	int tmp = max(A.n, B.n);
	Polynomial c(tmp);

	for (int i = 0; i <= tmp; i++) {  // 从小到大枚举指数
		int idx1 = 0, idx2 = 0;  // 记录项的下标
		bool flag1 = 0, flag2 = 0;  // 记录项是否存在

		// 查找a、b中是否有对应次数的项
		for (auto ai : A.coes) {
			if (ai.second == i) {
				flag1 = 1;
				break;
			}
			idx1++;
		}
		for (auto bi : B.coes) {
			if (bi.second == i) {
				flag2 = 1;
				break;
			}
			idx2++;
		}

		if (flag1 && flag2) {
			c.coes.push_back(make_pair(A.coes[idx1].first + B.coes[idx2].first, i));
			continue;
		}
		if (flag1) c.coes.push_back(make_pair(A.coes[idx1].first, i));
		if (flag2) c.coes.push_back(make_pair(B.coes[idx2].first, i));
	}

	return c;
}

Polynomial sub(Polynomial A, Polynomial B) {
	int tmp = max(A.n, B.n);
	Polynomial c(tmp);

	for (int i = 0; i <= tmp; i++) {  // 从小到大枚举指数
		int idx1 = 0, idx2 = 0;  // 记录项的下标
		bool flag1 = 0, flag2 = 0;  // 记录项是否存在

		// 查找a、b中是否有对应次数的项
		for (auto ai : A.coes) {
			if (ai.second == i) {
				flag1 = 1;
				break;
			}
			idx1++;
		}
		for (auto bi : B.coes) {
			if (bi.second == i) {
				flag2 = 1;
				break;
			}
			idx2++;
		}

		if (flag1 && flag2) {
			c.coes.push_back(make_pair(A.coes[idx1].first - B.coes[idx2].first, i));
			continue;
		}
		if (flag1) c.coes.push_back(make_pair(A.coes[idx1].first, i));
		if (flag2) c.coes.push_back(make_pair(-B.coes[idx2].first, i));
	}

	return c;
}

Polynomial mul(Polynomial A, Polynomial B) {
	int tmp = A.n + B.n;
	Polynomial c(tmp);

	int coa, cob;  // a的系数和b的系数
	set<int> p;  // 存已计算出的系数
	for (int i = 0; i <= tmp; i++) {  // 从小到大枚举次数
		for (auto ai : A.coes) {
			if (ai.second > i) break;

			coa = ai.first;
			int target = i - ai.second;  // B对应的次数
			int flag = 0;  // 记录B是否存在对应的次数
			for (auto bi : B.coes) {
				if (bi.second > target) break;
				if (bi.second == target) {
					cob = bi.first;
					flag = 1;
					break;
				}
			}

			if (flag) {
				if (!p.count(i)) {
					c.coes.push_back(make_pair(coa * cob, i));
					p.insert(i);
				}
				else {
					for (auto& item : c.coes) {
						if (item.second == i) {
							item.first += coa * cob;
							break;
						}
					}
				}
			}
		}
	}

	return c;
}

void work() {
	Polynomial p1, p2;
	p1.read(), p2.read();
	// p1.write(), p2.write();
	add(p1, p2).write();
	sub(p1, p2).write();
	mul(p1, p2).write();
}

int main() {
	IOS;
#ifndef ONLINE_JUDGE
	clock_t my_clock = clock();
//	freopen("in.txt", "r", stdin);
//	freopen("out.txt", "w", stdout);
#endif
	// ----------------------------------------------------------------
	CaseT{
		work();
	}
	// ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
	cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
	return 0;
}