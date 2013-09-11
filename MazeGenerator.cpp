/*
	Maze Generator for McGill Robotics Challenges
	Author: Alan Yang
	Credits To: Maze Work (http://www.mazeworks.com/mazegen/mazetut/)
*/

#include <iostream>
#include <algorithm>	// Use of std::fill
#include <stack>		// Use of std::stack
#include <stdlib>		// Use of srand and rand
#include <time>			// Used to generate seeds for random number
#include <vector>

#include "MazeGenerator.h"	// The header file where the Maze Generator is defined

// Generate walls in a maze given a dimension
// By defualt, every cell is surrounded by 4 walls
int* MazeGenerator::GenerateWalls(int width, int height)
{
	int walls[height * width][4];
	std::fill(&walls[0][0], &walls[0][0] + sizeof(walls), 1);

	return walls;
}

// Generate a perfect maze given a dimension
// A perfect maze contains no loops and there is only one path for any two cells of the maze
int* MazeGenerator::GenerateMaze(int dimension)
{
	// Initialize random seed
	srand(time(NULL));

	// Initialize stack for storing visited cells and generate walls
	std::stack<int> cellStack;
	int* walls = MazeGenerator::GenerateWalls(dimension, dimension);

	int totalNumber = dimension * dimension;
	int currentCell = rand() % totalNumber;
	int visitedNumber = 1;

	// Loop until all cells are visited
	while (visitedNumber < totalNumber) {
		// Vector used to store the neighbors of the current cell
		std::vector<int> neighbors;
		std::vector<int> temp;
		temp.push_back(currentCell - 1);			// Right neighbor
		temp.push_back(currentCell + 1);			// Left neighbor
		temp.push_back(currentCell - dimension);	// Upper neighbor
		temp.push_back(currentCell + dimension);	// Lower neighbor

		for(int i = 0; i < temp.size(); i++) {
			if (temp[i] < 0 || temp[i] > totalNumber) {
				continue;
			} else {
				bool wallsIntact = true;
				// Preserve only cells with all walls intact
				for(int j = 0; j < 4; j++) {
					if (walls[temp[i]][j] == 0) {
						wallsIntact = false;
						break;
					}
				} 
			}

			if (wallsIntact) {
				neighbors.push_back(temp[i]);
			}
		}

		// After the for loop a list of neighbors with all walls intact are found
		// If there is/are such neighbors, choose one and break the wall between it and the current cell
		if (neighbors.size() > 0) {
			int neighbor = neighbors[rand() % neighbors.size()];

			switch(currentCell - neighbor) {
				case 1:
					// neighbor = currentCell - 1, left neighbor
					// Break the left wall of current cell and right wall of the neighbor
					walls[currentCell][0] = walls[neighbor][2] = 0;
					break;
				case 0 - 1:
					// neighbor = currentCell + 1, right neighbor
					// Break the right wall of current cell and left wall of the neighbor
					walls[currentCell][2] = walls[neighbor][0] = 0;
					break;
				case dimension:
					// neighbor = currentCell - dimension, upper neighbor
					// Break the upper wall of current cell and lower wall of the neighbor
					walls[currentCell][1] = walls[neighbor][3] = 0;
					break;
				default:
					// neighbor = currentCell + dimension, lower neighbor
					// Break the lower wall of current cell and upper wall of the neighbor
					walls[currentCell][3] = walls[neighbor][1] = 0;
			}

			// Push the current cell to the stack
			cellStack.push(currentCell);
			// Set the neighbor as the new currentCell
			currentCell = neighbor;
			// Since this neighbor is visited, increase the visited number by 1
			visitedNumber += 1;
		} else {
			// If the neighbor list contains no elements, go back to the stack and retrieve the recently pushed cell
			// And starting from that cell, re-do the process
			currentCell = cellStack.pop();
		}
	}

	// Return the walls
	return walls;
}

int main()
{
	// To be implemented
	return 0;
}