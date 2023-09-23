#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdlib>
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
#include<bitset>
#include<valarray>
#include<sstream>
#include<functional>
#include<cassert>
#include<random>
#include<numeric>
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
const double eps = 1e-8;
const double pi = acos(-1.0);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;
// const int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
// ----------------------------------------------------------------
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll lcm(ll a, ll b) { return a * b / gcd(a, b); }
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

ll qpow(ll a, ll k) {  // 注意可能爆ll
    ll res = 1;
    while (k) {
        if (k & 1) res = res * a;
        k >>= 1;
        a *= a;
    }
    return res;
}
// ----------------------------------------------------------------

class Node {
	friend class Linklist;
private:
	int data;
	Node* nxt;

public:
	Node():data(-INF) { nxt = npt; }
    Node(int x) :data(x) { nxt = npt; }
};

int ans;  // 非空指针改变次数

class Linklist {
private:
	Node* head;
	int length;

public:
	Linklist() :length(0) { head = new Node; }

	~Linklist() {
		Node* p = head->nxt;
		while (p) {
			Node* tmp = p;
			p = p->nxt;
			delete tmp;
		}
	}

    int get_length() { return length; }

    void print() {  // 从前往后打印链表
        vi res;
        Node* cur = head->nxt;
        while (cur) {
            res.push_back(cur->data);
            cur = cur->nxt;
        }
        int n = res.size();
        for (int i = 0; i < n; i++) cout << res[i] << " \n"[i == n - 1];
    }

    void insert(int x) {
        Node* cur = head;
        while (cur->nxt && cur->nxt->data < x) cur = cur->nxt;
        Node* tmp = cur->nxt;
        Node* u = new Node(x);
        if (cur->nxt) ans++;
        cur->nxt = u, u->nxt = tmp;
        length++;
    }

    int find(int pos) {
        Node* cur = head;
        int cnt = 0;
        while (cnt != pos) cur = cur->nxt, cnt++;
        return cur->data;
    }

    int operator[](int x) {  // 需保证不越界
        return find(x);
    }
};

void solve() {
    Linklist lk;
    int n; cin >> n; 1;
    while (n--) {
        int x; cin >> x;
        lk.insert(x);
    }

    cout << "非空指针变化" << ans << "次" << endl;
    lk.print();

    int len = lk.get_length();
    CaseT{
        int pos; cin >> pos;
        if (pos < 1 || pos > len) cout << "error" << endl;
        else cout << lk[pos] << endl;
    }
}

int main() {
    IOS;
#ifndef ONLINE_JUDGE
    clock_t my_clock = clock();
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    // ----------------------------------------------------------------	
    // init();
    // CaseT  // 单测时注释掉该行
    solve();
    // ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
    cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
    return 0;
}