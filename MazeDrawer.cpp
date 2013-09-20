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

void MazeDrawer::FillPixel(SDL_Surface* screen, int x, int y, Uint32 color)
{
	if (x < screen->w && y < screen -> h) {	
		Uint32* pixels = (Uint32*) screen->pixels;
		Uint32* pixel = pixels + y * screen->pitch / 4 + x;
		*pixel = color;
	}
}

// Bresenham's Algorithm
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

		// Draw Maze
		for (int i = 0; i < totalCells - dimension; i++) {
			if (i % dimension == dimension - 1) {
				// Draw walls
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

		
		for (int i = totalCells - dimension; i < totalCells - 1; i++) {
			// Draw walls
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

		if (1000 / FPS > SDL_GetTicks() - startTick) {
			SDL_Delay(1000 / FPS - SDL_GetTicks() + startTick);
		}
	}

	SDL_Quit();
}