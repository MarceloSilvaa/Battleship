#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "gamematrix.h"
#include "aux.h"
#include "ship.h"

GAMEBOARD initializeStruct(int len) {
	int i,j;
	
	GAMEBOARD matrix = (Cell**) malloc(len * sizeof(Cell*));
	for(i = 0; i < len; i++) {
		matrix[i] = (Cell*) malloc(len * sizeof(Cell));
		for(j = 0; j < len; j++) {
			matrix[i][j] = makeCell(NULL, NOHIT);
		}
	}
	return matrix;
}

bool placeShip(Ship ship, int len, GAMEBOARD matrix) {	
	if(!hasSpace(ship, len, matrix)) { 
		return false; 
	}
	
	int length = ship->length;
	int i;
	
	for(i = 0; i < length; i++) {
		int l = ship->line[i];
		int c = ship->column[i];
		
		matrix[l][c]->ship = ship;
	}
	return true;
}


void markShipDestroyed(Ship ship, int len, GAMEBOARD matrix) {
	int i;
	for(i = 0; i < ship->length; i++) {
		int row = ship->line[i];
		int col = ship->column[i];
		matrix[row][col]->hit = SHIPDESTROYED;
	}
}

int makePlay(int row, int col, int len, GAMEBOARD matrix) {
	if(matrix[row][col]->hit != NOHIT) {
		return -1;
	}
	
	if(matrix[row][col]->ship == NULL) {
		matrix[row][col]->hit = MISS;
		return MISS;
	}
	else {
		matrix[row][col]->hit = HITSHIP;
		updateCoordHit(matrix[row][col]->ship, row, col);
		
		if(shipIsDestroyed(matrix[row][col]->ship)) {
			markShipDestroyed(matrix[row][col]->ship, len, matrix);
			return SHIPDESTROYED;
		}
		else {
			return HITSHIP;
		}
	}
}

bool checkPlay(int row, int col, GAMEBOARD matrix) {
	if(matrix[row][col]->hit != NOHIT) {
		return true;
	}
	return false;
}

void updatePlayFiles(int row, int col, int value, GAMEBOARD matrix) {
	matrix[row][col]->hit = value;
}

Ship getShip(int row, int col, GAMEBOARD matrix) {
	return matrix[row][col]->ship;
}

bool hasSpace(Ship ship, int len, GAMEBOARD matrix) {
	int length = ship->length;
	int i;
	
	for(i = 0; i < length; i++) {
		int l = ship->line[i];
		int c = ship->column[i];
		
		if(matrix[l][c]->ship != NULL) { 
			return false; 
		}
	}
	return true;
}

void drawOneBoard(int len, GAMEBOARD matrix, int flag) {
	drawMatrix(len, matrix, flag);
}

void drawBothBoards(int turn, int len, GAMEBOARD matrix1, GAMEBOARD matrix2) {
	drawMatrixBothPlayers(turn, len, matrix1, matrix2);
}

void drawBothBoardsFiles(int len, GAMEBOARD matrix1, int** matrix2) {
	drawMatrixBothPlayersFiles(len, matrix1, matrix2);
}

void drawBoardInFile(FILE *file, int len, GAMEBOARD matrix) {
	drawMatrixBoardInFile(file, len, matrix);
}

void freeStruct(int len, GAMEBOARD board) {
	int i, j, k;
	
	// Free all ships
	for(i = 0; i < len; i++) {
		for(j = 0; j < len; j++) {
			if(board[i][j]->ship == NULL) {
				continue;
			}
			
			int length = board[i][j]->ship->length;
			
			for(k = 0; k < length; k++) {
				int row, col;
				
				row = board[i][j]->ship->line[k];
				col = board[i][j]->ship->column[k];
				
				if(row == i && col == j) {
					continue;
				}
				
				board[row][col]->ship = NULL;
			}
			
			free(board[i][j]->ship);
		}
	}
	
	// Free board
	for(i = 0; i < len; i++) {
		for(j = 0; j < len; j++) {
			freeCell(board[i][j]);
		}
		free(board[i]);
	}
	free(board);
}
