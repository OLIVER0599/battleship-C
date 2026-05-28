#include "types.h"
#include "ship.h"
#include "player.h"
#include "placement.h"
#include "userinput.h"
#include "ai.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
FUNCTION intialiseShips
    input: players array of ships
    output: void (modifiers players ships)

    Given a players list of ships, for each ship
    1. sets ship name 
    2. set ships symbol
    3. sets size of ship
*/

void initialiseShips(Ship ships[]) {

    //CARRIER
    strcpy(ships[0].name, "Carrier");
    ships[0].shipSymbol = CARRIER;
    ships[0].size = CARRIER_SIZE;
    ships[0].hits = 0;
    ships[0].sunk = 0;

    //BATTLESHIP
    strcpy(ships[1].name, "Battleship");
    ships[1].shipSymbol = BATTLESHIP;
    ships[1].size = BATTLESHIP_SIZE;
    ships[1].hits = 0;
    ships[1].sunk = 0;

    // CRUISER
    strcpy(ships[2].name, "Cruiser");
    ships[2].shipSymbol = CRUISER;
    ships[2].size = CRUISER_SIZE;
    ships[2].hits = 0;
    ships[2].sunk = 0;

    //SUBMARINE
    strcpy(ships[3].name, "Submarine");
    ships[3].shipSymbol = SUBMARINE;
    ships[3].size = SUBMARINE_SIZE;
    ships[3].hits = 0;
    ships[3].sunk = 0;

    //DESTROYER
    strcpy(ships[4].name, "Destroyer");
    ships[4].shipSymbol = DESTROYER;
    ships[4].size = DESTROYER_SIZE;
    ships[4].hits = 0;
    ships[4].sunk = 0;
}

/*
FUNCTION: setUpShips
    input: list of players (pointer), number of players, number of ships (for possibility of dynamic allocation of ships)
    output: void (modify players)

    for each player in players:
        for each ship in ships:  
            prompt for coordinates, (x, y) direction ()
            
                a)  if (coordinates are in bounds of board continue) --> continue to b)
                    else --> a) (repeat)

                b) prompt for direction
                c) if (ship is a valid placement) --> go to d)
                    else --> c) (repeat)
                d) place ship

*/
void setUpShips(Player *players, int numplayers, int numShips) {

    int x;
    char rowchar;
    char dir;
    

    int result; //stores result of scan
    int validPlacement = 0; //stores 1 if valid placement, 0 if not. initialised to FALSE

    Ship *ship; //temporary ship pointer
    Player *player; //temp player pointer

    for (int i = 0; i < numplayers; i++) { // for each player

        player = &players[i];

        // ── AI PLACEMENT */
        if (player->isAI) {

            aiSetUpShips(player, numShips);
            continue;  /* skip the human input block below */
        }


        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\n==============================\n");
        printf("%s's Ship Placement Phase\n", player->name);
        printf("==============================\n");
        printf(
            "\n\n Please enter place your ships:\n"
            "\tCARRIER, BATTLESHIP, CRUISER, SUBMARINE, DESTROYER\n"
        );
        displayBoard(&player->shipBoard, player);
        printf(
            "\nCoordinates are entered as: x y direction\n"
            "x = coloumn, increases from left to right (1 -> %d)\n"
            "y = row, increases from top to bottom (A -> %c)\n\n",
            COLS, ('A' + ROWS - 1)
        );

        for (int j = 0; j < numShips; j++) { // for each ship

            ship = &players[i].ships[j];
            
            do {
                printf("\n\nPlacing %s: SIZE = %d", ship->name, ship->size);
                printf("\nEnter your ships coordinates:   ");
                result = scanf(" %d %c %c", &x, &rowchar, &dir); // get coordinates and direction
                
                // did scanf work?
                if (result != 3) {

                    printf("\n\n\tYou have entered an invalid coordinates/directions. Please try again\n\n");
                    clearInputBuffer(); //scanf failed, safely get rid of anything bad just in case
                }

                else  {

                    // is direction valid?
                    rowchar = toupper(rowchar);
                    dir = tolower(dir); 

                    if (isDirectionValid(dir) == 0) {

                        printf("\n\n\tThe direction you have entered is invalid\n\n");
                        validPlacement = 0;
                    }
                    
                     //are coordinates valid?
                    else {

                        ship->x = x - 1; 
                        ship->y = rowchar - 'A'; 
                        ship->direction = dir;

                        //does: (ship overlap with another ship() OR (does ship hang of map) OR (COORDINATE IS OFF MAP)
                        if (isShipValidPlacement(ship, player)  == 0 ) {

                            printf("\nPlease try again.\n\n");
                            validPlacement = 0; //not valid
                        }
                        else {

                            validPlacement = 1; //valid
                        }
                    }
                }
            printf("\n\n\n");
           
            } while (result != 3 || validPlacement == 0);

            // coordinates are completely valid. Therefore, we can:
            // place ship at these coordinates

            placeShipOnBoard(ship, &player->shipBoard);
            printf("\nSuccessful ship placement: \n\n");
            displayBoard(&player->shipBoard, player);
            clearInputBuffer();
            
        }
    }
}   



/*  
FUNCTION isCoordinatesInBounds()
    input: integer coordinate x, y (internal coordinates are like indexes: x: 0->COLS-1, y: 0->ROWS-1)
    output: integer {0, 1}

    returns a 0 if coordinates are outside of board
            a 1 if coodinates are inside of board
*/
int isCoordinatesInBounds(int x, int y) {

    return (x >= 0 && x < COLS && y >= 0 && y < ROWS);
}

/*
FUNCTION doesShipOverlap
    input: player, and his ship (bother pointers)
    output: {0, 1} true, false

    checks if a ship ovlerlaps with another ship on its players shipboard

    endX:= the x coordinate of the end of the ship (if facing left or right)
    endY:= the y coordinate of teh end of the ship (if facing up or down)

*/
int doesShipOverlap(Ship *ship, Player *player) {

    //important note:   coordinates (x, y) <--> board[y][x]
    int endX, endY;
    
    switch (ship->direction) {

         case RIGHT:

            endX = ship->x + (ship->size - 1);
            for (int x = ship->x; x <= endX; x++) { //loop right

                if (player->shipBoard.cells[ship->y][x] != EMPTY) {
                    printf(
                        "\n\nThe coordinates you have entered overlap with a ship.\n\n"
                    );
                    return 1;
                }
            }
            return 0;

        case LEFT:

            endX = ship->x - (ship->size - 1);
            for (int x = ship->x; x >= endX; x--) { //loop left

                if (player->shipBoard.cells[ship->y][x] != EMPTY) {
                    printf(
                        "\n\nThe coordinates you have entered overlap with a ship.\n\n"
                    );
                    return 1;
                }
            }
            
            return 0;
        case DOWN:

            endY = ship->y + (ship->size - 1);
            for (int y = ship->y; y <= endY; y++) {

                if (player->shipBoard.cells[y][ship->x] != EMPTY) {
                    printf(
                        "\n\nThe coordinates you have entered overlap with a ship.\n\n"
                    );
                    return 1;
                }
            }

            return 0;
           
        case UP:    

            endY = ship->y - (ship->size - 1);
            for (int y = ship->y; y >= endY; y--) {

                if (player->shipBoard.cells[y][ship->x] != EMPTY) { 
                    printf(
                        "\n\nThe coordinates you have entered overlap with a ship.\n\n"
                    );
                    return 1;
                }
            }
            return 0;

        default:
            return 0;
    }
}

/*
    FUNCTION isShipValidPlacement
        input: player, player's ship (both pointers)
        output: 0 or 1(false or true) or if its invalid

        checks if coordinate is on map: 

        finds ships direciton: 
            depending on direction, checks if its in bounds, if it overlaps with ships on that board (if its a shipboard)
            endX:= the x coordinate of the end of the ship (if facing left or right)
            endY:= the y coordinate of teh end of the ship (if facing up or down)

*/
int isShipValidPlacement(Ship *ship, Player *player) {


    // check if coordinate is in bounds
    if (isCoordinatesInBounds(ship->x, ship->y) == 0) {
        printf(
            "\n\nThe coordinates you entered are outside of the map borders."
            "\nCoordinates are entered as: x y direction\n"
            "x = coloumn, increases from left to right (1 -> %d)\n"
            "y = row, increases from top to bottom (A -> %c)\n\n",
            COLS, (ROWS - 'A' + 1)
        );
        displayBoard(&player->shipBoard, player);
        return 0;
    }

    //CHECKS if ship hangs of map
    int endX, endY; 

    switch (ship->direction) {
        case RIGHT:

            //check if end of ship is in bounds (print error and give solution)
            endX = ship->x + (ship->size - 1);
            if (endX >= COLS) {

                printf(
                    "Invalid Placement: ship goes off to the right too much for its size.\n"
                    "Please move it left by: %d squares",
                    (endX - (COLS - 1))
                );
                return 0;
            }
            break;
        case LEFT:

            //check if end of ship is in bounds (print error and give solution)
            endX = ship->x - (ship->size - 1);

            if (endX < 0) {

                
                printf(
                    "Invalid Placement: ship goes off to the left too much for its size.\n"
                    "Please move it right by: %d squares",
                    (-endX)
                );
                return 0;
            }
            break;
        case DOWN:

            //check if end of ship is in bounds (print error and give solution)
            endY = ship->y + (ship->size - 1);

            if (endY >= ROWS) {

                printf(
                    "Invalid Placement: ship goes off to the down too much for its size.\n"
                    "Please move it up by: %d squares",
                    (endY - (ROWS - 1))
                );
                return 0;
            }
            break;
        case UP:    

            endY = ship->y - (ship->size - 1);
            
            if (endY < 0) {

                printf(
                    "Invalid Placement: ship goes off to the up too much for its size.\n"
                    "Please move it down by: %d squares",
                    (-endY)
                );
                return 0;
            }
            break;
        default:
            printf("direction is invalid.\n");
            return 0;
    }

    //check if ship overlaps with another ship that has been placed
    if (doesShipOverlap(ship, player) == 1) {
        return 0;
    }

    return 1;
}

/*
FUNCTION placeShipOnBoard
    input: ship, and a shipboard (pointers)
    output: void (modifies shipboard)
    after the ship has been given valid coordinates, this sets the ship boards corresponding coordinates to a specific marker
    
*/

void placeShipOnBoard(Ship *ship, Board *shipBoard) {
    int endX, endY;

    switch (ship->direction) {

         case RIGHT:

            endX = ship->x + (ship->size - 1);
            for (int x = ship->x; x <= endX; x++) { //loop right
                
                shipBoard->cells[ship->y][x] = ship->shipSymbol;
            }
            break;

        case LEFT:

            endX = ship->x - (ship->size - 1);
            for (int x = ship->x; x >= endX; x--) { //loop left

                shipBoard->cells[ship->y][x] = ship->shipSymbol;
            }
            break;
            
        case DOWN:

            endY = ship->y + (ship->size - 1);
            for (int y = ship->y; y <= endY; y++) {

                shipBoard->cells[y][ship->x] = ship->shipSymbol;
            }
            break;

        case UP:    

            endY = ship->y - (ship->size - 1);
            for (int y = ship->y; y >= endY; y--) {

                shipBoard->cells[y][ship->x] = ship->shipSymbol;
            }
            break;
        default:
            return;
    }
}

int isDirectionValid(char direction) {
    return (
        direction == RIGHT ||
        direction == LEFT ||
        direction == UP ||
        direction == DOWN
    );
}