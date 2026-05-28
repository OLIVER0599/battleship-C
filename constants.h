#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PLAYERNAME_MAX_SIZE 50
#define ROWS 10
#define COLS 10
#define BOARDSIZE (ROWS*COLS) //TOTAL NUMBER OF SQUARES ON THE BOARD
#define NUM_OF_SHIPS 5
#define EMPTY '~' //this integer will represent the blank areas of teh boards (water for ships and unguessed spots for guesses)

#define RIGHT 'r'
#define LEFT 'l'
#define UP 'u'
#define DOWN 'd'

#define SHIPNAME_MAX_LEN 20

//CARRIER
#define CARRIER 'C'
#define CARRIER_SIZE 5

//BATTLESHIP
#define BATTLESHIP 'B'
#define BATTLESHIP_SIZE 4

// CRUISER
#define CRUISER 'R'
#define CRUISER_SIZE 3

//SUBMARINE
#define SUBMARINE 'S'
#define SUBMARINE_SIZE 3

//DESTROYER
#define DESTROYER 'D'
#define DESTROYER_SIZE 2

//GAMEPLAY
#define WON 1
#define LOST 0

#define HIT 'X'
#define MISS 'O'
#define INVALID_SHOT 'F'

//MARKERS (ALLOW FOR READING BACK SAVE FILES)
//WRITTEN IN HOW IT WOULD BE SAVED IN SAVE FILE

#define SAVE_FILENAME               "save.txt"



#define GAMESTATE_MARKER            "GAMESTATE"
#define END_GAMESTATE_MARKER        "END_GAMESTATE"

#define PLAYERS_MARKER              "PLAYERS"

#define PLAYER_MARKER               "PLAYER"

#define PLAYER_INF_MARKER           "PLAYER_INF"
#define END_PLAYER_INF_MARKER       "END_PLAYER_INF"

#define SHIP_BOARD_MARKER           "SHIP_BOARD"
#define END_SHIP_BOARD_MARKER       "END_SHIP_BOARD"

#define GUESS_BOARDS_MARKER         "GUESS_BOARDS"

#define GUESS_BOARD_MARKER          "GUESS_BOARD"
#define END_GUESS_BOARD_MARKER      "END_GUESS_BOARD"

#define END_GUESS_BOARDS_MARKER     "END_GUESS_BOARDS"

#define SHIPS_MARKER                "SHIPS"

#define SHIP_MARKER                 "SHIP"
#define END_SHIP_MARKER             "END_SHIP"

#define END_SHIPS_MARKER            "END_SHIPS"

#define END_PLAYER_MARKER "END_PLAYER"

#define END_PLAYERS_MARKER           "END_PLAYERS"

#define MOVES_MARKER                "MOVES"
#define END_MOVES_MARKER            "END_MOVES"




#endif