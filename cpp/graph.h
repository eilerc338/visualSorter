#pragma once

#include "SDL.h"
#include <vector>

#define CUSHION		2	//pixels between bars
#define EDGE_WIDTH	3

//MAIN color
#define MAIN_COLOR_R 0xFF
#define MAIN_COLOR_G 0xFF
#define MAIN_COLOR_B 0xFF
#define MAIN_COLOR_A SDL_ALPHA_OPAQUE

//red color
#define RED_COLOR_R 0xFF
#define RED_COLOR_G 0x00
#define RED_COLOR_B 0x00
#define RED_COLOR_A SDL_ALPHA_OPAQUE

//green color
#define GREEN_COLOR_R 0x00
#define GREEN_COLOR_G 0xFF
#define GREEN_COLOR_B 0x00
#define GREEN_COLOR_A SDL_ALPHA_OPAQUE

//blue color
#define BLUE_COLOR_R 0x00
#define BLUE_COLOR_G 0x00
#define BLUE_COLOR_B 0xFF
#define BLUE_COLOR_A SDL_ALPHA_OPAQUE

//clear color
#define CLEAR_COLOR_R 0x00
#define CLEAR_COLOR_G 0x00
#define CLEAR_COLOR_B 0x00
#define CLEAR_COLOR_A SDL_ALPHA_OPAQUE

typedef SDL_Rect surface_t;

enum color {
    CLEAR,
    MAIN,
    RED,
    GREEN,
    BLUE,
};

void changeColor(SDL_Renderer *renderer, enum color color);

class bar
{
	public:
		bar() = default;
		bar(int xO, int yO, int xL, int yL) : rect{xO, yO, xL, yL} {}

		// setters / getters
		void setX(int x) {rect.x = x;}
		void setY(int y) {rect.y = y;}
		void setW(int w) {rect.w = w;}
		void setH(int h) {rect.h = h;}
		void setColor(enum color color) {m_color = color;}

		int draw(SDL_Renderer *renderer)
		{
			changeColor(renderer, m_color);
			return SDL_RenderFillRect(renderer, &rect);
		}

		SDL_Rect rect;

	private:
		enum color m_color = MAIN;
};

class barGraph
{
	public:
		void init(const surface_t *surf) {m_fullSurface = surf;}
		bool resize(int size, int minValue, int maxValue);
		bool draw(SDL_Renderer *renderer, const std::vector<int>& data);
		bool clear(SDL_Renderer *renderer);
		void initBars(int barW, int stride);

	private:
		const surface_t *m_fullSurface;
		surface_t m_graphSurface;
		std::vector<bar> graph;
		int m_minValue = 0;
		int m_maxValue = 0;
		int m_resolution = 0;
};
