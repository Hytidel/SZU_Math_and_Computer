#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
#include<unordered_set>
using namespace std;
typedef long long ll;
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
const int INF = 0x3f3f3f3f;

template<class T>
class Array {
private:
    int capacity;  // 最大长度
    int length;  // 当前长度
    T* arr;

public:
    Array() :capacity(100), length(0), arr(nullptr) {}

    ~Array() {
        if (arr) delete[] arr;
    }

    void setCapacity(int n) {  // 设置最大容量
        T* tmp = new T[capacity = n];
        memcpy(tmp, arr, sizeof(T) * length);
        delete[] arr;
        arr = tmp;
    }

    int getLength() {  // 获取当前的元素个数
        return length;
    }

    T& operator[](int idx) {  // 支持通过[]访问顺序表元素,默认下标合法
        return arr[idx];
    }

    void create(int n, int maxLength = 0) {  // 创建一个长度为n的顺序表,最大长度为maxLength
        capacity = maxLength ? maxLength : 2 * n;
        arr = new T[capacity];
        length = n;
    }

    void input() {
        for (int i = 0; i < length; i++) cin >> arr[i];
    }

    void print() {
        for (int i = 0; i < length; i++) cout << arr[i] << ' ';
    }

    bool insertToPos(T val, int pos) {  // 在下标pos位置插入元素val
        if (pos < 0 || pos > length || length == capacity) return false;

        for (int i = length - 1; i >= pos; i--) arr[i + 1] = arr[i];
        arr[pos] = val;
        length++;
        return true;
    }

    bool removeByPos(int pos) {  // 删除下标pos的元素
        if (pos < 0 || pos >= length) return false;

        for (int i = pos; i < length; i++) arr[i] = arr[i + 1];
        length--;
        return true;
    }
};

void print(Array<int>& arr) {
    cout << arr.getLength() << ' ';
    arr.print();
    cout << endl;
}

void solve() {
    int n; cin >> n;
    Array<int> arr;
    arr.create(n);
    arr.setCapacity(1000);
    arr.input();
    print(arr);

    int pos, x; 
    for (int i = 0; i < 2; i++) {
        cin >> pos >> x;
        if (arr.insertToPos(x, pos - 1)) print(arr);
        else cout << "error" << endl;
    }
    
    for (int i = 0; i < 2; i++) {
        cin >> pos;
        if (arr.removeByPos(pos - 1)) print(arr);
        else cout << "error" << endl;
    }

    for (int i = 0; i < 2; i++) {
        cin >> pos;

        pos--;
        if (pos < 0 || pos >= arr.getLength()) cout << "error" << endl;
        else cout << arr[pos] << endl;
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