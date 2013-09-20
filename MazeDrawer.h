#ifndef MAZEDRAWER_H
#define MAZEDRAWER_H

#include <vector>
#include "SDL.h"

class MazeDrawer
{
	public:
		static void FillPixel(SDL_Surface* screen, int x, int y, Uint32 color);
		static void DrawLine(SDL_Surface* screen, int startX, int startY, int endX, int endY, Uint32 color);
		static void DrawMaze(std::vector<std::vector<int> > Maze);
};

#endif
