#ifndef STRATEGY_H__
#define STRATEGY_H__
#pragma once

typedef struct {
	void * (*init)();
	int (*cheat)(void *);
	void (*report)(void *, int);
} Strategy;

#endif
