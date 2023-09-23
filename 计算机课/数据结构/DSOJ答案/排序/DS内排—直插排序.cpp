#include<iostream>
using namespace std;
#define endl '\n'

const int MAXN = 1005;
int n;
int a[MAXN];

void print() {
    for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];
}

void insertSort() {
    for (int i = 1; i < n; i++) {
        for (int j = i - 1; j >= 0 && a[j] > a[j + 1]; j--) 
            swap(a[j], a[j + 1]);
        print();
    }
}

void solve() {
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];

    insertSort();
    cout << endl;
}

int main() {
    cin.tie(0), cout.tie(0)->sync_with_stdio(false);
    solve();
    return 0;
}