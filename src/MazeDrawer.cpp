#include "MazeDrawer.h"

#include "png.hpp"
#include <iostream>

// Constants
const int MARGIN = 10;
const int CELL_SIDE = 2;
const int WALL_WIDTH = 1;

const int BLACK_R = 0;
const int BLACK_G = 0;
const int BLACK_B = 0;

const int GREEN_R = 0;
const int GREEN_G = 255;
const int GREEN_B = 0;

const int RED_R = 255;
const int RED_G = 0;
const int RED_B = 0;

void swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// Fill a pixel with specified color
void MazeDrawer::FillPixel(png::image<png::rgb_pixel>& image, int x, int y, int red, int green, int blue)
{
	if ((size_t)x < image.get_width() && (size_t)y < image.get_height()) {	
		image.set_pixel(x, y, png::rgb_pixel(red, green, blue));
	}
}

// Fill a cell
void MazeDrawer::FillCell(png::image<png::rgb_pixel>& image, int x, int y, int red, int green, int blue)
{
	MazeDrawer::FillPixel(image, x, y, red, green, blue);
	MazeDrawer::FillPixel(image, x + 1, y, red, green, blue);
	MazeDrawer::FillPixel(image, x, y + 1, red, green, blue);
	MazeDrawer::FillPixel(image, x + 1, y + 1, red, green, blue);
}

// Bresenham's Algorithm to draw a line by filling pixels
// http://www.youtube.com/watch?v=jzFAZK9RUMk
void MazeDrawer::DrawLine(png::image<png::rgb_pixel>& image, int startX, int startY, int endX, int endY, int red, int green, int blue)
{
	bool step = abs(endX - startX) < abs(endY - startY);
	if (step) {
		swap(startX, startY);
		swap(endX, endY);
	}
	if (endX < startX) {
		swap(startX, endX);
		swap(startY, endY);
	}

	float error = 0.0;
	float slope = (float) abs(endY - startY) / (endX - startX);
	int y = startY, x = startX;
	int ystep = startY > endY? -1 : 1;

	while (x <= endX) {
		if (step) {
			MazeDrawer::FillPixel(image, y, x, red, green, blue);
		} else {
			MazeDrawer::FillPixel(image, x, y, red, green, blue);
		}

		error += slope;
		if (error >= 0.5) {
			y += ystep;
			error -= 1.0;
		}

		++x;
	}
}

// Draw a maze
void MazeDrawer::DrawMaze(std::vector<std::vector<int> > walls)
{
	const int dimension = (int) sqrt((float) walls.size());
	const int totalCells = walls.size();
	const int sizeOfScreen = dimension * (CELL_SIDE + WALL_WIDTH) + 2 * MARGIN;
	
	// Init png++
	png::image<png::rgb_pixel> image(sizeOfScreen, sizeOfScreen);

	for (size_t i = 0; i < image.get_width(); i++) {
		for (size_t j = 0; j < image.get_height(); j++) {
			image.set_pixel(i, j, png::rgb_pixel(255, 255, 255));
		}
	}

	// Horizontal Offset and Vertical Offset
	int horiOffst = 0, vertOffst = 0;

	// Implement the drawing function here instead of creating a new method
	// to save memory usage and improve performance
	// Draw Maze
	for (int i = 0; i < totalCells - dimension; i++) {
		// Iterate until second last row
		if (i % dimension == dimension - 1) {
			// Draw left, upper and right walls for rightmost cells
			if (walls[i][0]) {
				MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
			}
			if (walls[i][1]) {
				MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, BLACK_R, BLACK_G, BLACK_B);
			}
			if (walls[i][2]) {
				MazeDrawer::DrawLine(image, MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, 
								MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
			}

			horiOffst = 0;
			vertOffst += CELL_SIDE + WALL_WIDTH;
		} else {
			// Draw left and upper walls for other cells
			if (walls[i][0]) {
				MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
			}
			if (walls[i][1]) {
				MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, BLACK_R, BLACK_G, BLACK_B);
			}

			horiOffst += CELL_SIDE + WALL_WIDTH;
		}
	}

	// Draw walls for cells in last rows
	// Save the last cell
	for (int i = totalCells - dimension; i < totalCells - 1; i++) {
		// Draw left, upper and lower walls
		if (walls[i][0]) {
			MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
							MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
		}
		if (walls[i][1]) {
			MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, BLACK_R, BLACK_G, BLACK_B);
		}
		if (walls[i][3]) {
			MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
		}

		horiOffst += CELL_SIDE + WALL_WIDTH;
	}

	// Draw all four walls for the last cell
	if (walls[totalCells - 1][0]) {
		MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
						MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
	}
	if (walls[totalCells - 1][1]) {
		MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
						MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, BLACK_R, BLACK_G, BLACK_B);
	}
	if (walls[totalCells - 1][2]) {
		MazeDrawer::DrawLine(image, MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, 
						MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
	}
	if (walls[totalCells - 1][3]) {
		MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, 
						MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
	}

	image.write("out.png");
}

// Draw Maze with a Path
void MazeDrawer::DrawMaze(std::vector<std::vector<int> > walls, std::vector<int> path)
{
	const int dimension = (int) sqrt((float) walls.size());
	const int totalCells = walls.size();
	const int sizeOfScreen = dimension * (CELL_SIDE + WALL_WIDTH) + 2 * MARGIN;
	const int pathLength = path.size();
	
	// Init png++
	png::image<png::rgb_pixel> image(sizeOfScreen, sizeOfScreen);

	// Path
	int xPos, yPos, diff;
	bool nextRed = false;
	bool test = true;

	int horiOffst, vertOffst;
	

		// Horizontal Offset and Vertical Offset
		horiOffst = 0;
		vertOffst = 0;

		// Draw Maze
		for (int i = 0; i < totalCells - dimension; i++) {
			// Iterate until second last row
			if (i % dimension == dimension - 1) {
				// Draw left, upper and right walls for rightmost cells
				if (walls[i][0]) {
					MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
				}
				if (walls[i][1]) {
					MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, BLACK_R, BLACK_G, BLACK_B);
				}
				if (walls[i][2]) {
					MazeDrawer::DrawLine(image, MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, 
									MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
				}

				horiOffst = 0;
				vertOffst += CELL_SIDE + WALL_WIDTH;
			} else {
				// Draw left and upper walls for other cells
				if (walls[i][0]) {
					MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
				}
				if (walls[i][1]) {
					MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, BLACK_R, BLACK_G, BLACK_B);
				}

				horiOffst += CELL_SIDE + WALL_WIDTH;
			}
		}

		// Draw walls for cells in last rows
		// Save the last cell
		for (int i = totalCells - dimension; i < totalCells - 1; i++) {
			// Draw left, upper and lower walls
			if (walls[i][0]) {
				MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
			}
			if (walls[i][1]) {
				MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, BLACK_R, BLACK_G, BLACK_B);
			}
			if (walls[i][3]) {
				MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, 
								MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
			}

			horiOffst += CELL_SIDE + WALL_WIDTH;
		}

		// Draw all four walls for the last cell
		if (walls[totalCells - 1][0]) {
			MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
							MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
		}
		if (walls[totalCells - 1][1]) {
			MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, BLACK_R, BLACK_G, BLACK_B);
		}
		if (walls[totalCells - 1][2]) {
			MazeDrawer::DrawLine(image, MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
		}
		if (walls[totalCells - 1][3]) {
			MazeDrawer::DrawLine(image, MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, BLACK_R, BLACK_G, BLACK_B);
		}

		// Draw path
		for (int i = 0; i < pathLength - 1; i++) {
			xPos = (int) path[i] % dimension;
			yPos = (int) path[i] / dimension;

			xPos = MARGIN + WALL_WIDTH + xPos * (CELL_SIDE + WALL_WIDTH);
			yPos = MARGIN + WALL_WIDTH + yPos * (CELL_SIDE + WALL_WIDTH);

			diff = path[i + 1] - path[i];

			if (diff == 1) {
				// right neighbor
				if (!walls[path[i]][2]) {
					if (!nextRed) {
						MazeDrawer::FillCell(image, xPos, yPos, GREEN_R, GREEN_G, GREEN_B);
					} else {
						MazeDrawer::FillCell(image, xPos, yPos, RED_R, RED_G, RED_B);
						nextRed = false;
					}
					// Fill the white space
					MazeDrawer::FillPixel(image, xPos + CELL_SIDE, yPos, GREEN_R, GREEN_G, GREEN_B);
					MazeDrawer::FillPixel(image, xPos + CELL_SIDE, yPos + 1, GREEN_R, GREEN_G, GREEN_B);
				} else {
					MazeDrawer::FillCell(image, xPos, yPos, RED_R, RED_G, RED_B);
					nextRed = true;
				}
			} else if (diff == -1) {
				// left neighbor
				if (!walls[path[i]][0]) {
					if (!nextRed) {
						MazeDrawer::FillCell(image, xPos, yPos, GREEN_R, GREEN_G, GREEN_B);
					} else {
						MazeDrawer::FillCell(image, xPos, yPos, RED_R, RED_G, RED_B);
						nextRed = false;
					}
					// Fill the white space
					MazeDrawer::FillPixel(image, xPos - 1, yPos, GREEN_R, GREEN_G, GREEN_B);
					MazeDrawer::FillPixel(image, xPos - 1, yPos + 1, GREEN_R, GREEN_G, GREEN_B);
				} else {
					MazeDrawer::FillCell(image, xPos, yPos, RED_R, RED_G, RED_B);
					nextRed = true;
				}
			} else if (diff == dimension) {
				// lower neighbor
				if (!walls[path[i]][3]) {
					if (!nextRed) {
						MazeDrawer::FillCell(image, xPos, yPos, GREEN_R, GREEN_G, GREEN_B);
					} else {
						MazeDrawer::FillCell(image, xPos, yPos, RED_R, RED_G, RED_B);
						nextRed = false;
					}
					// Fill the white space
					MazeDrawer::FillPixel(image, xPos, yPos + CELL_SIDE, GREEN_R, GREEN_G, GREEN_B);
					MazeDrawer::FillPixel(image, xPos + 1, yPos + CELL_SIDE, GREEN_R, GREEN_G, GREEN_B);
				} else {
					MazeDrawer::FillCell(image, xPos, yPos, RED_R, RED_G, RED_B);
					nextRed = true;
				}
			} else if (diff == 0 - dimension) {
				// upper neighbor
				if (!walls[path[i]][1]) {
					if (!nextRed) {
						MazeDrawer::FillCell(image, xPos, yPos, GREEN_R, GREEN_G, GREEN_B);
					} else {
						MazeDrawer::FillCell(image, xPos, yPos, RED_R, RED_G, RED_B);
						nextRed = false;
					}
					// Fill the white space
					MazeDrawer::FillPixel(image, xPos, yPos - 1, GREEN_R, GREEN_G, GREEN_B);
					MazeDrawer::FillPixel(image, xPos + 1, yPos - 1, GREEN_R, GREEN_G, GREEN_B);
				} else {
					MazeDrawer::FillCell(image, xPos, yPos, RED_R, RED_G, RED_B);
					nextRed = true;
				}
			}
		}

		// Fill last cell
		xPos = (int) path[pathLength - 1] % dimension;
		yPos = (int) path[pathLength - 1] / dimension;

		xPos = MARGIN + WALL_WIDTH + xPos * (CELL_SIDE + WALL_WIDTH);
		yPos = MARGIN + WALL_WIDTH + yPos * (CELL_SIDE + WALL_WIDTH);

		if (!nextRed) {
			MazeDrawer::FillCell(image, xPos, yPos, GREEN_R, GREEN_G, GREEN_B);
		} else {
			MazeDrawer::FillCell(image, xPos, yPos, RED_R, RED_G, RED_B);
		}

		image.write("out.png");
}