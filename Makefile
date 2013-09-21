CC=g++
LD=g++

CC_FLAGS= -g -Wall
LDFLAGS=

OBJECTS=MazeSolver.o MazeGenerator.o

all:$(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o maze 

$(OBJECTS):%.o:%.cpp
	$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f maze

