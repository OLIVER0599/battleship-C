# Battleship Game in C

A terminal-based implementation of the classic Battleship board game, written in C.

## About the Game

This is my first project with C.

Battleship is a two-player strategy game where each player secretly places ships on a 10x10 grid, then takes turns guessing coordinates to "shoot" at the opponent's fleet. The first player to sink all of the opponent's ships wins.

## Features

- **Two-player local multiplayer** with a transition screen between turns to maintain secrecy
- **Custom player names**
- **Interactive ship placement** with validation (no overlaps, must fit on board)
- **Turn-based shooting** with coordinate input (e.g. A5, G2)
- **Hit/miss tracking** displayed before each turn
- **Ship sinking detection** with notifications to both players
- **Save and load game state** so games can be resumed later
- **Game replay** — replay any completed game shot by shot
- **AI opponent** for single-player mode
- **Colour terminal interface** for improved readability

## Ships

| Ship | Size |
|------|------|
| Carrier | 5 squares |
| Battleship | 4 squares |
| Cruiser | 3 squares |
| Submarine | 3 squares |
| Destroyer | 2 squares |

## Project Structure

```
├── main.c          # Entry point and game loop
├── board.c / .h    # Board display and management
├── ship.c / .h     # Ship data structures and logic
├── placement.c / .h# Ship placement and validation
├── move.c / .h     # Shot processing and move history
├── player.c / .h   # Player data and management
├── save.c / .h     # Save and load game state
├── load.c / .h     # Load game from file
├── ai.c / .h       # AI opponent logic
├── userinput.c / .h# Input handling and validation
├── game.c / .h     # Core game flow
├── types.h         # Shared type definitions
└── constants.h     # Game constants (board size, symbols, etc.)
```

## How to Compile

Using GCC:
```bash
gcc -o battleship main.c board.c ship.c placement.c move.c player.c save.c load.c ai.c userinput.c game.c -lm
```

Or with Code::Blocks / VSCode using the gcc compiler. "gcc *.c -o -lm" also works

## How to Play

1. Run the compiled executable
2. Choose to start a new game or load a saved one
3. Each player enters their name
4. Players decide if player is an AI or a Human
4. Each player places their 5 ships on the grid
5. Players alternate turns shooting at coordinates
6. The game ends when one player's fleet is fully sunk
7. Optionally save or replay the completed game


