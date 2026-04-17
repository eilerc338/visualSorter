#pragma once

struct rectangle {
	unsigned int xOrigin;
	unsigned int yOrigin;
	unsigned int width;
	unsigned int height;
};

class windowManager {
	public:
		int init (unsigned int width, unsigned int height);
		int createTiles(unsigned int num);
		// this returns the drawable area of the tile
		struct rectangle getDrawArea(int index);
		std::vector<struct rectangle> getBorders();

	private:
		unsigned int windowWidth;
		unsigned int windowHeight;
		std::vector<std::vector<struct>> tile> tiles;
};

// gonna know how to draw the bars on the tile
class draw {
	public:
		int Rectangle(std::vector<struct rectangle> rectangles);
		int barGraph(struct rectangle drawArea, std::vector<int> vals);
};
