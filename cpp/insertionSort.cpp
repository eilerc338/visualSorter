#include "insertionSort.h"
#include <iostream>

insertionSort::insertionSort(std::vector<int> initialData) {
	std::cout << "insertionSort Created" << std::endl;
}

void insertionSort::doNextStep() {
	std::cout << "insertionSort::doNextStep())" << std::endl;
}

const std::vector<int>& insertionSort::getData() {
	std::cout << "insertionSort::getData()" << std::endl;
	return data;
}

int insertionSort::getNumSteps() {
	std::cout << "insertionSort::getNumSteps()" << std::endl;
	return 0;
}

bool insertionSort::isComplete() {
	std::cout << "insertionSort::isComplete()" << std::endl;
	return 0;
}
