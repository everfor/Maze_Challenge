#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include <vector>

class MazeSolver
{
	public:
		static std::vector<int> SolveMaze(std::vector<std::vector<int> > walls);
		static bool ValidatePath(int dimension, std::vector<std::vector<int> > walls, std::vector<int> path);
};

#endif
