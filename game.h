#ifndef __game_h__
#define __game_h__

#include "ship.h"

#ifdef MATRIX

#include "gamematrix.h"
typedef Cell** GAMEBOARD;

#else

#include "gamequad.h"
#include "quadtree.h"
typedef QTree GAMEBOARD;

#endif

typedef enum {SINGLE=1, FILES, PIPES} GAME_TYPE;

// Pre-game configurations were completed. Start game
void initialize(int type, int len, int ship_nr, char* user1, char* user2);

// Choose type of placement
bool placement(int* ships, int len, char* user, GAMEBOARD board);

// Manual placement
void manualPlacement(int* ships, int len, GAMEBOARD board);

// Random placement
void randomPlacement(int* ships, int len, GAMEBOARD board);

// Function that runs the game
void game(char* user1, char* user2, int ship_nr, int len, GAMEBOARD board1, GAMEBOARD board2);

// Read coordinate and check if it is valid
bool askPlay(int len, int* row, int* col);

// Display game phase, player turn and display both matrix
void showGameState(int winner, int turn, char* user1, char* user2, int len, GAMEBOARD board1, GAMEBOARD board2);

// Function that runs the game when using files
void gameFiles(char* user1, char* user2, int ship_nr, int len, GAMEBOARD board1, GAMEBOARD board2);

// Get the other player name
char* getOtherPlayerName(char *user, char *user1, char *user2);

// Delete text files that were created during game (if the game used files)
void deleteFiles(char* user1, char* user2);

#endif
