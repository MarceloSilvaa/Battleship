#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "aux.h"
#include "game.h" 

int main(int argc, char* argv[]) {
	
	int len = 0, ships = 0;
	bool input = false;
	int type;
	char *dir;  
	
	if((dir = getcwd(NULL, 0)) == NULL) {
		printf("Error finding directory.\n");
		return 1;
	}  
	
	//No arguments used
	if(argc == 1) {
		
		int i;
		for(i = 1; i <= TRYS; i++) {
			FILE *config;
			char buf1[5], buf2[5], buf7[3]; 
			
			printf("Hello. Welcome to the battleship.");
			
			printf("\n\nHow to play:\n\n");
			printf("-> Single shell (Enter 1)\n");
			printf("-> Two shells with files control (Enter 2)\n");
			printf("\nAnswer: ");
			
			if(fgets(buf7, sizeof(buf7), stdin) != NULL) {
				clearInput(buf7, sizeof(buf7));
				type = strInInterval(SINGLE, FILES, buf7);
				
				if(type < 0) {
					system("clear");
					printf("\nInvalid option. %d more trys...\n", (TRYS-i));
					printf("\n\n\n\n");
					continue;
				}
			}
			
			printf("\n");
			
			if(type == FILES) {
				if(textFileExists("Config")) {
					
					printf("Waiting for configuration.\n\n");
					
					ReadConfig:
					config = fopen("Config.txt", "a+");
					fscanf(config, "%d\n%d", &len, &ships);
					fclose(config);
					
					if(len == 0 && ships == 0) {
						sleep(3);
						goto ReadConfig;
					}
					
					textFileRemove("Config");
					
					printf("There is already a player waiting. \n");
					printf("Configurations were choosen by that player.\n\n");
					
					printf("Gameboard size: %d\n", len);
					printf("Number of ships: %d\n\n", ships);
					input = true;
					break;
				}
			} 
			
			config = fopen("Config.txt", "w+");  
			fprintf(config, "%d\n%d", 0, 0);
			fclose(config);
			
			printf("Board configuration:\n\n");
			printf("-> Insert grid length (%d-%d): ", MIN_LEN, MAX_LEN);
			
			if(fgets(buf1, sizeof(buf1), stdin) != NULL) {
				clearInput(buf1, sizeof(buf1));
				len = strInInterval(MIN_LEN, MAX_LEN, buf1);
				if(len < 0) {
					system("clear");
					printf("\nInvalid option. %d more trys...\n", (TRYS-i));
					printf("\n\n\n\n");
					continue;
				}
			}
			
			int interval[2] = {0,0};
			validShips(interval, len);
			
			printf("-> Insert the number of ships(%d-%d): ", interval[0], interval[1]);
			
			if(fgets(buf2, sizeof(buf2), stdin) != NULL) {
				clearInput(buf2, sizeof(buf2));
				ships = strInInterval(interval[0], interval[1], buf2);
				if(ships < 0) {
					system("clear");
					printf("\nInvalid option. %d more trys...\n", (TRYS-i));
					printf("\n\n\n\n");
					continue;
				}
			}
			
			printf("\n");
			
			textFileRemove("Config");
			config = fopen("Config.txt", "w+");  
			fprintf(config, "%d\n%d", len, ships);
			fclose(config);
			
			input = true;
			break;	
		}
	}
	else if(argc == 2) {
		//  ./battleship test
		// Creates a 5x5 game with 5 ships
		
		char* test = argv[1];
		
		if(strLength(test) != 4) { 
			printf("\nError: Wrong arguments.\n\n");
			return 0; 
		}
		
		if(strcmp(test,"test")) { 
			printf("\nError: Wrong arguments.\n\n");
			return 0; 
		}
		
		len = 5;
		ships = 5;
		input = true;
		type = FILES;
	}
	//Bad use of arguments
	else {
		printf("Invalid number of arguments.\n");
		printf("Execute the code without arguments to see the available options.\n");
		return 0;
	}
	
	if(input == false) {
		return 0;
	}
	
	// Usernames
	
	char buf3[16], buf4[16];
	char buf5[8] = {'P','l','a','y','e','r','1','\0'};
	char buf6[8] = {'P','l','a','y','e','r','2','\0'};
	char buf8[9] = {'O','p','p','o','n','e','n','t','\0'}; //buf7 already used
	char buf9[7] = {'P','l','a','y','e','r','\0'}; 
	
	printf("Please choose your usernames.\n");
	printf(" . Limit of 15 characters\n\n");
	printf("-> Player 1: ");
	
	if(fgets(buf3, sizeof(buf3), stdin) != NULL) {
		clearInput(buf3, sizeof(buf3));
				
		if(type == SINGLE) {
			printf("\n-> Player 2: ");
			if(fgets(buf4, sizeof(buf4), stdin) != NULL) {
				clearInput(buf4, sizeof(buf4));
				printf("\n");
				
				if(strLength(buf3) <= 0) {
					if(strLength(buf4) <= 0) {
						initialize(type, len, ships, strDuplicate(buf5), strDuplicate(buf6));
					}
					else {
						initialize(type, len, ships, strDuplicate(buf5), strDuplicate(buf4));
					}
				}
				else {
					if(strLength(buf4) <= 0) {
						initialize(type, len, ships, strDuplicate(buf3), strDuplicate(buf6));
					}
					else {
						initialize(type, len, ships, strDuplicate(buf3), strDuplicate(buf4));
					}
				}
			}
		}
		else {
			if(strLength(buf3) <= 0) {
				if(textFileExists(buf5)) {
					textFileCreate(buf6);
					initialize(type, len, ships, strDuplicate(buf6), strDuplicate(buf8));
				}
				else {
					textFileCreate(buf5);
					initialize(type, len, ships, strDuplicate(buf5), strDuplicate(buf8));
				}
			}
			else {
				if(textFileExists(buf3)) {
					textFileCreate(buf9);
					initialize(type, len, ships, strDuplicate(buf9), strDuplicate(buf8));
				}
				else {
					textFileCreate(buf3);
					initialize(type, len, ships, strDuplicate(buf3), strDuplicate(buf8));
				}
			}
		}
	}
    return 0;
}
