/* -----------------------------------------------------------------------
 * Unit test for my refactored baron function handleBaron
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
#define BOLD "\x1b[1m"
#define RESETFONT "\x1B[0m"

void myAssertEqual(int expr, int expected, char *msg) {
    printf(CYAN "%s: " RESETFONT, msg);
    if (expr == expected) {
        printf(GREEN "PASS!: got %d\n" RESETFONT, expr);
        return;
    }
    else {
        printf(RED "FAILED: %d expected, but got %d.\n" RESETFONT, expected, expr);
        return;
    }
}

int countCardInHand(int currentPlayer, int card, struct gameState *G) {
    int count = 0;
    for (int i = 0; i < G->handCount[currentPlayer]; i++) {
        if (G->hand[currentPlayer][i] == card)
            count++;
    }
    return count;
}

int countCardInDiscard(int currentPlayer, int card, struct gameState *G) {
    int count = 0;
    for (int i = 0; i < G->discardCount[currentPlayer]; i++) {
        if (G->discard[currentPlayer][i] == card)
            count++;
    }
    return count;
}

int main() {
    int seed = 1000;
    int numPlayer = 2;
    // int maxBonus = 10;
    int r;
    int choice1, choice2, choice3;

    // set kingdom cards
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};

    struct gameState G;
    int currentPlayer = 0; 
    int handCount = 5;
    int maxHandCount = 5;
    int handForTest[5] = {baron, mine, copper, gold, estate};
    int beforeNumBuy, beforeHandCount, beforeNumEstate;

    printf (BOLD "--- UNIT TEST 1 STARTS ---\n" RESETFONT);

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

    // reset&set variables
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[currentPlayer] = handCount;
    memcpy(G.hand[currentPlayer], handWithoutEstate, sizeof(int) * handCount); // set player's hand
    beforeNumBuy = G.numBuys; // save the numBuys
    beforeHandCount = G.handCount[currentPlayer];
    beforeNumEstate = G.supplyCount[estate];
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

    printf (BOLD "--- UNIT TEST 1 ENDS ---\n" RESETFONT);
    return 0;
}
