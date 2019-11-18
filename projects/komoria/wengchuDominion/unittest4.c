/* -----------------------------------------------------------------------
 * Unit test for my refactored Tribute function handleTribute
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "unittestutil.h"

#define INITIAL_HANDCOUNT 5

enum {
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

int main() {
    int seed = 1000;
    int numPlayer = 2;
    int r;

    // set kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};

    struct gameState G;
    int currentPlayer = 0; 
    int nextPlayer = 1; 
    int handForTest[5] = {tribute, mine, copper, gold, estate};
    int beforeNumActions, beforeHandCount, beforeCoins;

    printf ("--------------------------\n");
    printf ("--- UNIT TEST 4 STARTS ---\n");

    /* ------------------------------------------------ 
     *  Testing tribute main cases
     * ------------------------------------------------ 
    */

    int revealedCards[3] = {baron, estate, gold}; // candidate card to be discarded from the deck

    // test all combinations of next players deck(3 * 3 types)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // count the number of each type
            int actions = 0;
            int victories = 0;
            int treasures = 0;
            if (typeOfCard(revealedCards[i]) == ACTION || typeOfCard(revealedCards[j]) == ACTION)
                actions = 1;
            if (typeOfCard(revealedCards[i]) == VICTORY || typeOfCard(revealedCards[j]) == VICTORY)
                victories = 1;
            if (typeOfCard(revealedCards[i]) == TREASURE || typeOfCard(revealedCards[j]) == TREASURE)
                treasures = 1;

            // reset&set variables
            memset(&G, 23, sizeof(struct gameState));   // clear the game state
            r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
            G.handCount[currentPlayer] = INITIAL_HANDCOUNT;
            G.handCount[nextPlayer] = INITIAL_HANDCOUNT;                                 // set other player's hand
            memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * INITIAL_HANDCOUNT); // set player's hand
            beforeNumActions = G.numActions;
            beforeHandCount = G.handCount[currentPlayer];
            beforeCoins = G.coins;

            // set next player's deck
            G.deckCount[nextPlayer] = 2;
            G.deck[nextPlayer][0] = revealedCards[i];
            G.deck[nextPlayer][1] = revealedCards[j];

            // call refactored function
            handleTribute(currentPlayer, nextPlayer, &G, 0);

            printf("--- check tribute with Action: %d, Treasure: %d, Victory: %d ---\n", actions, treasures, victories);
            // verify the result
            myAssertEqual(G.coins, beforeCoins + treasures * 2,
                          "Check coins");
            myAssertEqual(G.numActions, beforeNumActions + actions * 2,
                          "Check actions");
            myAssertEqual(G.handCount[currentPlayer], beforeHandCount + victories * 2 - 1,
                          "Check the hand count");
        }
    }

    /* ------------------------------------------------ 
     *  Testing tribute with next player's deckCount = 0 cases
     * ------------------------------------------------ 
    */

    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = INITIAL_HANDCOUNT;
    G.handCount[nextPlayer] = INITIAL_HANDCOUNT;                                 // set other player's hand
    memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * INITIAL_HANDCOUNT); // set player's hand
    beforeNumActions = G.numActions;
    beforeHandCount = G.handCount[currentPlayer];
    beforeCoins = G.coins;

    // set next player's deckCount to 0
    G.deckCount[nextPlayer] = 0; 
    // set next player's discard pile
    G.discardCount[nextPlayer] = 2;
    G.discard[nextPlayer][0] = gold;
    G.discard[nextPlayer][1] = estate;

    // call refactored function
    handleTribute(currentPlayer, nextPlayer, &G, 0);

    printf("--- check next player's deckCount == 0 case --- \n");
    // verify the result
    myAssertEqual(G.coins, beforeCoins + 2,
                  "Check coins");
    myAssertEqual(G.numActions, beforeNumActions,
                  "Check actions");
    myAssertEqual(G.handCount[currentPlayer], beforeHandCount + 1,
                  "Check the hand count");

    /* ------------------------------------------------ 
     *  Testing tribute with next player's deck + discard <= 1 cases
     * ------------------------------------------------ 
    */
    for (int i = 0; i < 3; i++) {
        // reset&set variables
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
        G.handCount[currentPlayer] = INITIAL_HANDCOUNT;
        G.handCount[nextPlayer] = INITIAL_HANDCOUNT;                                 // set other player's hand
        memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * INITIAL_HANDCOUNT); // set player's hand
        beforeNumActions = G.numActions;
        beforeHandCount = G.handCount[currentPlayer];
        beforeCoins = G.coins;

        // set next player's state
        int treasures = 0; // count treasure card in next player's deck
        switch (i) {
            case 0:
                G.deckCount[nextPlayer] = 1;
                G.discardCount[nextPlayer] = 0;
                G.deck[nextPlayer][0] = gold;
                treasures = 1;
                printf("--- check next player's deckCount = 1, discardCount = 0 case --- \n");
                break;
            case 1:
                G.deckCount[nextPlayer] = 0;
                G.discardCount[nextPlayer] = 1;
                G.discard[nextPlayer][0] = gold;
                treasures = 1;
                printf("--- check next player's deckCount = 0, discardCount = 1 case --- \n");
                break;
            case 2:
                G.deckCount[nextPlayer] = 0;
                G.discardCount[nextPlayer] = 0;
                printf("--- check next player's deckCount = 0, discardCount = 0 case --- \n");
                break;
        }

        // call refactored function
        handleTribute(currentPlayer, nextPlayer, &G, 0);

        // verify the result
        myAssertEqual(G.coins, beforeCoins + 2 * treasures,
                      "Check coins");
        myAssertEqual(G.numActions, beforeNumActions,
                      "Check actions");
        myAssertEqual(G.handCount[currentPlayer], beforeHandCount - 1,
                      "Check the hand count");
    }

    printf("--- UNIT TEST 4 ENDS ---\n");
    printf("--------------------------\n");
    return 0;
}
