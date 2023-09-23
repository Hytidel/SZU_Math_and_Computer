#include<iostream>
#include<cstring>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)

const int MAXN = 1005;
int n;
bool d[MAXN][MAXN];

void init() {
    memset(d, 0, sizeof(d));
    for (int i = 1; i <= n; i++) d[i][i] = true;
}

void floyd() {
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++)
                d[i][j] |= d[i][k] & d[k][j];
        }
    }
}

void solve() {
    cin >> n;
    init();
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) cin >> d[i][j];
        
    floyd();
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (!d[i][j]) {
                cout << "No" << endl;
                return;
            }
        }
    }
    cout << "Yes" << endl;
}

int main() {
    CaseT solve();
    return 0;
}