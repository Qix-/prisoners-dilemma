#include "strategy.h"

static void * init(void) {
	return 0;
}

static int cheat(void *v) {
	(void) v;
	return 1;
}

static void report(void *v, int c) {
	(void) v;
	(void) c;
}

static Strategy always_cheat = {
	&init,
	&cheat,
	&report
};

extern const Strategy * always_cheat_strategy() {
	return &always_cheat;
}
