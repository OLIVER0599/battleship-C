#include "types.h"
#include "board.h"

/*
turns a given an array into a bunch of integers n (whatever you want n to be)
*/

void fillsArray(char *array, int size, int n) {
    for (char *p = array; p < array + size; p++) {
        *p = n;
    }
}

/*
Given a 2D array it will empty the board and fill it with all zeros. 
Boardsize := total size of 2d array
*/

void initialisesBoard(Board *board) {
    fillsArray(board->cells[0], ROWS*COLS, EMPTY);
}

/*
FUNCTION: getBoardType
    input: board
    output: string (pointer) 

    returns "ship board" if its a shipboard or 
            "guess board" if its a guess board
*/
char *getBoardType(Board *board) {
    if (board->type == SHIP_BOARD) {
        return "Ship Board";
    }

    return "Guess Board";
}