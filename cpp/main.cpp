#include "insertionCreator.h"
#include "visualSorter.h"

#include <iostream>
#include <memory>
#include <vector>

int initAlgorithms(visualSorter &sorter, std::vector<int> &initialData) {
	insertionCreator insCreator;

	sorter.addAlgorithm(insCreator, initialData);

	return 0;
}

int main(int argc, char* argv[]) {
	visualSorter sorter {};
	std::cout << "Hello world" << std::endl;

	//init generators
	std::vector<int> initialData = {2,3,4,5};

	//init algorithms
	initAlgorithms(sorter, initialData);

	//init window

	//while (!isAllComplete()) {
		// doStep()

		// showdata()
	//}

	//showStatistics();
	sorter.isAllComplete();
	sorter.debugPrintData();
	return 0;
}

