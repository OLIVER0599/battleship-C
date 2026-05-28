#include <stdio.h>

#include "types.h"
#include "save.h"

void saveBoard(FILE *fp, Board *board)
{
    for (int y = 0; y < ROWS; y++) {

        for (int x = 0; x < COLS; x++) {

            fputc(board->cells[y][x], fp);
        }

        fputc('\n', fp);
    }
}

void saveShip(FILE *fp, Ship *ship)
{
    fprintf(fp, "%s\n", ship->name);
    fprintf(fp, "%c\n", ship->shipSymbol);
    fprintf(fp, "%d\n", ship->size);
    fprintf(fp, "%d\n", ship->x);
    fprintf(fp, "%d\n", ship->y);
    fprintf(fp, "%c\n", ship->direction);
    fprintf(fp, "%d\n", ship->hits);
    fprintf(fp, "%d\n", ship->sunk);
}

void savePlayer(FILE *fp, Player *player, int numPlayers)
{
    fprintf(fp, "%s\n", PLAYER_MARKER);

    fprintf(fp, "%s\n", PLAYER_INF_MARKER);
    fprintf(fp, "%s\n", player->name);
    fprintf(fp, "%d\n", player->isAlive);
    fprintf(fp, "%d\n", player->shipsRemaining);
    fprintf(fp, "%d\n", player->numShips);
    fprintf(fp, "%d\n", player->isAI);       
    fprintf(fp, "%s\n", END_PLAYER_INF_MARKER);

    fprintf(fp, "%s\n", SHIP_BOARD_MARKER);
    saveBoard(fp, &player->shipBoard);
    fprintf(fp, "%s\n", END_SHIP_BOARD_MARKER);

    fprintf(fp, "%s\n", GUESS_BOARDS_MARKER);
    for (int i = 0; i < numPlayers; i++) {
        fprintf(fp, "%s\n", GUESS_BOARD_MARKER);
        saveBoard(fp, &player->guessBoards[i]);
        fprintf(fp, "%s\n", END_GUESS_BOARD_MARKER);
    }
    fprintf(fp, "%s\n", END_GUESS_BOARDS_MARKER);


    fprintf(fp, "%s\n", SHIPS_MARKER);

    for (int j = 0; j < player->numShips; j++) {
        fprintf(fp, "%s\n", SHIP_MARKER);
        saveShip(fp, &player->ships[j]);
        fprintf(fp, "%s\n", END_SHIP_MARKER);
    }

    fprintf(fp, "%s\n", END_SHIPS_MARKER);

    fprintf(fp, "%s\n", END_PLAYER_MARKER);
}

void saveMoveHistory(FILE *fp, Move *moveHistory)
{
    Move *currentMove = moveHistory;

    fprintf(fp, "%s\n", MOVES_MARKER);

    while (currentMove != NULL) {
        fprintf(
            fp,
            "%d %d %d %d %d %c %c %d %d\n",
            currentMove->data.turnNumber,
            currentMove->data.shooterIndex,
            currentMove->data.targetIndex,
            currentMove->data.x,
            currentMove->data.y,
            currentMove->data.result,
            currentMove->data.shipHit,
            currentMove->data.shipSunk,
            currentMove->data.playerKilled
        );

        currentMove = currentMove->next;
    }

    fprintf(fp, "%s\n", END_MOVES_MARKER);
}

void saveGame(Player *players, int numPlayers, int totalTurns, Move *moveHistory)
{
    FILE *fp = fopen(SAVE_FILENAME, "w");

    if (fp == NULL) {
        printf("\nFAILED TO SAVE GAME\n");
        return;
    }

    fprintf(fp, "%s\n", GAMESTATE_MARKER);
    fprintf(fp, "%d\n", numPlayers);
    fprintf(fp, "%d\n", totalTurns);
    fprintf(fp, "%s\n", END_GAMESTATE_MARKER);

    fprintf(fp, "%s\n", PLAYERS_MARKER);
    for (int i = 0; i < numPlayers; i++) {
        savePlayer(fp, &players[i], numPlayers);
    }
    fprintf(fp, "%s\n", END_PLAYERS_MARKER);
    
    saveMoveHistory(fp, moveHistory);

    fclose(fp);

    printf("\n\nGame saved successfully.\n");
}