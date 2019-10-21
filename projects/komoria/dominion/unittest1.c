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

int main() {
    int seed = 1000;
    int numPlayer = 2;
    // int maxBonus = 10;
    int r, handCount;
    // int bonus;

    // set kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};

    struct gameState G;
    int maxHandCount = 5;

    printf ("TESTING STARTS\n");
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game

    // set the state to play the mine card
    int currentPlayer = 0; 
    handCount = 5;
    // set the hand to have 1 mine, 1 copper and 1 gold
    int handForTest[5] = {baron, mine, copper, gold, estate};
    int minePos = 1; // set mine position in hand
    int copperPos = 2; // set copper position in hand
    int goldPos = 3; // set gold position in hand
    G.handCount[currentPlayer] = handCount;
    memcpy(G.hand[currentPlayer], handForTest, sizeof(int) * handCount); // set player's hand

    // play the mine card, discarding copper card and tries to get smithy
    playCard(minePos, copperPos, gold, 0, &G);

    // count the number of copper and gold in hand after playing mine
    int nCopper = 0;
    int nGold = 0;
    for (int i = 0; i < G.handCount[currentPlayer]; i++)
    {
        if (G.hand[currentPlayer][i] == copper) nCopper++;
        if (G.hand[currentPlayer][i] == gold) nGold++;
    printf("All tests passed!\n");

    return 0;
}
