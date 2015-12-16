#include <stdlib.h>
#include "strategy.h"

static void * init(void) {
	int * i = malloc(sizeof(int));
	*i = 0;
	return i;
}

static int cheat(void *v) {
	if (*((int*)v)) {
		return rand() % 2;
	}

	return 0;
}

static void report(void *v, int c) {
	*((int *)v) = c;
}

static Strategy frand_tit_for_tat = {
	&init,
	&cheat,
	&report
};

extern const Strategy * frand_tit_for_tat_strategy() {
	return &frand_tit_for_tat;
}
