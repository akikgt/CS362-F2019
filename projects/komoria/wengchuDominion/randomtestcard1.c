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

    int bonus;
    // play baron
    baronR(choice1, p, post, &bonus);
    
    // check buys
    myAssertEqual(post->numBuys, pre.numBuys + 1, "Check Buys");

    // check the number of estate cards in hand
    int numEstateInHand = 0;
    numEstateInHand = countCardInHand(p, estate, &pre);

    if (numEstateInHand & choice1) {
        // discard estate card and get 4 coins pattern
        myAssertEqual(post->coins, pre.coins + 4, "Check coins");
        myAssertEqual(countCardInHand(p, estate, post), numEstateInHand - 1, "Check numEstate after discarding");
    }
    else {
        // gain estate card pattern
        if (pre.supplyCount[estate] >= 1)
            myAssertEqual(countCardInHand(p, estate, post), numEstateInHand + 1, "Check numEstate after gain");
    }

}


int main() {
    struct gameState G;
    memset(&G, 23, sizeof(struct gameState));   // clear the game state

    printf("--- baron random test ---\n");

    SelectStream(0);
    PutSeed(-1);

    // random generator
    for (int n = 0; n < 1000; n++) {
        // generate pure random game state
        for (int i = 0; i < sizeof(struct gameState); i++) {
            ((char *)&G)[i] = floor(Random() * 256);
        }

        /*  modify pure random game state to run the code */
        // set player
        int p = floor(Random() * 2);

        // set up each count to valid value
        G.deckCount[p] = floor(Random() * MAX_DECK);
        G.discardCount[p] = floor(Random() * MAX_DECK);
        G.handCount[p] = floor(Random() * (MAX_HAND - 1)) + 1;

        // set estate card count in the supply
        G.supplyCount[estate] = floor(Random() * 12);

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