#ifndef AI_H
#define AI_H


#include "types.h"

void initAIState(AIState *state);
void aiTakeTurn(AIState *state, Board *guessBoard, int *guessX, int *guessY);
void aiHunt(AIState *state, Board *guessBoard, int *guessX, int *guessY);
void aiTarget(AIState *state, Board *guessBoard, int *guessX, int *guessY);
void aiUpdateState(AIState *state, MoveData moveData, int guessX, int guessY);
int  isValidShot(Board *guessBoard, int x, int y);
void resetToHunt(AIState *state);
void tryNextDirection(AIState *state);
int  getNextTargetSquare(AIState *state, int *x, int *y);
void aiSetUpShips(Player *player, int numShips);


#endif
