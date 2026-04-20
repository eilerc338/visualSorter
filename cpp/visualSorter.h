#pragma once

#include "algorithmCreator.h"
#include "windowManager.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

class visualSorter {
	public:
		int initAlgorithms(std::vector<int> &initialData);
		int addAlgorithm(algorithmCreator &algCreator, std::vector<int> &initialData);
		bool initSurfaces();
		unsigned int getNumAlgorithms();
		bool isAllComplete();
		void debugPrintData();
		void doStep();
		void drawBorders();
		bool initWindow();

	private:
		std::vector<std::unique_ptr<algorithm>> algorithms {};
		windowManager winMgr{SCREEN_WIDTH, SCREEN_HEIGHT};
};
