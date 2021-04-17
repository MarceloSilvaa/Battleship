CFLAGS = -Wall -std=c99 -lpthread 
CC = gcc
PROGRAM = ./battleship
OBJS_CORE = main.o aux.o ship.o game.o
OBJS_MATRIX = gamematrix.o
OBJS_QUAD = gamequad.o quadtree.o
OBJS_ALL = $(OBJS_CORE) $(OBJS_MATRIX) $(OBJS_QUAD)

all: quad
	
matrix: clean $(OBJS_CORE) $(OBJS_MATRIX)
	$(CC) -D MATRIX -o $(PROGRAM) $(OBJS_CORE) $(OBJS_MATRIX) -lpthread
		
quad: clean $(OBJS_CORE) $(OBJS_QUAD)
	$(CC) -D QUAD -o $(PROGRAM) $(OBJS_CORE) $(OBJS_QUAD) -lpthread 
	
clean:
		rm -f $(PROGRAM) $(OBJS_ALL)
