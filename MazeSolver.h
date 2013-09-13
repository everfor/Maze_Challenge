#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include <vector>

class MazeSolver
{
	public:
		static int* SolveMaze(std::vector<std::vector<int>> walls);
};

#endif
