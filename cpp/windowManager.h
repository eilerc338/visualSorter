#pragma once

#include "SDL.h"
#include <vector>

typedef SDL_Rect surface_t;

class bar
{
	public:
		bar() = delete;
		bar(unsigned int xO, unsigned int yO, unsigned int xL, unsigned int yL)
			: rect{static_cast<int>(xO), static_cast<int>(yO), static_cast<int>(xL), static_cast<int>(yL)} {}

		int draw(SDL_Renderer *renderer)
		{
			return SDL_RenderFillRect(renderer, &rect);
		}

		SDL_Rect rect;
};

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

		bool init(unsigned int numSurfaces);
		void makeLayout(unsigned int numSurfaces);
		void drawBorders();

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
