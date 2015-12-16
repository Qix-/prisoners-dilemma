#include <stdlib.h>
#include "strategy.h"

static void * init(void) {
	int * i = malloc(sizeof(int));
	*i = 0;
	return i;
}

static int cheat(void *v) {
	return *((int*)v);
}

static void report(void *v, int c) {
	*((int *)v) = c;
}

static Strategy tit_for_tat = {
	&init,
	&cheat,
	&report
};

extern const Strategy * tit_for_tat_strategy() {
	return &tit_for_tat;
}
