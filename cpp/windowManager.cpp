#include "windowManager.h"

#include "SDL.h"
#include <iostream>

void windowManager::updateScreen()
{
    SDL_RenderPresent(m_mainRenderer);
}

void windowManager::drawBorders()
{
	for (auto &b : borders) {
		b.draw(m_mainRenderer);
	}
}

void windowManager::makeLayout(unsigned int numSurfaces)
{
	float targetRatio = (float)m_width / m_height;

	//figure out rows and columns using screen resolution and number of algs
	for (unsigned int i = 1; i < numSurfaces; ++i) {
		if (i + 1 > m_rows * m_columns) {
			// would adding to the m_columns or m_rows result in a closer ratio to the target?
			if (abs(((m_columns+1) / m_rows) - targetRatio) <= abs((m_columns / (m_rows+1)) - targetRatio)) {
				m_columns++;
			} else {
				m_rows++;
			}
		}
	}
}

void windowManager::debugPrintBorders()
{
	std::cout << std::endl;
	std::cout << "m_rows = " << m_rows << std::endl;
	std::cout << "m_columns = " << m_columns << std::endl;
	std::cout << "borders.size() = " << borders.size() << std::endl;

	for(const auto &b : borders) {
		std::cout << std::endl;
		std::cout << "x = " << b.rect.x << std::endl;
		std::cout << "y = " << b.rect.y << std::endl;
		std::cout << "w = " << b.rect.w << std::endl;
		std::cout << "h = " << b.rect.h << std::endl;
	}
}

void windowManager::createBorders()
{
	unsigned int xOrigin = 0;
	unsigned int yOrigin = 0;

	// vertical borders
	for (unsigned int i = 0; i < m_columns; ++i) {
		if (i < m_columns) {
			xOrigin += surfaces[i].x;

			// xO, yO, xL, yL
			if (i == 0) {
				// left most border
				borders.push_back(bar(0, 0, EDGE_WIDTH, m_height));
			} else {
				// middle vertical borders
				borders.push_back(bar(xOrigin - (EDGE_WIDTH * 2), 0, EDGE_WIDTH * 2, m_height));
			}

			if (i == m_columns - 1) {
				// right most border
				borders.push_back(bar(m_width - EDGE_WIDTH, 0, EDGE_WIDTH, m_height));
			}
		}
	}

	// horizontal borders
	xOrigin = 0;
	for (unsigned int i = 0; i < m_rows; ++i) {
		if (i < m_rows) {
			yOrigin += surfaces[i * m_columns].y;

			// xO, yO, xL, yL
			if (i == 0) {
				// top border
				borders.push_back(bar(0, 0, m_width, EDGE_WIDTH));
			} else {
				// middle horizontal borders
				borders.push_back(bar(0, yOrigin - (EDGE_WIDTH * 2), m_width, EDGE_WIDTH * 2));
			}

			if (i == m_rows - 1) {
				// bottom most border
				borders.push_back(bar(0, m_height - EDGE_WIDTH, m_width, EDGE_WIDTH));
			}
		}
	}

	debugPrintBorders();
}

void windowManager::createSurfaces()
{
	int xLength = m_width / m_columns;
	int yLength = m_height / m_rows;
	int rows = static_cast<int>(m_rows);
	int columns = static_cast<int>(m_columns);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			// xOrigin, yOrigin, xLength, yLength
			surfaces.push_back(surface_t{j * xLength + EDGE_WIDTH, i * yLength + EDGE_WIDTH, xLength - (2 * EDGE_WIDTH), yLength - (2 * EDGE_WIDTH)});
		}
	}
	m_nextSurface = surfaces.begin();
}

bool windowManager::init(unsigned int numSurfaces)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error SDL_Init: " << SDL_GetError() << std::endl;
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1") )
	{
		std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
	}

	//Create window
	m_mainWin = SDL_CreateWindow( "Visual Sorter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_SHOWN );
	if (m_mainWin == NULL)
	{
		std::cout <<  "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		//Create renderer for window
		m_mainRenderer = SDL_CreateRenderer( m_mainWin, -1, SDL_RENDERER_ACCELERATED );

		//try software render if hardware fails
		if (m_mainRenderer == NULL)
		{
			std::cout << "Accelerated renderer could not be created! SDL Error: " << SDL_GetError() << std::endl; 
			std::cout << "Switching to software renderer" << std::endl;
			m_mainRenderer = SDL_CreateRenderer( m_mainWin, -1, SDL_RENDERER_SOFTWARE);
		}

		if (m_mainRenderer == NULL)
		{
			std::cout <<  "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
			return false;
		}
	}
	
	// Figure out how many rows and columns
	makeLayout(numSurfaces);

	// create surfaces
	createSurfaces();

	// create borders
	createBorders();

	return true;
}

// Constructors
windowManager::windowManager(unsigned int width, unsigned int height)
	: m_width(width), m_height(height)
{
	m_rows = 1;
	m_columns = 1;
}

