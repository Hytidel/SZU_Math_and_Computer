#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;
const double eps = 1e-8;

int cmp(double a, double b) {
	if (fabs(a - b) < eps) return 0;
	return a < b ? -1 : 1;
}

const int MAXN = 235;
struct Country {
	int id;
	int gold;
	int medal;
	int population;
	int rk[5];
}countries[MAXN];

void solve() {
	int n, m; cin >> n >> m;
	for (int i = 0; i < n; i++) {
		countries[i].id = i;
		cin >> countries[i].gold >> countries[i].medal >> countries[i].population;
	}

	sort(countries, countries + n, [&](const Country& a, const Country& b) {
		return a.gold > b.gold;
		});
	for (int i = 0; i < n; i++) {
		if (!i || countries[i].gold != countries[i - 1].gold) countries[i].rk[1] = i + 1;
		else countries[i].rk[1] = countries[i - 1].rk[1];
	}

	sort(countries, countries + n, [&](const Country& a, const Country& b) {
		return a.medal > b.medal;
		});
	for (int i = 0; i < n; i++) {
		if (!i || countries[i].medal != countries[i - 1].medal) countries[i].rk[2] = i + 1;
		else countries[i].rk[1] = countries[i - 1].rk[2];
	}

	sort(countries, countries + n, [&](const Country& a, const Country& b) {
		return (double)a.gold / a.population > (double)b.gold / b.population;
		});
	for (int i = 0; i < n; i++) {
		if (!i || cmp((double)countries[i].gold / countries[i].population, (double)countries[i - 1].gold / countries[i - 1].population)) 
			countries[i].rk[3] = i + 1;
		else countries[i].rk[3] = countries[i - 1].rk[3];
	}

	sort(countries, countries + n, [&](const Country& a, const Country& b) {
		return (double)a.medal / a.population > (double)b.medal / b.population;
		});
	for (int i = 0; i < n; i++) {
		if (!i || cmp((double)countries[i].medal / countries[i].population, (double)countries[i - 1].medal / countries[i - 1].population))
			countries[i].rk[4] = i + 1;
		else countries[i].rk[4] = countries[i - 1].rk[4];
	}

	while (m--) {
		int x; cin >> x;
		for (int i = 0; i < n; i++) {
			if (countries[i].id == x) {
				int minrk = INF, minidx = 0;
				for (int j = 1; j <= 4; j++) {
					if (countries[i].rk[j] < minrk) {
						minrk = countries[i].rk[j];
						minidx = j;
					}
				}
				cout << minrk << ':' << minidx << " \n"[!m];
				break;
			}
		}
	}
}

int main() {
	cin.tie(0), cout.tie(0)->sync_with_stdio(false);
#ifndef ONLINE_JUDGE
	freopen("in.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
#endif
	// init();
	// CaseT
	solve();
	return 0;
}