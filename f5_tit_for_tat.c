#include <stdlib.h>
#include "strategy.h"

static void * init(void) {
	int * i = malloc(sizeof(int));
	*i = 5;
	return i;
}

static int cheat(void *v) {
	return ++*((int*)v) < 5;
}

static void report(void *v, int c) {
	int *iv = (int*) v;

	if (c) {
		*iv = 0;
	}
}

static Strategy f_tit_for_tat = {
	&init,
	&cheat,
	&report
};

extern const Strategy * f5_tit_for_tat_strategy() {
	return &f_tit_for_tat;
}
