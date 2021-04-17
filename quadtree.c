#include <stdio.h>
#include <stdlib.h>
#include "quadtree.h"
#include "ship.h"

/* ********** Create structs ********** */

QTree newQTree(int length) {
	QTree qt = (QTree) malloc(sizeof(*qt));
	qt->root = NULL;
	qt->board_size = length;
	return qt;
}

QTNode newQTNode(int x, int y, Cell cell, int type) {
	QTNode qtn = (QTNode) malloc(sizeof(*qtn));
	int i;
	qtn->type = type;
	if (type == QTINTERNAL) {
		for(i = 0; i < NR_QUADRANTS; i++) { qtn->node.quadrants[i] = NULL; }
	}
	else {
		qtn->node.leaf.coord = newPoint(x,y);
		qtn->node.leaf.cell = cell;
	}
	return qtn;
}

QTNode duplicateLeaf(QTNode original_node) {
	if(original_node->type != QTLEAF) {
		return NULL;
	}
	QTNode new_node = (QTNode) malloc(sizeof(*new_node));
	
	new_node->type = original_node->type;
	new_node->node.leaf.coord = original_node->node.leaf.coord;
	new_node->node.leaf.cell = original_node->node.leaf.cell;
	
	return new_node;
}

Point newPoint(int x, int y) {
	Point p = (Point) malloc(sizeof(*p));
	p->x = x;
	p->y = y;
	return p;
}

QTNodeLimits newQTNodeLimits(int x, int y, double xmin, double xmax, double ymin, double ymax) {
	QTNodeLimits qtnl = (QTNodeLimits) malloc(sizeof(*qtnl));
	
	double xhalf = (xmax - xmin) / (double) 2;
	double yhalf = (ymax - ymin) / (double) 2;
	
	if(x < xmin + xhalf) {
		if(y < ymin + yhalf) {
			qtnl->quadrant = SW;
			qtnl->minX = xmin;
			qtnl->maxX = xmin + xhalf;
			qtnl->minY = ymin;
			qtnl->maxY = ymin + yhalf;
		}
		else {
			qtnl->quadrant = NW;
			qtnl->minX = xmin;
			qtnl->maxX = xmin + xhalf;
			qtnl->minY = ymin + yhalf;
			qtnl->maxY = ymax;
		}
	}
	else {
		if(y < ymin + yhalf) {
			qtnl->quadrant = SE;
			qtnl->minX = xmin + xhalf;
			qtnl->maxX = xmax;
			qtnl->minY = ymin;
			qtnl->maxY = ymin + yhalf;
		}
		else {
			qtnl->quadrant = NE;
			qtnl->minX = xmin + xhalf;
			qtnl->maxX = xmax;
			qtnl->minY = ymin + yhalf;
			qtnl->maxY = ymax;
		}
	}
	return qtnl;
}

/* ********** Quadtree functions ********** */

void insertCell(QTree qt, int x, int y, Cell cell) {
	if(qt->root == NULL) {
		qt->root = newQTNode(x, y, cell, QTLEAF);
		return;
	}
	
	double xmin = 0;
	double xmax = qt->board_size - 1;
	
	double ymin = 0;
	double ymax = qt->board_size - 1;
	
	if(qt->root->type == QTLEAF) {
		QTNode prev_root = duplicateLeaf(qt->root);
		qt->root = newQTNode(-1, -1, NULL, QTINTERNAL);
		
		QTNodeLimits qtn_info = newQTNodeLimits
		(	prev_root->node.leaf.coord->x,
			prev_root->node.leaf.coord->y,
			xmin,
			xmax,
			ymin,
			ymax
		);
		
		qt->root->node.quadrants[qtn_info->quadrant] = newQTNode
		(	prev_root->node.leaf.coord->x,
			prev_root->node.leaf.coord->y,
			prev_root->node.leaf.cell,
			QTLEAF
		);
	}
		
	insertCellInternal(qt->root, x, y, xmin, xmax, ymin, ymax, cell);
}

Cell searchCell(QTree qt, int x, int y) {
	double xmin = 0;
	double xmax = qt->board_size - 1;
	
	double ymin = 0;
	double ymax = qt->board_size - 1;
	 
	return searchCellInternal(qt->root, x, y, xmin, xmax, ymin, ymax); 
}

void insertCellInternal(QTNode qtn, int x, int y, double xmin, double xmax, double ymin, double ymax, Cell cell) {	
	QTNodeLimits qtn_info = newQTNodeLimits(x, y, xmin, xmax, ymin, ymax);
	
	if(qtn->node.quadrants[qtn_info->quadrant] == NULL) {
		qtn->node.quadrants[qtn_info->quadrant] = newQTNode(x, y, cell, QTLEAF);
		return;
	}
		
	if(qtn->node.quadrants[qtn_info->quadrant]->type == QTLEAF) {
		QTNode cur_child = duplicateLeaf(qtn->node.quadrants[qtn_info->quadrant]);
		
		qtn->node.quadrants[qtn_info->quadrant] = newQTNode(-1, -1, NULL, QTINTERNAL);
		
		QTNodeLimits qtn_info_sub = newQTNodeLimits
		(	cur_child->node.leaf.coord->x,
			cur_child->node.leaf.coord->y,
			qtn_info->minX,
			qtn_info->maxX,
			qtn_info->minY,
			qtn_info->maxY
		);
		
		qtn->node.quadrants[qtn_info->quadrant]->node.quadrants[qtn_info_sub->quadrant] = newQTNode
		(	cur_child->node.leaf.coord->x,
			cur_child->node.leaf.coord->y,
			cur_child->node.leaf.cell,
			QTLEAF
		);
	}

	insertCellInternal
	(	qtn->node.quadrants[qtn_info->quadrant], 
		x, 
		y, 
		qtn_info->minX, 
		qtn_info->maxX, 
		qtn_info->minY, 
		qtn_info->maxY, 
		cell
	);
}

Cell searchCellInternal(QTNode qtn, int x, int y, double xmin, double xmax, double ymin, double ymax) {
	if(qtn == NULL) {
		return NULL;
	}
	
	if(qtn->type == QTLEAF) {
		if(qtn->node.leaf.coord->x == x && qtn->node.leaf.coord->y == y) {
			return qtn->node.leaf.cell;
		} 
		else {
			return NULL;
		}
	} 
	
	QTNodeLimits qtn_info = newQTNodeLimits(x, y, xmin, xmax, ymin, ymax);
	
	return searchCellInternal
	(	qtn->node.quadrants[qtn_info->quadrant], 
		x, 
		y, 
		qtn_info->minX,
		qtn_info->maxX,
		qtn_info->minY,
		qtn_info->maxY
	);
}

Cell** qtreeToMatrix(QTree qt) {
	int i, j, size;
	size = qt->board_size;
	Cell** matrix = (Cell**) malloc(size*sizeof(Cell*));
	for(i = 0; i < size; i++) {
		matrix[i] = (Cell*) malloc(size*sizeof(Cell));
		for(j = 0; j < size; j++) {
			matrix[i][j] = makeCell(NULL, NOHIT);
		}
	}

	qtreeToMatrixInternal(qt->root, matrix);

	return matrix;
}

void qtreeToMatrixInternal(QTNode qtn, Cell** matrix) {
	if(qtn == NULL) {
		return;
	}

	if(qtn->type == QTINTERNAL) {
		int i;
		for(i = 0; i < NR_QUADRANTS; i++) {
			qtreeToMatrixInternal(qtn->node.quadrants[i], matrix);
		}
	}
	
	if(qtn->type == QTLEAF) {
		matrix[qtn->node.leaf.coord->x][qtn->node.leaf.coord->y] = qtn->node.leaf.cell;
	}
}

/* ********** Free structs ********** */

void freeQTree(QTree qt) {
	freeQTNode(qt, qt->root);
	free(qt);
}

void freeQTNode(QTree qt, QTNode qtn) {
	if(qtn == NULL) {
		return;
	}

	if(qtn->type == QTINTERNAL) {
		int i;
		for(i = 0; i < NR_QUADRANTS; i++) {
			freeQTNode(qt, qtn->node.quadrants[i]);
		}
	}
	
	if(qtn->type == QTLEAF) {
		int i, j, k;
			
		i = qtn->node.leaf.coord->x;
		j = qtn->node.leaf.coord->y;
		
		free(qtn->node.leaf.coord);
		
		if(qtn->node.leaf.cell->ship != NULL) {
			int length = qtn->node.leaf.cell->ship->length;
			
			for(k = 0; k < length; k++) {
				int row, col;
				
				row = qtn->node.leaf.cell->ship->line[k];
				col = qtn->node.leaf.cell->ship->column[k];
				
				if(row == i && col == j) {
					continue;
				}
				
				Cell cell = searchCell(qt, row, col);
				
				cell->ship = NULL;
			}
		}
		
		freeCell(qtn->node.leaf.cell);
	}
	free(qtn); 
}

/* ********** Test functions ********** */

void testQuadTree() {
	QTree qt = newQTree(5);
	
	int l[2] = {1,2};
	int c[2] = {1,1};
	
	Ship ship = makeShip(R2,RECTANGLE, l, c);
	ship->hit[1] = true;
	
	Cell cell = makeCell(ship,HITSHIP);
	
	QTNode qtn1 = newQTNode(2,2,cell,QTLEAF);
	QTNode qtn2 = duplicateLeaf(qtn1);
	
	//printQTNode(qtn1);
	//printQTNode(qtn2);
	
	qtn1 = newQTNode(-1,-1,NULL,QTINTERNAL);
	qtn1->node.quadrants[SE] = qtn2;
	
	//printQTNode(qtn1);
	
	insertCell(qt,2,2,cell);
	insertCell(qt,2,3,cell);
	insertCell(qt,3,4,cell);
	insertCell(qt,4,4,cell);
	
	//printQTNode(qt->root);
	//printQTNode(qt->root->node.quadrants[NE]);
	//printQTNode(qt->root->node.quadrants[NE]->node.quadrants[NE]);
	//printQTNode(qt->root->node.quadrants[NE]->node.quadrants[NE]->node.quadrants[NE]);
	//printQTNode(qt->root->node.quadrants[NE]->node.quadrants[NE]->node.quadrants[NW]);
	
	Cell** matrix = qtreeToMatrix(qt);
	
	printMatrixFromQuadtree(matrix,qt->board_size);
	
	//printf("QTN1\n\n");
	//freeQTNode(qtn1);
	//printf("QTN2\n\n");
	//freeQTNode(qtn2);
	//printf("QT\n\n");
	//freeQTree(qt);
}

void printQTNode(QTNode qtn) {
	printf("******************\n");
	if(qtn == NULL) {
		printf("Node is NULL\n");
		printf("******************\n\n");
		return;
	} 
	if(qtn->type == QTINTERNAL) {
		printf("Type: Internal node\n");
		printf("Quadrants info: \n");
		printf("---------------\n");
		if(qtn->node.quadrants[NE] != NULL) {
			printf("NE is not NULL\n");
		}
		else {
			printf("NE is NULL\n");
		}
		if(qtn->node.quadrants[NW] != NULL) {
			printf("NW is not NULL\n");
		}
		else {
			printf("NW is NULL\n");
		}
		if(qtn->node.quadrants[SW] != NULL) {
			printf("SW is not NULL\n");
		}
		else {
			printf("SW is NULL\n");
		}
		if(qtn->node.quadrants[SE] != NULL) {
			printf("SE is not NULL\n");
		}
		else {
			printf("SE is NULL\n");
		}
		printf("---------------\n");
	}
	else if(qtn->type == QTLEAF) {
		printf("Type: Leaf node\n");
		printf("Coordinates: (%d,%d)\n",qtn->node.leaf.coord->x,qtn->node.leaf.coord->y);
		printf("Cell info: ");
		switch(qtn->node.leaf.cell->hit) {
			case NOHIT: printf("No Hit\n"); break;
			case MISS: printf("Miss\n"); break;
			case HITSHIP: printf("Ship Hit\n"); break;
			case SHIPDESTROYED: printf("Ship Destroyed\n"); break;
		}
		printShip(qtn->node.leaf.cell->ship);
	}
	else {
		printf("Invalid node type.\n");
	}
	printf("******************\n\n");
}

void printQTNodeLimits(QTNodeLimits qtnl) {
	printf("X range: %lf - %lf\n",qtnl->minX,qtnl->maxX);
	printf("Y range: %lf - %lf\n",qtnl->minY,qtnl->maxY);
	switch(qtnl->quadrant) {
		case NE: printf("Quadrant: NE\n"); break;
		case NW: printf("Quadrant: NW\n"); break;
		case SW: printf("Quadrant: SW\n"); break;
		case SE: printf("Quadrant: SE\n"); break;
	}
}

void printMatrixFromQuadtree(Cell** matrix, int size) {
	int i, j;

	printf("  ");

	for(i = 0; i < size; i++){
		printf("%d ", i);
	}
	printf("\n");

	for(i = 0; i < size; i++) {
		printf("%d ", i);
		for(j = 0; j < size; j++) {
			printf("%d ", matrix[i][j]->hit);
		}
		printf("\n");
	}
}
