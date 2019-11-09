#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// set player's hand by random
int setRandomHand(int player, struct gameState *G) {
    for (int i = 0; i < G->handCount[player]; i++) {
        G->hand[player][i] = floor(Random() * treasure_map); // set 0 to treasure_map(last card in enum)
    }
}