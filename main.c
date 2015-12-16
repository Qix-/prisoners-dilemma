#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "strategy.h"

#define MAX_ROUNDS 1000000

extern const Strategy * always_fair_strategy();
extern const Strategy * always_cheat_strategy();
extern const Strategy * tit_for_tat_strategy();
extern const Strategy * f5_tit_for_tat_strategy();
extern const Strategy * f10_tit_for_tat_strategy();

const Strategy * resolve_strategy(const char * name) {
#	define CHECK(n) if (strcmp(#n, name) == 0) return n##_strategy()
	CHECK(always_fair);
	CHECK(always_cheat);
	CHECK(tit_for_tat);
	CHECK(f5_tit_for_tat);
	CHECK(f10_tit_for_tat);
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
	int badChance;

	int games;
	int game;
	int leftWins;
	int rightWins;

	int confess;
	int cheat;
	int betrayed;
	int silence;

	confess = -2;
	cheat = 2;
	betrayed = -1;
	silence = 1;

	badChance = 0;

	games = 1;
	leftWins = 0;
	rightWins = 0;

	if (argc < 4) {
		fprintf(stderr, "usage: <strategy> <strategy> <starting points> [bad signal chance / 100 = 0] [games = 1] [confess pts = -2] [cheat pts = 2] [betrayed pts = -1] [silence pts = 1]\n");
		return 1;
	}

	left = resolve_strategy(argv[1]);
	right = resolve_strategy(argv[2]);
	if (!left || !right) {
		return 1;
	}

	if (argc >= 5) {
		badChance = atoi(argv[4]);
	}

	if (argc >= 6) {
		games = atoi(argv[5]);
	}

	if (argc >= 7) {
		confess = atoi(argv[6]);
	}

	if (argc >= 8) {
		cheat = atoi(argv[7]);
	}

	if (argc >= 9) {
		betrayed = atoi(argv[8]);
	}

	if (argc >= 10) {
		silence = atoi(argv[9]);
	}

	printf("\x1b[1mtesting \x1b[31m%s\x1b[39m against \x1b[32m%s\x1b[39m\n",
		argv[1], argv[2]);
	printf("starting with \x1b[33m%d\x1b[39m points\n",
		atoi(argv[3]));
	printf("confess=\x1b[33m%d\x1b[39m, cheat=\x1b[33m%d\x1b[39m, betrayed=\x1b[33m%d\x1b[39m, silence=\x1b[33m%d\n",
		confess, cheat, betrayed, silence);
	printf("playing \x1b[33m%d\x1b[39m games\x1b[m\n\n",
		games);

	srand(time(NULL));

	for (game = 0; game < games; game++) {
		count = 0;

		leftPts = atoi(argv[3]);
		rightPts = atoi(argv[3]);

		leftData = left->init();
		rightData = right->init();

		while (1) {
			count++;

			leftChoice = left->cheat(leftData);
			rightChoice = right->cheat(rightData);

			if (rand() % 100 < badChance) {
				if (rand() % 2) {
					leftChoice = !leftChoice;
				}

				if (rand() % 2) {
					rightChoice = !rightChoice;
				}

				printf("*");
			}

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
				rightWins++;
				break;
			} else if (rightPts <= 0) {
				printf("\n\n\x1b[1;31m%s\x1b[39m defeated \x1b[32m%s\x1b[39m in \x1b[33m%d\x1b[39m rounds.\n",
					argv[1], argv[2], count);
				printf("\x1b[31m%s\x1b[39m had \x1b[33m%d\x1b[39m points to spare.\x1b[m\n",
					argv[1], leftPts);
				leftWins++;
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
	}

	printf("\n\x1b[1mWAR RESULTS:\n");
	printf("\x1b[31m%s\x1b[39m won \x1b[33m%.2f%%\x1b[39m of the time in \x1b[33m%d\x1b[39m games\n",
		argv[1], ((float) leftWins / (float) games) * 100.0f, leftWins);
	printf("\x1b[32m%s\x1b[39m won \x1b[33m%.2f%%\x1b[39m of the time in \x1b[33m%d\x1b[39m games\n",
		argv[2], ((float) rightWins / (float) games) * 100.0f, rightWins);

	if (leftWins > rightWins) {
		printf("\n\x1b[31m%s wins.\x1b[m\n",
			argv[1]);
	} else if (rightWins > leftWins) {
		printf("\n\x1b[32m%s wins.\x1b[m\n",
			argv[2]);
	} else {
		printf("\n\x1b[33mstalemate!\x1b[m\n");
	}

	return 0;
}
