/* -----------------------------------------------------------------------
 * Automated random test for my refactored Baron function handleBaron
 * -----------------------------------------------------------------------
 */

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

int main() {
    struct gameState G;
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    // verify the result
    printf("--- baron random test ---\n");

    for (int n = 0; n < 2000; n++) {
        // generate pure random game state
        for (int i = 0; i < sizeof(struct gameState); i++) {
            ((char *)&G)[i] = floor(Random() * 256);
        }

        // modify pure random game state to run the code
        // set player
        int p = floor(Random() * 2);

        // set up each count to valid value
        G.deckCount[p] = floor(Random() * MAX_DECK);
        G.discardCount[p] = floor(Random() * MAX_DECK);
        G.handCount[p] = floor(Random() * (MAX_HAND - 1)) + 1;

        // set hand
        setRandomHand(p, &G);

        // The player must have at least one baron in hand
        int baronPos = floor(Random() * G.handCount[p]);
        G.hand[p][baronPos] = baron;

        // set choice {negative, 0, positive}
        int choice1 = floor(Random() * -2) + 1;

        // run the refactored function
        handleBaron(p, choice1, &G);
    }

    printf("--- baron random test end ---\n");
    printf ("--------------------------\n");
    return 0;
}