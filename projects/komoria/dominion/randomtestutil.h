#ifndef _RANDOM_TEST_UTILS
#define _RANDOM_TEST_UTILS

void setRandomHand(int player, struct gameState *G);
void setRandomDeck(int player, struct gameState *G);
void setRandomDiscard(int player, struct gameState *G);
void myAssertEqual(int expr, int expected, char *msg);
int countCardInHand(int currentPlayer, int card, struct gameState *G);
int countCardInDiscard(int currentPlayer, int card, struct gameState *G);

#endif