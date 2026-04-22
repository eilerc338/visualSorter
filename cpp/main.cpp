#include "algorithmCreator.h"
#include "visualSorter.h"

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
	visualSorter vs;
	bool exit = false;

	std::cout << "Visual Sorter" << std::endl;

	//init generators
	std::vector<int> initialData = {9,8,7,5};

	//init algorithms
	vs.initAlgorithms(initialData);

	//init window
	if (!vs.initWindow()) {
		goto FAIL;
	}

	vs.getSurfaces();
	vs.drawBorders();
	vs.drawGraphs();
	vs.updateScreen();

	while (!exit)
	{
		SDL_Event e;

		while(SDL_PollEvent(&e) != 0) {
			//user requests to quit
			if(e.type == SDL_QUIT) {
				exit = true;
			}
		}

		vs.doStep();
		vs.waitStepDone();
		vs.debugPrintData();
		vs.drawGraphs();
		vs.updateScreen();
	}


	//std::cout << "Press any key" << std::endl;
	//std::cin.ignore();

	//showStatistics();
	//vs.isAllComplete();
	//vs.debugPrintData();
	return 0;

FAIL:
	return 1;
}

