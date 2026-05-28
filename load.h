#ifndef LOAD_H
#define LOAD_H

#include <stdio.h>
#include "types.h"

void loadBoard(FILE *fp, Board *board);
void loadShip(FILE *fp, Ship *ship);
void loadShips(FILE *fp, Player *player);
void loadPlayer(FILE *fp, Player *player, int numPlayers);
void loadMoves(FILE *fp, Move **moveHistory);
void loadGame(Player **players, int *numPlayers, int *totalTurns, Move **moveHistory);

#endif
