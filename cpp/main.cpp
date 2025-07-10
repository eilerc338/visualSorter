#include "algorithmCreator.h"
#include "visualSorter.h"

#include <iostream>
#include <memory>
#include <vector>

int initAlgorithms(visualSorter &sorter, std::vector<int> &initialData) {
	insertionCreator insCreator;
	mergeCreator mrgCreator;

	sorter.addAlgorithm(insCreator, initialData);
	sorter.addAlgorithm(mrgCreator, initialData);

	return 0;
}

int main(int argc, char* argv[]) {
	visualSorter sorter;
	std::cout << "Visual Sorter" << std::endl;

	//init generators
	std::vector<int> initialData = {9,8,7,5};

	//init algorithms
	initAlgorithms(sorter, initialData);

	//init window

	while (!sorter.isAllComplete()) {
		// for now do one step per pressing enter
		std::cin.ignore();
		sorter.doStep();

		sorter.debugPrintData();
		//showdata()
	}

	//showStatistics();
	//sorter.isAllComplete();
	//sorter.debugPrintData();
	return 0;
}

