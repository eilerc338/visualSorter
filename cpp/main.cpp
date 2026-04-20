#include "algorithmCreator.h"
#include "visualSorter.h"

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
	visualSorter vs;

	std::cout << "Visual Sorter" << std::endl;

	//init generators
	std::vector<int> initialData = {9,8,7,5};

	//init algorithms
	vs.initAlgorithms(initialData);

	//init window
#if 1
	if (!vs.initWindow()) {
		goto FAIL;
	}
#endif

	vs.drawBorders();
    //Update Screen
	//vs.updateScreen();

	vs.getSurfaces();

	//while (!vs.isAllComplete()) {
	while (vs.mainLoop())
	{
		// for now do one step per pressing enter
		//std::cin.ignore();
		//vs.drawBorders();
		//vs.doStep();

		//vs.debugPrintData();
		//showdata()
	}

	std::cout << "1" << std::endl;
	//std::cin.ignore();
	std::cout << "2" << std::endl;

	//showStatistics();
	//vs.isAllComplete();
	//vs.debugPrintData();
	return 0;

FAIL:
	return 1;
}

