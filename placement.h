#ifndef PLACEMENT_H
#define PLACEMENT_H

#include "types.h"

void setUpShips(Player *players, int numplayers, int numShips);
int  isCoordinatesInBounds(int x, int y);
int  doesShipOverlap(Ship *ship, Player *player);
int  isShipValidPlacement(Ship *ship, Player *player);
void placeShipOnBoard(Ship *ship, Board *shipBoard);
int  isDirectionValid(char direction);

#endif