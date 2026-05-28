#ifndef TYPES_H
#define TYPES_H

#include "constants.h"

// helps avoid circular inclusion.

/* ── AI ─────────────────────────────────────────────────── */

typedef enum {
    AI_HUNT,
    AI_TARGET
} AIMode;

#define AI_NUM_DIRECTIONS 4

typedef struct {

    AIMode mode;

    int originX, originY;       /* first hit that triggered TARGET mode */
    int currentX, currentY;     /* square currently being probed */

    int dx[AI_NUM_DIRECTIONS];  /* x-components of the 4 directions */
    int dy[AI_NUM_DIRECTIONS];  /* y-components of the 4 directions */
    int dirIndex;               /* which direction we are currently trying */
    int dirTried[AI_NUM_DIRECTIONS]; /* 1 if direction has been exhausted */

} AIState;


/* ── BOARD ──────────────────────────────────────────────── */

typedef enum {
    SHIP_BOARD,
    GUESS_BOARD
} Boardtype;

typedef struct {

    char cells[ROWS][COLS];
    Boardtype type;
    char owner[PLAYERNAME_MAX_SIZE];

} Board;


/* ── SHIP ───────────────────────────────────────────────── */

typedef struct {

    char name[SHIPNAME_MAX_LEN];
    char shipSymbol;
    int  ID;
    int  size;

    int  x, y;
    char direction;

    int  hits;
    int  sunk;

} Ship;


/* ── PLAYER ─────────────────────────────────────────────── */

typedef struct {

    char name[PLAYERNAME_MAX_SIZE];

    Board  shipBoard;
    Board *guessBoards;

    Ship ships[NUM_OF_SHIPS];
    int  numShips;
    int  shipsRemaining;
    int  isAlive;
    int  index;

    int     isAI;
    AIState aiState;

} Player;


/* ── MOVE ───────────────────────────────────────────────── */

typedef struct {

    int  turnNumber;
    int  shooterIndex;
    int  targetIndex;

    int  x, y;

    char result;
    char shipHit;

    int  shipSunk;
    int  playerKilled;

} MoveData;

typedef struct Move {

    MoveData data;
    struct Move *next;

} Move;


#endif
