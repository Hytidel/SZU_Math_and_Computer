#include<iostream>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'

const int MAXN = 1005;
int n;
int a[MAXN];

void print() {
    for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];
}

void shellSort() {
    int gap = n;
    while (gap > 1) {
        gap /= 2;
        for (int i = gap; i < n; i++) {
            int tmp;
            if (a[i - gap] < a[i]) {
                tmp = a[i];
                int j = i - gap;
                while (j >= 0 && a[j] < tmp) {
                    a[j + gap] = a[j];
                    j -= gap;
                }
                a[j + gap] = tmp;
            }
        }
        print();
    }
}

void solve() {
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];

    shellSort();
    cout << endl;
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
    CaseT
    solve();
    return 0;
}