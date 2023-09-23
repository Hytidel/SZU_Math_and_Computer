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
const double eps = 1e-7;
const double pi = acos(-1);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;

class Node {
	friend class LinkList;

private:
	string name;
	Node* nxt;

public:
	Node() :nxt(npt) {}
};

class LinkList {
private:
	Node* head;
	int n;

public:
	LinkList() { head = new Node; }

	void creat(int _n) {
		Node* p = head;
		n = _n;

		while (_n--) {
			Node* tmp = new Node;
			cin >> tmp->name;
			p->nxt = tmp;
			p = tmp;
		}
	}

	Node* find(string a) {
		Node* p = head;
		while (p->nxt) {
			if (p->nxt->name == a) return p;
			p = p->nxt;
		}
		return npt;
	}
};

int main() {
	// IOS;
#ifndef ONLINE_JUDGE
	clock_t my_clock = clock();
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
#endif
	// ----------------------------------------------------------------
	int n; cin >> n;
	LinkList ll;
	ll.creat(n);

	CaseT{
		string a; cin >> a;
		if (ll.find(a)) cout << "ok." << endl;
		else cout << "no find." << endl;
	}
	// ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
	cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
	return 0;
}