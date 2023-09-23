#include<iostream>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)

const int MAXN = 1005;
namespace TopologicalSorting {
    int n;
    int d[MAXN][MAXN];
    int in[MAXN];
    int que[MAXN];

    void topo() {
        int hh = 0, tt = -1;
        for (int i = 0; i < n; i++)
            if (!in[i]) que[++tt] = i;

        while (hh <= tt) {
            int u = que[hh++];
            for (int i = 0; i < n; i++) {
                if (d[u][i]) 
                    if (--in[i] == 0) que[++tt] = i;
            }
        }
    }
    
}
using namespace TopologicalSorting;

void solve() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> d[i][j];
            if (d[i][j]) in[j]++; 
        }
    }

    topo();

    for (int i = 0; i < n; i++) cout << que[i] << ' ';
    cout << endl;
}

int main() {
    CaseT
    solve();
    return 0;
}