#include "MazeGenerator.h"
#include "MazeSolver.h"

int* MazeSolver::SolveMaze(std::vector<std::vector<int>> walls)
{
	// Implment your algorithm here
	return NULL;
}

int main()
{
	// The dimension of the maze
	int dimension = 20;

	// Generate walls for the maze given the dimension
	std::vector<std::vector<int>> walls = MazeGenerator::GenerateMaze(dimension);

	// Get the path that solves the maze
	int *path = MazeSolver::SolveMaze(walls);

	std::cout<<"done\n";
	return 0;
}
