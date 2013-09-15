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

// A simple example algorithm that solves the maze
std::vector<int> MazeSolver::ExampleSolver(std::vector<std::vector<int> > walls)
{
	// The path that solves the maze
	std::vector<int> path;

	// Store the status of visited cells
	std::vector<bool> visited (walls.size(), false);

	int totalNumber = walls.size();						// Total number of cells
	int dimension = (int) sqrt((float)totalNumber);	// Get dimension of the maze
	int currentCell = 0;								// Start from cell 0

	path.push_back(currentCell);

	while (currentCell < totalNumber - 1) {
		visited[currentCell] = true;	// Mark current cell as visited

		std::vector<int> neighbors;

		if (currentCell % dimension != 0 && currentCell > 0) {
			// Left neighbor
			// If it is adjacent to current cell and has not been visited,
			// Add it to valid neighbors list
			if (walls[currentCell][0] == 0 && !visited[currentCell - 1]) {
				neighbors.push_back(currentCell - 1);
			}
		}
		if (currentCell % dimension != dimension - 1 && currentCell < totalNumber - 1) {
			// Right neighbor
			// If it is adjacent to current cell and has not been visited,
			// Add it to valid neighbors list
			if (walls[currentCell][2] == 0 && !visited[currentCell + 1]) {
				neighbors.push_back(currentCell + 1);
			}
		}
		if (currentCell >= dimension) {
			// Upper neighbor
			// If it is adjacent to current cell and has not been visited,
			// Add it to valid neighbors list
			if (walls[currentCell][1] == 0 && !visited[currentCell - dimension]) {
				neighbors.push_back(currentCell - dimension);
			}
		}
		if (currentCell < totalNumber - dimension) {
			// Lower neighbor
			// If it is adjacent to current cell and has not been visited,
			// Add it to valid neighbors list
			if (walls[currentCell][3] == 0 && !visited[currentCell + dimension]) {
				neighbors.push_back(currentCell + dimension);
			}
		}

		if (neighbors.size() > 0) {
			// If there are valid neighbors
			// Take the first one and move to it
			currentCell = neighbors[0];
			path.push_back(currentCell);
		} else {
			// Otherwise go back to previous cell
			path.pop_back();
			currentCell = path.back();
		}
	}
	
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
			if (walls[path[i]][2] == 1) {
				return false;
			}
		} else if (difference == -1) {
			// The next cell is left to current cell and there is a wall to the left
			if (walls[path[i]][0] == 1) {
				return false;
			}
		} else if (difference == dimension) {
			// The next cell is lower to current cell
			if (walls[path[i]][3] == 1) {
				return false;
			}
		} else if (difference == 0 - dimension) {
			// The next cell is upper to current cell
			if (walls[path[i]][1] == 1) {
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
	// std::vector<int> path = MazeSolver::ExampleSolver(walls);

	// Timer continued
	// double duration = (std::clock() - startTime) / (double) CLOCKS_PER_SEC;
	// std::cout<<"Time spent: "<<duration<<"\n"

	// Validate your path
	// bool validation = MazeSolver::ValidatePath(dimension, walls, path);
	// std::cout<<validation<<std::endl;

	return 0;
}
