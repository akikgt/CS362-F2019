/* -----------------------------------------------------------------------
 * Automated random test for my refactored Minion function handleMinion
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


void checkHandleMinion(int p, int choice1, int choice2, struct gameState *post, int minionPos) {
    // save pre-game state
    struct gameState pre;
    memcpy(&pre, post, sizeof(struct gameState));

    // play minion
    handleMinion(p, choice1, choice2, post, minionPos);
    
    myAssertEqual(countCardInHand(p, minion, post), countCardInHand(p, minion, &pre), "Check Minion discarded correctly after playing");
    myAssertEqual(post->numActions, pre.numActions + 1, "Check Actions");

    if (choice1) {
        myAssertEqual(post->coins, pre.coins + 2, "Check coins");
    }
    else if (choice2) {
        myAssertEqual(post->handCount[p], 4, "Check current player's handCount after minion");

        for (int i = 0; i < post->numPlayers; i++) {
            if (i == p)
                continue;
            
            if (pre.handCount[i] >= 5) {
                myAssertEqual(post->handCount[i], 4, "Check other player's handCount after minion");
            }
            else {
                myAssertEqual(post->handCount[i], pre.handCount[i], "Check other player's handCount after minion");
            }
        }
        
    }
}


int main() {
    struct gameState G;
    memset(&G, 23, sizeof(struct gameState));   // clear the game state

    printf("--- minion random test ---\n");

    SelectStream(0);
    PutSeed(-1);

    // random generator
    for (int n = 0; n < 2000; n++) {
        // generate pure random game state
        for (int i = 0; i < sizeof(struct gameState); i++) {
            ((char *)&G)[i] = floor(Random() * 256);
        }

        // modify pure random game state to run the code
        // set numPlayers
        G.numPlayers = floor(Random() * (MAX_PLAYERS - 1)) + 1;

        // set each player's card count
        for (int i = 0; i < G.numPlayers; i++) {
            G.deckCount[i] = floor(Random() * MAX_DECK);
            G.discardCount[i] = floor(Random() * MAX_DECK);
            G.handCount[i] = floor(Random() * (MAX_HAND - 1)) + 1;
            G.playedCardCount = floor(Random() * MAX_DECK); // discard function uses playedCard array
        }

        // set current player
        int p = floor(Random() * G.numPlayers);

        // set current player's hand
        setRandomHand(p, &G);

        // The player must have at least one baron in hand
        int minionPos = floor(Random() * G.handCount[p]);
        G.hand[p][minionPos] = minion;

        // set choices {negative, 0, positive}
        int choice1 = round(Random() * -2) + 1;
        int choice2 = round(Random() * -2) + 1;

        // check refactored function
        checkHandleMinion(p, choice1, choice2, &G, minionPos);
    }

    printf("--- minion random test end ---\n");
    printf ("--------------------------\n");
    return 0;
}