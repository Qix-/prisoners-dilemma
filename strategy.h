#ifndef STRATEGY_H__
#define STRATEGY_H__
#pragma once

typedef struct {
	/* initialize some sort of "memory" construct, or 0 */
	/* non-zero return values are automatically free()'d by the harness,
	   so many sure you return an actual malloc()'d pointer! */
	void * (*init)();

	/* whether or not to cheat, given the current memory. */
	/* 1 = confess (cheat), 0 = stay silent */
	int (*cheat)(void *);

	/* recieves whether or not the other prisoner confessed. */
	/* this is after mixed signals have been applied, if any */
	void (*report)(void *, int);
} Strategy;

#endif
