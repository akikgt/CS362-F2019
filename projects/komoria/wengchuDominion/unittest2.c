/* -----------------------------------------------------------------------
 * Unit test for my refactored Minion function handleMinion
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

    // set kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};

    struct gameState G;
    int currentPlayer = 0; 
    int nextPlayer = 1; 
    int handForTest[5] = {minion, mine, copper, gold, estate};
    int beforeNumActions, beforeHandCount, beforeCoins;

    printf ("--------------------------\n");
    printf ("--- UNIT TEST 2 STARTS ---\n");

    /* ------------------------------------------------ 
     *  Testing minion with choice1 = 1
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
    choice1 = 1;
    choice2 = 0;

    // call refactored function
    // handleMinion(currentPlayer, choice1, choice2, &G, 0);
    minionR(choice1, choice2, currentPlayer, &G, 0, NULL);

    // verify the result
    printf("--- check minion with choice1 = 1 ---\n");
    myAssertEqual(G.numActions, beforeNumActions + 1,
                  "Check Actions count");
    myAssertEqual(countCardInHand(currentPlayer, minion, &G), 0,
                  "Check minion discarded after use");
    myAssertEqual(G.coins, beforeCoins + 2,
                  "Check coins");
    myAssertEqual(G.handCount[nextPlayer], INITIAL_HANDCOUNT,
                  "Check other player's hand count unaffected");

    /* ------------------------------------------------ 
     *  Testing minion with choice2 = 1
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
    choice2 = 1;

    // call refactored function
    // handleMinion(currentPlayer, choice1, choice2, &G, 0);
    minionR(choice1, choice2, currentPlayer, &G, 0, NULL);

    // verify the result
    printf("--- check minion with choice1 = 1 ---\n");
    myAssertEqual(G.numActions, beforeNumActions + 1,
                  "Check Actions count");
    myAssertEqual(G.handCount[currentPlayer], 4,
                  "Check current player's hand count after choice2");
    myAssertEqual(G.coins, beforeCoins,
                  "Check coins not updated");
    myAssertEqual(G.handCount[nextPlayer], 4,
                  "Check other player's hand count changed");

    /* ------------------------------------------------ 
     *  Testing minion with choice2 = 1 but other player's hand is less than 5
     * ------------------------------------------------ 
    */

    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = INITIAL_HANDCOUNT;
    G.handCount[nextPlayer] = 0; // set other player's hand
    memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * INITIAL_HANDCOUNT); // set player's hand
    beforeNumActions = G.numActions;
    beforeHandCount = G.handCount[currentPlayer];
    beforeCoins = G.coins;
    choice1 = 0;
    choice2 = 1;

    // call refactored function
    // handleMinion(currentPlayer, choice1, choice2, &G, 0);
    minionR(choice1, choice2, currentPlayer, &G, 0, NULL);

    // verify the result
    printf("--- check minion with other player's hand is less than 5 ---\n");
    myAssertEqual(G.numActions, beforeNumActions + 1,
                  "Check Actions count");
    myAssertEqual(G.handCount[currentPlayer], 4,
                  "Check current player's hand count after choice2");
    myAssertEqual(G.coins, beforeCoins,
                  "Check coins not updated");
    myAssertEqual(G.handCount[nextPlayer], 0,
                  "Check other player's hand count not changed"); 

    printf ("--- UNIT TEST 2 ENDS ---\n");
    printf ("--------------------------\n");
    return 0;
}
