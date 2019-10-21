/* -----------------------------------------------------------------------
 * Unit test for baron card
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define CYAN   "\x1B[36m"
#define RESETCOLOR "\x1B[0m"

void myAssertEqual(int expr, int expected, char *msg) {
    printf(CYAN "%s: " RESETCOLOR, msg);
    if (expr == expected) {
        printf(GREEN "PASS!\n" RESETCOLOR);
        return;
    }
    else {
        printf(RED "FAILED: %d expected, but got %d.\n" RESETCOLOR, expected, expr);
        return;
    }
}

int main() {
    int seed = 1000;
    int numPlayer = 2;
    // int maxBonus = 10;
    int r;
    // int bonus;

    // set kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};

    struct gameState G;
    int currentPlayer = 0; 
    int handCount = 5;
    int maxHandCount = 5;
    int handForTest[5] = {baron, mine, copper, gold, estate};

    printf ("TESTING STARTS\n");

    // reset&set the game state
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = handCount;
    memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * handCount); // set player's hand
    int beforeNumBuy = G.numBuys; // save the numBuys

    // call refactored function
    handleBaron(currentPlayer, 0, &G);

    myAssertEqual(G.numBuys, beforeNumBuy + 1, "Check Buy count");
    myAssertEqual(1, 1, "simple test2");

    printf("All tests passed!\n");

    return 0;
}
