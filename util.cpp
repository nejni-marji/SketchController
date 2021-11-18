#include "util.h"

void sleepFrame(int n) {
	struct timespec request = {n/60, 16666667*(n%60)};
	nanosleep(&request, NULL);
}
