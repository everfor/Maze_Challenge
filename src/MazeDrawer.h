#ifndef MAZEDRAWER_H
#define MAZEDRAWER_H

#include <vector>
#include "png.hpp"

class MazeDrawer
{
	public:
		static void FillPixel(png::image<png::rgb_pixel>& image, int x, int y, int red, int green, int blue);
		static void FillCell(png::image<png::rgb_pixel>& image, int x, int y, int red, int green, int blue);
		static void DrawLine(png::image<png::rgb_pixel>& image, int startX, int startY, int endX, int endY, int red, int green, int blue);
		static void DrawMaze(std::vector<std::vector<int> > walls);
		static void DrawMaze(std::vector<std::vector<int> > walls, std::vector<int> path);
};

#endif
