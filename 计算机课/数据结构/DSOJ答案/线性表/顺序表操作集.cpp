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

template<class T>
class Linnerlist {
private:
	int capacity; // 最大容量
	int length;  // 当前长度
	T* arr;

public:
	Linnerlist() :capacity(100), length(0), arr(npt) {}

	// ~Linnerlist() { if (arr) delete[] arr; }

	void set_capacity(int n) {  // 设置最大容量
		capacity = n;
		T* tmp = new T[capacity];
		memcpy(tmp, arr, so(T) * length);
		delete[] arr;
		arr = tmp;
	}

	int get_length() { return length; }

	T& operator[](int idx) {  // 支持通过[]访问线性表元素
		assert(idx < length);
		if (idx < length) return arr[idx];
	}

	void create(int n, int maxlength = 0) {  // 新建一个长度为n的线性表,最大长度
		if (!maxlength) maxlength = 2 * n;  // 最大长度默认为n的2倍
		capacity = maxlength, length = n;
		arr = new T[capacity];
	}

	void input() {  // 输入线性表元素
		for (int i = 0; i < length; i++) cin >> arr[i];
	}

	void print() {  // 打印线性表
		for (int i = 0; i < length; i++) cout << arr[i] << " \n"[i == length - 1];
	}

	void sort(int l, int r, int op = 1) {  // 对[l,r)的元素排序,op=1时为升序,op=0时为降序
		if (op) std::sort(arr + l, arr + r);
		else std::sort(arr + l, arr + r, greater<T>());
	}

	bool InsertToPos(T val, int pos) {  // 在下标为pos的地方插入数据val
		if (length == capacity) return false;  // 空间不足

		for (int i = length - 1; i >= pos; i--) arr[i + 1] = arr[i];
		arr[pos] = val;
		length++;
		return true;
	}

	int FindByData(T val) {  // 查找是否存在数值为val的元素
		for (int i = 0; i < length; i++)
			if (arr[i] == val) return i;
		return -1;
	}

	bool RemoveByPos(int pos) {  // 删除下标为pos的数据
		if (pos < 0 || pos >= length) return false;  // 下标越界

		for (int i = pos; i < length; i++) arr[i] = arr[i + 1];
		length--;
		return true;
	}

	bool RemoveByData(T val) {  // 删除第一个数值为val的元素
		for (int i = 0; i < length; i++) {
			if (arr[i] == val) {
				RemoveByPos(i);
				return true;
			}
		}
		return false;  // 不存在数值为val的元素
	}
};

void solve() {
	Linnerlist<int> l;
	l.create(0);
	l.set_capacity(5);

	int n; cin >> n;
	stack<int> stk;
	while (n--) {
		int x; cin >> x;
		stk.push(x);
	}
	while (stk.size()) {
		int x = stk.top(); stk.pop();
		if (!l.InsertToPos(x, l.get_length()))
			printf("FULL Insertion Error: %d is not in.\n", x);
	}

	l.print();

	cin >> n;
	while (n--) {
		int x; cin >> x;
		int res = l.FindByData(x);
		if (~res) printf("%d is at position %d.\n", x, res);
		else printf("Finding Error: %d is not in.\n", x);
			
	}

	cin >> n;
	while (n--) {
		int x; cin >> x;
		if (!l.RemoveByPos(x)) printf("POSITION %d EMPTY Deletion Error.\n", x);
		

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