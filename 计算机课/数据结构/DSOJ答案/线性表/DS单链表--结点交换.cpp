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
#include<random>
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
const int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
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

class Node {
private:
	int data;
	Node* nxt;

public:
	friend class Linklist;

	Node() { nxt = npt; }

	int get_data() { return data; }
};

class Linklist {
private:
	Node* head;
	int length;

public:
	Linklist() :length(0) { head = new Node; }  // 头节点为第0个节点

	~Linklist() {
		Node* p = head->nxt;
		while (p) {
			Node* tmp = p;
			p = p->nxt;
			delete tmp;
		}
	}

	int get_length() { return length; }

	void create(int n) {  // 新建一个长度为n的链表
		length = n;
		Node* p = head;
		for (int i = 1; i <= n; i++) {
			Node* cur = new Node;
			cin >> cur->data;
			p->nxt = cur;
			p = cur;
		}
	}

	void print() {  // 打印链表
		string res = "";
		Node* p = head->nxt;
		while (p) {
			res += to_string(p->data) + " ";
			p = p->nxt;
		}
		cout << res.substr(0, res.length()) << endl;
	}

	bool InsertToPos(int val, int pos) {  // 插入到第pos个节点后,头节点为第0个节点,返回是否插入成功
		if (pos < 0 || pos > length) return false;  // 不存在该节点

		Node* p = head;
		int cnt = 0;
		while (cnt != pos) p = p->nxt, cnt++;  // p指向第pos个节点
		Node* cur = new Node;
		cur->data = val;
		cur->nxt = p->nxt;
		p->nxt = cur;
		length++;
		return true;
	}

	Node* FindByPos(int pos) {  // 返回第pos个节点的前驱节点
		if (pos < 0 || pos > length) return npt;  // 不存在该节点

		Node* p = head;
		int cnt = 1;
		while (cnt != pos) p = p->nxt, cnt++;
		return p;
	}

	bool swap(int pos1, int pos2) {  // 交换两节点
		if (pos1 <= 0 || pos1 > length || pos2 <= 0 || pos2 > length) return false;

		// 要交换的节点的前驱、要交换的节点
		auto pre1 = FindByPos(pos1), node1 = pre1->nxt;
		auto pre2 = FindByPos(pos2), node2 = pre2->nxt;

		pre1->nxt = node2;
		auto tmp = node2->nxt;
		node2->nxt = node1->nxt;
		pre2->nxt = node1;
		node1->nxt = tmp;
		return true;
	}
};

void solve() {
	int n; cin >> n;
	Linklist l;
	l.create(n);
	l.print();

	int pos1, pos2; cin >> pos1 >> pos2;
	if (l.swap(pos1, pos2)) l.print();
	else cout << "error" << endl;
	cin >> pos1 >> pos2;
	if (l.swap(pos1, pos2)) l.print();
	else cout << "error" << endl;
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