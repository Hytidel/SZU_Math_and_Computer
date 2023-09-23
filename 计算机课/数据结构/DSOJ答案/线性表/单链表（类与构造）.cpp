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
	Node* nxt;

public:
	Node() { nxt = npt; }
};

class Linklist {
private:
	Node* head;
	int length;

public:
	Linklist() :length(0) { head = new Node; }

	void create(int n) {  // 新建一个长度为n的链表
		length = n;
		Node* p = head;
		for (int i = 1; i <= n; i++) {
			Node* now = new Node;
			cin >> now->data;
			p->nxt = now;
			p = now;
		}
	}

	void InsertToHead(int d) {  // 头插
		Node* now = new Node;
		now->data = d;
		now->nxt = head->nxt;
		head->nxt = now;
		length++;
	}

	void InsertToTail(int d) {  // 尾插
		Node* p = head;
		while (p->nxt) p = p->nxt;
		Node* now = new Node;
		now->data = d;
		p->nxt = now;
		length++;
	}

	void InsertToPos(int d, int pos) {  // 插到第pos个节点之后,头节点是第0个节点
		if (pos > length) {
			cout << "error" << endl;
			return;  // 不存在该节点
		}

		Node* p = head;
		int cnt = 0;
		while (cnt != pos) p = p->nxt, cnt++;
		Node* now = new Node;
		now->data = d;
		now->nxt = p->nxt;
		p->nxt = now;
		length++;
		print();
	}

	Node* FindByPos(int pos) {  // 找到第pos个节点
		if (pos >= length) return npt;  // 不存在第pos个节点

		Node* p = head;
		int cnt = 0;
		while (cnt != pos) p = p->nxt, cnt++;
		return p;
	}

	Node* FindByData(int d) {  // 找到第一个数据为d的节点
		Node* p = head;
		while (p->nxt) {
			if (p->nxt->data == d) return p;
			p = p->nxt;
		}
		return npt;  // 不存在数据为d的节点
	}

	void RemoveByPos(int pos) {  // 删除第pos个节点
		pos--;
		Node* p = FindByPos(pos);
		if (!p) {
			cout << "error" << endl;
			return;  // 不存在数据为d的节点
		} 

		Node* trash = p->nxt;
		p->nxt = trash->nxt;
		delete trash;
		length--;
		print();
	}

	void RemoveByData(int d) {  // 删除第一个数据为d的节点
		Node* p = FindByData(d);
		if (!p) return;  // 不存在数据为d的节点

		Node* trash = p->nxt;
		p->nxt = trash->nxt;
		delete trash;
		length--;
	}

	void print() {  // 从前往后打印链表
		string res = "";
		Node* p = head->nxt;
		while (p) {
			res += to_string(p->data) + " ";
			p = p->nxt;
		}
		cout << res.substr(0, res.length() - 1) << endl;
	}

	int get_length() { return length; }

	~Linklist() {
		Node* p = head->nxt;
		while (p) {
			Node* tmp = p;
			p = p->nxt;
			delete tmp;
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
	CaseT{
		int n; cin >> n;
		Linklist link;
		link.create(n);
		link.print();
		int m; cin >> m;
		while (m--) {
			int pos, val; cin >> pos >> val;
			link.InsertToPos(val, pos);
		}
		
		int k; cin >> k;
		while (k--) {
			int pos; cin >> pos;
			link.RemoveByPos(pos);
		}
	}
	// ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
	cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
	return 0;
}