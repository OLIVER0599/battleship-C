#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

Player *createPlayers(int numPlayers);
void initialisePlayer(Player *player, int numPlayers);
void initialisePlayers(Player players[], int numPlayers);
#endif