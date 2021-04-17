#ifndef __ship_h__
#define __ship_h__

#include <stdbool.h>

#define NR_TYPES 5
#define MAX_SIZE 5
#define SPECIAL_SIZE 3

typedef enum {RECTANGLE, SPECIAL} SHAPE;
typedef enum {R2=2, R3, R4, R5, L} TYPE;
typedef enum {HORIZONTAL, VERTICAL, DIAGONAL45, DIAGONAL135} ORIENTATION;
typedef enum {NOHIT, MISS, HITSHIP, SHIPDESTROYED} HIT_TYPE;

typedef struct _Cell *Cell;
typedef struct _Ship *Ship;

struct _Cell {
	Ship ship;
	HIT_TYPE hit;
};

struct _Ship {
        SHAPE shape;
        TYPE type;
        int length;
        int line[MAX_SIZE];
        int column[MAX_SIZE];
        bool hit[MAX_SIZE];
};

void testShip();

// Create cell
Cell makeCell(Ship ship, int hit);

// Free memory
void freeCell(Cell cell);

// Create ship
Ship makeShip(int type, int shape, int* l, int* c);

// Check if a given position of a ship was hit
bool checkCoordHit(Ship ship, int l, int c);

// Mark a coordinate as hit
void updateCoordHit(Ship ship, int l, int c);

// Check if the ship has been completely destroyed
bool shipIsDestroyed(Ship ship);

// Return the ship length
int calculateShipLength(int type, int shape);

// Knowing the space and shape, return the type of ship
int getShipType(int nr_points, int shape);

// Print all atributes of the ship
void printShip(Ship ship);


#endif
