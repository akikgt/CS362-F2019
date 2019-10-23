/* -----------------------------------------------------------------------
 * Unit test for my refactored Ambasaddor function handleAmbassador
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

int main() {
    int seed = 1000;
    int numPlayer = 2;
    int r;
    int choice1, choice2;
    int result;

    // set kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};

    struct gameState G;
    int currentPlayer = 0; 
    int nextPlayer = 1; 
    int handForTest[5] = {ambassador, mine, estate, estate, gold};
    int beforeNumActions, beforeHandCount, beforeCoins, beforeSupplyCount;

    printf ("--------------------------\n");
    printf ("--- UNIT TEST 3 STARTS ---\n");

    /* ------------------------------------------------ 
     *  Testing invalid choices
     * ------------------------------------------------ 
    */

    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = INITIAL_HANDCOUNT;
    G.handCount[nextPlayer] = INITIAL_HANDCOUNT; // set other player's hand
    memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * INITIAL_HANDCOUNT); // set player's hand
    beforeNumActions = G.numActions;
    beforeHandCount = G.handCount[currentPlayer];
    beforeCoins = G.coins;
    choice1 = 0;

    printf("--- Check invalid choice cases ---\n");
    // Case: choice2 > 2
    choice2 = 3;
    // call refactored function
    result = handleAmbassador(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid chioice2(greater than 2) case");
    // Case: choice2 < 0
    choice2 = -1;
    // call refactored function
    result = handleAmbassador(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid chioice2(less than 0) case");
    // Case: choice1 == handpos
    choice1 = 0;
    choice2 = 2;
    // call refactored function
    result = handleAmbassador(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid chioice1 case");
    // Case: choice2 is greater than the num of cards to be discarded
    choice1 = 1; // try to discard a mine card
    choice2 = 2; // try to discard two mine card
    // call refactored function
    result = handleAmbassador(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid choice2(greater than the num of cards to be discarded) case");


    /* ------------------------------------------------ 
     *  Testing Ambassador
     *  Reveal a card in hand and returns it.
     *  check two cases. choice2 = 1 and choice2 = 2
     * ------------------------------------------------ 
    */
    for (int choice2 = 1; choice2 <= 2; choice2++) {
        // reset&set variables
        memset(&G, 23, sizeof(struct gameState));   // clear the game state
        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
        G.handCount[currentPlayer] = INITIAL_HANDCOUNT;
        G.handCount[nextPlayer] = INITIAL_HANDCOUNT;                                 // set other player's hand
        memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * INITIAL_HANDCOUNT); // set player's hand
        beforeNumActions = G.numActions;
        beforeHandCount = G.handCount[currentPlayer];
        beforeSupplyCount = G.supplyCount[estate];
        int beforeDiscardCountOfOtherPlayer = G.discardCount[nextPlayer];
        choice1 = 2; // estate position in hand

        printf("--- Check valid choice case. choice2 = %d ---\n", choice2);
        // call refactored function
        result = handleAmbassador(currentPlayer, choice1, choice2, &G, 0);
        // verify the result
        myAssertEqual(G.supplyCount[estate], beforeSupplyCount + choice2 - (numPlayer - 1),
                      "Check the Supply cards");
        myAssertEqual(G.handCount[currentPlayer], beforeHandCount - choice2 - 1,
                      "Check the hand count");
        myAssertEqual(G.discardCount[nextPlayer], beforeDiscardCountOfOtherPlayer + 1,
                      "Check the dicard count of next player becomes +1");
    }

    printf("--- UNIT TEST 3 ENDS ---\n");
    printf("--------------------------\n");
    return 0;
}
