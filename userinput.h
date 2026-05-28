#ifndef USERINPUT_H
#define USERINPUT_H

#include "types.h"

void gameStart(void);
int  getNumberOfPlayers(void);
void clearInputBuffer(void);
void readPlayerNames(Player *players, int numplayers);
void displayBoard(Board *board, Player *player);
void trimTrailingSpaces(char *str);

#endif