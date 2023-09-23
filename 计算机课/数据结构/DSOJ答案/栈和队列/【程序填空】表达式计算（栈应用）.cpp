#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<complex>
#include<array>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<list>
#include<unordered_set>
#include<unordered_map>
#include<bitset>
#include<valarray>
#include<sstream>
#include<functional>
#include<cassert>
#include<random>
using namespace std;
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<bool> vb;
typedef queue<int> qi;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef pair<double, double> pdd;
typedef tuple<int, int, int> tiii;
typedef vector<pii> vii;
typedef vector<pll> vll;
typedef queue<pii> qii;
typedef complex<double> cp;
#define umap unordered_map
#define uset unordered_set
#define pque priority_queue
#define IOS ios::sync_with_stdio(0); cin.tie(0), cout.tie(0);
#define CaseT int CaseT; cin >> CaseT; while(CaseT--)
#define endl '\n'
#define npt nullptr
#define so sizeof
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define hashset_finetune(p) (p).reserve(1024); (p).max_load_factor(0.25);  // 哈希表防卡
const double eps = 1e-7;
const double pi = acos(-1.0);
const int INF = 0x3f3f3f3f;
const ll INFF = 0x3f3f3f3f3f3f3f3f;
const int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
// ----------------------------------------------------------------
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }  // 注意要不要改成ll
ll lcm(ll a, ll b) { return a * b / gcd(a, b); }  // 注意要不要改成ll
ll lowbit(ll x) { return x & (-x); }
int cmp(double a, double b) {
    if (fabs(a - b) < eps) return 0;
    else if (a > b) return 1;
    else return -1;
}
int sgn(double x) {
    if (fabs(x) < eps) return 0;
    else if (x > 0) return 1;
    else return -1;
}

template<typename T>
void read(T& x) {
    x = 0;
    T sgn = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') sgn = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = (x << 3) + (x << 1) + (ch & 15);
        ch = getchar();
    }
    x *= sgn;
}

template<typename T>
void write(T x) {
    if (x < 0) {
        putchar('-');
        x *= -1;
    }
    if (x < 10) putchar(x + 48);
    else write(x / 10), putchar(x % 10 + 48);
}

ll qpow(ll a, ll k, ll MOD) {
    ll res = 1;
    while (k) {
        if (k & 1) res = res * a % MOD;
        k >>= 1;
        a = a * a % MOD;
    }
    return res;
}
// ---------------------------------------------------------------

#define OPSETSIZE 7
unsigned char Prior[7][7] = { //运算符间的优先关系
      '>','>','<','<','<','>','>',
      '>','>','<','<','<','>','>',
      '>','>','>','>','<','>','>',
      '>','>','>','>','<','>','>',
      '<','<','<','<','<','=',' ',
      '>','>','>','>',' ','>','>',
      '<','<','<','<','<',' ','='
};
char OPSET[OPSETSIZE] = { '+' , '-' , '*' , '/' ,'(' , ')' , '#' }; //运算符集合

double Operate(double a, unsigned char theta, double b);	//计算类似a+b的表达式结果
bool In(char Test, char* TestOp); //判断字符Test是否是运算符，是则返回true
char precede(char Aop, char Bop); //返回两个运算符优先级的比较结果 

//以下完成算术表达式求值函数EvaluateExpression(string MyExp)的填空
double EvaluateExpression(string MyExp) {  // 算术表达式求值算法,参数MyExp是表达式字符串
    stack<char> OPTR;		//运算符栈，字符元素
    stack<double> OPND;	//运算数栈，实数元素
    char TempData[20];
    double Data, a, b, r;
    char theta, c, x, Dr[2];
    OPTR.push('#');
    strcpy(TempData, "\0");
    int i = 0;		//表达式字符串的当前字符位置
    c = MyExp[i];	//表达式字符串的当前字符

    while (c != '#' || OPTR.top() != '#') {
        if (In(c, OPSET)) {
            switch (precede(OPTR.top(), c)) {
                case '<' :
                    OPTR.push(c);
                    c = MyExp[++i];
                    break;
                case '=':
                    OPTR.pop();
                    c = MyExp[++i];
                    break;
                case '>':
                    theta = OPTR.top(); OPTR.pop();
                    b = OPND.top(); OPND.pop();
                    a = OPND.top(); OPND.pop();
                    OPND.push(Operate(a, theta, b));
                    break;
            }
        }
        else {
            Dr[0] = c;
            Dr[1] = '\0';
            strcat(TempData, Dr);
            c = MyExp[++i];
            if (In(c, OPSET)) {
                sscanf(TempData, "%lf", &Data);
                OPND.push(Data);
                strcpy(TempData, "\0");
            }
        }
    }
    return OPND.top();
}

double Operate(double a, unsigned char theta, double b) {
    switch (theta) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0;
    }
}

bool In(char Test, char* TestOp) {
    for (int i = 0; i < OPSETSIZE; i++)
        if (Test == TestOp[i]) return true;
    return false;
}

int get_id(char op, char* TestOp) {
    for (int i = 0; i < OPSETSIZE; i++)
        if (op == TestOp[i]) return i;
    return 0;
}

char precede(char Aop, char Bop) {
    return Prior[get_id(Aop, OPSET)][get_id(Bop, OPSET)];
}

void solve() {
    string Exp;
    double result;
    cin >> Exp;
    result = EvaluateExpression(Exp);
    cout << fixed << setprecision(2) << result << endl;
}

int main() {
    IOS;
#ifndef ONLINE_JUDGE
    clock_t my_clock = clock();
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    // ----------------------------------------------------------------	
    CaseT  // 单测时注释掉该行
    solve();
    // ----------------------------------------------------------------
#ifndef ONLINE_JUDGE
    cout << endl << "Time used " << clock() - my_clock << " ms." << endl;
#endif
    return 0;
}