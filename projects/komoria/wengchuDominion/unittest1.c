/* -----------------------------------------------------------------------
 * Unit test for my refactored Baron function handleBaron
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "unittestutil.h"

int main() {
    int seed = 1000;
    int numPlayer = 2;
    int r;
    int choice1;

    // set kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};

    struct gameState G;
    int currentPlayer = 0; 
    int handCount = 5;
    int handForTest[5] = {baron, mine, copper, gold, estate};
    int beforeNumBuy, beforeHandCount, beforeNumEstate, beforeCoins;

    printf ("--------------------------\n");
    printf ("--- UNIT TEST 1 STARTS ---\n");

    /* ------------------------------------------------ 
     *  Testing Baron with choice1 = positive
     * ------------------------------------------------ 
    */

    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = handCount;
    memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * handCount); // set player's hand
    beforeNumBuy = G.numBuys; // save the numBuys
    beforeHandCount = G.handCount[currentPlayer];
    beforeNumEstate = G.supplyCount[estate];
    beforeCoins = G.coins;
    choice1 = 1;

    // call refactored function
    handleBaron(currentPlayer, choice1, &G);

    // verify the result
    printf("--- baron with choice1 = 1 ---\n");
    myAssertEqual(G.numBuys, beforeNumBuy + 1,
                  "Check Buy count");
    myAssertEqual(countCardInHand(currentPlayer, baron, &G), 0,
                  "Check baron discarded after use");
    myAssertEqual(countCardInHand(currentPlayer, estate, &G), 0,
                  "Check estate discarded after playing baron");
    myAssertEqual(G.handCount[currentPlayer], beforeHandCount - 2,
                  "Check hand count");
    myAssertEqual(G.supplyCount[estate], beforeNumEstate - 1,
                  "Check supply count of estate card");
    myAssertEqual(countCardInDiscard(currentPlayer, estate, &G), 0,
                  "Check the number of estate cards in discard");
    myAssertEqual(G.coins, beforeCoins + 4,
                  "Check coins");

    /* ------------------------------------------------ 
     *  Testing Baron with choice1 = 0
     * ------------------------------------------------ 
    */

    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = handCount;
    memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * handCount); // set player's hand
    beforeNumBuy = G.numBuys; // save the numBuys
    beforeHandCount = G.handCount[currentPlayer];
    beforeNumEstate = G.supplyCount[estate];
    beforeCoins = G.coins;
    choice1 = 0;

    // call refactored function
    handleBaron(currentPlayer, choice1, &G);

    // verify the result
    printf("--- baron with choice1 = 0 ---\n");
    myAssertEqual(G.numBuys, beforeNumBuy + 1,
                  "Check Buy count");
    myAssertEqual(G.handCount[currentPlayer], beforeHandCount - 1,
                  "Check hand count");
    myAssertEqual(countCardInHand(currentPlayer, baron, &G), 0,
                  "Check baron discarded after use");
    myAssertEqual(G.supplyCount[estate], beforeNumEstate - 1,
                  "Check supply count of estate card");
    myAssertEqual(countCardInDiscard(currentPlayer, estate, &G), 1,
                  "Check the number of estate cards in discard");
    myAssertEqual(G.coins, beforeCoins,
                  "Check coins");

    /* ------------------------------------------------ 
     *  Testing Baron with choice1 = negative
     * ------------------------------------------------ 
    */

    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = handCount;
    memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * handCount); // set player's hand
    beforeNumBuy = G.numBuys; // save the numBuys
    beforeHandCount = G.handCount[currentPlayer];
    beforeNumEstate = G.supplyCount[estate];
    beforeCoins = G.coins;
    choice1 = -1;

    // call refactored function
    handleBaron(currentPlayer, choice1, &G);

    // verify the result
    printf("--- baron with choice1 = -1 ---\n");
    myAssertEqual(G.numBuys, beforeNumBuy + 1,
                  "Check Buy count");
    myAssertEqual(G.handCount[currentPlayer], beforeHandCount - 1,
                  "Check hand count");
    myAssertEqual(countCardInHand(currentPlayer, baron, &G), 0,
                  "Check baron discarded after use");
    myAssertEqual(G.supplyCount[estate], beforeNumEstate - 1,
                  "Check supply count of estate card");
    myAssertEqual(countCardInDiscard(currentPlayer, estate, &G), 1,
                  "Check the number of estate cards in discard");
    myAssertEqual(G.coins, beforeCoins,
                  "Check coins");


    /* ------------------------------------------------ 
     *  Testing Baron without estate card in hand
     * ------------------------------------------------ 
    */

    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = handCount;
    int handWithoutEstate[5] = {baron, copper, copper, copper, copper};
    memcpy(G.hand[currentPlayer], handWithoutEstate, sizeof(int) * handCount); // set player's hand
    beforeNumBuy = G.numBuys; // save the numBuys
    beforeHandCount = G.handCount[currentPlayer];
    beforeNumEstate = G.supplyCount[estate];
    beforeCoins = G.coins;
    choice1 = 1;

    // call refactored function
    handleBaron(currentPlayer, choice1, &G);

    // verify the result
    printf("--- No estate card in hand with choice1 = positive ---\n");
    myAssertEqual(G.numBuys, beforeNumBuy + 1,
                  "Check Buy count");
    myAssertEqual(G.handCount[currentPlayer], beforeHandCount - 1,
                  "Check hand count");
    myAssertEqual(countCardInHand(currentPlayer, baron, &G), 0,
                  "Check baron discarded after use");
    myAssertEqual(countCardInHand(currentPlayer, estate, &G), 0,
                  "Check estate card in hand");
    myAssertEqual(G.supplyCount[estate], beforeNumEstate - 1,
                  "Check supply count of estate card");
    myAssertEqual(countCardInDiscard(currentPlayer, estate, &G), 1,
                  "Check the number of estate cards in discard");
    myAssertEqual(G.coins, beforeCoins,
                  "Check coins");

    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = handCount;
    memcpy(G.hand[currentPlayer], handWithoutEstate, sizeof(int) * handCount); // set player's hand
    beforeNumBuy = G.numBuys; // save the numBuys
    beforeHandCount = G.handCount[currentPlayer];
    beforeNumEstate = G.supplyCount[estate];
    beforeCoins = G.coins;
    choice1 = -1;

    // call refactored function
    handleBaron(currentPlayer, choice1, &G);

    // verify the result
    printf("--- No estate card in hand with choice1 = negative ---\n");
    myAssertEqual(G.numBuys, beforeNumBuy + 1,
                  "Check Buy count");
    myAssertEqual(G.handCount[currentPlayer], beforeHandCount - 1,
                  "Check hand count");
    myAssertEqual(countCardInHand(currentPlayer, baron, &G), 0,
                  "Check baron discarded after use");
    myAssertEqual(countCardInHand(currentPlayer, estate, &G), 0,
                  "Check estate card in hand");
    myAssertEqual(G.supplyCount[estate], beforeNumEstate - 1,
                  "Check supply count of estate card");
    myAssertEqual(countCardInDiscard(currentPlayer, estate, &G), 1,
                  "Check the number of estate cards in discard");
    myAssertEqual(G.coins, beforeCoins,
                  "Check coins");

    printf ("--- UNIT TEST 1 ENDS ---\n");
    printf ("--------------------------\n");
    return 0;
}
