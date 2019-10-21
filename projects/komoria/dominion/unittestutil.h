#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define CYAN   "\x1B[36m"
#define BOLD "\x1b[1m"
#define RESETFONT "\x1B[0m"

void myAssertEqual(int expr, int expected, char *msg);
int countCardInHand(int currentPlayer, int card, struct gameState *G);
int countCardInDiscard(int currentPlayer, int card, struct gameState *G);