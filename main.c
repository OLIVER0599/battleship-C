#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "game.h"
#include "move.h"
#include "save.h"
#include "load.h"



int main(void) 
{   

    Player *players;
    int numPlayers;
    Move *moveHistory;
    int totalTurns;

    setupGame(&players, &numPlayers, &moveHistory, &totalTurns);
    playGame(players, numPlayers, &moveHistory, &totalTurns);
    cleanupGame(players, numPlayers, moveHistory);

    return 0;
}

