#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <iostream>
#include <algorithm>	// Use of std::fill
#include <stack>		// Use of std::stack
#include <stdlib.h>		// Use of srand and rand
#include <time.h>		// Used to generate seeds for random number
#include <vector>

class MazeGenerator
{
	public:
		static std::vector<std::vector<int> > GenerateMaze(int dimension);
		static std::vector<std::vector<int> > GenerateWalls(int width, int height);
};

#endif
