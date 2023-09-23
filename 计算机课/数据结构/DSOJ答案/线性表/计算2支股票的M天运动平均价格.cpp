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
#include<unordered_set>
#include<unordered_map>
#include<bitset>
#include<valarray>
#include<sstream>
#include<functional>
#include<cassert>
using namespace std;
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<bool> vb;
typedef queue<int> qi;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef pair<double, double> pdd;
typedef tuple<int, int, int> tiii;
typedef vector<pii> vii;
typedef vector<pll> vll;
typedef queue<pii> qii;
typedef complex<double> cp;
#define umap unordered_map
#define uset unordered_set
#define pque priority_queue
#define IOS ios::sync_with_stdio(0); cin.tie(0), cout.tie(0);
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define npt nullptr
#define so sizeof
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define hashset_finetune(p) (p).reserve(1024); (p).max_load_factor(0.25);  // 哈希表防卡
const double eps = 1e-7;
const double pi = acos(-1.0);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;
const int dx[4] = { 0,-1,0,1 }, dy[4] = { -1,0,1,0 };
// ----------------------------------------------------------------
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }  // 注意要不要改成ll
ll lcm(ll a, ll b) { return a * b / gcd(a, b); }  // 注意要不要改成ll
ll lowbit(ll x) { return x & (-x); }
int cmp(double a, double b) {
	if (fabs(a - b) < eps) return 0;
	else if (a > b) return 1;
	else return -1;
}
int sgn(double x) {
	if (fabs(x) < eps) return 0;
	else if (x > 0) return 1;
	else return -1;
}

//template<typename T>
//void read(T& x) {
//	x = 0;
//	T sgn = 1;
//	char ch = getchar();
//	while (ch < '0' || ch > '9') {
//		if (ch == '-') sgn = -1;
//		ch = getchar();
//	}
//	while (ch >= '0' && ch <= '9') {
//		x = (x << 3) + (x << 1) + (ch & 15);
//		ch = getchar();
//	}
//	x *= sgn;
//}
//
//template<typename T>
//void write(T x) {
//	if (x < 0) {
//		putchar('-');
//		x *= -1;
//	}
//	if (x < 10) putchar(x + 48);
//	else write(x / 10), putchar(x % 10 + 48);
//}

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

const int mday[15] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

class Date {
public:
	int y, m, d;

	Date() {}
	Date(int _y, int _m, int _d) :y(_y), m(_m), d(_d) {}

	bool operator<(const Date& B)const {
		if (y != B.y) return y < B.y;
		else if (m != B.m) return m < B.m;
		else return d < B.d;
	}

	bool is_leap(int year) { return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); }

	Date back() {  // 向前数1天
		Date res(*this);
		if (res.d != 1) res.d--;
		else if (res.m != 1) {
			res.m--;
			if (res.m != 2) res.d = mday[m];
			else res.d = is_leap(res.y) ? 29 : 28;
		}
		else {
			res.y--, res.m = 12, res.d = 31;
		}
		return res;
	}

	Date front() {  // 向后数1天
		Date res(*this);

		if (res.m == 2 && res.d == 29) res.m++, res.d = 1;
		else if (res.d == mday[res.m]) {
			if (res.m == 12) res, y++, res.m = 1, res.d = 1;
			else if (res.m != 2) res.m++, res.d = 1;
			else {
				if (res.d == 29) res.m++, res.d = 1;
				else if (is_leap(res.y)) res.d++;
				else res.m++, res.d = 1;
			}
		}
		else res.d++;
		return res;
	}

	void print() { cout << y << ' ' << m << ' ' << d << endl; }

	friend ostream& operator<<(ostream& stream, Date& p) {
		stream << p.y << '/' << p.m << '/' << p.d;
		return stream;
	}
};

class Stock {
public:
	Date day;
	string state;
	int s1, s2;

	Stock() {}
	Stock(int a, int b, int c, string d, int e, int f) :day(a, b, c), state(d), s1(e), s2(f) {}

	bool operator<(const Stock& B)const {
		if (state == "open" && B.state == "open") return day < B.day;
		else if (state == "open" && B.state == "close") return true;
		else if (state == "close" && B.state == "open") return false;
		else return day < B.day;
	}

	void print() {
		cout << day << ' ' << state << ' ';
	}
};

const int MAXN = 1e5 + 5;
int pre1[MAXN], pre2[MAXN];

void solve() {
	int n, m; cin >> n >> m;
	vector<Stock> arr(n + 1);
	for (int i = 1; i <= n; i++) {
		int y, m, d; scanf("%d/%d/%d", &y, &m, &d);
		string str; cin >> str;
		int s1, s2; cin >> s1 >> s2;
		arr[i] = { y, m, d, str, s1, s2 };
	}

	sort(arr.begin() + 1, arr.end());

	for (int i = 1; i <= n; i++) {
		pre1[i] = pre1[i - 1] + arr[i].s1;
		pre2[i] = pre2[i - 1] + arr[i].s2;
	}

	bool flag = false;
	for (int i = m; i <= n; i++) {
		if (arr[i].state == "close" && !flag) {
			flag = true;
			continue;
		}

		int res1 = (pre1[i] - pre1[i - m]) / m, res2 = (pre2[i] - pre2[i - m]) / m;
		arr[i].print();
		cout << res1 << ' ' << res2 << endl;
	}
}

int main() {
	// IOS;
#ifndef ONLINE_JUDGE
	clock_t my_clock = clock();
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
#endif
	// ----------------------------------------------------------------	
	// CaseT  // 单测时注释掉该行
	solve();
	// ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
	cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
	return 0;
}