#include "MazeDrawer.h"
#include <iostream>

// Constants
const int MARGIN = 10;
const int CELL_SIDE = 2;
const int WALL_WIDTH = 1;

void swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// Fill a pixel with specified color
void MazeDrawer::FillPixel(SDL_Surface* screen, int x, int y, Uint32 color)
{
	if (x < screen -> w && y < screen -> h) {	
		Uint32* pixels = (Uint32*) screen->pixels;
		Uint32* pixel = pixels + y * screen->pitch / 4 + x;
		*pixel = color;
	}
}

// Fill a cell
void MazeDrawer::FillCell(SDL_Surface* screen, int x, int y, Uint32 color)
{
	MazeDrawer::FillPixel(screen, x, y, color);
	MazeDrawer::FillPixel(screen, x + 1, y, color);
	MazeDrawer::FillPixel(screen, x, y + 1, color);
	MazeDrawer::FillPixel(screen, x + 1, y + 1, color);
}

// Bresenham's Algorithm to draw a line by filling pixels
// http://www.youtube.com/watch?v=jzFAZK9RUMk
void MazeDrawer::DrawLine(SDL_Surface* screen, int startX, int startY, int endX, int endY, Uint32 color)
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
			MazeDrawer::FillPixel(screen, y, x, color);
		} else {
			MazeDrawer::FillPixel(screen, x, y, color);
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
	
	// Preserve the cout functions for console
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	
	// Init SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface* screen = SDL_SetVideoMode(sizeOfScreen, sizeOfScreen, 32, SDL_SWSURFACE);
	Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
	Uint32 black = SDL_MapRGB(screen->format, 0, 0, 0);

	bool run = true;
	const int FPS = 10;
	Uint32 startTick;

	while (run) {
		startTick = SDL_GetTicks();
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					run = false;
					break;
			}
		}

		SDL_FillRect(screen, &screen->clip_rect, white);

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
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
				}
				if (walls[i][1]) {
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, black);
				}
				if (walls[i][2]) {
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, 
									MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
				}

				horiOffst = 0;
				vertOffst += CELL_SIDE + WALL_WIDTH;
			} else {
				// Draw left and upper walls for other cells
				if (walls[i][0]) {
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
				}
				if (walls[i][1]) {
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, black);
				}

				horiOffst += CELL_SIDE + WALL_WIDTH;
			}
		}

		// Draw walls for cells in last rows
		// Save the last cell
		for (int i = totalCells - dimension; i < totalCells - 1; i++) {
			// Draw left, upper and lower walls
			if (walls[i][0]) {
				MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
			}
			if (walls[i][1]) {
				MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, black);
			}
			if (walls[i][3]) {
				MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, 
								MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
			}

			horiOffst += CELL_SIDE + WALL_WIDTH;
		}

		// Draw all four walls for the last cell
		if (walls[totalCells - 1][0]) {
			MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
							MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
		}
		if (walls[totalCells - 1][1]) {
			MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, black);
		}
		if (walls[totalCells - 1][2]) {
			MazeDrawer::DrawLine(screen, MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
		}
		if (walls[totalCells - 1][3]) {
			MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
		}

		SDL_Flip(screen);

		// Maintain the specified FPS rate
		if (1000 / FPS > SDL_GetTicks() - startTick) {
			SDL_Delay(1000 / FPS - SDL_GetTicks() + startTick);
		}
	}

	SDL_Quit();
}

// Draw Maze with a Path
void MazeDrawer::DrawMaze(std::vector<std::vector<int> > walls, std::vector<int> path)
{
	const int dimension = (int) sqrt((float) walls.size());
	const int totalCells = walls.size();
	const int sizeOfScreen = dimension * (CELL_SIDE + WALL_WIDTH) + 2 * MARGIN;
	const int pathLength = path.size();
	
	// Preserve the cout functions for console
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	
	// Init SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface* screen = SDL_SetVideoMode(sizeOfScreen, sizeOfScreen, 32, SDL_SWSURFACE);
	Uint32 white = SDL_MapRGB(screen->format, 255, 255, 255);
	Uint32 black = SDL_MapRGB(screen->format, 0, 0, 0);

	bool run = true;
	const int FPS = 10;
	Uint32 startTick;

	// Path
	int xPos, yPos, diff;
	Uint32 green = SDL_MapRGB(screen -> format, 102, 255, 0);
	Uint32 red = SDL_MapRGB(screen -> format, 255, 69, 0);
	bool nextRed = false;
	bool test = true;

	int horiOffst, vertOffst;
	
	while (run) {
		startTick = SDL_GetTicks();
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					run = false;
					break;
			}
		}

		SDL_FillRect(screen, &screen->clip_rect, white);

		// Horizontal Offset and Vertical Offset
		horiOffst = 0;
		vertOffst = 0;

		// Draw Maze
		for (int i = 0; i < totalCells - dimension; i++) {
			// Iterate until second last row
			if (i % dimension == dimension - 1) {
				// Draw left, upper and right walls for rightmost cells
				if (walls[i][0]) {
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
				}
				if (walls[i][1]) {
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, black);
				}
				if (walls[i][2]) {
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, 
									MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
				}

				horiOffst = 0;
				vertOffst += CELL_SIDE + WALL_WIDTH;
			} else {
				// Draw left and upper walls for other cells
				if (walls[i][0]) {
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
				}
				if (walls[i][1]) {
					MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
									MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, black);
				}

				horiOffst += CELL_SIDE + WALL_WIDTH;
			}
		}

		// Draw walls for cells in last rows
		// Save the last cell
		for (int i = totalCells - dimension; i < totalCells - 1; i++) {
			// Draw left, upper and lower walls
			if (walls[i][0]) {
				MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
			}
			if (walls[i][1]) {
				MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
								MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, black);
			}
			if (walls[i][3]) {
				MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, 
								MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
			}

			horiOffst += CELL_SIDE + WALL_WIDTH;
		}

		// Draw all four walls for the last cell
		if (walls[totalCells - 1][0]) {
			MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
							MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
		}
		if (walls[totalCells - 1][1]) {
			MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, black);
		}
		if (walls[totalCells - 1][2]) {
			MazeDrawer::DrawLine(screen, MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
		}
		if (walls[totalCells - 1][3]) {
			MazeDrawer::DrawLine(screen, MARGIN + horiOffst, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, 
							MARGIN + horiOffst + CELL_SIDE + WALL_WIDTH, MARGIN + vertOffst + CELL_SIDE + WALL_WIDTH, black);
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
						MazeDrawer::FillCell(screen, xPos, yPos, green);
					} else {
						MazeDrawer::FillCell(screen, xPos, yPos, red);
						nextRed = false;
					}
					// Fill the white space
					MazeDrawer::FillPixel(screen, xPos + CELL_SIDE, yPos, green);
					MazeDrawer::FillPixel(screen, xPos + CELL_SIDE, yPos + 1, green);
				} else {
					MazeDrawer::FillCell(screen, xPos, yPos, red);
					nextRed = true;
				}
			} else if (diff == -1) {
				// left neighbor
				if (!walls[path[i]][0]) {
					if (!nextRed) {
						MazeDrawer::FillCell(screen, xPos, yPos, green);
					} else {
						MazeDrawer::FillCell(screen, xPos, yPos, red);
						nextRed = false;
					}
					// Fill the white space
					MazeDrawer::FillPixel(screen, xPos - 1, yPos, green);
					MazeDrawer::FillPixel(screen, xPos - 1, yPos + 1, green);
				} else {
					MazeDrawer::FillCell(screen, xPos, yPos, red);
					nextRed = true;
				}
			} else if (diff == dimension) {
				// lower neighbor
				if (!walls[path[i]][3]) {
					if (!nextRed) {
						MazeDrawer::FillCell(screen, xPos, yPos, green);
					} else {
						MazeDrawer::FillCell(screen, xPos, yPos, red);
						nextRed = false;
					}
					// Fill the white space
					MazeDrawer::FillPixel(screen, xPos, yPos + CELL_SIDE, green);
					MazeDrawer::FillPixel(screen, xPos + 1, yPos + CELL_SIDE, green);
				} else {
					MazeDrawer::FillCell(screen, xPos, yPos, red);
					nextRed = true;
				}
			} else if (diff == 0 - dimension) {
				// upper neighbor
				if (!walls[path[i]][1]) {
					if (!nextRed) {
						MazeDrawer::FillCell(screen, xPos, yPos, green);
					} else {
						MazeDrawer::FillCell(screen, xPos, yPos, red);
						nextRed = false;
					}
					// Fill the white space
					MazeDrawer::FillPixel(screen, xPos, yPos - 1, green);
					MazeDrawer::FillPixel(screen, xPos + 1, yPos - 1, green);
				} else {
					MazeDrawer::FillCell(screen, xPos, yPos, red);
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
			MazeDrawer::FillCell(screen, xPos, yPos, green);
		} else {
			MazeDrawer::FillCell(screen, xPos, yPos, red);
		}

		SDL_Flip(screen);

		if (1000 / FPS > SDL_GetTicks() - startTick) {
			SDL_Delay(1000 / FPS - SDL_GetTicks() + startTick);
		}
	}

	SDL_Quit();
}