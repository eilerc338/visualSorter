#CC = g++
#CC = clang
CC = gcc

#OBJS specifies which files to compile as part of the project
#OBJS = $(wildcard *.cpp)
OBJS = $(wildcard *.c)

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = visual_sorter_c

CFLAGS = -Wall -g $$(sdl2-config --cflags)
LIBS = $$(sdl2-config --libs)

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS) -o $(OBJ_NAME)


#C
#gcc -Wall -g `sdl2-config --cflags` -c main.c
#gcc -Wall -g `sdl2-config --cflags` -c insertionSort_alg.c
#gcc main.o insertionSort_alg.o -Wall -g `sdl2-config --libs` -o visual_sorter_c
#./visual_sorter_c
