#include "Config.h"

int generateRandomInteger(int l, int r) {
	int res = abs((int)rnd()) % (r - l + 1);
	return res + l;
}