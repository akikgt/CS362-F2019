/* -----------------------------------------------------------------------
 * Unit test for my refactored Mine function handleMine
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
    int handForTest[5] = {mine, copper, copper, gold, estate};
    int beforeNumActions, beforeHandCount, beforeSilverInSupply;

    printf (BOLD "--- UNIT TEST 5 STARTS ---\n" RESETFONT);

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
    choice1 = 0;

    printf("--- Check invalid choice cases ---\n");
    // Case: choice1 is invalid
    choice1 = 5; // position 5 is estate card. it is less than copper 
    choice2 = gold;
    // call refactored function
    result = handleMine(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid chice1 (less than copper enum value)");

    // Case: choice1 is invalid
    choice1 = 0; // postion 0 is mine itself and it is greater than gold
    choice2 = gold;
    // call refactored function
    result = handleMine(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid chice1 (greater than gold enum value)");

    // Case: choice2 is invalid (less than copper)
    choice1 = 1;
    choice2 = curse;
    // call refactored function
    result = handleMine(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid chioice2 (less than copper enum value) case");
    // Case: choice2 is invalid (greater than gold)
    choice1 = 1;
    choice2 = treasure_map;
    // call refactored function
    result = handleMine(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid chioice2 (greater than gold enum value) case");

    // Case: choice2 is invalid (invalid card value)
    choice1 = 1;
    choice2 = -1;
    // call refactored function
    result = handleMine(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid choice2 (invalid card enum value -1)");

    // Case: choice2 is invalid (invalid card value)
    choice1 = 1;
    choice2 = 27;
    // call refactored function
    result = handleMine(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid choice2 (invalid card enum value 27)");

    // Case: the cost of choice2 card is greather than choice1 + 3
    choice1 = 1; // copper card (cost 0)
    choice2 = gold; // the cost of gold is 6
    // call refactored function
    result = handleMine(currentPlayer, choice1, choice2, &G, 0);
    // verify the result
    myAssertEqual(result, -1,
                  "Check invalid choice2 (the cost of the card is too high)");


    /* ------------------------------------------------ 
     *  Testing valid choices
     * ------------------------------------------------ 
    */

    printf("--- Check valid case. Discard a copper and get a silver card---\n");
    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = INITIAL_HANDCOUNT;
    G.handCount[nextPlayer] = INITIAL_HANDCOUNT; // set other player's hand
    memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * INITIAL_HANDCOUNT); // set player's hand
    beforeNumActions = G.numActions;
    beforeHandCount = G.handCount[currentPlayer];
    beforeSilverInSupply = G.supplyCount[silver]; // save the supply count of silver

    choice1 = 1; // copper card
    choice2 = silver; // the cost of silver is 3
    int beforeCopperInHand = countCardInHand(currentPlayer, copper, &G);

    result = handleMine(currentPlayer, choice1, choice2, &G, 0);
    myAssertEqual(G.handCount[currentPlayer], beforeHandCount - 1,
                  "Check the hand count");
    myAssertEqual(G.supplyCount[silver], beforeSilverInSupply - 1,
                  "Check the supply count of silver card after Mine");
    myAssertEqual(countCardInHand(currentPlayer, silver, &G), 1,
                  "Check the number of silver cards in hand");
    myAssertEqual(countCardInHand(currentPlayer, copper, &G), beforeCopperInHand - 1,
                  "Check the copper card discarded correctly");

    printf (BOLD "--- UNIT TEST 5 ENDS ---\n" RESETFONT);
    return 0;
}
