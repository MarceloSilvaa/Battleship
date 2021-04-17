#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>     
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>   
#include <errno.h>     
#include <semaphore.h>    
#include <fcntl.h>       
#include "game.h"
#include "aux.h"
#include "ship.h"

#ifdef MATRIX
#include "gamematrix.h"
#else
#include "gamequad.h"
#endif

void initialize(int type, int len, int ship_nr, char* user1, char* user2) {
	int ships1[NR_TYPES] = {4,3,2,1,0};
	int ships2[NR_TYPES] = {4,3,2,1,0};
	
	distrShips(ships1, ship_nr);
	arrayDuplicate(ships2, ships1, NR_TYPES);
	
	srand(time(0));
	
	GAMEBOARD board1 = initializeStruct(len);
	GAMEBOARD board2 = initializeStruct(len);
	
	bool config1 = placement(ships1, len, user1, board1);
	
	if(!config1) {
		printf("Error: %s caused a problem in the placement phase of the game.\n\n", user1);
		return;
	}
	
	if(type == SINGLE) {
		bool config2 = placement(ships2, len, user2, board2);
	
		if(!config2) {
			printf("Error: %s caused a problem in the placement phase of the game.\n\n", user2);
			return;
		}
		
		game(user1, user2, ship_nr, len, board1, board2);
	}
	else if(type == FILES) {
		gameFiles(user1, user2, ship_nr, len, board1, board2);
	}
	
	freeStruct(len, board1);
	freeStruct(len, board2);
}

bool placement(int* ships, int len, char* user, GAMEBOARD board) {
	system("clear");
	
	printf("Game state: Placing pieces.\n\n");
	printf("Turn: %s\n\n",user);
	
	int val = choosePlacement();
	
	if(val == MANUAL) {
		manualPlacement(ships, len, board);
	}
	else if(val == RANDOM) {
		randomPlacement(ships, len, board); 
	}
	else { 
		return false; 
	}
	
	return true;
}

void manualPlacement(int* ships, int len, GAMEBOARD board) {
	while(!isEmpty(ships)) {	
		
		system("clear");
		drawOneBoard(len, board, PLACEMENT);
		
		printf("\nShips not yet introduced:\n\n");
		printf("Type L: %d\n", ships[4]);
		printf("Type 5: %d\n", ships[3]);
		printf("Type 4: %d\n", ships[2]);
		printf("Type 3: %d\n", ships[1]);
		printf("Type 2: %d\n\n", ships[0]);
		
		Ship ship = askToInsertShip(len);
		
		if(ship == NULL) {
			printf("Error: Invalid input coordinates.\n\n");
			continue; 
		}
		
		bool available = shipIsAvailable(ship->type, ships);
		
		if(!available) {
			printf("\nError: Not enough ships of that type.\n\n");
			continue; 
		}
		
		bool success = placeShip(ship, len, board);
		
		if(!success) {
			printf("\nError: Not enough space to place the ship.\n\n");
			continue; 
		}
		
		switch(ship->type) {
			case R2: ships[0] = ships[0] - 1; break;
			case R3: ships[1] = ships[1] - 1; break;
			case R4: ships[2] = ships[2] - 1; break;
			case R5: ships[3] = ships[3] - 1; break;
			case L:  ships[4] = ships[4] - 1; break;
		}
	}
}

void randomPlacement(int* ships, int len, GAMEBOARD board) {
	
	SHAPE shape;
	Ship ship;
	int size = len*len;
	int l[5] = {0,0,0,0,0};
	int c[5] = {0,0,0,0,0};
	int i, j, k, p, o, t = 4;	//p-> position; o-> orientation; t-> type;
	do {
		p = rand() % size;
		i = p / len;
		j = p % len;

		if(t == 0 || t == 1 || t == 2 || t == 3) {
			shape = RECTANGLE; 
			o = rand() % 2;
		} else {
			shape = SPECIAL; 
			o = rand() % 4;
		}

		switch(o) {
			case 0:
				if(t == 4) {
					if(i-1 < 0 || j+1 >= len) { break; }
					l[0] = i; c[0] = j;
					l[1] = i-1; c[1] = j;
					l[2] = i; c[2] = j+1;
				} else {
					if(j+t+1 >= len) { break; }
					for(k = 0; k<t+2; k++) {
						l[k] = i;
						c[k] = j+k;
					}
				}
				break;
			case 1:
				if(t == 4) {
					if(i+1 >= len || j+1 >= len) { break; }
					l[0] = i; c[0] = j;
					l[1] = i; c[1] = j+1;
					l[2] = i+1; c[2] = j;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
				} else {
					if(i+t+1 >= len) { break; }
					for(k = 0; k<t+2; k++) {
						l[k] = i+k;
						c[k] = j;
					}
				}
				break;
			case 2:
				if(i+1 >= len || j-1 < 0) { break; }
				l[0] = i; c[0] = j;
				l[1] = i+1; c[1] = j;
				l[2] = i; c[2] = j-1; 
				break;
			case 3:
				if(i-1 < 0 || j-1 < 0) { break; }
				l[0] = i; c[0] = j;
				l[1] = i; c[1] = j-1;
				l[2] = i-1; c[2] = j;
				break;
			default: break;
		}
		if(l[0]==l[1] && c[0]==c[1]) {
			continue;
		}
		ship = makeShip(t+2, shape, l, c);

		if(placeShip(ship, len, board)) {
			ships[t]--;
			if(ships[t] <= 0) { t--;}
		}

	} while(ships[0] > 0);
}

void game(char* user1, char* user2, int ship_nr, int len, GAMEBOARD board1, GAMEBOARD board2) {
	int turn = PLAYER1;
	int ships1 = ship_nr;
	int ships2 = ship_nr;
	int row = 0;
	int col = 0;
	int res = 0; 
	
	while(ships1 > 0 && ships2 > 0) {
		showGameState(-1, turn, user1, user2, len, board1, board2);

		if(!askPlay(len, &row, &col)) {
			printf("Coordinates refused. \n\n");
			continue;
		}
		
		if(turn == PLAYER1) {
			res = makePlay(row, col, len, board2);
			if(res == -1) {
				printf("You already played that coordinate.\n\n");
				continue;
			}
		}
		else {
			res = makePlay(row, col, len, board1);
			if(res == -1) {
				printf("You already played that coordinate.\n\n");
				continue;
			}
		}
		
		if(res == MISS) { 
			printf("Shot missed.\n"); 
		}
		else if(res == HITSHIP) { 
			printf("Ship hit.\n"); 
		}
		else {
			printf("Ship destroyed.\n");  
			if(turn == PLAYER1) {
				ships2--;
			}
			else {
				ships1--;
			}
		}
		
		if(turn == PLAYER1) {
			turn = PLAYER2;
		}
		else {
			turn = PLAYER1;
		}
	}
	
	if(ships1 == 0) {
		showGameState(PLAYER2, END, user1, user2, len, board1, board2);
	}
	else {
		showGameState(PLAYER1, END, user1, user2, len, board1, board2);
	}
}

bool askPlay(int len, int* row, int* col) {
	char buf[5];
	int i,j;
	
	printf("Coordinate to hit (CL): ");
	
	if(fgets(buf, sizeof(buf), stdin) != NULL) {
		clearInput(buf, sizeof(buf));
		
		if(!isCoordinate(buf)) {
			return false;
		}
		
		i = getLineFromCoord(buf);
		j = getColumnFromCoord(buf);
		
		if(!isInsideBoard(len, i, j)) {
			return false;
		}
		
		*row = i; 
		*col = j;
	}
	else { 
		return false;
	}
	
	return true;
}

void showGameState(int winner, int turn, char* user1, char* user2, int len, GAMEBOARD board1, GAMEBOARD board2) {
	system("clear");
	if(turn == PLAYER1) {
		printf("Game state: Playing.\n\n");
		printf("Turn: %s\n\n", user1);
	}
	else if(turn == PLAYER2){
		printf("Game state: Playing.\n\n");
		printf("Turn: %s\n\n", user2);
	}
	else {
		printf("Game state: Game finished.\n\n");
		if(winner == PLAYER1) {
			printf("%s won the game.\n\n", user1);
		}
		else {
			printf("%s won the game.\n\n", user2);
		}
	}
	drawBothBoards(turn, len, board1, board2);
	printf("\n");
}

//  Multiple shell game with files 

void gameFiles(char* user1, char* user2, int ship_nr, int len, GAMEBOARD board1, GAMEBOARD board2) {
	key_t shmkey;                 
    int shmid;                    
    sem_t *sem;

	FILE *play;
	FILE *reply;
	FILE *players;
	FILE *board;

	int i, j;
	int turn = PLAYER1;
	int ships1 = ship_nr;
	int ships2 = ship_nr;
	int row = -1;
	int col = -1;
	int res = 0;
	
	char *u1 = malloc(15*sizeof(char));
	char *u2 = malloc(15*sizeof(char));
	char *userfile = malloc(20*sizeof(char)); 
	char *dir;  
	
	if((dir = getcwd(NULL, 0)) == NULL) {
		printf("Error finding directory.\n");
		return;
	}   

    shmkey = ftok(dir, 5); 
    shmid = shmget(shmkey, sizeof (int), 0644 | IPC_CREAT);

    if (shmid < 0) {                          
        perror("shmget\n");
        exit(1);
    }

    sem = sem_open("/battleship", O_CREAT, 0644, 1);

    system("clear");
	printf("Game state: Playing.\n\n");
	printf("Turn: %s\n\n", user2);
	drawBothBoards(turn, len, board1, board2);

    while(ships1 > 0 && ships2 > 0) {
		sem_wait(sem);

		makeplay:
		row = -1;
		col = -1;
   		play = fopen("Play.txt", "a+");
   		fscanf(play, "%d\n%d", &row, &col);

		if(row == -1 && col == -1) { 
			repeat:
			showGameState(-1, turn, user1, user2, len, board1, board2);
			if(!askPlay(len, &row, &col)) {
				printf("Coordinates refused. \n\n");
				goto repeat;
			} else if(!checkPlay(row, col, board2)) {
				printf("You already played that coordinate.\n\n");
				goto repeat;
			}

			fprintf(play, "%d\n%d", row, col);
			fclose(play);

		} else {
			reply = fopen("Reply.txt", "w");
			res = makePlay(row, col, len, board1);
			if(res != SHIPDESTROYED) {
				fprintf(reply, "%d", res);
			} else {
				ships1--;
				fprintf(reply, "%d\n", res);
				Ship ship = getShip(row,col,board1);
				fprintf(reply, "%d\n", ship->length); 
				for(i = 0; i < ship->length; i++) { 
					fprintf(reply, "%d %d\n", ship->line[i], ship->column[i]); 
				}
			}
			fclose(reply);
		}

		sem_post(sem);

		sleep(2);

		sem_wait(sem);

		reply = fopen("Reply.txt", "r");
		fscanf(reply, "%d", &res);
		if(res == MISS) { 
			printf("Shot missed.\n");
			updatePlayFiles(row, col, MISS, board2);
		} else if(res == HITSHIP) { 
			printf("Ship hit.\n");
			updatePlayFiles(row, col, HITSHIP, board2); 
		} else if(res == SHIPDESTROYED) {
			printf("Ship destroyed.\n");
			int l;
			int r, c;
			fscanf(reply, "%d", &l);
			for(i = 0; i < l; i++) {
				fscanf(reply, "%d %d", &r, &c);
				updatePlayFiles(r, c, SHIPDESTROYED, board2);
			}
			ships2--;
		} else {
			if(ships1 > 0 && ships2 > 0) {
				goto makeplay;
			} else {
				goto end;
			}
		}

		system("clear");
		printf("Game state: Playing.\n\n");
		printf("Turn: %s\n\n", user2);
		drawBothBoards(turn, len, board1, board2);
		printf("\n");
		fclose(reply);
		reply = fopen("Reply.txt", "w");
		fprintf(reply, "0");
		fclose(reply);
		play = fopen("Play.txt", "w");
		fclose(play);

		end:
		sem_post(sem);

		sleep(2);
	}

	sem_wait(sem);

	players = fopen("Players.txt", "a");
	fprintf(players, "%s\n", user1);
	fclose(players);
	strcpy(userfile, user1);
	strcat(userfile, ".txt");
	board = fopen(userfile, "w");
	drawBoardInFile(board, len, board1);
	fclose(board);

	sem_post(sem);

	sleep(2);

	sem_wait(sem);

	players = fopen("Players.txt", "r");
	fscanf(players, "%s\n%s", u1, u2);
	fclose(players);

	sem_post(sem);

	sem_close(sem);

	user2 = getOtherPlayerName(user1, u1, u2);
	strcpy(userfile, user2);
	strcat(userfile, ".txt");
	board = fopen(userfile, "r");
	
	int** b = (int**) malloc(len*sizeof(int*));
	for(i = 0; i < len; i++) {
		b[i] = malloc(len*sizeof(int));
		for(j = 0; j < len; j++) {
			fscanf(board, "%d", &b[i][j]);
		}
	}

	system("clear");
	printf("Game state: Game finished.\n\n");

	if(ships1 == 0) {
		printf("%s won the game.\n\n", user2);
	} else {
		printf("You won the game.\n\n");
	}

	drawBothBoardsFiles(len, board1, b);
	
	deleteFiles(user1, user2);
}

char* getOtherPlayerName(char *user, char *user1, char *user2) {
	if(strcmp(user, user1) != 0) {
		return user1;
	}
	return user2;
}

void deleteFiles(char* user1, char* user2) {
	textFileRemove(user2);
	
	while(textFileExists(user1)) {
		sleep(5);
	}
	
	textFileRemove(user1);
	textFileRemove("Play");
	textFileRemove("Reply");
	textFileRemove("Players");
}
