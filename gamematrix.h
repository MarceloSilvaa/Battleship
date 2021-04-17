#ifndef __gamematrix_h__
#define __gamematrix_h__

#include <stdbool.h>
#include "ship.h"

typedef Cell** GAMEBOARD;

// Initialize player matrix with the symbol that represents water
GAMEBOARD initializeStruct(int length);

// Place the ship in the matrix
bool placeShip(Ship ship, int len, GAMEBOARD matrix);

// Mark ship as destroyed
void markShipDestroyed(Ship ship, int len, GAMEBOARD matrix);

// Update matrix hit status - Full information
int makePlay(int row, int col, int len, GAMEBOARD matrix);

// Update matrix hit status - Partial information (Game with files)
void updatePlayFiles(int row, int col, int value, GAMEBOARD matrix);

// Check if the move has already been made
bool checkPlay(int row, int col, GAMEBOARD matrix);

// Get ship in the position (row, col)
Ship getShip(int row, int col, GAMEBOARD matrix);

// Check if it is possible to place the ship 
bool hasSpace(Ship ship, int len, GAMEBOARD matrix);

// Draw the board of one player - Used during placement
void drawOneBoard(int len, GAMEBOARD matrix, int flag);

// Draw the board of both players - Used during game
void drawBothBoards(int turn, int len, GAMEBOARD matrix1, GAMEBOARD matrix2);

// Draw the board of both players - Used in the end of the game with files
void drawBothBoardsFiles(int len, GAMEBOARD matrix1, int** matrix2);

// Draw the board in the given file
void drawBoardInFile(FILE *file, int len, GAMEBOARD matrix);

// Undo memory allocation
void freeStruct(int len, GAMEBOARD board);

#endif
