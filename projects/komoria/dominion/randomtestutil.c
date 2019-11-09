#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"

// set player's hand by random
void setRandomHand(int player, struct gameState *G) {
    for (int i = 0; i < G->handCount[player]; i++) {
        G->hand[player][i] = floor(Random() * treasure_map); // set 0 to treasure_map(last card in enum)
    }
}

void myAssertEqual(int expr, int expected, char *msg) {
    printf("%s: ", msg);
    if (expr == expected) {
        printf("PASS!: got %d\n", expr);
        return;
    }
    else {
        printf("FAILED: %d expected, but got %d.\n", expected, expr);
        return;
    }
}

int countCardInHand(int currentPlayer, int card, struct gameState *G) {
    int count = 0;
    for (int i = 0; i < G->handCount[currentPlayer]; i++) {
        if (G->hand[currentPlayer][i] == card)
            count++;
    }
    return count;
}

int countCardInDiscard(int currentPlayer, int card, struct gameState *G) {
    int count = 0;
    for (int i = 0; i < G->discardCount[currentPlayer]; i++) {
        if (G->discard[currentPlayer][i] == card)
            count++;
    }
    return count;
}