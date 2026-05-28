#include "types.h"
#include "player.h"
#include "board.h"
#include "ship.h"

#include <stdio.h>
#include <stdlib.h>
/*
FUNCTION createPlayers
    input: numPlayers (number of players entered by the user)
    output: pointer to an array of players (which can later be accessed)

    creates numPlayers number of players of type Player 
    for each player: 
        1. allocate memory enough for each player
        2. return the collection of players (players)
*/

Player *createPlayers(int numPlayers) {
    Player *players;
    // allocate memory for players array
    players = malloc(sizeof(Player) * numPlayers);

    if (players == NULL) {
        printf("Memory allocation of players failed.\n");
        exit(1);
    }

    return players;
}

/*
FUNCTION initialisePLayer
    input: Player player (a player whose name has already been assigned by user)
    output: void (modifies player pointer)

    1. assigns number of ships to player (later will be used to create different number of ships dynaamically - > difficulty modes)
    2. initialises players ship and guess boards (empties them)
    3. initialise ships (give ships types, names etc.)
*/
void initialisePlayer(Player *player, int numPlayers) {

    //1
    player->numShips = NUM_OF_SHIPS;
    player->isAlive = 1;
    player->shipsRemaining = NUM_OF_SHIPS;

    //2 initialise boards
    initialisesBoard(&player->shipBoard);

    // dynamically allocate the guessboards
    player->guessBoards = malloc(sizeof(Board) * numPlayers);

    if (player->guessBoards == NULL) {

        printf("Memory allocation failned for guessboards.");
        exit(1);
    }

    for (int i = 0; i < numPlayers; i++) {

        initialisesBoard(&player->guessBoards[i]);
    }

    //3
    initialiseShips(player->ships);
}

/*
FUNCTION initialisePLayers
    input: list of players
    output: void (modifies players array pointer)

    1. assigns number of ships to player (later will be used to create different number of ships dynaamically - > difficulty modes)
    2. initialises players ship and guess boards (empties them)
    3. initialise ships (give ships types, names etc.)
*/
void initialisePlayers(Player players[], int numPlayers) {

    for (int i = 0; i < numPlayers; i++) {
        
        initialisePlayer(&players[i], numPlayers);
        players[i].index = i;
    }
}