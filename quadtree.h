#ifndef __quadtree_h__
#define __quadtree_h__

#include "ship.h"

#define NR_QUADRANTS 4

typedef enum {NE, NW, SW, SE} QUADRANT;
typedef enum {QTINTERNAL, QTLEAF} QT_NODE_TYPE;

typedef struct _QTree *QTree;
typedef struct _QT_Node *QTNode;
typedef struct _Point *Point;
typedef struct _QTNodeLimits *QTNodeLimits;

struct _QTree {
	QTNode root;
	int board_size;
}; 

struct _QT_Node {
	QT_NODE_TYPE type;
	union {
		struct _QT_Node* quadrants[4];
		struct {
			Point coord;
			Cell cell;
		} leaf;
	} node;
};

struct _Point {
	int x;
	int y;
};

struct _QTNodeLimits {
	double minX;
	double maxX;
	double minY;
	double maxY;
	QUADRANT quadrant;
};

/* ************************************** */
/* ********** QuadTree structs ********** */
/* ************************************** */

QTree newQTree(int length);
QTNode newQTNode(int x, int y, Cell cell, int type);
QTNode duplicateLeaf(QTNode original_node);


/* *************************************** */
/* ********** Auxiliary structs ********** */
/* *************************************** */

Point newPoint(int x, int y);
QTNodeLimits newQTNodeLimits(int x, int y, double xmin, double xmax, double ymin, double ymax);


/* ********************************************************* */
/* ********** QuadTree functions for external use ********** */
/* ********************************************************* */

//If point is exactly in the center, then it will be placed in the NE quadrant
void insertCell(QTree qt, int x, int y, Cell cell);
Cell searchCell(QTree qt, int x, int y);
Cell** qtreeToMatrix(QTree qt);	


/* ********************************************************* */
/* ********** QuadTree functions for internal use ********** */
/* ********************************************************* */

void insertCellInternal(QTNode qtn, int x, int y, double xmin, double xmax, double ymin, double ymax, Cell cell);
Cell searchCellInternal(QTNode qtn, int x, int y, double xmin, double xmax, double ymin, double ymax);
void qtreeToMatrixInternal(QTNode qtn, Cell** matrix);


/* ********************************** */
/* ********** Free structs ********** */
/* ********************************** */

void freeQTree(QTree qt);
void freeQTNode(QTree qt, QTNode qtn);

/* ************************************ */
/* ********** Test functions ********** */
/* ************************************ */

void testQuadTree();
void printQTNode(QTNode node);
void printQTNodeLimits(QTNodeLimits qtnl);
void printMatrixFromQuadtree(Cell** matrix, int size);


#endif
