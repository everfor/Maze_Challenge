#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

Class MazeGenerator
{
	public:
		static int* GenerateMaze(int dimension);
		static int* GenerateWalls(int width, int height);
};

#endif