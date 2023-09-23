#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<complex>
#include<array>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<list>
#include<unordered_set>
#include<unordered_map>
#include<sstream>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef queue<int> qi;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<pii> vii;
typedef vector<pll> vll;
typedef pair<double, double> pdd;
typedef complex<double> cp;
#define umap unordered_map
#define IOS ios::sync_with_stdio(0); cin.tie(0), cout.tie(0);
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define so sizeof
// #define pb push_back
#define npt nullptr
const double eps = 1e-7;
const double pi = acos(-1);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;
// ----------------------------------------------------------------
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }  // 注意要不要改成ll
int lcm(int a, int b) { return a * b / gcd(a, b); }  // 注意要不要改成ll
int lowbit(int x) { return x & (-x); }
int cmp(double a, double b) {
	if (fabs(a - b) < eps) return 0;
	else if (a > b) return 1;
	else return -1;
}

template<typename T>
void read(T& x) {
	x = 0;
	T sgn = 1;
	char ch = getchar();
	while (ch < '0' || ch > '9') {
		if (ch == '-') sgn = -1;
		ch = getchar();
	}
	while (ch >= '0' && ch <= '9') {
		x = (x << 3) + (x << 1) + (ch & 15);
		ch = getchar();
	}
	x *= sgn;
}

template<typename T>
void write(T x) {
	if (x < 0) {
		putchar('-');
		x *= -1;
	}
	if (x < 10) putchar(x + 48);
	else write(x / 10), putchar(x % 10 + 48);
}

ll qpow(ll a, ll k, ll MOD) {
	ll res = 1;
	while (k) {
		if (k & 1) res = res * a % MOD;
		k >>= 1;
		a = a * a % MOD;
	}
	return res;
}
// ----------------------------------------------------------------

class PhoneBook;

class Info {
	string name;
	int phoneNo;

public:
	Info(string tn = "no name", int pno = 0) { name = tn, phoneNo = pno; }

	friend class PhoneBook;

	void print() {
		cout << name << "--" << phoneNo << endl;
	}
};

class PhoneBook {
	list<Info> pbs[26];

public:
	Info* find(string s) {
		int idx = s[0] - 'A';
		for (list<Info>::iterator it = pbs[idx].begin(); it != pbs[idx].end(); it++) {
			auto tmp = *it;
			if (tmp.name == s) return &(*it);
		}
		return npt;
	}

	Info* operator()(string s) { return find(s); }

	void insert(Info p) {
		auto tmp = find(p.name);
		if (tmp) tmp->phoneNo = p.phoneNo;
		else pbs[p.name[0] - 'A'].push_front(p);
	}

	void print() {
		for (int i = 0; i < 26; i++) {
			if (pbs[i].size()) {
				cout << (char)('A' + i) << "--";
				for (list<Info>::iterator it = pbs[i].begin(); it != pbs[i].end(); it++) 
					cout << (*it).name << '.' << (*it).phoneNo << "--";
				cout << endl;
			}
		}
	}

	void input() {
		int n; cin >> n;
		while (n--) {
			string na; int ph; cin >> na >> ph;
			Info tmp(na, ph);
			insert(tmp);
		}
	}

	void operator+=(Info& p) { insert(p); }

	PhoneBook operator+(PhoneBook& B) {
		for (int i = 0; i < 26; i++) {
			if (B.pbs[i].size()) {
				for (list<Info>::iterator it = B.pbs[i].begin(); it != B.pbs[i].end(); it++) insert(*it);
			}
		}
		return *this;
	}
};

int main() {
	IOS;
#ifndef ONLINE_JUDGE
	clock_t my_clock = clock();
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
#endif
	// ----------------------------------------------------------------
	string tname;
	int i, tno;
	Info* p;
	PhoneBook pb;
	pb.input(); //接收输入数据，初始化第一个通讯录

	//两次姓名查找
	for (i = 0; i < 2; i++)

	{
		cin >> tname;
		p = pb(tname); //调用()运算符，实现查找
		if (p) p->print(); //查找成功，输出联系人信息
		else cout << "查找失败" << endl; //查找失败，输出提示信息

	}

	//一次插入
	cin >> tname >> tno;
	Info temp(tname, tno);
	pb += temp; //调用+=运算符，实现插入新联系人

	//通讯录合并
	PhoneBook pc;
	pc.input(); //初始化第二个通讯录

	pb = pb + pc; //调用+运算符，实现合并
	pb.print(); //输出所有操作后的通讯录
	// ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
	cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
	return 0;
}