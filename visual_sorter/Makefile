#CC = g++
CC = clang

#OBJS specifies which files to compile as part of the project
OBJS = $(wildcard *.cpp)

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = visual_sorter

CFLAGS = $$(sdl2-config --cflags) 
LIBS = $$(sdl2-config --libs)

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS) -o $(OBJ_NAME)
