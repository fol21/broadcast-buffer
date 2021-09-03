# C/C++ compiler
CC = gcc  
CXX = g++

# Linker
LD = ld

# Libraries to link with (include '-l')
LIBS = -pthread -lpthread -lrt

# Compiler and linker flags (warning levels, optimisation level, 

# include debugging symbols, add include search path, add library search path)
CFLAGS = -Wall -Iinclude
OFLAGS = -g -O -c

# Folders
BIN = ./bin
OBJ = ./src/obj

#Objects
_FOBJ = buffer.o queue.o vector.o
FOBJ := $(_FOBJ:%.o=./src/obj/%.o)

#Params
PARAMS = 16 1 1 1

buffer: ./src/buffer.c ./include/buffer.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o

queue: ./src/queue.c ./include/queue.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o

vector: ./src/vector.c ./include/vector.h
	$(CC) $(LIBS) ${CFLAGS} $(OFLAGS) $< -o ./src/obj/$@.o


dependencies: $(FOBJ)
	make buffer
	make queue
	make vector

% : %.c $(FOBJ)
	make dependencies
	$(CC) $(LIBS) $(CFLAGS) -o $(BIN)/$@ $^

%.test : test/%.test.c $(FOBJ)
	make dependencies
	$(CC) $(LIBS) $(CFLAGS) -o $(BIN)/test/$@ $^
	clear
	$(BIN)/test/$@ $(PARAMS)

%.o : %.c $(FOBJ)
	make dependencies
	$(CC) $(LIBS) $(CFLAGS) ${OFLAGS} -o $@ $^
