#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ship.h"

void testShip() {
	int l[2] = {1,2};
	int c[2] = {1,1};
	
	Ship ship = makeShip(R2,RECTANGLE, l, c);
	ship->hit[1] = true;
	
	printShip(ship);
}

Cell makeCell(Ship ship, int hit) {
	Cell cell = (Cell) malloc(sizeof(*cell));
	cell->ship = ship;
	cell->hit = hit;
	return cell;
}

void freeCell(Cell cell) {
	if(cell->ship != NULL) {
		free(cell->ship);	
	}
	free(cell);
}

Ship makeShip(int type, int shape, int* l, int* c) {
	
	if(l == NULL || c == NULL) { return NULL; }
	
	Ship ship = (Ship) malloc(sizeof(*ship));
	
	ship->type = type;
	ship->shape = shape;
	ship->length = calculateShipLength(type,shape);
	
	int i;
	for(i = 0; i < ship->length; i++) {
		ship->line[i] = l[i];
		ship->column[i] = c[i];
		ship->hit[i] = false;
	}
	
	return ship;
}

bool checkCoordHit(Ship ship, int l, int c) {
	bool ans = false;
	int i;
	for(i = 0; i < ship->length; i++) {
		if(ship->line[i] == l && ship->column[i] == c) {
			ans = ship->hit[i];
			break;
		}
	}
	return ans;
}

void updateCoordHit(Ship ship, int l, int c) {
	int i;
	for(i = 0; i < ship->length; i++) {
		if(ship->line[i] == l && ship->column[i] == c) {
			ship->hit[i] = true;
			break;
		}
	}
}

bool shipIsDestroyed(Ship ship) {
	int i;
	for(i = 0; i < ship->length; i++) {
		if(!ship->hit[i]) { 
			return false;
		}
	}
	return true;
}

int calculateShipLength(int type, int shape) {
	int length = type;
	
	if(shape == SPECIAL) { length = SPECIAL_SIZE; }
	
	return length;
}

int getShipType(int nr_points, int shape) {
	if(shape == RECTANGLE) {
		if(nr_points >= R2 && nr_points <= R5) { return nr_points; }
		else { return -1; }
	}
	else {
		if(nr_points == 3) { return L; }
		else { return -1; }
	}
}

void printShip(Ship ship) {
	if(ship == NULL) {
		return;
	}
	
	printf("---------------\n");
	switch(ship->shape) {
		case RECTANGLE: printf("Shape: Rectangle\n"); break;
		case SPECIAL: printf("Shape: Special\n"); break;
		default: printf("Shape: Unknown\n");
	}
	
	switch(ship->type) {
		case R2: printf("Ship type: 2\n"); break;
		case R3: printf("Ship type: 3\n"); break;
		case R4: printf("Ship type: 4\n"); break;
		case R5: printf("Ship type: 5\n"); break;
		case L: printf("Ship type: L\n"); break;
		default: printf("Shape: Unknown\n");
	}
	
	printf("Length: %d\n",ship->length);
	
	printf("Cells occupied: ");
	if(ship->line!=NULL && ship->column!=NULL) {
		int i;
		for(i = 0; i < ship->length; i++) {
			int l = ship->line[i];
			int c = ship->column[i];

			printf("[%d,%d] ",c,l);
		}
		printf("\n");
	}
	else { 
		printf("Unknown\n"); 
		printf("---------------\n");
		return; 
	}
	
	printf("Positions where ship was hit: ");
	if(ship->hit!=NULL) {
		int i;
		for(i = 0; i < ship->length; i++) {
			int l = ship->line[i];
			int c = ship->column[i];
			bool hit = ship->hit[i];
			
			if(hit) { printf("[%d,%d] ",c,l); }
		}
		printf("\n");
	}	
	printf("---------------\n");
}






