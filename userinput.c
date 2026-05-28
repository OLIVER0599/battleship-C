/*
                userinput.c
                This file is for functions related to handing input and output (e.g. reading players names, displaying board to terminal ... )
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "userinput.h"
#include "constants.h"
#include "player.h"
#include "board.h"
/*
FUNCTION gameStart:
    introducts game
*/
void gameStart(void) {

    printf("Welcome to:\n\t\t\t\t\tBATTLESHIPS!!!!\n\n");
    printf(
        "Each player sets up there ship boards.\n"
        "Then they will each make shots at where they think there opponents ships will be.\n"
    );
}

/*
    FUNCTION: clearInputBuffer
    input: void
    output: void
    Clears the input buffer. (loops through until and grabs a char from the stdin buffer and stops until a newline or an EOF)

*/
void clearInputBuffer(void) {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void trimTrailingSpaces(char *str) {
    int len = strlen(str);

    while (len > 0 && isspace(str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
}

/*
FUNCTION getNumberOfPlayers:
    prompts user for number of players until a valid number is given.
    returns number of players.
*/

int getNumberOfPlayers(void) {

    int numPlayers;
    int result;

    do {

        printf("\n\nHow many players will be playing?   ");

        result = scanf("%d", &numPlayers);

        if (result != 1) {

            printf("\nAn invalid value was given. Please try again.\n\n");
            clearInputBuffer();
        }

        else if (numPlayers <= 1) {

            printf("\nAt least 2 players are needed to play. Please try again.\n\n");
        }

    } while (result != 1 || numPlayers <= 1);

    clearInputBuffer();

    return numPlayers;
}

/*
FUNCTION: readPlayerNames
    input: list of player names (pointer), number of players
    output: void
    for each player, it: 
        i) prompts user for name    
        ii) stores name
    then displays all names
*/
void readPlayerNames(Player *players, int numplayers) {


    for (int i = 0; i < numplayers; i++) {

        printf("\n\nEnter the %dst player's name:   ", i + 1); //prompt user
        fgets(players[i].name, PLAYERNAME_MAX_SIZE, stdin); //store name
        players[i].name[strcspn(players[i].name, "\n")] = '\0'; //clean name (replace newline)
        trimTrailingSpaces(players[i].name);
    }

    for (int j = 0; j < numplayers; j++) { //display names
        printf("\nPlayer %d's name is:  %s", j + 1, players[j].name);
    }
}

/*
FUNCTION displayBoard:
    input: pointer to board, pointer to the player who owns that board
    output: void

    takes in a 2D board (array) and outputs it in a formatted nice way
    control for the type of square (EMPTY, HIT, ...) for the type of output desired later
*/
void displayBoard(Board *board, Player *player) {
    int i = 0;

    printf("\n\n\n");
    printf("\t%s's %s:\n\n",player->name, getBoardType(board));
    

    //print x axis (1 --> COLS)
    printf("\t\t\t\t  ");
    for (int x_val = 1; x_val <= COLS; x_val++) {
        printf("%2d ", x_val);
    }

    printf("\n\n");

    for (i = 0; i < ROWS; i++) {

        //print y axis (A --> J)
        printf("\t\t\t\t");
        printf("%c  ", 'A' + i);
        
        for (int j = 0; j < COLS; j++) {
            printf("%c  ", board->cells[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n\n");
}
