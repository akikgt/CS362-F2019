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
#include "randomtestutil.h"
#include <math.h>


void checkHandleBaron(int p, int choice1, struct gameState *post) {
    // save pre-game state
    struct gameState pre;
    memcpy(&pre, post, sizeof(struct gameState));

    // play baron
    handleBaron(p, choice1, post);
    
    if (pre.numBuys + 1 != post->numBuys) {
        printf("something wrong choice1 == %d\n", choice1);
    }

    // check the player has an estate card
    int hasEstate = 0;
    for (int i = 0; i < pre.handCount[p]; i++) {
        if (pre.hand[p][i] == estate)
            hasEstate = 1;
    }

    if (hasEstate & choice1) {
        printf("estate in your hand\n");
    }
    else {
        printf("estate not in your hand\n");

    }

}


int main() {
    struct gameState G;
    memset(&G, 23, sizeof(struct gameState));   // clear the game state

    printf("--- baron random test ---\n");

    SelectStream(0);
    PutSeed(-1);

    // random generator
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
        int choice1 = round(Random() * -2) + 1;

        // run the refactored function
        checkHandleBaron(p, choice1, &G);
    }

    printf("--- baron random test end ---\n");
    printf ("--------------------------\n");
    return 0;
}