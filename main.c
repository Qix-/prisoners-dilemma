#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strategy.h"

#define MAX_ROUNDS 100000

extern const Strategy * always_fair_strategy();
extern const Strategy * always_cheat_strategy();
extern const Strategy * tit_for_tat_strategy();
extern const Strategy * f5_tit_for_tat_strategy();

const Strategy * resolve_strategy(const char * name) {
#	define CHECK(n) if (strcmp(#n, name) == 0) return n##_strategy()
	CHECK(always_fair);
	CHECK(always_cheat);
	CHECK(tit_for_tat);
	CHECK(f5_tit_for_tat);
#	undef CHECK

	fprintf(stderr, "unknown strategy: %s\n", name);
	return 0;
}

int main(int argc, const char **argv) {
	const Strategy *left;
	const Strategy *right;
	int leftPts;
	int rightPts;
	void *leftData;
	void *rightData;
	int leftChoice;
	int rightChoice;
	int count;

	int confess;
	int cheat;
	int betrayed;
	int silence;

	confess = -2;
	cheat = 2;
	betrayed = -1;
	silence = 1;


	if (argc < 4) {
		fprintf(stderr, "usage: <strategy> <strategy> <starting points> [confess pts = -2] [cheat pts = 2] [betrayed pts = -1] [silence pts = 1]\n");
		return 1;
	}

	left = resolve_strategy(argv[1]);
	right = resolve_strategy(argv[2]);
	if (!left || !right) {
		return 1;
	}

	count = 0;

	leftPts = atoi(argv[3]);
	rightPts = atoi(argv[3]);

	leftData = left->init();
	rightData = right->init();

	if (argc >= 5) {
		confess = atoi(argv[4]);
	}

	if (argc >= 6) {
		cheat = atoi(argv[5]);
	}

	if (argc >= 7) {
		betrayed = atoi(argv[6]);
	}

	if (argc >= 8) {
		silence = atoi(argv[7]);
	}

	printf("\x1b[1mtesting \x1b[31m%s\x1b[39m against \x1b[32m%s\x1b[39m\n",
		argv[1], argv[2]);
	printf("starting with \x1b[33m%d\x1b[39m points\n",
		leftPts);
	printf("confess=\x1b[33m%d\x1b[39m, cheat=\x1b[33m%d\x1b[39m, betrayed=\x1b[33m%d\x1b[39m, silence=\x1b[33m%d\x1b[m\n\n",
		confess, cheat, betrayed, silence);

	while (1) {
		count++;

		leftChoice = left->cheat(leftData);
		rightChoice = right->cheat(rightData);

		if (leftChoice && rightChoice) {
			leftPts += confess;
			rightPts += confess;
			printf("\x1b[33m^\x1b[m");
		} else if (leftChoice && !rightChoice) {
			leftPts += cheat;
			rightPts += betrayed;
			printf("\x1b[31m<\x1b[m");
		} else if (!leftChoice && rightChoice) {
			rightPts += cheat;
			leftPts += betrayed;
			printf("\x1b[32m>\x1b[m");
		} else {
			leftPts += silence;
			rightPts += silence;
			printf("\x1b[34m.\x1b[m");
		}

		left->report(leftData, rightChoice);
		right->report(rightData, leftChoice);

		if (leftPts <= 0 && rightPts <= 0) {
			printf("\n\n\x1b[1mreached \x1b[33mstalemate\x1b[39m in \x1b[33m%d\x1b[39m rounds.\n",
				count);
			break;
		} else if (leftPts <= 0) {
			printf("\n\n\x1b[1;32m%s\x1b[39m defeated \x1b[31m%s\x1b[39m in \x1b[33m%d\x1b[39m rounds.\n",
				argv[2], argv[1], count);
			printf("\x1b[32m%s\x1b[39m had \x1b[33m%d\x1b[39m points to spare.\x1b[m\n",
				argv[2], rightPts);
			break;
		} else if (rightPts <= 0) {
			printf("\n\n\x1b[1;31m%s\x1b[39m defeated \x1b[32m%s\x1b[39m in \x1b[33m%d\x1b[39m rounds.\n",
				argv[1], argv[2], count);
			printf("\x1b[31m%s\x1b[39m had \x1b[33m%d\x1b[39m points to spare.\x1b[m\n",
				argv[1], leftPts);
			break;
		} else if (count == MAX_ROUNDS) {
			printf("\n\n\x1b[1mreached \x1b[33mtruce (timeout)\x1b[39m in \x1b[33m%d\x1b[39m rounds.\n",
				count);
			break;
		}
	}

	if (leftData) {
		free(leftData);
	}

	if (rightData) {
		free(rightData);
	}

	return 0;
}
