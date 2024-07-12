## 14.9 A\*

A\*算法类似于Dijkstra算法,是对BFS的优化.朴素BFS中直接从起点搜到终点可能会经过很多状态,而A\*算法中加入启发函数(估价函数),使得只需搜较少的状态即可找到从起点到终点的一条最短路.A\*算法只在搜索空间很大时才有明显的优化效果.A\*算法和Dijkstra算法都能解决边权非负的图中的最短路.Dijkstra算法可看作从每个点到终点的估计距离都是$0$的最短路.



A\*算法将BFS中的队列换为小根堆,队列中不仅存起点到当前点的真实距离,还存该点到终点的估计距离.每次选择与终点的估计距离最小的点扩展.



A\*算法中每个点可能会被扩展多少次.

①BFS入队时判重;②Dijkstra算法出队时判重;③A\*算法不判重.



A\*算法的成立条件:估计距离$\leq $真实距离.

A\*算法的应用场景:确定有解.若无解时,A\*算法会将整个搜索空间都搜索一遍,效率低于朴素BFS.

​	但实际应用中未必知道是否有解,也可用A\*算法,大部分时候比朴素BFS快.



当终点第一次出队时已确定最短距离.

[**证**]  设终点$u$当前出队,此时$u$与起点的距离为$d(u)$.

设起点到$u$的真实距离为$dis(u)$,$u$到终点的估计距离和真实距离分别为$f(u)$和$g(u)$.

设真实的最短路径为$d$,则$d=dis(u)+g(u)\geq dis(u)+f(u)$.

若$u$出队时不是最短距离,则$d(u)>d\geq dis(u)+f(u)$,且是最短距离的节点在队列中.

​	这表明:当前出队的不是队列中的最小值,矛盾.

[**注**]  A\*算法只能保证终点出队时是最短距离,不能保证其他节点.

---

----

### 14.9.1  八数码

#### 题意

在$3\times 3$的网格中不重不漏地填入$1\sim 8$这$8$个数字,空位用'X'表示.游戏过程中,可将X与其上、下、左、右四个方向之一的数字交换(若存在),目标是通过交换将初始网格变为如下形式(称为正确排列):

```c++
1 2 3
4 5 6
7 8 X
```

将X与其上、下、左、右四个方向的数字交换分别记作u、d、l、r.现给定一个初始网格,求通过最少的移动次数将其变为正确排列的交换序列,若有多种方案,输出任一种;若无方案,输出"unsolvable".

用一个字符串描述$3\times 3$的初始网格.若初始网格为:

```c++
1 2 3 
x 4 6 
7 5 8 
```

则输入$1\ 2\ 3\ x\ 4\ 6\ 7\ 5\ 8$.


#### 思路

八数码问题有解的充要条件:初始序列的逆序对数量是偶数.

[**证**]  (充)  较难,略.

(必)  显然左右交换不改变逆序对数量,上下交换只会改变两对逆序对,故初始序列的逆序对数与正确排列的逆序对数同奇偶.



注意到最优解中每次交换可使得交换的数与其目标位置的Manhattan距离减$1$,估价函数取每个数码与其目标位置的Manhattan距离之和.

#### 代码

```c++
int f(string state) {  // 估价函数
	int res = 0;
	for (int i = 0; i < state.size(); i++) {
		if (state[i] != 'x') {
			int tmp = state[i] - '1';
			res += abs(i / 3 - tmp / 3) + abs(i % 3 - tmp % 3);
		}
	}
	return res;
}

string bfs(string start) {
	const int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 }; 
	const char op[] = "urdl";  // 与上面的偏移量对应
	string end = "12345678x";
	umap<string, int> dis;
	umap<string, pair<string, char>> pre;  // 记录前驱和操作
	priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> heap;
	heap.push({ f(start),start });
	dis[start] = 0;

	while (heap.size()) {
		auto tmp = heap.top(); heap.pop();
		string state = tmp.second;
		if (state == end) break;

		int x, y;  // x的位置
		for (int i = 0; i < state.size(); i++) {
			if (state[i] == 'x') {
				x = i / 3, y = i % 3;
				break;
			}
		}

		int step = dis[state];
		string backup = state;
		for (int i = 0; i < 4; i++) {
			int curx = x + dx[i], cury = y + dy[i];
			if (curx < 0 || curx >= 3 || cury < 0 || cury >= 3) continue;

			swap(state[x * 3 + y], state[curx * 3 + cury]);
			if (!dis.count(state) || dis[state] > step + 1) {
				dis[state] = step + 1;
				pre[state] = { backup,op[i] };
				heap.push({ dis[state] + f(state),state });
			}
			swap(state[x * 3 + y], state[curx * 3 + cury]);  // 恢复现场
		}
	}

	string res;
	while (end != start) {
		res += pre[end].second;
		end = pre[end].first;
	}
	reverse(all(res));
	return res;
}

int main() {
	string s1, s2;  // s1为原序列,s2为去掉x的序列
	char c;
	while (cin >> c) {
		s1.push_back(c);
		if (c != 'x') s2.push_back(c);
	}

	int cnt = 0;  // 逆序对个数
	for (int i = 0; i < s2.size(); i++)
		for (int j = i + 1; j < s2.size(); j++)
			if (s2[i] > s2[j]) cnt++;

	if (cnt & 1) cout << "unsolvable";
	else cout << bfs(s1);
}
```

---

---

### 14.9.2 第k短路

#### 题意

给定一个包含$n$个点(编号$1\sim n$)、$m$条边的有向图,求起点$S$到终点$T$的第$k$短路的长度,路径允许重复经过点或边.要求每条最短路中至少包含一条边,即$S=T$时,$k++$.

第一行输入整数$n,m\ \ (1\leq n\leq 1000,0\leq m\leq 1\mathrm{e}4)$.接下来$m$行每行输入三个整数$u,v,w\ \ (1\leq u,v\leq n,1\leq l\leq 100)$.最后一行输入三个整数$S,T,k\ \ (1\leq S,T\leq n,1\leq k\leq 100)$,表示求起点$S$到终点$T$的第$k$短路.

若最短路存在,输出最短路长度;否则输出$-1$.

#### 思路

与最短路问题不同,扩展时应将当前点能扩展到的所有点都入队.显然路径中存在环时,起点到终点的路径可能有无数条,故搜索空间很大,考虑A\*算法.

估价函数可取每个点到终点的最短距离,该距离可通过以终点为起点跑一遍Dijksta算法得到.



A\*算法中当终点出队时确定最短路,一直出队至第$k$次即第$k$短路.

[**证**]  仿照终点出队时确定最短路的方法即证.

#### 代码

```c++
const int MAXN = 1005, MAXM = 2e5 + 5;  // 两倍边
int n, m;  // 点数、边数
int S, T, k;  // 起点S到终点T的第k短路
int head[MAXN], rhead[MAXN], edges[MAXM], w[MAXM], nxt[MAXM], idx = 0;  // head[]为正向边的头节点,rhead[]为反向边的头节点
int dis[MAXN];
bool vis[MAXN];
int cnt[MAXN];  // cnt[u]表示节点u出队的次数

void add(int h[], int a, int b, int c) {
	edges[idx] = b, w[idx] = c, nxt[idx] = h[a], h[a] = idx++;
}

void dijkstra() {
	priority_queue<pii, vii, greater<pii>> heap;
	heap.push({ 0,T });  // 终点
	memset(dis, INF, so(dis));
	dis[T] = 0;

	while (heap.size()) {
		auto tmp = heap.top(); heap.pop();

		int u = tmp.second;
		if (vis[u]) continue;

		vis[u] = true;
		for (int i = rhead[u]; ~i; i = nxt[i]) {
			int v = edges[i];
			if (dis[v] > dis[u] + w[i]) {
				dis[v] = dis[u] + w[i];
				heap.push({ dis[v],v });
			}
		}
	}
}

int Astar() {
	priority_queue<tiii, vector<tiii>, greater<tiii>> heap;  // 估价距离、真实距离、节点编号
	heap.push({ dis[S],0,S });
	
	while (heap.size()) {
		auto tmp = heap.top(); heap.pop();

		int d, u;  // 真实距离、节点编号
		tie(ignore, d, u) = tmp;
		cnt[u]++;
		if (cnt[T] == k) return d;  // 终点出队k次即找到第k短路

		for (int i = head[u]; ~i; i = nxt[i]) {
			int v = edges[i];
			if (cnt[v] < k) heap.push({ d + w[i] + dis[v],d + w[i],v });  // 出队不足k次才扩展
		}
	}
	return -1;  // 无解
}

int main() {
	memset(head, -1, so(head)), memset(rhead, -1, so(rhead));
	
	cin >> n >> m;
	for (int i = 0; i < m; i++) {
		int a, b, c; cin >> a >> b >> c;
		add(head, a, b, c), add(rhead, b, a, c);  // 分别建正向边和反向边
	}

	cin >> S >> T >> k;
	if (S == T) k++;  // 最短路至少包含一条边

	dijkstra();  // 求终点到每个节点的最短距离

	cout << Astar();
}
```

---

---

