#ifndef MOVE_H
#define MOVE_H

#include "types.h"

Move *createMove(MoveData moveData);
void addMove(Move **head, MoveData moveData);
void printMoves(Move *head);
void freeMoves(Move *head);






#endif