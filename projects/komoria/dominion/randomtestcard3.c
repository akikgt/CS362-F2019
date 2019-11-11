/* -----------------------------------------------------------------------
 * Automated random test for my refactored Tribute function handleTribute
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


// void checkHandleMinion(int p, int choice1, struct gameState *post) {
    // // save pre-game state
    // struct gameState pre;
    // memcpy(&pre, post, sizeof(struct gameState));

    // // play baron
    // handleBaron(p, choice1, post);
    
    // // check buys
    // myAssertEqual(post->numBuys, pre.numBuys + 1, "Check Buys");

    // // check the number of estate cards in hand
    // int numEstateInHand = 0;
    // numEstateInHand = countCardInHand(p, estate, &pre);

    // if (numEstateInHand & choice1) {
    //     // discard estate card and get 4 coins pattern
    //     myAssertEqual(post->coins, pre.coins + 4, "Check coins");
    //     myAssertEqual(countCardInHand(p, estate, post), numEstateInHand - 1, "Check numEstate after discarding");
    // }
    // else {
    //     // gain estate card pattern
    //     if (pre.supplyCount[estate] >= 1)
    //         myAssertEqual(countCardInHand(p, estate, post), numEstateInHand + 1, "Check numEstate after gain");
    // }

// }


int main() {
    struct gameState G;
    memset(&G, 23, sizeof(struct gameState));   // clear the game state

    printf("--- tribute random test ---\n");

    SelectStream(0);
    PutSeed(-1);

    // random generator
    for (int n = 0; n < 2000; n++) {
        printf("%d\n", n);
        // generate pure random game state
        for (int i = 0; i < sizeof(struct gameState); i++) {
            ((char *)&G)[i] = floor(Random() * 256);
        }

        // modify pure random game state to run the code
        // set numPlayers
        G.numPlayers = floor(Random() * (MAX_PLAYERS - 1)) + 1;

        // set each player's card count
        for (int i = 0; i < G.numPlayers; i++) {
            G.deckCount[i] = floor(Random() * 100);
            G.discardCount[i] = floor(Random() * 100);
            G.handCount[i] = floor(Random() * (MAX_HAND - 1)) + 1;
            G.playedCardCount = floor(Random() * MAX_DECK); // discard function uses playedCard array
        }

        // set current player
        int p = floor(Random() * G.numPlayers);
        // set next player
        int np = (p + 1) % G.numPlayers;

        // set current player's hand
        setRandomHand(p, &G);
        // The player must have at least one tribute in hand
        int tributePos = floor(Random() * G.handCount[p]);
        G.hand[p][tributePos] = tribute;

        // run the refactored function
        // checkHandleBaron(p, choice1, &G);
        handleTribute(p, np, &G, tributePos);
    }

    printf("--- tribute random test end ---\n");
    printf ("--------------------------\n");
    return 0;
}