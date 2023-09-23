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

struct Student {
	string num;
	int score;
	int final_rank;
	int room;
	int room_rank;

	bool operator<(const Student& B)const { 
		return score != B.score ? score > B.score : num < B.num;
	}

	void print() {
		cout << num << ' ' << final_rank << ' ' << room << ' ' << room_rank << endl;
	}
};
vector<Student> tot;

void solve() {
	int n; cin >> n;
	for (int i = 1; i <= n; i++) {
		vector<Student> stus;
		CaseT {
			string a; int b; cin >> a >> b;
			stus.push_back({ a,b,0,i,0 });
		}
		sort(all(stus));
		int rk = 1;
		stus[0].room_rank = rk;
		for (int j = 1; j < stus.size(); j++) {
			if (stus[j].score != stus[j - 1].score) stus[j].room_rank = ++rk;
			else stus[j].room_rank = rk++;
		}
		tot.insert(tot.end(), all(stus));
	}

	sort(all(tot));
	int rk = 1;
	tot[0].final_rank = rk;
	for (int j = 1; j < tot.size(); j++) {
		if (tot[j].score != tot[j - 1].score) tot[j].final_rank = ++rk;
		else tot[j].final_rank = rk++;
	}
	
	cout << tot.size() << endl;
	for (auto i : tot) i.print();
}

int main() {
	IOS;
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