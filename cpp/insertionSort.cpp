#include "insertionSort.h"
#include <iostream>

insertionSort::insertionSort(std::vector<int> initialData) {
	std::cout << "insertionSort Created" << std::endl;
	data = initialData;
}

void insertionSort::doNextStep() {
	std::cout << "insertionSort::doNextStep())" << std::endl;
}

int insertionSort::getNumSteps() {
	std::cout << "insertionSort::getNumSteps()" << std::endl;
	return 0;
}

bool insertionSort::isComplete() {
	std::cout << "insertionSort::isComplete()" << std::endl;
	return 0;
}
