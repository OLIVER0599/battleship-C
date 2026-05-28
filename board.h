#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
/*
    Boardtype: {0, 1}, defines the constants which are used in the board structure to allow us to figure out what kind of board it is
*/

#include "types.h"

void fillsArray(char *array, int size, int n);
void initialisesBoard(Board *board);
char *getBoardType(Board *board);

#endif