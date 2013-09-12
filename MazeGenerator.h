#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include <vector>

class MazeGenerator
{
	public:
		static std::vector<std::vector<int>> GenerateMaze(int dimension);
		static std::vector<std::vector<int>> GenerateWalls(int width, int height);
};

#endif