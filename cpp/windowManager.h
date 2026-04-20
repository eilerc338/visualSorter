#pragma once

#include "SDL.h"
#include <vector>

//MAIN color
#define MAIN_COLOR_R 0xFF
#define MAIN_COLOR_G 0xFF
#define MAIN_COLOR_B 0xFF
#define MAIN_COLOR_A SDL_ALPHA_OPAQUE

//green color
#define GREEN_COLOR_R 0x00
#define GREEN_COLOR_G 0xFF
#define GREEN_COLOR_B 0x00
#define GREEN_COLOR_A SDL_ALPHA_OPAQUE

typedef SDL_Rect surface_t;

class bar
{
	public:
		bar() = delete;
		bar(unsigned int xO, unsigned int yO, unsigned int xL, unsigned int yL)
			: rect{static_cast<int>(xO), static_cast<int>(yO), static_cast<int>(xL), static_cast<int>(yL)} {}

		int draw(SDL_Renderer *renderer)
		{
			SDL_SetRenderDrawColor(renderer, GREEN_COLOR_R, GREEN_COLOR_G, GREEN_COLOR_B, GREEN_COLOR_A);
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
