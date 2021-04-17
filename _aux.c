#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "aux.h"

/* ********** START: Simple auxiliary functions ********** */

int min(int num1, int num2) {
	if(num1 > num2) { return num2; }
	else { return num1; }
}

int max(int num1, int num2) {
	if(num1 > num2) { return num1; }
	else { return num2; }
}

int strLength(char* str) {
	int len;
	for(len=0; str[len]!='\n' && str[len]!='\0'; len++) {}
	return len;
}

char* strDuplicate(char* str) {
	if(str == NULL) { return NULL; }
	
	int len = strLength(str);
	char* newStr = malloc(len + 1);
	char* aux = newStr;
	
	int i;
	for(i = 0; i < len; i++) {
		*aux++ = *str++;
	} 
	*aux = '\0';
	
	return newStr;
}

void arrayDuplicate(int* dst, int* src, int len) {
	if(src == NULL || dst == NULL) { return; }
	
	int i;
	for(i = 0; i < len; i++) {
		dst[i] = src[i];
	}
}

bool textFileExists(char* str) {
	int len = strLength(str);
	int i;
	
	char aux[len+5];
	for(i = 0; i < len; i++) {
		aux[i] = str[i];
	}
	
	aux[i] = '.'; i++;
	aux[i] = 't'; i++;
	aux[i] = 'x'; i++;
	aux[i] = 't'; i++;
	aux[i] = '\0';
	
	if(access(aux, F_OK) != -1) {
		return true;
	}
	return false;
}

void textFileRemove(char* str) {
	if(textFileExists(str) == false) {
		return;
	}
	
	int len = strLength(str);
	int i;
	
	char aux[len+5];
	for(i = 0; i < len; i++) {
		aux[i] = str[i];
	}
	
	aux[i] = '.'; i++;
	aux[i] = 't'; i++;
	aux[i] = 'x'; i++;
	aux[i] = 't'; i++;
	aux[i] = '\0';
	
	remove(aux); 
}

void textFileCreate(char* str) {
	if(textFileExists(str) == true) {
		return;
	}
	
	int len = strLength(str);
	int i;
	
	char aux[len+5];
	for(i = 0; i < len; i++) {
		aux[i] = str[i];
	}
	
	aux[i] = '.'; i++;
	aux[i] = 't'; i++;
	aux[i] = 'x'; i++;
	aux[i] = 't'; i++;
	aux[i] = '\0';
	
	FILE* mf = fopen(aux, "w+");
	fprintf(mf, "created\n");  
	fclose(mf);
}

/* ********** START: Simple input control ********** */

bool isInt(char* str) {
	int i;
	for(i=0; str[i]!='\n' && str[i]!='\0'; i++) {
		if(!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

int strToInt(char* str) {
	if(isInt(str)) {
		return atoi(str);
	}
	return -1;
}

int inInterval(int a, int b, int v) {
	if(v >= a && v <= b) {
		return v;
	}
	return -1;
}

int strInInterval(int a, int b, char* str) {
	int val = strToInt(str);
	if(val != -1) {
		return inInterval(a,b,val);
	}
	return -1;
}

void clearInput(char* str, int limit) {
	if(strLength(str) >= limit - 1) {
		int c;
		while ( (c = getchar()) != '\n' && c != EOF ) { }
	}
}

/* ********** START: Configuration Game-Ships ********** */

void validShips(int* interval, int mlen) {
	
	int form = (mlen*mlen - BASE_SPACE) / FOUR_SPACE; 
	
	interval[0] = BASE_SHIPS + form;
	interval[1] = BASE_SHIPS + ( form * MAX_PERC / MIN_PERC );
}

void distrShips(int* ships, int ship_nr) {
	// Developer test case
	if(ship_nr == 5) {
		int j;
		for(j = 0; j < NR_TYPES; j++) {
			ships[j] = 1;
		}
		return;
	}
	
	//General user case
	int aux1 = (ship_nr - BASE_SHIPS) / NR_TYPES;
	
	int aux2 = (ship_nr - BASE_SHIPS) % NR_TYPES;
	
	int i;
	for(i = 0; i < NR_TYPES; i++) {
		ships[i] += aux1;
	}
	
	for(i = 0; i < aux2; i++) {
		ships[i] += 1;
	}
}

/* ********** START: Input-Coordinate translation ********** */

CoordInput splitCoordinates(char* str) {
	char* fullString = strDuplicate(str);
	char* partString;
	CoordInput input = malloc(sizeof(CoordInput));
	
	// Get rectangle coordinates
	
	partString = strtok(fullString,":");
	if(partString != NULL) {
		strcpy(input->coord1,partString);
		partString = strtok(NULL,":");
	}
	
	partString = strtok(partString,",");
	if(partString != NULL) {
		strcpy(input->coord2,partString);
	}
	
	// Get special coordinate
	
	partString = strtok(NULL,",");
	if(partString != NULL) {
		strcpy(input->coord3,partString);
	}
	
	return input;
}

bool isCoordinate(char* str) {
	//CL -> ( A-Z + a-n )( 0-25 ) 
	int size = strLength(str);
	
	if(size < 2 || size > 3) { return false; } 
	
	if(!isupper(str[0])) { 
		if(str[0] < LOWER_START || str[0] > LOWER_END) { return false; }
	}
	
	if(!isdigit(str[1])) { return false; }
	
	if(size == 3 && !isdigit(str[2])) { return false; }
	
	return true;
} 

bool checkRectangleType(char* str) {
	//CL:CL
	bool ans = true;
	int size = strLength(str);
	
	if(size < 5 || size > 7) { return false; } 
	
	char* fullString = strDuplicate(str); 
	char* partString = strtok(fullString,":");

	while(partString != NULL) {
		ans = isCoordinate(partString);
		partString = strtok(NULL,":");
	}
	
	return ans;
}

bool checkSpecialType(char* str) {
	//CL:CL,CL
	bool ans = true;
	int size = strLength(str);
	
	if(size < 8 || size > 11) { return false; } 
	
	char* fullString = strDuplicate(str); 
	char* partString = strtok(fullString,":");
	
	if(partString != NULL) {
		ans = isCoordinate(partString);
		partString = strtok(NULL,":");
	}
	
	partString = strtok(partString,",");
	
	while(partString != NULL) {
		ans = isCoordinate(partString);
		partString = strtok(NULL,",");
	}
	
	return ans;
}

int getLineFromCoord(char* str) {
	int len = strLength(str);
	
	if(len < 2 || len > 3) {
		return -1;
	}
	
	if(len == 2) {
		return str[1] - '0';
	}
	else {
		return (str[1] - '0')*10 + (str[2] - '0');
	}	
}

int getColumnFromCoord(char* str) {
	int len = strLength(str);
	
	if(len < 2 || len > 3) { 
		return -1;
	}
	
	char c = str[0];
	
	if(isupper(c)) {
		return c - 'A';
	}
	
	if(c >= LOWER_START && c <= LOWER_END) {
		int dif = 'Z' - 'A' + 1;
		return c - 'a' + dif;
	}
	
	return -1;
}

/* ********** START: Placement input control ********** */

int choosePlacement() {
	printf("Choose the type of placement you want to use:\n\n");
	printf("-> Manual (0)\n");
	printf("-> Random (1)\n\n");
	
	int i;
	for(i = 1; i <= TRYS; i++) {	
		printf("Option: ");
		
		char buf[3];
		if(fgets(buf,sizeof(buf),stdin) != NULL) {
			clearInput(buf,sizeof(buf));
			
			int val = strInInterval(MANUAL,RANDOM,buf);
			if(val != -1) {
				return val;
			}
		}
		printf("Invalid option. %d more chances.\n\n",(TRYS-i));
	}
	
	return -1;
}

Ship askToInsertShip(int len) {
	printf("How to insert a ship with:\n\n");
	printf(" . Rectangle shape - CL:CL e.g. A1:A5\n");
	printf(" . L shape - CL:CL,CL e.g. A1:A2,B2\n\n");
	
	char buf[13];
	
	int i;
	for(i = 1; i <= TRYS; i++) {
		printf("Ship coordinates: ");
		
		if(fgets(buf,sizeof(buf),stdin) != NULL) {
			clearInput(buf,sizeof(buf));
			
			Ship ship = coordToShip(buf,len);
			if(ship != NULL) { return ship; }
			
			printf("Coordinates refused. %d more chances.\n\n",(TRYS-i));
		}
	}
	
	return NULL;
}

/* ********** START: Ship creation ********** */

Ship coordToShip(char* str, int len) {
	
	// Check if input represents a rectangle or a L shape
	int shape = -1; 
	
	if(checkRectangleType(str)) { shape = RECTANGLE; } 
	else {
		if(checkSpecialType(str)) { shape = SPECIAL; }
		else { return NULL; }
	} 
	
	// Create list of points
	int pointsL[5] = {0,0,0,0,0};
	int pointsC[5] = {0,0,0,0,0};
	
	int nr_points = createPointList(shape,str,len,pointsL,pointsC);
	
	if(nr_points == -1 ) { return NULL; }
	
	int type = getShipType(nr_points, shape);
	
	Ship ship = makeShip(type,shape,pointsL,pointsC);
	
	return ship;
}

int createPointList(int shape, char* str, int len, int* pointsL, int* pointsC) {
	int nr_points = 0;
	int orientation;
	
	CoordInput input = splitCoordinates(str);
	
	// Get rectangle coordinates
	
	int l1 = getLineFromCoord(input->coord1);
	int l2 = getLineFromCoord(input->coord2);
	
	int c1 = getColumnFromCoord(input->coord1);   
	int c2 = getColumnFromCoord(input->coord2);
	
	if(!isInsideBoard(len,l1,c1) || !isInsideBoard(len,l2,c2)) { return -1; }
	
	int difL = abs(l1-l2);
	int difC = abs(c1-c2);
	
	if(difL!=0 && difC!=0) {
		return -1;
	}
	
	if(difL == 0) {
		orientation = HORIZONTAL;
		if(difC < 1 || difC > 4) { return -1; }
		
		int i;
		for(i = 0; i <= difC; i++) {
			pointsL[i] = l1;
			pointsC[i] = min(c1,c2) + i;
			nr_points++;
		}
		
	}
	else {
		orientation = VERTICAL;
		if(difL < 1 || difL > 4) { return -1; }
		
		int i;
		for(i = 0; i <= difL; i++) {
			pointsL[i] = min(l1,l2) + i;
			pointsC[i] = c1;
			nr_points++;
		}
	}
	
	// Get Special coordinate
	
	if(shape == SPECIAL) {
		
		int l3 = getLineFromCoord(input->coord3);
		int c3 = getColumnFromCoord(input->coord3);
		
		if(!isInsideBoard(len,l3,c3)) { return -1; }
	
		if(!isValidSpecial(orientation, l1, l2, l3, c1, c2, c3)) { return -1; }
		
		pointsL[nr_points] = l3;
		pointsC[nr_points] = c3;
		nr_points++;
	}
	
	return nr_points;
}

/* ********** START: Game verifications ********** */

bool isInsideBoard(int len, int l, int c) {
	
	if(l < 0 || l > len - 1) {
		return false;
	}
	
	if(c < 0 || c > len - 1) {
		return false;
	}
	
	return true;
}

bool isValidSpecial(int orientation, int l1, int l2, int l3, int c1, int c2, int c3) {
	
	if(orientation == HORIZONTAL) {
		if(c3 != c1 && c3 != c2) { return false; }
		if(l3 != l1+1 && l3 != l1-1) { return false; }
	}
	else {
		if(l3 != l1 && l3 != l2) { return false; }
		if(c3 != c1+1 && c3 != c1-1) { return false; }
	}
	
	return true;
}

bool shipIsAvailable(int type, int* ships) {
	bool ans = true;
	
	if(type >= R2 && type <= R5) {
		if(ships[type-2] <= 0) {
			ans = false;
		}
	}
	else if(type == L) {
		if(ships[4] <= 0) {
			ans = false;
		}
	}
	else {
		ans = false;
	}
	
	return ans;
}

bool isEmpty(int types[NR_TYPES]) {
	int i;
	for(i = 0; i < NR_TYPES; i++) {
		if(types[i] > 0) {
			return false;
		}
	}
	return true;
}

/* ********** START: Drawing functions - Full information ********** */

void drawMatrix(int len, Cell** matrix, int flag) {
	int i;
	char optional = '\0';
	
	if(len > 10) { optional = ' '; }
	
	drawMatrixAlphaCoords(len, optional);
	printf("\n");
	
	switch(flag) {
		case PLACEMENT: 
			for(i = 0; i < len; i++) {
				drawMatrixRowPlacement(len, matrix, i, optional);
				printf("\n");
			} break;
		case VISIBLE: 
			for(i = 0; i < len; i++) {
				drawMatrixRowVisible(len, matrix, i, optional);
				printf("\n");
			} break;
		case HIDDEN: 
			for(i = 0; i < len; i++) {
				drawMatrixRowHidden(len, matrix, i, optional);
				printf("\n");
			} break;
	}
}

void drawMatrixBothPlayers(int turn, int len, Cell** matrix1, Cell** matrix2) {
	
	int i;
	char optional = '\0';
	
	if(len > 10) { optional = ' '; }
	
	drawMatrixAlphaCoords(len, optional);
	printf("     ");
	drawMatrixAlphaCoords(len, optional);
	printf("\n");
	
	switch(turn) {
		case PLAYER1:
			for(i = 0; i < len; i++) {
				drawMatrixRowVisible(len, matrix1, i, optional);
				printf("     ");
				drawMatrixRowHidden(len, matrix2, i, optional);
				printf("\n");
			} break;
		case PLAYER2:
			for(i = 0; i < len; i++) {
				drawMatrixRowHidden(len, matrix1, i, optional);
				printf("     ");
				drawMatrixRowVisible(len, matrix2, i, optional);
				printf("\n");
			} break;
		case END:
			for(i = 0; i < len; i++) {
				drawMatrixRowVisible(len, matrix1, i, optional);
				printf("     ");
				drawMatrixRowVisible(len, matrix2, i, optional);
				printf("\n");
			} break;
	}
}

void drawMatrixAlphaCoords(int len, char optional) {
	int i,j;
	printf("  %c", optional);
	
	for(i = 0; i < len; i++) {
		if('A'+i > 'Z') { break; }
		printf(COLOR_COORD("%c") " ", ('A' + i));
	}
	for(j = 0; i < len; j++) {
		printf(COLOR_COORD("%c") " ", ('a' + j));
		i++;
	}
}

void drawMatrixRowPlacement(int len, Cell** matrix, int row, char optional) {
	int col;
	
	if(row < 10) { 
		printf(COLOR_COORD("%d") " %c", row, optional); 
	}
	else { 
		printf(COLOR_COORD("%d") " ",row);
	}
		
	for(col = 0; col < len; col++) {
		if(matrix[row][col]->ship == NULL) {
			printf("%c ", WATER);
		}
		else {
			printf("%c ", SHIP);
		}
	}
}

void drawMatrixRowVisible(int len, Cell** matrix, int row, char optional) {
	int col;
	
	if(row < 10) { 
		printf(COLOR_COORD("%d") " %c", row, optional); 
	}
	else { 
		printf(COLOR_COORD("%d") " ",row);
	}
		
	for(col = 0; col < len; col++) {
		switch(matrix[row][col]->hit) {
			case NOHIT: 
				if(matrix[row][col]->ship == NULL) {
					printf("%c ", WATER);
				}
				else {
					printf("%c ", SHIP);
				}
				break;
			case MISS: printf(BLUE("%c") " ", WATER); break;
			case HITSHIP: printf(YELLOW("%c") " ", HIT); break;
			case SHIPDESTROYED: printf(RED("%c") " ", HIT); break;
		}
	}
}

void drawMatrixRowHidden(int len, Cell** matrix, int row, char optional) {
	int col;
	
	if(row < 10) { 
		printf(COLOR_COORD("%d") " %c", row, optional); 
	}
	else { 
		printf(COLOR_COORD("%d") " ",row);
	}
		
	for(col = 0; col < len; col++) {
		switch(matrix[row][col]->hit) {
			case NOHIT: printf(MAGENTA("%c") " ", UNKNOWN); break;
			case MISS: printf(BLUE("%c") " ", WATER); break;
			case HITSHIP: printf(YELLOW("%c") " ", HIT); break;
			case SHIPDESTROYED: printf(RED("%c") " ", HIT); break;
		}
	}
}

/* ********** START: Drawing functions - Information shared with files ********** */

void drawMatrixBothPlayersFiles(int len, Cell** matrix1, int** matrix2) {
	
	int i;
	char optional = '\0';
	
	if(len > 10) { optional = ' '; }
	
	drawMatrixAlphaCoords(len, optional);
	printf("     ");
	drawMatrixAlphaCoords(len, optional);
	printf("\n");

	for(i = 0; i < len; i++) {
		drawMatrixRowVisible(len, matrix1, i, optional);
		printf("     ");
		drawMatrixRowFiles(len, matrix2, i, optional);
		printf("\n");
	}
}

void drawMatrixRowFiles(int len, int** matrix, int row, char optional) {
	int col;
	
	if(row < 10) { 
		printf(COLOR_COORD("%d") " %c", row, optional); 
	}
	else { 
		printf(COLOR_COORD("%d") " ",row);
	}
		
	for(col = 0; col < len; col++) {
		switch(matrix[row][col]) {
			case NOHIT: printf("%c ", SHIP); break;
			case -1: printf("%c ", WATER); break;
			case MISS: printf(BLUE("%c") " ", WATER); break;
			case HITSHIP: printf(YELLOW("%c") " ", HIT); break;
			case SHIPDESTROYED: printf(RED("%c") " ", HIT); break;
		}
	}
}

void drawMatrixBoardInFile(FILE *file, int len, Cell** matrix) {
	int i, j;

	for(i = 0; i < len; i++) {
		for(j = 0; j < len; j++) {
			if(matrix[i][j]->hit == NOHIT) {
				if(matrix[i][j]->ship == NULL) {
					fprintf(file, "-1 ");
				}
				else {
					fprintf(file, "%d ", NOHIT);
				}
			} else {
				fprintf(file, "%d ", matrix[i][j]->hit);
			}
		}
		fprintf(file, "\n");
	}
}


