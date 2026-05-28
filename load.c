#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "load.h"
#include "userinput.h"
#include "move.h"
#include "ai.h"
#include "player.h"    
/*
Read line
    ↓
identify marker
    ↓
load corresponding data
*/

/*
FUNCTION SaveFile:
    input: void
    output: if there exists a saved file-> returns file
            else - > returns NULL
*/

FILE *GetSaveFile(void) {

    FILE *fp = fopen(SAVE_FILENAME, "r");

    if (fp == NULL) {

        printf("\nNo saved game file was found.");
        return NULL;
    }

    return fp;
}



void loadBoard(FILE *fp, Board *board) {
    char line[COLS + 2]; // +2 accounts for teh \n and \0

    for (int y = 0; y < ROWS; y++) {

        //get line
        fgets(line, sizeof(line), fp);

        //clean line
        line[strcspn(line, "\n")] = '\0';

        // for char in line assign it to teh board corresponding cell
        for (int x = 0; x < COLS; x++) {

            board->cells[y][x] = line[x];
        }
    }
        
}

/*
Carrier: name
C :      shipsymbol
5 :     size
0 :     x
0 :     y
d :     direction
4 :     
0
*/
void loadShips(FILE *fp, Player *player) {

    char line[100];

    int shipIndex = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {

        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, END_SHIPS_MARKER) == 0) {

            break;
        }

        if (strcmp(line, SHIP_MARKER) == 0) {

            loadShip(fp, &player->ships[shipIndex]);

            shipIndex++;
        }
    }   
}

/*
Carrier: name
C :      shipsymbol
5 :     size
0 :     x
0 :     y
d :     direction
4 :     
0
*/

void loadShip(FILE *fp, Ship *ship) {

    char line[100];

    //name
    fgets(ship->name, sizeof(ship->name), fp);
    ship->name[strcspn(ship->name, "\n")] = '\0';

    //ship symbol
    fgets(line, sizeof(line), fp);
    ship->shipSymbol = line[0];

    //ship size
    fgets(line, sizeof(line), fp);
    ship->size = atoi(line); //convert asci char '34' to integer 34

    //x coordinate
    fgets(line, sizeof(line), fp);
    ship->x = atoi(line);

    //y coordinate
    fgets(line, sizeof(line), fp);
    ship->y = atoi(line);

    //direction
    fgets(line, sizeof(line), fp);
    ship->direction = line[0];

    //hits 
    fgets(line, sizeof(line), fp);
    ship->hits = atoi(line);

    // sunk 
    fgets(line, sizeof(line), fp);
    ship->sunk = atoi(line);

    // read END_SHIP marker
    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, END_SHIP_MARKER) != 0) {

        printf("\nERROR: END_SHIP marker missing.\n");
    }

    else {

        printf("\nShip was successfully loaded.\n");
    }

}

void loadPlayer(FILE *fp, Player *player, int numPlayers) {


    char line[100];
    /*
        EXPECTED FORMAT:

        PLAYER
            PLAYER_INF
            ...
            END_PLAYER_INF

            SHIP_BOARD
            ...
            END_SHIP_BOARD

            GUESS_BOARDS
            ...
            GUESS_BOARD
            ...
            END_GUESS_BOARD
            ...
            END_GUESS_BOARDS

            SHIPS
            ...
            END_SHIPS
        END_PLAYER
    */

    //PLAYER INF
    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, PLAYER_INF_MARKER) != 0) {

        printf("\nERROR: PLAYER_INF MARKER MISSING");
        return;
    }

    //name
    fgets(player->name, PLAYERNAME_MAX_SIZE, fp);
    player->name[strcspn(player->name, "\n")] = '\0';

    //isAlive
    fgets(line, sizeof(line), fp);
    player->isAlive = atoi(line);


    // shipsRemaining
    fgets(line, sizeof(line), fp);
    player->shipsRemaining = atoi(line);

    // numShips
    fgets(line, sizeof(line), fp);
    player->numShips = atoi(line);

    // isAI                          
    fgets(line, sizeof(line), fp);
    player->isAI = atoi(line);

    //GET END PLAYER INF
    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    //ENDPLAYER INF
    if (strcmp(line, END_PLAYER_INF_MARKER) != 0) {

        printf("\nERROR: END_PLAYER_INF marker missing.\n");
        return;
    }

    // after END_PLAYER_INF is confirmed:
    if (player->isAI) {
        initAIState(&player->aiState);
    }


    //SHIPBOARD
    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, SHIP_BOARD_MARKER) != 0) {

        printf("\nERROR: SHIP_BOARD marker missing.\n");
        return;
    }

    loadBoard(fp, &player->shipBoard);

    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, END_SHIP_BOARD_MARKER) != 0) {

        printf("\nERROR: END_SHIP_BOARD marker missing.\n");
        return;
    }

    //GUESSBOARDS

    fgets(line, sizeof(line), fp); //GUESSBOARDS MARKER
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, GUESS_BOARDS_MARKER) != 0) {
        printf("\nERROR: GUESS_BOARDS marker missing.\n");
        return;
    }

    //initialise the guessboards

    printf("DEBUG numPlayers = %d\n", numPlayers);
    printf("DEBUG sizeof(Board) = %zu\n", sizeof(Board));

    player->guessBoards = malloc(sizeof(Board) * numPlayers);

    if (player->guessBoards == NULL) {
        printf("Memory allocation failed for loaded guess boards.\n");
        exit(1);
    }  
        

    for (int i = 0; i < numPlayers; i++) {


        fgets(line, sizeof(line), fp);
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, GUESS_BOARD_MARKER) != 0) {

            printf("\nERROR: GUESS_BOARD marker missing.\n");
            return;
        }

        loadBoard(fp, &player->guessBoards[i]);

        fgets(line, sizeof(line), fp); //END GUESSBOARD MARKER
        line[strcspn(line, "\n")] = '\0';

    

        if (strcmp(line, END_GUESS_BOARD_MARKER) != 0) {

            printf("\nERROR: END_GUESS_BOARD marker missing.\n");
            return;
        }

    }
    //END GUESSBOARDS
    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, END_GUESS_BOARDS_MARKER) != 0) {
        printf("\nERROR: END_GUESS_BOARDS marker missing.\n");
        return;
    }   

    //SHIPS
    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';


    if (strcmp(line, SHIPS_MARKER) != 0) {

        printf("\nERROR: SHIPS marker missing.\n");
        return;
    }

    

    fgets(line, sizeof(line), fp); // get line //1ST SHIP
    line[strcspn(line, "\n")] = '\0'; // clean line

    int shipIndex = 0;
    while (strcmp(line, END_SHIPS_MARKER) != 0) {

        if (strcmp(line, SHIP_MARKER) == 0) {

            loadShip(fp, &player->ships[shipIndex]);

            shipIndex++;
        }

        fgets(line, sizeof(line), fp); // get line
        line[strcspn(line, "\n")] = '\0'; // clean line

    }

    //ENDPLAYER
    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, END_PLAYER_MARKER) != 0) {

        printf("\nERROR: END_PLAYER marker missing.\n");
        return;
    }







}
void loadMoves(FILE *fp, Move **moveHistory) {

      /*
        EXPECTED FORMAT:

        MOVES
        0 1 0 0 x C 0 0
        1 0 0 0 x C 0 0
        ...
        END_MOVES
    */

    char line[100];

    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';
    

    while (strcmp(line, END_MOVES_MARKER) != 0) {

        MoveData moveData;

        /*
            formAAT:
            shooter target x y result shipHit shipSunk playerKilled
        */

        sscanf(
            line,
            "%d %d %d %d %d %c %c %d %d",
            &moveData.turnNumber,
            &moveData.shooterIndex,
            &moveData.targetIndex,
            &moveData.x,
            &moveData.y,
            &moveData.result,
            &moveData.shipHit,
            &moveData.shipSunk,
            &moveData.playerKilled
        );

        addMove(moveHistory, moveData);


        fgets(line, sizeof(line), fp);
        line[strcspn(line, "\n")] = '\0';
    }
}

void loadGame(Player **players, int *numPlayers, int *totalTurns, Move **moveHistory) {

    FILE *fp = fopen(SAVE_FILENAME, "r");

    if (fp == NULL) {

        printf("\nNo saved game file was found.");
        return;
    }

    char line[100];

    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    //GAMESTATE
    if (strcmp(line, GAMESTATE_MARKER) != 0) {

        printf("\nERROR: GAMESTATE marker missing.\n");
        fclose(fp);
        return;
    }

    //numplayers
    fgets(line, sizeof(line), fp);
    *numPlayers = atoi(line);

    //total turns
    fgets(line, sizeof(line), fp);
    *totalTurns = atoi(line);

    //END GAMESTATE
    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, END_GAMESTATE_MARKER) != 0) {

        printf("\nERROR: END_GAMESTATE marker missing.\n");
        fclose(fp);
        return;
    }


    //plaers
    *players = createPlayers(*numPlayers);

    //PLAYERS MARKER
    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, PLAYERS_MARKER) != 0) {

        printf("\nERROR: PLAYERS marker missing.\n");
        fclose(fp);
        return;
    }


    int playerIndex = 0;

    // 1ST PLAYER MARKER
    fgets(line, sizeof(line), fp); // get line
    line[strcspn(line, "\n")] = '\0'; // clean line

    while (strcmp(line, END_PLAYERS_MARKER) != 0) {

    
        if (strcmp(line, PLAYER_MARKER) == 0) {

            loadPlayer(fp, &(*players)[playerIndex], *numPlayers);

            playerIndex++;
        }

        fgets(line, sizeof(line), fp); // get line
        line[strcspn(line, "\n")] = '\0'; // clean line
    }

    //loadmoves

    fgets(line, sizeof(line), fp);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, MOVES_MARKER) != 0) {

        printf("\nERROR: MOVES marker missing.\n");
        fclose(fp);
        return;
    }

    loadMoves(fp, moveHistory);

    fclose(fp);

    printf("\nGame loaded successfully.\n");


}
