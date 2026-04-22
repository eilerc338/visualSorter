#pragma once

#include "graph.h"
#include "SDL.h"
#include <vector>

class windowManager
{
	public:
		//constructors
		windowManager() = default;
		windowManager(unsigned int width, unsigned int height);

		//getters/setters
		const surface_t * getSurface()
		{
			if (m_nextSurface != surfaces.end()) {
				return &(*m_nextSurface++);
			}
			return nullptr;
		}

		SDL_Renderer * getRenderer() {return m_mainRenderer;}

		bool init(unsigned int numSurfaces);
		void makeLayout(unsigned int numSurfaces);
		void drawBorders();
		void updateScreen();

		//debug
		void debugPrintBorders();

	private:
		void createBorders();
		void createSurfaces();

		std::vector<surface_t>::iterator m_nextSurface;
		SDL_Window *m_mainWin = nullptr;
		SDL_Renderer *m_mainRenderer = nullptr;
		std::vector<surface_t> surfaces;
		std::vector<bar> borders;
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_rows;
		unsigned int m_columns;
};
