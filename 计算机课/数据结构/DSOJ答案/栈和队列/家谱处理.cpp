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

const int MAXN = 4e4 + 5, MAXM = MAXN << 1, MAXJ = 15;  // 注意15是否够
int n;
umap<string, int> mp;
int idx = 0;
vii a;  // 名字、所在层数
vi edges[MAXN];
int depth[MAXN];  // 节点的深度
int fa[MAXN][MAXJ + 1];  // fa[i][j]表示节点i向上跳2^j步到达的节点

int get(string s) {
	if (mp.count(s)) return mp[s];
	else return mp[s] = ++idx;
}

int get_space(string s) {
	int res = 0;
	for (auto ch : s) {
		if (ch == ' ') res++;
		else break;
	}
	return res;
}

int get_level(string s) {
	return get_space(s) / 2 + 1;
}

void build() {
	for (int i = 1; i < a.size(); i++) {
		int fa = 1;
		for (int j = i - 1; j >= 1; j--) {
			if (a[j].second == a[i].second - 1) {
				fa = j + 1;
				break;
			}
		}
		edges[fa].push_back(a[i].first), edges[a[i].first].push_back(fa);
	}
}

void bfs(int root) {  // 预处理depth[]和fa[][]
	memset(depth, INF, so(depth));
	depth[0] = 0;  // 哨兵
	depth[root] = 1;  // 初始条件

	qi que;
	que.push(root);
	while (que.size()) {
		auto tmp = que.front(); que.pop();
		for (auto j : edges[tmp]) {
			if (depth[j] > depth[tmp] + 1) {
				depth[j] = depth[tmp] + 1;
				que.push(j);
				fa[j][0] = tmp;
				for (int k = 1; k <= MAXJ; k++)
					fa[j][k] = fa[fa[j][k - 1]][k - 1];
			}
		}
	}
}

int LCA(int a, int b) {  // 求节点a和b的LCA
	if (depth[a] < depth[b]) swap(a, b);  // 保证节点a在下

	for (int k = MAXJ; k >= 0; k--)  // a与b跳到同层
		if (depth[fa[a][k]] >= depth[b]) a = fa[a][k];

	if (a == b) return a;

	for (int k = MAXJ; k >= 0; k--)  // 两节点同时向上跳至LCA的下一层
		if (fa[a][k] != fa[b][k]) a = fa[a][k], b = fa[b][k];
	return fa[a][0];  // 节点a向上跳1步到LCA
}

void solve() {
	int n, m; cin >> n >> m;
	getchar();
	for (int i = 0; i < n; i++) {
		string line; getline(cin, line);
		a.push_back({ get(line.substr(get_space(line))),get_level(line) });
	}

	/*for (auto i : a) cout << i.first << ' ' << i.second << endl;
	cout << endl;*/

	build();
	bfs(1);

	/*for (int i = 1; i <= n; i++) {
		for (auto u : edges[i]) cout << u << ' ';
		cout << endl;
	}*/

	while (m--) {
		int op1 = 1, op2 = 1;
		string name; cin >> name;
		// cout << name << endl;
		op1 = get(name);
		cin >> name >> name;
		string type; cin >> type;
		cin >> name >> name;
		// cout << name << endl;
		op2 = get(name);

		int lca = LCA(op1, op2);
		// cout << op1 << ' ' << op2 << ' ' << lca << endl;
		if (type == "child") {
			if (op2 == lca && depth[op2] + 1 == depth[op1]) cout << "True" << endl;
			else cout << "False" << endl;
		}
		else if (type == "ancestor") {
			if (op1 == lca) cout << "True" << endl;
			else cout << "False" << endl;
		}
		else if (type == "sibling") {
			if (depth[op1] == depth[op2] && depth[lca] + 1 == depth[op1]) cout << "True" << endl;
			else cout << "False" << endl;
		}
		else if (type == "parent") {
			swap(op1, op2);
			if (op1 == lca && depth[op1] + 1 == depth[op2]) cout << "True" << endl;
			else cout << "False" << endl;
		}
		else {  // descedant
			if (op2 == lca) cout << "True" << endl;
			else cout << "False" << endl;
		}
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
	// CaseT  // 单测时注释掉该行
	solve();
	// ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
	cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
	return 0;
}