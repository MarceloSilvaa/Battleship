#ifndef __gamequad_h__
#define __gamequad_h__

#include "quadtree.h"

typedef QTree GAMEBOARD;

// Initialize player matrix with the symbol that represents water
GAMEBOARD initializeStruct(int length);

// Place the ship in the matrix
bool placeShip(Ship ship, int len, GAMEBOARD qt);

// Mark ship as destroyed
void markShipDestroyed(Ship ship, int len, GAMEBOARD qt);

// Update matrix hit status - Full information
int makePlay(int row, int col, int len, GAMEBOARD qt);

// Update matrix hit status - Partial information (Game with files)
void updatePlayFiles(int row, int col, int value, GAMEBOARD qt);

// Check if the move has already been made
bool checkPlay(int row, int col, GAMEBOARD qt);

// Get ship in the position (row, col)
Ship getShip(int row, int col, GAMEBOARD qt);

// Check if it is possible to place the ship 
bool hasSpace(Ship ship, int len, GAMEBOARD qt);

// Draw the board of one player - Used during placement
void drawOneBoard(int len, GAMEBOARD qt, int flag);

// Draw the board of both players - Used during game
void drawBothBoards(int turn, int len, GAMEBOARD qt1, GAMEBOARD qt2);

// Draw the board of both players - Used in the end of the game with files
void drawBothBoardsFiles(int len, GAMEBOARD qt1, int** qt2);

// Draw the board in the given file
void drawBoardInFile(FILE *file, int len, GAMEBOARD qt);

// Undo memory allocation
void freeStruct(int len, GAMEBOARD qt);

#endif
