#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<complex>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<bitset>
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
typedef vector<pii> vii;
typedef vector<pll> vll;
typedef pair<double, double> pdd;
typedef complex<double> cp;
#define umap unordered_map
#define IOS ios::sync_with_stdio(0); cin.tie(0), cout.tie(0);
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define so sizeof
#define pb push_back
#define npt nullptr
const double eps = 1e-7;
const double pi = acos(-1);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;

class Node {
	friend class Linklist;

private:
	int data;
	bool die;
	Node* nxt;

public:
	Node() { die = false, nxt = npt; }
};

class Linklist {
private:
	Node* head;
	Node* tail;

public:
	void init() {
		head = new Node;
		head->nxt = npt;
		tail = head;
	}
	
	void insert(int x) {
		Node* p = new Node;
		p->data = x;
		p->nxt = npt;
		tail->nxt = p;
		tail = p;
	}

	void solve(int n, int k, int m) {  // 人数、开始位置、死亡数字
		tail->nxt = head->nxt;  // 循环链表

		Node* p = head->nxt;
		for (int i = 1; i < k; i++) p = p->nxt;  // p指向开始位置
		int life = n;  // 还活着的人数
		int cur = 1;  // 当前报数
		while (1) {
			if (cur == m) {  // 死人
				cur = 0;
				p->die = true;
				life--;
				cout << p->data;
				if (life) cout << ' ';
				else {
					cout << endl;
					break;
				}
			}

			p = p->nxt;
			while (p->die) p = p->nxt;
			cur++;
		}
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
	int n, k, m;
	while (cin >> n >> k >> m) {
		Linklist link;
		link.init();
		for (int i = 1; i <= n; i++) link.insert(i);
		link.solve(n, k, m);
	}
	// ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
	cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
	return 0;
}