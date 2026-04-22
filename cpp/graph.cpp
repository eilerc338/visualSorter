#include "graph.h"

#include <iostream>

void changeColor(SDL_Renderer *renderer, enum color color) {
    //todo add different color options in switch
	switch (color) {
	case CLEAR:
		SDL_SetRenderDrawColor(renderer, CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A);
		break;

	case MAIN:
		SDL_SetRenderDrawColor(renderer, MAIN_COLOR_R, MAIN_COLOR_G, MAIN_COLOR_B, MAIN_COLOR_A);
		break;

	case RED:
		SDL_SetRenderDrawColor(renderer, RED_COLOR_R, RED_COLOR_G, RED_COLOR_B, RED_COLOR_A);
		break;

	case GREEN:
		SDL_SetRenderDrawColor(renderer, GREEN_COLOR_R, GREEN_COLOR_G, GREEN_COLOR_B, GREEN_COLOR_A);
		break;

	case BLUE:
		SDL_SetRenderDrawColor(renderer, BLUE_COLOR_R, BLUE_COLOR_G, BLUE_COLOR_B, BLUE_COLOR_A);
		break;
	}
}


bool barGraph::resize(int size, int minV, int maxV)
{
	if (!m_fullSurface)
	{
		return false;
	}

	// resize graph
	graph.resize(size);

	// figure out m_graphSurface
	int stride = m_fullSurface->w - (CUSHION * 2) / size;
	int barWidth = stride - CUSHION;

	// this makes sense because stride was rounded down
	int graphWidth = stride * size;
	int graphStart = (m_fullSurface->w - graphWidth) / 2;
	m_resolution = (m_fullSurface->h - (CUSHION * 2)) / (maxV - minV);
	m_minValue = std::min(minV - 1, 0);
	m_maxValue = maxV;

	// adjust graph with new starts and width for each bar
	m_graphSurface.x = m_fullSurface->x + graphStart;
	m_graphSurface.y = m_fullSurface->y + CUSHION;
	m_graphSurface.w = graphWidth;
	m_graphSurface.h = m_fullSurface->h - (CUSHION * 2);

	std::cout << "m_graphSurface.x = " << m_graphSurface.x << std::endl;
	std::cout << "m_graphSurface.y = " << m_graphSurface.y << std::endl;
	std::cout << "m_graphSurface.w = " << m_graphSurface.w << std::endl;
	std::cout << "m_graphSurface.h = " << m_graphSurface.h << std::endl;

	initBars(barWidth, stride);

	return true;
}

void barGraph::initBars(int barW, int stride)
{
	int x = m_graphSurface.x;

	for (auto &b : graph) {
		b.setX(x);
		// start at bottom and draw up with negative height
		b.setY(m_graphSurface.y + m_graphSurface.h);
		b.setW(barW);

		x += stride;
	}
}

bool barGraph::clear(SDL_Renderer *renderer)
{
	changeColor(renderer, CLEAR);
	if(0 > SDL_RenderFillRect(renderer, &m_graphSurface)) {
		SDL_Log("SDL_RenderDrawRect: error = %s", SDL_GetError());
		return false;
	}
	return true;
}

bool barGraph::draw(SDL_Renderer *renderer, const std::vector<int>& data)
{
	if (graph.size() != data.size()) {
		return false;
	}

	clear(renderer);

	std::vector<int>::const_iterator val = data.begin();
	for (auto &b : graph) {
		// work with the negative so we don't have to calculate where the top is
		int height = -(*val * m_resolution);
		b.setH(height);
		b.draw(renderer);
		val++;
	}

	return true;
}
