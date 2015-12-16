#include "strategy.h"

static void * init(void) {
	return 0;
}

static int cheat(void *v) {
	(void) v;
	return 0;
}

static void report(void *v, int c) {
	(void) v;
	(void) c;
}

static Strategy always_fair = {
	&init,
	&cheat,
	&report
};

extern const Strategy * always_fair_strategy() {
	return &always_fair;
}
