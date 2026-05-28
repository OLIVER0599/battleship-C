#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "ai.h"
#include "placement.h"


void initAIState(AIState *state) {

    state->mode = AI_HUNT;

    state->originX  = -1;
    state->originY  = -1;
    state->currentX = -1;
    state->currentY = -1;

    /* load direction points */

    //this maps dirIndex {0, 1, 2, 3} to {RIGHT, LEFT, DOWN, UP}. thIS WILL ALLO W me TO ACCESS these positions easier
    state->dx[0] =  1; state->dy[0] =  0; /* RIGHT */
    state->dx[1] = -1; state->dy[1] =  0; /* LEFT  */
    state->dx[2] =  0; state->dy[2] =  1; /* DOWN  */
    state->dx[3] =  0; state->dy[3] = -1; /* UP    */

    state->dirIndex = 0;

    for (int i = 0; i < AI_NUM_DIRECTIONS; i++) {
        state->dirTried[i] = 0;
    }

    /* */
    srand((unsigned int)time(NULL));
}



int isValidShot(Board *guessBoard, int x, int y) {

    if (x < 0 || x >= COLS || y < 0 || y >= ROWS) {
        return 0;
    }

    return (guessBoard->cells[y][x] == EMPTY);
}

void resetToHunt(AIState *state) {

    state->mode     = AI_HUNT;
    state->originX  = -1;
    state->originY  = -1;
    state->currentX = -1;
    state->currentY = -1;
    state->dirIndex = 0;

    for (int i = 0; i < AI_NUM_DIRECTIONS; i++) {
        state->dirTried[i] = 0;
    }
}


void tryNextDirection(AIState *state) {

    /* mark current direction as done */
    state->dirTried[state->dirIndex] = 1;

    /* find next untried direction */
    for (int i = 0; i < AI_NUM_DIRECTIONS; i++) {

        if (state->dirTried[i] == 0) {
            state->dirIndex = i;
            /* reset probe position to origin */
            state->currentX = state->originX;
            state->currentY = state->originY;
            return;
        }
    }

    /*
        all directions exhausted. This shouldn't happen in a
       valid game (the ship must be somewhere), but if it does
       we fall back to HUNT to avoid getting stuck.
    */
    resetToHunt(state);
}

/* 
   getNextTargetSquare
   Computes the next (x, y) to probe in the current direction
   by advancing one step fromsd currentX/Y.

        Writes the result into *x and *y.
   Returns 1 if the computed square is valid (in bounds and
   unshot), 0 if it is not.

   Does NOT modify statds the caller decides what to do
   based on validity.
 */

int getNextTargetSquare(AIState *state, int *x, int *y) {

    *x = state->currentX + state->dx[state->dirIndex];
    *y = state->currentY + state->dy[state->dirIndex];

    return (*x >= 0 && *x < COLS && *y >= 0 && *y < ROWS);
}

/* 
   aiHunt
   HUNT mode implementation.

   Picks a random (x, y) that hasn't been shot yet.
   Loops until a valid square is found.

   The randomness here is the AI's prior — in the absence
   of information, all unshot squares are equally probable
   to contain a ship.
 */
void aiHunt(AIState *state, Board *guessBoard, int *guessX, int *guessY) {

    int x, y;

    do {
        x = rand() % COLS;
        y = rand() % ROWS;
    } while (!isValidShot(guessBoard, x, y));

    *guessX = x;
    *guessY = y;

    printf("\n[AI] Hunting... firing at %d %c\n", x + 1, 'A' + y);
}

/* 
   aiTarget
   TARGET mode implementation.

   Tries to find the next square to probe along the current
   direction from the origin hit. If the next square in the
   current direction is invalid (off board or already shot),
   it advances to the next untried direction and tries again.

   This loop continues until a valid unshot square is found.
   Because the game guarantees a ship exists near the origin,
   a valid square will always be found before directions run out
   (in a well-formed game state).
 */
void aiTarget(AIState *state, Board *guessBoard, int *guessX, int *guessY) {

    int x, y;
    int attempts = 0; /* safety counter to prevent infinite loop */

    while (attempts < AI_NUM_DIRECTIONS) {

        int inBounds = getNextTargetSquare(state, &x, &y);

        /* square is in bounds cand unshot  use it */
        if (inBounds && isValidShot(guessBoard, x, y)) {

            *guessX = x;
            *guessY = y;

            printf("\n[AI] Targeting... firing at %d %c\n", x + 1, 'A' + y);
            return;
        }

        /* square is invalid  try next direction */
        tryNextDirection(state);
        attempts++;
    }

    /*
       Fallback: all target directions failed.
       Return to chunt mode and pick randomly.
       this should not occur in normal gameplay.
    */
    printf("\n[AI] Target exhausted, returning to hunt.\n");
    resetToHunt(state);
    aiHunt(state, guessBoard, guessX, guessY);
}



void aiTakeTurn(AIState *state, Board *guessBoard, int *guessX, int *guessY) {

    printf("\n[AI] thinking...\n");

    if (state->mode == AI_HUNT) {

        aiHunt(state, guessBoard, guessX, guessY);
    }
    else {

        aiTarget(state, guessBoard, guessX, guessY);
    }
}

/* 
   aiUpdateState
   Called after processShot returns MoveData.
   This is the feedback function , the AI learns from what
   just happened and updates its state for next turn.

   Three cases:

   MISS:
       If in TARGET mode, the current direction is wrong.
       Try the next direction from the origin.
       If in HUNT mode, nothing to update.

   HIT (ship not sunk):
       Switch to TARGET mode if not already there.
       Sftore the origin (first hit) and advance currentX/Y
       to the square that was just hit, so next turn probes
       one step further in the same direction.

   HIT (ship sunk):
       The ship is fully destroyed. Reset to HUNT mode.
       The slate is clean  begin searching again.
 */
void aiUpdateState(AIState *state, MoveData moveData, int guessX, int guessY) {

    if (moveData.result == MISS) {

        if (state->mode == AI_TARGET) {
            tryNextDirection(state);
        }
        /* in HUNT mode a miss requires no state change */
        return;
    }

    if (moveData.result == HIT) {

        if (moveData.shipSunk) {

            /* ship destroyed → return to open search */
            resetToHunt(state);
            return;
        }

        /* hit but not sunk */
        if (state->mode == AI_HUNT) {

            /* first hit on this ship: switch to TARGET mode */
            state->mode    = AI_TARGET;
            state->originX = guessX;
            state->originY = guessY;
            state->currentX = guessX;
            state->currentY = guessY;
            state->dirIndex = 0;

            for (int i = 0; i < AI_NUM_DIRECTIONS; i++) {
                state->dirTried[i] = 0;
            }
        }
        else {

            /*
               Already in TARGET mode and got another hit.
               Advance currentX/Y to this square so the next
               probe continues one step further in this direction.
            */
            state->currentX = guessX;
            state->currentY = guessY;
        }
    }
}

/* 
   aiSetUpShips
   input: player (the AI player), numShips
   output: void (modifies player's shipBoard and ships array)

   Places each of the AI's ships randomly on its board.
   For each ship:
       1. generate random x, y, and direction
       2. assign them to the ship
       3. check validity using isShipValidPlacement
          (same function used for human placement —
           the rules don't care who is placing)
       4. if valid place ship and move to next
          if not    try again

   The reuse of isShipValidPlacement is the key point:
   the AI is subject to exactly the same constraints as
   a human player. The only difference is that random
   numbers replace human input.
*/

/* the four possible directions as chars, matching your constants */
static const char AI_DIRECTIONS[4] = { RIGHT, LEFT, DOWN, UP };

void aiSetUpShips(Player *player, int numShips) {

    printf("\n[AI] %s is placing ships...\n", player->name);

    for (int j = 0; j < numShips; j++) {

        Ship *ship = &player->ships[j];

        do {
            /* generate random position and direction */
            ship->x         = rand() % COLS;
            ship->y         = rand() % ROWS;
            ship->direction = AI_DIRECTIONS[rand() % 4];

        } while (isShipValidPlacement(ship, player) == 0);

        /* valid placement found  put it on the board */
        placeShipOnBoard(ship, &player->shipBoard);

        printf("[AI] Placed %s\n", ship->name);
    }

    printf("[AI] %s has finished placing ships.\n\n", player->name);
}
