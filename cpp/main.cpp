#include "algorithmCreator.h"
#include "visualSorter.h"

#include <iostream>
#include <vector>

#define DATA_SIZE	120

std::vector<int> generateData_descending()
{
	std::vector<int> data(DATA_SIZE);

	for (int i = 0; i < DATA_SIZE; ++i) {
		data[i] = DATA_SIZE - i;
	}

	return data;
}

int main(int argc, char* argv[]) {
	visualSorter vs;
	bool close = false;

	std::cout << "Visual Sorter" << std::endl;

	std::vector<int> initialData = generateData_descending();
	//std::vector<int> initialData = generateData_random();

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
#if 1
	while (!close)
	{
		SDL_Event e;

		while(SDL_PollEvent(&e) != 0) {
			//user requests to quit
			if(e.type == SDL_QUIT) {
				close = true;
			}
		}

		vs.doStep();
		vs.waitStepDone();
		//vs.debugPrintData();
		vs.drawGraphs();
		vs.updateScreen();
	}
#endif


	//std::cout << "Press any key" << std::endl;
	//std::cin.ignore();

	//vs.showStatistics();
	//vs.isAllComplete();
	//vs.debugPrintData();
	return 0;

FAIL:
	return 1;
}

