/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * testUpdateCoins: testUpdateCoins.c dominion.o rngs.o
 *      gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)
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
    }

    // copper is cost 0 and gold is cost 6. Therefore, mine cannot get gold by trashing a copper
    // check the number of copper cards in hand is the same as before playing mine
    assert(nCopper == 1); 

    // check the number of gold cards in hand is the same as before playing mine
    assert(nGold == 1); 


    // printf("%d %d\n", nCopper, nGold);



    // assert(G.hand[currentPlayer][baronPos] != baron); // check whether baron was discarded
    // assert(G.handCount[currentPlayer] == 4); // check whether baron was discarded

    // printf("handcount is %d\n", G.handCount[p]);
    // printf("%d\n", G.hand[p][3]);

    // updateCoins(p, &G, 0);

    // printf("check coin %d\n", G.coins);
    // printf("check hand %d\n", G.handCount[p]);
    // // playCard(0, 0, 0, 0, &G);
    // printf("check coin %d\n", G.coins);
    // printf("check hand %d\n", G.handCount[p]);

    // for (int i = 0; i < G.handCount[p]; i++)
    // {
    //     printf("%d\n", G.hand[p][i]);
    // }

    // printf("mine test starts\n");

    // playCard(1, 3, smithy, 0, &G);

    // after playing the baron, it should be discarded.
    // assert(G.hand[p][0] != baron);

    printf("All tests passed!\n");

    return 0;
}
