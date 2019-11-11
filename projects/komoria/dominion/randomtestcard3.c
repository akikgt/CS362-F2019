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
    myAssertEqual(countCardInHand(p, tribute, post), countCardInHand(p, tribute, &pre), "Check the tribute card discarded correctly after playing");

    int tributeRevealedCards[2] = {-1, -1};
    int bonusActions = 0;
    int bonusCoins = 0;
    int bonusHands = 0;

    if (pre.deckCount[np] + pre.discardCount[np] <= 1) {
        if (pre.deckCount[np] == 1) {
            tributeRevealedCards[0] = pre.deck[np][0];
        }
        else if (pre.discardCount[np] == 1) {
            tributeRevealedCards[0] = pre.discard[np][0];
        }
    }
    else {
        if (pre.deckCount[np] == 0) {
            tributeRevealedCards[0] = pre.discard[np][pre.discardCount[np] - 1];
            tributeRevealedCards[1] = pre.discard[np][pre.discardCount[np] - 2];
        }
        else if (pre.deckCount[np] == 1) {
            tributeRevealedCards[0] = pre.deck[np][0];
        }
        else { // there are more than or equal to 2 cards in next player's deck
            tributeRevealedCards[0] = pre.deck[np][pre.deckCount[np] - 1];
            tributeRevealedCards[1] = pre.deck[np][pre.deckCount[np] - 2];
        }
    }

    // handle duplicated cards
    if (tributeRevealedCards[0] == tributeRevealedCards[1]) {
        tributeRevealedCards[1] = -1;
    }

    // calculate bonus
    for (int i = 0; i < 2; i++) {
        int type = typeOfCard(tributeRevealedCards[i]);
        switch (type) {
            case ACTION:
                bonusActions += 2;
                break;
            case TREASURE:
                bonusCoins += 2;
                break;
            case VICTORY:
                bonusHands += 2;
                break;
            default:
                break;
        }
    }

    myAssertEqual(post->numActions, pre.numActions + bonusActions, "Check Actions after tribute");
    myAssertEqual(post->coins, pre.coins + bonusCoins, "Check Coins after tribute");
    myAssertEqual(post->handCount[p], pre.handCount[p] + bonusHands, "Check HandCount after tribute");
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

        /* modify pure random game state to run the code */
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
        // The player must have at least one tribute in hand to play it
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