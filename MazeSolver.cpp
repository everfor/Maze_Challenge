#include "MazeGenerator.h"
#include "MazeSolver.h"

#include <cstdio>
#include <ctime>
#include <cmath>

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
	// Initialize random seed
	time_t seed = time(NULL);
	std::cout << "Seed: " << seed << std::endl;
	srand(seed);
	
	if(argc != 3) {
		std::cout << "Usage: maze <dimension> <tries>" << std::endl;
		exit(1);
	}
	// The dimension of the maze
	int dimension = atoi(argv[1]);
	int tries = atoi(argv[2]);
	
	if(dimension < 1 || tries < 1) {
		std::cout << "Invalid input, please retry" << std::endl;
		exit(1);
	}

	double totalDuration = 0.d;
	
	for(int i = 0 ; i < tries ; i++) {
		std::cout << "Starting run #" << (i + 1) << std::endl;
		// Generate walls for the maze given the dimension
		std::vector<std::vector<int> > walls = MazeGenerator::GenerateMaze(dimension);
		
		std::clock_t startTime;
		startTime = std::clock();

		// Get the path that solves the maze
		std::vector<int> path = MazeSolver::SolveMaze(walls);

		double duration = (std::clock() - startTime) / (double) CLOCKS_PER_SEC;

		// Path validation
		if(!MazeSolver::ValidatePath(dimension, walls, path)) {
			std::cout << "Your solution for this run is invalid. Please check your algorithm." << std::endl;
			exit(1);
		}
		
		totalDuration += duration;
		std::cout << "Run #" << (i + 1) << " done." << std::endl;
	}
	
	std::cout << "Done! Your average time was " << (totalDuration / tries) << "s, over " 
				<< tries << " runs on mazes of dimension " << dimension << std::endl;

	return 0;
}
