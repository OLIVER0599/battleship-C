#ifndef GAME_H
#define GAME_H

#include "types.h"

void playGame(Player *players, int numPlayers, Move **moveHistory, int *totalTurns);
int  gameHasEnded(Player *players, int numPlayers);
void printListofAlivePlayers(Player *players, int numPlayers);
int  OpponentNameValid(int opponentIndex, int currentPlayerIndex, Player *players);
int  turnFinder(int totalTurns, int numPlayers);
int  findOpponentIndex(Player *players, char opponentName[], int numPlayers);
Player *getOpponent(Player *players, int numPlayers, int currentPlayerIndex, int *opponentIndex);
Player *aiGetOpponent(Player *players, int numPlayers, int currentPlayerIndex, int *opponentIndex);
void printCurrentPlayersTurn(Player *currPlayer);
void announceWhoOpponentIs(Player *opponent, Player *currentPlayer);
int  findWinnerIndex(Player *players, int numPlayers);
void takeTurn(Player *currentPlayer, Player *opponent, int opponentIndex, int *guessX, int *guessY);
int  shotAlreadyTaken(Board *guessBoard, int guessX, int guessY);
MoveData processShot(Player *currentPlayer, Player *opponent, int opponentIndex, int guessX, int guessY, int numPlayers, int totalTurns);
Ship *findShipBySymbol(Player *player, char shipSymbol);
Ship *updateHitShip(Player *opponent, char shipSymbol);
void setupGame(Player **players, int *numPlayers, Move **moveHistory, int *totalTurns);
void cleanupGame(Player *players, int numPlayers, Move *moveHistory);
int  saveFileExists(void);

#endif