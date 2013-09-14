#include "MazeGenerator.h"
#include "MazeSolver.h"

#include <cstdio>
#include <ctime>

int* MazeSolver::SolveMaze(std::vector<std::vector<int>> walls)
{
	// Implment your algorithm here
	return NULL;
}

int main(int argc,char *argv[])
{
	// The dimension of the maze
	int dimension = 4;

	// Generate walls for the maze given the dimension
	std::vector<std::vector<int>> walls = MazeGenerator::GenerateMaze(dimension);

	// Timer
	// Used to compute the time spent by the maze solving algorithm
	// Enable it if you want to measure the time
	// std::clock_t startTime;
	// startTime = std::clock();

	// Get the path that solves the maze
	int *path = MazeSolver::SolveMaze(walls);

	// Timer continued
	// double duration = (std::clock() - startTime) / (double) CLOCKS_PER_SEC;
	// std::cout<<"Time spent: "<<duration<<"\n"

	return 0;
}
