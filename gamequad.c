#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "gamequad.h"
#include "aux.h"
#include "quadtree.h"

GAMEBOARD initializeStruct(int length) { 
	return newQTree(length);
}

bool placeShip(Ship ship, int len, GAMEBOARD qt) {
	if(!hasSpace(ship, len, qt)) { 
		return false; 
	}
	
	int length = ship->length;
	int i;
	
	for(i = 0; i < length; i++) {
		int l = ship->line[i];
		int c = ship->column[i];	
		Cell cell = makeCell(ship, NOHIT);
	
		insertCell(qt, l, c, cell);
	}
	
	return true;
}

void markShipDestroyed(Ship ship, int len, GAMEBOARD qt) {
	int i;
	for(i = 0; i < ship->length; i++) {
		int row = ship->line[i];
		int col = ship->column[i];
		Cell cell = searchCell(qt, row, col);
		
		if(cell != NULL) {
			cell->hit = SHIPDESTROYED;
		}
	}
}

int makePlay(int row, int col, int len, GAMEBOARD qt) {
	Cell cell = searchCell(qt, row, col);
	
	if(cell == NULL) {
		cell = makeCell(NULL, NOHIT);
		insertCell(qt, row, col, cell);
	}
	
	if(cell->hit != NOHIT) {
		return -1;
	}
	
	if(cell->ship == NULL) {
		cell->hit = MISS;
		return MISS;
	}
	else {
		cell->hit = HITSHIP;
		updateCoordHit(cell->ship, row, col);
		
		if(shipIsDestroyed(cell->ship)) {
			markShipDestroyed(cell->ship, len, qt);
			return SHIPDESTROYED;
		}
		else {
			return HITSHIP;
		}
	}
}

bool checkPlay(int row, int col, GAMEBOARD qt) {
	Cell cell = searchCell(qt, row, col);

	if(cell == NULL) {
		return true;
	}

	if(cell->hit != NOHIT) {
		return false;
	}

	return true;
}

void updatePlayFiles(int row, int col, int value, GAMEBOARD qt) {
	Cell cell = searchCell(qt, row, col);
	
	if(cell == NULL) {
		cell = makeCell(NULL, value);
		insertCell(qt, row, col, cell);
	} else {
		cell->hit = value;
	}
}

Ship getShip(int row, int col, GAMEBOARD qt) {
	Cell cell = searchCell(qt, row, col);
	return cell->ship;
}

bool hasSpace(Ship ship, int len, GAMEBOARD qt) {
	int length = ship->length;
	int i;
	
	for(i = 0; i < length; i++) {
		int l = ship->line[i];
		int c = ship->column[i];
		
		if(searchCell(qt, l, c) != NULL) { 
			return false; 
		}
	}
	return true;
}

void drawOneBoard(int len, GAMEBOARD qt, int flag) {
	Cell** matrix = qtreeToMatrix(qt);
	drawMatrix(len, matrix, flag);
}

void drawBothBoards(int turn, int len, GAMEBOARD qt1, GAMEBOARD qt2) {
	Cell** matrix1 = qtreeToMatrix(qt1);
	Cell** matrix2 = qtreeToMatrix(qt2);
	drawMatrixBothPlayers(turn, len, matrix1, matrix2);
}

void drawBothBoardsFiles(int len, GAMEBOARD qt1, int** qt2) {
	Cell** matrix1 = qtreeToMatrix(qt1);
	drawMatrixBothPlayersFiles(len, matrix1, qt2);
}

void drawBoardInFile(FILE *file, int len, GAMEBOARD qt) {
	Cell** matrix = qtreeToMatrix(qt);
	drawMatrixBoardInFile(file, len, matrix);
}

void freeStruct(int len, GAMEBOARD qt) {
	freeQTree(qt);
}
