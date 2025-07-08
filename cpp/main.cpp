#include "insertionCreator.h"

#include <iostream>
#include <memory>
#include <vector>


bool isAllComplete() {
	return true;
}

int main(int argc, char* argv[]) {
	std::unique_ptr<insertionCreator> insCreator = std::make_unique<insertionCreator>();
	std::cout << "Hello world" << std::endl;

	//int generators
	std::vector<int> initialData = {2,3,4,5};

	//init algorithms
	insCreator->createAlgorithm(initialData);

	//init window

	while (!isAllComplete()) {
		// doStep()

		// showdata()
	}

	//showStatistics();

	return 0;
}

