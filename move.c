#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "move.h"


/*
FUNCTION createMove
    input: (move members)
    output: pointer to a new move (newMove points to the NULL)
dynamically allocates memory for a single move. returns it as pointer
*/

Move *createMove(
   MoveData moveData
) 
{
    //ALLOCATE MEMORY
    Move *newMove = malloc(sizeof(Move));

    if (newMove == NULL) {

        perror("Memory failed to be allocated for MOVE.");
        exit(EXIT_FAILURE);
    }

    //ASSIGN MEMBERS VALUES
    newMove->data.turnNumber = moveData.turnNumber;
    newMove->data.shooterIndex = moveData.shooterIndex;
    newMove->data.targetIndex = moveData.targetIndex;
    newMove->data.x = moveData.x;
    newMove->data.y = moveData.y;
    newMove->data.result = moveData.result;
    newMove->data.shipHit = moveData.shipHit;
    newMove->data.shipSunk = moveData.shipSunk;
    newMove->data.playerKilled = moveData.playerKilled;


    newMove->next = NULL;

    return newMove;
}


void addMove (
    Move **head,
    MoveData moveData
) 
{
    Move *newMove = createMove(moveData);

    if (*head == NULL) { //list is empty (no moves made)
        *head = newMove;
        return;
    }

    Move *current = *head; //create current pointer

    while (current->next != NULL) current = current->next;

    current->next = newMove;
}

/*
FUNCTION printMoves
    input: head of move list
    output: void (prints out all the moves made so far. (uses internal coordinates))
*/

void printMoves (Move *head) {

    Move *current = head;

    while (current != NULL) {
        printf(
            "Turn %d: Player %d shot Player %d at %d, %d. Result: %c. Ship sunk: %d. Player killed: %d",
            current->data.turnNumber,
            current->data.shooterIndex,
            current->data.targetIndex,
            current->data.x,
            current->data.y,
            current->data.result,
            current->data.shipSunk,
            current->data.playerKilled
        );

        if (current->data.shipHit != EMPTY) {
            printf(" Ship hit: %c", current->data.shipHit);
        }
        
        printf("\n");

        current = current->next;
    }
}

void freeMoves(Move *head) {

    Move *current = head;

    while (current != NULL) {

        Move *next = current->next;
        free(current);
        current = next;
    }
}

