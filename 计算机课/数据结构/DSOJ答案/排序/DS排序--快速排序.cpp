#include<iostream>
using namespace std;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'

const int MAXN = 1005;
int n;
int a[MAXN];

void print() {
    for (int i = 1; i <= n; i++) cout << a[i] << " \n"[i == n];
}

void quickSort(int l, int r) {
    if (l >= r) return;

    int pivot = a[l], st = l, ed = r;
    while (l < r) {
        for (int i = r; i > l; i--, r--) {
            if (a[i] < pivot) {
                a[l++] = a[i];
                break;
            }
        }

        for (int i = l; i < r; i++, l++) {
            if (a[i] > pivot) {
                a[r--] = a[i];
                break;
            }
        }
    }

    a[l] = pivot;
    print();
    quickSort(st, l - 1), quickSort(l + 1, ed);
}

void solve() {
    cin >> n;
    for (int i = 1; i <= n; i++) cin >> a[i];

    quickSort(1, n);
    cout << endl;
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
    CaseT
    solve();
    return 0;
}