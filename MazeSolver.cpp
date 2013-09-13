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
	int dimension = 4;

	// Generate walls for the maze given the dimension
	std::vector<std::vector<int>> walls = MazeGenerator::GenerateMaze(dimension);

	for (int i = 0; i < walls.size(); ++i) {
		std::cout<<walls[i][0]<<"\t"<<walls[i][1]<<"\t"<<walls[i][2]<<"\t"<<walls[i][3]<<"\n";
	}

	// Get the path that solves the maze
	int *path = MazeSolver::SolveMaze(walls);

	std::cout<<"done\n";
	return 0;
}
