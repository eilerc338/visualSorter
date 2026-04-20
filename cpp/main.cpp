#include "algorithmCreator.h"
#include "visualSorter.h"

#include "SDL.h"
#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char* argv[]) {
	visualSorter vs;

	std::cout << "Visual Sorter" << std::endl;

	//init generators
	std::vector<int> initialData = {9,8,7,5};

	//init algorithms
	vs.initAlgorithms(initialData);

	//init window
	if (!vs.initWindow()) {
		goto FAIL;
	}

	vs.initSurfaces();

	while (!vs.isAllComplete()) {
		// for now do one step per pressing enter
		//std::cin.ignore();
		vs.drawBorders();
		//vs.doStep();

		//vs.debugPrintData();
		//showdata()
	}
	std::cin.ignore();

	//showStatistics();
	//vs.isAllComplete();
	//vs.debugPrintData();
	return 0;

FAIL:
	return 1;
}

