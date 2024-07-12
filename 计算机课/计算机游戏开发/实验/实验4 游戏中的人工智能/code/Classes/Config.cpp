#include "Config.h"

// 比较浮点数大小
int doubleCompare(double a, double b) {
	if (fabs(a - b) <= epsilon) {
		return 0;
	}
	return a < b ? -1 : 1;
}

// 随机生成区间 [l, r] 内的整数
int generateRandomInteger(int l, int r) {
	int res = abs((int)rnd()) % (r - l + 1);
	return res + l;
}