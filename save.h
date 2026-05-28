#ifndef SAVE_H
#define SAVE_H


#include <stdio.h>
#include "types.h"

void saveBoard(FILE *fp, Board *board);
void saveShip(FILE *fp, Ship *ship);
void savePlayer(FILE *fp, Player *player, int numPlayers);
void saveMoveHistory(FILE *fp, Move *moveHistory);
void saveGame(Player *players, int numPlayers, int totalTurns, Move *moveHistory);

#endif