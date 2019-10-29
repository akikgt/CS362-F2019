#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    // TODO: rewrite this function

    // char pool = all lowercase alphabet + parentheses + space -> 33 characters
    char charPool[] = "abcdefghijklmnopqrstuvwxyz[](){} ";

    int idx = rand() % strlen(charPool);

    // printf("%d\n", strlen(charPool));
    // printf("%c\n", charPool[idx]);
    // exit(1);
    return charPool[idx];
}

char *inputString()
{
    // TODO: rewrite this function
    char stringCharPool[] = "abcdefghijklmnopqrstuvwxyz";
    // assume string length is less than or equal to 5
    // all string consist of all lowercase alphabet characters
    char *str = malloc(6);
    str[5] = '\0';
    for (int i = 0; i < 5; i++) {
      // str[i] = inputChar();
      int idx = rand() % strlen(stringCharPool);
      str[i] = stringCharPool[idx];
    }
    return str;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
