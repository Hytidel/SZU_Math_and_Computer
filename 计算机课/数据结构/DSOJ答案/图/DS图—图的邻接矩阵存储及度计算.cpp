#include<iostream>
#include<cstring>
using namespace std;
#define IOS ios::sync_with_stdio(0); cin.tie(0), cout.tie(0);
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define so sizeof
#define endl '\n'
const int INF = 0x3f3f3f3f;
// ----------------------------------------------------------------

const int MAXN = 1005;
int n;
int dis[MAXN][MAXN];
char str[MAXN][MAXN];
int d[MAXN], in[MAXN], out[MAXN];

int get(char* s) {
    for (int i = 1; i <= n; i++)
        if (!strcmp(str[i], s)) return i;
}

void solve() {
    memset(dis, 0, so(dis));
    memset(d, 0, so(d));
    memset(in, 0, so(in));
    memset(out, 0, so(out));
    
    char ty; cin >> ty >> n;
    for (int i = 1; i <= n; i++) cin >> str[i];
    
    int m; cin >> m;
    while (m--) {
        char uu[MAXN], vv[MAXN]; cin >> uu >> vv;
        int u = get(uu), v = get(vv);
        dis[u][v] = 1;
        if (ty == 'U') dis[v][u] = 1;
    }
    
    for (int i = 1; i <= n; i++) 
        for (int j = 1;j <= n; j++) cout << dis[i][j] << " \n"[j == n];
        
    if (ty == 'D') {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++)
                if (dis[i][j]) in[j]++, out[i]++;
        }
        
        for (int i = 1; i <= n; i++) {
            cout << str[i];
            if (in[i] || out[i]) cout << ": " << out[i] << ' ' << in[i] << ' ' << in[i] + out[i] << endl;
            else cout << endl;
        }
    }
    else {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++)
                if (dis[i][j]) d[i]++;
        }
        
        for (int i = 1; i <= n; i++) {
            cout << str[i];
            if (d[i]) cout << ": " << d[i] << endl;
            else cout << endl;
        }
    }
}

int main() {
    IOS;
    CaseT  // 单测时注释掉该行
    solve();
    return 0;
}