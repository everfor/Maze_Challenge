#include "MazeGenerator.h"
#include "MazeSolver.h"

#include <cstdio>
#include <ctime>

std::vector<int> MazeSolver::SolveMaze(std::vector<std::vector<int> > walls)
{
	// The path that solves the maze
	std::vector<int> path;
	
	// Implment your algorithm here
	
	// Return the final path
	return path;
}

// Validate the path for a maze
// Returns true if the path is valid, false otherwise
bool MazeSolver::ValidatePath(int dimension, std::vector<std::vector<int> > walls, std::vector<int> path)
{
	// Get the path length and total number of cells in a maze
	int pathLength = path.size();
	int totalCells = walls.size();

	// First simple check
	// Check the start and end cell
	if (path[0] != 0 || path[pathLength - 1] != totalCells - 1) {
		return false;
	}

	// Check along the path to see if it counters any walls
	for (int i = 0; i < pathLength - 1; i++) {
		// The difference of IDs between next cell and current cell
		// Used to determine the relative position of next cell
		int difference = path[i + 1] - path[i];
		
		if (difference == 1) {
			// The next cell is right to current cell and there is a wall to the right
			if (walls[i][2] == 1) {
				return false;
			}
		} else if (difference == -1) {
			// The next cell is left to current cell and there is a wall to the left
			if (walls[i][0] == 1) {
				return false;
			}
		} else if (difference == dimension) {
			// The next cell is lower to current cell
			if (walls[i][3] == 1) {
				return false;
			}
		} else if (difference == 0 - dimension) {
			// The next cell is upper to current cell
			if (walls[i][1] == 1) {
				return false;
			}
		} else {
			return false;
		}
	}

	// If the path passes validation then it is good
	return true;
}

int main(int argc,char *argv[])
{
	// The dimension of the maze
	int dimension = 2;

	// Generate walls for the maze given the dimension
	std::vector<std::vector<int> > walls = MazeGenerator::GenerateMaze(dimension);

	// Timer
	// Used to compute the time spent by the maze solving algorithm
	// Enable it if you want to measure the time
	// std::clock_t startTime;
	// startTime = std::clock();

	// Get the path that solves the maze
	std::vector<int> path = MazeSolver::SolveMaze(walls);

	// Timer continued
	// double duration = (std::clock() - startTime) / (double) CLOCKS_PER_SEC;
	// std::cout<<"Time spent: "<<duration<<"\n"

	// Validate your path
	// bool validation = MazeSolver::ValidatePath(dimension, walls, path);
	// std::cout<<validation;

	return 0;
}
