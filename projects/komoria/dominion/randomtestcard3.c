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

enum CARD_TYPE {
    ACTION = 1,
    TREASURE,
    VICTORY
};

int typeOfCard(int card) {
    if (card == copper || card == silver || card == gold)
    { //Treasure cards
        return TREASURE;
    }
    else if (card == estate || card == duchy || card == province || card == gardens || card == great_hall)
    { //Victory Card Found
        return VICTORY;
    }
    else
    { //Action Card
        return ACTION;
    }

    return -1;
}

void checkHandleTribute(int p, int np, struct gameState *post, int tributePos) {
    // save pre-game state
    struct gameState pre;
    memcpy(&pre, post, sizeof(struct gameState));

    // play tribute
    handleTribute(p, np, post, tributePos);


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
}


int main() {
    struct gameState G;
    memset(&G, 23, sizeof(struct gameState));   // clear the game state

    printf("--- tribute random test ---\n");

    SelectStream(0);
    PutSeed(-1);

    // random generator
    for (int n = 0; n < 5000; n++) {
        // generate pure random game state
        for (int i = 0; i < sizeof(struct gameState); i++) {
            ((char *)&G)[i] = floor(Random() * 256);
        }

        // modify pure random game state to run the code
        // set numPlayers
        G.numPlayers = floor(Random() * (MAX_PLAYERS - 1)) + 1;

        // set each player's card count
        for (int i = 0; i < G.numPlayers; i++) {
            G.deckCount[i] = floor(Random() * 50);
            G.discardCount[i] = floor(Random() * 50);
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

        // set next player's deck and discard pile
        setRandomDeck(np, &G);
        setRandomDiscard(np, &G);

        // run the refactored function
        // checkHandleBaron(p, choice1, &G);
        checkHandleTribute(p, np, &G, tributePos);
    }

    printf("--- tribute random test end ---\n");
    printf ("--------------------------\n");
    return 0;
}