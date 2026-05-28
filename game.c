#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "game.h"
#include "userinput.h"
#include "placement.h"
#include "save.h"
#include "load.h"
#include "ai.h"
#include "move.h"      
#include "player.h"    

/*
FUNCTION gameHasEnded
    input: players, number of players
    output: TRUE or FALSE

    determines if game has ended or not:
        loops through each player. checks if all but one player is alive

*/
int gameHasEnded(Player *players, int numPlayers) {

    int numberOfAlivePlayers = 0;

    for (int i = 0; i < numPlayers; i++) {

        if (players[i].isAlive == 1) {

            numberOfAlivePlayers++;
        }
    }

    return numberOfAlivePlayers <= 1;
}

/*
FUNCTION turnFinder
    input: int total turns, int number of players
    output: int (returns the index for the current player in players)
*/
int turnFinder(int totalTurns, int numPlayers) {

    return (totalTurns % numPlayers);
}

void printListofAlivePlayers(Player *players, int numPlayers) {
    
    for (int i = 0; i < numPlayers; i++) {

        if (players[i].isAlive == 1) {

            printf("\t\t%s\n", players[i].name);
        }
    }

    printf("\n\n");
}

/*
FUNCTION: OpponentNameValid
    input: opponentname (string)
    output: players (list of players)
    returns true/false if opponent name is valid

    case 1: same as current players name
    case 2: player is dead

    (the case for whether the name is not in the players list is handled through opponentIndex finder)
*/
int OpponentNameValid(int opponentIndex, int currentPlayerIndex, Player *players) {

    // opponents name was entered as currentplayers own name
    if (opponentIndex == currentPlayerIndex) {

        printf("You have entered your own name. Please do again.\n\n");
        return 0;
    }
    // the opponent is already eliminated
    else if (players[opponentIndex].isAlive == 0) {

        printf("\n%s has already been eliminated.\n\n", players[opponentIndex].name);
        return 0;
    }

    return 1;
}

/*
FUNCTION: findOpponentIndex
    input: opponentname (string)
    output: players (list of players)
    returns the index at which the opponent is in the players list

    returns index if found, else returns -1;
*/
int findOpponentIndex(Player *players, char opponentName[], int numPlayers) {

    for (int i = 0; i < numPlayers; i++) {

        if (strcmp(opponentName, players[i].name) == 0) {

            return i;
        }
    }

    return -1;
}
/*
FUNCTION getOpponent
    input: players, number of players, currentplayer index, opponentiNDEX (POINTER)
    output:  - returns the opponent as a pointer (type Player)
             - opponentIndex passed in gets parsed the index that the opponent lies in



*/

Player *getOpponent(Player *players, int numPlayers, int currentPlayerIndex, int *opponentIndex) {

    //prompt for opponent name
        char playerNameTemp[PLAYERNAME_MAX_SIZE];
        char *result;


        int opponentNameIsValid = 0;
        int foundIndex = -1;
        

        printf("\n\n");
        printf("PLAYERS ALIVE ARE: \n\n");
        printListofAlivePlayers(players, numPlayers);

        do {

            printf("Which opponent you would like to attack? ENTER THEIR NAME:  ");
            result = fgets(playerNameTemp, PLAYERNAME_MAX_SIZE, stdin); //store name

            //did fgets fail
            if (result == NULL) {

                printf("FAILIURE TO RECEIVE OPPONENTS NAME\n");
                continue;
            }

            //clean name
            playerNameTemp[strcspn(playerNameTemp, "\n")] = '\0'; //clean name (replace newline)
            trimTrailingSpaces(playerNameTemp);

            //entered a blank name
            if (strlen(playerNameTemp) == 0) {

                printf("\nYou didn't enter anything. Please enter a name.\n");
                foundIndex = -1;
                continue;
            }

            //find the index of player
            foundIndex = findOpponentIndex(players, playerNameTemp, numPlayers);

            

            //was the opponents name found in players
            if (foundIndex == -1) {

                printf("\nopponents name was not found amongst the players\n");
            }

            // was the opponents name valid
            else  {
    
                opponentNameIsValid = OpponentNameValid(foundIndex, currentPlayerIndex, players);
            }

        } while (result == NULL || foundIndex == -1 || !opponentNameIsValid);

        //valid opponent was selected
        
        *opponentIndex = foundIndex; //return opponentIndex as pointer
        return &players[*opponentIndex];

}



/*
FUNCTION aiGetOpponent
    input: players, current player index, opponent player index
    output: the ai's chosen opponent

    loop through the players and just select the fist alive player who isnt the ai
*/
Player *aiGetOpponent(Player *players, int numPlayers, int currentPlayerIndex, int *opponentIndex) {

    /* collect all valid opponents (alive and not self) */
    int validIndices[numPlayers];
    int validCount = 0;

    for (int i = 0; i < numPlayers; i++) {

        if (i != currentPlayerIndex && players[i].isAlive) {
            validIndices[validCount] = i;
            validCount++;
        }
    }

    if (validCount == 0) return NULL; /* should never happen in a valid game */

    /* pick one randomly */
    int chosen = validIndices[rand() % validCount];

    *opponentIndex = chosen;
    return &players[chosen];
}



void printCurrentPlayersTurn(Player *currPlayer) {

    printf("\n====================================\n");
    printf("%s's TURN\n", currPlayer->name);
    printf("====================================\n");
}

void announceWhoOpponentIs(Player *opponent, Player *currentPlayer) {
    printf("\n====================================\n");
    printf("%s's TURN\n", currentPlayer->name);
    printf("Targeting: %s\n", opponent->name);
    printf("====================================\n");
}

/*
FUNCTION findWinnerIndex
    input: players list, number of players
    output: the index for winner in the list of players
            if no winner is found, -1 is returned

*/

int findWinnerIndex(Player *players, int numPlayers) {

    for (int i = 0; i < numPlayers; i++) {

        if (players[i].isAlive) {

            return i;
        }
    }

    return -1;
}
/*
FUNCTION playGame
    input: list of players, number of players
    output: void (modifies players until single alive player)

    playGame takes turns until teh game has ended. For each turn, it 
    determines the player whose turn it is, the players choice of opponent, and displays the players current shipboard and his guessboard
    it then prompts user for the guess and processes that guess, updating ships and boards apppriately depending of the sucess of the ship

*/
void playGame(Player *players, int numPlayers, Move **moveHistory, int *totalTurns) {

    while (!gameHasEnded(players, numPlayers)) {

        int currentPlayerIndex = turnFinder(*totalTurns, numPlayers);
        Player *currentPlayer  = &players[currentPlayerIndex];
        printCurrentPlayersTurn(currentPlayer);

        /* skip eliminated players */
        if (currentPlayer->isAlive == 0) {
            (*totalTurns)++;
            continue;
        }

        int opponentIndex;
        Player *opponent;
        int guessX, guessY;
        MoveData moveData;

        /*  AI TURN ─ */
        if (currentPlayer->isAI) {

            /* AI picks opponent automatically */
            opponent = aiGetOpponent(players, numPlayers, currentPlayerIndex, &opponentIndex);

            if (opponent == NULL) {
                printf("[AI] ERROR: no valid opponent found.\n");
                (*totalTurns)++;
                continue;
            }

            announceWhoOpponentIs(opponent, currentPlayer);

            /* AI decides where to shoot */
            aiTakeTurn(
                &currentPlayer->aiState,
                &currentPlayer->guessBoards[opponentIndex],
                &guessX,
                &guessY
            );

            //process the shot (identical to human flow)
            moveData = processShot(
                currentPlayer, opponent, opponentIndex,
                guessX, guessY, numPlayers, *totalTurns
            );

            addMove(moveHistory, moveData);

            //AI learns from the result 
            aiUpdateState(&currentPlayer->aiState, moveData, guessX, guessY);
        }

        /* ── HUMAN TURN ── */
        else {

            //ask if user wants to quit 
            char answer;
            int scanSuccess;

            printf("\n\t\t\t\t\tQUIT GAME? (y/n):  ");
            scanSuccess = scanf(" %c", &answer);

            if (scanSuccess != 1) {
                printf("\nfailed to determine if player wants to quit.\n");
            }

            else {
                clearInputBuffer();
                if (tolower(answer) == 'y') {
                    saveGame(players, numPlayers, *totalTurns, *moveHistory);
                    return;
                }
            }

            /* human picks opponent */
            opponent = getOpponent(players, numPlayers, currentPlayerIndex, &opponentIndex);
            announceWhoOpponentIs(opponent, currentPlayer);

            /* display boards */
            printf("\nYOUR SHIP BOARD\n\n");
            displayBoard(&currentPlayer->shipBoard, currentPlayer);
            printf("\nYOUR GUESS BOARD FOR YOUR CHOSEN TARGET: %s BOARD\n\n", opponent->name);
            displayBoard(&currentPlayer->guessBoards[opponentIndex], opponent);

            /* human decides where to shoot */
            takeTurn(currentPlayer, opponent, opponentIndex, &guessX, &guessY);

            moveData = processShot(
                currentPlayer, opponent, opponentIndex,
                guessX, guessY, numPlayers, *totalTurns
            );

            addMove(moveHistory, moveData);
            /* no aiUpdateState for humans */
        }

        (*totalTurns)++;
    }

    Player *winner = &players[findWinnerIndex(players, numPlayers)];
    printf(
        "\n\nGame Over!!!\n"
        "\n\nThe winner is: %s\n",
        winner->name
    );
}


void takeTurn(Player *currentPlayer, Player *opponent, int opponentIndex, int *guessX, int *guessY) {
    

    //1. get shot coordinates
    char rowchar;
    int result; //flag: reading success
    int validPlacement; //flag: valid coordinates
    
    printf(
        "ENTER COORDINATES\n\n"
        "\nCoordinates are entered as: x y \n"
        "x = coloumn, increases from left to right (1 -> %d)\n"
        "y = row, increases from top to bottom (A -> %c)\n\n",
        COLS, ('A' + ROWS - 1)
    );

    do {
        
        validPlacement = 1; 

        printf("\nEnter your shot's coordinates:   ");
        result = scanf(" %d %c", guessX, &rowchar); // get coordinates and direction


        // did scanf work?
        if (result != 2) {

            printf("\n\n\tYou have entered an invalid coordinates/directions. Please try again\n");
            clearInputBuffer(); //scanf failed, safely get rid of anything bad just in case
        }

        else  {

            clearInputBuffer();

            //process the x, y coordinates for internal use
            rowchar = toupper(rowchar);
            *guessX = *guessX - 1;
            *guessY = (rowchar - 'A');

            //are coordinates valid?
            // are coordinates in bounds
                if (isCoordinatesInBounds(*guessX, *guessY) == 0) {

                    printf("\nCoordinates are out of bounds. Please try again.\n\n");
                    validPlacement = 0;
                }
            // shot was made somewhere you have shot already
                else if (shotAlreadyTaken(&currentPlayer->guessBoards[opponentIndex], *guessX, *guessY)){
                    
                    printf("\nYou have already made a shot at this location. Please try again\n\n");
                    validPlacement = 0;
                }
            }
        
    printf("\n\n\n");

    } while (result != 2 || validPlacement == 0);

}


int shotAlreadyTaken(Board *guessBoard, int guessX, int guessY) {

    return (guessBoard->cells[guessY][guessX] != EMPTY);
}

/*
FUNCTION processShot
    input: player, opponent, guessX and guessY 
    output: void (modifiers player and opponents boards and possibly opponents ship)

    determine what kind of shot is it:
    1. hits ship ->
    2. misses ship
    3. invalid shot (already done before)


*/

MoveData processShot(Player *currentPlayer, Player *opponent, int opponentIndex, int guessX, int guessY, int numPlayers, int turnNumber) {

    Board *guessBoard = &currentPlayer->guessBoards[opponentIndex]; // grab teh appropriate guess board for this opponent
    char targetCell = opponent->shipBoard.cells[guessY][guessX];

    MoveData moveData = {
        .x = guessX,
        .y = guessY,
        .targetIndex = opponentIndex,
        .shooterIndex = currentPlayer->index,
        .shipHit = EMPTY,
        .shipSunk = 0,
        .playerKilled = 0,
        .turnNumber = turnNumber
    };

    //if shot is a miss
    if (targetCell == EMPTY) {
        printf("\nMiss!\n\n");



        //update current players guesses and opponents shipboard
        guessBoard->cells[guessY][guessX] = MISS;
        opponent->shipBoard.cells[guessY][guessX] = MISS;

        moveData.result = MISS;
        return moveData;
    }

    //shot has already been made (either by current player or another player)
    if (targetCell == HIT || targetCell == MISS) {

        printf("\nThat coordinate has already been shot. \n");

        if (numPlayers > 2) {

            printf("Either by you or someone else\n");
        }
        
        moveData.result = INVALID_SHOT;
        return moveData;
    }
    // else shot must have hit a ship
    //update current players guesses and opponents shipboard

    printf("\nHit!\n\n");
    guessBoard->cells[guessY][guessX] = HIT;
    opponent->shipBoard.cells[guessY][guessX] = HIT;

    //update the ship
    Ship *struckedShip = updateHitShip(opponent, targetCell);

    moveData.result = HIT;
    moveData.shipHit = targetCell;

    if (struckedShip->sunk) {

        moveData.shipSunk = 1;

        if (opponent->isAlive == 0) {

            moveData.playerKilled = 1;

        }
    }

    return moveData;

}


Ship *findShipBySymbol(Player *player, char shipSymbol) {

    for (int i = 0; i < player->numShips; i++) {

        if (player->ships[i].shipSymbol == shipSymbol) {

            return &player->ships[i];
        }
    }
    return NULL;
}

/*
FUNCTION updateHitShip:
    input: player opponent, char ship symbol
    output: void (modifies opponent's ship)

    1. locates the ship that was hit (return struckedShip)
    2. increment struckedShip's hits
    3. if struckedShips hits == size:
        T-> struckedShip
             sunk = 1, 
             opponents number of ships--
             announce you have sunk a ship
             if (opponents number of ships == 0)
                T->opponent.isalive == 0;
                announce death of opponent;
        
    4. 
    
    
*/

/*
returns pointer to struck ship

*/
Ship *updateHitShip(Player *opponent, char shipSymbol) {

    //1. locate the ship that was hit
    Ship *struckedShip = findShipBySymbol(opponent, shipSymbol);

    if (struckedShip == NULL) return NULL;

    //2. increment struckedShips hits
    struckedShip->hits++;

    //3. check for case of (sunk ship)
    if (struckedShip->hits == struckedShip->size) {

        struckedShip->sunk = 1;
        opponent->shipsRemaining--;

        printf(
            "\nYou have sunk %s's %s!!\n\n",
            opponent->name, struckedShip->name
        );

        // opponent has died
        if (opponent->shipsRemaining == 0) {

            opponent->isAlive = 0;
            printf(
                "\nYou have killed %s'!!\n\n",
                opponent->name
            );
        }
    }
    return struckedShip;
}

void setupGame(Player **players, int *numPlayers, Move **moveHistory, int *totalTurns) {

      /*
    if (savefile exists):
        ask "LOAD SAVED GAME?" 
            if y->loadGame()
            if n-> start new game normally 
    playGame()
        after each turn: 
            ask "QUIT GAME? y/n"
                if y-
                    ask "save game y/n"
                        yes -> saveGame()
                    exit playGame() (need to parse in total turns)


    */

    //initialise game variables
    *players = NULL;
    *moveHistory = NULL;
    *numPlayers = 0;
    *totalTurns = 0;

    //if there is a saved file
    if (saveFileExists()) { // ask to load teh file

        char choice;

        printf("\nSaved game found. Load it? (y/n):  ");
        scanf(" %c", &choice);
        clearInputBuffer();

        if (tolower(choice) == 'y') { // if user wants to load teh file:

            loadGame(players, numPlayers, totalTurns, moveHistory);

            if (*players != NULL) return; //if the game loaded successfully and player information was delivered then go to playGame()
            printMoves(*moveHistory);
        }

    }
    //if there is no save file or user doesnt want to continue their game

    gameStart();

    *numPlayers = getNumberOfPlayers();
    printf("The number of players you entered was %d", *numPlayers);
    *players = createPlayers(*numPlayers);

    *moveHistory = NULL;

    readPlayerNames(*players, *numPlayers);
    initialisePlayers(*players, *numPlayers);


    //ask the players to decide if its a human or ai
    printf("\n\n--- Player Type Setup ---\n");

    int i = 0;
    char choice;
    do {
        
        int resultOfScan;

        printf("\nIs %s a human or AI player? (h/a):  ", (*players)[i].name);
        resultOfScan = scanf(" %c", &choice);
        clearInputBuffer();

        if (resultOfScan != 1)
        {
            printf("failed to scan response.Please try again. \n");
            continue;

        }

        choice = tolower(choice);

        if (choice == 'a') {

            (*players)[i].isAI = 1;
            initAIState(&(*players)[i].aiState);
            printf("%s will be controlled by the AI.\n", (*players)[i].name);

            i++;
        }

        else if (choice == 'h') {

            (*players)[i].isAI = 0;
            printf("%s will be controlled by a human.\n", (*players)[i].name);

            i++;
        }
        
        else {
            printf("You have entered an invalid response. Please enter (a or h)\n");
            
        }

    } while (i < *numPlayers);

    

    setUpShips(*players, *numPlayers, NUM_OF_SHIPS); // human manually sets up ships
}

void cleanupGame(Player *players, int numPlayers, Move *moveHistory)
{   

    for (int i = 0; i < numPlayers; i++) {
        free(players[i].guessBoards);  
    }

    freeMoves(moveHistory);
    free(players);
}

int saveFileExists(void) {

    FILE *fp = fopen(SAVE_FILENAME, "r");

    if (fp == NULL) {
        return 0;
    }

    fclose(fp);

    return 1;
}