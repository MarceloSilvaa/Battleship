#ifndef __aux_h__
#define __aux_h__

#include <stdbool.h>
#include "ship.h"

// Input
#define TRYS 3

// Matrix size
#define MIN_LEN 20
#define MAX_LEN 40

// Occupation percentages
#define MIN_PERC 30
#define MAX_PERC 72

// Information about the ships and the base game
#define BASE_SHIPS 10
#define BASE_SPACE 100
#define FOUR_SPACE 14

// Interface symbols 
#define WATER '~'
#define UNKNOWN '?'
#define SHIP 'S'
#define HIT 'X'

// Alphabetic coordinates
#define UPPER_START 'A'
#define UPPER_END 'Z'
#define LOWER_START 'a'
#define LOWER_END 'n'

// Placement choices
#define MANUAL 0
#define RANDOM 1

// Colors
#define RED(X) 	   "\x1b[31m" X "\x1b[0m"
#define GREEN(X)   "\x1b[32m" X "\x1b[0m"
#define YELLOW(X)  "\x1b[33m" X "\x1b[0m"
#define BLUE(X)    "\x1b[34m" X "\x1b[0m"
#define MAGENTA(X) "\x1b[35m" X "\x1b[0m"
#define CYAN(X)    "\x1b[36m" X "\x1b[0m"

#define BOLD_RED(X)     "\x1b[31m" X "\x1b[0m"
#define BOLD_GREEN(X)   "\x1b[32m" X "\x1b[0m"
#define BOLD_YELLOW(X)  "\x1b[33m" X "\x1b[0m"
#define BOLD_BLUE(X)    "\x1b[34m" X "\x1b[0m"
#define BOLD_MAGENTA(X) "\x1b[35m" X "\x1b[0m"
#define BOLD_CYAN(X)    "\x1b[36m" X "\x1b[0m"

#define COLOR_COORD(X)  "\e[1;97m" X "\e[0m"

typedef enum {PLACEMENT, VISIBLE, HIDDEN} DRAW_FLAG;
typedef enum {PLAYER1, PLAYER2, END} TURN;

typedef struct _CoordInput *CoordInput;

struct _CoordInput {
	char coord1[5];
	char coord2[5];
	char coord3[5];
};

/* ************************************************ */
/* ********** Simple auxiliary functions ********** */
/* ************************************************ */

// Return min
int min(int num1, int num2);

// Return max
int max(int num1, int num2);

// Returns the size of a string
int strLength(char* str);

// Duplicate a string
char* strDuplicate(char* str);

// Duplicate an array
void arrayDuplicate(int* dst, int* src, int len);

// Check if a text file exists
bool textFileExists(char* str);

// Remove a text file if it exists
void textFileRemove(char* str);

// Create a text file if possible
void textFileCreate(char* str);

/* ****************************************** */
/* ********** Simple input control ********** */
/* ****************************************** */

// Check is a string has only digits
bool isInt(char* str);

// Transforms string into int, but detects if string is not a valid int
int strToInt(char* str);

// Check if v is inside an interval
int inInterval(int a, int b, int v);

// Transform str to int and check it is a valid option
int strInInterval(int a, int b, char* str);

// Clear input read after buffer limit
void clearInput(char* str, int limit);


/* ********************************************** */
/* ********** Configuration Game-Ships ********** */
/* ********************************************** */

// Update nr of ships [min,max] with given matrix length 
void validShips(int* interval, int mlen);

// Update nr of ships of each type
void distrShips(int* ships, int ship_nr);


/* ************************************************** */
/* ********** Input-Coordinate translation ********** */
/* ************************************************** */

// Split the input in 3 coordinates
CoordInput splitCoordinates(char* str);

// Check if string is a coordinate
bool isCoordinate(char * str);

// Check if string represents a rectangle
bool checkRectangleType(char* str);

// Check if input represents L ship
bool checkSpecialType(char* str);

// Get the line(int) part of the coordinate
int getLineFromCoord(char* str);

// Get the column(letter) part of the coordinate
int getColumnFromCoord(char* str);


/* ********************************************* */
/* ********** Placement input control ********** */
/* ********************************************* */

// Ask user what type of placement he wants to use
int choosePlacement();

// Ask user to insert ship - User interaction
Ship askToInsertShip(int len);


/* *********************************** */
/* ********** Ship creation ********** */
/* *********************************** */

// Uses coordinates to detect the type of ship. Return -1 if coords are not valid or do not represent a ship
Ship coordToShip(char* str, int len);

// Create list of points that will be occupied by the ship. Return the ship size.
int createPointList(int shape, char* str, int len, int* pointsL, int* pointsC);


/* **************************************** */
/* ********** Game verifications ********** */
/* **************************************** */

// Check if a coordinate is inside the game board
bool isInsideBoard(int len, int l, int c);

// Check if a coordinate is a valid special coordinate
bool isValidSpecial(int orientation, int l1, int l2, int l3, int c1, int c2, int c3);

// Check if there are enough ships of a certain type to place in game
bool shipIsAvailable(int type, int* ships);

// Check if there are ships to place
bool isEmpty(int types[5]);


/* *********************************************************** */
/* ********** Drawing functions - Full information *********** */
/* *********************************************************** */

// Display matrix
void drawMatrix(int len, Cell** matrix, int flag);

// Display matrix side by side
void drawMatrixBothPlayers(int turn, int len, Cell** matrix1, Cell** matrix2);

// Display alphabetic coordinates of the matrix (top of matrix)
void drawMatrixAlphaCoords(int len, char optional);

// Display a row of the matrix with full information (NOT in game)
void drawMatrixRowPlacement(int len, Cell** matrix, int row, char optional);

// Display a row of the matrix with full information (player turn)
void drawMatrixRowVisible(int len, Cell** matrix, int row, char optional);

// Display a row of the matrix with limited information (oponent turn)
void drawMatrixRowHidden(int len, Cell** matrix, int row, char optional);


/* ************************************************************************ */
/* ********** Drawing functions - Information shared with files *********** */
/* ************************************************************************ */

// Display matrix side by side
void drawMatrixBothPlayersFiles(int len, Cell** matrix1, int** matrix2);

// Display a row of the matrix with full information (end of the game with files)
void drawMatrixRowFiles(int len, int** matrix, int row, char optional);

// Draw the matrix board in the given file
void drawMatrixBoardInFile(FILE *file, int len, Cell** matrix);

#endif
